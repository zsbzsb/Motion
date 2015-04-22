using System;
using System.Security;
using System.Runtime.InteropServices;
using SFML.System;
using SFML.Graphics;

namespace MotionNET
{
    public class VideoPlayback : Transformable, Drawable, IDisposable
    {
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

        #region Constructors
        public VideoPlayback(DataSource DataSource) : this(DataSource, Color.Black) { }
        public VideoPlayback(DataSource DataSource, Color BufferColor)
        {
            _pointer = CreateVideoPlayback(DataSource.Pointer, BufferColor);
            _base.DestroyCalled += () => { DestroyVideoPlayback(_pointer); };
        }
        ~VideoPlayback()
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
            RenderStatesMarshal marshaledstates = states.MarshalRenderStates();
            if (target is RenderWindow)
            {
                DrawRenderWindow(_pointer, ((RenderWindow)target).CPointer, ref marshaledstates);
            }
            else if (target is RenderTexture)
            {
                DrawRenderTexture(_pointer, ((RenderTexture)target).CPointer, ref marshaledstates);
            }
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
        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtVideoPlayback_Create"), SuppressUnmanagedCodeSecurity]
        private static extern IntPtr CreateVideoPlayback(IntPtr DataSourcePointer, Color BufferColor);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtVideoPlayback_Destroy"), SuppressUnmanagedCodeSecurity]
        private static extern void DestroyVideoPlayback(IntPtr Pointer);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtVideoPlayback_GetBufferColor"), SuppressUnmanagedCodeSecurity]
        private static extern Color GetBufferColor(IntPtr Pointer);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtVideoPlayback_SetBufferColor"), SuppressUnmanagedCodeSecurity]
        private static extern void SetBufferColor(IntPtr Pointer, Color BufferColor);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtVideoPlayback_DrawRenderWindow"), SuppressUnmanagedCodeSecurity]
        private static extern void DrawRenderWindow(IntPtr Pointer, IntPtr RenderWindowPointer, ref RenderStatesMarshal RenderStates);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtVideoPlayback_DrawRenderTexture"), SuppressUnmanagedCodeSecurity]
        private static extern void DrawRenderTexture(IntPtr Pointer, IntPtr RenderTexturePointer, ref RenderStatesMarshal RenderStates);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtVideoPlayback_GetLastFrame"), SuppressUnmanagedCodeSecurity]
        private static extern void GetLastFrame(IntPtr Pointer, IntPtr ImagePointer);

        [DllImport(Globals.Motion_DLL, CallingConvention = CallingConvention.Cdecl, EntryPoint = "mtVideoPlayback_GetPlayedFrameCount"), SuppressUnmanagedCodeSecurity]
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
            RenderStatesMarshal marshal = new RenderStatesMarshal();
            marshal.BlendMode = States.BlendMode;
            marshal.Transform = States.Transform;
            marshal.Texture = States.Texture != null ? States.Texture.CPointer : IntPtr.Zero;
            marshal.Shader = States.Shader != null ? States.Shader.CPointer : IntPtr.Zero;
            return marshal;
        }
    }
    #endregion
}
