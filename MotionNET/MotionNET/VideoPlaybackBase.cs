using System;
using System.Security;
using System.Runtime.InteropServices;

namespace MotionNET
{
    public abstract class VideoPlaybackBase : InternalBase
    {
        private const string ImportPrefix = "mtVideoPlaybackBase_";

        [UnmanagedFunctionPointer(Config.MotionCall)]
        private delegate void CreateTextureCb(int width, int height);

        [UnmanagedFunctionPointer(Config.MotionCall)]
        private delegate void UpdateTextureCb(IntPtr rgbaBuffer);

        [UnmanagedFunctionPointer(Config.MotionCall)]
        private delegate void ClearTextureCb();

        private IntPtr pointer = IntPtr.Zero;

        private CreateTextureCb create;
        private UpdateTextureCb update;
        private ClearTextureCb clear;

        public uint PlayedFrameCount
        {
            get
            {
                EnsureValid();

                return GetPlayedFrameCount(pointer);
            }
        }

        public VideoPlaybackBase(DataSource dataSource)
        {
            create = CreateTexture;
            update = UpdateTexture;
            clear = ClearTexture;

            pointer = CreateVideoPlayback(dataSource.Pointer, create, update, clear);
        }

        protected override void Destroy()
        {
            DestroyVideoPlayback(pointer);
            base.Destroy();
        }

        protected abstract void CreateTexture(int width, int height);

        protected abstract void UpdateTexture(IntPtr rgbaBuffer);

        protected abstract void ClearTexture();

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "Create"), SuppressUnmanagedCodeSecurity]
        private static extern IntPtr CreateVideoPlayback(IntPtr dataSourcePointer, CreateTextureCb createCb, UpdateTextureCb updateCb, ClearTextureCb clearCb);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "Destroy"), SuppressUnmanagedCodeSecurity]
        private static extern void DestroyVideoPlayback(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "GetPlayedFrameCount"), SuppressUnmanagedCodeSecurity]
        private static extern uint GetPlayedFrameCount(IntPtr pointer);
    }
}
