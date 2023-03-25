using System;
using System.Security;
using System.Runtime.InteropServices;
using SFML.System;

namespace MotionNET
{
    public abstract class AudioPlaybackBase : InternalBase
    {
        private const string ImportPrefix = "mtAudioPlaybackBase_";

        [UnmanagedFunctionPointer(Config.MotionCall)]
        private delegate void SetupStreamCb(uint channelCount, int sampleRate);

        [UnmanagedFunctionPointer(Config.MotionCall)]
        private delegate void SetPlaybackSpeedCb(float playbackSpeed);

        [UnmanagedFunctionPointer(Config.MotionCall)]
        private delegate void ChangeStateCb();

        private IntPtr pointer = IntPtr.Zero;

        private SetupStreamCb setup;
        private SetPlaybackSpeedCb setSpeed;
        private ChangeStateCb start;
        private ChangeStateCb pause;
        private ChangeStateCb stop;

        public Time OffsetCorrection
        {
            get
            {
                EnsureValid();

                return GetOffsetCorrection(pointer);
            }
            set
            {
                EnsureValid();

                SetOffsetCorrection(pointer, value);
            }
        }

        public AudioPlaybackBase(DataSource dataSource) :
            this(dataSource, Time.FromMilliseconds(50)) { }

        public AudioPlaybackBase(DataSource dataSource, Time offsetCorrection)
        {
            setup = SetupStream;
            setSpeed = SetPlaybackSpeed;
            start = StartStream;
            pause = PauseStream;
            stop = StopStream;

            pointer = CreateAudioPlayback(dataSource.Pointer, offsetCorrection, setup, setSpeed, start, pause, stop);
        }

        protected override void Destroy()
        {
            DestroyAudioPlayback(pointer);
            base.Destroy();
        }

        protected abstract void SetupStream(uint channelCount, int sampleRate);

        protected abstract void SetPlaybackSpeed(float playbackSpped);

        protected abstract void StartStream();

        protected abstract void PauseStream();

        protected abstract void StopStream();

        protected bool GetNextBuffer(ref IntPtr samples, ref ulong sampleCount)
        {
            EnsureValid();

            return GetNextBuffer(pointer, ref samples, ref sampleCount);
        }

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "Create"), SuppressUnmanagedCodeSecurity]
        private static extern IntPtr CreateAudioPlayback(IntPtr dataSourcePointer, Time offsetCorrection, SetupStreamCb setupCb, SetPlaybackSpeedCb setSpeedCb, ChangeStateCb startCb, ChangeStateCb pauseCb, ChangeStateCb stopCb);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "Destroy"), SuppressUnmanagedCodeSecurity]
        private static extern void DestroyAudioPlayback(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "GetNextBuffer"), SuppressUnmanagedCodeSecurity]
        private static extern bool GetNextBuffer(IntPtr pointer, ref IntPtr samples, ref ulong sampleCount);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "GetOffsetCorrection"), SuppressUnmanagedCodeSecurity]
        private static extern Time GetOffsetCorrection(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "SetOffsetCorrection"), SuppressUnmanagedCodeSecurity]
        private static extern void SetOffsetCorrection(IntPtr pointer, Time offsetCorrection);
    }
}
