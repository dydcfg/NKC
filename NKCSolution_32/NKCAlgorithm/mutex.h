#ifndef SM_API_TESTAPPCONSOLE_MUTEX_H
#define SM_API_TESTAPPCONSOLE_MUTEX_H

namespace sm
{
    namespace faceapi
    {
        namespace samplecode
        {
            // A very simple mutex class for sample code purposes. 
            // It is recommended that you use the boost threads library.
            class Mutex
            {
            public:
                Mutex()
                {
                    InitializeCriticalSection(&_cs);
                }
                ~Mutex()
                {
                    DeleteCriticalSection(&_cs);
                }
                void lock() const
                {
                    EnterCriticalSection(&_cs); 
                }
                void unlock() const
                {
                    LeaveCriticalSection(&_cs); 
                }
            private:
                // Noncopyable
                Mutex(const Mutex &);
                Mutex &operator=(const Mutex &);
            private:
                mutable CRITICAL_SECTION _cs;
            };
        }
    }
}
#endif
