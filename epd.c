
#include <stdlib.h>
#include "epd4in2.h"

// Epd structure to hold the state of the e-paper display
typedef struct {
    int reset_pin;
    int dc_pin;
    int cs_pin;
    int busy_pin;
    int width;
    int height;
} Epd;

// Function prototypes
void Epd_Init(Epd *epd);
int Epd_InitDisplay(Epd *epd);
int Epd_Init_4Gray(Epd *epd);
void Epd_SendCommand(Epd *epd, unsigned char command);
void Epd_SendData(Epd *epd, unsigned char data);
void Epd_WaitUntilIdle(Epd *epd);
void Epd_Reset(Epd *epd);
void Epd_SetPartialWindow(Epd *epd, const unsigned char* buffer_black, int x, int y, int w, int l);
void Epd_Set_4GrayDisplay(Epd *epd, const char *Image, int x, int y, int w, int l);
void Epd_SetLut(Epd *epd);
void Epd_set4Gray_lut(Epd *epd);
void Epd_DisplayFrame(Epd *epd, const unsigned char* frame_buffer);
void Epd_ClearFrame(Epd *epd);
void Epd_DisplayFrameWithoutBuffer(Epd *epd);
void Epd_Sleep(Epd *epd);

// LUT data arrays

/*
const unsigned char lut_vcom0[] = { ... }; // Define as in C++ code
const unsigned char lut_ww[] = { ... }; // Define as in C++ code
const unsigned char lut_bw[] = { ... }; // Define as in C++ code
const unsigned char lut_bb[] = { ... }; // Define as in C++ code
const unsigned char lut_wb[] = { ... }; // Define as in C++ code
const unsigned char EPD_4IN2_4Gray_lut_vcom[] = { ... }; // Define as in C++ code
const unsigned char EPD_4IN2_4Gray_lut_ww[] = { ... }; // Define as in C++ code
const unsigned char EPD_4IN2_4Gray_lut_bw[] = { ... }; // Define as in C++ code
const unsigned char EPD_4IN2_4Gray_lut_wb[] = { ... }; // Define as in C++ code
const unsigned char EPD_4IN2_4Gray_lut_bb[] = { ... }; // Define as in C++ code
*/



// Function implementations
void Epd_Init(Epd *epd) {
    epd->reset_pin = RST_PIN;
    epd->dc_pin = DC_PIN;
    epd->cs_pin = CS_PIN;
    epd->busy_pin = BUSY_PIN;
    epd->width = EPD_WIDTH;
    epd->height = EPD_HEIGHT;
}

int Epd_InitDisplay(Epd *epd) {
    if (IfInit() != 0) {
        return -1;
    }
    Epd_Reset(epd);
    Epd_SendCommand(epd, 0x01);
    Epd_SendData(epd, 0x03);
    Epd_SendData(epd, 0x00);
    Epd_SendData(epd, 0x2b);
    Epd_SendData(epd, 0x2b);
    Epd_SendCommand(epd, 0x06);
    Epd_SendData(epd, 0x17);
    Epd_SendData(epd, 0x17);
    Epd_SendData(epd, 0x17);
    Epd_SendCommand(epd, 0x04);
    Epd_WaitUntilIdle(epd);
    Epd_SendCommand(epd, 0x00);
    Epd_SendData(epd, 0xbf);
    Epd_SendCommand(epd, 0x30);
    Epd_SendData(epd, 0x3c);
    Epd_SendCommand(epd, 0x61);
    Epd_SendData(epd, 0x01);
    Epd_SendData(epd, 0x90);
    Epd_SendData(epd, 0x01);
    Epd_SendData(epd, 0x2c);
    Epd_SendCommand(epd, 0x82);
    Epd_SendData(epd, 0x12);
    Epd_SendCommand(epd, 0x50);
    Epd_SendData(epd, 0x97);
    Epd_SetLut(epd);
    return 0;
}

int Epd_Init_4Gray(Epd *epd) {
    if (IfInit() != 0) {
        return -1;
    }
    Epd_Reset(epd);
    Epd_SendCommand(epd, 0x01);
    Epd_SendData(epd, 0x03);
    Epd_SendData(epd, 0x00);
    Epd_SendData(epd, 0x2b);
    Epd_SendData(epd, 0x2b);
    Epd_SendData(epd, 0x13);
    Epd_SendCommand(epd, 0x06);
    Epd_SendData(epd, 0x17);
    Epd_SendData(epd, 0x17);
    Epd_SendData(epd, 0x17);
    Epd_SendCommand(epd, 0x04);
    Epd_WaitUntilIdle(epd);
    Epd_SendCommand(epd, 0x00);
    Epd_SendData(epd, 0x3f);
    Epd_SendCommand(epd, 0x30);
    Epd_SendData(epd, 0x3c);
    Epd_SendCommand(epd, 0x61);
    Epd_SendData(epd, 0x01);
    Epd_SendData(epd, 0x90);
    Epd_SendData(epd, 0x01);
    Epd_SendData(epd, 0x2c);
    Epd_SendCommand(epd, 0x82);
    Epd_SendData(epd, 0x12);
    Epd_SendCommand(epd, 0x50);
    Epd_SendData(epd, 0x97);
    return 0;
}

void Epd_SendCommand(Epd *epd, unsigned char command) {
    DigitalWrite(epd->dc_pin, LOW);
    SpiTransfer(command);
}

void Epd_SendData(Epd *epd, unsigned char data) {
    DigitalWrite(epd->dc_pin, HIGH);
    SpiTransfer(data);
}

void Epd_WaitUntilIdle(Epd *epd) {
    while (DigitalRead(epd->busy_pin) == 0) {
        // Wait
    }
}

void Epd_Reset(Epd *epd) {
    DigitalWrite(epd->reset_pin, LOW);
    DelayMs(2);
    DigitalWrite(epd->reset_pin, HIGH);
    DelayMs(20);
    DigitalWrite(epd->reset_pin, LOW);
    DelayMs(2);
    DigitalWrite(epd->reset_pin, HIGH);
    DelayMs(20);
    DigitalWrite(epd->reset_pin, LOW);
    DelayMs(2);
    DigitalWrite(epd->reset_pin, HIGH);
    DelayMs(20);
}

void Epd_SetPartialWindow(Epd *epd, const unsigned char* buffer_black, int x, int y, int w, int l) {
    Epd_SendCommand(epd, PARTIAL_IN);
    Epd_SendCommand(epd, PARTIAL_WINDOW);
    Epd_SendData(epd, x >> 8);
    Epd_SendData(epd, x & 0xf8);
    Epd_SendData(epd, ((x & 0xf8) + w - 1) >> 8);
    Epd_SendData(epd, ((x & 0xf8) + w - 1) | 0x07);
    Epd_SendData(epd, y >> 8);
    Epd_SendData(epd, y & 0xff);
    Epd_SendData(epd, (y + l - 1) >> 8);
    Epd_SendData(epd, (y + l - 1) & 0xff);
    Epd_SendData(epd, 0x01);
    DelayMs(2);
    Epd_SendCommand(epd, DATA_START_TRANSMISSION_2);
    if (buffer_black != NULL) {
        for (int i = 0; i < w / 8 * l; i++) {
            Epd_SendData(epd, buffer_black[i]);
        }
    } else {
        for (int i = 0; i < w / 8 * l; i++) {
            Epd_SendData(epd, 0x00);
        }
    }
    DelayMs(2);
    Epd_SendCommand(epd, PARTIAL_OUT);
}

void Epd_Set_4GrayDisplay(Epd *epd, const char *Image, int x, int y, int w, int l) {
    int i, j, k, m;
    int z = 0;
    unsigned char temp1, temp2, temp3;
    Epd_SendCommand(epd, 0x10);
    z = 0;
    x = x / 8 * 8;
    for (m = 0; m < EPD_HEIGHT; m++)
        for (i = 0; i < EPD_WIDTH / 8; i++) {
            if (i >= x / 8 && i < (x + w) / 8 && m >= y && m < y + l) {
                temp3 = 0;
                for (j = 0; j < 2; j++) {
                    temp1 = pgm_read_byte(&Image[z * 2 + j]);
                    for (k = 0; k < 2; k++) {
                        temp2 = temp1 & 0xC0;
                        if (temp2 == 0xC0)
                            temp3 |= 0x01; // white
                        else if (temp2 == 0x00)
                            temp3 |= 0x00; // black
                        else if (temp2 == 0x80)
                            temp3 |= 0x01; // gray1
                        else // 0x40
                            temp3 |= 0x00; // gray2
                        temp3 <<= 1;
                        temp1 <<= 2;
                        temp2 = temp1 & 0xC0;
                        if (temp2 == 0xC0) // white
                            temp3 |= 0x01;
                        else if (temp2 == 0x00) // black
                            temp3 |= 0x00;
                        else if (temp2 == 0x80)
                            temp3 |= 0x01; // gray1
                        else // 0x40
                            temp3 |= 0x00; // gray2
                        if (j != 1 || k != 1)
                            temp3 <<= 1;
                        temp1 <<= 2;
                    }
                }
                z++;
                Epd_SendData(epd, temp3);
            } else {
                Epd_SendData(epd, 0xff);
            }
        }
    Epd_SendCommand(epd, 0x13);
    z = 0;
    for (m = 0; m < EPD_HEIGHT; m++)
        for (i = 0; i < EPD_WIDTH / 8; i++) {
            if (i >= x / 8 && i < (x + w) / 8 && m >= y && m < y + l) {
                temp3 = 0;
                for (j = 0; j < 2; j++) {
                    temp1 = pgm_read_byte(&Image[z * 2 + j]);
                    for (k = 0; k < 2; k++) {
                        temp2 = temp1 & 0xC0;
                        if (temp2 == 0xC0)
                            temp3 |= 0x01; // white
                        else if (temp2 == 0x00)
                            temp3 |= 0x00; // black
                        else if (temp2 == 0x80)
                            temp3 |= 0x00; // gray1
                        else // 0x40
                            temp3 |= 0x01; // gray2
                        temp3 <<= 1;
                        temp1 <<= 2;
                        temp2 = temp1 & 0xC0;
                        if (temp2 == 0xC0) // white
                            temp3 |= 0x01;
                        else if (temp2 == 0x00) // black
                            temp3 |= 0x00;
                        else if (temp2 == 0x80)
                            temp3 |= 0x00; // gray1
                        else // 0x40
                            temp3 |= 0x01; // gray2
                        if (j != 1 || k != 1)
                            temp3 <<= 1;
                        temp1 <<= 2;
                    }
                }
                z++;
                Epd_SendData(epd, temp3);
            } else {
                Epd_SendData(epd, 0xff);
            }
        }
    Epd_set4Gray_lut(epd);
    Epd_SendCommand(epd, DISPLAY_REFRESH);
    DelayMs(100);
    Epd_WaitUntilIdle(epd);
}

void Epd_SetLut(Epd *epd) {
    unsigned int count;
    Epd_SendCommand(epd, LUT_FOR_VCOM);
    for (count = 0; count < 36; count++) {
        Epd_SendData(epd, lut_vcom0[count]);
    }
    Epd_SendCommand(epd, LUT_WHITE_TO_WHITE);
    for (count = 0; count < 36; count++) {
        Epd_SendData(epd, lut_ww[count]);
    }
    Epd_SendCommand(epd, LUT_BLACK_TO_WHITE);
    for (count = 0; count < 36; count++) {
        Epd_SendData(epd, lut_bw[count]);
    }
    Epd_SendCommand(epd, LUT_WHITE_TO_BLACK);
    for (count = 0; count < 36; count++) {
        Epd_SendData(epd, lut_bb[count]);
    }
    Epd_SendCommand(epd, LUT_BLACK_TO_BLACK);
    for (count = 0; count < 36; count++) {
        Epd_SendData(epd, lut_wb[count]);
    }
}

void Epd_set4Gray_lut(Epd *epd) {
    unsigned int count;
    Epd_SendCommand(epd, 0x20);
    for (count = 0; count < 42; count++) {
        Epd_SendData(epd, EPD_4IN2_4Gray_lut_vcom[count]);
    }
    Epd_SendCommand(epd, 0x21);
    for (count = 0; count < 42; count++) {
        Epd_SendData(epd, EPD_4IN2_4Gray_lut_ww[count]);
    }
    Epd_SendCommand(epd, 0x22);
    for (count = 0; count < 42; count++) {
        Epd_SendData(epd, EPD_4IN2_4Gray_lut_bw[count]);
    }
    Epd_SendCommand(epd, 0x23);
    for (count = 0; count < 42; count++) {
        Epd_SendData(epd, EPD_4IN2_4Gray_lut_wb[count]);
    }
    Epd_SendCommand(epd, 0x24);
    for (count = 0; count < 42; count++) {
        Epd_SendData(epd, EPD_4IN2_4Gray_lut_bb[count]);
    }
    Epd_SendCommand(epd, 0x25);
    for (count = 0; count < 42; count++) {
        Epd_SendData(epd, EPD_4IN2_4Gray_lut_ww[count]);
    }
}

void Epd_DisplayFrame(Epd *epd, const unsigned char* frame_buffer) {
    Epd_SendCommand(epd, RESOLUTION_SETTING);
    Epd_SendData(epd, epd->width >> 8);
    Epd_SendData(epd, epd->width & 0xff);
    Epd_SendData(epd, epd->height >> 8);
    Epd_SendData(epd, epd->height & 0xff);
    Epd_SendCommand(epd, VCM_DC_SETTING);
    Epd_SendData(epd, 0x12);
    Epd_SendCommand(epd, VCOM_AND_DATA_INTERVAL_SETTING);
    Epd_SendCommand(epd, 0x97);
    if (frame_buffer != NULL) {
        Epd_SendCommand(epd, DATA_START_TRANSMISSION_1);
        for (int i = 0; i < epd->width / 8 * epd->height; i++) {
            Epd_SendData(epd, 0xFF);
        }
        DelayMs(2);
        Epd_SendCommand(epd, DATA_START_TRANSMISSION_2);
        for (int i = 0; i < epd->width / 8 * epd->height; i++) {
            Epd_SendData(epd, pgm_read_byte(&frame_buffer[i]));
        }
        DelayMs(2);
    }
    Epd_SendCommand(epd, 0x12);
    DelayMs(100);
    Epd_WaitUntilIdle(epd);
}

void Epd_ClearFrame(Epd *epd) {
    Epd_SendCommand(epd, RESOLUTION_SETTING);
    Epd_SendData(epd, epd->width >> 8);
    Epd_SendData(epd, epd->width & 0xff);
    Epd_SendData(epd, epd->height >> 8);
    Epd_SendData(epd, epd->height & 0xff);
    Epd_SendCommand(epd, DATA_START_TRANSMISSION_1);
    DelayMs(2);
    for (int i = 0; i < epd->width / 8 * epd->height; i++) {
        Epd_SendData(epd, 0xFF);
    }
    DelayMs(2);
    Epd_SendCommand(epd, DATA_START_TRANSMISSION_2);
    DelayMs(2);
    for (int i = 0; i < epd->width / 8 * epd->height; i++) {
        Epd_SendData(epd, 0xFF);
    }
    DelayMs(2);
}

void Epd_DisplayFrameWithoutBuffer(Epd *epd) {
    Epd_SetLut(epd);
    Epd_SendCommand(epd, DISPLAY_REFRESH);
    DelayMs(100);
    Epd_WaitUntilIdle(epd);
}

void Epd_Sleep(Epd *epd) {
    Epd_SendCommand(epd, VCOM_AND_DATA_INTERVAL_SETTING);
    Epd_SendData(epd, 0x17);
    Epd_SendCommand(epd, VCM_DC_SETTING);
    Epd_SendCommand(epd, PANEL_SETTING);
    DelayMs(100);
    Epd_SendCommand(epd, POWER_SETTING);
    Epd_SendData(epd, 0x00);
    Epd_SendData(epd, 0x00);
    Epd_SendData(epd, 0x00);
    Epd_SendData(epd, 0x00);
    Epd_SendData(epd, 0x00);
    DelayMs(100);
    Epd_SendCommand(epd, POWER_OFF);
    Epd_WaitUntilIdle(epd);
    Epd_SendCommand(epd, DEEP_SLEEP);
    Epd_SendData(epd, 0xA5);
}



const unsigned char lut_vcom0[] =
{
    0x00, 0x08, 0x08, 0x00, 0x00, 0x02,  
  0x00, 0x0F, 0x0F, 0x00, 0x00, 0x01, 
  0x00, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,   

};
const unsigned char lut_ww[] ={
  0x50, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x90, 0x0F, 0x0F, 0x00, 0x00, 0x01, 
  0xA0, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

};
const unsigned char lut_bw[] ={
  0x50, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x90, 0x0F, 0x0F, 0x00, 0x00, 0x01, 
  0xA0, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      
};

const unsigned char lut_bb[] ={
  0xA0, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x90, 0x0F, 0x0F, 0x00, 0x00, 0x01, 
  0x50, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
             
};

const unsigned char lut_wb[] ={
  0x20, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x90, 0x0F, 0x0F, 0x00, 0x00, 0x01, 
  0x10, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
            
};

/******************************gray*********************************/
//0~3 gray
const unsigned char EPD_4IN2_4Gray_lut_vcom[] =
{
0x00	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x60	,0x14	,0x14	,0x00	,0x00	,0x01,
0x00	,0x14	,0x00	,0x00	,0x00	,0x01,
0x00	,0x13	,0x0A	,0x01	,0x00	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00
				
};
//R21
const unsigned char EPD_4IN2_4Gray_lut_ww[] ={
0x40	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
0x10	,0x14	,0x0A	,0x00	,0x00	,0x01,
0xA0	,0x13	,0x01	,0x00	,0x00	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};
//R22H	r
const unsigned char EPD_4IN2_4Gray_lut_bw[] ={
0x40	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
0x00	,0x14	,0x0A	,0x00	,0x00	,0x01,
0x99	,0x0C	,0x01	,0x03	,0x04	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};
//R23H	w
const unsigned char EPD_4IN2_4Gray_lut_wb[] ={
0x40	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
0x00	,0x14	,0x0A	,0x00	,0x00	,0x01,
0x99	,0x0B	,0x04	,0x04	,0x01	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};
//R24H	b
const unsigned char EPD_4IN2_4Gray_lut_bb[] ={
0x80	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
0x20	,0x14	,0x0A	,0x00	,0x00	,0x01,
0x50	,0x13	,0x01	,0x00	,0x00	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};


/* END OF FILE */
