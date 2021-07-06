//
// impressionistUI.h
//
// The user interface part for the program.
//


#include <FL/fl_ask.h>

#include <math.h>
#include <iostream>

#include "impressionistUI.h"
#include "impressionistDoc.h"
#include "KernalBruah.h"
#include <fstream>
#include <iostream>
#include "helper.h"
/*
//------------------------------ Widget Examples -------------------------------------------------
Here is some example code for all of the widgets that you may need to add to the 
project.  You can copy and paste these into your code and then change them to 
make them look how you want.  Descriptions for all of the widgets here can be found 
in links on the fltk help session page.

//---------Window/Dialog and Menubar-----------------------------------
	
	//----To install a window--------------------------
	Fl_Window* myWindow = new Fl_Window(600, 300, "MyWindow");
		myWindow->user_data((void*)(this));	// record self to be used by static callback functions
		
		// install menu bar
		myMenubar = new Fl_Menu_Bar(0, 0, 600, 25);
		Fl_Menu_Item ImpressionistUI::myMenuItems[] = {
			{ "&File",		0, 0, 0, FL_SUBMENU },
				{ "&Load...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load },
				{ "&Save...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save }.
				{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
				{ 0 },
			{ "&Edit",		0, 0, 0, FL_SUBMENU },
				{ "&Copy",FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_copy, (void *)COPY },
				{ "&Cut",	FL_ALT + 'x', (Fl_Callback *)ImpressionistUI::cb_cut, (void *)CUT },
				{ "&Paste",	FL_ALT + 'v', (Fl_Callback *)ImpressionistUI::cb_paste, (void *)PASTE },
				{ 0 },
			{ "&Help",		0, 0, 0, FL_SUBMENU },
				{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
				{ 0 },
			{ 0 }
		};
		myMenubar->menu(myMenuItems);
    m_mainWindow->end();

	//----The window callback--------------------------
	// One of the callbacks
	void ImpressionistUI::cb_load(Fl_Menu_* o, void* v) 
	{	
		ImpressionistDoc *pDoc=whoami(o)->getDocument();

		char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
		if (newfile != NULL) {
			pDoc->loadImage(newfile);
		}
	}


//------------Slider---------------------------------------

	//----To install a slider--------------------------
	Fl_Value_Slider * mySlider = new Fl_Value_Slider(10, 80, 300, 20, "My Value");
	mySlider->user_data((void*)(this));	// record self to be used by static callback functions
	mySlider->type(FL_HOR_NICE_SLIDER);
    mySlider->labelfont(FL_COURIER);
    mySlider->labelsize(12);
	mySlider->minimum(1);
	mySlider->maximum(40);
	mySlider->step(1);
	mySlider->value(m_nMyValue);
	mySlider->align(FL_ALIGN_RIGHT);
	mySlider->callback(cb_MyValueSlides);

	//----The slider callback--------------------------
	void ImpressionistUI::cb_MyValueSlides(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyValue=int( ((Fl_Slider *)o)->value() ) ;
	}
	

//------------Choice---------------------------------------
	
	//----To install a choice--------------------------
	Fl_Choice * myChoice = new Fl_Choice(50,10,150,25,"&myChoiceLabel");
	myChoice->user_data((void*)(this));	 // record self to be used by static callback functions
	Fl_Menu_Item ImpressionistUI::myChoiceMenu[3+1] = {
	  {"one",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)ONE},
	  {"two",FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)TWO},
	  {"three",FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)THREE},
	  {0}
	};
	myChoice->menu(myChoiceMenu);
	myChoice->callback(cb_myChoice);
	
	//-----The choice callback-------------------------
	void ImpressionistUI::cb_myChoice(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
		ImpressionistDoc* pDoc=pUI->getDocument();

		int type=(int)v;

		pDoc->setMyType(type);
	}


//------------Button---------------------------------------

	//---To install a button---------------------------
	Fl_Button* myButton = new Fl_Button(330,220,50,20,"&myButtonLabel");
	myButton->user_data((void*)(this));   // record self to be used by static callback functions
	myButton->callback(cb_myButton);

	//---The button callback---------------------------
	void ImpressionistUI::cb_myButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI*)(o->user_data()));
		ImpressionistDoc* pDoc = pUI->getDocument();
		pDoc->startPainting();
	}


//---------Light Button------------------------------------
	
	//---To install a light button---------------------
	Fl_Light_Button* myLightButton = new Fl_Light_Button(240,10,150,25,"&myLightButtonLabel");
	myLightButton->user_data((void*)(this));   // record self to be used by static callback functions
	myLightButton->callback(cb_myLightButton);

	//---The light button callback---------------------
	void ImpressionistUI::cb_myLightButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI *pUI=((ImpressionistUI*)(o->user_data()));

		if (pUI->myBool==TRUE) pUI->myBool=FALSE;
		else pUI->myBool=TRUE;
	}

//----------Int Input--------------------------------------

    //---To install an int input-----------------------
	Fl_Int_Input* myInput = new Fl_Int_Input(200, 50, 5, 5, "&My Input");
	myInput->user_data((void*)(this));   // record self to be used by static callback functions
	myInput->callback(cb_myInput);

	//---The int input callback------------------------
	void ImpressionistUI::cb_myInput(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyInputValue=int( ((Fl_Int_Input *)o)->value() );
	}

//------------------------------------------------------------------------------------------------
*/

//------------------------------------- Help Functions --------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI* ImpressionistUI::whoami(Fl_Menu_* o)	
{
	return ( (ImpressionistUI*)(o->parent()->user_data()) );
}


//--------------------------------- Callback Functions --------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
	if (newfile != NULL) {
		pDoc->loadImage(newfile);
	}
}

void ImpressionistUI::cb_load_Dissolveimage(Fl_Widget* o, void* v)
{
	ImpressionistDoc *pDoc= ((ImpressionistUI*)(o->user_data()))->m_pDoc;

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
	if (newfile != NULL && pDoc->m_ucBitmap != NULL) {
		pDoc->loadDissolveImage(newfile);
	}
}

void ImpressionistUI::cb_applyConvolution(Fl_Widget* o, void* v)
{
	ImpressionistDoc* pDoc = ((ImpressionistUI*)(o->user_data()))->m_pDoc;
	
	const char* weightImput = pDoc->m_pUI->m_ConvolutionWeightsInput->value();
	char* temp = new char[strlen(weightImput)];
	memcpy(temp, weightImput, strlen(weightImput));

	KernalBruah* kernalbrush = KernalBruah::GenerateConverlution(pDoc, temp, pDoc->m_pUI->m_ConvolutionWeightNormalize->value());
	if (kernalbrush == nullptr) {
		fl_message("Input is not a valid converlution kernal weight");
	}
	else {
		if (pDoc->currentKernal) delete pDoc->currentKernal;
		pDoc->currentKernal = kernalbrush;
		pDoc->m_pUI->m_paintView->doConverlution = true;
	}
}

void ImpressionistUI::cb_load_Another_image(Fl_Menu_* o, void* v)
{
	ImpressionistDoc *pDoc= whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
	if (newfile != NULL && pDoc->m_ucBitmap != NULL) {
		pDoc->loadAnotherImage(newfile);
	}
}
//helper func
void getFiles(std::string path, std::vector<std::string>& files)
{
	//文件句柄
	long   hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;

	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void ImpressionistUI::cb_load_Video(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "Video Files(*.{wmv,avi,dat,asf,mpeg,mpg,rm,rmvb,ram,flv,mp4,3gp,mov,divx,dv,vob,mkv,qt,cpk,fli,flc,f4v,m4v,mod,m2t,swf,webm,mts,m2ts})", pDoc->getImageName());
	if (newfile != NULL) {

		CreateDirectory(".\\temp\\", 0);
		deleteALL(".\\temp");
		CreateDirectory(".\\temp\\ori\\", 0);
		CreateDirectory(".\\temp\\aft\\", 0);
		
		std::string filePath = newfile;
		std::string com = ".\\ffmpeg\\bin\\ffmpeg.exe -i \"" + filePath + "\" -q:v 2 -vsync 2 -f image2 .\\temp\\ori\\core-%02d.bmp";
		
		if (!system(com.c_str())) {

			pDoc->m_pUI->m_paintView->files.clear();

			getFiles(".\\temp\\ori", pDoc->m_pUI->m_paintView->files);
			
			int realPathLength = (pDoc->workingPath + (pDoc->m_pUI->m_paintView->files[0])).length() + 1;
			char* temp = new char[realPathLength];
			temp[realPathLength-1] = '\0';
			memcpy(temp, (pDoc->workingPath + (pDoc->m_pUI->m_paintView->files[0])).c_str(), realPathLength);
			pDoc->loadImage(temp);
			delete[] temp;
			pDoc->m_pUI->m_paintView->doVideoProcess = true;
			pDoc->m_pUI->m_paintView->redraw();
		}
		else {
			fl_message("Unsupported format/Fail to open file");
		}

	}
}

//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Save File?", "*.bmp", "save.bmp" );
	if (newfile != NULL) {
		pDoc->saveImage(newfile);
	}
}

//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_brushDialog->show();
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc=whoami(o)->getDocument();

	pDoc->clearCanvas();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_mainWindow->hide();
	whoami(o)->m_brushDialog->hide();

}



//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("Impressionist FLTK version for CS341, Spring 2002");
}

//------- UI should keep track of the current for all the controls for answering the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush 
// choice.  
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc=pUI->getDocument();

	int type=(int)v;


	pDoc->setBrushType(type);/*
	if (type == BRUSH_LINES || type == BRUSH_SCATTERED_LINES)
	{
		m_BrushDirectionChoice->activate();
		m_BrushWidthSlider->activate();
		m_BrushAngleSlider->activate();
	}
	else
	{
		m_BrushDirectionChoice->deactivate();
		m_BrushWidthSlider->deactivate();
		m_BrushAngleSlider->deactivate();
	}*/
}

void ImpressionistUI::cb_directionChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	int type = (int)v;


	pDoc->setDirectionType(type);
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->clearCanvas();
}

void ImpressionistUI::cb_AutoDraw_button(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));

	pUI->m_paintView->toAutoDraw = true;
}

/// <summary>
/// update alpha value of brush
/// </summary>
void ImpressionistUI::cb_AlphaSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nBrushAlpha = float(((Fl_Slider*)o)->value());
}

void ImpressionistUI::cb_DissolveAlphaSlides(Fl_Widget* o, void* v){
	((ImpressionistUI*)(o->user_data()))->m_nDissolveAlpha = float(((Fl_Slider*)o)->value());
}

//-----------------------------------------------------------
// Swap the original image with current painting image
//-----------------------------------------------------------
void ImpressionistUI::cb_swapPaintWithOriginal(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	if(pDoc)
		pDoc->SwapPaintviewWithOriginal();
}

void ImpressionistUI::cb_swapPaintWithAnother(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	if(pDoc)
		pDoc->swapWithAnother();
}

void ImpressionistUI::cb_undo(Fl_Menu_* o, void* v){
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	if (pDoc)
		pDoc->UndoStep();
}

/// <summary>
/// show RGB scale dialog when clicked
/// </summary>
void ImpressionistUI::cb_RGBscaleWidge(Fl_Menu_* o, void* v) {
	whoami(o)->m_RGBScaleDialog->show();
}

void ImpressionistUI::cb_DissolveWidge(Fl_Menu_* o, void* v) {
	whoami(o)->m_DissolveScaleDialog->show();
}
void ImpressionistUI::cb_ConvolutionWidge(Fl_Menu_* o, void* v) {
	whoami(o)->m_ConvolutionDialog->show();
}

void ImpressionistUI::cb_alphaSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_Alpha = int(((Fl_Slider*)o)->value());
}

void ImpressionistUI::cb_RSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nColorScaleR = int(((Fl_Slider*)o)->value());
	((ImpressionistUI*)(o->user_data()))->m_origView->redraw();
}
void ImpressionistUI::cb_GSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nColorScaleG = int(((Fl_Slider*)o)->value());
	((ImpressionistUI*)(o->user_data()))->m_origView->redraw();

}
void ImpressionistUI::cb_BSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nColorScaleB = int(((Fl_Slider*)o)->value());
	((ImpressionistUI*)(o->user_data()))->m_origView->redraw();

}


//-----------------------------------------------------------
// Updates the brush size to use from the value of the size
// slider
// Called by the UI when the size slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
}

void ImpressionistUI::cb_widthSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_LineWidth = int(((Fl_Slider*)o)->value());
}

void ImpressionistUI::cb_angleSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_LineAngle = int(((Fl_Slider*)o)->value());
}

//void ImpressionistUI::cb_alphaSlides(Fl_Widget* o, void* v)
//{
//	((ImpressionistUI*)(o->user_data()))->m_Alpha = int(((Fl_Slider*)o)->value());
//}
//---------------------------------- per instance functions --------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
ImpressionistDoc* ImpressionistUI::getDocument()
{
	return m_pDoc;
}

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show() {
	m_mainWindow->show();
	m_paintView->show();
	m_origView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to 
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h) {
	m_paintView->size(w,h);
	m_origView->size(w,h);
}

//------------------------------------------------ 
// Set the ImpressionistDoc used by the UI to 
// communicate with the brushes 
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc* doc)
{
	m_pDoc = doc;

	m_origView->m_pDoc = doc;
	m_paintView->m_pDoc = doc;
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getSize()
{
	return m_nSize;
}

float* ImpressionistUI::getRGBScale()
{
	float result[3] = { m_ColorRSlider->value(),m_ColorGSlider->value(),m_ColorBSlider->value() };
	return result;
}

float ImpressionistUI::getBrushAlpha() {
	return m_nBrushAlpha;
}
float ImpressionistUI::getDissolveAlpha() {
	return m_nDissolveAlpha;
}


//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize( int size )
{
	m_nSize=size;

	if (size<=40) 
		m_BrushSizeSlider->value(m_nSize);
}

int ImpressionistUI::getWidth()
{
	return m_LineWidth;
}
void ImpressionistUI::setWidth(int width) 
{
	m_LineWidth = width;
	if (width <= 40&&width>=1)
		m_BrushWidthSlider->value(m_LineWidth);
}

int ImpressionistUI::getAngle()
{
	return m_LineAngle;
}
void ImpressionistUI::setAngle(int angle)
{
	m_LineAngle = angle;
	if (angle <= 359&&angle>=0)
		m_BrushAngleSlider->value(m_LineAngle);
}

// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Image...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load_image },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image },
		{ "&Brushes...",	FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes }, 
		{ "&Clear Canvas", FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_clear_canvas, 0, FL_MENU_DIVIDER },
		{ "&Undo", FL_CTRL + 'z', (Fl_Callback *)ImpressionistUI::cb_undo, 0, FL_MENU_DIVIDER },

		{ "Load &Another Image...",	FL_ALT + 'a', (Fl_Callback*)ImpressionistUI::cb_load_Another_image , 0, FL_MENU_DIVIDER },

	
		{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
		{ 0 },

	{ "&ImageControl",		0, 0, 0, FL_SUBMENU },
		{ "Swap &Paintview with original",	FL_ALT + 'P', (Fl_Callback*)ImpressionistUI::cb_swapPaintWithOriginal },
		{ "Swap &Original with Another",	FL_ALT + 'O', (Fl_Callback*)ImpressionistUI::cb_swapPaintWithAnother },
		{ "&Color Scale",	FL_ALT + 'C', (Fl_Callback*)ImpressionistUI::cb_RGBscaleWidge },
		{ "&Dissolve",	FL_ALT + 'D', (Fl_Callback*)ImpressionistUI::cb_DissolveWidge },
		{ "&Convolution ",	0, (Fl_Callback*)ImpressionistUI::cb_ConvolutionWidge },

		{ 0 },
	{ "&Video",		0, 0, 0, FL_SUBMENU },
			{ "&Video Auto Draw",	FL_ALT + 'V', (Fl_Callback*)ImpressionistUI::cb_load_Video },

	{0},

	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
		{ 0 },

	{ 0 }
};

// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE+1] = {
  {"Points",			FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_POINTS},
  {"Lines",				FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_LINES},
  {"Circles",			FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_CIRCLES},
  {"Scattered Points",	FL_ALT+'q', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_POINTS},
  {"Scattered Lines",	FL_ALT+'m', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_LINES},
  {"Scattered Circles",	FL_ALT+'d', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_CIRCLES},
  {"Gaussian Blur",		FL_ALT+'g', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_GAUSSIAN_BLUR},
  {"Star",		FL_ALT + 's', (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_STAR},
  {0}
};

Fl_Menu_Item ImpressionistUI::brushDirectionMenu[NUM_DIRECTION_TYPE + 1] = {
	{"Slider/Right Mouse", FL_ALT+'s', (Fl_Callback *)ImpressionistUI::cb_directionChoice, (void*)RIGHT_MOUSE},
	{"Gradient", FL_ALT + 'g', (Fl_Callback*)ImpressionistUI::cb_directionChoice, (void*)GRADIENT},
	{"Brush Direction", FL_ALT + 'b', (Fl_Callback*)ImpressionistUI::cb_directionChoice, (void*)BRUSH_DIRECTION},
	{0}
};


//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI() {
	// Create the main window
	m_mainWindow = new Fl_Window(600, 300, "Impressionist");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
		m_menubar->menu(menuitems);

		// Create a group that will hold two sub windows inside the main
		// window
		Fl_Group* group = new Fl_Group(0, 25, 600, 275);

			// install paint view window
			m_paintView = new PaintView(300, 25, 300, 275, "This is the paint view");//0jon
			m_paintView->box(FL_DOWN_FRAME);

			// install original view window
			m_origView = new OriginalView(0, 25, 300, 275, "This is the orig view");//300jon
			m_origView->box(FL_DOWN_FRAME);
			m_origView->deactivate();

		group->end();
		Fl_Group::current()->resizable(group);
    m_mainWindow->end();

	// init values

	m_nSize = 10;
	m_nColorScaleR = 1.0;
	m_nColorScaleG = 1.0;
	m_nColorScaleB = 1.0;
	m_nBrushAlpha = 1.0;
	m_nDissolveAlpha = 1.0;
	m_LineWidth = 1;
	m_LineAngle = 0;
	m_Alpha = 1;

	// brush dialog definition
	m_brushDialog = new Fl_Window(600, 525, "Brush Dialog");
		// Add a brush type choice to the dialog
		m_BrushTypeChoice = new Fl_Choice(50,10,150,25,"&Brush");
		m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushTypeChoice->menu(brushTypeMenu);
		m_BrushTypeChoice->callback(cb_brushChoice);

		m_BrushDirectionChoice = new Fl_Choice(115, 45, 150, 25, "&Stroke Direction");
		m_BrushDirectionChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushDirectionChoice->menu(brushDirectionMenu);
		m_BrushDirectionChoice->callback(cb_directionChoice);

		m_ClearCanvasButton = new Fl_Button(240,10,150,25,"&Clear Canvas");
		m_ClearCanvasButton->user_data((void*)(this));
		m_ClearCanvasButton->callback(cb_clear_canvas_button);

		m_AutoDrawButton = new Fl_Button(400,10,150,25,"&Auto Draw");
		m_AutoDrawButton->user_data((void*)(this));
		m_AutoDrawButton->callback(cb_AutoDraw_button);
		
		m_AutoDrawRandomCheck = new Fl_Check_Button(400,40,150,25,"Random stroks");
		m_AutoDrawRandomCheck->user_data((void*)(this));
		


		// Add brush size slider to the dialog 
		m_BrushSizeSlider = new Fl_Value_Slider(10, 80, 300, 20, "Size");
		m_BrushSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
        m_BrushSizeSlider->labelfont(FL_COURIER);
        m_BrushSizeSlider->labelsize(12);
		m_BrushSizeSlider->minimum(1);
		m_BrushSizeSlider->maximum(40);
		m_BrushSizeSlider->step(1);
		m_BrushSizeSlider->value(m_nSize);
		m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
		m_BrushSizeSlider->callback(cb_sizeSlides);
	
		// Add brush alpha slider to the dialog 
		m_AlphaSlider = new Fl_Value_Slider(10, 170, 300, 20, "Alpha");
		m_AlphaSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_AlphaSlider->type(FL_HOR_NICE_SLIDER);
		m_AlphaSlider->labelfont(FL_COURIER);
		m_AlphaSlider->labelsize(12);
		m_AlphaSlider->minimum(0);
		m_AlphaSlider->maximum(1);
		m_AlphaSlider->step(0.01);
		m_AlphaSlider->value(m_nBrushAlpha);
		m_AlphaSlider->align(FL_ALIGN_RIGHT);
		m_AlphaSlider->callback(cb_AlphaSlides);

		m_BrushWidthSlider = new Fl_Value_Slider(10, 110, 300, 20, "Line Width"); //std::cout << m_BrushTypeChoice->value() << std::endl;
		//if(m_BrushTypeChoice->value()==1|| m_BrushTypeChoice->value()==4)
		m_BrushWidthSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushWidthSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushWidthSlider->labelfont(FL_COURIER);
		m_BrushWidthSlider->labelsize(12);
		m_BrushWidthSlider->minimum(1);
		m_BrushWidthSlider->maximum(40);
		m_BrushWidthSlider->step(1);
		m_BrushWidthSlider->value(m_LineWidth);
		m_BrushWidthSlider->align(FL_ALIGN_RIGHT);
		//if (m_BrushTypeChoice->value() == 1 || m_BrushTypeChoice->value() == 4)
		m_BrushWidthSlider->callback(cb_widthSlides);

		m_BrushAngleSlider = new Fl_Value_Slider(10, 140, 300, 20, "Line Angle"); //std::cout << m_BrushTypeChoice->value() << std::endl;
		//if(m_BrushTypeChoice->value()==1|| m_BrushTypeChoice->value()==4)
		m_BrushAngleSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushAngleSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushAngleSlider->labelfont(FL_COURIER);
		m_BrushAngleSlider->labelsize(12);
		m_BrushAngleSlider->minimum(0);
		m_BrushAngleSlider->maximum(359);
		m_BrushAngleSlider->step(1);
		m_BrushAngleSlider->value(m_LineAngle);
		m_BrushAngleSlider->align(FL_ALIGN_RIGHT);
		//if (m_BrushTypeChoice->value() == 1 || m_BrushTypeChoice->value() == 4)
		m_BrushAngleSlider->callback(cb_angleSlides);



    m_brushDialog->end();	
	
// RGB dialog
	m_RGBScaleDialog = new Fl_Window(400, 150, "Color adjustment ");
		m_ColorRSlider = new Fl_Value_Slider(10, 10, 300, 20, "R");
		m_ColorRSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_ColorRSlider->type(FL_HOR_NICE_SLIDER);
		m_ColorRSlider->labelfont(FL_COURIER);
		m_ColorRSlider->labelsize(12);
		m_ColorRSlider->minimum(0);
		m_ColorRSlider->maximum(1);
		m_ColorRSlider->step(0.01);
		m_ColorRSlider->value(m_nColorScaleR);
		m_ColorRSlider->align(FL_ALIGN_RIGHT);
		m_ColorRSlider->callback(cb_RSlides);

		m_ColorGSlider = new Fl_Value_Slider(10, 35, 300, 20, "G");
		m_ColorGSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_ColorGSlider->type(FL_HOR_NICE_SLIDER);
		m_ColorGSlider->labelfont(FL_COURIER);
		m_ColorGSlider->labelsize(12);
		m_ColorGSlider->minimum(0);
		m_ColorGSlider->maximum(1);
		m_ColorGSlider->step(0.01);
		m_ColorGSlider->value(m_nColorScaleG);
		m_ColorGSlider->align(FL_ALIGN_RIGHT);
		m_ColorGSlider->callback(cb_GSlides);

		m_ColorBSlider = new Fl_Value_Slider(10, 60, 300, 20, "B");
		m_ColorBSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_ColorBSlider->type(FL_HOR_NICE_SLIDER);
		m_ColorBSlider->labelfont(FL_COURIER);
		m_ColorBSlider->labelsize(12);
		m_ColorBSlider->minimum(0);
		m_ColorBSlider->maximum(1);
		m_ColorBSlider->step(0.01);
		m_ColorBSlider->value(m_nColorScaleG);
		m_ColorBSlider->align(FL_ALIGN_RIGHT);
		m_ColorBSlider->callback(cb_BSlides);
	m_RGBScaleDialog->end();

	//Dissolve dialog
	m_DissolveScaleDialog = new Fl_Window(400, 150, "Dissolve an image to paint view");

		m_DissolveAlphaSlider = new Fl_Value_Slider(10, 10, 300, 20, "Alpha");
		m_DissolveAlphaSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_DissolveAlphaSlider->type(FL_HOR_NICE_SLIDER);
		m_DissolveAlphaSlider->labelfont(FL_COURIER);
		m_DissolveAlphaSlider->labelsize(12);
		m_DissolveAlphaSlider->minimum(0);
		m_DissolveAlphaSlider->maximum(1);
		m_DissolveAlphaSlider->step(0.01);
		m_DissolveAlphaSlider->value(m_nDissolveAlpha);
		m_DissolveAlphaSlider->align(FL_ALIGN_RIGHT);
		m_DissolveAlphaSlider->callback(cb_DissolveAlphaSlides);

		Fl_Button* m_DissolveSelectFileButton = new Fl_Button(190, 40, 150, 20, "Select File");
		m_DissolveSelectFileButton->user_data((void*)(this));   // record self to be used by static callback functions
		m_DissolveSelectFileButton->callback(cb_load_Dissolveimage);

	m_DissolveScaleDialog->end();	

	m_ConvolutionDialog = new Fl_Window(500, 150, "Convolution");
		m_ConvolutionWeightsInput = new Fl_Input(100, 10, 300, 20, "Weights");

		m_ConvolutionApply = new Fl_Button(100, 30, 50, 20, "Apply");
		m_ConvolutionApply->user_data((void*)(this));   // record self to be used by static callback functions
		m_ConvolutionApply->callback(cb_applyConvolution);

		m_ConvolutionWeightNormalize = new Fl_Check_Button(100, 60, 50, 20, "Normalize");

	m_ConvolutionDialog->end();

}
