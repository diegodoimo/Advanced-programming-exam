#include<utility>
#include<memory>
#include<vector>
#include<iostream>

#ifndef BST_GR
#define BST_GR

/*Map   <class key  //key type
 *      class T>     //mapped type
 *      associative container implemented as binary search tree;
 *      Member types:
 *          value_type      :pair <key_type, mapped_type>
 *          Node            :stores the value_types
 *          Iterator        :forward iterator to value_type
 *          ConstIterator   :forward iterator to const value_type
 *
 *      Member functions (public):
 *          constructor         :copy/move
 *          operator            :copy/move
 *          destructor
 *
 *          Iterators:
 *          begin               :return iterator to beginning
 *          end                 :return iterator to end 
 *          cbegin              :return ConstIterator to beginnin 
 *          cend                :return ConstIterator to end
 *
 *          Modifiers:
 *          insert              :insert elements
 *          clear               :clear content of the Map
 *          balance             :balance the Map
 *
 *          Operations:
 *          find                :get the Itarator to searched element
 *
 *
 */
template <class key, class T>
class Map {
    using value_type = std::pair<const key,T>;

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
    ConstIterator cbegin() const;
    ConstIterator cend() const { return ConstIterator{nullptr};}

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
        std::unique_ptr<Node> right;
        Node(const value_type& v, Node* u, Node* l, Node* r) : val{v}, up{u}, left{l}, right{r} {}
        
        Node()=default;
        ~Node() = default;
    };

    std::unique_ptr<Node> head;
    
    void add_node(const value_type& v, std::unique_ptr<Node>& current, Node* head);
    void build_map(std::unique_ptr<Node>& pn, Node* head, const std::vector<value_type>& v, int start,int stop);
    void copy_tree(std::unique_ptr<Node>& head, const std::unique_ptr<Node>& p);
    Iterator search(Node* curr, const key k);
};


/*Map< key,  T>::Iterator   
 *      Member functions (public):
 *          constructor
 *          destructor
            operator++      :return iterator to next node
            operator*       :return pair<key, value> pointed to
            operator==      
            operator!=

 */

template <class key, class T>
class Map<key, T>::Iterator {
    using Node = Map<key, T>::Node;
    using value_type = Map<key, T>::value_type;

    public:

    Iterator(Node* n) : current{n} {}    
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


/*Operator++
 * current->right* > current
 * current->left* < current:
 *      if current->right!=nullptr return the smallest value in the subtree with head current->right
 *      if current->right==nullptr go up until parent == nullptr or current is left-child of parent (i.e. <parent)
 */
template<class key, class T>
typename Map<key, T>::Iterator& Map<key, T>::Iterator::operator++()
{   
    if (current->right) {
        current = current->right.get();
        while(current->left) {
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

/*begin(): returns the leftmost child i.e. the smaller
 *
 */
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


template<class key, class T>
typename Map<key,T>::ConstIterator Map<key,T>::cbegin() const{
    Node* tmp=head.get();
    while(tmp->left!=nullptr){
        tmp = tmp->left.get();
    }
    return tmp;
}

/* copy_tree(replica, current)     :private function - helper of copy consturctor
 * recursively copies current node into replica
 *
 * replica      :unique_prt to replica of current 
 * current      :unique_ptr to node to be copied into replica
 * 
 */


template<class key, class T>
void Map<key, T>::copy_tree(std::unique_ptr<Node>& replica, const std::unique_ptr<Node>& current){

    if (current->left!=nullptr){
        replica->left.reset(new Node{current->left->val, replica.get(), nullptr, nullptr});
        copy_tree(replica->left, current->left);
    }

    if (current->right!=nullptr){
        replica->right.reset(new Node{current->right->val, replica.get(), nullptr, nullptr});
        copy_tree(replica->right, current->right);
    }
    return;
}


/*insert pair<key, value> with the aid of the recursive helper function add_node
 */
template <class key, class T>
void Map<key, T>::insert(const value_type& value) {        
    add_node(value, head, nullptr);
}

/*add_node(v, current, head):
 *  insert v in the Map recursively: if v is already present it replace the oldvalue with v;
 *  v           :pair<key, value> to be inserted
 *  current     :unique_ptr to current node 
 *  head        :ptr to the parent of the current node
 *
 *  if the key of v < current key, the insertion prceeds using current->left as current and current as parent;
 *
 */
template <class key, class T>
void Map<key, T>::add_node(const value_type& v, std::unique_ptr<Node>& current, Node* head){
        
    if(current == nullptr){
        current.reset(new Node{v, head, nullptr, nullptr});
        return;
    }
        
    else if(current->val.first < v.first) {
        add_node(v, current->right, current.get());
    }
    else{
        add_node(v, current->left, current.get());
    }
    return;
}

/*find(k):
 * returns the iterator to the searched element; nullptr in case there is no key k in the tree.
 *
 * k        :key to be looked for
 */
template <class key, class T>
typename Map<key, T>::Iterator  Map<key, T>::find(const key& k){    
    return search(head.get(), k);
}

/*search(curr, k)
 * curr     :pointer to current node;
 * k        :key to be looked for in the subtree that has current as root
 */
template <class key, class T>
typename Map<key, T>::Iterator Map< key, T>::search(Node* curr, const key k) 
{  
    if (k == curr->val.first || nullptr == curr) 
       return Iterator{curr}; 
      
    if (k > curr->val.first) 
       return search(curr->right.get(), k); 
  
    else
        return search(curr->left.get(), k); 
} 

/*clear():
 *empties the binary search tree
 */

template<class key, class T>
void Map<key,T>::clear()
{
    head.reset();
}


/*balance():
 * balances the binary search tree
 */
template<class key, class T>
void Map<key,T>::balance()
{
    std::vector<value_type> sorted_nodes;   //contains the all the value_types in increasing order of keys.
    Iterator it = begin();
    for(; it!=end(); it++){
        sorted_nodes.push_back((*it));
    }
    

    Map<key, T> tmp;                                                        //temporary balanced map 
    build_map(tmp.head, nullptr,  sorted_nodes, 0, sorted_nodes.size()-1);  //builds recursively tmp
    *this = std::move(tmp);
    return;

}

/*
 * build_map(current, head, v, start, stop):
 * builds a balanced map with current as root
 *
 * current      :unique_ptr to the current node;
 * head         :ptr to the parent of current
 * v            :vector with all the key node pairs sorted by key
 * start(stop)  :v[start]->v[stop] is the subset of v elements to be added under current
 */
template<class key, class T>
void Map<key,T>::build_map(std::unique_ptr<Node>& current, Node* head, const std::vector<value_type>& v, int start,int stop)
{
    if (start>stop) return;     

    int m=int((start+stop)*0.5+0.1);                        //m = half vector index v[m].first is the median of the keys
    current.reset(new Node{v[m], head, nullptr, nullptr});  //to the current node is associated v[m] {key, value}
    build_map(current->left, current.get(), v, start, m-1); //balanced map is build on the left of current with the first half of v
    build_map(current->right, current.get(), v, m+1, stop); //balanced map is build on the right of current with the second half of v
    return;
}

/* operator <<:
 * if map is empty  it outputs '\n'
 * else             'key: value' with keys in increasing order
 */
template <class key, class T>
std::ostream& operator<<(std::ostream& os, const Map<key, T>& m) {
    if (m.head==nullptr) 
        std::cout<<'\n';
    else{
    for (const auto& x : m)
        os << x.first << ": "<<x.second<<'\n';
    }
  return os;
}


#endif
