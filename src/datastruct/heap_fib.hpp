#ifndef HEAP_FIBONACCI_H
#define HEAP_FIBONACCI_H

#include <memory>

template< class TypeItem >
class HeapFibonacci {
public:
    class Node {
    public:
        std::shared_ptr< Node > _parent;
        std::shared_ptr< Node > _child;
        std::shared_ptr< Node > _left;
        std::shared_ptr< Node > _right;
        std::shared_ptr< TypeItem > _item;
    };
    //mergeable heap interfaces
    static std::shared_ptr< HeapFibonacci > MakeHeap(){
        std::shared_ptr< HeapFibonacci > heap = std::make_shared< HeapFibonacci >();
        return heap;
    }
    static std::shared_ptr< HeapFibonacci > Union( HeapFibonacci a, HeapFibonacci b ){
        std::shared_ptr< HeapFibonacci > heap = std::make_shared< HeapFibonacci >();
        //TODO
        return heap;
    }
    bool Insert( std::shared_ptr< TypeItem > item ){
        //TODO
        return false;
    }
    bool Min( std::shared_ptr< TypeItem > & item ){
        //TODO
        return false;
    }
    bool ExtractMin( std::shared_ptr< TypeItem > & item ){
        //TODO
        return false;
    }
    bool DecreaseKey( std::shared_ptr< TypeItem > item, int new_key ){
        //TODO
        return false;
    }
    bool Delete( std::shared_ptr< TypeItem > item ){
        //TODO
        return false;
    }
private:
    HeapFibonacci(){}
    int GetNumberOfTrees(){
        //TODO
        return 0;
    }
    int GetNumberOfMarkedNodes(){
        //TODO
        return 0;
    }
    int GetPotential(){
        return GetNumberOfTrees() + 2 * GetNumberOfMakedNodes();
    }
    int GetNumberOfNode(){
        //TODO
        return 0;
    }
};

#endif
