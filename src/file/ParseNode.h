#ifndef PARSENODE_H
#define PARSENODE_H

#include <vector>
#include <string>

class ParseNode {
public:
    ParseNode(){}		     
    std::string _strVarName;
    std::string _strVarVal;
    std::vector< ParseNode * > _children;
};

#endif
