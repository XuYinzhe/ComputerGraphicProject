//
// originalview.cpp
//
// The code maintaining the original view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "ImpressionistUI.h"
#include "originalview.h"
#include "PaintView.h"

#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#endif

OriginalView::OriginalView(int			x, 
						   int			y, 
						   int			w, 
						   int			h, 
						   const char*	l)
							: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;

}

void OriginalView::draw()
{
	if(!valid())
	{
		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		// Tell openGL to read from the front buffer when capturing
		// out paint strokes 
		glReadBuffer( GL_FRONT );
		ortho();

	}

	glClear( GL_COLOR_BUFFER_BIT );

	if ( m_pDoc->m_ucBitmap ) 
	{
		// note that both OpenGL pixel storage and the Windows BMP format
		// store pixels left-to-right, BOTTOM-to-TOP!!  thus all the fiddling
		// around with startrow.

		m_nWindowWidth=w();
		m_nWindowHeight=h();

		int drawWidth, drawHeight;
		GLvoid* bitstart;

		// we are not using a scrollable window, so ignore it
		Point scrollpos;	// = GetScrollPosition();
		scrollpos.x=scrollpos.y=0;

		drawWidth	= min( m_nWindowWidth, m_pDoc->m_nWidth );
		drawHeight	= min( m_nWindowHeight, m_pDoc->m_nHeight );

		int	startrow	= m_pDoc->m_nHeight - (scrollpos.y + drawHeight);
		if ( startrow < 0 ) 
			startrow = 0;

		{


			int brushSize = m_pDoc->getSize();
			GLubyte color[3] = { 255,0,0 };
			Point mousePoint = PaintView::getCurrentMOusePos();
			//Point* disp = new Point(max(min(drawWidth - brushSize / 2, mousePoint.x), brushSize / 2), drawHeight - max(brushSize / 2, min(drawHeight - brushSize / 2, mousePoint.y)));
			Point* disp = new Point(mousePoint.x, drawHeight - mousePoint.y);
			//printf("%d,%d \n", disp->x, disp->y);

		//Scale color
			for (int i = 0; i < drawWidth; i++) {
				for (int j = 0; j < drawHeight; j++) {
					GLubyte* beforeColor = m_pDoc->GetFileCopyPixel(i, j);
					float* scaleFactor = m_pDoc->m_pUI->getRGBScale();
					GLubyte afterColor[3] = { beforeColor[0]* scaleFactor [0],beforeColor[1] * scaleFactor[1] ,beforeColor[2] * scaleFactor[2] };
					memcpy(m_pDoc->GetOriginalPixel(i, j), afterColor, 3);

				}
			}

		//Marker
			memcpy(m_pDoc->m_ucDisplayCopy, m_pDoc->m_ucBitmap, m_pDoc->m_nWidth * m_pDoc->m_nHeight * 3);


			for (int i = -brushSize / 2; i < brushSize / 2; i++) {
				for (int j = -brushSize / 2; j < brushSize / 2; j++) {
					memcpy(m_pDoc->GetDisplayImgPixel((*disp).x + i, (*disp).y + j), color, 3);
				}
			}

			delete(disp);
		}

		bitstart = m_pDoc->m_ucDisplayCopy + 3 * ((m_pDoc->m_nWidth * startrow) + scrollpos.x);

		// just copy image to GLwindow conceptually
		glRasterPos2i( 0, m_nWindowHeight - drawHeight );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nWidth );
		glDrawBuffer( GL_BACK );
		glDrawPixels( drawWidth, drawHeight, GL_RGB, GL_UNSIGNED_BYTE, bitstart );

	}
			
	glFlush();
}

void OriginalView::refresh()
{
	redraw();
}

void OriginalView::resizeWindow(int	width, 
								int	height)
{
	resize(x(), y(), width, height);
}

