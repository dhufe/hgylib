#include "producer.h"
#include "hglib.h"
#include "hlibexeception.h"
#include <iostream>

#include <chrono>


Producer::Producer ( ConArgs** ppConArg, TQueue<std::vector<char>>& queue )
    : _pConArgs ( *ppConArg )
    , _queue  ( queue )
{

}

void* Producer::run( void ) {
    char* pData = nullptr;
    std::cout << "Producer thread is up and running." << std::endl;
    std::cout << "Input file: " << _pConArgs->szInputFileName << std::endl;

    std::unique_lock<std::mutex> locker(_pConArgs->mutex );

    HGParser hp( _pConArgs->szInputFileName, _pConArgs->export_flag );

    try {
        hp.parseFile(& (_pConArgs->pFile)) ;
        //      std::cout << "Number of coordinates : " << _pConArgs->pFile->nCoordinates << std::endl;
    }
    catch (const HLibException& e) {
        std::cerr << "An error was reported : " << e.what() << std::endl;
        //        return EXIT_FAILURE;
    }

    _pConArgs->condi.notify_one();
    _pConArgs->bReady = true;
    /*
     *  This is the place where the magic happens.
     */


//    std::this_thread::sleep_for(std::chrono::seconds(5) );
    std::cout << "producer thread end of life reached." << std::endl;
    return 0;
}
