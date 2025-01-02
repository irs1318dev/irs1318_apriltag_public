package frc1318.ws2812;

public class Color
{
    private final int red;
    private final int green;
    private final int blue;

    /**
     * Initializes a new instance of the Color class
     * @param red amount of red in the color (0 to 255 - values are clamped to this range)
     * @param green amount of green in the color (0 to 255 - values are clamped to this range)
     * @param blue amount of blue in the color (0 to 255 - values are clamped to this range)
     */
    public Color(int red, int green, int blue)
    {
        this.red = Math.max(0, Math.min(red, 255));
        this.green = Math.max(0, Math.min(green, 255));
        this.blue = Math.max(0, Math.min(blue, 255));
    }

    /**
     * Gets the amount of red in the color (0 to 255)
     * @return the amount of red in the color
     */
    public int getRed()
    {
        return this.red;
    }

    /**
     * Gets the amount of green in the color (0 to 255)
     * @return the amount of green in the color
     */
    public int getGreen()
    {
        return this.green;
    }

    /**
     * Gets the amount of blue in the color (0 to 255)
     * @return the amount of blue in the color
     */
    public int getBlue()
    {
        return this.blue;
    }

    @Override
    public String toString()
    {
        return String.format("%02x%02x%02x", this.red, this.green, this.blue);
    }

    /**
     * Gets the amount of red in the color as a byte (in java, bytes are signed so -128 to 127, where -1 is "more" than 127)
     * @return the amount of red in the color
     */
    byte getRedByte()
    {
        return (byte)this.red;
    }

    /**
     * Gets the amount of green in the color as a byte (in java, bytes are signed so -128 to 127, where -1 is "more" than 127)
     * @return the amount of green in the color
     */
    byte getGreenByte()
    {
        return (byte)this.green;
    }

    /**
     * Gets the amount of blue in the color as a byte (in java, bytes are signed so -128 to 127, where -1 is "more" than 127)
     * @return the amount of blue in the color
     */
    byte getBlueByte()
    {
        return (byte)this.blue;
    }
}
