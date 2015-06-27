#ifndef FORMATPOLYMESH_H
#define FORMATPOLYMESH_H

#include <vector>
#include <string>

class FormatPolyMesh_Base {
public:
    enum Enum {
	UNINIT,
	VERT,
	FACE,
	EDGE,
	TEXSRC,
	TEXCOORD,
	ENUM_COUNT,
    };
    FormatPolyMesh_Base() : _type(UNINIT) {}
    Enum _type;
    std::string strVarName;
    std::string strVarVal;
    std::vector< FormatPolyMesh_Base * > children;
};

// class FormatPolyMesh_Vert : public FormatPolyMesh_Base {
// public:
//     FormatPolyMesh_Vert() : _type( VERT ) {}
//     int _id;
//     std::vector<double> _coords;
// };

// class FormatPolyMesh_Face : public FormatPolyMesh_Base {
// public:
//     FormatPolyMesh_Face() : _type( FACE ) {}
//     int _id;
//     std::vector<int> _id_verts;
// };

// class FormatPolyMesh_Edge : public FormatPolyMesh_Base {
// public:
//     FormatPolyMesh_Edge() : _type( EDGE ) {}
//     int _id;
//     std::vector<int> _id_verts;
// };

// class FormatPolyMesh_TexSrc : public FormatPolyMesh_Base {
// public:
//     FomatPolyMesh_TexSrc() : _type( TEXSRC ) {}
//     int _id;
//     std::string _str_path;
// };

// class FormatPolyMesh_TexCoord : public FormatPolyMesh_Base {
// public:
//     FomatPolyMesh_TexCoord() : _type( TEXCOORD ) {}
//     int _id;
//     int _id_vert;
//     int _id_texsrc;
//     std::vector<int> _txcoords;
// };

#endif
