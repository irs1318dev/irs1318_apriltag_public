#include <jni.h>
#include <iostream>
#include <cstdint>
#include <iomanip>

#ifdef IS_LINUX
#include <fcntl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "../piolib/include/piolib.h"
#endif

#include "WS2812LedStrip.hpp"

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
    return static_cast<jint>(ledStrip->openPio());
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
    uint32_t red =   static_cast<uint8_t>(this->pioData[START_OFFSET + index + WS2812LedStrip::RED_OFFSET]);
    uint32_t green = static_cast<uint8_t>(this->pioData[START_OFFSET + index + WS2812LedStrip::GREEN_OFFSET]);
    uint32_t blue =  static_cast<uint8_t>(this->pioData[START_OFFSET + index + WS2812LedStrip::BLUE_OFFSET]);
    return Color(red, green, blue);
}

void WS2812LedStrip::blank(uint32_t startIndex, uint32_t count)
{
    for (uint32_t i = startIndex; i < startIndex + count; i++)
    {
        int colorOffset = START_OFFSET + i * WS2812LedStrip::BYTES_PER_PIXEL;
        this->pioData[colorOffset + 0] = std::byte{0U};
        this->pioData[colorOffset + 1] = std::byte{0U};
        this->pioData[colorOffset + 2] = std::byte{0U};
        this->pioData[colorOffset + 3] = std::byte{0U};
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
    for (uint32_t i = startIndex; i < startIndex + count; i++)
    {
        this->setColor(i, r, g, b);
    }
}

void WS2812LedStrip::setColor(uint32_t index, const uint8_t r, const uint8_t g, const uint8_t b)
{
    uint32_t colorOffset = START_OFFSET + index * WS2812LedStrip::BYTES_PER_PIXEL;
    this->pioData[colorOffset + WS2812LedStrip::RED_OFFSET] = static_cast<std::byte>(r);
    this->pioData[colorOffset + WS2812LedStrip::GREEN_OFFSET] = static_cast<std::byte>(g);
    this->pioData[colorOffset + WS2812LedStrip::BLUE_OFFSET] = static_cast<std::byte>(b);
}

int WS2812LedStrip::openPio()
{
// restructured from https://github.com/raspberrypi/utils/blob/0c02957911da1c661be201236c4f2a2be1778b3e/piolib/examples/ws2812.pio.h and piotest.c:
#ifdef IS_LINUX
    this->pio = pio_open_helper(0);
    this->sm = pio_claim_unused_sm(this->pio, true);

    pio_sm_config_xfer(this->pio, this->sm, PIO_DIR_TO_SM, 256, 1);
    uint offset = pio_add_program(this->pio, &ws2812_program);

    pio_sm_clear_fifos(this->pio, this->sm);
    pio_sm_set_clkdiv(this->pio, this->sm, 1.0);

    pio_gpio_init(this->pio, this->gpioPin);
    pio_sm_set_consecutive_pindirs(this->pio, this->sm, this->gpioPin, 1, true);

    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + 0 /*ws2812_wrap_target*/, offset + 3 /*ws2812_wrap*/);
    sm_config_set_sideset(&c, 1, false, false);

    sm_config_set_sideset_pins(&c, this->gpioPin);
    sm_config_set_out_shift(&c, false, true, 3 * 8);
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);

    int cycles_per_bit = 3 /*ws2812_T1*/ + 4 /*ws2812_T2*/ +  3 /*ws2812_T3*/;
    float div = clock_get_hz(clk_sys) / (this->frequencyHz * cycles_per_bit);
    sm_config_set_clkdiv(&c, div);
    pio_sm_init(this->pio, this->sm, offset, &c);
    pio_sm_set_enabled(this->pio, this->sm, true);
#endif

    return 0;
}

int WS2812LedStrip::render()
{
//    std::cout << "rendering (" << this->pioData.size() << " bytes):" << std::endl;
//    for (int i = 0; i < this->pioData.size(); i++)
//    {
//        std::byte b = this->pioData[i];
//        std::cout << std::hex << std::setw(2) << std::setfill('0')  << static_cast<int>(b);
//    }
//
//    std::cout << std::endl;

#ifdef IS_LINUX
    return pio_sm_xfer_data(this->pio, this->sm, PIO_DIR_TO_SM, this->pioData.size(), reinterpret_cast<void*>(this->pioData.data()));
#else
    return 0;
#endif
}

void WS2812LedStrip::release()
{
#ifdef IS_LINUX
    // nothing?
#endif
}
