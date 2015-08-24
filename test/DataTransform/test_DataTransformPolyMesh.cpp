#include "DataTransformDriver.h"
#include "PassParsePolyMesh.h"
#include "DataTransformPass.h"
#include "DataTransformMetaInfo.h"

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
    DataTransformDriver data_transform_driver;
    DataTransformMetaInfo meta_info_input("sample_input");
    DataTransformMetaInfo meta_info_output("sample_output");

    string strFilePathPolyMesh = string( file_path_polymesh );
    meta_info_input.AddMetaInfo( "INPUT_DATATYPE", "FILE_POLYMESH" );
    meta_info_input.AddMetaInfo( "INPUT_DATAPATH", strFilePathPolyMesh );

    meta_info_output.AddMetaInfo( "OUTPUT_DATATYPE", "DATASTRUCT_POLYMESH");
	
    if( !data_transform_driver.RegisterPass( p_pass_parse_polymesh ) ){
	assert( 0 && "Register Pass failed.");
	return -1;
    }
    if( !data_transform_driver.AddTransformInputInfo( &meta_info_input ) ){
	assert( 0 && "Add Transform Input Info failed.");
	return -1;
    }    
    if( !data_transform_driver.AddTransformOutputInfo( &meta_info_output ) ){
	assert( 0 && "Add Transform Output Info failed.");
	return -1;
    }
    if( !data_transform_driver.BuildTransformInfoAggregate() ){
	assert( 0 && "Build Transform Info Aggregate failed.");
	return -1;
    }
    if( !data_transform_driver.ExecuteSuitablePath() ){
	assert( 0 && "Execute Suitable Path failed.");
	return -1;
    }
    
    return 0;
}
