template< typename T >
bool Filter0<T>::init(){
    return true;
}
template< typename T >
bool Filter0<T>::deinit(){
    return true;
}
template< typename T >
bool Filter0<T>::process( std::list<T> & out, std::list<T> & in ){
    return true;
}
