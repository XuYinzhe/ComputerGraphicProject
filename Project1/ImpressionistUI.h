//
// impressionistUI.h
//
// The header file for the UI part
//

#ifndef ImpressionistUI_h
#define ImpressionistUI_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_file_chooser.H>		// FLTK file chooser
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>

#include "Impressionist.h"
#include "OriginalView.h"
#include "PaintView.h"

#include "ImpBrush.h"

class ImpressionistUI {
public:
	ImpressionistUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;
								
	PaintView*			m_paintView;
	OriginalView*		m_origView;

// for brush dialog
	Fl_Window*			m_brushDialog;
	Fl_Choice*			m_BrushTypeChoice;
	Fl_Choice* m_BrushDirectionChoice;

	Fl_Slider*			m_BrushSizeSlider;
	Fl_Slider*			m_AlphaSlider;

	Fl_Slider* m_BrushWidthSlider;
	Fl_Slider* m_BrushAngleSlider;
	Fl_Button* m_ClearCanvasButton;
	Fl_Button* m_AutoDrawButton;
	Fl_Check_Button * m_AutoDrawRandomCheck;
	
// for color adjust
	Fl_Window* m_RGBScaleDialog;
	Fl_Slider* m_ColorRSlider;
	Fl_Slider* m_ColorGSlider;
	Fl_Slider* m_ColorBSlider;

// for Dissolve image
	Fl_Window* m_DissolveScaleDialog;
	Fl_Slider* m_DissolveAlphaSlider;
	Fl_Button* m_DissolveSelectFileButton;
// Convolution Kernal
	Fl_Window* m_ConvolutionDialog;
	Fl_Input* m_ConvolutionWeightsInput;
	Fl_Button* m_ConvolutionApply;	
	Fl_Check_Button* m_ConvolutionWeightNormalize;



	// Member functions
	void				setDocument(ImpressionistDoc* doc);
	ImpressionistDoc*	getDocument();

	void				show();
	void				resize_windows(int w, int h);

	// Interface to get attribute

	int					getSize();
		float* getRGBScale();
	float getBrushAlpha();
	float getDissolveAlpha();
	void				setSize(int size);

	int getWidth();
	void setWidth(int width);

	int getAngle();
	void setAngle(int angle);

private:
	ImpressionistDoc*	m_pDoc;		// pointer to document to communicate with the document

	// All attributes here
	int		m_nSize;
	float m_nColorScaleR;
	float m_nColorScaleG;
	float m_nColorScaleB;

	float m_nBrushAlpha;
	float m_nDissolveAlpha;
	int m_LineWidth;
	int m_LineAngle;
	int m_Alpha;

	// Static class members
	static Fl_Menu_Item		menuitems[];
	static Fl_Menu_Item		brushTypeMenu[NUM_BRUSH_TYPE+1];
	static Fl_Menu_Item		brushDirectionMenu[NUM_DIRECTION_TYPE + 1];

	static ImpressionistUI*	whoami(Fl_Menu_* o);

	// All callbacks here.  Callbacks are declared 
	// static
	static void	cb_load_image(Fl_Menu_* o, void* v);
	static void	cb_save_image(Fl_Menu_* o, void* v);
	static void	cb_brushes(Fl_Menu_* o, void* v);
	static void	cb_clear_canvas(Fl_Menu_* o, void* v);
	static void	cb_exit(Fl_Menu_* o, void* v);
	static void	cb_about(Fl_Menu_* o, void* v);
	static void	cb_brushChoice(Fl_Widget* o, void* v);
	static void	cb_directionChoice(Fl_Widget* o, void* v);
	static void	cb_clear_canvas_button(Fl_Widget* o, void* v);
	static void cb_AutoDraw_button(Fl_Widget* o, void* v);
	static void	cb_sizeSlides(Fl_Widget* o, void* v);
	static void	cb_widthSlides(Fl_Widget* o, void* v);
	static void	cb_angleSlides(Fl_Widget* o, void* v);
	static void	cb_alphaSlides(Fl_Widget* o, void* v);
	static void cb_AlphaSlides(Fl_Widget* o, void* v);
	static void cb_DissolveAlphaSlides(Fl_Widget* o, void* v);
	static void cb_swapPaintWithOriginal(Fl_Menu_* o, void* v);
	static void cb_swapPaintWithAnother(Fl_Menu_* o, void* v);
	static void cb_load_Dissolveimage(Fl_Widget* o, void* v);

	static void cb_applyConvolution(Fl_Widget* o, void* v);


	static void cb_load_Another_image(Fl_Menu_* o, void* v);

	static void cb_load_Video(Fl_Menu_* o, void* v);


	static void cb_undo(Fl_Menu_* o, void* v);

	static void cb_RGBscaleWidge(Fl_Menu_* o, void* v);

	static void cb_DissolveWidge(Fl_Menu_* o, void* v);

	static void cb_ConvolutionWidge(Fl_Menu_* o, void* v);

	static void cb_RSlides(Fl_Widget* o, void* v);

	static void cb_GSlides(Fl_Widget* o, void* v);

	static void cb_BSlides(Fl_Widget* o, void* v);
};

#endif
