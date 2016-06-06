#ifndef WAYMATCHSTRINGMAPPING_H
#define WAYMATCHSTRINGMAPPING_H

#include "WayString.h"
#include "WaySublineMatchString.h"

namespace hoot
{

/**
 * Maintains a mapping from one waystring to another. This allows the caller to get a corresponding
 * WayLocation on WayString1 for a WayLocation on WayString2, or vice versa.
 *
 * This interface guarantees:
 *
 * - The beginning of WayString1 will map to the beginning of WayString2
 * - The end of WayString1 will map to the end of WayString2
 * - The mapping is not necessarily commutative. For instance, you could have multiple points on
 *   1 map to a single point on 2 so that single point on 2 may map to any of the corresponding
 *   points on 1.
 * - The results will be consistent for the same input.
 *
 * If WayStringPtr or any of its children change then the operation of this class is undefined. If
 * You do change or move a WayStringPtr or its children please call setWayString* accordingly to
 * update any underlying data structures.
 */
class WayMatchStringMapping
{
public:
  WayMatchStringMapping();

  virtual WayStringPtr getWayString1() = 0;

  virtual WayStringPtr getWayString2() = 0;

  virtual WayLocation map1To2(WayLocation l1) = 0;

  virtual WayLocation map2To1(WayLocation l2) = 0;

  virtual void setWayString1(const WayStringPtr& ws1) = 0;

  virtual void setWayString2(const WayStringPtr& ws2) = 0;

};

typedef boost::shared_ptr<WayMatchStringMapping> WayMatchStringMappingPtr;
typedef boost::shared_ptr<const WayMatchStringMapping> ConstWayMatchStringMappingPtr;

}

#endif // WAYMATCHSTRINGMAPPING_H
