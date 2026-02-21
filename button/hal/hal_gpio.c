
#include "hal_gpio.h"

// 辅助函数：获取引脚号 (0-15)
static uint8_t get_pin_num(uint16_t pin) {
    uint8_t i = 0;
    while ((pin & 0x01) == 0) {
        pin >>= 1;
        i++;
    }
    return i;
}

// 使能时钟
static void enable_gpio_clock(GPIO_TypeDef* port) {
    if (port == GPIOA) RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    else if (port == GPIOB) RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    else if (port == GPIOC) RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    else if (port == GPIOD) RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
    else if (port == GPIOE) RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
}

/**
 * @brief 初始化 GPIO
 * @param mode: 
 *   - HAL_GPIO_MODE_INPUT_FLOATING: CNF=01, MODE=00 (浮空)
 *   - HAL_GPIO_MODE_INPUT_PULLUP:   CNF=10, MODE=00, ODR=1 (上拉)
 *   - HAL_GPIO_MODE_INPUT_PULLDOWN: CNF=10, MODE=00, ODR=0 (下拉)
 */
void hal_gpio_init(GPIO_TypeDef* port, uint16_t pin, hal_gpio_mode_t mode) {
    if (!port) return;

    enable_gpio_clock(port);

    uint8_t pin_num = get_pin_num(pin);
    volatile uint32_t* cr_reg;
    uint32_t shift;

    // 选择 CRL (0-7) 或 CRH (8-15)
    if (pin_num < 8) {
        cr_reg = &port->CRL;
        shift = pin_num * 4;
    } else {
        cr_reg = &port->CRH;
        shift = (pin_num - 8) * 4;
    }

    // 清除该引脚的 4 位配置
    *cr_reg &= ~(0xF << shift);

    uint32_t cnf = 0;
    uint32_t md = 0;

    // 配置 CNF 和 MODE
    switch (mode) {
        case HAL_GPIO_MODE_INPUT_FLOATING:
            md = 0x0;  // 输入模式
            cnf = 0x1; // 浮空
            break;
        case HAL_GPIO_MODE_INPUT_PULLUP:
        case HAL_GPIO_MODE_INPUT_PULLDOWN:
            md = 0x0;  // 输入模式
            cnf = 0x2; // 上拉/下拉
            break;
        case HAL_GPIO_MODE_OUTPUT_PP:
            md = 0x3;  // 50MHz 输出
            cnf = 0x0; // 推挽
            break;
        case HAL_GPIO_MODE_OUTPUT_OD:
            md = 0x3;
            cnf = 0x1; // 开漏
            break;
    }

    // 写入配置 (MODE 在低 2 位，CNF 在高 2 位)
    *cr_reg |= ((cnf << 2) | md) << shift;

    // 如果是上拉/下拉模式，需要配置 ODR
    if (mode == HAL_GPIO_MODE_INPUT_PULLUP) {
        port->ODR |= pin;      // ODR=1 -> 上拉
    } else if (mode == HAL_GPIO_MODE_INPUT_PULLDOWN) {
        port->ODR &= ~pin;     // ODR=0 -> 下拉
    } else if (mode == HAL_GPIO_MODE_OUTPUT_PP || mode == HAL_GPIO_MODE_OUTPUT_OD) {
        port->ODR &= ~pin;     // 输出默认低电平
    }
}

/**
 * @brief 读取引脚电平
 * @return 1 (高电平) 或 0 (低电平)
 */
int hal_gpio_read(GPIO_TypeDef* port, uint16_t pin) {
    if (!port) return 0;
    return (port->IDR & pin) ? 1 : 0;
}











