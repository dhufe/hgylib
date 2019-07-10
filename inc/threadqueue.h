#ifndef __T_QUEUE_H__
#define __T_QUEUE_H__

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T>
class TQueue {
    public:
        T pop ( void ) {
            std::unique_lock<std::mutex> mlock(_mutex);

            while ( _queue.empty() ) {
                _cond.wait ( mlock );
            }

            auto item = _queue.front();
            _queue.pop();
            return item;
        }

        bool empty ( void ) {
            std::unique_lock<std::mutex> mlock(_mutex);
            return _queue.empty();
        }

        void pop ( T& item ) {
            std::unique_lock<std::mutex> mlock (_mutex);
            while ( _queue.empty() ) {
                _cond.wait ( mlock );
            }
            item = _queue.front();
            _queue.pop();
        }

        void push ( T& item ) {
            std::unique_lock<std::mutex> mlock(_mutex);
            _queue.push(item);
            mlock.unlock();
            _cond.notify_one();
        }

        void push ( T&& item ) {
             std::unique_lock<std::mutex> mlock(_mutex);
            _queue.push( std::move(item));
            mlock.unlock();
            _cond.notify_one();
        }

    private:
        std::queue<T>           _queue;
        std::mutex              _mutex;
        std::condition_variable _cond;
};

#endif // __T_QUEUE_H__
