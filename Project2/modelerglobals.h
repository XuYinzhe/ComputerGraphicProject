#ifndef _MODELER_GLOBALS_H
#define _MODELER_GLOBALS_H

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif

// This is a list of the controls for the SampleModel
// We'll use these constants to access the values 
// of the controls from the user interface.
/*
enum SampleModelControls
{ 
	XPOS, YPOS, ZPOS, HEIGHT, ROTATE, 

	LIGHT_X, LIGHT_Y, LIGHT_Z,


	NUMCONTROLS
};*/

enum MyModelControls
{
	XPOS, YPOS, ZPOS, 
	FRONT_BODY, BACK_BODY,
	LEG_LEFT1, LEG_LEFT2, LEG_LEFT3,
	LEG_RIGHT1, LEG_RIGHT2, LEG_RIGHT3,
	ARM_LEFT1, ARM_LEFT2_1, ARM_LEFT2_2,
	ARM_RIGHT1, ARM_RIGHT2_1, ARM_RIGHT2_2,
	JAW_LEFT, JAW_RIGHT,
	TAIL1, TAIL2, TAIL3, TAIL4, HOOK1, HOOK2,
	LIGHT_X, LIGHT_Y, LIGHT_Z,
	CAM_ANG,
	MOOD,
	TORUS, RADIUS, RADIUS_D, SECTIONS, FACES,

	NUMCONTROLS
};

// Colors
#define COLOR_RED		1.0f, 0.0f, 0.0f
#define COLOR_GREEN		0.0f, 1.0f, 0.0f
#define COLOR_BLUE		0.0f, 0.0f, 1.0f

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

#endif