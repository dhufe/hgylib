#ifndef __PRODUCER_H__
#define __PRODUCER_H__

#include "threadqueue.h"
#include "threadbase.h"
#include "console_arg.h"

class Producer : public ThreadBase {
    public:
        explicit    Producer ( ConArgs**, TQueue<std::vector<char>>& );

        void* run ( void );


    private:
        TQueue<std::vector<char>>&  _queue;
        ConArgs*                    _pConArgs;
};

#endif // __PRODUCER_H__
