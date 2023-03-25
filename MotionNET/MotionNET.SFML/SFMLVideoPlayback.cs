using System;
using System.Security;
using System.Runtime.InteropServices;
using SFML.Graphics;

namespace MotionNET.SFML
{
    public class SfmlVideoPlayback : Transformable, Drawable, IDisposable
    {
        private const string ImportPrefix = "mtSFMLVideoPlayback_";

        private IntPtr pointer = IntPtr.Zero;
        private InternalBaseMember @base = new InternalBaseMember();

        public Color BufferColor
        {
            get
            {
                @base.EnsureValid();

                return GetBufferColor(pointer);
            }
            set
            {
                @base.EnsureValid();

                SetBufferColor(pointer, value);
            }
        }

        public uint PlayedFrameCount
        {
            get
            {
                @base.EnsureValid();

                return GetPlayedFrameCount(pointer);
            }
        }

        public SfmlVideoPlayback(DataSource dataSource) : this(dataSource, Color.Black)
        {
            
        }

        public SfmlVideoPlayback(DataSource dataSource, Color bufferColor)
        {
            pointer = CreateVideoPlayback(dataSource.Pointer, bufferColor);

            @base.DestroyCalled += () => { DestroyVideoPlayback(pointer); };
        }

        ~SfmlVideoPlayback()
        {
            @base.Dispose();
        }

        public new void Dispose()
        {
            base.Dispose();
            @base.Dispose();

            GC.SuppressFinalize(this);
        }
        public void Draw(RenderTarget target, RenderStates states)
        {
            @base.EnsureValid();

            states.Transform *= Transform;
            var marshaledstates = states.MarshalRenderStates();

            if (target is RenderWindow)
                DrawRenderWindow(pointer, ((RenderWindow)target).CPointer, ref marshaledstates);
            else if (target is RenderTexture)
                DrawRenderTexture(pointer, ((RenderTexture)target).CPointer, ref marshaledstates);
        }

        public Image GetLastFrame()
        {
            @base.EnsureValid();

            var image = new Image(0, 0);

            GetLastFrame(pointer, image.CPointer);

            return image;
        }

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "Create"), SuppressUnmanagedCodeSecurity]
        private static extern IntPtr CreateVideoPlayback(IntPtr dataSourcePointer, Color bufferColor);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "Destroy"), SuppressUnmanagedCodeSecurity]
        private static extern void DestroyVideoPlayback(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "GetBufferColor"), SuppressUnmanagedCodeSecurity]
        private static extern Color GetBufferColor(IntPtr pointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "SetBufferColor"), SuppressUnmanagedCodeSecurity]
        private static extern void SetBufferColor(IntPtr pointer, Color bufferColor);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "DrawRenderWindow"), SuppressUnmanagedCodeSecurity]
        private static extern void DrawRenderWindow(IntPtr pointer, IntPtr renderWindowPointer, ref RenderStatesMarshal renderStates);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "DrawRenderTexture"), SuppressUnmanagedCodeSecurity]
        private static extern void DrawRenderTexture(IntPtr pointer, IntPtr renderTexturePointer, ref RenderStatesMarshal renderStates);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "GetLastFrame"), SuppressUnmanagedCodeSecurity]
        private static extern void GetLastFrame(IntPtr pointer, IntPtr imagePointer);

        [DllImport(Config.MotionDll, CallingConvention = Config.MotionCall, EntryPoint = ImportPrefix + "GetPlayedFrameCount"), SuppressUnmanagedCodeSecurity]
        private static extern uint GetPlayedFrameCount(IntPtr pointer);
    }

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
        public static RenderStatesMarshal MarshalRenderStates(this RenderStates states)
        {
            var marshal = new RenderStatesMarshal();

            marshal.BlendMode = states.BlendMode;
            marshal.Transform = states.Transform;
            marshal.Texture = states.Texture?.CPointer ?? IntPtr.Zero;
            marshal.Shader = states.Shader?.CPointer ?? IntPtr.Zero;

            return marshal;
        }
    }
}
