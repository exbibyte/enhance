#ifndef INSTANCEMANAGER_H
#define INSTANCEMANAGER_H

#include "AssetManager.h"
#include "RenderType.h"

#include <map>

// //math library
// #define GLM_FORCE_RADIANS
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <glm/gtx/transform2.hpp>
// using glm::mat4;
// using glm::vec3;

template< class InstanceType >
class InstanceManager { };

template< >
class InstanceManager< RenderPolyVertices > : public AssetManager< unsigned int, std::vector< float > > {
public:
    using AssetManager< unsigned int, std::vector< float > >::AddData;
    using AssetManager< unsigned int, std::vector< float > >::GetData;
    using AssetManager< unsigned int, std::vector< float > >::GetDataArray;
};

template< >
class InstanceManager< RenderLightAmbient > : public AssetManager< unsigned int, std::vector< double > > {
public:
    using AssetManager< unsigned int, std::vector< double > >::AddData;
    using AssetManager< unsigned int, std::vector< double > >::GetData;
    using AssetManager< unsigned int, std::vector< double > >::GetDataArray; 
};

template< >
class InstanceManager< RenderLightSpectral > : public AssetManager< unsigned int, std::vector< double > > {
public:
    using AssetManager< unsigned int, std::vector< double > >::AddData;
    using AssetManager< unsigned int, std::vector< double > >::GetData;
    using AssetManager< unsigned int, std::vector< double > >::GetDataArray; 
};

template< >
class InstanceManager< RenderLightDiffuse > : public AssetManager< unsigned int, std::vector< double > > {
public:
    using AssetManager< unsigned int, std::vector< double > >::AddData;
    using AssetManager< unsigned int, std::vector< double > >::GetData;
    using AssetManager< unsigned int, std::vector< double > >::GetDataArray; 
};

template< >
class InstanceManager< RenderCameraProjection > : public AssetManager< unsigned int, std::vector< double > > {
public:
    using AssetManager< unsigned int, std::vector< double > >::AddData;
    using AssetManager< unsigned int, std::vector< double > >::GetData;
    using AssetManager< unsigned int, std::vector< double > >::GetDataArray;
};

#endif
