using System;
using System.Security;
using System.Runtime.InteropServices;
using SFML.System;

namespace MotionNET.SFML
{
    public class SfmlAudioPlayback : InternalBase
    {
        private const string ImportPrefix = "mtSFMLAudioPlayback_";

        private IntPtr pointer = IntPtr.Zero;

        public float Volume
        {
            get
            {
                EnsureValid();

                return GetVolume(pointer);
            }
            set
            {
                EnsureValid();

                SetVolume(pointer, value);
            }
        }

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

        public SfmlAudioPlayback(DataSource dataSource) :
            this(dataSource, Time.FromMilliseconds(50)) { }

        public SfmlAudioPlayback(DataSource dataSource, Time offsetCorrection)
        {
            pointer = CreateAudioPlayback(dataSource.Pointer, offsetCorrection);
        }

        protected override void Destroy()
        {
            DestroyAudioPlayback(pointer);
            base.Destroy();
        }

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "Create"), SuppressUnmanagedCodeSecurity]
        private static extern IntPtr CreateAudioPlayback(IntPtr dataSourcePointer, Time offsetCorrection);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "Destroy"), SuppressUnmanagedCodeSecurity]
        private static extern void DestroyAudioPlayback(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "GetVolume"), SuppressUnmanagedCodeSecurity]
        private static extern float GetVolume(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "SetVolume"), SuppressUnmanagedCodeSecurity]
        private static extern void SetVolume(IntPtr pointer, float volume);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "GetOffsetCorrection"), SuppressUnmanagedCodeSecurity]
        private static extern Time GetOffsetCorrection(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "SetOffsetCorrection"), SuppressUnmanagedCodeSecurity]
        private static extern void SetOffsetCorrection(IntPtr pointer, Time offsetCorrection);
    }
}
