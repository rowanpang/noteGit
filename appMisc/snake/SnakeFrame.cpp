
/*@ <FileComment ID=1243004285187> @*/
/**********************************************************************
 Copyright (c) 2006-2009 MStar Semiconductor, Inc.
 All rights reserved.

 Unless otherwise stipulated in writing, any and all information contained
 herein regardless in any format shall remain the sole proprietary of
 MStar Semiconductor Inc. and be kept in strict confidence
 (MStar Confidential Information) by the recipient.
 Any unauthorized act including without limitation unauthorized disclosure,
 copying, use, reproduction, sale, distribution, modification, disassembling,
 reverse engineering and compiling of the contents of MStar Confidential
 Information is unlawful and strictly prohibited. MStar hereby reserves the
 rights to any and all damages, losses, costs and expenses resulting therefrom.

* Class :
* File  :
**********************************************************************/
/*@ </FileComment ID=1243004285187> @*/


/*@ <Include> @*/
#include "SnakeFrame.h"
#include "MApplet.h"
#include "AbsContainer.h"
#include "Container.h"
#include "MWnd.h"
#include "MEvtHandle.h"
#include "MGfx.h"
#include "Str.rc.h"
#include "Color.rc.h"
#include "Image.rc.h"
//#include "Font.rc.h"
//#include "Mba.rc.h"
#include "Num.rc.h"
#include "RcManager.h"
#include "mlogger.h"
#include "MLoggerMacro.h"
#include <string.h>
/*@ </Include> @*/
#include "MSrv_Control.h"
#include "MSrv_Timer.h"
#include "MKeycode.h"
#include <sstream>

BEGIN_EVENT_MAP(SnakeFrame, MMainFrame)
ON_CREATE(&SnakeFrame::OnCreate)
ON_TIMER(ID_SNAKE_RENEWAL_TIMER, &SnakeFrame::OnTimer)
ON_TIMER(ID_SNAKE_MESSAGE_BOX_TIMER, &SnakeFrame::OnTimer)
ON_KEY(&SnakeFrame::OnKey)
//(*EventMap
//*)
END_EVENT_MAP();

/*@ <Operation ID=I53d2cee3m121525d89fbmm62bb> @*/
SnakeFrame::SnakeFrame(MApplet * pParentApp, Rect & stRect)
        : MMainFrame(pParentApp, stRect)
{
    /* -> add your code here */

    //(*MLoggerCtor
    strncpy(name,"Snake",MAX_APPNAME_LENGTH-1);
    logid = MLogger_RegisterComponent(name, E_MLG_DARK_WHITE, E_MLG_DARK_BLACK);
    printf_info(" %s->Constructor called!!\n",name);
    printf_info("Component %s Registered!\n",name);
    //*)

}

SnakeFrame::~SnakeFrame()
{
    //(*MLoggerDtor
    printf_info(" %s->Destructor called!!\n",name);
    printf_info("Component %s Un-Registered!\n",name);
    MLogger_UnRegisterComponent(logid, name);
    //*)
}
/*@ </Operation ID=I53d2cee3m121525d89fbmm62bb> @*/

//(*EventHandlerImp
//*)
#define SNAKE_LENGTH_INIT 4

/*@ <Operation ID=If58cf9dm12168c8928bmm640b> @*/
BOOL SnakeFrame::OnCreate(void * arg1, void * arg2, void * arg3)
{
    (void)arg1;
    (void)arg2;
    (void)arg3;

    Rect rc;

    //================ AbsContainer_CoinTetrisPage ================
  rc.set(0,0,1366,768);
    AbsContainer_SnakePage = new (std::nothrow) AbsContainer(this->root_cntr, rc, 0, ID_Snake_ABSCONTAINER_PAGE);
    UICREATE_RET_CHK(AbsContainer_SnakePage);
    AbsContainer_SnakePage->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    AbsContainer_SnakePage->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    AbsContainer_SnakePage->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    AbsContainer_SnakePage->style.bg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    AbsContainer_SnakePage->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_0);
    AbsContainer_SnakePage->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_0);
    AbsContainer_SnakePage->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_0);
    AbsContainer_SnakePage->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_0);
    AbsContainer_SnakePage->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_0);
    AbsContainer_SnakePage->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_0);
    AbsContainer_SnakePage->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);//ID_IMG_MAINMENU_BG
    AbsContainer_SnakePage->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    AbsContainer_SnakePage->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    AbsContainer_SnakePage->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    this->root_cntr->AddChild(AbsContainer_SnakePage);

	//================ TextBox_Help_Info_4_img ================
    rc.set(1100,487,30,30);
    Image_Help_Info_4 = new (std::nothrow) Image(AbsContainer_SnakePage, rc, 0, ID_SNAKE_HELP_INFO_IMG);
    UICREATE_RET_CHK(Image_Help_Info_4);
    Image_Help_Info_4->img[E_WDG_ACTIVE].Set(ID_IMG_SODUKU_HELP_INFO);
    Image_Help_Info_4->img[E_WDG_DISABLED].Set(ID_IMG_SODUKU_HELP_INFO);
    Image_Help_Info_4->img[E_WDG_FOCUSED].Set(ID_IMG_SODUKU_HELP_INFO);
    Image_Help_Info_4->img[E_WDG_SELECTED].Set(ID_IMG_SODUKU_HELP_INFO);
    Image_Help_Info_4->style.img_bg_shown = FALSE;
    Image_Help_Info_4->style.focusable = FALSE;
    Image_Help_Info_4->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    Image_Help_Info_4->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    Image_Help_Info_4->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
    Image_Help_Info_4->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
    Image_Help_Info_4->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    Image_Help_Info_4->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    Image_Help_Info_4->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
    Image_Help_Info_4->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    Image_Help_Info_4->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    Image_Help_Info_4->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    AbsContainer_SnakePage->AddChild(Image_Help_Info_4);

    /*================ TextBox_Help_Info_4 ================*/
    rc.set(1136,490,150,25);
    TextBox_Help_Info_4 = new (std::nothrow) TextBox(AbsContainer_SnakePage, rc, 0, ID_SNAKE_HELP_INFO_4);
    UICREATE_RET_CHK(TextBox_Help_Info_4);
    TextBox_Help_Info_4->text.Set(ID_STR_SUDOKU_HELP_INFO_4);
    TextBox_Help_Info_4->style.bg_shown = FALSE;
    TextBox_Help_Info_4->style.focusable = FALSE;
    TextBox_Help_Info_4->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_BLACK_TRANS);
    TextBox_Help_Info_4->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_4->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_4->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_MAIN);
    TextBox_Help_Info_4->style.text_fg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_4->style.text_fg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_4->style.text_fg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOUR_WHITE);
    TextBox_Help_Info_4->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_4->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_4->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_4->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_4->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Help_Info_4->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Help_Info_4->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_4->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_4->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_4->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_4->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_4->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_4->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_4->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_4->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_4->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_4->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_4->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_4->style.text_align = E_WDG_ALIGN_MIDDLELEFT;
    AbsContainer_SnakePage->AddChild(TextBox_Help_Info_4);

	/*================ TextBox_Help_Info_6 ================*/
    rc.set(1000,550,150,25);
    TextBox_Help_Info_6 = new (std::nothrow) TextBox(AbsContainer_SnakePage, rc, 0, ID_SNAKE_HELP_INFO_6);
    UICREATE_RET_CHK(TextBox_Help_Info_6);
    TextBox_Help_Info_6->text.Set(ID_STR_SUDOKU_HELP_INFO_6);
    TextBox_Help_Info_6->style.bg_shown = FALSE;
    TextBox_Help_Info_6->style.focusable = FALSE;
    TextBox_Help_Info_6->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_BLACK_TRANS);
    TextBox_Help_Info_6->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_6->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_6->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_MAIN);
    TextBox_Help_Info_6->style.text_fg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_6->style.text_fg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_6->style.text_fg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOUR_WHITE);
    TextBox_Help_Info_6->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_6->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_6->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_6->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_6->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Help_Info_6->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Help_Info_6->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_6->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_6->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_6->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_6->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_6->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_6->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_6->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_6->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_6->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_6->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_6->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_6->style.text_align = E_WDG_ALIGN_MIDDLERIGHT;
    AbsContainer_SnakePage->AddChild(TextBox_Help_Info_6);

    /*================ TextBox_Help_Info_6_1 ================*/
    rc.set(1150,550,200,25);
    TextBox_Help_Info_6_1 = new (std::nothrow) TextBox(AbsContainer_SnakePage, rc, 0, ID_SNAKE_HELP_INFO_6_1);
    UICREATE_RET_CHK(TextBox_Help_Info_6_1);
    TextBox_Help_Info_6_1->text.Set(ID_STR_SUDOKU_HELP_INFO_6_1);
    TextBox_Help_Info_6_1->style.bg_shown = FALSE;
    TextBox_Help_Info_6_1->style.focusable = FALSE;
    TextBox_Help_Info_6_1->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_BLACK_TRANS);
    TextBox_Help_Info_6_1->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_6_1->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_6_1->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_MAIN);
    TextBox_Help_Info_6_1->style.text_fg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_6_1->style.text_fg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_6_1->style.text_fg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOUR_WHITE);
    TextBox_Help_Info_6_1->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_6_1->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_6_1->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_6_1->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_6_1->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Help_Info_6_1->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Help_Info_6_1->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_6_1->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_6_1->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_6_1->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_6_1->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_6_1->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_6_1->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_6_1->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_6_1->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_6_1->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_6_1->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_6_1->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_6_1->style.text_align = E_WDG_ALIGN_MIDDLELEFT;
    AbsContainer_SnakePage->AddChild(TextBox_Help_Info_6_1);

    /*================ TextBox_Score================*/
    rc.set(980,200,200,50);
    TextBox_Score_Info = new (std::nothrow) TextBox(AbsContainer_SnakePage, rc, 0, ID_SNAKE_SCORE_INFO);
    UICREATE_RET_CHK(TextBox_Score_Info);
    TextBox_Score_Info->text.Set(ID_STR_COIN_SCORE);
    TextBox_Score_Info->style.bg_shown = FALSE;
    TextBox_Score_Info->style.focusable = FALSE;
    TextBox_Score_Info->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_BLACK_TRANS);
    TextBox_Score_Info->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Score_Info->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Score_Info->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);//ID_COLOR_GRASS_GREEN
    TextBox_Score_Info->style.text_fg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
    TextBox_Score_Info->style.text_fg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
    TextBox_Score_Info->style.text_fg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
    TextBox_Score_Info->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Score_Info->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Score_Info->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Score_Info->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Score_Info->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Score_Info->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Score_Info->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
    TextBox_Score_Info->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    TextBox_Score_Info->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    TextBox_Score_Info->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    TextBox_Score_Info->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
    TextBox_Score_Info->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
    TextBox_Score_Info->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
    TextBox_Score_Info->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
    TextBox_Score_Info->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
    TextBox_Score_Info->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
    TextBox_Score_Info->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
    TextBox_Score_Info->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
    TextBox_Score_Info->style.text_align = E_WDG_ALIGN_MIDDLERIGHT;
    AbsContainer_SnakePage->AddChild(TextBox_Score_Info);

	rc.set(1200,200,300,50);
	TextBox_Score = new (std::nothrow) TextBox(AbsContainer_SnakePage, rc, 0, ID_SNAKE_SCORE);
	UICREATE_RET_CHK(TextBox_Score);
	TextBox_Score->text.Set(ID_STR_0);
	TextBox_Score->style.bg_shown = FALSE;
	TextBox_Score->style.focusable = FALSE;
	TextBox_Score->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_BLACK_TRANS);
	TextBox_Score->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
	TextBox_Score->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
	TextBox_Score->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_GRASS_GREEN);
	TextBox_Score->style.text_fg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOR_GRASS_GREEN);
	TextBox_Score->style.text_fg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOR_GRASS_GREEN);
	TextBox_Score->style.text_fg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOR_GRASS_GREEN);
	TextBox_Score->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_Score->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_Score->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_Score->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_Score->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
	TextBox_Score->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
	TextBox_Score->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
	TextBox_Score->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
	TextBox_Score->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
	TextBox_Score->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
	TextBox_Score->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
	TextBox_Score->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
	TextBox_Score->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
	TextBox_Score->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
	TextBox_Score->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_Score->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_Score->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_Score->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_Score->style.text_align = E_WDG_ALIGN_MIDDLELEFT;
	AbsContainer_SnakePage->AddChild(TextBox_Score);

#if SNAKE_SUPPORT_INIT_LENGTH_PAUSE
		
	/*================ TextBox_Score================*/
	rc.set(880,300,300,50);
	TextBox_InitLength_Info = new (std::nothrow) TextBox(AbsContainer_SnakePage, rc, 0, ID_SNAKE_INIT_LENGTH_INFO);
	UICREATE_RET_CHK(TextBox_Score_Info);
	TextBox_InitLength_Info->text.Set("InitLength :");
	TextBox_InitLength_Info->style.bg_shown = FALSE;
	TextBox_InitLength_Info->style.focusable = TRUE;
	TextBox_InitLength_Info->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_BLACK_TRANS);
	TextBox_InitLength_Info->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
	TextBox_InitLength_Info->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
	TextBox_InitLength_Info->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_MAIN);//ID_COLOR_GRASS_GREEN
	TextBox_InitLength_Info->style.text_fg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
	TextBox_InitLength_Info->style.text_fg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOR_BLUE);
	TextBox_InitLength_Info->style.text_fg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
	TextBox_InitLength_Info->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_InitLength_Info->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_InitLength_Info->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_InitLength_Info->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_InitLength_Info->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
	TextBox_InitLength_Info->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
	TextBox_InitLength_Info->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
	TextBox_InitLength_Info->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
	TextBox_InitLength_Info->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
	TextBox_InitLength_Info->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
	TextBox_InitLength_Info->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
	TextBox_InitLength_Info->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
	TextBox_InitLength_Info->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
	TextBox_InitLength_Info->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
	TextBox_InitLength_Info->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_InitLength_Info->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_InitLength_Info->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_InitLength_Info->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_InitLength_Info->style.text_align = E_WDG_ALIGN_MIDDLERIGHT;
	AbsContainer_SnakePage->AddChild(TextBox_InitLength_Info);

	rc.set(1200,300,300,50);
	TextBox_InitLength_Num = new (std::nothrow) TextBox(AbsContainer_SnakePage, rc, 0, ID_SNAKE_INIT_LENGTH_NUM);
	UICREATE_RET_CHK(TextBox_InitLength_Num);
	TextBox_InitLength_Num->text.Set(ID_STR_0);
	TextBox_InitLength_Num->style.bg_shown = FALSE;
	TextBox_InitLength_Num->style.focusable = TRUE;
	TextBox_InitLength_Num->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_BLACK_TRANS);
	TextBox_InitLength_Num->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
	TextBox_InitLength_Num->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
	TextBox_InitLength_Num->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_MAIN);
	TextBox_InitLength_Num->style.text_fg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
	TextBox_InitLength_Num->style.text_fg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOR_BLUE);
	TextBox_InitLength_Num->style.text_fg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
	TextBox_InitLength_Num->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_InitLength_Num->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_InitLength_Num->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_InitLength_Num->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_InitLength_Num->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
	TextBox_InitLength_Num->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
	TextBox_InitLength_Num->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
	TextBox_InitLength_Num->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
	TextBox_InitLength_Num->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
	TextBox_InitLength_Num->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
	TextBox_InitLength_Num->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
	TextBox_InitLength_Num->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
	TextBox_InitLength_Num->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
	TextBox_InitLength_Num->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
	TextBox_InitLength_Num->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_InitLength_Num->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_InitLength_Num->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_InitLength_Num->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_InitLength_Num->style.text_align = E_WDG_ALIGN_MIDDLELEFT;
	AbsContainer_SnakePage->AddChild(TextBox_InitLength_Num);
#endif

    //================ Snake_MessageBox ================
    rc.set(400,250,360,80);
    Snake_MessageBox = new (std::nothrow) AbsContainer(this->root_cntr, rc, 0, ID_SNAKE_MESSAGEBOX);
    UICREATE_RET_CHK(Snake_MessageBox);
    Snake_MessageBox->style.bg_shown = FALSE;
    Snake_MessageBox->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    Snake_MessageBox->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    Snake_MessageBox->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
    Snake_MessageBox->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
    Snake_MessageBox->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    Snake_MessageBox->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    Snake_MessageBox->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
    Snake_MessageBox->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    Snake_MessageBox->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    Snake_MessageBox->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    Snake_MessageBox->SetFlag(E_FLAG_VISIBLE,FALSE);
    this->root_cntr->AddChild(Snake_MessageBox);

    //================ Image_MessageBoxBG ================
    rc.set(0,0,360,80);
    Image_MessageBoxBG = new (std::nothrow) Image(Snake_MessageBox, rc, 0, ID_SNAKE_IMAGE_MESSAGEBOXBG);
    UICREATE_RET_CHK(Image_MessageBoxBG);
    Image_MessageBoxBG->img[E_WDG_ACTIVE].Set(ID_IMAGE_POP_UP_BG1);//ID_IMAGE_POP_UP_BG1
    Image_MessageBoxBG->img[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    Image_MessageBoxBG->img[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    Image_MessageBoxBG->img[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    Image_MessageBoxBG->style.img_bg_shown = TRUE;
    Image_MessageBoxBG->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    Image_MessageBoxBG->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    Image_MessageBoxBG->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
    Image_MessageBoxBG->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
    Image_MessageBoxBG->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    Image_MessageBoxBG->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    Image_MessageBoxBG->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
    Image_MessageBoxBG->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    Image_MessageBoxBG->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    Image_MessageBoxBG->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    Snake_MessageBox->AddChild(Image_MessageBoxBG);
    //================ Snake_MessageBoxText ================
    rc.set(20,20,320,40);
    Snake_MessageBoxText = new (std::nothrow) TextBox(Snake_MessageBox, rc, 0, ID_SNAKE_TEXTBOX_MESSAGEBOXTEXT);
    UICREATE_RET_CHK(Snake_MessageBoxText);
    Snake_MessageBoxText->style.bg_shown = FALSE;
    Snake_MessageBoxText->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_RED);
    Snake_MessageBoxText->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    Snake_MessageBoxText->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    Snake_MessageBoxText->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
    Snake_MessageBoxText->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
    Snake_MessageBoxText->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    Snake_MessageBoxText->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    Snake_MessageBoxText->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
    Snake_MessageBoxText->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    Snake_MessageBoxText->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    Snake_MessageBoxText->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    Snake_MessageBoxText->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
    Snake_MessageBoxText->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
    Snake_MessageBoxText->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
    Snake_MessageBoxText->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
    Snake_MessageBoxText->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_24);
    Snake_MessageBoxText->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_24);
    Snake_MessageBoxText->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_24);
    Snake_MessageBoxText->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_24);
    Snake_MessageBoxText->style.text_align = E_WDG_ALIGN_MIDDLECENTER;
    Snake_MessageBox->AddChild(Snake_MessageBoxText);
	
    //================ Snake_AbsContainer1 ================
    //rc.set(178,159,750,450);
	rc.set(SNAKE_AREA_HSTART,SNAKE_AREA_VSTART,SNAKE_UNIT_SIZE*SNAKE_AREA_H,SNAKE_UNIT_SIZE*SNAKE_AREA_V);
    AbsContainer1 = new (std::nothrow) AbsContainer(AbsContainer_SnakePage, rc, 0, ID_SNAKE_ABSCONTAINER1);
    UICREATE_RET_CHK(AbsContainer1);
    AbsContainer1->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_80_152_76_255);
    AbsContainer1->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_80_152_76_255);
    AbsContainer1->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    AbsContainer1->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    AbsContainer1->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
    AbsContainer1->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
    AbsContainer1->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    AbsContainer1->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    AbsContainer1->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
    AbsContainer1->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    AbsContainer1->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    AbsContainer1->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    AbsContainer_SnakePage->AddChild(AbsContainer1);

#if SNAKE_SUPPORT_INIT_LENGTH_PAUSE
	/*================ init help info ================*/
	rc.set(SNAKE_AREA_HSTART,280,SNAKE_UNIT_SIZE*SNAKE_AREA_H,50);
	TextBox_InitHelpInfo = new (std::nothrow) TextBox(AbsContainer_SnakePage, rc, 0, ID_SNAKE_INIT_HELP_INFO);
	UICREATE_RET_CHK(TextBox_InitHelpInfo);
	TextBox_InitHelpInfo->text.Set("'Up/Down' Change InitLength");
	TextBox_InitHelpInfo->style.bg_shown = FALSE;
	TextBox_InitHelpInfo->style.focusable = FALSE;
	TextBox_InitHelpInfo->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_BLACK_TRANS);
	TextBox_InitHelpInfo->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
	TextBox_InitHelpInfo->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
	TextBox_InitHelpInfo->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
	TextBox_InitHelpInfo->style.text_fg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
	TextBox_InitHelpInfo->style.text_fg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
	TextBox_InitHelpInfo->style.text_fg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
	TextBox_InitHelpInfo->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_InitHelpInfo->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_InitHelpInfo->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_InitHelpInfo->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_InitHelpInfo->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
	TextBox_InitHelpInfo->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
	TextBox_InitHelpInfo->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
	TextBox_InitHelpInfo->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
	TextBox_InitHelpInfo->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
	TextBox_InitHelpInfo->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
	TextBox_InitHelpInfo->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
	TextBox_InitHelpInfo->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
	TextBox_InitHelpInfo->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
	TextBox_InitHelpInfo->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
	TextBox_InitHelpInfo->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_InitHelpInfo->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_InitHelpInfo->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_InitHelpInfo->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_InitHelpInfo->style.text_align = E_WDG_ALIGN_MIDDLECENTER;
	AbsContainer_SnakePage->AddChild(TextBox_InitHelpInfo);

	
	/*================ init help info ================*/
	rc.set(SNAKE_AREA_HSTART,350,SNAKE_UNIT_SIZE*SNAKE_AREA_H,50);
	TextBox_InitHelpInfo_1 = new (std::nothrow) TextBox(AbsContainer_SnakePage, rc, 0, ID_SNAKE_INIT_HELP_INFO_1);
	UICREATE_RET_CHK(TextBox_InitHelpInfo_1);
	TextBox_InitHelpInfo_1->text.Set("'OK' To Start");
	TextBox_InitHelpInfo_1->style.bg_shown = FALSE;
	TextBox_InitHelpInfo_1->style.focusable = FALSE;
	TextBox_InitHelpInfo_1->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_BLACK_TRANS);
	TextBox_InitHelpInfo_1->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
	TextBox_InitHelpInfo_1->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
	TextBox_InitHelpInfo_1->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
	TextBox_InitHelpInfo_1->style.text_fg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
	TextBox_InitHelpInfo_1->style.text_fg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
	TextBox_InitHelpInfo_1->style.text_fg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
	TextBox_InitHelpInfo_1->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_InitHelpInfo_1->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_InitHelpInfo_1->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_InitHelpInfo_1->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_InitHelpInfo_1->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
	TextBox_InitHelpInfo_1->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
	TextBox_InitHelpInfo_1->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
	TextBox_InitHelpInfo_1->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
	TextBox_InitHelpInfo_1->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
	TextBox_InitHelpInfo_1->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
	TextBox_InitHelpInfo_1->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
	TextBox_InitHelpInfo_1->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
	TextBox_InitHelpInfo_1->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
	TextBox_InitHelpInfo_1->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
	TextBox_InitHelpInfo_1->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_InitHelpInfo_1->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_InitHelpInfo_1->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_InitHelpInfo_1->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_InitHelpInfo_1->style.text_align = E_WDG_ALIGN_MIDDLECENTER;
	AbsContainer_SnakePage->AddChild(TextBox_InitHelpInfo_1);

	/*================ pause help info ================*/
	rc.set(SNAKE_AREA_HSTART,270,SNAKE_UNIT_SIZE*SNAKE_AREA_H,100);
	TextBox_PauseHelpInfo = new (std::nothrow) TextBox(AbsContainer_SnakePage, rc, 0, ID_SNAKE_PAUSE_HELP_INFO);
	UICREATE_RET_CHK(TextBox_PauseHelpInfo);
	TextBox_PauseHelpInfo->text.Set("'OK' To Restart");
	TextBox_PauseHelpInfo->style.bg_shown = FALSE;
	TextBox_PauseHelpInfo->style.focusable = FALSE;
	TextBox_PauseHelpInfo->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_BLACK_TRANS);
	TextBox_PauseHelpInfo->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
	TextBox_PauseHelpInfo->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
	TextBox_PauseHelpInfo->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
	TextBox_PauseHelpInfo->style.text_fg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
	TextBox_PauseHelpInfo->style.text_fg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
	TextBox_PauseHelpInfo->style.text_fg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
	TextBox_PauseHelpInfo->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_PauseHelpInfo->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_PauseHelpInfo->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_PauseHelpInfo->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_PauseHelpInfo->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
	TextBox_PauseHelpInfo->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
	TextBox_PauseHelpInfo->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
	TextBox_PauseHelpInfo->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
	TextBox_PauseHelpInfo->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
	TextBox_PauseHelpInfo->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
	TextBox_PauseHelpInfo->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
	TextBox_PauseHelpInfo->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
	TextBox_PauseHelpInfo->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
	TextBox_PauseHelpInfo->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
	TextBox_PauseHelpInfo->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_PauseHelpInfo->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_PauseHelpInfo->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_PauseHelpInfo->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_PauseHelpInfo->style.text_align = E_WDG_ALIGN_MIDDLECENTER;
	AbsContainer_SnakePage->AddChild(TextBox_PauseHelpInfo);
	TextBox_PauseHelpInfo->Hide();	
#endif


	//(*Initialize
	//*)
    
    //(*Setup
    //*)
#if SNAKE_SUPPORT_INIT_LENGTH_PAUSE
	ostringstream StrStream;
	
	AbsContainer_SnakePage->FocusChild(TextBox_InitLength_Info,FALSE); 
	AbsContainer_SnakePage->FocusChild(TextBox_InitLength_Num,FALSE); 

	sSnake._IsMoving = FALSE;
	sSnake._IsInited = FALSE;
	sSnake._InitLength = SNAKE_LENGTH_INIT;
	
	StrStream << (U32)(sSnake._InitLength);
	TextBox_InitLength_Num->text.str = StrStream.str();
	TextBox_InitLength_Num->text.SetFlag(E_MSTRING_INDEXED, false);
	TextBox_InitLength_Num->Invalidate();
#endif

#if !SNAKE_SUPPORT_INIT_LENGTH_PAUSE
	_initSnake();
#if NEW_METHOD
	_initFood();
#endif
    _ShowSnake();
    
	SetTimer(SHAKE_RENEWAL_TIME,ID_SNAKE_RENEWAL_TIMER,E_TIMER_CONTI);
#endif
    return false;

}

BOOL SnakeFrame::OnTimer(void* arg1, void* arg2, void* arg3)
{
	U32 TimerID = (U32)arg2;

    switch(TimerID)
    {
		case ID_SNAKE_RENEWAL_TIMER:
			_SnakeRenewal();
			break;
			
		case ID_SNAKE_MESSAGE_BOX_TIMER:
			{
			  KillTimer(ID_SNAKE_MESSAGE_BOX_TIMER);
			  Snake_MessageBox->Hide();
			  AbsContainer_SnakePage->Hide();
			  GetParentApp()->Kill();
			}
		    break;
			
		default:
			break;
    }
	
	return TRUE;
}

void SnakeFrame::_initSnake(void)
{
  //  Rect rc;
	U8 i;
	MSrv_Timer* pTimer = MSrv_Control::GetMSrvTimer();
    struct tm timeinfo;

	 pTimer->GetCLKTime(timeinfo);
#if SNAKE_SUPPORT_INIT_LENGTH_PAUSE
	sSnake.snake_longth = sSnake._InitLength;
#else
	sSnake.snake_longth = SNAKE_LENGTH_INIT;
#endif
	
	sSnake.snake_loction[0].loc_H = SNAKE_AREA_H/2;
	sSnake.snake_loction[0].loc_V = 0;
	sSnake.score = 0;

	for(i=1;i<=SNAKE_MAX_LONGTH;i++)
	{
		sSnake.snake_loction[i].loc_H = 0xFF;
		sSnake.snake_loction[i].loc_V = 0xFF;
	}
	printf("\nBEICL___initSnake_1");

	srand((unsigned int)(timeinfo.tm_sec));
	snake_rand = rand()%255;

#if !NEW_METHOD		
	 _SnakeGetFoodNewLocation(FALSE);
#endif
     
	sSnake.direction = GAME_SNAKE_DOWN;
}
#if NEW_METHOD
void SnakeFrame::_initFood(void)
{
	Rect rc;
	food._IsInited = FALSE;
	
	if(!food._IsInited)
	{
		food.location.loc_H = rand()%SNAKE_AREA_H;
		food.location.loc_V = rand()%SNAKE_AREA_V;

		rc.set(0,0, SNAKE_UNIT_SIZE, SNAKE_UNIT_SIZE);
		food.image = new (std::nothrow) Image(AbsContainer1, rc, 0, ID_SNAKE_FOOD_IMG);
		UICREATE_RET_CHK(food.image);
		food.image->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_SNAKE_FOOD);
		AbsContainer1->AddChild(food.image); 	   

		food.image->Move(SNAKE_UNIT_SIZE*food.location.loc_H,SNAKE_UNIT_SIZE*food.location.loc_V);
		
		food._IsInited = TRUE;
	}
}
#endif

void SnakeFrame::_SnakeGetFoodNewLocation(BOOL x)
{  
    Rect rc;
	int i,j;
	BOOL at_snake;
	BOOL food_flag;

	UNUSED(rc);
	UNUSED(j);

	food_flag = x;
	
	printf("\nBEICL____SnakeGetFoodNewLocation_S,%d",food_flag);

	//j=sSnake.food_loction.loc_H*25+sSnake.food_loction.loc_V;	
	//j=sSnake.food_loction.loc_H+sSnake.food_loction.loc_V*SNAKE_AREA_H;	//pangwz
#if !NEW_METHOD
	j = 199;
    if(food_flag)
	     Image1[j]->Hide(); 
	
	do
	{
		at_snake = FALSE;
		snake_rand = snake_rand *191;
		sSnake.food_loction.loc_H = snake_rand % SNAKE_AREA_H;
		sSnake.food_loction.loc_V = (snake_rand / SNAKE_AREA_H) % SNAKE_AREA_V;

		for(i=0;i<sSnake.snake_longth;i++)
		{
			if(sSnake.food_loction.loc_H == sSnake.snake_loction[i].loc_H  && sSnake.food_loction.loc_V == sSnake.snake_loction[i].loc_V)
			{
				at_snake = TRUE;
				printf("\nBEICL____at_snake = TRUE");
				break;
			}
		}			
	}while(at_snake);
	printf("\n ddd %d",i);
	printf("food coordinate x:%d,y:%d",sSnake.food_loction.loc_H,sSnake.food_loction.loc_V);//pangwz
	
///////// show food
     //i=sSnake.food_loction.loc_H*25+sSnake.food_loction.loc_V;
     //i=sSnake.food_loction.loc_H+sSnake.food_loction.loc_V*SNAKE_AREA_H;
     i=199;
	 rc.set(0,0, SNAKE_UNIT_SIZE, SNAKE_UNIT_SIZE);
	 Image1[i] = new (std::nothrow) Image(AbsContainer1, rc, 0, ID_SNAKE_IMAGE1+i);
     UICREATE_RET_CHK(Image1[i]);
	 Image1[i]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_SNAKE_FOOD);
     AbsContainer1->AddChild(Image1[i]);		
     Image1[i]->Move(SNAKE_UNIT_SIZE*sSnake.food_loction.loc_H,SNAKE_UNIT_SIZE*sSnake.food_loction.loc_V);
#else
	do
	{
		at_snake = FALSE;
		snake_rand = snake_rand *191;
		food.location.loc_H = snake_rand % SNAKE_AREA_H;
		food.location.loc_V = (snake_rand / SNAKE_AREA_H) % SNAKE_AREA_V;

		for(i=0;i<sSnake.snake_longth;i++)
		{
			if(food.location.loc_H == sSnake.snake_loction[i].loc_H  && food.location.loc_V == sSnake.snake_loction[i].loc_V)
			{
				at_snake = TRUE;
				printf("\nBEICL____at_snake = TRUE");
				break;
			}
		}			
	}while(at_snake);
	printf("\n ddd %d",i);
	printf("food coordinate x:%d,y:%d",food.location.loc_H,food.location.loc_V);//pangwz

	food.image->Move(SNAKE_UNIT_SIZE*food.location.loc_H,SNAKE_UNIT_SIZE*food.location.loc_V);
	
#endif

	 return;
}

void SnakeFrame:: _ShowSnake(void)	
 {

   Rect rc;
   U8 i; 
   for(i=0;i<sSnake.snake_longth;i++)
     {
       rc.set(0,0, SNAKE_UNIT_SIZE, SNAKE_UNIT_SIZE);
	   Image1[i] = new (std::nothrow) Image(AbsContainer1, rc, 0, ID_SNAKE_IMAGE1+i);
       UICREATE_RET_CHK(Image1[i]);

	   Image1[i]->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
	   Image1[i]->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
	   Image1[i]->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
	   Image1[i]->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
	   Image1[i]->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
	   Image1[i]->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);

		if(i==0)
		  {
		     switch(sSnake.direction)
			   {
				 case GAME_SNAKE_UP:
					   Image1[i]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_SNAKE_HEAD_UP);
					   break;
				 case GAME_SNAKE_DOWN:
					   Image1[i]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_SNAKE_HEAD_DOWN);
					   break;
				 case GAME_SNAKE_RIGHT:
					   Image1[i]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_SNAKE_HEAD_RIGHT);
					   break;
				 case GAME_SNAKE_LEFT:
					   Image1[i]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_SNAKE_HEAD_LEFT);
					   break;
				 default:
					   Image1[i]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_SNAKE_HEAD_LEFT);
					   break;
			    }
			}
		else if(i==sSnake.snake_longth-1)
			Image1[i]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_SNAKE_TAIL1);
		else if(i==sSnake.snake_longth-2)
			Image1[i]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_SNAKE_TAIL2);
		else
			Image1[i]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_SNAKE_BODY);
		
         AbsContainer1->AddChild(Image1[i]);	
         Image1[i]->Move(SNAKE_UNIT_SIZE*sSnake.snake_loction[0].loc_H,SNAKE_UNIT_SIZE*sSnake.snake_loction[0].loc_V);	
		 printf("\n_showsnake-pangwz");
		 
     }
   return;
}

void SnakeFrame::_SnakeRenewal(void)
{
	BOOL game_over = FALSE;
	BOOL eat_food = FALSE;
	S_GAME_SNAKE_LOCTION locationTemp;
	U8 i;
	ostringstream StrStream;

	//printf("\nBEICL___SnakeRenewal");
	locationTemp.loc_H = sSnake.snake_loction[0].loc_H;
	locationTemp.loc_V = sSnake.snake_loction[0].loc_V;

	switch(sSnake.direction)
	{
		case GAME_SNAKE_DOWN:
			
			if(locationTemp.loc_V==SNAKE_AREA_V-1)
				game_over = TRUE;
			else
				locationTemp.loc_V++; 
			break;
		case GAME_SNAKE_UP:
			
			if(locationTemp.loc_V==0)
				game_over = TRUE;
			else
				locationTemp.loc_V--;
			break;
		case GAME_SNAKE_LEFT:
			
			if(locationTemp.loc_H==0)
				game_over = TRUE;
			else
				locationTemp.loc_H--; 
			break;
		case GAME_SNAKE_RIGHT:
			
			
			if(locationTemp.loc_H==SNAKE_AREA_H-1)
				game_over = TRUE;
			else
				locationTemp.loc_H++;
			break;
		default:
			break;
	}
	//is gameover?
	for(i=1;i<sSnake.snake_longth;i++)
	{
		if(sSnake.snake_loction[i].loc_H == locationTemp.loc_H && sSnake.snake_loction[i].loc_V == locationTemp.loc_V )
		{
			game_over = TRUE;
			printf("\nBEICL___GAMEOVER");
			break;
		}
	}
	//game over
	if(game_over)
	{
        SetTimer(SHAKE_MASSEGE_TIMEOUT, ID_SNAKE_MESSAGE_BOX_TIMER, E_TIMER_ONESHOT);
	    Snake_MessageBox->Show();
	    Snake_MessageBoxText->text.Set(ID_STR_COIN_GAMEOVER);
		
		KillTimer(ID_SNAKE_RENEWAL_TIMER);
		return;
	}
#if NEW_METHOD	
	if(locationTemp.loc_H == food.location.loc_H && locationTemp.loc_V == food.location.loc_V)
#else
	if(locationTemp.loc_H == sSnake.food_loction.loc_H && locationTemp.loc_V == sSnake.food_loction.loc_V)
#endif
	{
       printf("\nBEICL___eat_food\n");
		eat_food = TRUE;
		sSnake.score++;
        StrStream << (U32)(sSnake.score);
	    TextBox_Score->text.str = StrStream.str();
	    TextBox_Score->text.SetFlag(E_MSTRING_INDEXED, false);
	    TextBox_Score->Invalidate();
		
		
		if(sSnake.score == SNAKE_MAX_LONGTH)
			{ 
			#if 0   //show good done goto next level
				MApp_ZUI_API_KillTimer(HWND_PLAY_AREA,0);
				enGameState = STATE_GAME_WAIT;
				MApp_ZUI_ACT_ExecuteGameAction(EN_EXE_GOTO_GAME_START_PAGE);
			#endif
				return;
			}
	}
//eat food 
	if(eat_food)
	{   printf("\nBEICL___AddSnakeLongth\n"); 
		sSnake.snake_longth++;
		_AddSnakeLongth();
		printf("\nBEICL___AddSnakeLongth1\n"); 
		_SnakeGetFoodNewLocation(TRUE);
	}

	for(i=sSnake.snake_longth -1;i>0;i--)
		{
			sSnake.snake_loction[i].loc_H = sSnake.snake_loction[i-1].loc_H;
			sSnake.snake_loction[i].loc_V = sSnake.snake_loction[i-1].loc_V;
		}
	sSnake.snake_loction[0].loc_H = locationTemp.loc_H;
	sSnake.snake_loction[0].loc_V = locationTemp.loc_V;

	//shuaxin OSD

	if(!game_over)
	 {
		  _SnakeDisplay();
	 }
	
	return;
}

void SnakeFrame::_SnakeDisplay(void)
{
	U8 i;
	//printf("DISPLAY_SNAKE");
	for(i=0;i<sSnake.snake_longth;i++)
	{
		if(sSnake.snake_loction[i].loc_H != 0xFF && sSnake.snake_loction[i].loc_V != 0xFF)
		{  
			Image1[i]->Move(SNAKE_UNIT_SIZE*sSnake.snake_loction[i].loc_H,SNAKE_UNIT_SIZE*sSnake.snake_loction[i].loc_V);
		}
		else
			break;
	}
	return;
}

void SnakeFrame:: _ChangeSnakeDirection(void)
{
	Rect rc;
	int i;
	UNUSED(rc);
//hide 	
	i=0;
#if 0
	Image1[i]->Hide();
//show another	
	i=0;
	rc.set(0,0, SNAKE_UNIT_SIZE, SNAKE_UNIT_SIZE);
    Image1[i] = new (std::nothrow) Image(AbsContainer1, rc, 0, ID_SNAKE_IMAGE1+i);
    UICREATE_RET_CHK(Image1);

	   Image1[i]->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
	   Image1[i]->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
	   Image1[i]->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
	   Image1[i]->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
	   Image1[i]->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
	   Image1[i]->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
#endif
		     switch(sSnake.direction)
			   {
				 case GAME_SNAKE_UP:
					   Image1[i]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_SNAKE_HEAD_UP);
					   break;
				 case GAME_SNAKE_DOWN:
					   Image1[i]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_SNAKE_HEAD_DOWN);
					   break;
				 case GAME_SNAKE_RIGHT:
					   Image1[i]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_SNAKE_HEAD_RIGHT);
					   break;
				 case GAME_SNAKE_LEFT:
					   Image1[i]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_SNAKE_HEAD_LEFT);
					   break;
				 default:
					   Image1[i]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_SNAKE_HEAD_LEFT);
					   break;
			    }	
#if 0
       AbsContainer1->AddChild(Image1[i]);
#endif
	   Image1[i]->Move(SNAKE_UNIT_SIZE*sSnake.snake_loction[i].loc_H,SNAKE_UNIT_SIZE*sSnake.snake_loction[i].loc_V);
	  return;  
}

void SnakeFrame::_AddSnakeLongth(void)
{
	Rect rc;
	rc.set(0,0, SNAKE_UNIT_SIZE, SNAKE_UNIT_SIZE);
    Image1[sSnake.snake_longth - 1] = new (std::nothrow) Image(AbsContainer1, rc, 0, ID_SNAKE_IMAGE1+sSnake.snake_longth - 1);
    UICREATE_RET_CHK(Image1);

    Image1[sSnake.snake_longth - 1]->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    Image1[sSnake.snake_longth - 1]->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    Image1[sSnake.snake_longth - 1]->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
    Image1[sSnake.snake_longth - 1]->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
    Image1[sSnake.snake_longth - 1]->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    Image1[sSnake.snake_longth - 1]->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);

	Image1[sSnake.snake_longth - 1]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_SNAKE_TAIL1);
	Image1[sSnake.snake_longth - 2]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_SNAKE_TAIL2);
	Image1[sSnake.snake_longth - 3]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_SNAKE_BODY);

    AbsContainer1->AddChild(Image1[sSnake.snake_longth - 1]);
	
	 return;  
}


BOOL SnakeFrame::OnKey(void* arg1, void* arg2, void* arg3)
{
	U32 keycode = (U32)arg2;	
	ostringstream StrStream;
	printf("\nSnake Onkey--%s", keycode == MVK_MENU 	? "MVK_MENU" :\
								keycode == MVK_UP 		? "UP" :\
								keycode == MVK_DOWN 	? "DOWN" :\
								keycode == MVK_SELECT	? "SELECT" :\
														  "DEFAULT"
							   	);
	switch(keycode)
	{
	    case MVK_POWER:
        case MVK_EXIT:	
	    case MVK_MENU:
		        GetParentApp()->Kill();
		        return TRUE;
		case MVK_UP:
			if(!sSnake._IsInited)
			{
				if(sSnake._InitLength < SNAKE_MAX_LONGTH -1)
					sSnake._InitLength++;

				StrStream << (U32)(sSnake._InitLength);
				TextBox_InitLength_Num->text.str = StrStream.str();
				TextBox_InitLength_Num->text.SetFlag(E_MSTRING_INDEXED, false);
				TextBox_InitLength_Num->Invalidate();
				return TRUE;
			}
			if(!sSnake._IsMoving)
				return TRUE;
			if(sSnake.direction != GAME_SNAKE_DOWN)
			    sSnake.direction = GAME_SNAKE_UP;
		    _ChangeSnakeDirection();
			return TRUE;
		case MVK_DOWN:
			if(!sSnake._IsInited)
			{
				if(sSnake._InitLength > 4)
					sSnake._InitLength--;
			
				StrStream << (U32)(sSnake._InitLength);
				TextBox_InitLength_Num->text.str = StrStream.str();
				TextBox_InitLength_Num->text.SetFlag(E_MSTRING_INDEXED, false);
				TextBox_InitLength_Num->Invalidate();
				return TRUE;
			}
			if(!sSnake._IsMoving)
				return TRUE;
			if(sSnake.direction != GAME_SNAKE_UP)
			     sSnake.direction = GAME_SNAKE_DOWN;	
		    _ChangeSnakeDirection();
			return TRUE;
		case MVK_LEFT:
			if(!sSnake._IsMoving)
				return TRUE;
			if(sSnake.direction != GAME_SNAKE_RIGHT)
			     sSnake.direction = GAME_SNAKE_LEFT;	
		    _ChangeSnakeDirection();
			return TRUE;
		case MVK_RIGHT:
			if(!sSnake._IsMoving)
				return TRUE;
			if(sSnake.direction != GAME_SNAKE_LEFT)
			     sSnake.direction = GAME_SNAKE_RIGHT;	
		    _ChangeSnakeDirection();
			return TRUE;
	#if SNAKE_SUPPORT_INIT_LENGTH_PAUSE
		case MVK_SELECT:
			{
				sSnake._IsMoving = (sSnake._IsMoving & 0x01)^1;
				if(!sSnake._IsInited)
				{
					sSnake._IsInited = TRUE;
                	_initSnake();
                #if NEW_METHOD
                	_initFood();
                #endif
                	_ShowSnake();
                	
					AbsContainer_SnakePage->UnfocusChild(TextBox_InitLength_Info,FALSE); 
					AbsContainer_SnakePage->UnfocusChild(TextBox_InitLength_Num,FALSE);
					AbsContainer_SnakePage->RemoveChild(TextBox_InitHelpInfo);
					AbsContainer_SnakePage->RemoveChild(TextBox_InitHelpInfo_1);
				}
				printf("\n IsStart--%s",sSnake._IsMoving == 1? "yes":"no");
				if(sSnake._IsMoving)
				{
					TextBox_PauseHelpInfo->Hide();	
            		SetTimer(SHAKE_RENEWAL_TIME,ID_SNAKE_RENEWAL_TIMER,E_TIMER_CONTI);
            	}
            	else
            	{
            		KillTimer(ID_SNAKE_RENEWAL_TIMER);					
					TextBox_PauseHelpInfo->Show();	
            	}
			}
			break;
	#endif
		default:
			return TRUE;
	}
  return TRUE;
}
/*@ </Operation ID=If58cf9dm12168c8928bmm640b> @*/

