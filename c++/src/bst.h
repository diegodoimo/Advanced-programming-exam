#include<utility>
#include<memory>
#include<vector>

#ifndef BST_GR
#define BST_GR

template <class key, class T>
class Map {
    using value_type = std::pair<key,T>;

    public:
    //copy constructor
    Map(const Map& m)
    {
        if (m.head==nullptr) return;
        head.reset(new Node{m.head->val, nullptr, nullptr, nullptr});
        copy_tree(head, m.head);
    }
    
    //copy assigment
    Map& operator=(const Map& m){
        head.reset();
        auto tmp = m;
        *this = std::move(tmp);
        return *this;
    }
    
    //move constructor
    Map(Map&& m) noexcept: head{std::move(m.head)} {}

    //move assignment
    Map& operator=(Map&& m) noexcept {
        head = std::move(m.head);
        return *this;
    }

    Map() = default;
    ~Map() = default;
    
    class Iterator;
    class ConstIterator;

    Iterator begin();
    Iterator end() { return Iterator{nullptr}; }
    ConstIterator begin() const;
    ConstIterator end() const { return ConstIterator{nullptr}; }

    void insert(const value_type&);
    Iterator find(const key&);    
    void  balance();
    void clear();
    
    template <class ok, class ot>
    friend std::ostream& operator<<(std::ostream&, const Map<ok, ot>&);
    
    private:

    struct Node {
        value_type val;
        Node* up;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node>right;
        Node(const value_type& v, Node* u, Node* l, Node* r) : val{v}, up{u}, left{l}, right{r} {}
        ~Node() = default;
    };

    std::unique_ptr<Node> head;
    
    void add_node(const value_type& v, std::unique_ptr<Node>& current, Node* head);
    void build_map(std::unique_ptr<Node>& pn, Node*  head,  std::vector<value_type>& v, int start,int stop);
    void copy_tree(std::unique_ptr<Node>& head, const std::unique_ptr<Node>& p);
    Iterator search(Node* curr, key k);
};


template <class key, class T>
class Map<key, T>::Iterator {
    using Node = Map<key, T>::Node;
    using value_type = Map<key, T>::value_type;

    public:

    Iterator(Node* n) : current{n} {}
    
    //copy constructor
    Iterator(const Iterator& it)
    {
        if (it.current==nullptr) return;
        current = it.current; 
    }

    //copy assigment
    Iterator& operator=(const Iterator& it){
        current = nullptr;
        auto tmp = it;
        *this = std::move(tmp);
        return *this;
    }
 
    //move assignment
    Iterator& operator=(Iterator&& it) noexcept {
        current = std::move(it.current);
        return *this;
    }
    
    //move constructor
    Iterator(Iterator&& it) noexcept: current{std::move(it.current)} {}
    
    Iterator () = default;
    ~Iterator()=default;

    value_type& operator*() const { return current->val; }
    Iterator& operator++();

    Iterator operator++(int){
        Iterator it{*this};
        ++(*this);
        return it;
    }

    bool operator==(const Iterator& other) { return current == other.current; }
    bool operator!=(const Iterator& other) { return !(*this == other); }

    private:
    Node* current;
};

#endif
