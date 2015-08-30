#include "Filter_ParsePolyMesh.h"
#include "Filter_ParseNode.h"
#include "ParseNode.h"
#include "PolyMesh_Data.h"

#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;

#undef DEBUG_FILTER_PARSE

Filter_ParsePolyMesh::Filter_ParsePolyMesh(){
    SetNodeTraversal( Filter_ParseNode_NodeTraversal::POST_ORDER );
}

bool Filter_ParsePolyMesh::TransformNode( ParseNode * node ){
    if( "vertices" == node->_strVarName ){
	for( auto * i : node->_children ){
	    vector<double> vecVal;
	    int iId;
	    for( auto * j : i->_children ){
		if( "id" == j->_strVarName ){
		    iId = atoi( j->_strVarVal.c_str() );		    
		}
		else if( "vec" == j->_strVarName ){
		    for( auto * k : j->_children ){
			double dVal = atof( k->_strVarVal.c_str() );
			vecVal.push_back( dVal );
		    }
		}	    
	    }
#ifdef DEBUG_FILTER_PARSE
	    cout<< "Found Vertice: id: " <<  iId << ". vec: ";
	    for( auto & m : vecVal ){
	    	cout<< m << " ";
	    }
	    cout<<endl;
#endif

	    //create PolyMesh_Data
	    PolyMesh_Data_Vert * vert = new PolyMesh_Data_Vert;
	    vert->_id = iId;
	    vert->_vec_coord = vecVal;
	    _vec_PolyMesh_Data_Vert.push_back( vert );
	}
    }
    else if( "normals" == node->_strVarName ){
	for( auto * i : node->_children ){
	    vector<double> vecVal;
	    int iId;
	    for( auto * j : i->_children ){
		if( "id" == j->_strVarName ){
		    iId = atoi( j->_strVarVal.c_str() );		    
		}
		else if( "vec" == j->_strVarName ){
		    for( auto * k : j->_children ){
			double dVal = atof( k->_strVarVal.c_str() );
			vecVal.push_back( dVal );
		    }
		}	    
	    }
#ifdef DEBUG_FILTER_PARSE
	    cout<< "Found Normal: id: " <<  iId << ". vec: ";
	    for( auto & m : vecVal ){
		cout<< m << " ";
	    }
	    cout<<endl;
#endif

	    //create PolyMesh_Data
	    PolyMesh_Data_Normal * normal = new PolyMesh_Data_Normal;
	    normal->_id = iId;
	    normal->_vec_coord = vecVal;
	    _vec_PolyMesh_Data_Normal.push_back( normal );
	}
    }
    else if( "edges" == node->_strVarName ){
	for( auto * i : node->_children ){
	    vector<int> vecVertIds;
	    int iId;
	    for( auto * j : i->_children ){
		if( "id" == j->_strVarName ){
		    iId = atoi( j->_strVarVal.c_str() );		    
		}
		else if( "vert_ids" == j->_strVarName ){
		    for( auto * k : j->_children ){
			int iVertId = atoi( k->_strVarVal.c_str() );
			vecVertIds.push_back( iVertId );
		    }
		}	    
	    }
#ifdef DEBUG_FILTER_PARSE
	    cout<< "Found Edge: id: " <<  iId << ". vert_ids: ";
	    for( auto & m : vecVertIds ){
		cout<< m << " ";
	    }
	    cout<<endl;
#endif
	    
	    //create PolyMesh_Data
	    PolyMesh_Data_Edge * edge = new PolyMesh_Data_Edge;
	    edge->_id = iId;
	    edge->_vec_id_vert = vecVertIds;
	    _vec_PolyMesh_Data_Edge.push_back( edge );
	}
    }
    else if( "faces" == node->_strVarName ){
	for( auto * i : node->_children ){
	    vector<int> vecVertIds;
	    int iId;
	    int iDirVertsCcw;
	    int iDirNormAlignVerts;
	    for( auto * j : i->_children ){
		if( "id" == j->_strVarName ){
		    iId = atoi( j->_strVarVal.c_str() );		    
		}
		else if( "vert_ids" == j->_strVarName ){
		    for( auto * k : j->_children ){
			int iVertId = atoi( k->_strVarVal.c_str() );
			vecVertIds.push_back( iVertId );
		    }
		}
		else if( "dir_verts_ccw" == j->_strVarName ){
		    iDirVertsCcw = atoi( j->_strVarVal.c_str() );
		}
		else if( "dir_norm_align_verts" == j->_strVarName ){
		    iDirNormAlignVerts = atoi( j->_strVarVal.c_str() );
		}
	    }
#ifdef DEBUG_FILTER_PARSE
	    cout<< "Found Face: id: " <<  iId << ". ";
	    cout<< "dir_verts_ccw: " << iDirVertsCcw << ". ";
	    cout<< "dir_norm_align_verts: " << iDirNormAlignVerts << ". ";
	    cout<< "vert_ids: ";
	    for( auto & m : vecVertIds ){
		cout<< m << " ";
	    }
	    cout<<endl;
#endif

	    //create PolyMesh_Data
	    PolyMesh_Data_Face * face = new PolyMesh_Data_Face;
	    face->_id = iId;
	    face->_dir_verts_ccw = ( iDirVertsCcw == 0 )? false : true;
	    face->_dir_norm_align_verts= ( iDirNormAlignVerts == 0 )? false : true;
	    face->_vec_id_vert = vecVertIds;
	    _vec_PolyMesh_Data_Face.push_back( face );
	}
    }    
    else if( "txsrcs" == node->_strVarName ){
	for( auto * i : node->_children ){
	    string strTexSrcPath;
	    int iId;
	    for( auto * j : i->_children ){
		if( "id" == j->_strVarName ){
		    iId = atoi( j->_strVarVal.c_str() );		    
		}
		else if( "path" == j->_strVarName ){
		    strTexSrcPath = j->_strVarVal;
		}
	    }
#ifdef DEBUG_FILTER_PARSE
	    cout<< "Found Texture Source: id: " <<  iId << ". ";
	    cout<< "Path: "<< strTexSrcPath;
	    cout<<endl;
#endif

	    //create PolyMesh_Data
	    PolyMesh_Data_TexSrc * texsrc = new PolyMesh_Data_TexSrc;
	    texsrc->_id = iId;
	    texsrc->_str_path = strTexSrcPath;
	    _vec_PolyMesh_Data_TexSrc.push_back( texsrc );
	}
    }
    else if( "txcoords" == node->_strVarName ){
	for( auto * i : node->_children ){
	    int iId;
	    int iId_Vert;
	    int iId_TextureSource;
	    vector<double> vecTextureCoord;
	    for( auto * j : i->_children ){
		if( "id" == j->_strVarName ){
		    iId = atoi( j->_strVarVal.c_str() );		    
		}
		else if( "vert_id" == j->_strVarName ){
		    iId_Vert = atoi( j->_strVarVal.c_str() );
		}
		else if( "ts_id" == j->_strVarName ){
		    iId_TextureSource = atoi( j->_strVarVal.c_str() );
		}
		else if( "coord" == j->_strVarName ){
		    for( auto * k : j->_children ){
			double dCoord = atof( j->_strVarVal.c_str() );
			vecTextureCoord.push_back( dCoord );
		    }
		}
	    }
#ifdef DEBUG_FILTER_PARSE
	    cout<< "Found Texture Coordinate: id: " <<  iId << ". ";
	    cout<< "vert_id: " << iId_Vert << ". ";
	    cout<< "ts_id: " << iId_TextureSource << ". ";
	    cout<< "coord: ";
	    for( auto & m : vecTextureCoord ){
		cout << m << " ";
	    }
	    cout<<endl;
#endif
	    //create PolyMesh_Data
	    PolyMesh_Data_TexCoord * texcoord = new PolyMesh_Data_TexCoord;
	    texcoord->_id = iId;
	    texcoord->_id_vert = iId_Vert;
	    texcoord->_id_texsrc = iId_TextureSource;
	    texcoord->_vec_txcoord = vecTextureCoord;
	    _vec_PolyMesh_Data_TexCoord.push_back( texcoord );
	}
    }
    else if( "bufferinfo" == node->_strVarName ){
	for( auto * i : node->_children ){
	    string strName;
	    int iOffset;
	    int iLength;
	    for( auto * j : i->_children ){
		if( "name" == j->_strVarName ){
		    strName = j->_strVarVal;
		}
		else if( "offset" == j->_strVarName ){
		    iOffset = atoi( j->_strVarVal.c_str() );
		}
		else if( "length" == j->_strVarName ){
		    iLength = atoi( j->_strVarVal.c_str() );
		}
	    }
#ifdef DEBUG_FILTER_PARSE
	    cout<< "Found BufferInfo: name: " <<  strName << ". ";
	    cout<< "offset: " << iOffset << ". ";
	    cout<< "length: " << iLength << endl;
#endif
	    //create PolyMesh_Data
	    PolyMesh_Data_BufferInfo * buffer_info = new PolyMesh_Data_BufferInfo;
	    buffer_info->_name = strName;
	    buffer_info->_offset = iOffset;
	    buffer_info->_length = iLength;
	    _vec_PolyMesh_Data_BufferInfo.push_back( buffer_info );
	}
    }
    else if( "bufferinfosequence" == node->_strVarName ){
	for( auto * i : node->_children ){
	    string strName;
	    vector< string > vecSequence;
	    int iLoop;
	    for( auto * j : i->_children ){
		if( "name" == j->_strVarName ){
		    strName = j->_strVarVal;
		}
		else if( "sequence" == j->_strVarName ){
		    for( auto * k : j->_children ){
		        string strVal = k->_strVarVal;
		        vecSequence.push_back( strVal );
		    }
		}
		else if( "loop" == j->_strVarName ){
		    iLoop = atoi( j->_strVarVal.c_str() );
		}
	    }
#ifdef DEBUG_FILTER_PARSE
	    cout<< "Found BufferInfoSequence: name: " <<  strName << ". ";
	    cout<< "sequence: ";
	    for( auto & m : vecSequence ){
		cout << m << " ";
	    }
	    cout<<". ";
	    cout<< "loop: " << iLoop << endl;
#endif
	    //create PolyMesh_Data
	    PolyMesh_Data_BufferInfoSequence * buffer_info_sequence = new PolyMesh_Data_BufferInfoSequence;
	    buffer_info_sequence->_name = strName;
	    buffer_info_sequence->_vec_sequence = vecSequence;
	    buffer_info_sequence->_loop = iLoop;
	    _vec_PolyMesh_Data_BufferInfoSequence.push_back( buffer_info_sequence );
	}
    }
    return true;
}
