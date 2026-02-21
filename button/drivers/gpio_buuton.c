#include "gpio_button.h"
#include "button_pool.h"
#include "hal_gpio.h"

static int gpio_button_init(button_driver_t* self) {
    gpio_button_ctx_t* ctx = (gpio_button_ctx_t*)self->priv;
    if(!ctx) return -1;

    // 根据 active_low 和 pull_up 共同决定配置
    if (ctx->active_low) {
        // 按键按下为低电平（接地）
        if (ctx->pull_up) {
            // 需要上拉电阻：引脚默认高电平，按下时被拉低
            hal_gpio_init(ctx->port, ctx->pin, HAL_GPIO_MODE_INPUT_PULLUP);
        } else {
            // 无上拉或配置错误，配置为浮空输入（不推荐，易受干扰）
            hal_gpio_init(ctx->port, ctx->pin, HAL_GPIO_MODE_INPUT_FLOATING);
            // 更安全的做法是强制使用上拉，或者返回错误：
            // printf("Warning: Active-low button should use pull-up.\n");
            // hal_gpio_init(ctx->port, ctx->pin, HAL_GPIO_MODE_INPUT_PULLUP);
        }
    } else {
        // 按键按下为高电平（接正极/VCC）
        if (!ctx->pull_up) { // 注意：这里使用 !ctx->pull_up，因为 pull_up=0 表示我们需要下拉
            // 需要下拉电阻：引脚默认低电平，按下时被拉高
            hal_gpio_init(ctx->port, ctx->pin, HAL_GPIO_MODE_INPUT_PULLDOWN);
        } else {
            // 无下拉或配置错误，配置为浮空输入（不推荐）
            hal_gpio_init(ctx->port, ctx->pin, HAL_GPIO_MODE_INPUT_FLOATING);
            // 更安全的做法是强制使用下拉，或者返回错误：
            // printf("Warning: Active-high button should use pull-down.\n");
            // hal_gpio_init(ctx->port, ctx->pin, HAL_GPIO_MODE_INPUT_PULLDOWN);
        }
    }
    return 0;
}


static int gpio_button_read(button_driver_t* self) {
    gpio_button_ctx_t* ctx =(gpio_button_ctx_t*) self->priv;
    uint8_t phys=hal_gpio_read_pin(ctx->port,ctx->pin);
    uint8_t pressed =ctx->active_low ?(!phys):phys;
    return pressed;

}


static void gpio_button_enable_irq(button_driver_t* self) { (void)self; }
static void gpio_button_disable_irq(button_driver_t* self) { (void)self; }



button_driver_t* gpio_button_create(const button_config_t* cfg) {

    if(!cfg) return 0;

    gpio_button_pool_t* obj =(gpio_button_pool_t*) button_pool_alloc(BUTTON_TYPE_GPIO);
    if(!obj) return 0;

    obj->drv.priv=&obj->ctx;
    obj->drv.init=gpio_button_init;
    obj->drv.read =gpio_button_read;
    obj->drv.enable_irq=gpio_button_enable_irq;
    obj->drv.disable_irq=gpio_button_disable_irq;

    obj->ctx.port=cfg->port;
    obj->ctx.pin=cfg->pin;
    obj->ctx.active_low=cfg->active_low;
    obj->ctx.pull_up=cfg->use_pull_up;
    
    return &obj->drv;

}