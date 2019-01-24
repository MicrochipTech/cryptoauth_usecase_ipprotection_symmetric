/**
 * \file
 *
 * \brief Console Header file
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
#ifndef CONSOLE_H
#define CONSOLE_H

#include <stddef.h>

#define CONSOLE_LOG_MAX_SIZE    (500)
#define CONSOLE_LOG_ENABLED     1
#define OLED_LOG_ENABLED    1

extern char console_log[CONSOLE_LOG_MAX_SIZE];

//Macro to print on the OLED and in the EDBG COM Port
#define debug_print(fmt, ...)                                                   \
    do {                                            \
        if (CONSOLE_LOG_ENABLED) {                  \
            sprintf(console_log, fmt, __VA_ARGS__); \
            printf("%s", console_log);              \
            if (OLED_LOG_ENABLED) {   \
                print_on_oled(console_log); }    \
        }    \
    } while (0)

//Macro to print on the OLED
#define debug_print_oled(fmt, ...)                                              \
    do {                                            \
        if (CONSOLE_LOG_ENABLED) {                  \
            sprintf(console_log, fmt, __VA_ARGS__); \
            print_on_oled(console_log); }    \
    } while (0)

//Macro to print on the EDBG COM Port
#define debug_print_uart(fmt, ...)                                          \
    do {                                            \
        if (CONSOLE_LOG_ENABLED) {                  \
            sprintf(console_log, fmt, __VA_ARGS__); \
            printf("%s", console_log); }  \
    } while (0)

void console_init(void);
void print_on_oled(const char *data);

#endif // CONSOLE_H
