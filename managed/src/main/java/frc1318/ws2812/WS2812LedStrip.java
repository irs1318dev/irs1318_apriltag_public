package frc1318.ws2812;

import frc1318.AssemblyLoader;
import frc1318.ReleaseableBase;

public class WS2812LedStrip extends ReleaseableBase
{
    static {
        AssemblyLoader.Instance.ensureLoaded();
    }

    private final long nativeObj;

    private WS2812LedStrip(long nativeObj)
    {
        super();

        this.nativeObj = nativeObj;
    }

    public static WS2812LedStrip create(int ledCount)
    {
        long nativeObj = WS2812LedStrip.createStrip(ledCount);
        if (nativeObj == 0)
        {
            return null;
        }

        return new WS2812LedStrip(nativeObj);
    }

    public boolean open()
    {
        int result = WS2812LedStrip.open(this.nativeObj);
        return result == 0;
    }

    public void blank(int startIndex, int count)
    {
        this.checkReleased();

        WS2812LedStrip.blank(this.nativeObj, startIndex, count);
    }

    public void setColor(int index, Color color)
    {
        this.checkReleased();

        WS2812LedStrip.setColor(this.nativeObj, index, color.getRedByte(), color.getGreenByte(), color.getBlueByte());
    }

    public void setColor(int index, byte r, byte g, byte b)
    {
        this.checkReleased();

        WS2812LedStrip.setColor(this.nativeObj, index, r, g, b);
    }

    public void setColor(int startIndex, int count, Color color)
    {
        this.checkReleased();

        WS2812LedStrip.setColor(this.nativeObj, startIndex, count, color.getRedByte(), color.getGreenByte(), color.getBlueByte());
    }

    public void setColor(int startIndex, int count, byte r, byte g, byte b)
    {
        this.checkReleased();

        WS2812LedStrip.setColor(this.nativeObj, startIndex, count, r, g, b);
    }

    public int render()
    {
        this.checkReleased();

        return WS2812LedStrip.render(this.nativeObj);
    }

    @Override
    protected void releaseInternal()
    {
        WS2812LedStrip.release(this.nativeObj);
    }

    private static native long createStrip(int ledCount);
    private static native int open(long nativeObj);
    private static native void release(long nativeObj);

    private static native void blank(long nativeObj, int startIndex, int count);
    private static native void setColor(long nativeObj, int index, byte r, byte g, byte b);
    private static native void setColor(long nativeObj, int startIndex, int count, byte r, byte g, byte b);
    private static native int render(long nativeObj);
}
