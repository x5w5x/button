/*
 * @Author: 轩
 * @Date: 2026-02-21 20:51:50
 * @LastEditTime: 2026-02-21 21:48:46
 * @FilePath: \button\button\config\button_config.h
 */
#ifndef BUTTON_CONFIG_H
#define BUTTON_CONFIG_H

#include"stdint.h"

typedef struct {
    const char* name; //按键名称（调试）
    void* port; //端口
    uint16_t pin; //引脚
    uint8_t active_low; //是否按下低电平
    uint8_t use_pull_up; //是否上拉
    uint8_t use_interrupt; //是否使用中断
    uint32_t debounce_ms; //去抖动时间
    uint32_t long_press_ms; //长按触发时间
    uint32_t double_click_interval; //双击最大间隔

} button_config_t;


#endif // !BUTTON_CONFIG_H
