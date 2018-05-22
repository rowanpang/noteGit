
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
#include "PushBoxFrame.h"
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
#include "MKeycode.h"

BEGIN_EVENT_MAP(PushBoxFrame, MMainFrame)
ON_CREATE(&PushBoxFrame::OnCreate)
ON_KEY(&PushBoxFrame::OnKey)
ON_TIMER(ID_PUSHBOX_MESSAGE_BOX_TIMER, &PushBoxFrame::OnTimer)
//(*EventMap
//*)
END_EVENT_MAP();

#define BOXGAMEDATA_OFFSET_BOY	0
#define BOXGAMEDATA_OFFSET_BOX	1
#define BOXGAMEDATA_OFFSET_BALL	7
#define BOXGAMEDATA_OFFSET_WALL_FLOOR	12

#define BOXGAMEDATA_MAX_BOX_NUM	6
#define BOXGAMEDATA_MAX_BALL_NUM	BOXGAMEDATA_MAX_BOX_NUM//must be equal

#define PUSHBOX_MAP_UNIT_H	12	//grid unit per horizon line
#define PUSHBOX_MAP_UNIT_V	10	//grid unit per row

U8  BoxGameData[PUSH_BOX_MAX_LEVEL][25]=
{
//level 1
	{									/*	1		*/
		0X57,									/*	boy		*/		//because boy and box may by move so this data just valid for map init; 
		0X46, 0X56, 0X67, 0X48, 0X00, 0X00,			/*	box		*/	
		0X26, 0X54, 0X49, 0X77, 0X00, 0X00,			/*	ball	*/		//the high hex bit for ball y coordinate,low hex bit for x; ball may be together with box or together with boy; y start from 0,x start from 1;
		0X00, 0X1c, 0X1c, 0Xf4, 0Xf7, 0X87, 0Xe1, 0Xef, 0X2f, 0X38, 0X38, 0X00/* wall&floor */	//total 12 bytes for 12 horizon unit; 
																							//per bit of one byte present the y coordinate and '0' or '1' for wall or floor;
	},

//level 2
	{									/*	2102	*/
		0X35, 								/*	boy 	*/
		0X38, 0X47, 0X57, 0X00, 0X00, 0X00,			/*	box 	*/
		0X27, 0X38, 0X57, 0X00, 0X00, 0X00,			/*	ball	*/
		0X00, 0X00, 0Xfe, 0Xfe, 0X82, 0Xaa, 0X82, 0X82, 0Xfe, 0Xfe, 0X00, 0X00
	},
//level 3

	{									/*	17		*/
		0X4a,									/*	boy		*/
		0X36, 0X38, 0X44, 0X48, 0X55, 0X56,			/*	box		*/
		0X67, 0X68, 0X75, 0X76, 0X77, 0X78,			/*	ball	*/
		0X00, 0X78, 0X48, 0Xcf, 0X81, 0X81, 0X89, 0Xc9, 0X4f, 0X48, 0X78, 0X00
	},
//level 4
	{									/*	44	*/
		0X27, 								/*	boy 	*/
		0X36, 0X37, 0X38, 0X58, 0X64, 0X66,			/*	box 	*/
		0X56, 0X65, 0X67, 0X69, 0X76, 0X78,			/*	ball	*/
		0X00, 0X1f, 0X11, 0X11, 0Xf3, 0X81, 0X8b, 0X81, 0Xfb, 0X0e, 0X00, 0X00
	},
//level 5
	{												/*	118 */
		0X66, 								/*	boy 	*/
		0X35, 0X44, 0X48, 0X54, 0X56, 0X58,			/*	box 	*/
		0X23, 0X24, 0X25, 0X26, 0X27, 0X28,			/*	ball	*/
		0X00, 0Xf8, 0X8e, 0X82, 0X82, 0X92, 0Xa2, 0X82, 0Xe2, 0X3e, 0X00, 0X00
	},
//level 6
	{												/*	118 */
		0X59,									/*	boy		*/
		0X34, 0X36, 0X38, 0X53, 0X55, 0X57,			/*	box		*/
		0X29, 0X39, 0X49, 0X44, 0X54, 0X64,			/*	ball	*/
		0X7e, 0X42, 0X42, 0Xc2, 0X92, 0X92, 0X96, 0X94, 0X84, 0X9c, 0Xf0, 0X00
	},
//level 7
	{												/*	118 */
		0X65,									/*	boy		*/
		0X34, 0X35, 0X36, 0X58, 0X00, 0X00,			/*	box		*/
		0X46, 0X47, 0X56, 0X57, 0X00, 0X00,			/*	ball	*/
		0X00, 0Xf8, 0X8e, 0X82, 0X92, 0X82, 0Xe2, 0X32, 0X12, 0X1e, 0X00, 0X00
	},
//level 8
	{												/*	118 */
		0X43,									/*	boy		*/
		0X34, 0X36, 0X46, 0X56, 0X66, 0X68,			/*	box		*/
		0X45, 0X46, 0X47, 0X55, 0X56, 0X57,			/*	ball	*/
		0X00, 0X7e, 0X43, 0X59, 0Xc1, 0X81, 0X83, 0X9a, 0Xc2, 0X7e, 0X00, 0X00
	},
//level 9
	{												/*	118 */
		0X29,									/*	boy		*/
		0X35, 0X36, 0X37, 0X56, 0X65, 0X00,			/*	box		*/
		0X26, 0X43, 0X46, 0X49, 0X66, 0X00,			/*	ball	*/
		0X00, 0X7e, 0X42, 0X52, 0Xd3, 0X81, 0X95, 0X91, 0X87, 0Xfc, 0X00, 0X00
	},
//level 10
	{												/*	118 */
		0X43,									/*	boy		*/
		0X37, 0X44, 0X4a, 0X54, 0X57, 0X00,			/*	box		*/
		0X27, 0X45, 0X47, 0X49, 0X67, 0X00,			/*	ball	*/
		0X78, 0X4e, 0Xc2, 0X82, 0Xaa, 0Xaa, 0X82, 0Xaa, 0Xaa, 0X82, 0X9e, 0X78
	},
//level 11
	{												/*	118 */
		0X79,									/*	boy		*/
		0X38, 0X47, 0X55, 0X58, 0X64, 0X67,			/*	box		*/
		0X35, 0X36, 0X46, 0X47, 0X55, 0X56,			/*	ball	*/
		0X00, 0X0f, 0X79, 0X41, 0X51, 0X43, 0Xcb, 0X81, 0Xa1, 0X8f, 0Xf8, 0X00
	},
//level 12
	{												/*	118 */
		0X5a,									/*	boy		*/
		0X44, 0X54, 0X58, 0X49, 0X59, 0X69,			/*	box		*/
		0X46, 0X47, 0X56, 0X57, 0X66, 0X67,			/*	ball	*/
		0X00, 0X78, 0X4e, 0X42, 0X42, 0X62, 0X22, 0X22, 0X22, 0X22, 0X3e, 0X00
	},
//level 13
	{												/*	118 */
		0X43,
		0X34, 0X46, 0X49, 0X58, 0X00, 0X00,
		0X54, 0X55, 0X64, 0X65, 0X00, 0X00,
		0X3c, 0Xfe, 0Xe6, 0X82, 0Xa2, 0Xae, 0Xa2, 0X82, 0Xc2, 0Xce, 0X7e, 0X3c
	},
//level 14
	{												/*	118 */
		0X36,
		0X37, 0X46, 0X57, 0X66, 0X77, 0X00,
		0X65, 0X75, 0X76, 0X77, 0X78, 0X00,
		0X00, 0X00, 0X7f, 0Xff, 0Xd9, 0X81, 0X81, 0Xf1, 0Xff, 0X1f, 0X00, 0X00
	},
//level 15
	{												/*	118 */
		0X25,
		0X36, 0X65, 0X78, 0X00, 0X00, 0X00,
		0X54, 0X64, 0X74, 0X00, 0X00, 0X00,
		0X00, 0X1f, 0Xff, 0Xf1, 0X99, 0X81, 0Xd9, 0Xc5, 0X71, 0X7f, 0X1f, 0X00
	},
//level 16
	{												/*	118 */
		0X2a,
		0X45, 0X47, 0X49, 0X56, 0X66, 0X00,
		0X73, 0X74, 0X75, 0X76, 0X77, 0X00,
		0X07, 0X07, 0X7d, 0Xfd, 0Xc1, 0X81, 0X89, 0Xed, 0X81, 0X87, 0Xff, 0X7e
	},
//level 17
	{												/*	118 */
		0X76,
		0X37, 0X46, 0X65, 0X66, 0X67, 0X00,
		0X35, 0X55, 0X56, 0X65, 0X66, 0X00,
		0X00, 0X0f, 0Xff, 0Xf9, 0X81, 0X81, 0X81, 0Xf9, 0Xff, 0X0f, 0X00, 0X00
	},
//level 18
	{												/*	118 */
		0X4a,
		0X36, 0X45, 0X47, 0X56, 0X58, 0X00,
		0X34, 0X43, 0X44, 0X53, 0X54, 0X00,
		0X3c, 0X7c, 0X64, 0Xc4, 0Xc4, 0X86, 0Xa6, 0Xa2, 0X82, 0Xee, 0Xfe, 0X1c
	},
//level 19
	{												/*	118 */
		0X44,
		0X36, 0X37, 0X38, 0X46, 0X55, 0X00,
		0X47, 0X48, 0X56, 0X57, 0X58, 0X00,
		0X00, 0X3c, 0X3c, 0Xe4, 0Xe6, 0X86, 0X82, 0X82, 0X8e, 0Xfe, 0Xf8, 0X00
	},
//level 20
	{												/*	118 */
		0X38,
		0X34, 0X58, 0X5a, 0X63, 0X64, 0X67,
		0X43, 0X53, 0X63, 0X49, 0X59, 0X69,
		0X1e, 0Xf3, 0X83, 0X81, 0Xf1, 0Xf7, 0X41, 0X45, 0X41, 0X67, 0X67, 0X1e
	}

};


/*@ <Operation ID=I53d2cee3m121525d89fbmm62bb> @*/
PushBoxFrame::PushBoxFrame(MApplet * pParentApp, Rect & stRect)
        : MMainFrame(pParentApp, stRect)
{
    /* -> add your code here */

    //(*MLoggerCtor
    strncpy(name,"PushBox",MAX_APPNAME_LENGTH-1);
    logid = MLogger_RegisterComponent(name, E_MLG_DARK_WHITE, E_MLG_DARK_BLACK);
    printf_info(" %s->Constructor called!!\n",name);
    printf_info("Component %s Registered!\n",name);
    //*)

}

PushBoxFrame::~PushBoxFrame()
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

BOOL PushBoxFrame::OnTimer(void* arg1, void* arg2, void* arg3)
{
    U32 TimerID = (U32)arg2;

    switch(TimerID)
    {
        case ID_PUSHBOX_MESSAGE_BOX_TIMER:
        {
            KillTimer(ID_PUSHBOX_MESSAGE_BOX_TIMER);
            PushBox_MessageBox->Hide();
			
			if(sPushBox.level == PUSH_BOX_MAX_LEVEL)
			     sPushBox.level = 1;
	        else
		         sPushBox.level++;
			
	        _removeAllChild();	
	        _initPushBox();
        }
        break;
		
        default:
            break;
    }

    return true;
}

/*@ <Operation ID=If58cf9dm12168c8928bmm640b> @*/
BOOL PushBoxFrame::OnCreate(void * arg1, void * arg2, void * arg3)
{
    (void)arg1;
    (void)arg2;
    (void)arg3;

    Rect rc;

    //================ AbsContainer_PushBoxPage ================
    rc.set(0,0,1366,768);
    AbsContainer_PushBoxPage = new (std::nothrow) AbsContainer(this->root_cntr, rc, 0, ID_PUSHBOX_ABSCONTAINER_PushBox_PAGE);
    UICREATE_RET_CHK(AbsContainer_PushBoxPage);
    AbsContainer_PushBoxPage->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    AbsContainer_PushBoxPage->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    AbsContainer_PushBoxPage->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    AbsContainer_PushBoxPage->style.bg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    AbsContainer_PushBoxPage->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_0);
    AbsContainer_PushBoxPage->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_0);
    AbsContainer_PushBoxPage->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_0);
    AbsContainer_PushBoxPage->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_0);
    AbsContainer_PushBoxPage->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_0);
    AbsContainer_PushBoxPage->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_0);
    AbsContainer_PushBoxPage->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
    AbsContainer_PushBoxPage->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    AbsContainer_PushBoxPage->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    AbsContainer_PushBoxPage->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    this->root_cntr->AddChild(AbsContainer_PushBoxPage);

	/*================ TextBox_Help_Info_1 ================*/
    rc.set(950,400,150,25);
    TextBox_Help_Info_1 = new (std::nothrow) TextBox(AbsContainer_PushBoxPage, rc, 0, ID_PUSHBOX_HELP_INFO_1);
    UICREATE_RET_CHK(TextBox_Help_Info_1);
    TextBox_Help_Info_1->text.Set(ID_STR_SUDOKU_HELP_INFO_1);
    TextBox_Help_Info_1->style.bg_shown = FALSE;
    TextBox_Help_Info_1->style.focusable = FALSE;
    TextBox_Help_Info_1->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_BLACK_TRANS);
    TextBox_Help_Info_1->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_1->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_1->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_MAIN);
    TextBox_Help_Info_1->style.text_fg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_1->style.text_fg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_1->style.text_fg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOUR_WHITE);
    TextBox_Help_Info_1->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_1->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_1->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_1->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_1->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Help_Info_1->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Help_Info_1->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_1->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_1->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_1->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_1->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_1->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_1->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_1->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_1->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_1->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_1->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_1->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_1->style.text_align = E_WDG_ALIGN_MIDDLERIGHT;
    AbsContainer_PushBoxPage->AddChild(TextBox_Help_Info_1);

    /*================ TextBox_Help_Info_1_1 ================*/
    rc.set(1100,400,150,25);
    TextBox_Help_Info_1_1 = new (std::nothrow) TextBox(AbsContainer_PushBoxPage, rc, 0, ID_PUSHBOX_HELP_INFO_1_1);
    UICREATE_RET_CHK(TextBox_Help_Info_1_1);
    TextBox_Help_Info_1_1->text.Set(ID_STR_SUDOKU_HELP_INFO_1_1);
    TextBox_Help_Info_1_1->style.bg_shown = FALSE;
    TextBox_Help_Info_1_1->style.focusable = FALSE;
    TextBox_Help_Info_1_1->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_BLACK_TRANS);
    TextBox_Help_Info_1_1->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_1_1->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_1_1->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_MAIN);
    TextBox_Help_Info_1_1->style.text_fg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_1_1->style.text_fg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_1_1->style.text_fg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOUR_WHITE);
    TextBox_Help_Info_1_1->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_1_1->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_1_1->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_1_1->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_1_1->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Help_Info_1_1->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Help_Info_1_1->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_1_1->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_1_1->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_1_1->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_1_1->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_1_1->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_1_1->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_1_1->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_1_1->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_1_1->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_1_1->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_1_1->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_1_1->style.text_align = E_WDG_ALIGN_MIDDLELEFT;
    AbsContainer_PushBoxPage->AddChild(TextBox_Help_Info_1_1);

    /*================ TextBox_Help_Info_2 ================*/
    rc.set(950,450,150,25);
    TextBox_Help_Info_2 = new (std::nothrow) TextBox(AbsContainer_PushBoxPage, rc, 0, ID_PUSHBOX_HELP_INFO_2);
    UICREATE_RET_CHK(TextBox_Help_Info_2);
    TextBox_Help_Info_2->text.Set(ID_STR_GAME_OK);
    TextBox_Help_Info_2->style.bg_shown = FALSE;
    TextBox_Help_Info_2->style.focusable = FALSE;
    TextBox_Help_Info_2->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_BLACK_TRANS);
    TextBox_Help_Info_2->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_2->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_2->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_MAIN);
    TextBox_Help_Info_2->style.text_fg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_2->style.text_fg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_2->style.text_fg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOUR_WHITE);
    TextBox_Help_Info_2->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_2->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_2->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_2->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_2->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Help_Info_2->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Help_Info_2->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_2->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_2->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_2->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_2->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_2->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_2->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_2->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_2->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_2->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_2->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_2->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_2->style.text_align = E_WDG_ALIGN_MIDDLERIGHT;
    AbsContainer_PushBoxPage->AddChild(TextBox_Help_Info_2);

    /*================ TextBox_Help_Info_2_1 ================*/
    rc.set(1110,450,150,25);
    TextBox_Help_Info_2_1 = new (std::nothrow) TextBox(AbsContainer_PushBoxPage, rc, 0, ID_PUSHBOX_HELP_INFO_2_1);
    UICREATE_RET_CHK(TextBox_Help_Info_2_1);
    TextBox_Help_Info_2_1->text.Set(ID_STR_GAME_BACK);
    TextBox_Help_Info_2_1->style.bg_shown = FALSE;
    TextBox_Help_Info_2_1->style.focusable = FALSE;
    TextBox_Help_Info_2_1->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_BLACK_TRANS);
    TextBox_Help_Info_2_1->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_2_1->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_2_1->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_MAIN);
    TextBox_Help_Info_2_1->style.text_fg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_2_1->style.text_fg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_2_1->style.text_fg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOUR_WHITE);
    TextBox_Help_Info_2_1->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_2_1->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_2_1->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_2_1->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_2_1->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Help_Info_2_1->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Help_Info_2_1->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_2_1->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_2_1->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_2_1->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_2_1->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_2_1->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_2_1->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_2_1->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_2_1->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_2_1->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_2_1->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_2_1->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_2_1->style.text_align = E_WDG_ALIGN_MIDDLELEFT;
    AbsContainer_PushBoxPage->AddChild(TextBox_Help_Info_2_1); 
	
    //================ TextBox_Help_Info_4_img ================
    rc.set(1050,497,30,30);
    Image_Help_Info_4 = new (std::nothrow) Image(AbsContainer_PushBoxPage, rc, 0, ID_PUSHBOX_HELP_INFO_img);
    UICREATE_RET_CHK(Image_Help_Info_4);
    Image_Help_Info_4->img[E_WDG_ACTIVE].Set(ID_IMG_SODUKU_HELP_INFO);
    Image_Help_Info_4->img[E_WDG_DISABLED].Set(ID_IMG_SODUKU_HELP_INFO);
    Image_Help_Info_4->img[E_WDG_FOCUSED].Set(ID_IMG_SODUKU_HELP_INFO);
    Image_Help_Info_4->img[E_WDG_SELECTED].Set(ID_IMG_SODUKU_HELP_INFO);
    Image_Help_Info_4->style.img_bg_shown = FALSE;//TRUE
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
    AbsContainer_PushBoxPage->AddChild(Image_Help_Info_4);

    /*================ TextBox_Help_Info_4 ================*/
    rc.set(1086,500,150,25);
    TextBox_Help_Info_4 = new (std::nothrow) TextBox(AbsContainer_PushBoxPage, rc, 0, ID_PUSHBOX_HELP_INFO_4);
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
    AbsContainer_PushBoxPage->AddChild(TextBox_Help_Info_4);

    /*================ TextBox_Help_Info_5 ================*/
    rc.set(950,550,150,25);
    TextBox_Help_Info_5 = new (std::nothrow) TextBox(AbsContainer_PushBoxPage, rc, 0, ID_PUSHBOX_HELP_INFO_5);
    UICREATE_RET_CHK(TextBox_Help_Info_5);
    TextBox_Help_Info_5->text.Set(ID_STR_SUDOKU_HELP_INFO_5);
    TextBox_Help_Info_5->style.bg_shown = FALSE;
    TextBox_Help_Info_5->style.focusable = FALSE;
    TextBox_Help_Info_5->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_BLACK_TRANS);
    TextBox_Help_Info_5->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_5->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_5->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_MAIN);
    TextBox_Help_Info_5->style.text_fg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_5->style.text_fg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_5->style.text_fg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOUR_WHITE);
    TextBox_Help_Info_5->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_5->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_5->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_5->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_5->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Help_Info_5->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Help_Info_5->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_5->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_5->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_5->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_5->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_5->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_5->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_5->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_5->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_5->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_5->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_5->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_5->style.text_align = E_WDG_ALIGN_MIDDLERIGHT;
    AbsContainer_PushBoxPage->AddChild(TextBox_Help_Info_5);

    /*================ TextBox_Help_Info_5_1 ================*/
    rc.set(1100,550,200,25);
    TextBox_Help_Info_5_1 = new (std::nothrow) TextBox(AbsContainer_PushBoxPage, rc, 0, ID_PUSHBOX_HELP_INFO_5_1);
    UICREATE_RET_CHK(TextBox_Help_Info_5_1);
    TextBox_Help_Info_5_1->text.Set(ID_STR_SUDOKU_HELP_INFO_5_1);
    TextBox_Help_Info_5_1->style.bg_shown = FALSE;
    TextBox_Help_Info_5_1->style.focusable = FALSE;
    TextBox_Help_Info_5_1->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_BLACK_TRANS);
    TextBox_Help_Info_5_1->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_5_1->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_5_1->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_MAIN);
    TextBox_Help_Info_5_1->style.text_fg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_5_1->style.text_fg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Help_Info_5_1->style.text_fg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOUR_WHITE);
    TextBox_Help_Info_5_1->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_5_1->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_5_1->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_5_1->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Help_Info_5_1->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Help_Info_5_1->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Help_Info_5_1->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_5_1->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_5_1->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_5_1->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    TextBox_Help_Info_5_1->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_5_1->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_5_1->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_5_1->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
    TextBox_Help_Info_5_1->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_5_1->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_5_1->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_5_1->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_25);
    TextBox_Help_Info_5_1->style.text_align = E_WDG_ALIGN_MIDDLELEFT;
    AbsContainer_PushBoxPage->AddChild(TextBox_Help_Info_5_1);

    /*================ TextBox_Help_Info_6 ================*/
    rc.set(950,600,150,25);
    TextBox_Help_Info_6 = new (std::nothrow) TextBox(AbsContainer_PushBoxPage, rc, 0, ID_PUSHBOX_HELP_INFO_6);
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
    AbsContainer_PushBoxPage->AddChild(TextBox_Help_Info_6);

    /*================ TextBox_Help_Info_6_1 ================*/
    rc.set(1100,600,200,25);
    TextBox_Help_Info_6_1 = new (std::nothrow) TextBox(AbsContainer_PushBoxPage, rc, 0, ID_PUSHBOX_HELP_INFO_6_1);
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
    AbsContainer_PushBoxPage->AddChild(TextBox_Help_Info_6_1);

     /*================ TextBox_Level ================*/
    rc.set(950,200,200,50);
    TextBox_Level_Info = new (std::nothrow) TextBox(AbsContainer_PushBoxPage, rc, 0, ID_PUSHBOX_LEVEL_INFO);
    UICREATE_RET_CHK(TextBox_Level_Info);
    TextBox_Level_Info->text.Set(ID_STR_SUDOKU_HELP_INFO_1);
    TextBox_Level_Info->style.bg_shown = FALSE;
    TextBox_Level_Info->style.focusable = FALSE;
    TextBox_Level_Info->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_BLACK_TRANS);
    TextBox_Level_Info->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Level_Info->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
    TextBox_Level_Info->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);//ID_COLOR_GRASS_GREEN
    TextBox_Level_Info->style.text_fg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
    TextBox_Level_Info->style.text_fg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
    TextBox_Level_Info->style.text_fg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_255);
    TextBox_Level_Info->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Level_Info->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Level_Info->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Level_Info->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
    TextBox_Level_Info->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Level_Info->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    TextBox_Level_Info->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
    TextBox_Level_Info->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    TextBox_Level_Info->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    TextBox_Level_Info->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    TextBox_Level_Info->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
    TextBox_Level_Info->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
    TextBox_Level_Info->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
    TextBox_Level_Info->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
    TextBox_Level_Info->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
    TextBox_Level_Info->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
    TextBox_Level_Info->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
    TextBox_Level_Info->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
    TextBox_Level_Info->style.text_align = E_WDG_ALIGN_MIDDLERIGHT;
    AbsContainer_PushBoxPage->AddChild(TextBox_Level_Info);

	rc.set(1170,200,50,50);
	TextBox_Level1 = new (std::nothrow) TextBox(AbsContainer_PushBoxPage, rc, 0, ID_PUSHBOX_LEVEL);
	UICREATE_RET_CHK(TextBox_Level1);
	TextBox_Level1->text.Set(" ");
	TextBox_Level1->style.bg_shown = FALSE;
	TextBox_Level1->style.focusable = FALSE;
	TextBox_Level1->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_BLACK_TRANS);
	TextBox_Level1->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
	TextBox_Level1->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
	TextBox_Level1->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_GRASS_GREEN);//ID_COLOR_GRASS_GREEN
	TextBox_Level1->style.text_fg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOR_GRASS_GREEN);
	TextBox_Level1->style.text_fg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOR_GRASS_GREEN);
	TextBox_Level1->style.text_fg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOR_GRASS_GREEN);
	TextBox_Level1->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_Level1->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_Level1->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_Level1->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_Level1->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
	TextBox_Level1->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
	TextBox_Level1->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
	TextBox_Level1->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
	TextBox_Level1->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
	TextBox_Level1->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
	TextBox_Level1->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
	TextBox_Level1->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
	TextBox_Level1->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
	TextBox_Level1->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
	TextBox_Level1->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_Level1->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_Level1->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_Level1->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_Level1->style.text_align = E_WDG_ALIGN_MIDDLELEFT;
	AbsContainer_PushBoxPage->AddChild(TextBox_Level1);

	rc.set(1220,200,50,50);
	TextBox_Level2 = new (std::nothrow) TextBox(AbsContainer_PushBoxPage, rc, 0, ID_PUSHBOX_LEVEL2);
	UICREATE_RET_CHK(TextBox_Level2);
	TextBox_Level2->text.Set(" ");
	TextBox_Level2->style.bg_shown = FALSE;
	TextBox_Level2->style.focusable = FALSE;
	TextBox_Level2->style.bg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_BLACK_TRANS);
	TextBox_Level2->style.bg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
	TextBox_Level2->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_BLACK);
	TextBox_Level2->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOR_GRASS_GREEN);//ID_COLOR_GRASS_GREEN
	TextBox_Level2->style.text_fg_color[E_WDG_DISABLED] = GetRcEngine::RcGetColor(ID_COLOR_GRASS_GREEN);
	TextBox_Level2->style.text_fg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOR_GRASS_GREEN);
	TextBox_Level2->style.text_fg_color[E_WDG_SELECTED] = GetRcEngine::RcGetColor(ID_COLOR_GRASS_GREEN);
	TextBox_Level2->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_Level2->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_Level2->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_Level2->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
	TextBox_Level2->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
	TextBox_Level2->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
	TextBox_Level2->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
	TextBox_Level2->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
	TextBox_Level2->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
	TextBox_Level2->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
	TextBox_Level2->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
	TextBox_Level2->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
	TextBox_Level2->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
	TextBox_Level2->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
	TextBox_Level2->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_Level2->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_Level2->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_Level2->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_48);
	TextBox_Level2->style.text_align = E_WDG_ALIGN_MIDDLELEFT;
	AbsContainer_PushBoxPage->AddChild(TextBox_Level2);

    //================ PushBox_MessageBox ================
    rc.set(400,250,360,80);
    PushBox_MessageBox = new (std::nothrow) AbsContainer(this->root_cntr, rc, 0, ID_PUSHBOX_MESSAGEBOX);
    UICREATE_RET_CHK(PushBox_MessageBox);
    PushBox_MessageBox->style.bg_shown = FALSE;
    PushBox_MessageBox->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    PushBox_MessageBox->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    PushBox_MessageBox->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
    PushBox_MessageBox->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
    PushBox_MessageBox->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    PushBox_MessageBox->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    PushBox_MessageBox->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
    PushBox_MessageBox->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    PushBox_MessageBox->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    PushBox_MessageBox->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    PushBox_MessageBox->SetFlag(E_FLAG_VISIBLE,FALSE);
    this->root_cntr->AddChild(PushBox_MessageBox);

    //================ Image_MessageBoxBG ================
    rc.set(0,0,360,80);
    Image_MessageBoxBG = new (std::nothrow) Image(PushBox_MessageBox, rc, 0, ID_PUSHBOX_IMAGE_MESSAGEBOXBG);
    UICREATE_RET_CHK(Image_MessageBoxBG);
    Image_MessageBoxBG->img[E_WDG_ACTIVE].Set(ID_IMAGE_POP_UP_BG1);
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
    PushBox_MessageBox->AddChild(Image_MessageBoxBG);
    //================ PushBox_MessageBoxText ================
    rc.set(20,20,320,40);
    PushBox_MessageBoxText = new (std::nothrow) TextBox(PushBox_MessageBox, rc, 0, ID_PUSHBOX_TEXTBOX_MESSAGEBOXTEXT);
    UICREATE_RET_CHK(PushBox_MessageBoxText);
    PushBox_MessageBoxText->style.bg_shown = FALSE;
    PushBox_MessageBoxText->style.text_fg_color[E_WDG_ACTIVE] = GetRcEngine::RcGetColor(ID_COLOUR_RED);
    PushBox_MessageBoxText->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    PushBox_MessageBoxText->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_3);
    PushBox_MessageBoxText->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_3);
    PushBox_MessageBoxText->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_3);
    PushBox_MessageBoxText->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    PushBox_MessageBoxText->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_4);
    PushBox_MessageBoxText->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT);
    PushBox_MessageBoxText->style.bg_image[E_WDG_DISABLED].Set(ID_IMAGE_DEFAULT);
    PushBox_MessageBoxText->style.bg_image[E_WDG_FOCUSED].Set(ID_IMAGE_DEFAULT);
    PushBox_MessageBoxText->style.bg_image[E_WDG_SELECTED].Set(ID_IMAGE_DEFAULT);
    PushBox_MessageBoxText->style.font_style[E_WDG_ACTIVE].family = ID_FONT_DEFAULT;
    PushBox_MessageBoxText->style.font_style[E_WDG_DISABLED].family = ID_FONT_DEFAULT;
    PushBox_MessageBoxText->style.font_style[E_WDG_FOCUSED].family = ID_FONT_DEFAULT;
    PushBox_MessageBoxText->style.font_style[E_WDG_SELECTED].family = ID_FONT_DEFAULT;
    PushBox_MessageBoxText->style.font_style[E_WDG_ACTIVE].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_24);
    PushBox_MessageBoxText->style.font_style[E_WDG_DISABLED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_24);
    PushBox_MessageBoxText->style.font_style[E_WDG_FOCUSED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_24);
    PushBox_MessageBoxText->style.font_style[E_WDG_SELECTED].size = GetRcEngine::RcGetNum(ID_FONT_SIZE_24);
    PushBox_MessageBoxText->style.text_align = E_WDG_ALIGN_MIDDLECENTER;
    PushBox_MessageBox->AddChild(PushBox_MessageBoxText);
	
    //================ GridContainer1 ================
    //rc.set(103,34,840,700);
    rc.set(PUSHBOX_MAP_H_START,PUSHBOX_MAP_V_START,PUSHBOX_UNIT_SIZE*PUSHBOX_MAP_UNIT_H,PUSHBOX_UNIT_SIZE*PUSHBOX_MAP_UNIT_V);
    GridContainer1 = new (std::nothrow) GridContainer(AbsContainer_PushBoxPage, rc, 0, ID_PUSHBOX_GRIDCONTAINER1, 2, 2);
    UICREATE_RET_CHK(GridContainer1);
    GridContainer1->style.bg_color[E_WDG_FOCUSED] = GetRcEngine::RcGetColor(ID_COLOUR_255_255_255_0);
    GridContainer1->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_0);
    GridContainer1->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_0);
    GridContainer1->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_0);
    GridContainer1->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_0);
    GridContainer1->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_0);
    GridContainer1->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_0);
    GridContainer1->style.bg_image[E_WDG_ACTIVE].Set(ID_IMG_GAME_PUSHBOX_WALL);
    GridContainer1->style.bg_image[E_WDG_DISABLED].Set(ID_IMG_GAME_PUSHBOX_WALL);
    GridContainer1->style.bg_image[E_WDG_FOCUSED].Set(ID_IMG_GAME_PUSHBOX_WALL);
    GridContainer1->style.bg_image[E_WDG_SELECTED].Set(ID_IMG_GAME_PUSHBOX_WALL);
    //GridContainer1->SetRowColumn(10,12);
    GridContainer1->SetRowColumn(PUSHBOX_MAP_UNIT_V,PUSHBOX_MAP_UNIT_H);
    AbsContainer_PushBoxPage->AddChild(GridContainer1);
	
	//(*Initialize
	//*)
    
	sPushBox.level=1;
    _initPushBox();

	//(*Setup
	//*)	

    return false;
}

void PushBoxFrame::_initPushBox(void)

{
	U8  i,j,k;
	U8  *GameDataPointer;
	GameDataPointer = &BoxGameData[sPushBox.level - 1][0];
	/*------initial ram location-------*/
	location[0].boy = *GameDataPointer;
	for(i = 0; i < BOXGAMEDATA_MAX_BOX_NUM;++i)
	{
		++GameDataPointer;
		location[0].box[i] = *GameDataPointer;	//for map init and move box in _GetPushBoxMap
	}
	/*------clear undo-------*/
	for(i = 1;i < UNDOMAX+1; ++i) location[i].boy = 0; 

 // show map	
	 for(k=0;k<PUSHBOX_MAP_UNIT_V;k++)
	 {
	    for(j=0;j<PUSHBOX_MAP_UNIT_H;j++) 
	     {
         	_ShowPushBoxMap(j,k,_GetPushBoxMap(j+1,k));//j+1,for pointer to the correspond horizon unit;
	     }
	 }
	
     SwitchFocusTo(GridContainer1->GetChild(0));
	 ClearFocus();
	 
//  Invalidate score 
   if(sPushBox.level<10)
	{
		TextBox_Level1->text.Set(ID_STR_NUM_0 + (sPushBox.level&0x0F));
		TextBox_Level2->text.Set(" ");
	}
	else if((sPushBox.level>=10)&&(sPushBox.level<20))
	{
		TextBox_Level1->text.Set(ID_STR_NUM_1);
		TextBox_Level2->text.Set(ID_STR_NUM_0 + ((sPushBox.level - 10)&0x0F));
	}
	else
	{	
		TextBox_Level1->text.Set(ID_STR_NUM_2);
		TextBox_Level2->text.Set(ID_STR_NUM_0);
	}
	TextBox_Level1->Invalidate();
	TextBox_Level2->Invalidate();
	
}

void PushBoxFrame::_removeAllChild(void)
{
	U8 k,j;
	for(k=0;k<PUSHBOX_MAP_UNIT_V;k++)
		for(j=0;j<PUSHBOX_MAP_UNIT_H;j++)
	  {
		GridContainer1->RemoveChild(Image1[k*PUSHBOX_MAP_UNIT_H+j]);
		delete Image1[k*PUSHBOX_MAP_UNIT_H +j];	
		Image1[k*PUSHBOX_MAP_UNIT_H +j] = NULL;				
	  }
}

U8 PushBoxFrame:: _GetPushBoxMap(U8 x,U8 y)
{

   U8  GameData;	
   U8  Mapinfo;	
   U8  i;	
   U8  *GameDataPointer;
	
   GameDataPointer = &BoxGameData[sPushBox.level- 1][0];	
   GameDataPointer += BOXGAMEDATA_OFFSET_WALL_FLOOR;	
   GameDataPointer += x;	
   GameData = *GameDataPointer;
	
   for(i = 1;i < y;++i) GameData <<= 1;	
   
   if((GameData&0x80) == 0x80) Mapinfo = Wall;	
   else Mapinfo = Floor;	
   if(y == 0)	//first line are always floor;
    {	
	  Mapinfo = Floor;	
    }
	
   GameDataPointer = &BoxGameData[sPushBox.level - 1][0];	
   GameDataPointer += BOXGAMEDATA_OFFSET_BALL;	
   x |= (y << 4);	//coordinate expression for boy/box/ball	
   for(i = 0;i < BOXGAMEDATA_MAX_BALL_NUM;++i)	
     {	
	    if(*GameDataPointer == x) Mapinfo = Ball;	
	        ++GameDataPointer;	
     }	
   if( location[0].boy == x ) Mapinfo |= Boy;	
   for(i = 0;i < 6;++i)	
     {	
	    if(location[0].box[i] == x) Mapinfo |= Box;
     }
   
    return Mapinfo;
}

void PushBoxFrame:: _ShowPushBoxMap(U8 x,U8 y,U8 Mapinfo)	//coordinat x for x,y for y;
 {
     Rect rc;
     //rc.set(103+70*y,34+70*x,70,70);
     rc.set(PUSHBOX_MAP_H_START+PUSHBOX_UNIT_SIZE*x,PUSHBOX_MAP_V_START+PUSHBOX_UNIT_SIZE*y,PUSHBOX_UNIT_SIZE,PUSHBOX_UNIT_SIZE);
     Image1[y*PUSHBOX_MAP_UNIT_H+x] = new (std::nothrow) Image(GridContainer1, rc, 0, ID_PUSHBOX_IMAGE1+y*PUSHBOX_MAP_UNIT_H+x);
     UICREATE_RET_CHK(Image1[y*PUSHBOX_MAP_UNIT_H+x]);
	 
	 switch(Mapinfo)
	{
         case GAME_BITMAP_FLOOR:
			Image1[y*PUSHBOX_MAP_UNIT_H+x]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_PUSHBOX_WALL);
			break;
		case GAME_BITMAP_WALL:	
			Image1[y*PUSHBOX_MAP_UNIT_H+x]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_PUSHBOX_HALL); 
               break;	
		case GAME_BITMAP_BALL:	
			Image1[y*PUSHBOX_MAP_UNIT_H+x]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_PUSHBOX_AIM);	 
               break;
		case GAME_BITMAP_BOX:	
			Image1[y*PUSHBOX_MAP_UNIT_H+x]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_PUSHBOX_BOX);	 
               break;	
		case GAME_BITMAP_BOY:	
			Image1[y*PUSHBOX_MAP_UNIT_H+x]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_PUSHBOX_MAN);
               break;	
		case GAME_BITMAP_BOY_AT_BALL:	
			Image1[y*PUSHBOX_MAP_UNIT_H+x]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_PUSHBOX_MANATAIM);	
               break;	
		case GAME_BITMAP_BOX_AT_BALL:	
			Image1[y*PUSHBOX_MAP_UNIT_H+x]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_PUSHBOX_BOXATAIM);	
               break;	
		default:	
			Image1[y*PUSHBOX_MAP_UNIT_H+x]->img[E_WDG_ACTIVE].Set(ID_IMG_GAME_PUSHBOX_BOXATAIM);	
			break;
	  }
 
         Image1[y*PUSHBOX_MAP_UNIT_H+x]->style.padding[E_WDG_PADDING_LEFT] = GetRcEngine::RcGetNum(ID_PADDING_0);
         Image1[y*PUSHBOX_MAP_UNIT_H+x]->style.padding[E_WDG_PADDING_RIGHT] = GetRcEngine::RcGetNum(ID_PADDING_0);
         Image1[y*PUSHBOX_MAP_UNIT_H+x]->style.padding[E_WDG_PADDING_TOP] = GetRcEngine::RcGetNum(ID_PADDING_0);
         Image1[y*PUSHBOX_MAP_UNIT_H+x]->style.padding[E_WDG_PADDING_BOTTOM] = GetRcEngine::RcGetNum(ID_PADDING_0);
         Image1[y*PUSHBOX_MAP_UNIT_H+x]->style.padding[E_WDG_PADDING_INT] = GetRcEngine::RcGetNum(ID_PADDING_0); 
         Image1[y*PUSHBOX_MAP_UNIT_H+x]->style.padding[E_WDG_PADDING_EXT] = GetRcEngine::RcGetNum(ID_PADDING_0); 
         Image1[y*PUSHBOX_MAP_UNIT_H+x]->style.bg_image[E_WDG_ACTIVE].Set(ID_IMAGE_DEFAULT); 
         GridContainer1->AddChild(Image1[y*PUSHBOX_MAP_UNIT_H+x]);		
	     Image1[y*PUSHBOX_MAP_UNIT_H+x]->Show();
   }

BOOL PushBoxFrame::OnKey(void* arg1, void* arg2, void* arg3)
{
	
  U32 keycode = (U32)arg2;	
  Rect rc;	
  //rc.set(300,70,630,630);
  rc.set(103,34,840,700);
 
  U8  *GameDataPointer;	
  U8  i,l;	
  U8  j,k;
  
  U8  winnum;	
  U8  box_aim_location_x;	
  U8  box_aim_location_y;	
  U8  boy_aim_location_x;	
  U8  boy_aim_location_y;

  printf("\n PushBox Onkey--%s", 	keycode == MVK_MENU	  ? "MVK_MENU" :\
									keycode == MVK_UP 	  ? "UP" :\
									keycode == MVK_DOWN	  ? "DOWN" :\
									keycode == MVK_SELECT ? "SELECT" :\
															"CHECK"
							  );
	
   //SwitchFocusTo(GridContainer1->GetChild(0));	pangwz for test;
   
   switch(keycode) 
   {
	
	    case MVK_POWER:
        case MVK_EXIT:	
	    case MVK_MENU:
		        GetParentApp()->Kill();
		        return TRUE;
		
	    case MVK_RIGHT:
     			boy_aim_location_y = (location[0].boy) >>4;
     			boy_aim_location_x = ((location[0].boy)&0x0f) + 1;
     			break;
  		case MVK_LEFT:
     			boy_aim_location_y = (location[0].boy) >>4; 
     			boy_aim_location_x = ((location[0].boy)&0x0f) - 1;				 
     			break; 
  		case MVK_UP: 
      			boy_aim_location_x = (location[0].boy) & 0x0f; 
      			boy_aim_location_y = ((location[0].boy) >>4) - 1; 
     			break; 
  		case MVK_DOWN: 
      			boy_aim_location_x = (location[0].boy) & 0x0f;
      			boy_aim_location_y = ((location[0].boy) >>4) + 1; 
      			break;	
				
		 case(MVK_SELECT):
       			PushBox_CallUndo();			
       			return TRUE;
	    case MVK_VOLUME_PLUS:	
		     {
			    if(sPushBox.level == PUSH_BOX_MAX_LEVEL)
				       sPushBox.level = 1;
			    else
				       sPushBox.level++;
			    _removeAllChild();	
			    _initPushBox();
			    return TRUE;
	         }	
	
	    case MVK_VOLUME_MINUS:
		     {
			    if(sPushBox.level == 1)
				       sPushBox.level = PUSH_BOX_MAX_LEVEL;
		    	else
				       sPushBox.level--;	
			    _removeAllChild();
		        _initPushBox();
		        return TRUE;
		     }
     	default:
			{
				ClearFocus();
     			return TRUE;
     		}
 	}
		
	switch(_GetPushBoxMap(boy_aim_location_x,boy_aim_location_y))	
	{
		case Floor:
		case Ball:
			location[0].boy = (boy_aim_location_y <<4) | boy_aim_location_x;

            _removeAllChild();
            for(k=0;k<10;k++)
	         {
	           for(j=0;j<PUSHBOX_MAP_UNIT_H;j++) 
	             {
                   _ShowPushBoxMap(j,k,_GetPushBoxMap(j+1,k));
	             }
	         }
	         
		case Wall:
			break;
	
		case Box:
		case (Box|Ball):  

			for(i = 0;i < 6;++i)	/*	which box will be move	*/
			{
				if((location[0].box[i]) == ((boy_aim_location_y << 4) | boy_aim_location_x)) break;
			}
			switch(keycode)
			{
				case MVK_RIGHT:
					box_aim_location_y = boy_aim_location_y;
					box_aim_location_x = boy_aim_location_x + 1;
					break;
				case MVK_LEFT:
					box_aim_location_y = boy_aim_location_y;
					box_aim_location_x = boy_aim_location_x - 1;
					break;
				case MVK_UP:
					box_aim_location_x = boy_aim_location_x;
					box_aim_location_y = boy_aim_location_y - 1;
					break;
				case MVK_DOWN:
					box_aim_location_x = boy_aim_location_x;
					box_aim_location_y = boy_aim_location_y + 1;
				default:
					break;
			}

			switch(_GetPushBoxMap(box_aim_location_x,box_aim_location_y))
			{
				case Floor:
				case Ball:
					PushBox_SaveUndo();
					location[0].boy = (boy_aim_location_y <<4) |boy_aim_location_x;
					location[0].box[i] = (box_aim_location_y << 4) | box_aim_location_x;

                    _removeAllChild();
                    for(k=0;k<10;k++)
					{
						for(j=0;j<PUSHBOX_MAP_UNIT_H;j++) 
						{
							_ShowPushBoxMap(j,k,_GetPushBoxMap(j+1,k));
						}
					}
					break;
				default:
					 break;
			}
			break;
		
	}
	/*---game win----*/
	winnum = 0;
	for(i = 0; i < 6; ++i)
	{
		GameDataPointer = &BoxGameData[sPushBox.level - 1][0];
		
		GameDataPointer += BOXGAMEDATA_OFFSET_BALL;
		
		for(l = 0; l< BOXGAMEDATA_MAX_BALL_NUM;++l)		
		{			
			
			if(location[0].box[i] == *GameDataPointer)
			{
				++winnum;
				break;
			}
			++GameDataPointer;
		}
	}

	if(winnum == 6)
	{
      SetTimer(PUSHBOX_MESSAGEBOX_TIME, ID_PUSHBOX_MESSAGE_BOX_TIMER, E_TIMER_ONESHOT);
	  PushBox_MessageBox->Show();
	  PushBox_MessageBoxText->text.Set(ID_STR_GAME_SUDOKU_CONGRATULATE);
	}
   SwitchFocusTo(GridContainer1->GetChild(0));
   ClearFocus();
  return TRUE; //FALSE;
}
//can be optimize as stack push/pop-pangwz
 void PushBoxFrame:: PushBox_SaveUndo(void)
{
	U8  i;
	U8  j;
	location_struct *PtrUndo1;
	location_struct *PtrUndo2;
	PtrUndo1 = &location[1];
	for(i = 0;i < UNDOMAX; ++i) 
	{
		if(PtrUndo1->boy == 0)
		{
			PtrUndo1->boy = location[0].boy;
			for(j = 0;j < 6;++j) PtrUndo1->box[j] = location[0].box[j];
			return;
		}
		++PtrUndo1;
	}
	PtrUndo1 = &location[0];
	PtrUndo2 = &location[1];
	for(i = 0;i < UNDOMAX - 1;++i)
	{
		++PtrUndo1;
		++PtrUndo2;
		PtrUndo1->boy = PtrUndo2->boy;
		for(j = 0;j < 6;++j) PtrUndo1->box[j] = PtrUndo2->box[j];
	}
	location[UNDOMAX].boy = location[0].boy;
	for(i = 0;i < 6;++i) location[UNDOMAX].box[i] = location[0].box[i];
}
 
void PushBoxFrame:: PushBox_CallUndo(void)
{
	U8 i,l;
	U8 j,k;
	for(i = UNDOMAX; i >0;  --i)
	{
		if(location[i].boy != 0)
		{
			location[0].boy = location[i].boy;
			for(l = 0;l < BOXGAMEDATA_MAX_BOX_NUM;++l) location[0].box[l] = location[i].box[l];
			location[i].boy = 0;

            _removeAllChild();
            for(k=0;k<PUSHBOX_MAP_UNIT_V;k++)
	          {
	            for(j=0;j<PUSHBOX_MAP_UNIT_H;j++) 
	             {
                    _ShowPushBoxMap(j,k,_GetPushBoxMap(j+1,k));
	             }
	          }
			break;
		}
	}
	SwitchFocusTo(GridContainer1->GetChild(0));
    ClearFocus();
}


/*@ </Operation ID=If58cf9dm12168c8928bmm640b> @*/

