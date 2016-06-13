#ifndef WAYMATCHSTRINGMAPPINGCONVERTER_H
#define WAYMATCHSTRINGMAPPINGCONVERTER_H

#include "WayMatchStringMapping.h"
#include "WaySublineMatchString.h"

namespace hoot
{

/**
 * Provides a conversion from a WayMatchStringMapping to a WaySublineMatchString
 */
class WayMatchStringMappingConverter
{
public:
  WayMatchStringMappingConverter();

  WaySublineMatchStringPtr toWaySublineMatchString(WayMatchStringMappingPtr mapping);
};

}

#endif // WAYMATCHSTRINGMAPPINGCONVERTER_H
