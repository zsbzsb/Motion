using System;
using System.Security;
using System.Runtime.InteropServices;
using SFML.System;

namespace MotionNET
{
    public class DataSource : InternalBase
    {
        #region Constants
        private const string ImportPrefix = "mtDataSource_";
        #endregion

        #region Variables
        private IntPtr _pointer = IntPtr.Zero;
        private bool _eofEventRaised = false;
        #endregion

        #region Properties
        /// <summary>DO NOT USE - for internal use only</summary>
        public IntPtr Pointer
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

                _eofEventRaised = false;
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

        public bool IsEndofFileReached
        {
            get
            {
                EnsureValid();

                return GetIsEndofFileReached(_pointer);
            }
        }
        #endregion

        #region Events
        public event Action<DataSource> EndofFileReached;
        #endregion

        #region CTOR
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

            _eofEventRaised = false;
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

            _eofEventRaised = false;
        }
        public void Update()
        {
            EnsureValid();

            Update(_pointer);

            if (IsEndofFileReached && !_eofEventRaised)
            {
                _eofEventRaised = true;

                EndofFileReached?.Invoke(this);
            }
        }
        #endregion

        #region Imports
        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "Create"), SuppressUnmanagedCodeSecurity]
        private static extern IntPtr CreateDataSource();

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "Destroy"), SuppressUnmanagedCodeSecurity]
        private static extern void DestroyDataSource(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "LoadFromFile"), SuppressUnmanagedCodeSecurity]
        private static extern bool LoadFromFile(IntPtr Pointer, string Filename, bool EnableVideo, bool EnableAudio);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "Play"), SuppressUnmanagedCodeSecurity]
        private static extern void Play(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "Pause"), SuppressUnmanagedCodeSecurity]
        private static extern void Pause(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "Stop"), SuppressUnmanagedCodeSecurity]
        private static extern void Stop(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "HasVideo"), SuppressUnmanagedCodeSecurity]
        private static extern bool GetHasVideo(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "HasAudio"), SuppressUnmanagedCodeSecurity]
        private static extern bool GetHasAudio(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "GetVideoSize"), SuppressUnmanagedCodeSecurity]
        private static extern Vector2i GetVideoSize(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "GetState"), SuppressUnmanagedCodeSecurity]
        private static extern State GetState(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "GetVideoFrameTime"), SuppressUnmanagedCodeSecurity]
        private static extern Time GetVideoFrameTime(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "GetAudioChannelCount"), SuppressUnmanagedCodeSecurity]
        private static extern int GetAudioChannelCount(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "GetAudioSampleRate"), SuppressUnmanagedCodeSecurity]
        private static extern int GetAudioSampleRate(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "GetFileLength"), SuppressUnmanagedCodeSecurity]
        private static extern Time GetFileLength(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "GetPlayingOffset"), SuppressUnmanagedCodeSecurity]
        private static extern Time GetPlayingOffset(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "SetPlayingOffset"), SuppressUnmanagedCodeSecurity]
        private static extern void SetPlayingOffset(IntPtr Pointer, Time PlayingOffset);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "Update"), SuppressUnmanagedCodeSecurity]
        private static extern void Update(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "GetPlaybackSpeed"), SuppressUnmanagedCodeSecurity]
        private static extern float GetPlaybackSpeed(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "SetPlaybackSpeed"), SuppressUnmanagedCodeSecurity]
        private static extern void SetPlaybackSpeed(IntPtr Pointer, float PlaybackSpeed);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "GetIsEndofFileReached"), SuppressUnmanagedCodeSecurity]
        private static extern bool GetIsEndofFileReached(IntPtr Pointer);
        #endregion
    }
}
