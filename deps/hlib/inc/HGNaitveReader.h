#ifndef __HGNAIVEREADER_H__
#define __HGNAIVEREADER_H__

#include <HGReaderInterface.h>
#include <hglib.h>

class HGNaiveReader : HGReaderInterface {
public:
  bool readData(const HGFileInfo **ppfileInfo) { return false; }
  bool readAll(const HGFileInfo **ppfileInfo);
};

#endif // __HGNAIVEREADER_H__
