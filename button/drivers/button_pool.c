/*
 * @Author: 轩
 * @Date: 2026-02-21 20:58:10
 * @LastEditTime: 2026-02-21 23:43:58
 * @FilePath: \button\button\drivers\button_pool.c
 */
#include "button_pool.h"
#include "string.h"

#define MAX_BUTTON_INSTANCE 8

static button_driver_pool_t g_button_pool[MAX_BUTTON_INSTANCE];

static button_type_t g_button_type[MAX_BUTTON_INSTANCE]={BUTTON_TYPE_NONE};

void* button_pool_alloc(button_type_t type) {

    for(int i=0;i<MAX_BUTTON_INSTANCE;i++) {

        if(g_button_type[i]==BUTTON_TYPE_NONE) {
            g_button_type[i]=type;
            memset(&g_button_pool[i],0,sizeof(button_driver_pool_t));
            return (&g_button_pool[i]);
        }
    }

    return 0;

}


void button_pool_free(void* ptr) {

    if(!ptr) return;
    uintptr_t base = (uintptr_t)g_button_pool;
    uintptr_t addr = (uintptr_t)ptr;
    if(addr >= base && addr <base + sizeof(g_button_pool)) {
        int idx = (addr - base) / sizeof(button_driver_pool_t);
        g_button_type[idx] = BUTTON_TYPE_NONE;
    }
}