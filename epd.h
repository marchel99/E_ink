/**
 *  @filename   :   epd4in2.h
 *  @brief      :   Header file for Dual-color e-paper library epd4in2.c
 *  @author     :   Yehui from Waveshare
 *  
 *  Copyright (C) Waveshare     August 10 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef EPD4IN2_H
#define EPD4IN2_H

//#include "epd4in2.h"

// Display resolution
#define EPD_WIDTH       400
#define EPD_HEIGHT      300

// EPD4IN2 commands
#define PANEL_SETTING                               0x00
#define POWER_SETTING                               0x01
#define POWER_OFF                                   0x02
#define POWER_OFF_SEQUENCE_SETTING                  0x03
#define POWER_ON                                    0x04
#define POWER_ON_MEASURE                            0x05
#define BOOSTER_SOFT_START                          0x06
#define DEEP_SLEEP                                  0x07
#define DATA_START_TRANSMISSION_1                   0x10
#define DATA_STOP                                   0x11
#define DISPLAY_REFRESH                             0x12
#define DATA_START_TRANSMISSION_2                   0x13
#define LUT_FOR_VCOM                                0x20 
#define LUT_WHITE_TO_WHITE                          0x21
#define LUT_BLACK_TO_WHITE                          0x22
#define LUT_WHITE_TO_BLACK                          0x23
#define LUT_BLACK_TO_BLACK                          0x24
#define PLL_CONTROL                                 0x30
#define TEMPERATURE_SENSOR_COMMAND                  0x40
#define TEMPERATURE_SENSOR_SELECTION                0x41
#define TEMPERATURE_SENSOR_WRITE                    0x42
#define TEMPERATURE_SENSOR_READ                     0x43
#define VCOM_AND_DATA_INTERVAL_SETTING              0x50
#define LOW_POWER_DETECTION                         0x51
#define TCON_SETTING                                0x60
#define RESOLUTION_SETTING                          0x61
#define GSST_SETTING                                0x65
#define GET_STATUS                                  0x71
#define AUTO_MEASUREMENT_VCOM                       0x80
#define READ_VCOM_VALUE                             0x81
#define VCM_DC_SETTING                              0x82
#define PARTIAL_WINDOW                              0x90
#define PARTIAL_IN                                  0x91
#define PARTIAL_OUT                                 0x92
#define PROGRAM_MODE                                0xA0
#define ACTIVE_PROGRAMMING                          0xA1
#define READ_OTP                                    0xA2
#define POWER_SAVING                                0xE3

extern const unsigned char lut_vcom0[];
extern const unsigned char lut_ww[];
extern const unsigned char lut_bw[];
extern const unsigned char lut_bb[];
extern const unsigned char lut_wb[];

extern const unsigned char EPD_4IN2_4Gray_lut_vcom[];
extern const unsigned char EPD_4IN2_4Gray_lut_ww[];
extern const unsigned char EPD_4IN2_4Gray_lut_bw[];
extern const unsigned char EPD_4IN2_4Gray_lut_wb[];
extern const unsigned char EPD_4IN2_4Gray_lut_bb[];

// Epd structure to hold the state of the e-paper display
typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned int reset_pin;
    unsigned int dc_pin;
    unsigned int cs_pin;
    unsigned int busy_pin;
} Epd;

// Function prototypes
void Epd_Init(Epd *epd);
int Epd_InitDisplay(Epd *epd);
int Epd_Init_4Gray(Epd *epd);
void Epd_SendCommand(Epd *epd, unsigned char command);
void Epd_SendData(Epd *epd, unsigned char data);
void Epd_WaitUntilIdle(Epd *epd);
void Epd_Reset(Epd *epd);
void Epd_SetPartialWindow(Epd *epd, const unsigned char* frame_buffer, int x, int y, int w, int l);
void Epd_SetPartialWindowBlack(Epd *epd, const unsigned char* buffer_black, int x, int y, int w, int l);
void Epd_SetPartialWindowRed(Epd *epd, const unsigned char* buffer_red, int x, int y, int w, int l);
void Epd_Set_4GrayDisplay(Epd *epd, const char *Image, int x, int y, int w, int l);
void Epd_SetLut(Epd *epd);
void Epd_set4Gray_lut(Epd *epd);
void Epd_DisplayFrame(Epd *epd, const unsigned char* frame_buffer);
void Epd_DisplayFrameWithoutBuffer(Epd *epd);
void Epd_ClearFrame(Epd *epd);
void Epd_Sleep(Epd *epd);

#endif /* EPD4IN2_H */

/* END OF FILE */
