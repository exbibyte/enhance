/**
   Bill Liu
   #63461081

   Info:
   Convert .obj file to custom file format
   .obj file need have vertex and texture coordinates
   This generates custom file format having vertices, face normals, triangles, texture coordinates, and 1 reference to 1 texture image per object having the name as object_name.ppm

*/

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <tuple>
#include <regex>

using namespace std;

typedef tuple<int,int,int, int, int, float,float,float,float,float,float> tTriangleData;

int main(int argc, char** argv)
{

    if(argc<2)
    {
//    cout<<"need: <input .obj file> <output file name> <object name>"<<endl;
        cout << "need: <input .obj file>" << endl;
        return 0;
    }

    fstream input;
    fstream output;

    input.open (argv[1], std::fstream::in);
//  output.open (argv[2], std::fstream::out | std::fstream::trunc);
  
    string current;
    stringstream ss;

    vector<vector<float> > vTexturecoord;
    vector<vector<double> > vVertex;
    vector<vector<double> > vNormal;
    vector<tTriangleData> vTriangle;

    vector<string> vTextureName;
    int objectCount = 0;
    int triangleCount = 0;

    while (getline(input, current)) 
    {    
        // object name
        std::regex reg_obj_name("^o (\\w+)");
        std::smatch match_obj_name;
        string result_obj_name;
        if (std::regex_search( current, match_obj_name, reg_obj_name ) && match_obj_name.size() > 1 ) {
            result_obj_name = match_obj_name.str(1);
	    cout << "Object Name: " << result_obj_name << endl;
	    continue;
        }

        //vertices
        std::regex reg_vert_coord("^v ([+-]?\\d+(?:\\.\\d+)?) ([+-]?\\d+(?:\\.\\d+)?) ([+-]?\\d+(?:\\.\\d+)?)");
        std::smatch match_vert_coord;
        string result_vert_coord;
        if (std::regex_search( current, match_vert_coord, reg_vert_coord ) && match_vert_coord.size() > 3 ) {
	    vector<double> vec_vert_coord;
	    vec_vert_coord.push_back( stod( match_vert_coord.str(1) ) );
	    vec_vert_coord.push_back( stod( match_vert_coord.str(2) ) );
	    vec_vert_coord.push_back( stod( match_vert_coord.str(3) ) );
	    cout << "Vertice Coord: " << vec_vert_coord[0] << ", " << vec_vert_coord[1] <<", " << vec_vert_coord[2] << endl;
	    vVertex.push_back( vec_vert_coord );
	    continue;
        }

        //normals
        std::regex reg_vert_norm("^vn ([+-]?\\d+(?:\\.\\d+)?) ([+-]?\\d+(?:\\.\\d+)?) ([+-]?\\d+(?:\\.\\d+)?)");
        std::smatch match_vert_norm;
        string result_vert_norm;
        if (std::regex_search( current, match_vert_norm, reg_vert_norm ) && match_vert_norm.size() > 3 ) {
	    vector<double> vec_vert_norm;
	    vec_vert_norm.push_back( stod( match_vert_norm.str(1) ) );
	    vec_vert_norm.push_back( stod( match_vert_norm.str(2) ) );
	    vec_vert_norm.push_back( stod( match_vert_norm.str(3) ) );
	    cout << "Vertice Normal: " << vec_vert_norm[0] << ", " << vec_vert_norm[1] <<", " << vec_vert_norm[2] << endl;
	    vNormal.push_back( vec_vert_norm );
	    continue;
        }

        //triangle faces
	std::regex reg_face("^f (\\d)//(\\d) (\\d)//(\\d) (\\d)//(\\d)");
        std::smatch match_face;
        string result_face;
        if (std::regex_search( current, match_face, reg_face ) && match_face.size() > 6 ) {
	    vector<int> vec_vertcoord_index;
	    vec_vertcoord_index.push_back( stoi( match_face.str(1) ) );
	    vec_vertcoord_index.push_back( stoi( match_face.str(3) ) );
	    vec_vertcoord_index.push_back( stoi( match_face.str(5) ) );
	    vector<int> vec_vertnorm_index;
	    vec_vertnorm_index.push_back( stoi( match_face.str(2) ) );
	    vec_vertnorm_index.push_back( stoi( match_face.str(4) ) );
	    vec_vertnorm_index.push_back( stoi( match_face.str(6) ) );
	    cout << "Face: v0: " << vec_vertcoord_index[0] << ", ";
	    cout << "v1: " << vec_vertcoord_index[1] << ", ";
	    cout << "v2: " << vec_vertcoord_index[2] << ", ";
	    cout << "vn0: " << vec_vertnorm_index[0] << ", ";
	    cout << "vn1: " << vec_vertnorm_index[1] << ", ";
	    cout << "vn2: " << vec_vertnorm_index[2] << endl;
	    continue;
        }
    }

    // //write vertices
    // output<<"<vertices>"<<endl;
    // for(auto i : vVertex)
    // {
    //   for(auto j : i)  
    //   {
    //     output<< j << " ";
    //   }
    //   output<<endl;
    // }
    // output<<"</vertices>"<<endl;  

    // //write computed face normals
    // output<<"<normals>"<<endl;
    // for(auto i : vNormal)
    // {
    //   for(auto j : i)
    //   {
    //     output<< j <<" ";
    //   }
    //   output<<endl;
    // }
    // output<<"</normals>"<<endl;

    input.close();
//  output.close();

    return 0;
}
