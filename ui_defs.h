#ifndef __UI_DEFS__
#define __UI_DEFS__

#include "utils_tft.h"

#define STATUSBAR_X                	0
#define STATUSBAR_Y                	0
#define STATUSBAR_H                	20
#define STATUSBAR_W                	TFT_WIDTH
#define STATUSBAR_OBJ_MAX			4
#define STATUSBAR_OBJ_SIZE			16											//16x16
#define STATUSBAR_OBJ_GAP_Y			((STATUSBAR_H - STATUSBAR_OBJ_SIZE)/2)
#define STATUSBAR_OBJ_GAP_X			4
#define STATUSBAR_OBJ_Y				STATUSBAR_OBJ_GAP_Y

#define TOOLBAR_OBJ_MAX             6
#define TOOLBAR_X                   0
#define TOOLBAR_Y                   STATUSBAR_H
#define TOOLBAR_H                   44
#define TOOLBAR_W                   TFT_WIDTH
#define TOOLBAR_OBJ_SIZE            32                                          //32x32
#define TOOLBAR_OBJ_GAP_Y           ((TOOLBAR_H - TOOLBAR_OBJ_SIZE)/2)
#define TOOLBAR_OBJ_GAP_X           ((TOOLBAR_W - (TOOLBAR_OBJ_SIZE * TOOLBAR_OBJ_MAX))/(TOOLBAR_OBJ_MAX + 1))

#define WORK_AREA_FRAME_GAP         4
#define WORK_AREA_FRAME_X           WORK_AREA_FRAME_GAP      
#define WORK_AREA_FRAME_Y           (TOOLBAR_Y + TOOLBAR_H + WORK_AREA_FRAME_GAP)
#define WORK_AREA_FRAME_H           (TFT_HEIGHT - WORK_AREA_FRAME_Y - WORK_AREA_FRAME_GAP)
#define WORK_AREA_FRAME_W           (TFT_WIDTH - (2*WORK_AREA_FRAME_GAP))
#define WORK_AREA_FRAME_R           4
#define WORK_AREA_OBJ_SIZE          48                                          //GRID : 64x64     
#define WORK_AREA_OBJ_W_RAW         128                                         //RAWS : 64x128
#define WORK_AREA_OBJ_ROW           3
#define WORK_AREA_OBJ_COL           3
#define WORK_AREA_OBJ_MAX           (WORK_AREA_OBJ_ROW * WORK_AREA_OBJ_COL)
#define WORK_AREA_PER_PAGE          3
#define WORK_AREA_OBJ_TOT           (WORK_AREA_OBJ_MAX * WORK_AREA_PER_PAGE)
#define WORK_AREA_OBJ_GAP_X_GRID    ((WORK_AREA_FRAME_W - (WORK_AREA_OBJ_SIZE * WORK_AREA_OBJ_COL))/(WORK_AREA_OBJ_COL + 1))
#define WORK_AREA_OBJ_GAP_Y_GRID    ((WORK_AREA_FRAME_H - (WORK_AREA_OBJ_SIZE * WORK_AREA_OBJ_ROW))/(WORK_AREA_OBJ_ROW + 1))
#define WORK_AREA_OBJ_GAP_X_RAW     (WORK_AREA_FRAME_GAP + (WORK_AREA_FRAME_W - WORK_AREA_OBJ_W_RAW)/2)
#define WORK_AREA_OBJ_GAP_Y_RAW     WORK_AREA_OBJ_GAP_Y_GRID

#define WORK_AREA_ARROW_WIDTH       4
#define WORK_AREA_ARROW_LEFT_X0     (WORK_AREA_FRAME_GAP + 1)
#define WORK_AREA_ARROW_LEFT_Y0     (WORK_AREA_FRAME_Y + (WORK_AREA_FRAME_H/2))
#define WORK_AREA_ARROW_LEFT_X1     (WORK_AREA_ARROW_LEFT_X0 + WORK_AREA_ARROW_WIDTH)
#define WORK_AREA_ARROW_LEFT_Y1     (WORK_AREA_ARROW_LEFT_Y0 - 2)
#define WORK_AREA_ARROW_LEFT_X2     (WORK_AREA_ARROW_LEFT_X0 + WORK_AREA_ARROW_WIDTH)
#define WORK_AREA_ARROW_LEFT_Y2     (WORK_AREA_ARROW_LEFT_Y0 + 2)
#define WORK_AREA_ARROW_RIGHT_X0    (TFT_WIDTH - (WORK_AREA_FRAME_GAP + 2))
#define WORK_AREA_ARROW_RIGHT_Y0    (WORK_AREA_FRAME_Y + (WORK_AREA_FRAME_H/2))
#define WORK_AREA_ARROW_RIGHT_X1    (WORK_AREA_ARROW_RIGHT_X0 - WORK_AREA_ARROW_WIDTH)
#define WORK_AREA_ARROW_RIGHT_Y1    (WORK_AREA_ARROW_LEFT_Y0 - 2)
#define WORK_AREA_ARROW_RIGHT_X2    (WORK_AREA_ARROW_RIGHT_X0 - WORK_AREA_ARROW_WIDTH)
#define WORK_AREA_ARROW_RIGHT_Y2    (WORK_AREA_ARROW_LEFT_Y0 + 2)
#define WORK_AREA_ARROW_UP_X0       (WORK_AREA_FRAME_X + (WORK_AREA_FRAME_W/2))       
#define WORK_AREA_ARROW_UP_Y0       (WORK_AREA_FRAME_Y + 1)
#define WORK_AREA_ARROW_UP_X1       (WORK_AREA_ARROW_UP_X0 + 2)
#define WORK_AREA_ARROW_UP_Y1       (WORK_AREA_ARROW_UP_Y0 + WORK_AREA_ARROW_WIDTH)
#define WORK_AREA_ARROW_UP_X2       (WORK_AREA_ARROW_UP_X0 - 2)
#define WORK_AREA_ARROW_UP_Y2       (WORK_AREA_ARROW_UP_Y0 + WORK_AREA_ARROW_WIDTH)
#define WORK_AREA_ARROW_DOWN_X0     (WORK_AREA_FRAME_X + (WORK_AREA_FRAME_W/2))     
#define WORK_AREA_ARROW_DOWN_Y0     (TFT_HEIGHT - (WORK_AREA_FRAME_GAP + 2))  
#define WORK_AREA_ARROW_DOWN_X1     (WORK_AREA_ARROW_UP_X0 + 2)  
#define WORK_AREA_ARROW_DOWN_Y1     (WORK_AREA_ARROW_DOWN_Y0 - WORK_AREA_ARROW_WIDTH)  
#define WORK_AREA_ARROW_DOWN_X2     (WORK_AREA_ARROW_UP_X0 - 2)  
#define WORK_AREA_ARROW_DOWN_Y2     (WORK_AREA_ARROW_DOWN_Y0 - WORK_AREA_ARROW_WIDTH)  

#define UI_MAX_PAGES                16
#define MAX_OBJ_PER_PAGE            (TOOLBAR_OBJ_MAX + WORK_AREA_OBJ_TOT)

typedef enum
{
  ID_HOME,
  ID_SETTINGS
} page_id_t;

typedef enum
{
  ID_HOME_BUTTON = 1,
  ID_BACK_BUTTON,
  ID_USER_START,            // In each page, start objects IDs from this value !!!
}obj_id_t;

#define HOME_UP_BMP         "/homeUP.palette"
#define HOME_DOWN_BMP       "/homeDOWN.palette"
#define BACK_UP_BMP         "/backUP.palette"
#define BACK_DOWN_BMP       "/backDOWN.palette"

#define STATUSBAR_WIFI_BMP	"/wifi.palette"
#define STATUSBAR_BLYNK_BMP	"/blynk.palette"

#endif // __UI_DEFS__
