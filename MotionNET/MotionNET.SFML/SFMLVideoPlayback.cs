using System;
using System.Security;
using System.Runtime.InteropServices;
using SFML.Graphics;

namespace MotionNET.SFML
{
    public class SFMLVideoPlayback : Transformable, Drawable, IDisposable
    {
        #region Constants
        private const string ImportPrefix = "mtSFMLVideoPlayback_";
        #endregion

        #region Variables
        private IntPtr _pointer = IntPtr.Zero;
        private InternalBaseMember _base = new InternalBaseMember();
        #endregion

        #region Properties
        public Color BufferColor
        {
            get
            {
                _base.EnsureValid();

                return GetBufferColor(_pointer);
            }
            set
            {
                _base.EnsureValid();

                SetBufferColor(_pointer, value);
            }
        }

        public uint PlayedFrameCount
        {
            get
            {
                _base.EnsureValid();

                return GetPlayedFrameCount(_pointer);
            }
        }
        #endregion

        #region CTOR
        public SFMLVideoPlayback(DataSource DataSource) :
            this(DataSource, Color.Black) { }

        public SFMLVideoPlayback(DataSource DataSource, Color BufferColor)
        {
            _pointer = CreateVideoPlayback(DataSource.Pointer, BufferColor);

            _base.DestroyCalled += () => { DestroyVideoPlayback(_pointer); };
        }
        #endregion

        #region DTOR
        ~SFMLVideoPlayback()
        {
            _base.Dispose();
        }
        #endregion

        #region Functions
        public new void Dispose()
        {
            base.Dispose();
            _base.Dispose();

            GC.SuppressFinalize(this);
        }
        public void Draw(RenderTarget target, RenderStates states)
        {
            _base.EnsureValid();

            states.Transform *= Transform;
            var marshaledstates = states.MarshalRenderStates();

            if (target is RenderWindow)
                DrawRenderWindow(_pointer, ((RenderWindow)target).CPointer, ref marshaledstates);
            else if (target is RenderTexture)
                DrawRenderTexture(_pointer, ((RenderTexture)target).CPointer, ref marshaledstates);
        }

        public Image GetLastFrame()
        {
            _base.EnsureValid();

            var image = new Image(0, 0);

            GetLastFrame(_pointer, image.CPointer);

            return image;
        }
        #endregion

        #region Imports
        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "Create"), SuppressUnmanagedCodeSecurity]
        private static extern IntPtr CreateVideoPlayback(IntPtr DataSourcePointer, Color BufferColor);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "Destroy"), SuppressUnmanagedCodeSecurity]
        private static extern void DestroyVideoPlayback(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "GetBufferColor"), SuppressUnmanagedCodeSecurity]
        private static extern Color GetBufferColor(IntPtr Pointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "SetBufferColor"), SuppressUnmanagedCodeSecurity]
        private static extern void SetBufferColor(IntPtr Pointer, Color BufferColor);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "DrawRenderWindow"), SuppressUnmanagedCodeSecurity]
        private static extern void DrawRenderWindow(IntPtr Pointer, IntPtr RenderWindowPointer, ref RenderStatesMarshal RenderStates);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "DrawRenderTexture"), SuppressUnmanagedCodeSecurity]
        private static extern void DrawRenderTexture(IntPtr Pointer, IntPtr RenderTexturePointer, ref RenderStatesMarshal RenderStates);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "GetLastFrame"), SuppressUnmanagedCodeSecurity]
        private static extern void GetLastFrame(IntPtr Pointer, IntPtr ImagePointer);

        [DllImport(Config.Motion_DLL, CallingConvention = Config.Motion_Call, EntryPoint = ImportPrefix + "GetPlayedFrameCount"), SuppressUnmanagedCodeSecurity]
        private static extern uint GetPlayedFrameCount(IntPtr Pointer);
        #endregion
    }

    #region Internal Structures
    [StructLayout(LayoutKind.Sequential)]
    internal struct RenderStatesMarshal
    {
        public BlendMode BlendMode;
        public Transform Transform;
        public IntPtr Texture;
        public IntPtr Shader;
    }

    internal static class MarshalExtentions
    {
        public static RenderStatesMarshal MarshalRenderStates(this RenderStates States)
        {
            var marshal = new RenderStatesMarshal();

            marshal.BlendMode = States.BlendMode;
            marshal.Transform = States.Transform;
            marshal.Texture = States.Texture?.CPointer ?? IntPtr.Zero;
            marshal.Shader = States.Shader?.CPointer ?? IntPtr.Zero;

            return marshal;
        }
    }
    #endregion
}
