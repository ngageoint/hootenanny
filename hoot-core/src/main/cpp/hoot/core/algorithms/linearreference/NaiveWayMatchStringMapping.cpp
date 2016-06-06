#include "NaiveWayMatchStringMapping.h"

namespace hoot
{

NaiveWayMatchStringMapping::NaiveWayMatchStringMapping(WayStringPtr str1,
  WayStringPtr str2) :
  _ws1(str1),
  _ws2(str2)
{
  _length1 = _ws1->calculateLength();
  _length2 = _ws2->calculateLength();
}

WayLocation NaiveWayMatchStringMapping::map1To2(WayLocation l1)
{
  // be certain that the ends match exactly (avoid floating point rounding issues)
  if (_ws1->at(0).getStart() == l1)
  {
    return _ws2->at(0).getStart();
  }
  if (_ws1->back().getEnd() == l1)
  {
    return _ws2->back().getEnd();
  }

  Meters distanceOn1 = _ws1->calculateDistanceOnString(l1);
  double p = distanceOn1 / _length1;
  Meters distanceOn2 = p * _length2;
  return _ws2->calculateLocationFromStart(distanceOn2);
}

WayLocation NaiveWayMatchStringMapping::map2To1(WayLocation l2)
{
  // be certain that the ends match exactly (avoid floating point rounding issues)
  if (_ws2->at(0).getStart() == l2)
  {
    return _ws1->at(0).getStart();
  }
  if (_ws2->back().getEnd() == l2)
  {
    return _ws1->back().getEnd();
  }

  Meters distanceOn2 = _ws2->calculateDistanceOnString(l2);
  double p = distanceOn2 / _length2;
  Meters distanceOn1 = p * _length1;
  return _ws1->calculateLocationFromStart(distanceOn1);
}

}
