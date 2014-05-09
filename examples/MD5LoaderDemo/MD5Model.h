// MD5 Loader, by A.J. Tavakoli

#ifndef MD5MODEL_H
#define MD5MODEL_H

#include <string>
#include <stdexcept>
#include <vector>
#include <fstream>
#include "Quat.h"


class MD5Model {
public:
  MD5Model();
  ~MD5Model();

  void clear();
  void loadMesh(const char *filename);
  int  loadAnim(const char *filename);
  void setAnim(int animIndex, int frameIndex = 0);
  void setFrame(int frameIndex);
  void animate(float dt);
  void render();

  inline int getNumAnims() const { return int(anims.size()); }

  class Exception : public std::runtime_error {
  public:
    Exception(const std::string &msg): std::runtime_error(msg) { }
  };

protected:
  enum TOKEN { TOKEN_KEYWORD,
               TOKEN_INT,
               TOKEN_FLOAT,
               TOKEN_STRING,
               TOKEN_LBRACE,
               TOKEN_RBRACE,
               TOKEN_LPAREN,
               TOKEN_RPAREN,
               TOKEN_INVALID };

  class Vertex {
  public:
    int weightIndex;
    int weightCount;
    float pos[3];
    float tc[2];
    float n[3];
  };
  
  class Joint {
  public:
    std::string name;
    float pos[3];
    Quat quat;
    int parent;
    std::vector<int> children;
  };
  
  class Tri {
  public:
    int v[3]; // vertex indices
  };
  
  class Weight {
  public:
    int joint;
    float w;
    float pos[3];
  };
  
  class Mesh {
  public:
    std::string texture;
    std::vector<Vertex> verts;
    std::vector<Tri> tris;
    std::vector<Weight> weights;
  };
  
  class Frame {
  public:
    std::vector<float> animatedComponents;
    std::vector<Joint> joints;
    float min[3]; // min point of bounding box
    float max[3]; // max point of bounding box
  };
  
  class JointInfo {
  public:
    std::string name;
    int parentIndex;
    int flags;
    int startIndex;
  };

  // stores data from an anim file
  class Anim {
  public:
    Anim(); 
    int numFrames;
    int frameRate;
    int numAnimatedComponents;
    std::vector<Frame> frames;
    std::vector<Joint> baseJoints;
    std::vector<JointInfo> jointInfo;
  };

  // methods for parser
  void  readElements(std::ifstream &fin);
  void  readAnimElements(std::ifstream &fin, Anim &anim);
  void  readCommandLineEl(std::ifstream &fin);
  void  readNumMeshesEl(std::ifstream &fin);
  void  readNumJointsEl(std::ifstream &fin);
  void  readNumFramesEl(std::ifstream &fin, Anim &anim);
  void  readFrameRateEl(std::ifstream &fin, Anim &anim);
  void  readNumAnimatedComponentsEl(std::ifstream &fin, Anim &anim);
  void  readJointsEl(std::ifstream &fin);
  void  readMeshEl(std::ifstream &fin);
  void  readHierarchyEl(std::ifstream &fin, Anim &anim);
  void  readBoundsEl(std::ifstream &fin, Anim &anim);
  void  readBaseframeEl(std::ifstream &fin, Anim &anim);
  void  readFrameEl(std::ifstream &fin, Anim &anim);
  int   readInt(std::ifstream &fin);
  float readFloat(std::ifstream &fin);
  void  readVec(std::ifstream &fin, float *v, int n);

  // methods for lexer
  void readString(std::ifstream &fin, std::string &str);
  void skipComments(std::ifstream &fin);
  void skipWhitespace(std::ifstream &fin);
  TOKEN getNextToken(std::ifstream &fin, std::string *tokStr=NULL);

  // utility method to compute w component of a normalized quaternion
  Quat buildQuat(float x, float y, float z) const;

  void buildVerts();
  void buildVerts(Frame &frame);
  void buildNormals();
  void buildFrames(Anim &anim);

  int numJoints;
  int numMeshes;
  int currAnim;
  int currFrame;
  float animTime;
  Frame interpFrame; // used to interpolate between frames

  std::string textureName;

  std::vector<Mesh*> meshes;
  std::vector<Joint> joints;
  std::vector<Anim*> anims;
};


#endif
