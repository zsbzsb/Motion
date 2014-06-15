using System;
using System.Collections.Generic;
using System.Text;

namespace MotionNET
{
    internal class InternalBaseMember : InternalBase
    {
        public event Action DestroyCalled;
        public new void EnsureValid()
        {
            base.EnsureValid();
        }
        protected override void Destroy()
        {
            if (DestroyCalled != null) DestroyCalled();
            base.Destroy();
        }
    }
}
