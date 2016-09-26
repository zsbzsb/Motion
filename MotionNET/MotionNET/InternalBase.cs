using System;

namespace MotionNET
{
    /// <summary>DO NOT USE - for internal use only</summary>
    public abstract class InternalBase : IDisposable
    {
        #region Variables
        private bool _disposed = false;
        #endregion

        #region Properties
        protected bool Disposed
        {
            get
            {
                return _disposed;
            }
        }
        #endregion

        #region CTOR
        ~InternalBase()
        {
            Dispose();
        }
        #endregion

        #region Functions
        public void Dispose()
        {
            if (!_disposed)
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

                _disposed = true;

                GC.SuppressFinalize(this);
            }
        }

        protected virtual void Destroy() { }

        protected void EnsureValid()
        {
            if (_disposed)
                throw new Exception("Attempt to access disposed object");
        }
        #endregion
    }
}
