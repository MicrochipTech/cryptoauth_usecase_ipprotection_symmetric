/**
 * \file
 * \brief  Device personalization related functions
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


#include "configuration.h"

#ifdef IP_PROTECTION_LOAD_CONFIG

#include <asf.h>
#include "main.h"
#include "led_patterns.h"
#include "cryptoauthlib.h"
#include "host/atca_host.h"

#include "provision_device.h"
#include "symmetric_authentication.h"
#include "console.h"
#ifndef CRYPTOAUTH_DEVICE
#error "Device not selected, select it in the configuration.h file."
#endif

#if (CRYPTOAUTH_DEVICE == DEVICE_ATSHA204A)

//ATSHA204A Configuration to store the symmetric diversified key
const uint8_t sha204_configdata[ATCA_SHA_CONFIG_SIZE] = {
    // block 0
    // Not Written: First 16 bytes are not written
    0x01, 0x23, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0xEE, 0x55, 0x00, 0x00,
    // I2C, CheckMacConfig, OtpMode, SelectorMode
    0xC8, 0x00, 0x55, 0x00,
    // SlotConfig
    0x8F, 0x80, 0x80, 0xA1,
    0x82, 0xE0, 0xC4, 0xF4,
    0x84, 0x00, 0xA0, 0x85,
    // block 1
    0x80, 0xB0, 0x87, 0x07,
    0x0F, 0x00, 0xC4, 0x64,
    0x8A, 0x7A, 0x0B, 0x8B,
    0x0C, 0x4C, 0x80, 0xB0,
    0xC2, 0x42, 0xAF, 0x8F,
    // Use Flags
    0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0x00, 0xFF, 0x00,
    // block 2
    0xFF, 0x00, 0xFF, 0x00,
    // Last Key Use
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    // Not Written: UserExtra, Selector, LockData, LockConfig (word offset = 5)
    0x00, 0x00, 0x55, 0x55,
};

#elif (CRYPTOAUTH_DEVICE == DEVICE_ATECC508A)
//ATECC508A Configuration to store the symmetric diversified key
const uint8_t ecc508_configdata[ATCA_ECC_CONFIG_SIZE] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xC0, 0x00, 0xAA, 0x00, 0x8F, 0x20, 0xC4, 0x44, 0x87,  0x20, 0x87, 0x20, 0x8F, 0x0F, 0xC4, 0x36,
    0x8F, 0x0F, 0x82, 0x20, 0x0F, 0x0F, 0xC4, 0x44, 0x0F,  0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
    0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,  0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,  0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x33, 0x00, 0x1C, 0x00, 0x13, 0x00, 0x13, 0x00, 0x7C,  0x00, 0x1C, 0x00, 0x7C, 0x00, 0x33, 0x00,
    0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x30, 0x00, 0x3C,  0x00, 0x3C, 0x00, 0x3C, 0x00, 0x30, 0x00
};
#elif (CRYPTOAUTH_DEVICE == DEVICE_ATECC608A)
//ATECC608A Configuration to store the symmetric diversified key
const uint8_t ecc608_configdata[ATCA_ECC_CONFIG_SIZE] = {
    0x01,            0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00,
    ECC608A_ADDRESS, 0x00, 0x00, 0x01, 0x85, 0x00, 0x82, 0x00, 0x85,  0x20, 0x85, 0x20, 0x85, 0x20, 0x8F, 0x46,
    0x8F,            0x0F, 0x9F, 0x8F, 0x0F, 0x0F, 0x8F, 0x0F, 0x0F,  0x8F, 0x0F, 0x8F, 0x0F, 0x8F, 0x0F, 0x0F,
    0x0D,            0x1F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,  0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00,            0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF7, 0x00,  0x69, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00,            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,  0xFF, 0x0E, 0x60, 0x00, 0x00, 0x00, 0x00,
    0x53,            0x00, 0x53, 0x00, 0x73, 0x00, 0x73, 0x00, 0x73,  0x00, 0x38, 0x00, 0x7C, 0x00, 0x1C, 0x00,
    0x3C,            0x00, 0x1A, 0x00, 0x1C, 0x00, 0x10, 0x00, 0x1C,  0x00, 0x30, 0x00, 0x12, 0x00, 0x30, 0x00
};
#endif

//Function to configure the device configuration zone and the symmetric diversified key in the slot
ATCA_STATUS device_provision(uint8_t slot)
{
    ATCA_STATUS status;
    uint8_t sn[ATCA_SERIAL_NUM_SIZE];
    uint8_t symmetric_key[ATCA_KEY_SIZE];
    bool is_locked;
    uint8_t master_key[ATCA_KEY_SIZE];
    atca_temp_key_t temp_key_derive;
    struct atca_derive_key_in_out derivekey_params;

    do
    {
         #if (CRYPTOAUTH_DEVICE == DEVICE_ATECC608A)
        if ((status = detect_crypto_device()) != ATCA_SUCCESS)
        {
            break;
        }
         #endif
        //Check current status of configuration lock status
        if ((status = atcab_is_locked(LOCK_ZONE_CONFIG, &is_locked)) != ATCA_SUCCESS)
        {
            break;
        }

        //Write configuration if it is not already locked
        if (!is_locked)
        {

            debug_print("%s", "Device is not provisioned...");
            debug_print("%s\r\n", "Press SW0 button to provision");

            //Update the led pattern with provision led pattern
            update_led_pattern(provision_pattern);

            //Wait for the SW0 button to be pressed
            while (port_pin_get_input_level(BUTTON_0_PIN) == SW0_INACTIVE)
            {
                ;
            }

            //Trigger Configuration write
            #if (CRYPTOAUTH_DEVICE == DEVICE_ATSHA204A)
            if ((status = atcab_write_config_zone(sha204_configdata)) != ATCA_SUCCESS)
            {
                break;
            }

            #elif (CRYPTOAUTH_DEVICE == DEVICE_ATECC508A)
            if ((status = atcab_write_config_zone(ecc508_configdata)) != ATCA_SUCCESS)
            {
                break;
            }
            #elif (CRYPTOAUTH_DEVICE == DEVICE_ATECC608A)
            if ((status = atcab_write_config_zone(ecc608_configdata)) != ATCA_SUCCESS)
            {
                break;
            }
            if (cfg_ateccx08a_i2c_default.atcai2c.slave_address != ecc608_configdata[16])
            {
                atcab_wakeup();
                atcab_sleep();
                cfg_ateccx08a_i2c_default.atcai2c.slave_address = ecc608_configdata[16];
                atcab_init(&cfg_ateccx08a_i2c_default);
            }
            #endif


            //Lock Configuration Zone on completing configuration
            if ((status = atcab_lock_config_zone()) != ATCA_SUCCESS)
            {
                break;
            }
        }


        //Check current status of data zone lock status
        if ((status = atcab_is_locked(LOCK_ZONE_DATA, &is_locked)) != ATCA_SUCCESS)
        {
            break;
        }

        //Write shared secret if it is not already locked
        if (!is_locked)
        {

            //Read the serial number from the device
            if ((status = atcab_read_serial_number(sn)) != ATCA_SUCCESS)
            {
                break;
            }

            get_master_key(master_key);

            memset(&temp_key_derive, 0, sizeof(temp_key_derive));
            temp_key_derive.valid = 1;
            memcpy(temp_key_derive.value, sn, sizeof(sn));// 32 bytes TempKey ( SN[0:8] with padded 23 zeros used in symmetric key calculation)

            // Parameters used deriving the symmetric key
            derivekey_params.mode = 0;
            derivekey_params.target_key_id = slot;
            derivekey_params.parent_key = master_key;
            derivekey_params.sn = sn;
            derivekey_params.target_key = symmetric_key;
            derivekey_params.temp_key = &temp_key_derive;

            // calculate the symmetric_diversified_key
            if ((status = atcah_derive_key(&derivekey_params)) != ATCA_SUCCESS)
            {
                break;
            }

            //Store the symmetric diversified key in device
            if ((status = atcab_write_zone(ATCA_ZONE_DATA, slot, 0, 0, symmetric_key, ATCA_KEY_SIZE)) != ATCA_SUCCESS)
            {
                break;
            }

            //Lock the data zone of the device
            if ((status = atcab_lock_data_zone()) != ATCA_SUCCESS)
            {
                break;
            }

            #if (CRYPTOAUTH_DEVICE == DEVICE_ATECC508A) || (CRYPTOAUTH_DEVICE == DEVICE_ATECC608A)
            if ((status = atcab_lock_data_slot(slot)) != ATCA_SUCCESS)
            {
                break;
            }
            #endif

            update_led_pattern(NULL);

            debug_print("%s", "Device provisioned succesfully...");
            debug_print("%s\r\n", "Press SW0 button to continue");

            //Press the SW0 button to start proceeding
            while (port_pin_get_input_level(BUTTON_0_PIN) == SW0_INACTIVE)
            {
                ;
            }
        }

    }
    while (0);

    return status;
}

#if (CRYPTOAUTH_DEVICE == DEVICE_ATECC608A)
ATCA_STATUS detect_crypto_device(void)
{
    ATCA_STATUS status;

    uint8_t addr_list[] = { ECC608A_DEFAULT_ADDRESS, ECC608A_ADDRESS };

    for (uint8_t addr_index = 0; addr_index < (sizeof(addr_list) / sizeof(addr_list[0])); addr_index++)
    {
        cfg_ateccx08a_i2c_default.atcai2c.slave_address = addr_list[addr_index];
        if ((status = atcab_init(&cfg_ateccx08a_i2c_default)) == ATCA_SUCCESS)
        {
            /*ECC608A with addr_list[addr_index] address is found */
            break;
        }
    }

    return status;
}
#endif


#endif