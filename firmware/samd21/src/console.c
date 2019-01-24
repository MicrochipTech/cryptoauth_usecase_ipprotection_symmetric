/**
 * \file
 *
 * \brief  OLED and Console Configuration file
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
#include "console.h"
#include "oled1.h"
#include "conf_sysfont.h"
#include <string.h>
//! Height of area in which to draw content
#define CANVAS_HEIGHT           (GFX_MONO_LCD_HEIGHT)

//! Width of area in which to draw content
#define CANVAS_WIDTH            (GFX_MONO_LCD_WIDTH)

//! Character columns on display
#define TERMINAL_COLUMNS        (CANVAS_WIDTH / SYSFONT_WIDTH)

//! Character lines in terminal buffer
#define TERMINAL_BUFFER_LINES   (GFX_MONO_LCD_HEIGHT / (SYSFONT_HEIGHT + 1))

//! Character columns in terminal buffer
#define TERMINAL_BUFFER_COLUMNS (1 + TERMINAL_COLUMNS)


// Global variable
static struct usart_module g_usart_instance;  // The USART module instance
static OLED1_CREATE_INSTANCE(oled1, OLED1_EXT_HEADER);
static uint8_t terminal_buffer[TERMINAL_BUFFER_LINES][TERMINAL_BUFFER_COLUMNS];
char console_log[CONSOLE_LOG_MAX_SIZE];

/**
 * \brief Initializes the console EDBG USART interface.
 */
void console_init(void)
{
    struct usart_config usart_configuration;

    usart_get_config_defaults(&usart_configuration);
    usart_configuration.baudrate    = 115200;
    usart_configuration.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
    usart_configuration.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
    usart_configuration.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
    usart_configuration.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
    usart_configuration.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
    stdio_serial_init(&g_usart_instance, EDBG_CDC_MODULE, &usart_configuration);
    usart_enable(&g_usart_instance);


    gfx_mono_init();
    oled1_init(&oled1);
    gfx_mono_draw_filled_rect(0, 0, GFX_MONO_LCD_WIDTH, GFX_MONO_LCD_HEIGHT / 2, GFX_PIXEL_CLR);
}

void print_on_oled(const char *data)
{
    static gfx_coord_t x, y;
    static uint8_t terminal_column;
    static uint8_t terminal_line_offset;

    uint8_t current_line;
    uint8_t printed_lines;
    char current_char;
    uint8_t current_column;

    current_char = *data++;
    while (current_char != '\0')
    {
        switch (current_char)
        {
        case '\n':
            break;

        case '\r':
            terminal_column = 0;
            terminal_line_offset++;
            if (terminal_line_offset >= TERMINAL_BUFFER_LINES)
            {
                terminal_line_offset -= 1;
                memcpy(terminal_buffer[0], terminal_buffer[1], TERMINAL_COLUMNS);
                memcpy(terminal_buffer[1], terminal_buffer[2], TERMINAL_COLUMNS);
                memcpy(terminal_buffer[2], terminal_buffer[3], TERMINAL_COLUMNS);
                memset(terminal_buffer[3], 0, TERMINAL_COLUMNS);
            }
            break;

        default:
            if (terminal_column >= TERMINAL_COLUMNS)
            {
                terminal_column = 0;
                terminal_line_offset++;
                if (terminal_line_offset >= TERMINAL_BUFFER_LINES)
                {
                    terminal_line_offset -= 1;
                    memcpy(terminal_buffer[0], terminal_buffer[1], TERMINAL_COLUMNS);
                    memcpy(terminal_buffer[1], terminal_buffer[2], TERMINAL_COLUMNS);
                    memcpy(terminal_buffer[2], terminal_buffer[3], TERMINAL_COLUMNS);
                    memset(terminal_buffer[3], 0, TERMINAL_COLUMNS);
                }
            }
            terminal_buffer[terminal_line_offset][terminal_column++] = current_char;
            break;
        }
        current_char = *data++;
    }

    y = 0;
    current_line = 0;

    for (printed_lines = 0; printed_lines < TERMINAL_BUFFER_LINES; printed_lines++)
    {
        x = 0;
        current_column = 0;

        // Print characters of string until zero terminator is encountered
        current_char = terminal_buffer[current_line][current_column];
        while (current_char != '\0')
        {
            gfx_mono_draw_char(current_char, x, y, &sysfont);

            // Move to next character on display and in buffer
            x += SYSFONT_WIDTH;
            current_column++;
            current_char = terminal_buffer[current_line][current_column];
        }

        // Erase remaining part of line on display
        if (current_column < TERMINAL_COLUMNS)
        {
            gfx_mono_draw_filled_rect(x, y,
                                      CANVAS_WIDTH - (current_column * SYSFONT_WIDTH),
                                      SYSFONT_HEIGHT, GFX_PIXEL_CLR);
        }

        // Move to next line on display and in buffer
        y += 1 + SYSFONT_HEIGHT;
        current_line++;
        current_line %= TERMINAL_BUFFER_LINES;

    }
}



