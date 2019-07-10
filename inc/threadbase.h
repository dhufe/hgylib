#ifndef __THREADBASE_H__
#define __THREADBASE_H__

#include <thread>

class ThreadBase {
    public:
        explicit ThreadBase ( void );
        virtual ~ThreadBase ( void );

        void start  ( void );
        void detach ( void );
        bool join   ( void );

        virtual void* run ( void ) = 0;

    private:
        std::thread     _thread;
        bool            _bRunning;
};

#endif // __THREADBASE_H__
