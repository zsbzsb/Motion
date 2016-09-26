using System;
using System.Security;
using System.Runtime.InteropServices;
using SFML.System;

namespace MotionNET.SFML
{
    public class SFMLAudioPlayback : InternalBase
    {
        #region Constants
        private const string ImportPrefix = "mtSFMLAudioPlayback_";
        #endregion

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

        #region CTOR
        public SFMLAudioPlayback(DataSource DataSource) :
            this(DataSource, Time.FromMilliseconds(50)) { }

        public SFMLAudioPlayback(DataSource DataSource, Time OffsetCorrection)
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
        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "Create"), SuppressUnmanagedCodeSecurity]
        private static extern IntPtr CreateAudioPlayback(IntPtr DataSourcePointer, Time OffsetCorrection);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "Destroy"), SuppressUnmanagedCodeSecurity]
        private static extern void DestroyAudioPlayback(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "GetVolume"), SuppressUnmanagedCodeSecurity]
        private static extern float GetVolume(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "SetVolume"), SuppressUnmanagedCodeSecurity]
        private static extern void SetVolume(IntPtr Pointer, float Volume);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "GetOffsetCorrection"), SuppressUnmanagedCodeSecurity]
        private static extern Time GetOffsetCorrection(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "SetOffsetCorrection"), SuppressUnmanagedCodeSecurity]
        private static extern void SetOffsetCorrection(IntPtr Pointer, Time OffsetCorrection);
        #endregion
    }
}
