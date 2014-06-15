using System;
using System.Security;
using System.Runtime.InteropServices;
using SFML.System;

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
        public Vector2i VideoSize
        {
            get
            {
                EnsureValid();
                return GetVideoSize(_pointer);
            }
        }
        public State State
        {
            get
            {
                EnsureValid();
                return GetState(_pointer);
            }
        }
        public Time VideoFrameTime
        {
            get
            {
                EnsureValid();
                return GetVideoFrameTime(_pointer);
            }
        }
        public int AudioChannelCount
        {
            get
            {
                EnsureValid();
                return GetAudioChannelCount(_pointer);
            }
        }
        public int AudioSampleRate
        {
            get
            {
                EnsureValid();
                return GetAudioSampleRate(_pointer);
            }
        }
        public Time FileLength
        {
            get
            {
                EnsureValid();
                return GetFileLength(_pointer);
            }
        }
        public Time PlayingOffset
        {
            get
            {
                EnsureValid();
                return GetPlayingOffset(_pointer);
            }
            set
            {
                EnsureValid();
                SetPlayingOffset(_pointer, value);
            }
        }
        public float PlaybackSpeed
        {
            get
            {
                EnsureValid();
                return GetPlaybackSpeed(_pointer);
            }
            set
            {
                EnsureValid();
                SetPlaybackSpeed(_pointer, value);
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
        public void Update()
        {
            EnsureValid();
            Update(_pointer);
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

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtDataSource_GetVideoSize"), SuppressUnmanagedCodeSecurity]
        private static extern Vector2i GetVideoSize(IntPtr Pointer);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtDataSource_GetState"), SuppressUnmanagedCodeSecurity]
        private static extern State GetState(IntPtr Pointer);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtDataSource_GetVideoFrameTime"), SuppressUnmanagedCodeSecurity]
        private static extern Time GetVideoFrameTime(IntPtr Pointer);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtDataSource_GetAudioChannelCount"), SuppressUnmanagedCodeSecurity]
        private static extern int GetAudioChannelCount(IntPtr Pointer);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtDataSource_GetAudioSampleRate"), SuppressUnmanagedCodeSecurity]
        private static extern int GetAudioSampleRate(IntPtr Pointer);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtDataSource_GetFileLength"), SuppressUnmanagedCodeSecurity]
        private static extern Time GetFileLength(IntPtr Pointer);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtDataSource_GetPlayingOffset"), SuppressUnmanagedCodeSecurity]
        private static extern Time GetPlayingOffset(IntPtr Pointer);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtDataSource_SetPlayingOffset"), SuppressUnmanagedCodeSecurity]
        private static extern void SetPlayingOffset(IntPtr Pointer, Time PlayingOffset);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtDataSource_Update"), SuppressUnmanagedCodeSecurity]
        private static extern void Update(IntPtr Pointer);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtDataSource_GetPlaybackSpeed"), SuppressUnmanagedCodeSecurity]
        private static extern float GetPlaybackSpeed(IntPtr Pointer);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtDataSource_SetPlaybackSpeed"), SuppressUnmanagedCodeSecurity]
        private static extern void SetPlaybackSpeed(IntPtr Pointer, float PlaybackSpeed);
        #endregion
    }
}
