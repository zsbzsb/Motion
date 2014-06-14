#if DEBUG
#define ENSURE_VALID_OBJECT
#endif
#define EXCEPTIONS_TO_CONSOLE

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

        #region Constructors/Destructors
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
                    #elif EXCEPTIONS_TO_CONSOLE // else notify in console
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
            #if ENSURE_VALID_OBJECT
                if (_disposed) throw new Exception("Attempt to access disposed object");
            #endif
        }
        #endregion
    }
}
