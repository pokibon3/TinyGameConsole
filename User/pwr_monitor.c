#include <stdlib.h>

#include "debug.h"
#include "ch32v00x.h"
#include "tick.h"

#include "pwr_monitor.h"

static uint8_t power_low = 0;
static uint8_t led_state = 0;
static uint32_t last_tick;

void PWR_MONITOR_Init(void)
{
    // Set PC0 as output (built-in LED on UIAPduino)
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOC;
    GPIOC->CFGLR &= ~(0xF << 0);
    GPIOC->CFGLR |= (0x1 << 0);

    // Enable PVD to trigger below 2.9 V
    RCC->APB1PCENR |= RCC_APB1Periph_PWR;
    PWR->CTLR |= (1 << 4) | (0b000 << 5);

    // Turn on the LED
    GPIOC->BSHR = (1 << 0);

}

void PWR_MONITOR_Loop(void)
{
    // Check if PVD is triggered
    if(PWR->CSR & (1 << 2))
    {
        power_low = 1;
    }
    else
    {
        power_low = 0;
    }

    if(power_low)
    {
        if((TICK_Get() - last_tick) > 500)
        {
            last_tick = TICK_Get();

            if(led_state == 1)
            {
                GPIOC->BCR = (1 << 0);
                led_state = 0;
            }
            else
            {
                GPIOC->BSHR = (1 << 0);
                led_state = 1;
            }
        }
    }
    else
    {
        GPIOC->BSHR = (1 << 0);
    }
}

uint8_t PWR_Monitor_IsPowerLow(void)
{
    return power_low;
}
