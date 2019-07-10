#include <consumer.h>
#include <iostream>

Consumer::Consumer ( ConArgs** ppConArg, TQueue<std::vector<char>>& queue )
    : _pConArgs ( *ppConArg )
      , _queue     ( queue )
{

}

void* Consumer::run ( void ) {
    char* pData = nullptr;
    std::cout << "Consumer thread is up and running." << std::endl;
    std::cout << "Output file: " << this->_pConArgs->szOutputFileName << std::endl;

    std::unique_lock<std::mutex> lk(_pConArgs->mutex);

    while (! _pConArgs->bReady ) {
        _pConArgs->condi.wait( lk );
    }


    //
    // This is the place where the magic happens.
    //

    if (_pConArgs->verbose_flag) {

        std::cout << "Dimensions : ";
        for (auto i = 0; i < _pConArgs->pFile->nCoordinates; i++) {
            if (i < _pConArgs->pFile->nCoordinates - 1)
                std::cout << _pConArgs->pFile->pnDimension[i] << " x ";
            else
                std::cout << _pConArgs->pFile->pnDimension[i] << std::endl;
        }

        std::cout << "Start : ";
        for (auto i = 0; i < _pConArgs->pFile->nCoordinates; i++) {
            if (i < _pConArgs->pFile->nCoordinates - 1)
                std::cout << _pConArgs->pFile->pdStart[i] << " x ";
            else
                std::cout << _pConArgs->pFile->pdStart[i] << std::endl;
        }

        std::cout << "Scaling : ";
        for (auto i = 0; i < _pConArgs->pFile->nCoordinates; i++) {
            if (i < _pConArgs->pFile->nCoordinates - 1)
                std::cout << _pConArgs->pFile->pdScale[i] << " x ";
            else
                std::cout << _pConArgs->pFile->pdScale[i] << std::endl;
        }


        if (_pConArgs->pFile->pDataTypes) {
            std::cout << std::endl << _pConArgs->pFile->pDataTypes->size() << " different types of measurement data." << std::endl;
            int i = 0;
            for (std::vector<HGDataType>::iterator pIter = _pConArgs->pFile->pDataTypes->begin(); pIter != _pConArgs->pFile->pDataTypes->end(); ++pIter) {
                i++;
                std::cout << "data set #" << i << " Datatype : " << (*pIter).toString() << " DataOffset : " << (*pIter).nDataOffset << " Size : " << (*pIter).nBytes << " bytes" << std::endl;
            }
        }

        std::cout << "total size of data region : " << _pConArgs->pFile->nBytes << " Bytes" << std::endl;
    }
    std::cout << "Consumer thread end of life reached." << std::endl;

    return 0;
}

void print ( const std::vector<char>& inp ) {
    static std::mutex _mutex;
    std::unique_lock<std::mutex> locker(_mutex);
    std::cout << inp.size() * 8 << "bytes written." << std::endl;
}
