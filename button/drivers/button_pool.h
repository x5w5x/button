#ifndef BUTTON_POOL_H
#define BUTTON_POOL_H

#include "button_driver.h"
#include "gpio_button.h"

//按键池
typedef struct {

    button_driver_t drv;
    gpio_button_ctx_t ctx;

} gpio_button_pool_t;
//驱动池
typedef union {

    gpio_button_pool_t gpio;

} button_driver_pool_t;

//按键类型
typedef enum {

    BUTTON_TYPE_NONE=0,
    BUTTON_TYPE_GPIO,
    
}button_type_t;

//分配内存
void* button_pool_alloc(button_type_t type);
//挂起内存
void button_pool_free(void* ptr);

#endif // !BUTTON_POOL_H
