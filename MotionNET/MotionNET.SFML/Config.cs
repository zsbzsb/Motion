using System.Runtime.InteropServices;

namespace MotionNET.SFML
{
    internal static class Config
    {
        #if DEBUG
            internal const string Motion_DLL = "Motion-d";
            internal const CallingConvention Motion_Call = CallingConvention.Cdecl;
        #else
            internal const string Motion_DLL = "Motion";
            internal const CallingConvention Motion_Call = CallingConvention.Cdecl;
        #endif
    }
}
