using System;
using System.Security;
using System.Runtime.InteropServices;

namespace MotionNET
{
    public abstract class VideoPlaybackBase : InternalBase
    {
        #region Constants
        private const string ImportPrefix = "mtVideoPlaybackBase_";
        #endregion

        #region Delegates
        [UnmanagedFunctionPointer(Config.Motion_Call)]
        private delegate void CreateTextureCB(int Width, int Height);

        [UnmanagedFunctionPointer(Config.Motion_Call)]
        private delegate void UpdateTextureCB(IntPtr RGBABuffer);

        [UnmanagedFunctionPointer(Config.Motion_Call)]
        private delegate void ClearTextureCB();
        #endregion

        #region Variables
        private IntPtr _pointer = IntPtr.Zero;

        private CreateTextureCB _create;
        private UpdateTextureCB _update;
        private ClearTextureCB _clear;
        #endregion

        #region Properties
        public uint PlayedFrameCount
        {
            get
            {
                EnsureValid();

                return GetPlayedFrameCount(_pointer);
            }
        }
        #endregion

        #region CTOR
        public VideoPlaybackBase(DataSource DataSource)
        {
            _create = CreateTexture;
            _update = UpdateTexture;
            _clear = ClearTexture;

            _pointer = CreateVideoPlayback(DataSource.Pointer, _create, _update, _clear);
        }
        #endregion

        #region Functions
        protected override void Destroy()
        {
            DestroyVideoPlayback(_pointer);
            base.Destroy();
        }

        protected abstract void CreateTexture(int Width, int Height);

        protected abstract void UpdateTexture(IntPtr RGBABuffer);

        protected abstract void ClearTexture();
        #endregion

        #region Imports
        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "Create"), SuppressUnmanagedCodeSecurity]
        private static extern IntPtr CreateVideoPlayback(IntPtr DataSourcePointer, CreateTextureCB CreateCB, UpdateTextureCB UpdateCB, ClearTextureCB ClearCB);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "Destroy"), SuppressUnmanagedCodeSecurity]
        private static extern void DestroyVideoPlayback(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "GetPlayedFrameCount"), SuppressUnmanagedCodeSecurity]
        private static extern uint GetPlayedFrameCount(IntPtr Pointer);
        #endregion
    }
}
