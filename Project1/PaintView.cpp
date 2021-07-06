//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintview.h"
#include "ImpBrush.h"
#include <iostream>
#include <algorithm>
#include <time.h>
#include <corecrt_io.h>
#include <ctime>


#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6


#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent = 0;
static Point	coord;


Point PaintView::getCurrentMOusePos()
{
	return coord;
}

PaintView::PaintView(int			x,
	int			y,
	int			w,
	int			h,
	const char* l)
	: Fl_Gl_Window(x, y, w, h, l)
{
	m_nWindowWidth = w;
	m_nWindowHeight = h;
	toAutoDraw = false;
	doConverlution = false;
	doVideoProcess = false;

}


void PaintView::draw()
{
#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
#endif // !MESA

	//Enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (!valid())
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable(GL_DEPTH_TEST);

		ortho();

		glClear(GL_COLOR_BUFFER_BIT);
	}

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y = 0;

	m_nWindowWidth = w();
	m_nWindowHeight = h();

	int drawWidth, drawHeight;
	drawWidth = min(m_nWindowWidth, m_pDoc->m_nPaintWidth); //std::cout << drawWidth << std::endl;
	drawHeight = min(m_nWindowHeight, m_pDoc->m_nPaintHeight); //std::cout << drawHeight << std::endl;

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight); //std::cout << startrow << std::endl;
	if (startrow < 0) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting +
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth = drawWidth;
	m_nDrawHeight = drawHeight;

	m_nStartRow = startrow;
	m_nEndRow = startrow + drawHeight;
	m_nStartCol = scrollpos.x;
	m_nEndCol = m_nStartCol + drawWidth;
	if (m_pDoc->m_ucPainting) {

		if (doVideoProcess) {

			if (!files.empty()) {
				int realPathLength = (m_pDoc->workingPath + (m_pDoc->m_pUI->m_paintView->files[0])).length() + 1;
				realPathLength = (m_pDoc->workingPath + (m_pDoc->m_pUI->m_paintView->files[0])).length() + 1;
				outPath[realPathLength-1] = '\0';
				memcpy(outPath, (m_pDoc->workingPath + (m_pDoc->m_pUI->m_paintView->files[0])).c_str(), realPathLength);
				int w, h;
				delete[] m_pDoc->m_ucBitmap;
				m_pDoc->m_ucBitmap = readBMP(outPath, w, h);
				autoDraw(true);
				std::string savepath = outPath;
				savepath.replace(m_pDoc->workingPath.length()+7, 3, "aft");
				memcpy(outPath, savepath.c_str(), realPathLength);
				m_pDoc->saveImage(outPath);
				m_pDoc->clearCanvas();
				RestoreContent();
				std::vector<std::string>::iterator k = files.begin();
				files.erase(k);
			}
			else {
				doVideoProcess = false;
				SaveCurrentContent();
				system(".\\ffmpeg\\bin\\ffmpeg.exe -f image2 -i ./temp/aft/core-%02d.bmp .\\result.avi");
			}
		}
		else if (toAutoDraw)
		{
			autoDraw(m_pDoc->m_pUI->m_AutoDrawRandomCheck->value());
			toAutoDraw = false;

		}
		else if (doConverlution) {
			glPointSize(1);

			for (int x = 0; x < m_nDrawWidth; x++) {
				for (int y = 0; y < m_nDrawHeight; y++) {
					const Point target(x, m_nWindowHeight - y);
					m_pDoc->currentKernal->BrushMove(target, target);
				}
				glFlush();
			}

			SaveCurrentContent();
			RestoreContent();
			redraw();
			doConverlution = false;
		}
		else {
			if (!isAnEvent)
			{
				RestoreContent();

			}



			if (isAnEvent)
			{

				// Clear it after processing.
				isAnEvent = 0;

				Point source(coord.x + m_nStartCol, m_nEndRow - coord.y); //std::cout << source.x<<' '<< source.y<< std::endl;
				Point target(coord.x, m_nWindowHeight - coord.y); //std::cout << target.x<<' '<<target.y << std::endl << std::endl;

				// This is the event handler
				switch (eventToDo)
				{
				case LEFT_MOUSE_DOWN:
					SaveStep();
					m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
					break;
				case LEFT_MOUSE_DRAG:
					m_pDoc->m_pCurrentBrush->BrushMove(source, target);
					SaveCurrentContent();
					RestoreContent();
					break;
				case LEFT_MOUSE_UP:
					m_pDoc->m_pCurrentBrush->BrushEnd(source, target);

					SaveCurrentContent();
					RestoreContent();
					break;
				case RIGHT_MOUSE_DOWN:
					//std::cout << "begin" << std::endl;
					m_pDoc->m_pCurrentBrush->DirectionBegin(source, target);
					break;
				case RIGHT_MOUSE_DRAG:
					RestoreContent();
					m_pDoc->m_pCurrentBrush->DirectionMove(source, target);
					break;
				case RIGHT_MOUSE_UP:
					m_pDoc->m_pCurrentBrush->DirectionEnd(source, target);
					RestoreContent();
					break;

				default:
					printf("Unknown event!!\n");
					break;
				}
			}
		}
	}



	glFlush();

#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
#endif // !MESA

}


int PaintView::handle(int event)
{
	switch (event)
	{
	case FL_ENTER:
		redraw();
		break;
	case FL_PUSH:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button() > 1)
			eventToDo = RIGHT_MOUSE_DOWN;
		else
			eventToDo = LEFT_MOUSE_DOWN;
		isAnEvent = 1;
		redraw();
		break;
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button() > 1)
			eventToDo = RIGHT_MOUSE_DRAG;
		else
			eventToDo = LEFT_MOUSE_DRAG;
		isAnEvent = 1;
		m_pDoc->m_pUI->m_origView->refresh();
		redraw();
		break;
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button() > 1)
			eventToDo = RIGHT_MOUSE_UP;
		else
			eventToDo = LEFT_MOUSE_UP;
		isAnEvent = 1;
		redraw();
		break;
	case FL_MOVE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		m_pDoc->m_pUI->m_origView->refresh();
		break;
	default:
		return 0;
		break;

	}

	if (doVideoProcess) {
		m_pDoc->m_pUI->m_origView->refresh();
		redraw();
	}


	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent()
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(GL_FRONT);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);

	glReadPixels(0,
		m_nWindowHeight - m_nDrawHeight,
		m_nDrawWidth,
		m_nDrawHeight,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		m_pPaintBitstart);
}


void PaintView::RestoreContent()
{
	glDrawBuffer(GL_BACK);

	glClear(GL_COLOR_BUFFER_BIT);

	glRasterPos2i(0, m_nWindowHeight - m_nDrawHeight);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);
	glDrawPixels(m_nDrawWidth,
		m_nDrawHeight,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		m_pPaintBitstart);

	//	glDrawBuffer(GL_FRONT);
}
void PaintView::SaveStep() {
	memcpy(m_pDoc->m_ucLastStep, m_pDoc->m_ucPainting, m_pDoc->m_nWidth * m_pDoc->m_nHeight * 3);
}

void PaintView::autoDraw(bool doRandom)
{
	int size = m_pDoc->getSize();

	int* x_vlaues = new int[m_nDrawWidth];
	for (int x = 0; x < m_nDrawWidth; x++) {
		x_vlaues[x] = x;
	}
	int* y_vlaues = new int[m_nDrawHeight];
	for (int y = 0; y < m_nDrawHeight; y++) {
		y_vlaues[y] = y;
	}


	SaveStep();

	if (doRandom) {
		unsigned seed = time(0);
		srand(seed);

		std::random_shuffle(x_vlaues, x_vlaues + m_nDrawWidth);
		std::random_shuffle(y_vlaues, y_vlaues + m_nDrawHeight);

		for (int x = 0; x < m_nDrawWidth; x++) {
			for (int y = 0; y < m_nDrawHeight; y++) {
				const Point target(x_vlaues[x], m_nWindowHeight - y_vlaues[y]);
				m_pDoc->m_pUI->setSize(max(size + (rand() % size) - size / 2, 1));
				m_pDoc->m_pCurrentBrush->BrushBegin(target, target);


			}
			glFlush();
		}
		m_pDoc->m_pUI->setSize(size);

	}
	else {
		for (int x = 0; x < m_nDrawWidth; x += size / 2) {
			for (int y = 0; y < m_nDrawHeight; y += size / 2) {
				const Point target(x, m_nWindowHeight - y);
				m_pDoc->m_pCurrentBrush->BrushBegin(target, target);


			}
			glFlush();
		}
	}


	SaveCurrentContent();
	RestoreContent();

	redraw();
	delete[] x_vlaues;
	delete[] y_vlaues;
}


