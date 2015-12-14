#ifndef EN_ASSETMANAGER_H
#define EN_ASSETMANAGER_H

#include "AssetManager.h"

#include <string>
#include <vector>

class enAssetManager : public AssetManager< std::vector< double > >, AssetManager< std::vector< int > >, AssetManager< std::vector< float > >, AssetManager< double >, AssetManager< int >, AssetManager< float >, AssetManager< std::string > {
public:
    using AssetManager< std::vector< double > >::AddData;
    using AssetManager< std::vector< int > >::AddData;
    using AssetManager< std::vector< float > >::AddData;
    using AssetManager< double >::AddData;
    using AssetManager< int >::AddData;
    using AssetManager< float >::AddData;
    using AssetManager< std::string >::AddData;

    using AssetManager< std::vector< double > >::GetData;
    using AssetManager< std::vector< int > >::GetData;
    using AssetManager< std::vector< float > >::GetData;
    using AssetManager< double >::GetData;
    using AssetManager< int >::GetData;
    using AssetManager< float >::GetData;
    using AssetManager< std::string >::GetData;

    using AssetManager< std::vector< double > >::GetDataArray;
    using AssetManager< std::vector< int > >::GetDataArray;
    using AssetManager< std::vector< float > >::GetDataArray;
};

#endif
