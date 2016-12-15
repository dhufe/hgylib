#ifndef __HLIB_EXCEPTION_H__
#define __HLIB_EXCEPTION_H__

#include <stdexcept>

class HLibException {
    public:
        explicit HLibException( const char* pcMsg = "There's a problem") : pcMessage ( pcMsg ) {
        
        }
    
        const char* what( void ) const {
            return pcMessage;
        }
    private:
        const char* pcMessage;
};


#endif // __HLIB_EXCEPTION_H__