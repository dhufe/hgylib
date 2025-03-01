#ifndef __HGREADER_INTERFACE_H__
#define __HGREADER_INTERFACE_H__

#include <hglib.h>

class HGReaderInterface {
public:
  virtual bool readData(const HGFileInfo **ppfileInfo);
  virtual bool readAll(const HGFileInfo **ppfileInfo);
};

#endif // __HGREADER_INTERFACE_H__
