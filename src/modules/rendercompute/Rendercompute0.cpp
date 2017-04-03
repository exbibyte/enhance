#include "Rendercompute0.hpp"
#include "IRendercompute.hpp"

#include "RenderData.hpp"
#include "PassParsePolyMesh.h"
#include "PassConvertPolyMeshDataStructToArray.h"
#include "DataTransformDriver.h"
#include "PolyMesh_Data_Arrays.h"

#include "Vec.hpp"

#include <vector>
#include <iostream>
#include <memory>
#include <list>
#include <cassert>

RenderData Rendercompute0::compute( light l, camera c, std::list< IRendercompute::RenderDataPack > render_data ){

    RenderData renderdata;

    //context --------------------------------------------------------------------
    vector<int> context_windowsize { 500, 500 };
    vector<int> context_texturesize_shadowmap { 2500, 2500 };
    string context_title = "renderer";

    //apply settings -------------------------------------------------------------
    std::shared_ptr<RenderLight> light = std::make_shared< RenderLight >();
    light->AddDataSingle( RenderLightData::Coordinate(),  l._light_position    );
    light->AddDataSingle( RenderLightData::Lookat(),      l._light_lookat      );
    light->AddDataSingle( RenderLightData::Up(),          l._light_up          );
    light->AddDataSingle( RenderLightData::Perspective(), l._light_perspective );
    light->AddDataSingle( RenderLightData::Ambient(),     l._light_ambient     );
    light->AddDataSingle( RenderLightData::Diffuse(),     l._light_diffuse     );
    light->AddDataSingle( RenderLightData::Specular(),    l._light_specular    );
	    
    std::shared_ptr<RenderCamera> camera = std::make_shared< RenderCamera >();
    camera->AddDataSingle( RenderCameraData::Coordinate(),  c._camera_position    );
    camera->AddDataSingle( RenderCameraData::Lookat(),      c._camera_lookat      );
    camera->AddDataSingle( RenderCameraData::Up(),          c._camera_up          );
    camera->AddDataSingle( RenderCameraData::Perspective(), c._camera_perspective );
    camera->AddDataSingle( RenderCameraData::Ambient(),     c._camera_ambient     );
    camera->AddDataSingle( RenderCameraData::Diffuse(),     c._camera_diffuse     );
    camera->AddDataSingle( RenderCameraData::Specular(),    c._camera_specular    );

    std::shared_ptr<RenderContext> context = std::make_shared< RenderContext >();
    context->AddDataSingle( RenderContextData::WindowSize(),           context_windowsize );
    context->AddDataSingle( RenderContextData::TextureSizeShadowMap(), context_texturesize_shadowmap );
    context->AddDataSingle( RenderContextData::Title(),                context_title );

    renderdata._light = light;
    renderdata._camera = camera;
    renderdata._context = context;
	
    //entities -------------------------------------------------------------------
    for( auto & i : render_data )
    {
	//orientation
	
	vector<double> entity_translate    { i.translate._vec[0], i.translate._vec[1], i.translate._vec[2] };
	vector<double> entity_rotate_axis  { i.orient_axis._vec[0], i.orient_axis._vec[1], i.orient_axis._vec[2] };
	vector<double> entity_rotate_angle { i.orient_angle };
    
	//set vertex data 
	vector<double> & entity_vertices = i.vert_coord;
	vector<double> & entity_normals = i.vert_normal;

	//apply settings -------------------------------------------------------------
	std::shared_ptr<RenderEntity> entity_01 = std::make_shared<RenderEntity>();
	entity_01->AddDataSingle( RenderPolyData::Coordinate(),    entity_translate );
	entity_01->AddDataSingle( RenderPolyData::RotationAxis(),  entity_rotate_axis );
	entity_01->AddDataSingle( RenderPolyData::RotationAngle(), entity_rotate_angle );

	entity_01->AddDataSingle( RenderMaterialData::Ambient(),   i._material._ambient );
	entity_01->AddDataSingle( RenderMaterialData::Diffuse(),   i._material._diffuse );
	entity_01->AddDataSingle( RenderMaterialData::Specular(),  i._material._specular );
	entity_01->AddDataSingle( RenderMaterialData::Shininess(), i._material._shininess );

	entity_01->AddDataSingle( RenderVertexData::Normals(),  entity_normals );
	entity_01->AddDataSingle( RenderVertexData::Vertices(), entity_vertices );
	
	renderdata._entities.push_back( entity_01 );
    }

    return renderdata;
}
