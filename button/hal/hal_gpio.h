/*
 * @Author: 轩
 * @Date: 2026-02-14 16:29:30
 * @LastEditTime: 2026-02-21 22:36:27
 * @FilePath: \button\button\hal\hal_gpio.h
 */
#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include "stm32f10x.h"

// 定义 GPIO 模式
typedef enum {
    HAL_GPIO_MODE_INPUT_FLOATING = 0, // 浮空输入
    HAL_GPIO_MODE_INPUT_PULLUP,       // 上拉输入
    HAL_GPIO_MODE_INPUT_PULLDOWN,     // 下拉输入
    HAL_GPIO_MODE_OUTPUT_PP,          // 推挽输出
    HAL_GPIO_MODE_OUTPUT_OD           // 开漏输出
} hal_gpio_mode_t;

// 初始化 GPIO (支持配置模式)
void hal_gpio_init(GPIO_TypeDef* port, uint16_t pin, hal_gpio_mode_t mode);

// 读取引脚电平 (返回 0 或 1)
int hal_gpio_read(GPIO_TypeDef* port, uint16_t pin);

// 兼容旧代码的宏 (可选)
#define hal_gpio_init_input(p, pin) hal_gpio_init(p, pin, HAL_GPIO_MODE_INPUT_PULLDOWN)
#define hal_gpio_read_pin(p, pin) hal_gpio_read(p, pin)

#endif
