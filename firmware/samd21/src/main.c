/**
 * \file
 * \brief  Main file for the IP protection
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
#include "cryptoauthlib.h"
#include "symmetric_authentication.h"
#include "provision_device.h"
#include "console.h"
#include "configuration.h"
#include "host_random.h"
#include "application.h"
#include "led_patterns.h"
#include "main.h"


static ATCA_STATUS cryptoauthlib_init(void);

volatile uint32_t g_ticks_msec = 0;          //!< Tracks elapsed time in milliseconds
volatile uint32_t g_auth_interval_msec = 1;  //!< Interval until next authentication check in milliseconds
volatile static bool g_do_auth = false;      //!< Indicates the authentication sequence should be performed


//Function to get the master secret key .
void get_master_key(uint8_t *master_key)
{

    //The master secret key should be securely stored in application.
    uint8_t master_secret_key[] = {
        0x37, 0x80, 0xe6, 0x3d, 0x49, 0x68, 0xad, 0xe5,
        0xd8, 0x22, 0xc0, 0x13, 0xfc, 0xc3, 0x23, 0x84,
        0x5d, 0x1b, 0x56, 0x9f, 0xe7, 0x05, 0xb6, 0x00,
        0x06, 0xfe, 0xec, 0x14, 0x5a, 0x0d, 0xb1, 0xe3
    };

    memcpy(master_key, master_secret_key, sizeof(master_secret_key));

}

//Systick callback handler called every 1ms, which sets the g_do_auth after
//the random interval (g_auth_interval_msec) for authentication to happen and
//also performs the led blinking operation
void SysTick_Handler(void)
{
    g_ticks_msec++;
    if (g_ticks_msec % g_auth_interval_msec == 0)
    {
        g_do_auth = true;
        g_ticks_msec = 0;
        g_auth_interval_msec = (rand() % AUTHENTICATION_RANGE_MSEC) + AUTHENTICATION_MIN_MSEC;
    }
    play_led_pattern();
}


//Initialize the CryptoAuthLib library with the particular interface and device
static ATCA_STATUS cryptoauthlib_init(void)
{
    ATCA_STATUS status;

    #if (CRYPTOAUTH_DEVICE == DEVICE_ATSHA204A)
    status = atcab_init(&cfg_atsha204a_i2c_default);
    #elif (CRYPTOAUTH_DEVICE == DEVICE_ATECC508A)
    status = atcab_init(&cfg_ateccx08a_i2c_default);
    #elif (CRYPTOAUTH_DEVICE == DEVICE_ATECC608A)
    cfg_ateccx08a_i2c_default.atcai2c.slave_address = ECC608A_ADDRESS;
    cfg_ateccx08a_i2c_default.devtype = ATECC608A;
    status = atcab_init(&cfg_ateccx08a_i2c_default);
    #endif

    return status;
}



//Function to be called by application at random interval to perform authentication
state authenticate_application(void)
{
    static state auth_status = NOT_AUTHENTICATED;
    uint8_t master_key[ATCA_KEY_SIZE];
    uint8_t host_nonce[NONCE_NUMIN_SIZE];

    if (!g_do_auth)
    {
        return auth_status;
    }

    get_master_key(master_key);
    host_generate_random_number(host_nonce);
    if (symmetric_authenticate(CRYPTOAUTH_DEVICE_AUTH_KEY_SLOT, master_key, host_nonce) == ATCA_SUCCESS)
    {
        auth_status = AUTHENTICATED;
        update_led_pattern(success_pattern);
    }
    else
    {
        auth_status = NOT_AUTHENTICATED;
        update_led_pattern(fail_pattern);
    }
    g_do_auth = false;

    return auth_status;
}


int main(void)
{
    system_init();

    // Initialize interrupt vector table support.
    irq_initialize_vectors();

    // Enable interrupts
    cpu_irq_enable();

    // Update the seed value for use with rand()
    random_seed_init();

    //Initialize the EDBG Port for the output log
    console_init();

    //Initialize CryptoAuthlib library
    cryptoauthlib_init();

    //Initialize the Systick module
    SysTick_Config(system_gclk_gen_get_hz(GCLK_GENERATOR_0) / 1000);

    //Provision the device with the configuration and shared secret data
#if IP_PROTECTION_LOAD_CONFIG
    if (device_provision(CRYPTOAUTH_DEVICE_AUTH_KEY_SLOT) != ATCA_SUCCESS)
    {
        debug_print("%s", "Provision failed...");
        debug_print("%s\r\n", "Check device and press RESET");
    }
#endif

    //Initialize the time for the authentication to perform
    g_auth_interval_msec = (rand() % AUTHENTICATION_RANGE_MSEC) + AUTHENTICATION_MIN_MSEC;

    //Forcing to do Authentication at the start
    g_do_auth = true;
    while (authenticate_application() == NOT_AUTHENTICATED)
    {
        ;
    }
    printf("%s\r\n", "Authentication succeeded");

    //Initialize the Application
    init_display();

    while (true)
    {
        run_application();
    }
}



