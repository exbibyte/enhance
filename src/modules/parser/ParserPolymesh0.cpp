#include "ParserPolymesh0.hpp"
#include "IParser.hpp"

#include "GLIncludes.hpp"

#include "PassParsePolyMesh.h"
#include "PassConvertPolyMeshDataStructToArray.h"
#include "DataTransformDriver.h"
#include "PolyMesh_Data_Arrays.h"

#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <iostream>

using namespace std;

bool ParserPolymesh0::parse( string path, map< string, GLBufferInfo * > & map_buffer_info, map< string, GLBufferInfoSequence * > & map_buffer_info_sequence, vector<double> & vert_pos, vector<double> & vert_norm ){

    //parse input polymesh file
    PassParsePolyMesh pass_parse_polymesh;
    PassParsePolyMesh * p_pass_parse_polymesh = & pass_parse_polymesh;

    PassConvertPolyMeshDataStructToArray pass_convert_polymesh_to_array;
    PassConvertPolyMeshDataStructToArray * p_pass_convert_polymesh_to_array = & pass_convert_polymesh_to_array;
    
    DataTransformDriver data_transform_driver;

    //set meta info for parsing polymesh
    DataTransformMetaInfo meta_info_parse_polymesh("parse_polymesh");
    // string strFilePathPolyMesh = "../../testcase/file/BS_Skate_v2.pmesh";
    string strFilePathPolyMesh = path;
    meta_info_parse_polymesh.AddMetaInfo( "INPUT_DATATYPE", "FILE_POLYMESH" );
    meta_info_parse_polymesh.AddMetaInfo( "INPUT_DATAPATH", strFilePathPolyMesh );
    meta_info_parse_polymesh.AddMetaInfo( "OUTPUT_DATATYPE", "DATASTRUCT_POLYMESH");
    if( !pass_parse_polymesh.RegisterDataTransformMetaInfo( & meta_info_parse_polymesh ) ){
        assert( 0 && "PassParsePolyMesh::RegisterDataTransformMetaInfo failed" );
    }

    //set meta info for converting polymesh to array
    DataTransformMetaInfo meta_info_convert_polymesh_to_array("convert_polymesh_to_array");
    meta_info_convert_polymesh_to_array.AddMetaInfo( "INPUT_DATATYPE", "DATASTRUCT_POLYMESH" );
    meta_info_convert_polymesh_to_array.AddMetaInfo( "OUTPUT_DATATYPE", "DATASTRUCT_POLYMESH_ARRAY");
    if( !pass_convert_polymesh_to_array.RegisterDataTransformMetaInfo( & meta_info_convert_polymesh_to_array ) ){
        assert( 0 && "PassParsePolyMesh::RegisterDataTransformMetaInfo failed" );
    }    

    //register passes
    if( !data_transform_driver.RegisterPass( p_pass_parse_polymesh ) ){
        assert( 0 && "DataTransformDriver::RegisterPass failed.");
    }
    if( !data_transform_driver.RegisterPass( p_pass_convert_polymesh_to_array ) ){
        assert( 0 && "DataTransformDriver::RegisterPass failed.");
    }

    void * data_in;
    void * data_out;
    if( !data_transform_driver.ExecutePasses( data_in, data_out ) ){
        assert( 0 && "DataTransformDriver::ExecutePasses failed.");
    }

    PolyMesh_Data_Arrays * polymesh_data_arrays = ( PolyMesh_Data_Arrays * ) data_out;

    float * data_vertex;
    float * data_normal;
    int iNumDataVertex;
    int iNumDataNormal;
    if( !polymesh_data_arrays->Get( PolyMesh_Data_Arrays_Type::VERTEX, data_vertex, iNumDataVertex ) ){
        assert( 0 && "PolyMesh_Data_Arrays::Get failed.");
    }
    if( !polymesh_data_arrays->Get( PolyMesh_Data_Arrays_Type::NORMAL, data_normal, iNumDataNormal ) ){
        assert( 0 && "PolyMesh_Data_Arrays::Get failed.");
    }

    polymesh_data_arrays->GetMapBufferInfo( map_buffer_info );
    polymesh_data_arrays->GetMapBufferInfoSequence( map_buffer_info_sequence );

    if( !data_transform_driver.CleanUpPasses() ){
        assert( 0 && "DataTransformDriver::CleanUpPasses failed.");
    }

    for( int i = 0; i < iNumDataVertex; ++i ){
    	vert_pos.push_back( data_vertex[i] );
    }
    for( int i = 0; i < iNumDataNormal; ++i ){
    	vert_norm.push_back( data_normal[i] );
    }
    cout << "Size of vert_pos: " << vert_pos.size() << endl;
    cout << "Size of vert_norm: " << vert_norm.size() << endl;

    delete [] data_vertex;
    data_vertex = nullptr;
    delete [] data_normal;
    data_normal = nullptr;
    
    return true;
}
