using System;
using System.Security;
using System.Runtime.InteropServices;

namespace MotionNET
{
    public class DataSource : InternalBase
    {
        #region Variables
        private IntPtr _pointer = IntPtr.Zero;
        #endregion

        #region Properties
        internal IntPtr Pointer
        {
            get
            {
                EnsureValid();
                return _pointer;
            }
        }
        public bool HasVideo
        {
            get
            {
                EnsureValid();
                return GetHasVideo(_pointer);
            }
        }
        public bool HasAudio
        {
            get
            {
                EnsureValid();
                return GetHasAudio(_pointer);
            }
        }
        #endregion

        #region Constructors
        public DataSource()
        {
            _pointer = CreateDataSource();
        }
        #endregion

        #region Functions
        protected override void Destroy()
        {
            DestroyDataSource(_pointer);
            base.Destroy();
        }
        public bool LoadFromFile(string Filename, bool EnableVideo = true, bool EnableAudio = true)
        {
            EnsureValid();
            return LoadFromFile(_pointer, Filename, EnableVideo, EnableAudio);
        }
        public void Play()
        {
            EnsureValid();
            Play(_pointer);
        }
        public void Pause()
        {
            EnsureValid();
            Pause(_pointer);
        }
        public void Stop()
        {
            EnsureValid();
            Stop(_pointer);
        }
        #endregion

        #region Imports
        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtDataSource_Create"), SuppressUnmanagedCodeSecurity]
        private static extern IntPtr CreateDataSource();

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtDataSource_Destroy"), SuppressUnmanagedCodeSecurity]
        private static extern void DestroyDataSource(IntPtr Pointer);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtDataSource_LoadFromFile"), SuppressUnmanagedCodeSecurity]
        private static extern bool LoadFromFile(IntPtr Pointer, string Filename, bool EnableVideo, bool EnableAudio);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtDataSource_Play"), SuppressUnmanagedCodeSecurity]
        private static extern void Play(IntPtr Pointer);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtDataSource_Pause"), SuppressUnmanagedCodeSecurity]
        private static extern void Pause(IntPtr Pointer);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtDataSource_Stop"), SuppressUnmanagedCodeSecurity]
        private static extern void Stop(IntPtr Pointer);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtDataSource_HasVideo"), SuppressUnmanagedCodeSecurity]
        private static extern bool GetHasVideo(IntPtr Pointer);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtDataSource_HasAudio"), SuppressUnmanagedCodeSecurity]
        private static extern bool GetHasAudio(IntPtr Pointer);
        #endregion
    }
}
