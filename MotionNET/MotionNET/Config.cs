using System.Runtime.InteropServices;

namespace MotionNET
{
    internal static class Config
    {
        #if DEBUG
            internal const string MotionDll = "Motion-d";
            internal const CallingConvention MotionCall = CallingConvention.Cdecl;
        #else
            internal const string Motion_DLL = "Motion";
            internal const CallingConvention Motion_Call = CallingConvention.Cdecl;
        #endif
    }
}
