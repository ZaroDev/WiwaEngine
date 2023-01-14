using System;

namespace Wiwa
{
    unsafe class URef<T> where T : unmanaged
    {
        private IntPtr m_RefPtr;

        public URef(ref T val){
            fixed (T* ptr = &val)
            {
                m_RefPtr = (IntPtr)ptr;
            }
        }

        public ref T Get()
        {
            return ref *(T*)m_RefPtr;
        }
    }
}
