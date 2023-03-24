using System;
using System.Security;
using System.Runtime.InteropServices;
using SFML.System;

namespace MotionNET
{
    public class DataSource : InternalBase
    {
        private const string ImportPrefix = "mtDataSource_";

        private IntPtr pointer = IntPtr.Zero;
        private bool eofEventRaised = false;

        /// <summary>DO NOT USE - for internal use only</summary>
        public IntPtr Pointer
        {
            get
            {
                EnsureValid();

                return pointer;
            }
        }

        public bool HasVideo
        {
            get
            {
                EnsureValid();

                return GetHasVideo(pointer);
            }
        }

        public bool HasAudio
        {
            get
            {
                EnsureValid();

                return GetHasAudio(pointer);
            }
        }

        public Vector2i VideoSize
        {
            get
            {
                EnsureValid();

                return GetVideoSize(pointer);
            }
        }

        public State State
        {
            get
            {
                EnsureValid();

                return GetState(pointer);
            }
        }

        public Time VideoFrameTime
        {
            get
            {
                EnsureValid();

                return GetVideoFrameTime(pointer);
            }
        }

        public int AudioChannelCount
        {
            get
            {
                EnsureValid();

                return GetAudioChannelCount(pointer);
            }
        }

        public int AudioSampleRate
        {
            get
            {
                EnsureValid();

                return GetAudioSampleRate(pointer);
            }
        }

        public Time FileLength
        {
            get
            {
                EnsureValid();

                return GetFileLength(pointer);
            }
        }

        public Time PlayingOffset
        {
            get
            {
                EnsureValid();

                return GetPlayingOffset(pointer);
            }
            set
            {
                EnsureValid();

                SetPlayingOffset(pointer, value);

                eofEventRaised = false;
            }
        }

        public float PlaybackSpeed
        {
            get
            {
                EnsureValid();

                return GetPlaybackSpeed(pointer);
            }
            set
            {
                EnsureValid();

                SetPlaybackSpeed(pointer, value);
            }
        }

        public bool IsEndofFileReached
        {
            get
            {
                EnsureValid();

                return GetIsEndofFileReached(pointer);
            }
        }

        public event Action<DataSource> EndofFileReached;

        public DataSource()
        {
            pointer = CreateDataSource();
        }

        protected override void Destroy()
        {
            DestroyDataSource(pointer);

            base.Destroy();
        }

        public bool LoadFromFile(string filename, bool enableVideo = true, bool enableAudio = true)
        {
            EnsureValid();

            return LoadFromFile(pointer, filename, enableVideo, enableAudio);
        }
        public void Play()
        {
            EnsureValid();

            Play(pointer);

            eofEventRaised = false;
        }
        public void Pause()
        {
            EnsureValid();

            Pause(pointer);
        }
        public void Stop()
        {
            EnsureValid();

            Stop(pointer);

            eofEventRaised = false;
        }
        public void Update()
        {
            EnsureValid();

            Update(pointer);

            if (IsEndofFileReached && !eofEventRaised)
            {
                eofEventRaised = true;

                EndofFileReached?.Invoke(this);
            }
        }

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "Create"), SuppressUnmanagedCodeSecurity]
        private static extern IntPtr CreateDataSource();

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "Destroy"), SuppressUnmanagedCodeSecurity]
        private static extern void DestroyDataSource(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "LoadFromFile"), SuppressUnmanagedCodeSecurity]
        private static extern bool LoadFromFile(IntPtr pointer, string filename, bool enableVideo, bool enableAudio);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "Play"), SuppressUnmanagedCodeSecurity]
        private static extern void Play(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "Pause"), SuppressUnmanagedCodeSecurity]
        private static extern void Pause(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "Stop"), SuppressUnmanagedCodeSecurity]
        private static extern void Stop(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "HasVideo"), SuppressUnmanagedCodeSecurity]
        private static extern bool GetHasVideo(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "HasAudio"), SuppressUnmanagedCodeSecurity]
        private static extern bool GetHasAudio(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "GetVideoSize"), SuppressUnmanagedCodeSecurity]
        private static extern Vector2i GetVideoSize(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "GetState"), SuppressUnmanagedCodeSecurity]
        private static extern State GetState(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "GetVideoFrameTime"), SuppressUnmanagedCodeSecurity]
        private static extern Time GetVideoFrameTime(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "GetAudioChannelCount"), SuppressUnmanagedCodeSecurity]
        private static extern int GetAudioChannelCount(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "GetAudioSampleRate"), SuppressUnmanagedCodeSecurity]
        private static extern int GetAudioSampleRate(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "GetFileLength"), SuppressUnmanagedCodeSecurity]
        private static extern Time GetFileLength(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "GetPlayingOffset"), SuppressUnmanagedCodeSecurity]
        private static extern Time GetPlayingOffset(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "SetPlayingOffset"), SuppressUnmanagedCodeSecurity]
        private static extern void SetPlayingOffset(IntPtr pointer, Time playingOffset);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "Update"), SuppressUnmanagedCodeSecurity]
        private static extern void Update(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "GetPlaybackSpeed"), SuppressUnmanagedCodeSecurity]
        private static extern float GetPlaybackSpeed(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "SetPlaybackSpeed"), SuppressUnmanagedCodeSecurity]
        private static extern void SetPlaybackSpeed(IntPtr pointer, float playbackSpeed);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "GetIsEndofFileReached"), SuppressUnmanagedCodeSecurity]
        private static extern bool GetIsEndofFileReached(IntPtr pointer);
    }
}
