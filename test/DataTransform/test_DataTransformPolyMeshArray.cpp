#include "DataTransformDriver.h"
#include "PassParsePolyMesh.h"
#include "PassConvertPolyMeshDataStructToArray.h"
#include "DataTransformPass.h"
#include "DataTransformMetaInfo.h"
#include "Filter_ParsePolyMesh.h"
#include "PolyMesh_Data_Arrays.h"

#include <iostream>
#include <string>
#include <cassert>

using namespace std;

int main( int argc, char ** argv ){
    if( argc < 2 ){
	assert( 0 && "Require argument: path_to_polymesh_file" );
	return -1;
    }
    char * file_path_polymesh = argv[1];
    
    PassParsePolyMesh pass_parse_polymesh;
    PassParsePolyMesh * p_pass_parse_polymesh = & pass_parse_polymesh;

    PassConvertPolyMeshDataStructToArray pass_convert_polymesh_to_array;
    PassConvertPolyMeshDataStructToArray * p_pass_convert_polymesh_to_array = & pass_convert_polymesh_to_array;
    
    DataTransformDriver data_transform_driver;

    //set meta info for parsing polymesh
    DataTransformMetaInfo meta_info_parse_polymesh("parse_polymesh");
    string strFilePathPolyMesh = string( file_path_polymesh );
    meta_info_parse_polymesh.AddMetaInfo( "INPUT_DATATYPE", "FILE_POLYMESH" );
    meta_info_parse_polymesh.AddMetaInfo( "INPUT_DATAPATH", strFilePathPolyMesh );
    meta_info_parse_polymesh.AddMetaInfo( "OUTPUT_DATATYPE", "DATASTRUCT_POLYMESH");
    if( !pass_parse_polymesh.RegisterDataTransformMetaInfo( & meta_info_parse_polymesh ) ){
	assert( 0 && "PassParsePolyMesh::RegisterDataTransformMetaInfo failed" );
	return -1;
    }

    //set meta info for converting polymesh to array
    DataTransformMetaInfo meta_info_convert_polymesh_to_array("convert_polymesh_to_array");
    meta_info_convert_polymesh_to_array.AddMetaInfo( "INPUT_DATATYPE", "DATASTRUCT_POLYMESH" );
    meta_info_convert_polymesh_to_array.AddMetaInfo( "OUTPUT_DATATYPE", "DATASTRUCT_POLYMESH_ARRAY");
    if( !pass_convert_polymesh_to_array.RegisterDataTransformMetaInfo( & meta_info_convert_polymesh_to_array ) ){
	assert( 0 && "PassParsePolyMesh::RegisterDataTransformMetaInfo failed" );
	return -1;
    }    

    //register passes
    if( !data_transform_driver.RegisterPass( p_pass_parse_polymesh ) ){
	assert( 0 && "DataTransformDriver::RegisterPass failed.");
	return -1;
    }
    if( !data_transform_driver.RegisterPass( p_pass_convert_polymesh_to_array ) ){
	assert( 0 && "DataTransformDriver::RegisterPass failed.");
	return -1;
    }

    void * data_in;
    void * data_out;
    if( !data_transform_driver.ExecutePasses( data_in, data_out ) ){
	assert( 0 && "DataTransformDriver::ExecutePasses failed.");
	return -1;
    }

    PolyMesh_Data_Arrays * polymesh_data_arrays = ( PolyMesh_Data_Arrays * ) data_out;

    float * data_vertex;
    float * data_normal;
    int iNumDataVertex;
    int iNumDataNormal;
    if( !polymesh_data_arrays->Get( PolyMesh_Data_Arrays_Type::VERTEX, data_vertex, iNumDataVertex ) ){
	assert( 0 && "PolyMesh_Data_Arrays::Get failed.");
	return -1;
    }
    if( !polymesh_data_arrays->Get( PolyMesh_Data_Arrays_Type::NORMAL, data_normal, iNumDataNormal ) ){
	assert( 0 && "PolyMesh_Data_Arrays::Get failed.");
	return -1;
    }

    if( !data_transform_driver.CleanUpPasses() ){
	assert( 0 && "DataTransformDriver::CleanUpPasses failed.");
	return -1;
    }
    
    float * current_vertex = data_vertex;
    float * current_normal = data_normal;
    cout << "vertex: " << endl;
    for( int i = 0; i < iNumDataVertex; i++ ){
	cout << *current_vertex++ << " ";
    }
    cout << endl;
    cout << "normal: " << endl;
    for( int i = 0; i < iNumDataNormal; i++ ){
	cout << *current_normal++ << " ";
    }
    cout << endl;

    delete [] data_vertex;
    data_vertex = nullptr;
    delete [] data_normal;
    data_normal = nullptr;

    delete polymesh_data_arrays;
    polymesh_data_arrays = nullptr;

    return 0;
}
