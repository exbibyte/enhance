#include "DataTransformDriver.h"
#include "DataTransformPass.h"
#include "DataTransformMetaInfo.h"

#include <vector>
#include <algorithm>

using namespace std;

DataTransformDriver::DataTransformDriver(){
}
bool DataTransformDriver::RegisterPass( DataTransformPass * pass ){
    if( !pass ){
	return false;
    }
    _Passes.push_back( pass );
    return true;
}

bool DataTransformDriver::ExecutePasses( void * & data_in, void * & data_out ){
    bool bRet = true;
    void * data_temporary;
    for( auto & i : _Passes ){
	DataTransformMetaInfo * meta_info;
	if( !i->GetDataTransformMetaInfo( meta_info ) ){
	    bRet = false;
	    return bRet;
	}
	else
	{
	    bRet = i->ExecutePass( data_in, data_out );
	    if( !bRet ){
		return bRet;
	    }
	}
	data_temporary = data_in;
	data_in = data_out;
    }
    return bRet;
}
