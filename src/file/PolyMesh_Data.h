#ifndef POLYMESH_DATA_H
#define POLYMESH_DATA_H

#include <iostream>

class PolyMesh_Data {
public:
    enum Enum{
	VERT,
	NORMAL,
	EDGE,
	FACE,
	TEXSRC,
	TEXCOORD,
	BUFFERINFO,
	BUFFERINFO_SEQUENCE,
    };
    Enum _type;
    virtual void PrintData(){};
    virtual bool GetArray( double * & data, int & iCount ){ return false; }
};

class PolyMesh_Data_Vert : public PolyMesh_Data {
public:
    PolyMesh_Data_Vert(){
	_type = VERT;
    }
    int _id;
    std::vector<double> _vec_coord;
    void PrintData(){
	std::cout<< "Vert{ Id: " << _id << ". coord: ";
	for( auto & i : _vec_coord ){
	    std::cout<< i << " ";	   
	}
	std::cout<< "}" << std::endl;
    }
    bool GetArray( double * & data, int & iCount ){
	iCount = _vec_coord.size();
	if( 0 == iCount ){
	    return false;
	}
	data = new double [ iCount ];
	double * current = data;
	for( auto & i : _vec_coord ){
	    *current++ = i;
	}
	return true;
    }
};

class PolyMesh_Data_Normal : public PolyMesh_Data {
public:
    PolyMesh_Data_Normal(){
	_type = NORMAL;
    }
    int _id;
    std::vector<double> _vec_coord;
    void PrintData(){
	std::cout<< "Normal{ Id: " << _id << ". coord: ";
	for( auto & i : _vec_coord ){
	    std::cout<< i << " ";
	}
	std::cout<< "}" << std::endl;
    }
    bool GetArray( double * & data, int & iCount ){
	iCount = _vec_coord.size();
	if( 0 == iCount ){
	    return false;
	}
	data = new double [ iCount ];
	double * current = data;
	for( auto & i : _vec_coord ){
	    *current++ = i;
	}
	return true;
    }
};

class PolyMesh_Data_Face : public PolyMesh_Data {
public:
    PolyMesh_Data_Face(){
	_type = FACE;
    }
    int _id;
    std::vector<int> _vec_id_vert;
    bool _dir_verts_ccw;
    bool _dir_norm_align_verts;
    void PrintData(){
	std::cout<< "Face{ Id: " << _id << ". ";
	std::cout<< "dir_verts_ccw: " << _dir_verts_ccw << ". ";
	std::cout<< "dir_norm_align_verts: " << _dir_norm_align_verts << ". ";
	std::cout<< "id_verts: ";
	for( auto & i : _vec_id_vert ){
	    std::cout<< i << " ";
	}	
	std::cout<< "}" << std::endl;
    } 
};

class PolyMesh_Data_Edge : public PolyMesh_Data {
public:
    PolyMesh_Data_Edge(){
	_type = EDGE;
    }
    int _id;
    std::vector<int> _vec_id_vert;
    void PrintData(){
	std::cout<< "Edge{ Id: " << _id << ". ";
	std::cout<< "id_verts: ";
	for( auto & i : _vec_id_vert ){
	    std::cout<< i << " ";
	}	
	std::cout<< "}" << std::endl;
    }
};

class PolyMesh_Data_TexSrc : public PolyMesh_Data {
public:
    PolyMesh_Data_TexSrc(){
	_type = TEXSRC;
    }
    int _id;
    std::string _str_path;
    void PrintData(){
	std::cout<< "TexSrc{ Id: " << _id << ". ";
	std::cout<< "path: " << _str_path;
	std::cout<< " }" << std::endl;
    }
};

class PolyMesh_Data_TexCoord : public PolyMesh_Data {
public:
    PolyMesh_Data_TexCoord(){
	_type = TEXCOORD;
    }
    int _id;
    int _id_vert;
    int _id_texsrc;
    std::vector<double> _vec_txcoord;
    void PrintData(){
	std::cout<< "TexCoord{ Id: " << _id << ". ";
	std::cout<< "id_vert: " << _id_vert << ". ";
	std::cout<< "id_texsrc: " << _id_texsrc << ". coord: ";
	for( auto & i : _vec_txcoord ){
	    std::cout<< i << " ";
	}	
	std::cout<< "}" << std::endl;
    }    
};

class PolyMesh_Data_BufferInfo : public PolyMesh_Data {
public:
    PolyMesh_Data_BufferInfo(){
	_type = BUFFERINFO;
    }
    int _id;
    int _offset;
    int _length;
    void PrintData(){
	std::cout<< "BufferInfo{ Id: " << _id << ". ";
	std::cout<< "offset: " << _offset << ". ";
	std::cout<< "length: " << _length << " ";
	std::cout<< "}" << std::endl;
    }    
};

class PolyMesh_Data_BufferInfoSequence : public PolyMesh_Data {
public:
    PolyMesh_Data_BufferInfoSequence(){
	_type = BUFFERINFO_SEQUENCE;
    }
    int _id;
    std::vector<int> _vec_sequence;
    int _loop;
    void PrintData(){
	std::cout<< "BufferInfoSequence{ Id: " << _id << ". ";
	std::cout<< "sequence: ";
	for( auto & i : _vec_sequence ){
	    std::cout<< i << " ";
	}	
	std::cout<< ". ";
	std::cout<< "loop: " << _loop << ". ";
	std::cout<< "}" << std::endl;
    }
};

#endif
