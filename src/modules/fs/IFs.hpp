#ifndef IFS_H
#define IFS_H

//TODO
enum class eFileType {
    ORG,
    POLY,
    TEXT,
    STORAGE,
    PORT,
};

class FileInfoOrg {
public:
    
};
class FileInfoPoly {
public:
    
};
class FileInfoText {
public:
    
};
class FileInfoStorage {
public:
    
};
class FileInfoPort {
public:
    
};

class MetaFile {
public:
    MetaFile *  _parent;
    eFileType   _filetype;
    void *      _data;
    char        _hash[128];
};

class IFs {
public:
    virtual ~IFs(){}
    //TODO
    virtual void write( eFileType , void * data ){}
    virtual void read( eFileType , void * & data ){}
};

#endif
