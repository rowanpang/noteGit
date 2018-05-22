#ifndef __PUSHBOXFRAME_H__
#define __PUSHBOXFRAME_H__

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
#include "MMainFrame.h"
#include "MApplet.h"
/*@ </Include> @*/

//(*MLoggerDefine
#define MAX_APPNAME_LENGTH 100
//*)

//(*InternalHeaders
#include "AutoGenHeader.h"
//*)
#include "GridContainer.h"

#include "Image.h"
#include "TextBox.h"

#define PUSH_BOX_MAX_LEVEL 20
#define	Floor		0x0	/* should be 0 */
#define	Wall		0x01
#define	Ball		0x02
#define	Boy			0x04
#define Boy_at_Ball 0x06
#define	Box			0x08
#define UNDOMAX     6
#define PLAY_AREA_MAX_ICON 150
#define PUSHBOX_MESSAGEBOX_TIME	3000

#define PUSHBOX_MAP_H_START	103
#define PUSHBOX_MAP_V_START	34

#define PUSHBOX_UNIT_SIZE	70


typedef struct
{
	U8 level;
	U16 best_score[PUSH_BOX_MAX_LEVEL];
	U16 score[PUSH_BOX_MAX_LEVEL];
}S_GAME_PUSH_BOX;

typedef enum
{
	MOVE_UP,
	MOVE_DOWN,
	MOVE_RIGHT,
	MOVE_LEFT,
}EN_MOVE_DIRECTION;

typedef enum
{
    GAME_BITMAP_FLOOR,
    GAME_BITMAP_WALL,
    GAME_BITMAP_BALL,
    GAME_BITMAP_BOY = 4,
    GAME_BITMAP_BOY_AT_BALL=6,
    GAME_BITMAP_BOX = 8,
    GAME_BITMAP_BOX_AT_BALL = 10,
} EN_GAME_PUSH_BITMAP_TYPE;

enum {
    //(*Identifiers
    //*)
    ID_PUSHBOX_GRIDCONTAINER1,
    ID_PUSHBOX_HELP_INFO_1,
	ID_PUSHBOX_HELP_INFO_1_1,
	ID_PUSHBOX_HELP_INFO_2,
	ID_PUSHBOX_HELP_INFO_2_1,
	ID_PUSHBOX_HELP_INFO_4,
	ID_PUSHBOX_HELP_INFO_5,
	ID_PUSHBOX_HELP_INFO_5_1,
	ID_PUSHBOX_HELP_INFO_6,
	ID_PUSHBOX_HELP_INFO_6_1,
	ID_PUSHBOX_HELP_INFO_img,
	ID_PUSHBOX_LEVEL_INFO,
	ID_PUSHBOX_LEVEL,
	ID_PUSHBOX_LEVEL2,
	ID_PUSHBOX_MESSAGEBOX,
	ID_PUSHBOX_IMAGE_MESSAGEBOXBG,
	ID_PUSHBOX_TEXTBOX_MESSAGEBOXTEXT,
    ID_PUSHBOX_ABSCONTAINER_PushBox_PAGE,
    ID_PUSHBOX_IMAGE1,
    ID_PUSHBOX_COUNT, //=121,
};



typedef struct
{
  	char	boy;		/* BOY */
	char	box[6];	/* BOX POSISION */
} location_struct;

enum
{
    ID_PUSHBOX_IDLE_TIMER = 0,
    ID_PUSHBOX_MESSAGE_NOT_SUPPORT,
	ID_PUSHBOX_MESSAGE_BOX_TIMER,
    ID_PUSHBOX_TIMER_COUNT
};

/*@ <Definitions> @*/
class PushBoxFrame : public MMainFrame
{
    DECLARE_EVENT_MAP();

public:

    // ------------------------------------------------------------
    // public constructors
    // ------------------------------------------------------------

    PushBoxFrame(MApplet * pParentApp, Rect & stRect);
    ~PushBoxFrame();

    // ------------------------------------------------------------
    // public operations
    // ------------------------------------------------------------

    //(*MLoggerDeclare
    unsigned int logid;
    char name[MAX_APPNAME_LENGTH];
    //*)

    //(*Declare
    //*)
    GridContainer* GridContainer1;
    AbsContainer* AbsContainer_PushBoxPage;
	TextBox* TextBox_Help_Info_1;
    TextBox* TextBox_Help_Info_1_1;
    TextBox* TextBox_Help_Info_2;
    TextBox* TextBox_Help_Info_2_1;
    TextBox* TextBox_Help_Info_4;
    TextBox* TextBox_Help_Info_5;
    TextBox* TextBox_Help_Info_5_1;
    TextBox* TextBox_Help_Info_6;
    TextBox* TextBox_Help_Info_6_1;
	Image* Image_Help_Info_4;
    Image* Image1[120];

	TextBox* TextBox_Level_Info;
    TextBox* TextBox_Level1;
    TextBox* TextBox_Level2;

	AbsContainer* PushBox_MessageBox;
    Image* Image_MessageBoxBG;
    TextBox* PushBox_MessageBoxText;

    BOOL OnCreate(void * arg1, void * arg2, void * arg3);

    //(*EventHandler
    BOOL OnKey(void*, void*, void*);
    //*)

protected:
	S_GAME_PUSH_BOX sPushBox;
	location_struct  location[UNDOMAX +1];//one location for one step,location[0] for the cur step,and location[6] for backup the cur step -releate with fun PushBox_SaveUndo;
    void _initPushBox(void);
	void _reflashLevel(void);
	U8 _GetPushBoxMap(U8 x,U8 y);
	void PushBox_SaveUndo(void);
	void PushBox_CallUndo(void);
	void _ShowPushBoxMap(U8 i,U8 j,U8 Mapinfo);
	void _removeAllChild(void);
	BOOL OnTimer(void* arg1, void* arg2, void* arg3);
	
};
/*@ </Definitions> @*/

#endif
