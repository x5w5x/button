/*
 * @Author: 轩
 * @Date: 2026-02-21 20:40:59
 * @LastEditTime: 2026-02-21 23:39:09
 * @FilePath: \button\button\core\button_event.h
 */
#ifndef BUTTON_EVENT_T
#define BUTTON_EVENT_T

//按键事件
typedef enum{
    BUTTON_EVENT_NONE=0, //无事件
    BUTTON_EVENT_PRESS, //按下瞬间
    BUTTON_EVENT_RELEASE, //释放瞬间
    BUTTON_EVENT_CLICK, //单击 
    BUTTON_EVENT_DOUBLE_CLICK,//双击
    BUTTON_EVENT_LONG_PRESS,
    BUTTON_EVENT_LONG_PRESS_HOLD, //长按保持
    BUTTON_EVENT_PEREAT, //连发
} button_event_t;



#endif // !BUTTON_EVENT_T