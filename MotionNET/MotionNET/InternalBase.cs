using System;

namespace MotionNET
{
    /// <summary>DO NOT USE - for internal use only</summary>
    public abstract class InternalBase : IDisposable
    {
        private bool disposed = false;

        protected bool Disposed
        {
            get
            {
                return disposed;
            }
        }

        ~InternalBase()
        {
            Dispose();
        }

        public void Dispose()
        {
            if (!disposed)
            {
                try
                {
                    Destroy();
                }
                catch (Exception e)
                {
                    #if DEBUG // only rethrow the exception if we are in debug mode
                        throw e;
                    #else
                        Console.WriteLine("Suppresed exception during Dispose() call - run in debug mode to throw the exception.");
                    #endif
                }

                disposed = true;

                GC.SuppressFinalize(this);
            }
        }

        protected virtual void Destroy() { }

        protected void EnsureValid()
        {
            if (disposed)
            {
                throw new Exception("Attempt to access disposed object");
            }
        }
    }
}
