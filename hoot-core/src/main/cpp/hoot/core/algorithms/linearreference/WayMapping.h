#ifndef WAYMAPPING_H
#define WAYMAPPING_H

#include <hoot/core/algorithms/linearreference/WayLocation.h>

namespace hoot
{

/**
 * Given a WaySublineMatchString, produce a mapping from one way string to the other.
 */
class WayMapping
{
public:
  virtual ~WayMapping();

  /**
   * Given a point on subline string 1, return the corresponding point on subline string 2.
   */
  virtual WayLocationPtr map1to2(ConstWayLocationPtr wl1) const = 0;

  /**
   * Given a point on subline string 2, return the corresponding point on subline string 1.
   */
  virtual WayLocationPtr map2to1(ConstWayLocationPtr wl1) const = 0;

};

}

#endif // WAYMAPPING_H
