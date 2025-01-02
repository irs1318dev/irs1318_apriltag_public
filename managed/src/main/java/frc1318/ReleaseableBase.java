package frc1318;

public abstract class ReleaseableBase implements IReleasable
{
    private boolean isReleased;

    protected ReleaseableBase()
    {
        this.isReleased = false;
    }

    /**
     * Release/clean up native resources
     */
    @Override
    public void release()
    {
        this.checkReleased();

        this.isReleased = true;
    }

    /**
     * Actually release/clean up native resources (run after it is verified that we have not already released)
     */
    protected abstract void releaseInternal();

    /**
     * Check to ensure that the native resources haven't already been released (throws RuntimeException if so!)
     */
    protected void checkReleased()
    {
        if (this.isReleased)
        {
            throw new RuntimeException("object has already been released!");
        }
    }
}
