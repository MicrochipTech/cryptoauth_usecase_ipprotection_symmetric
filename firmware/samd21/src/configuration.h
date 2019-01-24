/**
 * \file
 * \brief  Configuration file for the device
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


#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#define DEVICE_ATSHA204A 0
#define DEVICE_ATECC508A 1
#define DEVICE_ATECC608A 2

//To support provisioning of the device, by default it is set to provision the device
#define  IP_PROTECTION_LOAD_CONFIG 1

//To select the device for the Authentication, by default set to ATSHA204A device.
#define  CRYPTOAUTH_DEVICE DEVICE_ATSHA204A

//The device slot containing the symmetric key to be used for authentication
#define CRYPTOAUTH_DEVICE_AUTH_KEY_SLOT 6

//The minimum wait period for the authentication to happen
#define AUTHENTICATION_MIN_MSEC  1500

//The Maximum wait time is random value between AUTHENTICATION_MIN_MSEC and  AUTHENTICATION_MIN_MSEC + AUTHENTICATION_RANGE_MSEC
#define AUTHENTICATION_RANGE_MSEC 3000


#endif /* CONFIGURATION_H_ */