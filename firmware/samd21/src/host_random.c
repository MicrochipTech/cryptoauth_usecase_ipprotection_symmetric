/**
 * \file
 * \brief  Contains Host related Random number generation with ADC value as seed
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

#include <stdlib.h>
#include <stdio.h>
#include <asf.h>
#include "host_random.h"
#include "cryptoauthlib.h"
#include "symmetric_authentication.h"

struct adc_module g_adc_instance;
struct adc_config g_config_adc;
static uint16_t calculate_adc(enum adc_positive_input adc_input);
static void configure_adc(void);

//Function to calculate the ADC value
uint16_t calculate_adc(enum adc_positive_input adc_input)
{
    uint16_t result;

    adc_set_positive_input(&g_adc_instance, adc_input);
    adc_start_conversion(&g_adc_instance);
    do
    {
        /* Wait for conversion to be done and read out result */
    }
    while (adc_read(&g_adc_instance, &result) == STATUS_BUSY);
    adc_flush(&g_adc_instance);

    return result;
}

//Function to initialize and enable the ADC
void configure_adc(void)
{
    system_voltage_reference_enable(SYSTEM_VOLTAGE_REFERENCE_TEMPSENSE);
    adc_get_config_defaults(&g_config_adc);
    g_config_adc.resolution =  ADC_CTRLB_RESSEL_16BIT;

    adc_init(&g_adc_instance, ADC, &g_config_adc);

    adc_enable(&g_adc_instance);
}

//Function that calculates the ADC value from different analog sources and updates that value as Seed for the random number
void random_seed_init(void)
{
    uint16_t result;
    uint32_t rand_seed = 0;

    configure_adc();

    result = calculate_adc(ADC_POSITIVE_INPUT_TEMP);
    rand_seed |= (uint32_t)(result & 0x00FF) << 0;

    result = calculate_adc(ADC_POSITIVE_INPUT_SCALEDCOREVCC);
    rand_seed |= (uint32_t)(result & 0x00FF) << 8;

    result = calculate_adc(ADC_POSITIVE_INPUT_SCALEDIOVCC);
    rand_seed |= (uint32_t)(result & 0x00FF) << 16;

    result = calculate_adc(ADC_POSITIVE_INPUT_BANDGAP);
    rand_seed |= (uint32_t)(result & 0x00FF) << 24;

    srand(rand_seed);
}

//Function to calculate 20 byte random number from host
void host_generate_random_number(uint8_t *rand_num)
{
    size_t i;
    int num;

    for (i = 0; i < 20; i += sizeof(int))
    {
        num = rand();
        memcpy(&rand_num[i], &num, sizeof(num));
    }
}
