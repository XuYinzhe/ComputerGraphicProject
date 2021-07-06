//
// TraceUI.h
//
// Handles FLTK integration and other user interface tasks
//
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <iostream>

#include <FL/fl_ask.h>

#include "TraceUI.h"
#include "../RayTracer.h"

static bool done;

//------------------------------------- Help Functions --------------------------------------------
TraceUI* TraceUI::whoami(Fl_Menu_* o)	// from menu item back to UI itself
{
	return ( (TraceUI*)(o->parent()->user_data()) );
}

//--------------------------------- Callback Functions --------------------------------------------
void TraceUI::cb_load_scene(Fl_Menu_* o, void* v) 
{
	TraceUI* pUI=whoami(o);
	
	char* newfile = fl_file_chooser("Open Scene?", "*.ray", NULL );

	if (newfile != NULL) {
		char buf[256];

		if (pUI->raytracer->loadScene(newfile)) {
			sprintf(buf, "Ray <%s>", newfile);
			done=true;	// terminate the previous rendering
		} else{
			sprintf(buf, "Ray <Not Loaded>");
		}

		pUI->m_mainWindow->label(buf);
	}
}

void TraceUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	TraceUI* pUI=whoami(o);
	
	char* savefile = fl_file_chooser("Save Image?", "*.bmp", "save.bmp" );
	if (savefile != NULL) {
		pUI->m_traceGlWindow->saveImage(savefile);
	}
}

void TraceUI::cb_load_background(Fl_Menu_* o, void* v)
{
	TraceUI* pUI = whoami(o);

	char* newfile = fl_file_chooser("Open background?", "*.bmp", NULL);
	if (newfile != NULL)
	{
		int width, height;
		unsigned char* data = readBMP(newfile, width, height);
		if (data != NULL)
			pUI->m_backgroundData = data;
		else 
			fl_alert("Background loading error!");

		pUI->bmp_height = height;
		pUI->bmp_width = width;
	}
}

void TraceUI::cb_load_texture(Fl_Menu_* o, void* v)
{
	TraceUI* pUI = whoami(o);

	char* newfile = fl_file_chooser("Open texture?", "*.bmp", NULL);
	if (newfile != NULL)
	{
		int width, height;
		unsigned char* data = readBMP(newfile, width, height);
		if (data != NULL)
			pUI->m_textureData = data;
		else
			fl_alert("Texture loading error!");

		pUI->texture_height = height;
		pUI->texture_width = width;
	}//std::cout << "aaa" << std::endl;
}

void TraceUI::cb_load_normal(Fl_Menu_* o, void* v)
{
	TraceUI* pUI = whoami(o);

	char* newfile = fl_file_chooser("Open normal?", "*.bmp", NULL);
	if (newfile != NULL)
	{
		int width, height;
		unsigned char* data = readBMP(newfile, width, height);
		if (data != NULL)
			pUI->m_NormalData = data;
		else
			fl_alert("normal loading error!");

		pUI->normal_height = height;
		pUI->normal_width = width;
	}//std::cout << "aaa" << std::endl;
}

void TraceUI::cb_exit(Fl_Menu_* o, void* v)
{
	TraceUI* pUI=whoami(o);

	// terminate the rendering
	done=true;

	pUI->m_traceGlWindow->hide();
	pUI->m_mainWindow->hide();
}

void TraceUI::cb_exit2(Fl_Widget* o, void* v) 
{
	TraceUI* pUI=(TraceUI *)(o->user_data());
	
	// terminate the rendering
	done=true;

	pUI->m_traceGlWindow->hide();
	pUI->m_mainWindow->hide();
}

void TraceUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("RayTracer Project, FLTK version for CS 341 Spring 2002. Latest modifications by Jeff Maurer, jmaurer@cs.washington.edu");
}

void TraceUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	TraceUI* pUI=(TraceUI*)(o->user_data());
	
	pUI->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
	int	height = (int)(pUI->m_nSize / pUI->raytracer->aspectRatio() + 0.5);
	pUI->m_traceGlWindow->resizeWindow( pUI->m_nSize, height );
}

void TraceUI::cb_depthSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nDepth=int( ((Fl_Slider *)o)->value() ) ;
}

void TraceUI::cb_attenuationConstant(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_attConstant = double(((Fl_Slider*)o)->value());
}

void TraceUI::cb_attenuationLinear(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_attLinear = double(((Fl_Slider*)o)->value());
}

void TraceUI::cb_attenuationQuatric(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_attQuatric = double(((Fl_Slider*)o)->value());
}

void TraceUI::cb_ambient(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_ambient = double(((Fl_Slider*)o)->value());
}

void TraceUI::cb_threshold(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_threshold = double(((Fl_Slider*)o)->value());
}

void TraceUI::cb_supersampling(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_supersampling = int(((Fl_Slider*)o)->value());
}

void TraceUI::cb_jittering(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_jittering = int(((Fl_Slider*)o)->value());
}

void TraceUI::cb_background(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_background = int(((Fl_Slider*)o)->value());
}

void TraceUI::cb_texture(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_texture = int(((Fl_Slider*)o)->value());
}

void TraceUI::cb_soft(Fl_Widget* o, void* v) {
	((TraceUI*)(o->user_data()))->m_SoftShadow = bool(((Fl_Check_Button*)o)->value());
}

void TraceUI::cb_dof(Fl_Widget* o, void* v) {
	((TraceUI*)(o->user_data()))->m_DOF = bool(((Fl_Check_Button*)o)->value());
}
void TraceUI::cb_adapt(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_adapt = int(((Fl_Slider*)o)->value());
}

void TraceUI::cb_adaptDiff(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_adaptDiff = double(((Fl_Slider*)o)->value());
}

void TraceUI::cb_focalLength(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->focusLength = double(((Fl_Slider*)o)->value());
}

void TraceUI::cb_Apa(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->apa = double(((Fl_Slider*)o)->value());
}

void TraceUI::cb_glossy(Fl_Widget* o, void* v) {
	((TraceUI*)(o->user_data()))->m_Glossy = bool(((Fl_Check_Button*)o)->value());
}

void TraceUI::cb_render(Fl_Widget* o, void* v)
{
	char buffer[256];

	TraceUI* pUI=((TraceUI*)(o->user_data()));
	
	if (pUI->raytracer->sceneLoaded()) {
		int width=pUI->getSize();
		int	height = (int)(width / pUI->raytracer->aspectRatio() + 0.5);
		pUI->m_traceGlWindow->resizeWindow( width, height );

		pUI->m_traceGlWindow->show();

		pUI->raytracer->traceSetup(width, height);

		pUI->raytracer->bmp_height = pUI->bmp_height;
		pUI->raytracer->bmp_width = pUI->bmp_height;
		pUI->raytracer->m_backgroundData = pUI->m_backgroundData;

		pUI->raytracer->getScene()->texture_width = pUI->texture_width;
		pUI->raytracer->getScene()->texture_height = pUI->texture_height;
		pUI->raytracer->getScene()->m_textureData = pUI->m_textureData;
		pUI->raytracer->getScene()->m_NormalData = pUI->m_NormalData;

		pUI->raytracer->getScene()->m_nDepth = pUI->m_nDepth;
		pUI->raytracer->getScene()->m_attConstant = pUI->m_attConstant;
		pUI->raytracer->getScene()->m_attLinear = pUI->m_attLinear;
		pUI->raytracer->getScene()->m_attQuatric = pUI->m_attQuatric;
		pUI->raytracer->getScene()->m_supersampling = pUI->m_supersampling;
		pUI->raytracer->getScene()->m_background = pUI->m_background;
		pUI->raytracer->getScene()->m_texture = pUI->m_texture;
		pUI->raytracer->getScene()->m_adapt = pUI->m_adapt;
		pUI->raytracer->getScene()->m_adaptDiff = pUI->m_adaptDiff;
		pUI->raytracer->getScene()->m_ambient = vec3f(pUI->m_ambient, pUI->m_ambient, pUI->m_ambient);
		pUI->raytracer->getScene()->m_threshold = vec3f(pUI->m_threshold, pUI->m_threshold, pUI->m_threshold);
		
		// Save the window label
		const char *old_label = pUI->m_traceGlWindow->label();

		// start to render here	
		done=false;
		clock_t prev, now;
		prev=clock();
		
		pUI->m_traceGlWindow->refresh();
		Fl::check();
		Fl::flush();

		for (int y=0; y<height; y++) {
			for (int x=0; x<width; x++) {
				if (done) break;
				
				// current time
				now = clock();

				// check event every 1/2 second
				if (((double)(now-prev)/CLOCKS_PER_SEC)>0.5) {
					prev=now;

					if (Fl::ready()) {
						// refresh
						pUI->m_traceGlWindow->refresh();
						// check event
						Fl::check();

						if (Fl::damage()) {
							Fl::flush();
						}
					}
				}

				pUI->raytracer->tracePixel( x, y );
		
			}
			if (done) break;

			// flush when finish a row
			if (Fl::ready()) {
				// refresh
				pUI->m_traceGlWindow->refresh();

				if (Fl::damage()) {
					Fl::flush();
				}
			}
			// update the window label
			sprintf(buffer, "(%d%%) %s", (int)((double)y / (double)height * 100.0), old_label);
			pUI->m_traceGlWindow->label(buffer);
			
		}
		done=true;
		pUI->m_traceGlWindow->refresh();

		// Restore the window label
		pUI->m_traceGlWindow->label(old_label);		
	}
}

void TraceUI::cb_stop(Fl_Widget* o, void* v)
{
	done=true;
}

void TraceUI::show()
{
	m_mainWindow->show();
}

void TraceUI::setRayTracer(RayTracer *tracer)
{
	raytracer = tracer;
	m_traceGlWindow->setRayTracer(tracer);
}

int TraceUI::getSize()
{
	return m_nSize;
}

int TraceUI::getDepth()
{
	return m_nDepth;
}

// menu definition
Fl_Menu_Item TraceUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Scene...",	FL_ALT + 'l', (Fl_Callback *)TraceUI::cb_load_scene },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)TraceUI::cb_save_image },
		{ "&Load Background...",	FL_ALT + 'b', (Fl_Callback*)TraceUI::cb_load_background },
		{ "&Load Texture...",	FL_ALT + 't', (Fl_Callback*)TraceUI::cb_load_texture },
		{ "&Load Texture...",	FL_ALT + 't', (Fl_Callback*)TraceUI::cb_load_normal },
		{ "&Exit",			FL_ALT + 'e', (Fl_Callback *)TraceUI::cb_exit },
		{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)TraceUI::cb_about },
		{ 0 },

	{ 0 }
};

TraceUI::TraceUI() {
	// init.
	m_nDepth = 0;
	m_nSize = 150;
	m_mainWindow = new Fl_Window(100, 40, 360, 500, "Ray <Not Loaded>");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 360, 25);
		m_menubar->menu(menuitems);

		// install slider depth
		m_depthSlider = new Fl_Value_Slider(10, 30, 180, 20, "Depth");
		m_depthSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_depthSlider->type(FL_HOR_NICE_SLIDER);
        m_depthSlider->labelfont(FL_COURIER);
        m_depthSlider->labelsize(12);
		m_depthSlider->minimum(0);
		m_depthSlider->maximum(10);
		m_depthSlider->step(1);
		m_depthSlider->value(m_nDepth);
		m_depthSlider->align(FL_ALIGN_RIGHT);
		m_depthSlider->callback(cb_depthSlides);

		// install slider size
		m_sizeSlider = new Fl_Value_Slider(10, 55, 180, 20, "Size");
		m_sizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_sizeSlider->type(FL_HOR_NICE_SLIDER);
        m_sizeSlider->labelfont(FL_COURIER);
        m_sizeSlider->labelsize(12);
		m_sizeSlider->minimum(64);
		m_sizeSlider->maximum(512);
		m_sizeSlider->step(1);
		m_sizeSlider->value(m_nSize);
		m_sizeSlider->align(FL_ALIGN_RIGHT);
		m_sizeSlider->callback(cb_sizeSlides);

		m_attConstant_ = new Fl_Value_Slider(10, 80, 180, 20, "Attenuation, Constant");
		m_attConstant_->user_data((void*)(this));	// record self to be used by static callback functions
		m_attConstant_->type(FL_HOR_NICE_SLIDER);
		m_attConstant_->labelfont(FL_COURIER);
		m_attConstant_->labelsize(12);
		m_attConstant_->minimum(0.00);
		m_attConstant_->maximum(1.00);
		m_attConstant_->step(0.001);
		m_attConstant_->value(1);
		m_attConstant_->align(FL_ALIGN_RIGHT);
		m_attConstant_->callback(cb_attenuationConstant);

		m_attLinear_ = new Fl_Value_Slider(10, 105, 180, 20, "Attenuation, Linear");
		m_attLinear_->user_data((void*)(this));	// record self to be used by static callback functions
		m_attLinear_->type(FL_HOR_NICE_SLIDER);
		m_attLinear_->labelfont(FL_COURIER);
		m_attLinear_->labelsize(12);
		m_attLinear_->minimum(0.00);
		m_attLinear_->maximum(1.00);
		m_attLinear_->step(0.001);
		m_attLinear_->value(0.2);
		m_attLinear_->align(FL_ALIGN_RIGHT);
		m_attLinear_->callback(cb_attenuationLinear);

		m_attQuatric_ = new Fl_Value_Slider(10, 130, 180, 20, "Attenuation, Quatric");
		m_attQuatric_->user_data((void*)(this));	// record self to be used by static callback functions
		m_attQuatric_->type(FL_HOR_NICE_SLIDER);
		m_attQuatric_->labelfont(FL_COURIER);
		m_attQuatric_->labelsize(12);
		m_attQuatric_->minimum(0.00);
		m_attQuatric_->maximum(1.00);
		m_attQuatric_->step(0.1);
		m_attQuatric_->value();
		m_attQuatric_->align(FL_ALIGN_RIGHT);
		m_attQuatric_->callback(cb_attenuationQuatric);

		m_ambient_ = new Fl_Value_Slider(10, 155, 180, 20, "Ambient Light");
		m_ambient_->user_data((void*)(this));	// record self to be used by static callback functions
		m_ambient_->type(FL_HOR_NICE_SLIDER);
		m_ambient_->labelfont(FL_COURIER);
		m_ambient_->labelsize(12);
		m_ambient_->minimum(0.00);
		m_ambient_->maximum(1.00);
		m_ambient_->step(0.01);
		m_ambient_->value(1);
		m_ambient_->align(FL_ALIGN_RIGHT);
		m_ambient_->callback(cb_ambient);

		m_threshold_ = new Fl_Value_Slider(10, 180, 180, 20, "Threshold");
		m_threshold_->user_data((void*)(this));	// record self to be used by static callback functions
		m_threshold_->type(FL_HOR_NICE_SLIDER);
		m_threshold_->labelfont(FL_COURIER);
		m_threshold_->labelsize(12);
		m_threshold_->minimum(0.00);
		m_threshold_->maximum(1.00);
		m_threshold_->step(0.01);
		m_threshold_->value(1);
		m_threshold_->align(FL_ALIGN_RIGHT);
		m_threshold_->callback(cb_threshold);

		m_supersampling_ = new Fl_Value_Slider(10, 205, 180, 20, "Num of sup-pixel");
		m_supersampling_->user_data((void*)(this));	// record self to be used by static callback functions
		m_supersampling_->type(FL_HOR_NICE_SLIDER);
		m_supersampling_->labelfont(FL_COURIER);
		m_supersampling_->labelsize(12);
		m_supersampling_->minimum(1);
		m_supersampling_->maximum(5);
		m_supersampling_->step(1);
		m_supersampling_->value(1);
		m_supersampling_->align(FL_ALIGN_RIGHT);
		m_supersampling_->callback(cb_supersampling);

		m_jittering_ = new Fl_Value_Slider(10, 230, 180, 20, "Jittering");
		m_jittering_->user_data((void*)(this));	// record self to be used by static callback functions
		m_jittering_->type(FL_HOR_NICE_SLIDER);
		m_jittering_->labelfont(FL_COURIER);
		m_jittering_->labelsize(12);
		m_jittering_->minimum(0);
		m_jittering_->maximum(1);
		m_jittering_->step(1);
		m_jittering_->value(0);
		m_jittering_->align(FL_ALIGN_RIGHT);
		m_jittering_->callback(cb_jittering);

		m_background_ = new Fl_Value_Slider(10, 255, 180, 20, "Background");
		m_background_->user_data((void*)(this));	// record self to be used by static callback functions
		m_background_->type(FL_HOR_NICE_SLIDER);
		m_background_->labelfont(FL_COURIER);
		m_background_->labelsize(12);
		m_background_->minimum(0);
		m_background_->maximum(1);
		m_background_->step(1);
		m_background_->value(0);
		m_background_->align(FL_ALIGN_RIGHT);
		m_background_->callback(cb_background);

		m_texture_ = new Fl_Value_Slider(10, 280, 180, 20, "Texture");
		m_texture_->user_data((void*)(this));	// record self to be used by static callback functions
		m_texture_->type(FL_HOR_NICE_SLIDER);
		m_texture_->labelfont(FL_COURIER);
		m_texture_->labelsize(12);
		m_texture_->minimum(0);
		m_texture_->maximum(1);
		m_texture_->step(1);
		m_texture_->value(0);
		m_texture_->align(FL_ALIGN_RIGHT);
		m_texture_->callback(cb_texture);

		m_adapt_ = new Fl_Value_Slider(10, 305, 180, 20, "Adapt Supper");
		m_adapt_->user_data((void*)(this));	// record self to be used by static callback functions
		m_adapt_->type(FL_HOR_NICE_SLIDER);
		m_adapt_->labelfont(FL_COURIER);
		m_adapt_->labelsize(12);
		m_adapt_->minimum(0);
		m_adapt_->maximum(1);
		m_adapt_->step(1);
		m_adapt_->value(0);
		m_adapt_->align(FL_ALIGN_RIGHT);
		m_adapt_->callback(cb_adapt);

		m_adaptDiff_ = new Fl_Value_Slider(10, 330, 180, 20, "Adapt Supper Diff");
		m_adaptDiff_->user_data((void*)(this));	// record self to be used by static callback functions
		m_adaptDiff_->type(FL_HOR_NICE_SLIDER);
		m_adaptDiff_->labelfont(FL_COURIER);
		m_adaptDiff_->labelsize(12);
		m_adaptDiff_->minimum(0.01);
		m_adaptDiff_->maximum(1);
		m_adaptDiff_->step(0.01);
		m_adaptDiff_->value(0.5);
		m_adaptDiff_->align(FL_ALIGN_RIGHT);
		m_adaptDiff_->callback(cb_adaptDiff);

		m_glossyButton = new Fl_Check_Button(10, 355, 70, 25, "&glossy ");
		m_glossyButton->value(m_Glossy);
		m_glossyButton->callback(cb_glossy);
		m_glossyButton->user_data((void*)(this));
		//m_glossyButton->callback(cb_render);
		m_SSButton = new Fl_Check_Button(90, 355, 70, 25, "&Soft Shadow ");
		m_SSButton->value(m_SoftShadow);
		m_SSButton->callback(cb_soft);
		m_SSButton->user_data((void*)(this));

		m_DOFButton = new Fl_Check_Button(210, 355, 70, 25, "&DOF ");
		m_DOFButton->value(m_DOF);
		m_DOFButton->callback(cb_dof);
		m_DOFButton->user_data((void*)(this));

		m_focalLength_ = new Fl_Value_Slider(10, 380, 180, 20, "Focal Length");
		m_focalLength_->user_data((void*)(this));	// record self to be used by static callback functions
		m_focalLength_->type(FL_HOR_NICE_SLIDER);
		m_focalLength_->labelfont(FL_COURIER);
		m_focalLength_->labelsize(12);
		m_focalLength_->minimum(0.01);
		m_focalLength_->maximum(5.0);
		m_focalLength_->step(0.01);
		m_focalLength_->value(3);
		m_focalLength_->align(FL_ALIGN_RIGHT);
		m_focalLength_->callback(cb_focalLength);

		m_Apa_ = new Fl_Value_Slider(10, 400, 180, 20, "Aperture");
		m_Apa_->user_data((void*)(this));	// record self to be used by static callback functions
		m_Apa_->type(FL_HOR_NICE_SLIDER);
		m_Apa_->labelfont(FL_COURIER);
		m_Apa_->labelsize(12);
		m_Apa_->minimum(0.1);
		m_Apa_->maximum(5.0);
		m_Apa_->step(0.1);
		m_Apa_->value(1);
		m_Apa_->align(FL_ALIGN_RIGHT);
		m_Apa_->callback(cb_Apa);


		m_renderButton = new Fl_Button(240, 27, 70, 25, "&Render");
		m_renderButton->user_data((void*)(this));
		m_renderButton->callback(cb_render);

		m_stopButton = new Fl_Button(240, 55, 70, 25, "&Stop");
		m_stopButton->user_data((void*)(this));
		m_stopButton->callback(cb_stop);

		m_mainWindow->callback(cb_exit2);
		m_mainWindow->when(FL_HIDE);
    m_mainWindow->end();

	// image view
	m_traceGlWindow = new TraceGLWindow(100, 150, m_nSize, m_nSize, "Rendered Image");
	m_traceGlWindow->end();
	m_traceGlWindow->resizable(m_traceGlWindow);
}