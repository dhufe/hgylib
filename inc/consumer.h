#ifndef __CONSUMER_H__
#define __CONSUMER_H__

#include <threadqueue.h>
#include <threadbase.h>
#include <console_arg.h>

class Consumer : public ThreadBase {
    public:
        explicit Consumer ( ConArgs** , TQueue<std::vector<char>>& );

        void *run ( void );

        void print ( const std::vector<char>& );

    private:
        TQueue<std::vector<char>>&  _queue;
        ConArgs*                    _pConArgs;
};

#endif // __CONSUMER_H__
