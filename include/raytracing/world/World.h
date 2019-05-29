#pragma once

#include "raytracing/world/ViewPlane.h"
#include "raytracing/utilities/ShadeRec.h"

#include <vector>
#include <memory>

namespace rt
{

class GeoPrimitive;
class Light;
class Tracer;
class Ray;
class Camera;
class RenderOutput;

class World
{
public:
	World();
    ~World();

	//void Build();

	//void RenderScene();

	ShadeRec HitObjects(const Ray& ray) const;

	auto& GetBackgroundColor() const { return m_background_color; }
    void  SetBackgroundColor(const RGBColor& col) { m_background_color = col; }

	auto& GetLights() const { return m_lights; }

    void SetAmbient(std::unique_ptr<Light> ambient);
	auto& GetAmbient() const { return m_ambient; }

    void SetCamera(std::unique_ptr<Camera> camera);
	auto& GetCamera() const { return m_camera; }

    void SetViewPlane(const ViewPlane& vp) { m_vp = vp; }
	const ViewPlane& GetViewPlane() const { return m_vp; }

    void SetTracer(std::unique_ptr<Tracer> tracer);
	auto& GetTracer() const { return m_tracer; }

    void SetRenderOutput(RenderOutput* output) { m_output = output; }

	void DisplayPixel(const int row, const int column, const RGBColor& pixel_color) const;

	auto& GetObjects() const { return m_objects; }

	void AddObject(std::unique_ptr<GeoPrimitive> obj);

	void AddLight(std::unique_ptr<Light> light);

private:
	ViewPlane m_vp;

	RGBColor m_background_color;

	std::vector<std::unique_ptr<GeoPrimitive>> m_objects;

	std::vector<std::unique_ptr<Light>> m_lights;

    std::unique_ptr<Light> m_ambient = nullptr;

	std::unique_ptr<Tracer> m_tracer = nullptr;

    std::unique_ptr<Camera> m_camera = nullptr;

    // fixme: raylab's wxThread dtor
//    std::shared_ptr<RenderOutput> m_output = nullptr;
    RenderOutput* m_output = nullptr;

}; // World

}
