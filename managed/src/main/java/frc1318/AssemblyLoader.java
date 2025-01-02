package frc1318;

import java.util.HashSet;
import java.util.Set;

public class AssemblyLoader
{
    static {
        System.loadLibrary("irs1318_apriltag_native");
    }

    public static AssemblyLoader Instance = new AssemblyLoader();

    private final Set<String> loadedLibraries;
    private AssemblyLoader()
    {
        this.loadedLibraries = new HashSet<String>();
    }

    /**
     * No-op, just doing something to ensure that the static type constructor has been run...
     */
    public void ensureLoaded()
    {
    }

    public void ensureLoaded(String library)
    {
        if (!this.loadedLibraries.contains(library))
        {
            System.loadLibrary(library);
            this.loadedLibraries.add(library);
        }
    }
}
