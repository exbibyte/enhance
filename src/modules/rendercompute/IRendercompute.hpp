#ifndef IRENDERCOMPUTE_H
#define IRENDERCOMPUTE_H

#include "RenderData.hpp"

#include "Vec.hpp"

#include <vector>
#include <list>

class light {
public:
    vector<double> _light_position     { 0, 0, 80 };
    vector<double> _light_lookat       { 0, 0, 0 };
    vector<double> _light_up           { 0, 1, 0 };
    vector<double> _light_perspective  { 60.0f, 1.0f, 0.1f, 1000.0f };
    vector<double> _light_ambient      { 0.05f, 0.05f, 0.05f };
    vector<double> _light_diffuse      { 0.5f, 0.5f, 0.5f };
    vector<double> _light_specular     { 0.45f, 0.45f, 0.45f };
};

class camera {
public:
    vector<double> _camera_position    { 15, 15, 15 };
    vector<double> _camera_lookat      { 0, 0, 0 };
    vector<double> _camera_up          { 0, 1, 0 };
    vector<double> _camera_perspective { 90.0f, 1.0f, 0.1f, 500.0f };
    vector<double> _camera_ambient     { 0.05f, 0.05f, 0.05f };
    vector<double> _camera_diffuse     { 0.5f, 0.5f, 0.5f };
    vector<double> _camera_specular    { 0.45f, 0.45f, 0.45f };
};

class material {
public:
    vector<double> _ambient   { 1.0, 1.0, 1.0 };
    vector<double> _diffuse   { 1, 1, 1 };
    vector<double> _specular  { 1, 1, 1 };
    vector<double> _shininess { 2 };
};

class IRendercompute {
public:
    class RenderDataPack {
    public:
	std::vector<double> vert_coord;
	std::vector<double> vert_normal;
	Vec orient_axis;
	double orient_angle;
	Vec translate;
	material _material;
    };
    virtual ~IRendercompute(){}
    virtual RenderData compute( light l, camera c, std::list< IRendercompute::RenderDataPack > render_data ) = 0;
};

#endif
