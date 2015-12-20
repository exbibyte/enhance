#ifndef EN_ASSETMANAGER_H
#define EN_ASSETMANAGER_H

#include "AssetManager.h"

#include <string>
#include <vector>

class enAssetManager : public AssetManager< std::string, std::vector< double > >,
		       public AssetManager< std::string, std::vector< int > >,
		       public AssetManager< std::string, std::vector< float > >,
		       public AssetManager< std::string, double >,
                       public AssetManager< std::string, int >,
		       public AssetManager< std::string, float >,
		       public AssetManager< std::string, std::string >,
		       
		       public AssetManager< unsigned int, std::vector< double > >,
		       public AssetManager< unsigned int, std::vector< int > >,
		       public AssetManager< unsigned int, std::vector< float > >,
		       public AssetManager< unsigned int, double >,
                       public AssetManager< unsigned int, int >,
		       public AssetManager< unsigned int, float >,
		       public AssetManager< unsigned int, std::string > {
public:
    using AssetManager< std::string, std::vector< double > >::AddData;
    using AssetManager< std::string, std::vector< int > >::AddData;
    using AssetManager< std::string, std::vector< float > >::AddData;
    using AssetManager< std::string, double >::AddData;
    using AssetManager< std::string, int >::AddData;
    using AssetManager< std::string, float >::AddData;
    using AssetManager< std::string, std::string >::AddData;

    using AssetManager< std::string, std::vector< double > >::GetData;
    using AssetManager< std::string, std::vector< int > >::GetData;
    using AssetManager< std::string, std::vector< float > >::GetData;
    using AssetManager< std::string, double >::GetData;
    using AssetManager< std::string, int >::GetData;
    using AssetManager< std::string, float >::GetData;
    using AssetManager< std::string, std::string >::GetData;

    using AssetManager< std::string, std::vector< double > >::GetDataArray;
    using AssetManager< std::string, std::vector< int > >::GetDataArray;
    using AssetManager< std::string, std::vector< float > >::GetDataArray;

    using AssetManager< unsigned int, std::vector< double > >::AddData;
    using AssetManager< unsigned int, std::vector< int > >::AddData;
    using AssetManager< unsigned int, std::vector< float > >::AddData;
    using AssetManager< unsigned int, double >::AddData;
    using AssetManager< unsigned int, int >::AddData;
    using AssetManager< unsigned int, float >::AddData;
    using AssetManager< unsigned int, std::string >::AddData;

    using AssetManager< unsigned int, std::vector< double > >::GetData;
    using AssetManager< unsigned int, std::vector< int > >::GetData;
    using AssetManager< unsigned int, std::vector< float > >::GetData;
    using AssetManager< unsigned int, double >::GetData;
    using AssetManager< unsigned int, int >::GetData;
    using AssetManager< unsigned int, float >::GetData;
    using AssetManager< unsigned int, std::string >::GetData;

    using AssetManager< unsigned int, std::vector< double > >::GetDataArray;
    using AssetManager< unsigned int, std::vector< int > >::GetDataArray;
    using AssetManager< unsigned int, std::vector< float > >::GetDataArray;
};

#endif
