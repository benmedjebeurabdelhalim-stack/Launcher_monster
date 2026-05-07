#ifndef VOLTSHIELD_X_H
#define VOLTSHIELD_X_H

#include <LovyanGFX.hpp>

// تفعيل اللمس وإجبار إعدادات بطاقة الـ SD 
#define HAS_TOUCH 1
#define SDCARD_CS   21
#define SDCARD_SCK  12
#define SDCARD_MISO 13
#define SDCARD_MOSI 11
#define SD_CS       21
#define SD_SCK      12
#define SD_MISO     13
#define SD_MOSI     11

// الألوان الأساسية للقوائم
#define DARKGREY TFT_DARKGREY
#define BLACK TFT_BLACK
#define RED TFT_RED
#define GREEN TFT_GREEN
#define WHITE TFT_WHITE
#define DARKCYAN TFT_DARKCYAN
#define LIGHTGREY TFT_LIGHTGREY

class Ard_eSPI : public lgfx::LGFX_Device {
    lgfx::Panel_ST7796     _panel_instance;
    lgfx::Bus_SPI          _bus_instance;
    lgfx::Touch_XPT2046    _touch_instance;

public:
    inline int getTextsize() { return _text_style.size_x; };
    inline uint16_t getTextcolor() { return _text_style.fore_rgb888; };
    inline uint16_t getTextbgcolor() { return _text_style.back_rgb888; };
    inline void drawChar2(int16_t x, int16_t y, char c, int16_t a, int16_t b) {
        lgfx::LGFX_Device::drawChar(x, y, c, a, b, _text_style.size_x);
    }
    inline void drawCentreString(String s, uint16_t x, uint16_t y, int f) {
        lgfx::LGFX_Device::drawCentreString(s, x, y);
    };
    inline void drawRightString(String s, uint16_t x, uint16_t y, int f) {
        lgfx::LGFX_Device::drawRightString(s, x, y);
    };
    inline void display(bool a = false) {};

    Ard_eSPI(void) {
        { // 1. إعداد ناقل SPI المشترك
            auto cfg = _bus_instance.config();
            cfg.spi_host = SPI2_HOST; 
            cfg.spi_mode = 0;
            cfg.freq_write = 40000000; 
            cfg.freq_read  = 16000000; 
            cfg.spi_3wire  = false;
            cfg.use_lock   = true;
            cfg.dma_channel = SPI_DMA_CH_AUTO;
            
            cfg.pin_sclk = 12; // SCK
            cfg.pin_mosi = 11; // MOSI
            cfg.pin_miso = 13; // MISO
            cfg.pin_dc   = 14; // TFT_DC
            
            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }
        { // 2. إعداد شاشة ST7796
            auto cfg = _panel_instance.config();
            cfg.pin_cs   = 10; // TFT_CS
            cfg.pin_rst  = 5;  // TFT_RST
            cfg.panel_width  = 320;
            cfg.panel_height = 480;
            cfg.offset_x     = 0;
            cfg.offset_y     = 0;
            cfg.offset_rotation = 0;
            cfg.dummy_read_pixel = 8;
            cfg.dummy_read_bits  = 1;
            cfg.readable         = true;
            cfg.invert           = true;  
            cfg.rgb_order        = false;
            cfg.dlen_16bit       = false;
            cfg.bus_shared       = true;  // السماح بمشاركة الناقل
            
            _panel_instance.config(cfg);
        }
        { // 3. إعداد اللمس XPT2046
            auto cfg = _touch_instance.config();
            cfg.x_min      = 0;
            cfg.x_max      = 319;
            cfg.y_min      = 0;
            cfg.y_max      = 479;
            cfg.pin_int    = 2;  // TOUCH_IRQ
            cfg.pin_cs     = 4;  // TOUCH_CS
            cfg.bus_shared = true;
            cfg.offset_rotation = 0;
            cfg.spi_host   = SPI2_HOST;
            cfg.freq       = 1000000;
            
            _touch_instance.config(cfg);
            _panel_instance.setTouch(&_touch_instance);
        }
        setPanel(&_panel_instance);
    }
};

#endif // VOLTSHIELD_X_H
