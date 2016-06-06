#ifndef NAIVEWAYMATCHSTRINGMAPPING_H
#define NAIVEWAYMATCHSTRINGMAPPING_H

// hoot
#include <hoot/core/algorithms/linearreference/WayString.h>

#include "WayMatchStringMapping.h"

namespace hoot
{

class NaiveWayMatchStringMapping : public WayMatchStringMapping
{
public:
  NaiveWayMatchStringMapping(WayStringPtr str1, WayStringPtr str2);

  virtual WayStringPtr getWayString1() { return _ws1; }

  virtual WayStringPtr getWayString2() { return _ws2; }

  virtual WayLocation map1To2(WayLocation l1);

  virtual WayLocation map2To1(WayLocation l2);

  virtual void setWayString1(const WayStringPtr& ws1) { _ws1 = ws1; }

  virtual void setWayString2(const WayStringPtr& ws2) { _ws2 = ws2; }

private:
  WayStringPtr _ws1, _ws2;
  Meters _length1, _length2;
};

}

#endif // NAIVEWAYMATCHSTRINGMAPPING_H
