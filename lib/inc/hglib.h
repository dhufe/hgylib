#ifndef __HG_LIB_H__
#define __HG_LIB_H__

#include <map>
#include <string>
#include <cstdint>
#include <vector>
#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

struct HGDataType {

    enum eDataType {
        HGYVScan = 0x00,
        HGYCScan = 0x01,
        HGYDScan = 0x02
    };

    HGDataType::HGDataType( eDataType _type ) 
        : DataType( _type )
        , nBytes( 0 )
        , nDataOffset( 0 )
        , nCoordniates( 0 )
    {

    }

    ssize_t nBytes;
    ssize_t nDataOffset;
    size_t  nCoordniates;
    eDataType  DataType;
};

struct HGFileInfo {
    size_t                  *pnDimension;
    double                  *pdScale;
    double                  *pdStart;
    std::string             *pUnits;
    char	            	*pcUnits;
    ssize_t                 nSamples;
    double                  *pdMeasurementScale;
    double		            dAmpScaling;
    size_t                  nCoordinates;
    ssize_t		            nDataOffset;
    std::vector<HGDataType>* pDataTypes;
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
