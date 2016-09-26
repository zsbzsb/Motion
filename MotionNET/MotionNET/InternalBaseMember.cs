using System;

namespace MotionNET
{
    /// <summary>DO NOT USE - for internal use only</summary>
    public class InternalBaseMember : InternalBase
    {
        #region Events
        public event Action DestroyCalled;
        #endregion

        #region Functions
        public new void EnsureValid()
        {
            base.EnsureValid();
        }

        protected override void Destroy()
        {
            DestroyCalled?.Invoke();
            base.Destroy();
        }
        #endregion
    }
}
