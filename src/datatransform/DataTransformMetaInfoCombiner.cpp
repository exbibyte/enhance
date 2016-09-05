#include "DataTransformMetaInfoCombiner.h"
#include "DataTransformMetaInfo.h"

#include <utility>
#include <vector>
#include <iterator>
#include <algorithm>
using namespace std;

bool DataTransformMetaInfoCombiner::BuildAggregate( vector< DataTransformMetaInfo * > inputs, DataTransformMetaInfo * & aggregate ){
    if( !aggregate ){
	return false;
    }
    aggregate->RemoveMetaInfoAll(); //clear
    for( auto & i : inputs ){
	aggregate->AddMetaInfo( *i );
    }
    return true;
}
