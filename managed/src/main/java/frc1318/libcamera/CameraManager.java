package frc1318.libcamera;

import frc1318.ReleaseableBase;

public class CameraManager extends ReleaseableBase
{
    static {
        System.loadLibrary("irs1318_apriltag_native");
    }

    private final long nativeObj;

    private CameraManager(long nativeObj)
    {
        super();

        this.nativeObj = nativeObj;
    }

    public static CameraManager create()
    {
        return new CameraManager(CameraManager.createCameraManager());
    }

    public Camera getCamera(String id)
    {
        this.checkReleased();

        long nativeCameraObj = CameraManager.getCamera(this.nativeObj, id);
        if (nativeCameraObj == 0)
        {
            return null;
        }

        return new Camera(nativeCameraObj);
    }

    public String[] getCameraIds()
    {
        this.checkReleased();

        return CameraManager.getCameraIds(this.nativeObj);
    }

    @Override
    protected void releaseInternal()
    {
        CameraManager.release(this.nativeObj);
    }

    private static native long createCameraManager();
    private static native String[] getCameraIds(long nativeObj);
    private static native long getCamera(long nativeObj, String id);
    private static native void release(long nativeObj);
}
