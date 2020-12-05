/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: panelCB */
#define  PANEL_QUITBUTTON                 2       /* control type: command, callback function: QuitCallback */
#define  PANEL_ABS_PRES                   3       /* control type: numeric, callback function: (none) */
#define  PANEL_THROTTLE                   4       /* control type: numeric, callback function: timer_throttle */
#define  PANEL_SPEED                      5       /* control type: numeric, callback function: (none) */
#define  PANEL_RPM_2                      6       /* control type: numeric, callback function: (none) */
#define  PANEL_TEMPERATURE                7       /* control type: numeric, callback function: (none) */
#define  PANEL_COOLTEMP                   8       /* control type: scale, callback function: (none) */
#define  PANEL_TIMER                      9       /* control type: timer, callback function: TimerCallback */
#define  PANEL_TIMER_RPM                  10      /* control type: timer, callback function: timer_rpm */
#define  PANEL_TIMER_SPEED                11      /* control type: timer, callback function: timer_speed */
#define  PANEL_TIMER_PRESSURE             12      /* control type: timer, callback function: timer_pressure */
#define  PANEL_RPMGAUGE                   13      /* control type: scale, callback function: (none) */
#define  PANEL_SPEEDMETER                 14      /* control type: scale, callback function: (none) */
#define  PANEL_PRESSMETER                 15      /* control type: scale, callback function: (none) */
#define  PANEL_THROTTLETANK               16      /* control type: scale, callback function: (none) */
#define  PANEL_TIMER_THROTT               17      /* control type: timer, callback function: timer_throttle */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK timer_pressure(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK timer_rpm(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK timer_speed(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK timer_throttle(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TimerCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
