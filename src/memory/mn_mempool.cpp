/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2021 Amber-Sophia Schroeck
*
*The Mini Thread Library is free software; you can redistribute it and/or modify  
*it under the terms of the GNU Lesser General Public License as published by  
*the Free Software Foundation, version 3, or (at your option) any later version.

*The Mini Thread Library is distributed in the hope that it will be useful, but 
*WITHOUT ANY WARRANTY; without even the implied warranty of 
*MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
*General Public License for more details.
*
*You should have received a copy of the GNU Lesser General Public
*License along with the Mini Thread  Library; if not, see
*<https://www.gnu.org/licenses/>.  
*/
#include "freertos/FreeRTOS.h"
#include "memory/mn_mempool.hpp";
#include <malloc.h>
#include <iostream>


void* malloc_timed(unsigned long size, unsigned int xTicksToWait) {
#if MN_THREAD_CONFIG_MEMPOOL_USETIMED == MN_THREAD_CONFIG_YES 
    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;
    void *address = NULL;

    while( (xTicksRemaining <= xTicksToWait) ) {
        address = malloc(size);
        if(address != NULL) break;

        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
    }
    return address;
#endif
    return malloc(size);
}


void* realloc_timed(void* addr, unsigned long size, unsigned int xTicksToWait) {
    if(addr == NULL) return addr;

#if MN_THREAD_CONFIG_MEMPOOL_USETIMED == MN_THREAD_CONFIG_YES 
    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;
    
    while( (xTicksRemaining <= xTicksToWait) ) {
        addr = realloc(addr, size);
        if(addr != NULL) break;

        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
    }
    return addr;

#endif
    return realloc(addr, size);
}
void* calloc_timed(unsigned long nmemb, unsigned long size, unsigned int xTicksToWait) {
#if MN_THREAD_CONFIG_MEMPOOL_USETIMED == MN_THREAD_CONFIG_YES
    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;
    void *address = NULL;

    while( (xTicksRemaining <= xTicksToWait) ) {
        address = calloc(nmemb, size);
        if(address != NULL) break;

        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
    }
    return address;
#endif
    return calloc(nmemb, size);
}
#include <string.h>

void* memcpy_timed(void* dest, const void* src, unsigned int size, unsigned int xTicksToWait) {
    if(dest == NULL || src == NULL) return NULL;

#if MN_THREAD_CONFIG_MEMPOOL_USETIMED == MN_THREAD_CONFIG_YES

    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;
    void *address = NULL;

    while( (xTicksRemaining <= xTicksToWait) ) {
        
        address = memcpy(dest, src, size);
        if(address != NULL) break;

        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
    }
    return address;
#endif
    return memcpy(dest, src, size);
}
void* memset_timed(void* addr, int set, unsigned int size, unsigned int xTicksToWait) {
    if(addr == NULL) return addr;
#if MN_THREAD_CONFIG_MEMPOOL_USETIMED == MN_THREAD_CONFIG_YES
    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;
    void *address = NULL;

    while( (xTicksRemaining <= xTicksToWait) ) {
        address = memset(addr, set, size);
        if(address != NULL) break;

        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
    }
    return address;
#endif
    return memset(addr, set, size);
}

