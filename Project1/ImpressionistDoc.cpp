// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "ImpBrush.h"

// Include individual brush headers here.
#include "PointBrush.h"
#include "LineBrush.h"
#include "CircleBrush.h"
#include "ScatteredPointBrush.h"
#include "ScatteredLineBrush.h"
#include "ScatteredCircleBrush.h"
#include "GaussianBlur.h"
#include "StarBrush.h"
#include <direct.h>
#include <ctime>


#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucBitmap		= NULL;
	m_ucPainting	= NULL;
	
	m_ucDisplayCopy = NULL;
	m_ucSwapCache = NULL;
	m_ucOriginalCopy = NULL;
	m_ucOriginalCopy2 = NULL;
	m_ucLastStep = NULL;

	m_ucDissolveImage = NULL;

	currentKernal = NULL;

	char path[MAX_PATH];
	workingPath = getcwd(path, MAX_PATH);

	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES]				
		= new LineBrush( this, "Lines" );
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]				
		= new CircleBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]	
		= new ScatteredPointBrush( this, "Scattered Points" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]		
		= new ScatteredLineBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]	
		= new ScatteredCircleBrush( this, "Scattered Circles" );
	ImpBrush::c_pBrushes[BRUSH_GAUSSIAN_BLUR]
		= new GaussianBlurBrush(this, "Gaussian Blur Brush");
	ImpBrush::c_pBrushes[BRUSH_STAR]
		= new StarBrush(this, "Star Brush");


	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];

}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}



//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
}

void ImpressionistDoc::setDirectionType(int type)
{
	m_pCurrentBrush->m_DirectionType = type;
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}
int ImpressionistDoc::getWidth()
{
	return m_pUI->getWidth();
}
int ImpressionistDoc::getAngle()
{
	return m_pUI->getAngle();
}

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=readBMP(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	isOriginal = true;

	// release old storage
	if ( m_ucBitmap ) delete [] m_ucBitmap;
	if ( m_ucOriginalCopy ) delete [] m_ucOriginalCopy;
	if ( m_ucOriginalCopy2 ) delete [] m_ucOriginalCopy2;
	if ( m_ucDisplayCopy ) delete [] m_ucDisplayCopy;
	if ( m_ucPainting ) delete [] m_ucPainting;
	if (m_ucLastStep) delete[] m_ucLastStep;

	m_ucBitmap		= data;

	// allocate space for draw view
	m_ucDisplayCopy = new unsigned char[width * height * 3];
	m_ucOriginalCopy = new unsigned char[width * height * 3];

	m_ucOriginalCopy2 = new unsigned char[width * height * 3];
	memset(m_ucOriginalCopy2, 0, width * height * 3);


	memcpy(m_ucDisplayCopy, m_ucBitmap, width * height * 3);
	memcpy(m_ucOriginalCopy, m_ucBitmap, width * height * 3);

	// allocate space for draw view
	m_ucPainting	= new unsigned char [width*height*3];
	m_ucLastStep = new unsigned char[width * height * 3];
	memset(m_ucPainting, 0, width*height*3);
	memset(m_ucLastStep, 0, width * height * 3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();


	return 1;
}

//int ImpressionistDoc::reloadForVideo(char* iname)
//{
//	// try to open the image to read
//	unsigned char* data;
//	int				width,
//		height;
//
//	if ((data = readBMP(iname, width, height)) == NULL)
//	{
//		fl_alert("Can't load bitmap file");
//		return 0;
//	}
//
//	// reflect the fact of loading the new image
//	m_nWidth = width;
//	m_nPaintWidth = width;
//	m_nHeight = height;
//	m_nPaintHeight = height;
//
//
//
//	// reflect the fact of loading the new image
//	m_nWidth = width;
//	m_nPaintWidth = width;
//	m_nHeight = height;
//	m_nPaintHeight = height;
//
//	isOriginal = true;
//
//	// release old storage
//	if (m_ucBitmap) delete[] m_ucBitmap;
//	if (m_ucOriginalCopy) delete[] m_ucOriginalCopy;
//	if (m_ucOriginalCopy2) delete[] m_ucOriginalCopy2;
//	if (m_ucDisplayCopy) delete[] m_ucDisplayCopy;
//	if (m_ucPainting) delete[] m_ucPainting;
//	if (m_ucLastStep) delete[] m_ucLastStep;
//
//	m_ucBitmap = new unsigned char[width * height * 3];
//
//	// allocate space for draw view
//	m_ucDisplayCopy = new unsigned char[width * height * 3];
//	m_ucOriginalCopy = new unsigned char[width * height * 3];
//
//	m_ucOriginalCopy2 = new unsigned char[width * height * 3];
//	memset(m_ucOriginalCopy2, 0, width * height * 3);
//	memset(m_ucBitmap, 0, width * height * 3);
//
//
//	memset(m_ucOriginalCopy, 0, width * height * 3);
//
//	// allocate space for draw view
//	m_ucPainting = new unsigned char[width * height * 3];
//	m_ucLastStep = new unsigned char[width * height * 3];
//	memset(m_ucPainting, 0, width * height * 3);
//	memset(m_ucLastStep, 0, width * height * 3);
//
//	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(),
//		m_pUI->m_mainWindow->y(),
//		width * 2,
//		height + 25);
//
//	// display it on origView
//	m_pUI->m_origView->resizeWindow(width, height);
//	m_pUI->m_origView->refresh();
//
//	// refresh paint view as well
//	m_pUI->m_paintView->resizeWindow(width, height);
//	m_pUI->m_paintView->refresh();
//
//
//	return 1;
//}
int ImpressionistDoc::loadDissolveImage(char* iname) {
	// try to open the image to read
	unsigned char* data;
	int				width,
		height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	if (m_ucDissolveImage) delete[] m_ucDissolveImage;

	m_ucDissolveImage = data;

	glPointSize(10.0f);

	GLubyte originalColor[3];
	GLubyte disolveColor[3];
	float alpha = m_pUI->getDissolveAlpha();
	for (int i = 0; i < min(height, m_nPaintHeight); i++) {
		for (int j = 0; j < min(width, m_nWidth); j++) {
			
			memcpy(disolveColor, (GLubyte*)(m_ucDissolveImage + 3 * (i * width + j)), 3);
			memcpy(originalColor,(GLubyte*)(m_ucPainting + 3 * (i * m_nWidth + j)),  3);
			GLubyte final[3] = { originalColor[0] * (1 - alpha) + disolveColor[0] * alpha, originalColor[1] * (1 - alpha) + disolveColor[1] * alpha, originalColor[2] * (1 - alpha) + disolveColor[2] * alpha, };
			memcpy((GLubyte*)(m_ucPainting + 3 * (i * m_nWidth + j)),final , 3);


		}

	}
	m_pUI->m_paintView->refresh();

}

int ImpressionistDoc::loadAnotherImage(char* iname) {
	// try to open the image to read
	unsigned char* data;
	int				width,
		height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	if (m_ucOriginalCopy2) delete[] m_ucOriginalCopy2;

	m_ucOriginalCopy2 = new unsigned char[m_nWidth * m_nHeight * 3];
	memset(m_ucOriginalCopy2, 0, m_nWidth * m_nHeight * 3);



	for (int i = 0; i < min(height, m_nHeight); i++) {
		for (int j = 0; j < min(width, m_nWidth); j++) {
			memcpy((GLubyte*)(m_ucOriginalCopy2 + 3 * (i * m_nWidth + j)), (GLubyte*)(data + 3 * (i * width + j)), 3);
		}
	}

	if (height != m_nHeight) {
		fl_alert("Size mismatch, file is clipped");
	}

}

int ImpressionistDoc::swapWithAnother() {
	if (m_ucOriginalCopy2) {
		unsigned char* temp = new unsigned char[m_nWidth * m_nHeight * 3];
		memcpy(temp, m_ucOriginalCopy, m_nWidth * m_nHeight * 3);
		m_ucOriginalCopy = m_ucOriginalCopy2;
		m_ucOriginalCopy2 = temp;
		return 0;
	}

}



//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) 
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;

}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}

GLubyte* ImpressionistDoc::GetFileCopyPixel2( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth2 ) 
		x = m_nWidth2-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight2 ) 
		y = m_nHeight2-1;

	return (GLubyte*)(m_ucOriginalCopy2 + 3 * (y*m_nWidth2 + x));
}

GLubyte* ImpressionistDoc::GetDisplayImgPixel(int x, int y)
{
	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return (GLubyte*)(m_ucDisplayCopy + 3 * (y * m_nWidth + x));
}

GLubyte* ImpressionistDoc::GetFileCopyPixel(int x, int y)
{
	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return (GLubyte*)(m_ucOriginalCopy + 3 * (y * m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}

void ImpressionistDoc::SwapPaintviewWithOriginal() {
	if (m_ucBitmap)
		if (m_ucSwapCache) {
			delete[] m_ucBitmap;
			m_ucBitmap = m_ucSwapCache;
			memcpy(m_ucOriginalCopy, m_ucBitmap, m_nPaintWidth * m_nPaintHeight * 3);
			m_ucSwapCache = NULL;
		}
		else {
			m_ucSwapCache = m_ucBitmap;
			m_ucBitmap = new unsigned char[m_nPaintWidth * m_nPaintHeight * 3];
			memcpy(m_ucBitmap, m_ucPainting, m_nPaintWidth * m_nPaintHeight * 3);
			memcpy(m_ucOriginalCopy, m_ucPainting, m_nPaintWidth * m_nPaintHeight * 3);
		}
	m_pUI->m_origView->refresh();
}



void ImpressionistDoc::UndoStep() {
	memcpy(m_ucPainting, m_ucLastStep, m_nWidth * m_nHeight * 3);
	m_pUI->m_paintView->redraw();
}

