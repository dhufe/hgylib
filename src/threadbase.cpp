
#include <threadbase.h>

ThreadBase::ThreadBase ( void  )
    : _bRunning (false )  {

}

ThreadBase::~ThreadBase ( void ) {
    _bRunning = false;
    if ( _thread.joinable()  ) {
        _thread.join();
    }
}

bool ThreadBase::join ( void ) {
    if ( _thread.joinable()  ) {
        _thread.join();
        return true;
    }
    return false;
}

void ThreadBase::detach ( void ) {
    if ( _bRunning ) {
        _thread.detach();
    }
}

void ThreadBase::start ( void ) {
    _bRunning = true;
    _thread   = std::thread ( &ThreadBase::run, this );
}
