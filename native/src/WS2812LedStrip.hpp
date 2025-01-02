#include <vector>

// Some pieces inspired by the very helpful repository here: https://github.com/jgarff/rpi_ws281x/tree/master
// Note that we would simply re-use that library, but it has not been updated to work with Raspberry Pi 5 as of December 2024.
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

enum class LEDStripType : uint8_t
{
    GRB = uint8_t{0},
    RGB = uint8_t{1},
    GBR = uint8_t{2},
    RBG = uint8_t{3},
    BGR = uint8_t{4},
    BRG = uint8_t{5},
};

// Some pieces inspired by the very helpful repository here: https://github.com/jgarff/rpi_ws281x/
// Note that we would simply re-use that library, but it has not been updated to work with Raspberry Pi 5 as of December 2024.
class WS2812LedStrip
{
private:
    int ledCount;
    int spiFd;
    int frequencyHz;
    std::vector<std::byte> spiEncodedData; // SPI-encoded data.  0b100 == off, 0b110 == on.
    uint8_t rOffset;
    uint8_t gOffset;
    uint8_t bOffset;

    uint8_t getData(uint32_t index) const;
    void setData(uint32_t index, const uint8_t value);

public:
    static const int START_OFFSET = 9; // extra "0" byte(s) at the beginning of spiEncodedData to help make sure that the data starts with a clean "off"
    static const int LED_COLORS = 3;
    static const int RESET_TIME = 300; // microseconds
    static const int SPI_BITS_PER_BIT = 3;

    // defaults to 800 KHz frequency, GRB type
    WS2812LedStrip(int ledCount, int frequencyHz = 800 * 1000, LEDStripType type = LEDStripType::GRB) :
        ledCount(ledCount),
        spiFd(0),
        frequencyHz(frequencyHz),
        spiEncodedData(LED_COLORS * SPI_BITS_PER_BIT * ledCount + START_OFFSET)
    {
        for (int i = 0; i < START_OFFSET; i++)
        {
            this->spiEncodedData[i] = std::byte{0};
        }

        switch (type)
        {
            case LEDStripType::RGB:
                this->rOffset = uint8_t{0};
                this->gOffset = uint8_t{1};
                this->bOffset = uint8_t{2};
                break;

            case LEDStripType::GBR:
                this->rOffset = uint8_t{2};
                this->gOffset = uint8_t{0};
                this->bOffset = uint8_t{1};
                break;

            case LEDStripType::RBG:
                this->rOffset = uint8_t{0};
                this->gOffset = uint8_t{2};
                this->bOffset = uint8_t{1};
                break;

            case LEDStripType::BGR:
                this->rOffset = uint8_t{2};
                this->gOffset = uint8_t{1};
                this->bOffset = uint8_t{0};
                break;

            case LEDStripType::BRG:
                this->rOffset = uint8_t{1};
                this->gOffset = uint8_t{2};
                this->bOffset = uint8_t{0};
                break;

            default:
            case LEDStripType::GRB:
                this->rOffset = uint8_t{1};
                this->gOffset = uint8_t{0};
                this->bOffset = uint8_t{2};
                break;
        }
    }

    int openSpi(std::string spidev);
    void release();

    Color getColor(uint32_t index) const;
    void blank(uint32_t index, uint32_t count);
    void setColor(uint32_t index, const uint8_t r, const uint8_t g, const uint8_t b);
    void setColor(uint32_t startIndex, uint32_t count, const uint8_t r, const uint8_t g, const uint8_t b);
    void setColor(uint32_t index, const Color c);
    void setColor(uint32_t startIndex, uint32_t count, const Color c);
    int render();
};
