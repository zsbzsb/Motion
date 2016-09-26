using System;
using System.Security;
using System.Runtime.InteropServices;
using SFML.System;

namespace MotionNET
{
    public abstract class AudioPlaybackBase : InternalBase
    {
        #region Constants
        private const string ImportPrefix = "mtAudioPlaybackBase_";
        #endregion

        #region Delegates
        [UnmanagedFunctionPointer(Config.Motion_Call)]
        private delegate void SetupStreamCB(uint ChannelCount, int SampleRate);

        [UnmanagedFunctionPointer(Config.Motion_Call)]
        private delegate void SetPlaybackSpeedCB(float PlaybackSpeed);

        [UnmanagedFunctionPointer(Config.Motion_Call)]
        private delegate void ChangeStateCB();
        #endregion

        #region Variables
        private IntPtr _pointer = IntPtr.Zero;

        private SetupStreamCB _setup;
        private SetPlaybackSpeedCB _setSpeed;
        private ChangeStateCB _start;
        private ChangeStateCB _pause;
        private ChangeStateCB _stop;
        #endregion

        #region Properties
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

        #region CTOR
        public AudioPlaybackBase(DataSource DataSource) :
            this(DataSource, Time.FromMilliseconds(50)) { }

        public AudioPlaybackBase(DataSource DataSource, Time OffsetCorrection)
        {
            _setup = SetupStream;
            _setSpeed = SetPlaybackSpeed;
            _start = StartStream;
            _pause = PauseStream;
            _stop = StopStream;

            _pointer = CreateAudioPlayback(DataSource.Pointer, OffsetCorrection, _setup, _setSpeed, _start, _pause, _stop);
        }
        #endregion

        #region Functions
        protected override void Destroy()
        {
            DestroyAudioPlayback(_pointer);
            base.Destroy();
        }

        protected abstract void SetupStream(uint ChannelCount, int SampleRate);

        protected abstract void SetPlaybackSpeed(float PlaybackSpped);

        protected abstract void StartStream();

        protected abstract void PauseStream();

        protected abstract void StopStream();

        protected bool GetNextBuffer(ref IntPtr Samples, ref ulong SampleCount)
        {
            EnsureValid();

            return GetNextBuffer(_pointer, ref Samples, ref SampleCount);
        }
        #endregion

        #region Imports
        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "Create"), SuppressUnmanagedCodeSecurity]
        private static extern IntPtr CreateAudioPlayback(IntPtr DataSourcePointer, Time OffsetCorrection, SetupStreamCB SetupCB, SetPlaybackSpeedCB SetSpeedCB, ChangeStateCB StartCB, ChangeStateCB PauseCB, ChangeStateCB StopCB);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "Destroy"), SuppressUnmanagedCodeSecurity]
        private static extern void DestroyAudioPlayback(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "GetNextBuffer"), SuppressUnmanagedCodeSecurity]
        private static extern bool GetNextBuffer(IntPtr Pointer, ref IntPtr Samples, ref ulong SampleCount);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "GetOffsetCorrection"), SuppressUnmanagedCodeSecurity]
        private static extern Time GetOffsetCorrection(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "SetOffsetCorrection"), SuppressUnmanagedCodeSecurity]
        private static extern void SetOffsetCorrection(IntPtr Pointer, Time OffsetCorrection);
        #endregion
    }
}
