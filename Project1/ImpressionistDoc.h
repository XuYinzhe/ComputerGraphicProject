// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"
#include "KernalBruah.h"


class ImpressionistUI;

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *iname);
	int reloadForVideo();
	// called by the UI to load image
	int 	loadDissolveImage(char* iname);
	int		loadAnotherImage(char* iname);
	int swapWithAnother();
	int		saveImage(char *iname);			// called by the UI to save image


	int     clearCanvas();                  // called by the UI to clear the drawing canvas
	void	setBrushType(int type);			// called by the UI to set the brushType
	void	setDirectionType(int type);
	int		getSize();						// get the UI size
	int getWidth();
	int getAngle();
	void	setSize(int size);				// set the UI size
	char*	getImageName();					// get the current image name

	KernalBruah* currentKernal;

	std::string workingPath;
	
	void UndoStep();

// Attributes
public:
	//indecator of current image
	bool isOriginal;


	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;

	// Dimensions of another image.
	int				m_nWidth2,
					m_nHeight2;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	
	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;
	unsigned char*	m_ucOriginalCopy;
	unsigned char*	m_ucOriginalCopy2;
	unsigned char*	m_ucDisplayCopy;
	unsigned char*	m_ucPainting;
	unsigned char*  m_ucSwapCache;
	unsigned char*	m_ucLastStep;

	// Bitmap for dissolve image
	unsigned char*	m_ucDissolveImage;


	// The current active brush.
	ImpBrush*			m_pCurrentBrush;	
	// Size of the brush.
	int m_nSize;	
	//int m_LineWidth;
	//int m_LineAngle;

	ImpressionistUI*	m_pUI;


// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );
	GLubyte* GetFileCopyPixel2(int x, int y);
	GLubyte* GetDisplayImgPixel(int x, int y);
	GLubyte* GetFileCopyPixel(int x, int y);
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );  

	void SwapPaintviewWithOriginal();


private:
	char			m_imageName[256];

};

extern void MessageBox(char *message);

#endif
