#ifndef WAYMATCHSTRINGMAPPING_H
#define WAYMATCHSTRINGMAPPING_H

#include "WaySublineMatchString.h"

namespace hoot
{

class WayMatchStringMapping
{
public:
  WayMatchStringMapping();
};

typedef boost::shared_ptr<WayMatchStringMapping> WayMatchStringMappingPtr;
typedef boost::shared_ptr<const WayMatchStringMapping> ConstWayMatchStringMappingPtr;

}

#endif // WAYMATCHSTRINGMAPPING_H
