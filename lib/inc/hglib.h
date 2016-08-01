#ifndef __HG_LIB_H__
#define __HG_LIB_H__

#include <map>
#include <string>

struct HGFileInfo {
    size_t      *pnDimension;
    double      *pdScale;
    double      *pdStart;
    std::string *pUnits;
    ssize_t     nSamples;
    double      *pdMeasurementScale;
    size_t      nCoordinates;
};

class HGParser {
    public:
        explicit HGParser   ( const std::string& szFileName );
        void     parseFile  ( HGFileInfo** ppFileInfo );
        void     getData    ( char* pcAmplitude, HGFileInfo** );
        virtual ~HGParser   ( void );
        
    private:
        std::string         szHFileName;
        
        struct data: std::map <std::string, std::string> {
            public:
                bool    iskey           ( const std::string& s ) const;
                int     getIntValue     ( const std::string& key );
                float   getFloatValue   ( const std::string& key );
        };
    
        friend std::istream& operator >> ( std::istream& ins, data& d );
        friend std::ostream& operator << ( std::ostream& outs, const data& d );

        data hconfig;
        ssize_t nDataOffset;
    
    private:
        void parseTextPart ( void );

};

#endif // __HG_LIB_H__