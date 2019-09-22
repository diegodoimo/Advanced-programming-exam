#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
#include <map>
#include "bst.h"    

/////////////////////////////////////////////////////////////////////////////////////

/*
 */
template<class key, class T>
typename Map<key, T>::Iterator& Map<key, T>::Iterator::operator++()
{
    if (current->right != nullptr) {
        current = current->right.get();
        while(current->left!=nullptr) {
            current=current->left.get();
        }
        return *this;
    }
    else{
        Node* temp_root= current->up;
        while (temp_root != nullptr && current == temp_root->right.get()) {
            current = temp_root;
            temp_root=current->up;
        }
        current=temp_root;
        return *this;
    }
}

template <class key, class T>
class Map<key, T>::ConstIterator : public Map<key, T>::Iterator {
 public:
  using parent = Map<key, T>::Iterator;
  using parent::Iterator;
  const value_type& operator*() const { return parent::operator*(); }
  using parent::operator==;
  using parent::operator!=;
};

template<class key, class T>
typename Map<key,T>::Iterator Map<key,T>::begin(){
    Node* tmp=head.get();
    while(tmp->left!=nullptr){
        tmp = tmp->left.get();
    }
    return tmp;
}


template<class key, class T>
typename Map<key,T>::ConstIterator Map<key,T>::begin() const{
    Node* tmp=head.get();
    while(tmp->left!=nullptr){
        tmp = tmp->left.get();
    }
    return tmp;
}


template <class key, class T>
void Map<key, T>::insert(const value_type& value) {
        
    add_node(value, head, nullptr);
}

template <class key, class T>
void Map<key, T>::add_node(const value_type& v, std::unique_ptr<Node>& current, Node* head){
        
    if(current == nullptr){
            current.reset(new Node{v, head, nullptr, nullptr});
            return;
        }
        
        if(current->val.first < v.first) {
            add_node(v, current->right, current.get());
        } 
        else{
            add_node(v, current->left, current.get());
        }
        return;
    }


template <class key, class T>
typename Map<key, T>::Iterator  Map<key, T>::find(const key& k){    
    return search(head.get(), k);
}

template <class key, class T>
typename Map<key, T>::Iterator Map<key, T>::search(Node* curr, key k) 
{  
    if (curr == nullptr || curr->val.first == k) 
       return Iterator{curr}; 
      
    if (curr->val.first < k) 
       return search(curr->right.get(), k); 
  
    else
        return search(curr->left.get(), k); 
} 

template<class key, class T>
void Map<key, T>::copy_tree(std::unique_ptr<Node>& head, const std::unique_ptr<Node>& p){
    if (p->left!=nullptr){
        head->left.reset(new Node{p->left->val, head.get(), nullptr, nullptr});
        copy_tree(head->left, p->left);
    }

    if (p->right!=nullptr){
        head->right.reset(new Node{p->right->val, head.get(), nullptr, nullptr});
        copy_tree(head->right, p->right);
    }
    return;
}

template<class key, class T>
void Map<key,T>::clear()
{
    head.reset();
}

template<class key, class T>
void Map<key,T>::balance()
{
    std::vector<value_type> sorted_nodes;
    Iterator it = begin();
    for(; it!=end(); it++){
        sorted_nodes.push_back((*it));
    }
    

    Map<key, T> tmp;
    build_map(tmp.head, nullptr, sorted_nodes, 0, sorted_nodes.size()-1);
    *this = std::move(tmp);
    return;

}

template<class key, class T>
void Map<key,T>::build_map(std::unique_ptr<Node>& pn, Node* head, std::vector<value_type>& v, int start,int stop)
{
    if (start>stop) return;

    int m=int((start+stop)*0.5+0.1);
    pn.reset(new Node{v[m], head, nullptr, nullptr});
    build_map(pn->left, pn.get(),  v, start, m-1);
    build_map(pn->right, pn.get(), v, m+1, stop);
    return;
}

//operator <<
template <class key, class T>
std::ostream& operator<<(std::ostream& os, const Map<key, T>& m) {
    if (m.head==nullptr)
        os<<'\n';
    else{
  for (const auto& x : m)
    os << x.first << ": "<<x.second<<'\n';
    }
  return os;
}

