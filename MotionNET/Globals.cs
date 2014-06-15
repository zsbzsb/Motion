using System;

namespace MotionNET
{
    internal static class Globals
    {
        #if DEBUG
            internal const string Motion_DLL = "Motion-d";
        #else
            internal const string Motion_DLL = "Motion";
        #endif
    }
}
