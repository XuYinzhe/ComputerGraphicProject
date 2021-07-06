#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

// The main ray tracer.

#include "scene/scene.h"
#include "scene/ray.h"
#include <stack>

class RayTracer
{
public:
    RayTracer();
    ~RayTracer();
	void* UI;

    vec3f trace( Scene *scene, double x, double y );
	vec3f traceRay( Scene *scene, const ray& r, const vec3f& thresh, int depth , std::stack<Material*> materials);



	void getBuffer( unsigned char *&buf, int &w, int &h );
	Scene* getScene() const { return scene; }
	double aspectRatio();
	void traceSetup( int w, int h );
	void traceLines( int start = 0, int stop = 10000000 );
	void tracePixel( int i, int j );
	unsigned char* backgroundPixel(const vec3f& u, const vec3f& v, const vec3f& look, const ray& r);
	vec3f adaptSupper(const double X, const double Y, const double deltaX, const double deltaY, int depth);

	bool loadScene( char* fn );

	bool sceneLoaded();

	int bmp_width, bmp_height;
	unsigned char* m_backgroundData = nullptr;

private:
	unsigned char *buffer;
	int buffer_width, buffer_height;
	int bufferSize;
	Scene *scene;
	bool m_bSceneLoaded;
};

#endif // __RAYTRACER_H__
