package frc1318.libcamera;

public enum ImageFormat
{
    Gray(0);

    int value; // must match ImageFormat value mapping in CameraHolder.cpp
    ImageFormat(int value)
    {
        this.value = value;
    }
}
