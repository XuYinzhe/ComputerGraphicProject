//
// rayUI.h
//
// The header file for the UI part
//

#ifndef __rayUI_h__
#define __rayUI_h__

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Button.H>

#include <FL/fl_file_chooser.H>		// FLTK file chooser

#include "TraceGLWindow.h"
#include "../fileio/bitmap.h"

class TraceUI {
public:
	TraceUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;

	Fl_Slider*			m_sizeSlider;
	Fl_Slider*			m_depthSlider;
	Fl_Slider* m_attConstant_;
	Fl_Slider* m_attLinear_;
	Fl_Slider* m_attQuatric_;
	Fl_Slider* m_ambient_;
	Fl_Slider* m_threshold_;
	Fl_Slider* m_supersampling_;
	Fl_Slider* m_jittering_;
	Fl_Slider* m_background_;
	Fl_Slider* m_texture_;
	Fl_Slider* m_adapt_;
	Fl_Slider* m_adaptDiff_;
	Fl_Slider* m_focalLength_;
	Fl_Slider* m_Apa_;

	Fl_Check_Button * m_glossyButton;
	Fl_Check_Button * m_SSButton;
	Fl_Check_Button * m_DOFButton;


	Fl_Button*			m_renderButton;
	Fl_Button*			m_stopButton;

	TraceGLWindow*		m_traceGlWindow;

	// member functions
	void show();

	void		setRayTracer(RayTracer *tracer);

	int			getSize();
	int			getDepth();

	int bmp_width = 0;
	int bmp_height = 0;
	int texture_width = 0;
	int texture_height = 0;
	int normal_width = 0;
	int normal_height = 0;
	int m_supersampling = 1;
	int m_jittering = 0;
	int m_background = 0;
	int m_texture = 0;
	int m_adapt = 0;

	double m_attConstant=1;
	double m_attLinear=0.2;
	double m_attQuatric=0.1;
	double m_ambient = 1;
	double m_threshold;
	double m_adaptDiff;

	unsigned char* m_backgroundData;
	unsigned char* m_textureData = nullptr;
	unsigned char* m_NormalData = nullptr;
	
	bool		m_Glossy = false;
	bool		m_SoftShadow = false;
	bool		m_DOF = false;

	float focusLength = 3.0;
	float apa = 1;


private:
	RayTracer*	raytracer;

	int			m_nSize;
	int			m_nDepth;
	

// static class members
	static Fl_Menu_Item menuitems[];

	static TraceUI* whoami(Fl_Menu_* o);

	static void cb_load_scene(Fl_Menu_* o, void* v);
	static void cb_save_image(Fl_Menu_* o, void* v);
	static void cb_load_background(Fl_Menu_* o, void* v);
	static void cb_load_texture(Fl_Menu_* o, void* v);
	static void cb_load_normal(Fl_Menu_* o, void* v);
	static void cb_exit(Fl_Menu_* o, void* v);
	static void cb_about(Fl_Menu_* o, void* v);

	static void cb_exit2(Fl_Widget* o, void* v);

	static void cb_sizeSlides(Fl_Widget* o, void* v);
	static void cb_depthSlides(Fl_Widget* o, void* v);
	static void cb_attenuationConstant(Fl_Widget* o, void* v);
	static void cb_attenuationLinear(Fl_Widget* o, void* v);
	static void cb_attenuationQuatric(Fl_Widget* o, void* v);
	static void cb_ambient(Fl_Widget* o, void* v);
	static void cb_threshold(Fl_Widget* o, void* v);
	static void cb_supersampling(Fl_Widget* o, void* v);
	static void cb_jittering(Fl_Widget* o, void* v);
	static void cb_background(Fl_Widget* o, void* v);
	static void cb_texture(Fl_Widget* o, void* v);
	static void cb_dof(Fl_Widget* o, void* v);
	static void cb_adapt(Fl_Widget* o, void* v);
	static void cb_adaptDiff(Fl_Widget* o, void* v);

	static void cb_focalLength(Fl_Widget* o, void* v);

	static void cb_Apa(Fl_Widget* o, void* v);

	static void cb_soft(Fl_Widget* o, void* v);

	static void cb_glossy(Fl_Widget* o, void* v);

	static void cb_render(Fl_Widget* o, void* v);
	static void cb_stop(Fl_Widget* o, void* v);
};

#endif
