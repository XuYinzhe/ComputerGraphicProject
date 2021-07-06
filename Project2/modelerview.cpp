#include <gl/glew.h>
#include "modelerview.h"
#include "camera.h"
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.h>
#include <FL/gl.h>
#include <GL/glu.h>
#include <cstdio>
#include "modelerapp.h"
#include "modelerglobals.h"

static const int	kMouseRotationButton			= FL_LEFT_MOUSE;
static const int	kMouseTranslationButton			= FL_MIDDLE_MOUSE;
static const int	kMouseZoomButton				= FL_RIGHT_MOUSE;

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

ModelerView::ModelerView(int x, int y, int w, int h, char *label)
: Fl_Gl_Window(x,y,w,h,label)
{
    m_camera = new Camera();
}

ModelerView::~ModelerView()
{
	delete m_camera;
}
int ModelerView::handle(int event)
{
	m_camera->screenWidth = w();
	m_camera->screenHeight = h();

	

#ifndef __APPLE__
	static int first = 1;
	if (first && event == FL_SHOW && shown()) {
		first = 0;
		make_current();
		glewInit(); // defines pters to functions of OpenGL V 1.2 and above
	}
#endif

    unsigned eventCoordX = Fl::event_x();
	unsigned eventCoordY = Fl::event_y();
	unsigned eventButton = Fl::event_button();
	unsigned eventState  = Fl::event_state();
	/*printf((isLMB ?  "true,":"false,"));
	printf((isRMB ? "true\n" : "false\n"));*/
	switch(event)	 
	{
	case FL_PUSH:
		{

				switch(eventButton)
				{
				case kMouseRotationButton:
					m_camera->clickMouse(kActionRotate, eventCoordX, eventCoordY );
					isLMB = true;
					break;
				case kMouseTranslationButton:
					m_camera->clickMouse(kActionTranslate, eventCoordX, eventCoordY );
					break;
				case kMouseZoomButton:
					m_camera->clickMouse(kActionZoom, eventCoordX, eventCoordY );
					isRMB = true;
					break;
	
				}
				if (isRMB && isLMB) {

					m_camera->clickMouse(kActionTwist, eventCoordX, eventCoordY);


				}

           // printf("push %d %d\n", eventCoordX, eventCoordY);
		}
		break;
	case FL_DRAG:
		{
			m_camera->dragMouse(eventCoordX, eventCoordY);
            //printf("drag %d %d\n", eventCoordX, eventCoordY);
		}
		break;
	case FL_RELEASE:
		{
			switch(eventButton)
			{
			case kMouseRotationButton:
				m_camera->releaseMouse(eventCoordX, eventCoordY);
				isLMB = false;
				break;
			case kMouseTranslationButton:
				m_camera->releaseMouse(eventCoordX, eventCoordY);

				break;
			case kMouseZoomButton:
				m_camera->releaseMouse(eventCoordX, eventCoordY );
				isRMB = false;
				break;
			}
          //  printf("release %d %d\n", eventCoordX, eventCoordY);
		}
		break;
	default:
		return 0;
	}
	
	redraw();

	return 1;
}

static GLfloat lightPosition0[] = { 4, 2, -4, 0 };
static GLfloat lightDiffuse0[]  = { 1,1,1,1 };
static GLfloat lightPosition1[] = { -2, 1, 5, 0 };
static GLfloat lightDiffuse1[]  = { 1, 1, 1, 1 };

void ModelerView::draw()
{
    if (!valid())
    {
        glShadeModel( GL_SMOOTH );
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_LIGHTING );
		glEnable( GL_LIGHT0 );
        glEnable( GL_LIGHT1 );
		glEnable( GL_NORMALIZE );
    }

  	glViewport( 0, 0, w(), h() );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0,float(w())/float(h()),1.0,100000.0);
				
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_camera->applyViewingTransform();

	lightPosition0[0] = VAL(LIGHT_X);
	lightPosition0[1] = VAL(LIGHT_Y);
	lightPosition0[2] = VAL(LIGHT_Z);

    glLightfv( GL_LIGHT0, GL_POSITION, lightPosition0 );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, lightDiffuse0 );
    glLightfv( GL_LIGHT1, GL_POSITION, lightPosition1 );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse1 );
}