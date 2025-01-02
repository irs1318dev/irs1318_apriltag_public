#include <vector>

#ifdef IS_LINUX
#include "../piolib/include/piolib.h"
#endif

class Color
{
private:
    uint8_t red;
    uint8_t green;
    uint8_t blue;

public:
    Color(uint8_t red, uint8_t green, uint8_t blue) :
        red(red),
        green(green),
        blue(blue)
    {
    }

    uint8_t getRed() const
    {
        return this->red;
    }

    uint8_t getGreen() const
    {
        return this->green;
    }

    uint8_t getBlue() const
    {
        return this->blue;
    }
};

// Uses Raspberry Pi 5's PIO library from here: https://github.com/raspberrypi/utils/tree/master/piolib
class WS2812LedStrip
{
private:
    int ledCount;
#ifdef IS_LINUX
    PIO pio;
    int sm;
#endif
    int gpioPin;
    int frequencyHz;
    std::vector<std::byte> pioData; // pio data

public:
    static const int START_OFFSET = 0; // extra "0" byte(s) at the beginning of spiEncodedData to help make sure that the data starts with a clean "off"
    static const int BYTES_PER_PIXEL = 4; // the number of bytes per pixel (4 - 0, G, R, B)
    static const int RED_OFFSET = 2;
    static const int GREEN_OFFSET = 3;
    static const int BLUE_OFFSET = 1;

    // defaults to GPIO pin 4, 800 KHz frequency, GRB type
    WS2812LedStrip(int ledCount, int gpioPin = 4, int frequencyHz = 800 * 1000) :
        ledCount(ledCount),
        gpioPin(gpioPin),
        frequencyHz(frequencyHz),
        pioData()
    {
        this->pioData.resize(BYTES_PER_PIXEL * ledCount + START_OFFSET);
        for (int i = 0; i < START_OFFSET; i++)
        {
            this->pioData[i] = std::byte{0};
        }
    }

    int openPio();
    void release();

    Color getColor(uint32_t index) const;
    void blank(uint32_t index, uint32_t count);
    void setColor(uint32_t index, const uint8_t r, const uint8_t g, const uint8_t b);
    void setColor(uint32_t startIndex, uint32_t count, const uint8_t r, const uint8_t g, const uint8_t b);
    void setColor(uint32_t index, const Color c);
    void setColor(uint32_t startIndex, uint32_t count, const Color c);
    int render();
};

// borrowed directly from https://github.com/raspberrypi/utils/blob/master/piolib/examples/ws2812.pio.h:
#ifdef IS_LINUX
static const uint16_t ws2812_program_instructions[] = {
            //     .wrap_target
    0x6221, //  0: out    x, 1            side 0 [2] 
    0x1223, //  1: jmp    !x, 3           side 1 [2] 
    0x1300, //  2: jmp    0               side 1 [3] 
    0xa342, //  3: nop                    side 0 [3] 
            //     .wrap
};

static const struct pio_program ws2812_program = {
    .instructions = ws2812_program_instructions,
    .length = 4,
    .origin = -1,
};
#endif