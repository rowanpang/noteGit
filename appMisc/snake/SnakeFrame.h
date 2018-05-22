#ifndef __SNAKEFRAME_H__
#define __SNAKEFRAME_H__

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
#include "AbsContainer.h"
#include "Image.h"
#include "TextBox.h"

#define SNAKE_AREA_H   25
#define SNAKE_AREA_V   15
#define SNAKE_MAX_LONGTH 200  //first for background last for food
#define SNAKE_UNIT_SIZE 30
#define SNAKE_AREA_HSTART 150//300
#define SNAKE_AREA_VSTART 150
#define SHAKE_RENEWAL_TIME 300
#define SHAKE_LEVEL_DIFFERENT_TIME 200
#define SHAKE_MASSEGE_TIMEOUT 3000

#define NEW_METHOD	1//pangwz
#define SNAKE_SUPPORT_INIT_LENGTH_PAUSE	1

typedef struct
{
	U8 loc_H;
	U8 loc_V;
}S_GAME_SNAKE_LOCTION;
typedef enum
{
	GAME_SNAKE_LEVEL_EASY,
	GAME_SNAKE_LEVEL_NORMAL,
	GAME_SNAKE_LEVEL_HARD,

}EN_GAME_SNAKE_LEVEL;
typedef enum
{
	GAME_SNAKE_UP,
	GAME_SNAKE_DOWN,
	GAME_SNAKE_LEFT,
	GAME_SNAKE_RIGHT,

}EN_GAME_SNAKE_DIRECTION;

typedef struct
{
	U8 level;
	U8 snake_longth;
	S_GAME_SNAKE_LOCTION snake_loction[SNAKE_MAX_LONGTH];//   lastone for food
#if !NEW_METHOD
	S_GAME_SNAKE_LOCTION food_loction;
#endif
	EN_GAME_SNAKE_DIRECTION direction;
	U8 score;
	U8 bestscore[3];
#if SNAKE_SUPPORT_INIT_LENGTH_PAUSE
	BOOL _IsMoving;
	BOOL _IsInited;
	U8 _InitLength;
#endif
}S_GAME_SNAKE;

#if NEW_METHOD
typedef struct
{
	S_GAME_SNAKE_LOCTION location;
	Image *image;
	BOOL _IsInited;
}S_GAME_SNAKE_FOOD;
#endif

enum {
    //(*Identifiers
    //*)
    ID_SNAKE_ABSCONTAINER1,
    ID_SNAKE_IMAGE_FOOD,
    ID_SNAKE_IMAGE1,
#if NEW_METHOD
	ID_SNAKE_COUNT = ID_SNAKE_IMAGE1 +SNAKE_MAX_LONGTH, // 1 FOOD
#endif
    ID_Snake_ABSCONTAINER_PAGE,
    ID_SNAKE_HELP_INFO_4,
    ID_SNAKE_HELP_INFO_IMG,
	ID_SNAKE_HELP_INFO_6,
	ID_SNAKE_HELP_INFO_6_1,
	ID_SNAKE_SCORE_INFO,
	ID_SNAKE_SCORE,
	ID_SNAKE_MESSAGEBOX,
	ID_SNAKE_IMAGE_MESSAGEBOXBG,
	ID_SNAKE_TEXTBOX_MESSAGEBOXTEXT,
#if!NEW_METHOD
    ID_SNAKE_COUNT = ID_SNAKE_IMAGE1 +SNAKE_MAX_LONGTH, // 1 FOOD
#endif
#if NEW_METHOD
	ID_SNAKE_FOOD_IMG,
#endif

#if SNAKE_SUPPORT_INIT_LENGTH_PAUSE
	ID_SNAKE_INIT_LENGTH_INFO,
	ID_SNAKE_INIT_LENGTH_NUM,
	ID_SNAKE_INIT_HELP_INFO,
	ID_SNAKE_INIT_HELP_INFO_1,
	ID_SNAKE_PAUSE_HELP_INFO,
#endif

};
enum{
	ID_SNAKE_RENEWAL_TIMER,
	ID_SNAKE_MESSAGE_BOX_TIMER,
	ID_SNAKE_TIMER_COUNT
};


/*@ <Definitions> @*/
class SnakeFrame : public MMainFrame
{
    DECLARE_EVENT_MAP();

public:

    // ------------------------------------------------------------
    // public constructors
    // ------------------------------------------------------------

    SnakeFrame(MApplet * pParentApp, Rect & stRect);
    ~SnakeFrame();

    // ------------------------------------------------------------
    // public operations
    // ------------------------------------------------------------

    //(*MLoggerDeclare
    unsigned int logid;
    char name[MAX_APPNAME_LENGTH];
    //*)

    //(*Declare
    //*)
     AbsContainer* AbsContainer1;
    AbsContainer* AbsContainer_SnakePage;
    Image* Image_Help_Info_4;
    TextBox* TextBox_Help_Info_4;
	TextBox* TextBox_Help_Info_6;
	TextBox* TextBox_Help_Info_6_1;
	TextBox* TextBox_Score_Info;
	TextBox* TextBox_Score;
	AbsContainer* Snake_MessageBox;
    Image* Image_MessageBoxBG;
    TextBox* Snake_MessageBoxText;
#if SNAKE_SUPPORT_INIT_LENGTH_PAUSE
	TextBox* TextBox_InitLength_Info;
	TextBox* TextBox_InitLength_Num;
	TextBox* TextBox_InitHelpInfo;
	TextBox* TextBox_InitHelpInfo_1;

	TextBox* TextBox_PauseHelpInfo;
#endif


    BOOL OnCreate(void * arg1, void * arg2, void * arg3);
	BOOL OnTimer(void* arg1, void* arg2, void* arg3);
	BOOL OnKey(void*, void*, void*);

    //(*EventHandler
    //*)
protected:

	S_GAME_SNAKE sSnake;
#if NEW_METHOD
	S_GAME_SNAKE_FOOD food;
#endif
	U16 snake_rand;
	Image* Image1[SNAKE_MAX_LONGTH];

	void _initSnake(void);
#if NEW_METHOD
	void _initFood(void);
#endif
	void _SnakeGetFoodNewLocation(BOOL x);
	void _SnakeRenewal(void);
	void _SnakeDisplay(void);
	void _AddSnakeLongth(void);
	void _ShowSnake(void);
	void _ChangeSnakeDirection(void);
};
/*@ </Definitions> @*/

#endif
