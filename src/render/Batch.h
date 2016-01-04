#ifndef BATCH_H
#define BATCH_H

#include <vector>
#include <map>

template< typename T >
class Batch {
public:
    bool AddToBatch( std::vector< T > vec_data ){
	_batch.insert( _batch.end(), vec_data.begin(), vec_data.end() );
	return true;
    }
    bool AddToBatch( T data ){
	_batch.push_back( data );
	return true;
    }
    bool GetBatch( std::vector< T > & vec_data ){
	vec_data = _batch;
	return true;
    }
    bool EmptyBatch(){
	_batch.clear();
	return true;
    }
    bool GetSizeBatch( unsigned int & size ){
	size = _batch.size();
	return true;
    }
private:
    std::vector< T > _batch;
};

template< typename T, unsigned int N > 
class BatchsWapper {
public:
    BatchsWapper(){

    }
    Batch< T > * GetCurrentBatch(){
	return &_batches[ _index ];
    }
    bool IncrementBatch(){
	unsigned int count = 0;
	while( count < N ){
	    if( _index + 1 < N ){
		++_index;
	    }else{
		_index = 0;
	    }
	    bool bIsSealed;
	    IsBatchSealed( _batches[ _index ], bIsSealed );
	    if( !bIsSealed ){
		return true;
	    }else{
		++count;
	    }
	}
	return false;
    }
    bool SignalBatchSealedAndIncrement( Batch< T > * batch ){
	auto it = _batch_sealed.find( batch );
	if( it != _batch_sealed.end() ){
	    it->second = true;
	    IncrementBatch();
	    return true;
	}else{
	    return false;
	}
    }
    bool SignalBatchFree( Batch< T > * batch ){
	auto it = _batch_sealed.find( batch );
	if( it != _batch_sealed.end() ){
	    it->second = false;
	    return true;
	}else{
	    return false;
	}
    }
    bool IsBatchSealed( Batch< T > * batch, bool & bIsSealed ){
	auto it = _batch_sealed.find( batch );
	if( it != _batch_sealed.end() ){
	    bIsSealed = it->second;
	    return true;
	}else{
	    return false;
	}
    }
private:
    unsigned int _index;
    Batch< T >  _batches[N];
    std::map< Batch< T > *, bool > _batch_sealed;
};

#endif
