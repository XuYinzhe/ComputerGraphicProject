#if defined(__APPLE__)
#  include <OpenGL/gl3.h> // defines OpenGL 3.0+ functions
#else
#  if defined(WIN32)
#    define GLEW_STATIC 1
#  endif
#  include <GL/glew.h>
#endif

#include "modelerview.h"
#include "camera.h"
#include "bitmap.h"
#include "modelerapp.h"
#include "particleSystem.h"

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.h>
#include <FL/gl.h>
#include <GL/glu.h>
#include <cstdio>

static const int	kMouseRotationButton			= FL_LEFT_MOUSE;
static const int	kMouseTranslationButton			= FL_MIDDLE_MOUSE;
static const int	kMouseZoomButton				= FL_RIGHT_MOUSE;

static const char *bmp_name = NULL;

const std::string DarkShader = R"(void main (void)
{gl_FragColor = (0.1,0.1,0.1,0.1);}
)";

const std::string PhongShader = R"(varying vec3 normal, eyeVec;
#define MAX_LIGHTS 8
#define NUM_LIGHTS 2
varying vec3 lightDir[MAX_LIGHTS];
void main (void)
{
  vec4 final_color = 
       gl_FrontLightModelProduct.sceneColor;
  vec3 N = normalize(normal);
  int i;
  for (i=0; i<NUM_LIGHTS; ++i)
  {  
    vec3 L = normalize(lightDir[i]);
    float lambertTerm = dot(N,L);
    if (lambertTerm > 0.0)
    {
      final_color += 
        gl_LightSource[i].diffuse * 
        gl_FrontMaterial.diffuse * 
        lambertTerm;	
      vec3 E = normalize(eyeVec);
      vec3 R = reflect(-L, N);
      float specular = pow(max(dot(R, E), 0.0), 
                           gl_FrontMaterial.shininess);
      final_color += 
        gl_LightSource[i].specular * 
        gl_FrontMaterial.specular * 
        specular;	
    }
  }
  gl_FragColor = final_color;			
})";

const std::string fragmentShader = R"(
varying vec3 normal, eyeVec;
#define MAX_LIGHTS 8
#define NUM_LIGHTS 1
varying vec3 lightDir[MAX_LIGHTS];
void main (void)
{
  vec4 final_color = 
       gl_FrontLightModelProduct.sceneColor;
  vec3 N = normalize(normal);
  int i;
  for (i=0; i<NUM_LIGHTS; ++i)
  {  
    vec3 L = normalize(lightDir[i]);
    float lambertTerm = dot(N,L);
//lambertTerm = max(lambertTerm,0.0);
    if (lambertTerm > 0.0)
    {
	float level = floor(lambertTerm * 4.0);
	lambertTerm = level / 4.0;
      final_color += 
        gl_LightSource[i].diffuse * 
        gl_FrontMaterial.diffuse * 
        lambertTerm;	
      vec3 E = normalize(eyeVec);
      vec3 R = reflect(-L, N);
      float specular = pow(max(dot(R, E), 0.0), 
                           gl_FrontMaterial.shininess);
      final_color += 
        gl_LightSource[i].specular * 
        gl_FrontMaterial.specular * 
        specular;	
    }
  }
  gl_FragColor = final_color;			
}
)";



const std::string vertexShader = R"(
varying vec3 normal, eyeVec;
#define MAX_LIGHTS 8
#define NUM_LIGHTS 1
varying vec3 lightDir[MAX_LIGHTS];
void main()
{	
  gl_Position = ftransform();		
  normal = gl_NormalMatrix * gl_Normal;
  vec4 vVertex = gl_ModelViewMatrix * gl_Vertex;
  eyeVec = -vVertex.xyz;
  int i;
  for (i=0; i<NUM_LIGHTS; ++i)
    lightDir[i] = 
      vec3((gl_LightSource[i].position).xyz - vVertex.xyz);
}
)";


void printShaderLog(GLuint shader)
{
	GLint shaderState;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderState);
	if (shaderState == GL_TRUE)
	{
		return;
	}
	GLsizei bufferSize = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufferSize);
	if (bufferSize > 0)
	{
		GLchar* buffer = new char[bufferSize];
		glGetShaderInfoLog(shader, bufferSize, NULL, buffer);
		printf("%s", buffer);
		delete[] buffer;
	}
}
//https ://blog.csdn.net/u010223072/article/details/80641050

ModelerView::ModelerView(int x, int y, int w, int h, char *label)
: Fl_Gl_Window(x,y,w,h,label), t(0), save_bmp(false) 
{
	m_ctrl_camera = new Camera();
	m_curve_camera = new Camera();
	camera(CURVE_MODE);



}

ModelerView::~ModelerView()
{
	delete m_ctrl_camera;
	delete m_curve_camera;
}
int ModelerView::handle(int event)
{
    unsigned eventCoordX = Fl::event_x();
	unsigned eventCoordY = Fl::event_y();
	unsigned eventButton = Fl::event_button();
	unsigned eventState  = Fl::event_state();

	switch(event)	 
	{
	case FL_PUSH:
		{
			switch(eventButton)
			{
			case kMouseRotationButton:
				if (!Fl::event_state(FL_ALT)) {
					m_camera->clickMouse(kActionRotate, eventCoordX, eventCoordY );
					break;
				} // ALT + LEFT = MIDDLE
			case kMouseTranslationButton:
				m_camera->clickMouse(kActionTranslate, eventCoordX, eventCoordY );
				break;
			case kMouseZoomButton:
				m_camera->clickMouse(kActionZoom, eventCoordX, eventCoordY );
				break;
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
			case kMouseTranslationButton:
			case kMouseZoomButton:
				m_camera->releaseMouse(eventCoordX, eventCoordY );
				break;
			}
          //  printf("release %d %d\n", eventCoordX, eventCoordY);
		}
		break;
	default:
		return Fl_Gl_Window::handle(event);
	}
	
	redraw();

	return 1;
}

static GLfloat lightPosition0[] = { 40, 10, 2, 0 };
static GLfloat lightDiffuse0[]  = { 1,1,1,1 };
static GLfloat lightAmb0[]  = { 0.21,0.21,0.21,0.21 };
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
		glEnable(GL_CULL_FACE);

		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			std::cerr << "Error: " << glewGetErrorString(err) << std::endl;

		}
		std::cerr << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
		
		const char* Fshaders = fragmentShader.c_str();
		const char* Vshaders = vertexShader.c_str();
		const char* Dshaders = DarkShader.c_str();
		const char* Phongshaders = PhongShader.c_str();

		FshaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(FshaderID, 1, &Fshaders, NULL);
		glCompileShader(FshaderID);
		cout<<"frag info"<<endl;
		printShaderLog(FshaderID);

		VshaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VshaderID, 1, &Vshaders, NULL);
		glCompileShader(VshaderID);
		cout << "Vert info" << endl;
		printShaderLog(VshaderID);

		DarkshaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(DarkshaderID, 1, &Dshaders, NULL);
		glCompileShader(DarkshaderID);
		cout << "Darkshader info" << endl;
		printShaderLog(DarkshaderID);

		PhonghaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(PhonghaderID, 1, &Phongshaders, NULL);
		glCompileShader(PhonghaderID);
		cout << "Phonghader info" << endl;
		printShaderLog(PhonghaderID);


		cellProgramID = glCreateProgram();
		glAttachShader(cellProgramID, FshaderID);
		glAttachShader(cellProgramID, VshaderID);
		glLinkProgram(cellProgramID);
		 
		darkProgramID = glCreateProgram();
		glAttachShader(darkProgramID, DarkshaderID);
		glAttachShader(darkProgramID, VshaderID);
		glLinkProgram(darkProgramID);

		defaultprogramID = glCreateProgram();
		glAttachShader(defaultprogramID, PhonghaderID);
		glAttachShader(defaultprogramID, VshaderID);
		glLinkProgram(defaultprogramID);
    }

	

  	glViewport( 0, 0, w(), h() );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0,float(w())/float(h()),1.0,100.0);
				
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_camera->applyViewingTransform();

	/*glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);*/

    glLightfv( GL_LIGHT0, GL_POSITION, lightPosition0 );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, lightDiffuse0 );
    glLightfv( GL_LIGHT0, GL_AMBIENT, lightAmb0);
    glLightfv( GL_LIGHT0, GL_SPECULAR, lightAmb0);
    glLightfv( GL_LIGHT1, GL_POSITION, lightPosition1 );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse1 );
	//glUniform1fv(glGetAttribLocation(programID, "lightPos"),3, lightPosition0);


	// If particle system exists, draw it
	ParticleSystem *ps = ModelerApplication::Instance()->GetParticleSystem();
	if (ps != NULL) {
		ps->computeForcesAndUpdateParticles(t);
		ps->drawParticles(t);
	}
}


/** Set the active camera **/
void ModelerView::camera(cam_mode_t mode)
{
	switch (mode) {
	case CTRL_MODE:
		m_camera = m_ctrl_camera;
		break;
	case CURVE_MODE:
		m_camera = m_curve_camera;
		break;
	}
}


/** Cleanup fxn for saving bitmaps **/
void ModelerView::endDraw()
{
	if ((bmp_name == NULL) || (!save_bmp)) return;
	glFinish();
	saveBMP(bmp_name);
	save_bmp = false;
}


void ModelerView::setBMP(const char *fname)
{
	save_bmp = true;
	bmp_name = fname;
}

void ModelerView::saveBMP(const char* szFileName)
{
	int xx = x();
	int yy = y();
	int ww = w();
	int hh = h();
	
	make_current();
	
	unsigned char *imageBuffer = new unsigned char[3 * ww * hh];
	
	glReadBuffer(GL_BACK);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ROW_LENGTH, ww);
    
    glReadPixels( 0, 0, ww, hh, 
		GL_RGB, GL_UNSIGNED_BYTE, 
		imageBuffer );
	
	writeBMP(szFileName, ww, hh, imageBuffer);
	
	delete [] imageBuffer;
}

