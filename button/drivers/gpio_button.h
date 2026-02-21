#ifndef GPIO_BUTTON_H
#define GPIO_BUTTON_H

#include"button_driver.h"
#include"button_config.h"

typedef struct {
    void* port; //端口
    uint16_t pin; //引脚
    uint8_t active_low; //是否按下低电平
    uint8_t pull_up; //是否上拉
} gpio_button_ctx_t;

button_driver_t* gpio_button_create(const button_config_t* cfg);

#endif // !GPIO_BUTTON_H
