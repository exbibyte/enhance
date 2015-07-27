#ifndef DATA_TRANSFORM_METAINFO_H
#define DATA_TRANSFORM_METAINFO_H

#include <vector>
#include <string>
#include <utility>
#include <map>

class DataTransformMetaInfo {
public:
    DataTransformMetaInfo( std::string id = "" );
    bool AddMetaInfo( std::string key, std::string val );
    bool GetMetaInfo( std::string key, std::vector< std::string > & vals ) const;    
    bool RemoveMetaInfo( std::string key );
    bool RemoveMetaInfoAll();
    bool GetId( std::string & id ) const;
    bool AddMetaInfo( DataTransformMetaInfo & meta_info );
private:
    std::multimap< std::string, std::string > _MetaInfo;
    std::string _id;
};

#endif
