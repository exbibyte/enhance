template< class T >
list_lockfree_total_impl::list_lockfree_total_impl(){
    Node * h = new Node;
    h->_is_head = true;
    _head.store( h, std::memory_order_release );
}
template< class T >
list_lockfree_total_impl::~list_lockfree_total_impl(){
    clear();
}
template< class T >
bool list_lockfree_total_impl::clear(){
    //todo
    return false;
}
template< class T >
bool list_lockfree_total_impl::empty(){
    return size() == 0;
}
template< class T >
size_t list_lockfree_total_impl::size(){
    //todo
    return 0;
}
template< class T >
bool list_lockfree_total_impl::add( T const & val, size_t key ){
    //todo
    return false;
}
template< class T >
bool list_lockfree_total_impl::remove( T & val, size_t key ){
    //todo
    return false;
}
template< class T >
bool list_lockfree_total_impl::contains( T const & val, size_t key ){
    //todo
    return false;
}
template< class T >
find_result list_lockfree_total_impl::find( _t_node & head, size_t key ){
    //todo
    return false;
}
