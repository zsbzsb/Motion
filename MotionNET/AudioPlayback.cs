using System;
using System.Security;
using System.Runtime.InteropServices;
using SFML.System;

namespace MotionNET
{
    public class AudioPlayback : InternalBase
    {
        #region Variables
        private IntPtr _pointer = IntPtr.Zero;
        #endregion

        #region Properties
        public float Volume
        {
            get
            {
                EnsureValid();
                return GetVolume(_pointer);
            }
            set
            {
                EnsureValid();
                SetVolume(_pointer, value);
            }
        }
        public Time OffsetCorrection
        {
            get
            {
                EnsureValid();
                return GetOffsetCorrection(_pointer);
            }
            set
            {
                EnsureValid();
                SetOffsetCorrection(_pointer, value);
            }
        }
        #endregion

        #region Constructors
        public AudioPlayback(DataSource DataSource) : this(DataSource, Time.FromMilliseconds(300)) { }
        public AudioPlayback(DataSource DataSource, Time OffsetCorrection)
        {
            _pointer = CreateAudioPlayback(DataSource.Pointer, OffsetCorrection);
        }
        #endregion

        #region Functions
        protected override void Destroy()
        {
            DestroyAudioPlayback(_pointer);
            base.Destroy();
        }
        #endregion

        #region Imports
        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtAudioPlayback_Create"), SuppressUnmanagedCodeSecurity]
        private static extern IntPtr CreateAudioPlayback(IntPtr DataSourcePointer, Time OffsetCorrection);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtAudioPlayback_Destroy"), SuppressUnmanagedCodeSecurity]
        private static extern void DestroyAudioPlayback(IntPtr Pointer);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtAudioPlayback_GetVolume"), SuppressUnmanagedCodeSecurity]
        private static extern float GetVolume(IntPtr Pointer);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtAudioPlayback_SetVolume"), SuppressUnmanagedCodeSecurity]
        private static extern void SetVolume(IntPtr Pointer, float Volume);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtAudioPlayback_GetOffsetCorrection"), SuppressUnmanagedCodeSecurity]
        private static extern Time GetOffsetCorrection(IntPtr Pointer);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtAudioPlayback_SetOffsetCorrection"), SuppressUnmanagedCodeSecurity]
        private static extern void SetOffsetCorrection(IntPtr Pointer, Time OffsetCorrection);
        #endregion
    }
}
