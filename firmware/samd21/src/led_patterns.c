/**
 * \file
 * \brief  Configuration file for the different LED patterns.
 *
 * \copyright (c) 2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip software
 * and any derivatives exclusively with Microchip products. It is your
 * responsibility to comply with third party license terms applicable to your
 * use of third party software (including open source software) that may
 * accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT,
 * SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE
 * OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF
 * MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE
 * FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL
 * LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED
 * THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR
 * THIS SOFTWARE.
 */
#include <asf.h>
#include <stdlib.h>
#include <stdio.h>
#include "led_patterns.h"


//LED pattern for provisioning
const led_pattern provision_pattern[] =
{
    { 1,    LED_PATTERN_SHORT_DELAY_MSEC    }, { 0, LED_PATTERN_SHORT_DELAY_MSEC },
    { 0,    LED_PATTERN_LONG_DELAY_MSEC     },
    { 0xFF, 0xFFFF                          } /*Last element   for repeat*/
};


//LED pattern for successful Authentication
const led_pattern success_pattern[] =
{
    { 1,    LED_PATTERN_SHORT_DELAY_MSEC    }, { 0, LED_PATTERN_SHORT_DELAY_MSEC },
    { 1,    LED_PATTERN_SHORT_DELAY_MSEC    }, { 0, LED_PATTERN_SHORT_DELAY_MSEC },
    { 0,    LED_PATTERN_LONG_DELAY_MSEC     },
    { 0xFF, 0xFFFF                          } /*Last element   for repeat*/
};

//LED pattern for not Authentication
const led_pattern fail_pattern[] =
{
    { 1,    LED_PATTERN_SHORT_DELAY_MSEC    }, { 0, LED_PATTERN_SHORT_DELAY_MSEC },
    { 1,    LED_PATTERN_SHORT_DELAY_MSEC    }, { 0, LED_PATTERN_SHORT_DELAY_MSEC },
    { 1,    LED_PATTERN_SHORT_DELAY_MSEC    }, { 0, LED_PATTERN_SHORT_DELAY_MSEC },
    { 0,    LED_PATTERN_LONG_DELAY_MSEC     },
    { 0xFF, 0xFFFF                          } /*Last element   for repeat*/
};

const led_pattern* g_active_pattern;
static uint8_t g_pattern_index;
static uint16_t g_current_state_time;


//Function to update the LED pattern
void update_led_pattern(const led_pattern* pattern)
{
    /*Following lines need to be protected for context switching if play_led_pattern gets called from interrupt*/
    if (pattern == NULL)
    {
        port_pin_set_output_level(LED0, LED_0_INACTIVE);
    }

    if (g_active_pattern != pattern)
    {
        g_active_pattern = pattern;
        g_current_state_time = 0;
        g_pattern_index = 0;
    }
}

/*This needs to be called on every time unit i.e. base for counts*/
void play_led_pattern(void)
{
    if (g_current_state_time)
    {
        g_current_state_time--;
    }
    else if (g_active_pattern)
    {
        /*drive led with status */
        port_pin_set_output_level(LED0, !g_active_pattern[g_pattern_index].led_state);
        if (g_active_pattern[g_pattern_index].state_time_unit_count)
        {
            g_current_state_time = g_active_pattern[g_pattern_index].state_time_unit_count;
            g_pattern_index++;
            if (g_active_pattern[g_pattern_index].led_state == 0xFF)
            {
                g_pattern_index = 0;
            }
        }
        else
        {
            update_led_pattern(NULL);
        }
    }
}