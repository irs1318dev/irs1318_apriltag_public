

#include <jni.h>
#include <iostream>
#include <cstdint>

#ifdef IS_LINUX
#include <fcntl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif

#include "WS2812LedStrip.hpp"
#include <iomanip>

/*
 * Class:     frc1318_ws2812_WS2812LedStrip
 * Method:    createStrip
 * Signature: (I)J
 */
extern "C"
JNIEXPORT jlong JNICALL Java_frc1318_ws2812_WS2812LedStrip_createStrip
    (JNIEnv* env, jclass objClass, jint ledCount)
{
    WS2812LedStrip* ledStrip = new WS2812LedStrip(static_cast<int>(ledCount));
    return reinterpret_cast<jlong>(ledStrip);
}

/*
 * Class:     frc1318_ws2812_WS2812LedStrip
 * Method:    open
 * Signature: (J)I
 */
extern "C"
JNIEXPORT jint JNICALL Java_frc1318_ws2812_WS2812LedStrip_open
    (JNIEnv* env, jclass objClass, jlong nativeObj)
{
    WS2812LedStrip* ledStrip = reinterpret_cast<WS2812LedStrip*>(nativeObj);
    return static_cast<jint>(ledStrip->openSpi("/dev/spidev0.0"));
}

/*
 * Class:     frc1318_ws2812_WS2812LedStrip
 * Method:    release
 * Signature: (J)V
 */
extern "C"
JNIEXPORT void JNICALL Java_frc1318_ws2812_WS2812LedStrip_release
    (JNIEnv* env, jclass objClass, jlong nativeObj)
{
    WS2812LedStrip* ledStrip = reinterpret_cast<WS2812LedStrip*>(nativeObj);
    ledStrip->release();
    delete ledStrip;
}

/*
 * Class:     frc1318_ws2812_WS2812LedStrip
 * Method:    blank
 * Signature: (JII)V
 */
extern "C"
JNIEXPORT void JNICALL Java_frc1318_ws2812_WS2812LedStrip_blank
    (JNIEnv* env, jclass objClass, jlong nativeObj, jint startIndex, jint count)
{
    WS2812LedStrip* ledStrip = reinterpret_cast<WS2812LedStrip*>(nativeObj);
    ledStrip->blank(static_cast<uint32_t>(startIndex), static_cast<uint32_t>(count));
}

/*
 * Class:     frc1318_ws2812_WS2812LedStrip
 * Method:    setColor
 * Signature: (JIBBB)V
 */
extern "C"
JNIEXPORT void JNICALL Java_frc1318_ws2812_WS2812LedStrip_setColor__JIBBB
    (JNIEnv* env, jclass objClass, jlong nativeObj, jint index, jbyte r, jbyte g, jbyte b)
{
    WS2812LedStrip* ledStrip = reinterpret_cast<WS2812LedStrip*>(nativeObj);
    ledStrip->setColor(static_cast<uint32_t>(index), static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b));
}

/*
 * Class:     frc1318_ws2812_WS2812LedStrip
 * Method:    setColor
 * Signature: (JIIBBB)V
 */
extern "C"
JNIEXPORT void JNICALL Java_frc1318_ws2812_WS2812LedStrip_setColor__JIIBBB
    (JNIEnv* env, jclass objClass, jlong nativeObj, jint startIndex, jint count, jbyte r, jbyte g, jbyte b)
{
    WS2812LedStrip* ledStrip = reinterpret_cast<WS2812LedStrip*>(nativeObj);
    ledStrip->setColor(static_cast<uint32_t>(startIndex), static_cast<uint32_t>(count), static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b));
}

/*
 * Class:     frc1318_ws2812_WS2812LedStrip
 * Method:    render
 * Signature: (J)I
 */
extern "C"
JNIEXPORT jint JNICALL Java_frc1318_ws2812_WS2812LedStrip_render
    (JNIEnv* env, jclass objClass, jlong nativeObj)
{
    WS2812LedStrip* ledStrip = reinterpret_cast<WS2812LedStrip*>(nativeObj);
    return ledStrip->render();
}

Color WS2812LedStrip::getColor(uint32_t index) const
{
    uint32_t red = this->getData(index + rOffset);
    uint32_t green = this->getData(index + gOffset);
    uint32_t blue = this->getData(index + bOffset);
    return Color(red, green, blue);
}

void WS2812LedStrip::blank(uint32_t index, uint32_t count)
{
    const std::byte byte1{uint8_t{0b10010010}};
    const std::byte byte2{uint8_t{0b01001001}};
    const std::byte byte3{uint8_t{0b00100100}};
    for (uint32_t i = 0; i < count * WS2812LedStrip::LED_COLORS; i++)
    {
        this->spiEncodedData[START_OFFSET + (index + i) * WS2812LedStrip::LED_COLORS + 0] = byte1;
        this->spiEncodedData[START_OFFSET + (index + i) * WS2812LedStrip::LED_COLORS + 1] = byte2;
        this->spiEncodedData[START_OFFSET + (index + i) * WS2812LedStrip::LED_COLORS + 2] = byte3;
    }
}

void WS2812LedStrip::setColor(uint32_t index, const Color c)
{
    this->setColor(index, c.getRed(), c.getGreen(), c.getBlue());
}

void WS2812LedStrip::setColor(uint32_t startIndex, uint32_t count, const Color c)
{
    this->setColor(startIndex, count, c.getRed(), c.getGreen(), c.getBlue());
}

void WS2812LedStrip::setColor(uint32_t startIndex, uint32_t count, const uint8_t r, const uint8_t g, const uint8_t b)
{
    // note: we could probably make this more efficient by doing the bit manipulation in advance and applying it many times
    // instead of repeating the calculation for each pixel (like we do for blanking).
    for (uint32_t i = startIndex; i < startIndex + count; i++)
    {
        this->setColor(i, r, g, b);
    }
}

void WS2812LedStrip::setColor(uint32_t index, const uint8_t r, const uint8_t g, const uint8_t b)
{
    uint32_t startingOffset = (index + rOffset) * 3;
    this->setData((index * WS2812LedStrip::LED_COLORS) + rOffset, r);
    this->setData((index * WS2812LedStrip::LED_COLORS) + gOffset, g);
    this->setData((index * WS2812LedStrip::LED_COLORS) + bOffset, b);
}

uint8_t WS2812LedStrip::getData(uint32_t index) const
{
    // we expect to have 0b 1f01g01h 01i01j01 k01l01m0 (split across 3 bytes), and want a result value with bits (0b fghijklm), 
    // (1f01g01h & 01001001) => 0f00g00h
    // (01i01j01 & 00100100) => 00i00j00
    // (k01l01m0 & 10010010) => k00l00m0
    // Also, sometimes C++ seems to widen the type after a bitwise operator.  So we have to throw in a bunch of static_casts,
    // in addition to uint8_t-ifying literals :-(
    // Isn't bit manipulation fun?!
    uint8_t result = 0;
    uint8_t byte1 = static_cast<uint8_t>(this->spiEncodedData[START_OFFSET + index * 3 + 0]) & uint8_t{0b01001001};
    uint8_t byte2 = static_cast<uint8_t>(this->spiEncodedData[START_OFFSET + index * 3 + 1]) & uint8_t{0b00100100};
    uint8_t byte3 = static_cast<uint8_t>(this->spiEncodedData[START_OFFSET + index * 3 + 2]) & uint8_t{0b10010010};
    result +=
        static_cast<uint8_t>(static_cast<uint8_t>((byte1 & uint8_t{0x40}) << 1) & uint8_t{0x80}) +
        static_cast<uint8_t>(static_cast<uint8_t>((byte1 & uint8_t{0x08}) << 3) & uint8_t{0x40}) +
        static_cast<uint8_t>(static_cast<uint8_t>((byte1 & uint8_t{0x01}) << 5) & uint8_t{0x20});
    result +=
        static_cast<uint8_t>(static_cast<uint8_t>((byte2 & uint8_t{0x20}) >> 1) & uint8_t{0x10}) +
        static_cast<uint8_t>(((byte2 & uint8_t{0x04}) << 1) & uint8_t{0x08});
    result +=
        static_cast<uint8_t>(static_cast<uint8_t>((byte3 & uint8_t{0x80}) >> 5) & uint8_t{0x04}) +
        static_cast<uint8_t>(static_cast<uint8_t>((byte3 & uint8_t{0x10}) >> 3) & uint8_t{0x02}) +
        static_cast<uint8_t>(static_cast<uint8_t>((byte3 & uint8_t{0x02}) >> 1) & uint8_t{0x01});
    return result;
}

void WS2812LedStrip::setData(uint32_t index, const uint8_t value)
{
    // if we have a value with bits (0b fghijklm), we want to end up with 1f0 1g0 1h0 1i0 1j0 1k0 1l0 1m0 split across 3 bytes:
    // 1f01 g01h (or 10010010 | 0f00g00h)
    // 01i0 1j01 (or 01001001 | 00i00j00)
    // k01l 01m0 (or 00100100 | k00l00m0)
    // Also, sometimes C++ seems to widen the type after a bitwise operator.  So we have to throw in a bunch of static_casts,
    // in addition to uint8_t-ifying literals :-(
    // Isn't bit manipulation fun?!
    this->spiEncodedData[START_OFFSET + index * 3] = 
        std::byte{ static_cast<uint8_t>(
            uint8_t{0b10010010} |
                (
                    static_cast<uint8_t>(static_cast<uint8_t>((value & uint8_t{0x80}) >> 1) & uint8_t{0x40}) +
                    static_cast<uint8_t>(static_cast<uint8_t>((value & uint8_t{0x40}) >> 3) & uint8_t{0x08}) +
                    static_cast<uint8_t>(static_cast<uint8_t>((value & uint8_t{0x20}) >> 5) & uint8_t{0x01})
                )
            ) };
    this->spiEncodedData[START_OFFSET + index * 3 + 1] =
        std::byte{ static_cast<uint8_t>(
            uint8_t{0b01001001} |
                (
                    static_cast<uint8_t>(static_cast<uint8_t>((value & uint8_t{0x10}) << 1) & uint8_t{0x20}) +
                    static_cast<uint8_t>(static_cast<uint8_t>((value & uint8_t{0x08}) >> 1) & uint8_t{0x04})
                )
            ) };
    this->spiEncodedData[START_OFFSET + index * 3 + 2] =
        std::byte{ static_cast<uint8_t>(
            uint8_t{0b00100100} |
                (
                    static_cast<uint8_t>(static_cast<uint8_t>((value & uint8_t{0x04}) << 5) & uint8_t{0x80}) +
                    static_cast<uint8_t>(static_cast<uint8_t>((value & uint8_t{0x02}) << 3) & uint8_t{0x10}) +
                    static_cast<uint8_t>(static_cast<uint8_t>((value & uint8_t{0x01}) << 1) & uint8_t{0x02})
                )
            ) };
}

int WS2812LedStrip::openSpi(std::string spidev)
{
    int spiFd = 0;
    static uint8_t mode = 0;
    static uint8_t bits = 8;
    static uint32_t speed = this->frequencyHz * WS2812LedStrip::SPI_BITS_PER_BIT;

#ifdef IS_LINUX
    spiFd = open(spidev.c_str(), O_RDWR);
    if (spiFd < 0)
    {
        std::cerr << "Couldn't open /dev/spidev0.0: " << spiFd << std::endl;
        return spiFd;
    }

    this->spiFd = spiFd;

    // SPI mode
    int spiModeResult = ioctl(spiFd, SPI_IOC_WR_MODE, &mode);
    if (spiModeResult < 0)
    {
        std::cerr << "Couldn't write mode /dev/spidev0.0: " << spiModeResult << std::endl;
        return spiModeResult;
    }

    spiModeResult = ioctl(spiFd, SPI_IOC_RD_MODE, &mode);
    if (spiModeResult < 0)
    {
        std::cerr << "Couldn't read mode /dev/spidev0.0: " << spiModeResult << std::endl;
        return spiModeResult;
    }

    // // Bits per word
    int spiBitsResult = ioctl(spiFd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (spiBitsResult < 0)
    {
        std::cerr << "Couldn't write bits-per-word /dev/spidev0.0: " << spiBitsResult << std::endl;
        return spiBitsResult;
    }

    // spiBitsResult = ioctl(spiFd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (spiBitsResult < 0)
    {
        std::cerr << "Couldn't read bits-per-word /dev/spidev0.0: " << spiBitsResult << std::endl;
        return spiBitsResult;
    }

    // Max speed Hz
    int spiSpeedResult = ioctl(spiFd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (spiSpeedResult < 0)
    {
        std::cerr << "Couldn't write speed hz /dev/spidev0.0: " << spiSpeedResult << std::endl;
        return spiSpeedResult;
    }

    spiSpeedResult = ioctl(spiFd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (spiSpeedResult < 0)
    {
        std::cerr << "Couldn't read speed hz /dev/spidev0.0: " << spiSpeedResult << std::endl;
        return spiSpeedResult;
    }
#endif

    return 0;
}

int WS2812LedStrip::render()
{
    std::cout << "rendering (" << this->spiEncodedData.size() << " bytes):" << std::endl;
    for (int i = 0; i < this->spiEncodedData.size(); i++)
    {
        std::byte b = this->spiEncodedData[i];
        std::cout << std::hex << std::setw(2) << std::setfill('0')  << static_cast<int>(b);
    }

    std::cout << std::endl;

#ifdef IS_LINUX
    struct spi_ioc_transfer tr;

    memset(&tr, 0, sizeof(struct spi_ioc_transfer));
    tr.delay_usecs = WS2812LedStrip::RESET_TIME;
    tr.tx_buf = reinterpret_cast<unsigned long long>(this->spiEncodedData.data());
    tr.len = this->spiEncodedData.size();
    tr.bits_per_word = uint8_t{8};
    tr.speed_hz = this->frequencyHz * WS2812LedStrip::SPI_BITS_PER_BIT;

    int result = ioctl(this->spiFd, SPI_IOC_MESSAGE(1), &tr);
    if (result < 0)
    {
        std::cerr << "Unable to write message to SPI: " << result << std::endl;
        return result;
    }
#else
#endif

    return 0;
}

void WS2812LedStrip::release()
{
#ifdef IS_LINUX
    close(this->spiFd);
#endif
}
