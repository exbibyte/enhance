#include "Rendercompute0.hpp"
#include "IRendercompute.hpp"

#include "RenderData.hpp"
#include "PassParsePolyMesh.h"
#include "PassConvertPolyMeshDataStructToArray.h"
#include "DataTransformDriver.h"
#include "PolyMesh_Data_Arrays.h"

#include "Vec.h"

#include <vector>
#include <iostream>
#include <memory>
#include <list>

RenderData Rendercompute0::compute( std::vector<double> vert_coord, std::vector<double> vert_normal, Vec orient_axis, double orient_angle ){

    RenderData renderdata;

    //entities -------------------------------------------------------------------
    //orientation	
    vector<double> entity_translate    { 0, 0, 0};
    // vector<double> entity_rotate_axis  { 1, 0, 0};
    // // vector<double> entity_rotate_angle { 0 };
    _rotation_angle += 0.00;
    // vector<double> entity_rotate_angle { _rotation_angle };

    vector<double> entity_rotate_axis  { orient_axis._vec[0], orient_axis._vec[1], orient_axis._vec[2] };
    vector<double> entity_rotate_angle { orient_angle + _rotation_angle };
    
    //set material data
    vector<double> entity_material_ambient   { 1.0, 1.0, 1.0 };
    vector<double> entity_material_diffuse   { 1, 1, 1 };
    vector<double> entity_material_specular  { 1, 1, 1 };
    vector<double> entity_material_shininess { 2 };
    //set vertex data 
    vector<double> & entity_vertices = vert_coord;
    vector<double> & entity_normals = vert_normal;

    //light ----------------------------------------------------------------------
    vector<double> light_position     { 0, 0, 10 };
    vector<double> light_lookat       { 0, 0, 0 };
    vector<double> light_up           { 0, 1, 0 };
    vector<double> light_perspective  { 60.0f, 1.0f, 0.1f, 1000.0f };
    vector<double> light_ambient      { 0.05f, 0.05f, 0.05f };
    vector<double> light_diffuse      { 0.5f, 0.5f, 0.5f };
    vector<double> light_specular     { 0.45f, 0.45f, 0.45f };

    //camera ---------------------------------------------------------------------
    vector<double> camera_position    { -5, -5, 8.0 };
    vector<double> camera_lookat      { 0, 0, 0 };
    vector<double> camera_up          { 0, 1, 0 };
    vector<double> camera_perspective { 90.0f, 1.0f, 0.1f, 500.0f };
    vector<double> camera_ambient     { 0.05f, 0.05f, 0.05f };
    vector<double> camera_diffuse     { 0.5f, 0.5f, 0.5f };
    vector<double> camera_specular    { 0.45f, 0.45f, 0.45f };

    //context --------------------------------------------------------------------
    vector<int> context_windowsize { 500, 500 };
    vector<int> context_texturesize_shadowmap { 2500, 2500 };
    string context_title = "engine0";

    //apply settings -------------------------------------------------------------
    std::shared_ptr<RenderEntity> entity_01 = std::make_shared<RenderEntity>();
    entity_01->AddDataSingle( RenderPolyData::Coordinate(),    entity_translate );
    entity_01->AddDataSingle( RenderPolyData::RotationAxis(),  entity_rotate_axis );
    entity_01->AddDataSingle( RenderPolyData::RotationAngle(), entity_rotate_angle );

    entity_01->AddDataSingle( RenderMaterialData::Ambient(),   entity_material_ambient );
    entity_01->AddDataSingle( RenderMaterialData::Diffuse(),   entity_material_diffuse );
    entity_01->AddDataSingle( RenderMaterialData::Specular(),  entity_material_specular );
    entity_01->AddDataSingle( RenderMaterialData::Shininess(), entity_material_shininess );
    entity_01->AddDataSingle( RenderVertexData::Normals(),  entity_normals );
    entity_01->AddDataSingle( RenderVertexData::Vertices(), entity_vertices );
    
    std::shared_ptr<RenderLight> light = std::make_shared< RenderLight >();
    light->AddDataSingle( RenderLightData::Coordinate(),  light_position    );
    light->AddDataSingle( RenderLightData::Lookat(),      light_lookat      );
    light->AddDataSingle( RenderLightData::Up(),          light_up          );
    light->AddDataSingle( RenderLightData::Perspective(), light_perspective );
    light->AddDataSingle( RenderLightData::Ambient(),     light_ambient     );
    light->AddDataSingle( RenderLightData::Diffuse(),     light_diffuse     );
    light->AddDataSingle( RenderLightData::Specular(),    light_specular    );
	    
    std::shared_ptr<RenderCamera> camera = std::make_shared< RenderCamera >();
    camera->AddDataSingle( RenderCameraData::Coordinate(),  camera_position    );
    camera->AddDataSingle( RenderCameraData::Lookat(),      camera_lookat      );
    camera->AddDataSingle( RenderCameraData::Up(),          camera_up          );
    camera->AddDataSingle( RenderCameraData::Perspective(), camera_perspective );
    camera->AddDataSingle( RenderCameraData::Ambient(),     camera_ambient     );
    camera->AddDataSingle( RenderCameraData::Diffuse(),     camera_diffuse     );
    camera->AddDataSingle( RenderCameraData::Specular(),    camera_specular    );

    std::shared_ptr<RenderContext> context = std::make_shared< RenderContext >();
    context->AddDataSingle( RenderContextData::WindowSize(),           context_windowsize );
    context->AddDataSingle( RenderContextData::TextureSizeShadowMap(), context_texturesize_shadowmap );
    context->AddDataSingle( RenderContextData::Title(),                context_title );

    renderdata._entities.push_back( entity_01 );
    renderdata._light = light;
    renderdata._camera = camera;
    renderdata._context = context;

    return renderdata;
}
