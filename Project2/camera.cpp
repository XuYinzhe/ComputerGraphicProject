#include <windows.h>
#include <Fl/gl.h>
#include <gl/glu.h>
#include "modelerglobals.h"
#include "modelerapp.h"
#include "camera.h"
#include "sizeRecorder.h"

#pragma warning(push)
#pragma warning(disable : 4244)

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif 

const float kMouseRotationSensitivity		= 1.0f/90.0f;
const float kMouseTranslationXSensitivity	= 0.03f;
const float kMouseTranslationYSensitivity	= 0.03f;
const float kMouseZoomSensitivity			= 0.08f;

void MakeDiagonal(Mat4f &m, float k)
{
	register int i,j;

	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			m[i][j] = (i==j) ? k : 0.0f;
}

void MakeHScale(Mat4f &m, const Vec3f &s)	
{
	MakeDiagonal(m,1.0f);
	m[0][0] = s[0]; m[1][1] = s[1];	m[2][2] = s[2];
}

void MakeHTrans(Mat4f &m, const Vec3f &s)
{
	MakeDiagonal(m,1.0f);
	m[0][3] = s[0]; m[1][3] = s[1]; m[2][3] = s[2];
}

void MakeHRotX(Mat4f &m, float theta)
{
	MakeDiagonal(m,1.0f);
	float cosTheta = cos(theta);
	float sinTheta = sin(theta);
	m[1][1] = cosTheta;
	m[1][2] = -sinTheta;
	m[2][1] = sinTheta;
	m[2][2] = cosTheta;
}

void MakeHRotY(Mat4f &m, float theta)
{
	MakeDiagonal(m,1.0f);
	float cosTheta = cos(theta);
	float sinTheta = sin(theta);
	m[0][0] = cosTheta;
	m[2][0] = -sinTheta;
	m[0][2] = sinTheta;
	m[2][2] = cosTheta;
}

void MakeHRotZ(Mat4f &m, float theta)
{
	MakeDiagonal(m,1.0f);
	float cosTheta = cos(theta);
	float sinTheta = sin(theta);
	m[0][0] = cosTheta;
	m[0][1] = -sinTheta;
	m[1][0] = sinTheta;
	m[1][1] = cosTheta;
}


void Camera::calculateViewingTransformParameters() 
{
	Mat4f dollyXform;
	Mat4f azimXform;
	Mat4f elevXform;
	Mat4f twistXform;
	Mat4f originXform;

	Vec3f upVector;

	MakeHTrans(dollyXform, Vec3f(0,0,mDolly));
	MakeHRotY(azimXform, mAzimuth);
	MakeHRotX(elevXform, mElevation);
	MakeDiagonal(twistXform, 1.0f);
	MakeHTrans(originXform, mLookAt);
	
	mPosition = Vec3f(0,0,0);
	// grouped for (mat4 * vec3) ops instead of (mat4 * mat4) ops
	mPosition = originXform * (azimXform * (elevXform * (dollyXform * mPosition)));

	if ( fmod((double)mElevation, 2.0*M_PI) < 3*M_PI/2 && fmod((double)mElevation, 2.0*M_PI) > M_PI/2 )
		mUpVector= Vec3f(0,-1,0);
	else
		mUpVector= Vec3f(0,1,0);

	//mUpVector = Vec3f()
	//	VAL(CAM_ANG);

	mDirtyTransform = false;
}

Camera::Camera() 
{
	mElevation = mAzimuth = mTwist = 0.0f;
	mDolly = -20.0f;
	mElevation = 0.2f;
	mAzimuth = (float)M_PI;

	mLookAt = Vec3f( 0, 0, 0 );
	mCurrentMouseAction = kActionNone;

	calculateViewingTransformParameters();
}

void Camera::clickMouse( MouseAction_t action, int x, int y )
{
	mCurrentMouseAction = action;
	mLastMousePosition[0] = x;
	mLastMousePosition[1] = y;
}

void Camera::dragMouse( int x, int y )
{
	
	double tanVal = -(atan2(y - screenHeight/2, x - screenWidth/2) - atan2(mLastMousePosition[1] - screenHeight / 2, mLastMousePosition[0] - screenWidth / 2));
	Vec3f mouseDelta   = Vec3f(x,y,0.0f) - mLastMousePosition;
	mLastMousePosition = Vec3f(x,y,0.0f);

	switch(mCurrentMouseAction)
	{
	case kActionTranslate:
		{
			calculateViewingTransformParameters();

			double xTrack =  -mouseDelta[0] * kMouseTranslationXSensitivity;
			double yTrack =  mouseDelta[1] * kMouseTranslationYSensitivity;

			Vec3f transXAxis = mUpVector ^ (mPosition - mLookAt);
			transXAxis /= sqrt((transXAxis*transXAxis));
			Vec3f transYAxis = (mPosition - mLookAt) ^ transXAxis;
			transYAxis /= sqrt((transYAxis*transYAxis));

			setLookAt(getLookAt() + transXAxis*xTrack + transYAxis*yTrack);
			
			break;
		}
	case kActionRotate:
		{
			float dAzimuth =   -mouseDelta[0] * kMouseRotationSensitivity;
			float dElevation	=   mouseDelta[1] * kMouseRotationSensitivity;
			
			setAzimuth(getAzimuth() + dAzimuth);
			setElevation(getElevation() + dElevation);
			
			break;
		}
	case kActionZoom:
		{
			float dDolly = -mouseDelta[1] * kMouseZoomSensitivity;
			setDolly(getDolly() + dDolly);
			break;
		}
	case kActionTwist: {
		// Not implemented
		printf("%f \n", ModelerApplication::Instance()->GetControlValue(CAM_ANG));
		ModelerApplication::Instance()->SetControlValue(CAM_ANG, ModelerApplication::Instance()->GetControlValue(CAM_ANG)+ (tanVal * 180 / M_PI));
	}

	default:
		break;
	}

}

void Camera::releaseMouse( int x, int y )
{
	mCurrentMouseAction = kActionNone;
}


void Camera::applyViewingTransform() {
	if( mDirtyTransform )
		calculateViewingTransformParameters();

	// Place the camera at mPosition, aim the camera at
	// mLookAt, and twist the camera such that mUpVector is up
	/*gluLookAt(	mPosition[0], mPosition[1], mPosition[2],
				mLookAt[0],   mLookAt[1],   mLookAt[2],
				mUpVector[0], mUpVector[1], mUpVector[2]);*/

	double* temp = SizeRecorder::getInstance()->minCoord;

	//printf("%f, %f, %f \n", temp[0], temp[1], temp[2]);

	lookAt(mPosition, mLookAt, mUpVector);
	/*printf("UP: %f %f %f \n", mUpVector[0], mUpVector[1], mUpVector[2]);
	printf("AT: %f %f %f \n", mLookAt[0], mLookAt[1], mLookAt[2]);
	printf("PO: %f %f %f \n\n", mPosition[0], mPosition[1], mPosition[2]);*/
}

void Camera::lookAt(Vec3f eye, Vec3f at, Vec3f up)
{
	// Z unit vector
	const Vec3f Zaxis = { 0,0,1 };

	// make at the lookAt direction vector
	at = at - eye;
	at.normalize();

	//at vector project to xz plane
	Vec3f atNoy = { at[0],0,at[2] };
	atNoy.normalize();


	glRotatef(VAL(CAM_ANG), 0, 0, 1);

	//up back to +y

		//angle of camera looking up+ or down- (angle of attack)
	float upDownAngle = -1 * 180 / M_PI * asin(at[1] / at.length() );
	glRotatef(upDownAngle + (up[1] > 0 ? 0 : 180), 1 * up[1],0, 0);






	//printf("up to +y: %f \n", toprotateangle);



	// at back to z-
	float temp = (atNoy * -Zaxis) / (atNoy.length() * Zaxis.length());
	float verticalrotasteangle = 180 / M_PI * acos( temp ) + (up[1] > 0 ? 0 : -180)/*this fix flip problem when aoa > 90*/;
	glRotatef(verticalrotasteangle,  0, at[2] * -Zaxis[0] - at[0] * -Zaxis[2]/*get the direction of rotate*/, 0);

	//printf("at to z-: %f \n", verticalrotasteangle);

	//simple move
	glTranslatef(-eye[0], -eye[1], -eye[2]);


	//printf("U/D: %f\n", toprotateangle);
	//printf("L/R: %f \n\n", verticalrotasteangle);

}

#pragma warning(pop)