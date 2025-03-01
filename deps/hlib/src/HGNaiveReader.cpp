#include "hglib.h"
#include <HGNaitveReader.h>
#include <iostream>

bool HGNaiveReader::readAll(const HGFileInfo **ppfileInfo) {
  std::cout << (*ppfileInfo)->nBytes << std::endl;
  return true;
}
