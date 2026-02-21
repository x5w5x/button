/*
 * @Author: 轩
 * @Date: 2026-02-21 20:45:26
 * @LastEditTime: 2026-02-21 23:38:26
 * @FilePath: \button\button\drivers\button_driver.h
 */
#ifndef BUTTON_DRIVER_H
#define BUTTON_DRIVER_H

#include"stdint.h"

typedef struct button_driver button_driver_t;

struct button_driver {
    void* priv; //私有数据指针
    int (*init)(button_driver_t* self); 
    int (*read)(button_driver_t* self);
    void (*enable_irq)(button_driver_t* self);
    void (*disable_irq)(button_driver_t* self);
};

#endif // !BUTTON_DRIVER_H
