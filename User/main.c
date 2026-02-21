/*
 * @Author: 轩
 * @Date: 2026-02-06 19:12:36
 * @LastEditTime: 2026-02-21 23:45:20
 * @FilePath: \button\User\main.c
 */



#include "stm32f10x.h"
#include "hal_gpio.h"
#include "gpio_button.h"      // 要测试的驱动
#include "button_driver.h"    // 驱动基类
#include "Serial.h"
#include "delay1.h"
#include <stdio.h>
const button_config_t g_button_configs[] = {
	0
    // {"KEY1", GPIOB, GPIO_Pin_13, 1, 1, 20, 1000, 300},
    // {"KEY2", GPIOB, GPIO_Pin_15, 1, 1, 20, 1000, 300},
};
// LED 初始化
void LED_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

int main(void) {
    // 1. 初始化串口
    Serial_Init();
    printf("\r\n=== GPIO_Button Driver Test ===\r\n");
    
    // 2. 初始化 LED
    LED_Init();
    
    // 3. 配置按键参数
    // 根据你的 gpio_button.h，需要 button_config_t 结构体
    button_config_t key_cfg = {
        .name = "KEY1",
        .port = GPIOB,           // 按键端口
        .pin = GPIO_Pin_15,      // 按键引脚
        .active_low = 1,         // 0=高电平有效(接正极), 1=低电平有效(接地)
        .use_pull_up = 0,            // 0=下拉(接正极), 1=上拉(接地)
        .debounce_ms = 20,       // 消抖时间
        .long_press_ms = 1000,   // 长按阈值
        .double_click_interval = 300   // 双击间隔
    };
    
    // 4. 创建按键驱动实例
    button_driver_t* drv = gpio_button_create(&key_cfg);
    
	drv->init(drv);
    if (drv == NULL) {
        printf("ERROR: gpio_button_create failed!\r\n");
        while (1);
    }
    
    printf("gpio_button created OK!\r\n");
    printf("Press KEY1 and watch LED/serial...\r\n\r\n");
    
    // 5. 主循环 - 直接调用驱动的 read 函数测试
    uint8_t last_state = 0;
    
    while (1) {
        // 调用驱动的 read 函数读取按键状态
        // 假设 button_driver_t 有 read 函数指针
        // if (drv->read && drv->priv) {
        //     uint8_t current_state = drv->read(drv);
            
        //     // 检测按下沿 (0->1 或 1->0 取决于 active_low)
        //     if (current_state != last_state) {
        //         if (current_state == 1) {
        //             printf("KEY PRESSED! (Raw=1)\r\n");
        //             GPIO_ResetBits(GPIOC, GPIO_Pin_13);  // 亮灯
        //         } else {
        //             printf("KEY RELEASED! (Raw=0)\r\n");
        //             GPIO_SetBits(GPIOC, GPIO_Pin_13);    // 灭灯
        //         }
        //         last_state = current_state;
        //     }
        // }
        
        // Delay_ms(50);  // 简单消抖
			if(drv->read(drv))
				//   if (hal_gpio_read_pin(GPIOB, GPIO_Pin_15) == 0) 
				  {
        GPIO_ResetBits(GPIOC, GPIO_Pin_13); // 按下：亮
    } else {
        GPIO_SetBits(GPIOC, GPIO_Pin_13);   // 释放：灭
    }
    }
}

// #include "button_core.h"
// #include "stm32f10x.h"
// #include "hal_gpio.h"
// #include "Serial.h"
// #include "delay1.h"

// // 静态按键配置（在 config_button.c 中定义）
// // const button_config_t g_button_configs[] = {
// //     {"KEY1", GPIOB, GPIO_Pin_15, 0, 0, 20, 1000, 300}, // active_low=0, use_pull_up=0 表示需要下拉
// // };
// const button_config_t g_button_configs[] = {
// 	0
//     // {"KEY1", GPIOB, GPIO_Pin_13, 1, 1, 20, 1000, 300},
//     // {"KEY2", GPIOB, GPIO_Pin_15, 1, 1, 20, 1000, 300},
// };
// void my_button_callback(button_event_t event, void* user) {
//     int id = (int)user;
//     switch (event) {
//         case BUTTON_EVENT_PRESS:
//             printf("Button %d press\n", id);
//             break;
//         case BUTTON_EVENT_RELEASE:
//             printf("Button %d release\n", id);
//             break;
//         case BUTTON_EVENT_CLICK:
//             printf("Button %d click\n", id);
//             break;
//         case BUTTON_EVENT_DOUBLE_CLICK:
//             printf("Button %d double click\n", id);
//             break;
//         case BUTTON_EVENT_LONG_PRESS:
//             printf("Button %d long press\n", id);
//             break;
//         default:
//             break;
//     }
// }

// uint32_t get_tick(void) {
//     return GetTickCount(); // 你的系统毫秒函数
// }

// int main(void) {
//     Serial_Init();
//     Delay_Init(); // 假设有延时初始化

//     // 初始化时间接口
//     TimeInterface time = { .get_tick_ms = get_tick };
//     button_core_init(&time);

//     // 为静态按键注册回调（注意：button_core.h 未提供直接获取句柄的函数，这里用动态创建代替）
//     // 或者修改 button_core.h 增加 button_get_handle_by_id，这里为简化，直接动态创建
//     button_handle_t btn = button_create_gpio("KEY1", GPIOB, GPIO_Pin_15,
//                                              1,          // active_low=0（高电平按下）
//                                              20,         // 去抖动
//                                              1000,       // 长按 1s
//                                              300);       // 双击间隔 300ms
//     if (btn) {
//         button_register_callback(btn, my_button_callback, (void*)1);
//         printf("Button created, waiting for events...\r\n");
//     } else {
//         printf("Button create failed!\r\n");
//         while (1);
//     }

//     while (1) {
//         button_core_process();
		
//         // 其他任务
//     }
// }