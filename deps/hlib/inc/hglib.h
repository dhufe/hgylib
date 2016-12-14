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

static const char *HGDataTypeStr[] = {
    "HGYVScan",
    "HGYCScan",
    "HGYDScan"
};

struct HGDataType {

    enum eDataType {
        HGYVScan = 0x00,
        HGYCScan = 0x01,
        HGYDScan = 0x02
    };

    HGDataType( eDataType _type )
        : DataType( _type )
        , nBytes( 0 )
        , nDataOffset( 0 )
    {

    }

    std::string toString( void )  const {
        return HGDataTypeStr[ DataType ];
    }

    std::string toString( eDataType _type )  const {
        return HGDataTypeStr[_type];
    }

    ssize_t nBytes;
    ssize_t nDataOffset;
    double  dScaling;

    eDataType  DataType;
};

struct HGFileInfo {
    size_t                  *pnDimension;
    double                  *pdScale;
    double                  *pdStart;
    std::string             *pUnits;
    char	            	*pcUnits;
    ssize_t                 nSamples;
    ssize_t                 nBytes;
    size_t                  nCoordinates;
    ssize_t		            nDataOffset;
    std::vector<HGDataType>* pDataTypes;
};

class HGParser {
    public:
        explicit HGParser   ( const std::string& szFileName );
        void     parseFile  ( HGFileInfo** ppFileInfo );
        void     getData	( char *pcAmplitude, HGFileInfo**);
        virtual ~HGParser   ( void );

    private:
        std::string         szHFileName;

        struct data: std::map <std::string, std::string> {
            public:
                bool    iskey           ( const std::string& s ) const;
                long    getIntValue     ( const std::string& key );
                float   getFloatValue   ( const std::string& key );
                double  getDoubleValue  ( const std::string& key );
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
