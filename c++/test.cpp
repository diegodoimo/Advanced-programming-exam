#include "bst_full.h"
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <map>
#include<iostream>
#include<fstream>

enum class method {mine, std};

void write_speed(int size, std::vector<double> speed){
    std::ofstream file;
    file.open("speed_unbalanced", std::ios_base::app);
    file<<size<<' ';
    for(auto i : speed){
        file<<i<<' ';
    }
    file<<'\n';
    file.close();
}

void write_speed(int size, double speed, method m){
    std::ofstream file;
    switch(m){
        case method::mine:
            file.open("speed_balanced", std::ios_base::app);
            file<<size<<' '<<speed<<'\n';
            file.close();
            break;
        case method::std:
            file.open("speed_std", std::ios_base::app);
            file<<size<<' '<<speed<<'\n';
            file.close();
            break;
        default:
            throw std::runtime_error{"unkown method\n"};
    };
}
           
double compute_speed(Map<int, double>& map, const std::vector<int>& keys)
{

        using namespace std::chrono;
        Map<int, double>::Iterator It;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        for(auto i : keys){
            It = map.find(i);
        }
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2-t1);
        return double(time_span.count());
}

double compute_speed(std::map<int, double>& m, const std::vector<int>& keys)
{
        using namespace std::chrono;
        std::map<int, double>::iterator st_it;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        for(auto i : keys){
        st_it = m.find(i);
        }
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2-t1);
        return double(time_span.count());
}



///////////////////////////////////////////////////////////////////////////
//
int main() {
    Map<int, double> map;

    //test find speed

    int seed = 3;
    std::mt19937 gen(seed); 
    std::uniform_int_distribution<> key(0, 10000000);
    std::uniform_real_distribution<> value(0,1);
    std::vector<int> keys{};
    std::map<int, double> stmap;
    
    for(int size=100; size<1000; size*=5){
        
        std::vector<double> vel_tmp;
        for(int rep = 0; rep<10; rep++){
            int size_tmp{1};
            map.clear();
            stmap.clear();
            keys.clear();
            while (size_tmp<size){
                int tmp_key = key(gen);
                std::vector<int>::iterator it;
                it = std::find(keys.begin(), keys.end(), tmp_key);
                if (it==keys.end()){
                    keys.push_back(tmp_key);
                    map.insert(std::make_pair(tmp_key, value(gen)));
                    stmap.insert(std::make_pair(tmp_key, value(gen)));
                    size_tmp+=1;
                }
            } 
            vel_tmp.push_back(compute_speed(map, keys));
        }

        map.balance();
        double v_my_map = compute_speed(map, keys);
        double v_std_map = compute_speed(stmap, keys);
        
        write_speed(size, vel_tmp);
        write_speed(size, v_my_map, method::mine);
        write_speed(size, v_std_map, method::std);
    }
}

