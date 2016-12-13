#ifndef __HG_LIB_H__
#define __HG_LIB_H__

#include <map>
#include <string>
#include <cstdint>
#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif


enum HGYDataType {
    HGYVolumeScan = 0x00,
    HGYScan       = 0x01
};

struct HGFileInfo {
    size_t      *pnDimension;
    double      *pdScale;
    double      *pdStart;
    std::string *pUnits;
    char		*pcUnits;
    ssize_t     nSamples;
    double      *pdMeasurementScale;
    double		dAmpScaling;
    size_t      nCoordinates;
    ssize_t		nDataOffset;
    HGYDataType eType;
};

class HGParser {
    public:
        explicit HGParser   ( const std::string& szFileName );
        void     parseFile  ( HGFileInfo** ppFileInfo );
        void     getData	( int16_t *piAmplitude, HGFileInfo**);
        virtual ~HGParser   ( void );

    private:
        std::string         szHFileName;

        struct data: std::map <std::string, std::string> {
            public:
                bool    iskey           ( const std::string& s ) const;
                long    getIntValue     ( const std::string& key );
                float   getFloatValue   ( const std::string& key );
                std::string getStringValue(const std::string& key );
        };

        friend std::istream& operator >> ( std::istream& ins, data& d );
        friend std::ostream& operator << ( std::ostream& outs, const data& d );

        data hconfig;
        ssize_t nDataOffset;

    private:
        void parseTextPart ( void );

};

#endif // __HG_LIB_H__
