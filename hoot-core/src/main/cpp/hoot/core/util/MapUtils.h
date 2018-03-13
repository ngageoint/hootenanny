#ifndef MAP_UTILS_H
#define MAP_UTILS_H

// Hoot
#include <hoot/core/OsmMap.h>

namespace hoot
{

/**

  */
class MapUtils
{
public:

  static bool containsTwoOrMorePois(ConstOsmMapPtr map);
  static bool containsTwoOrMoreBuildings(ConstOsmMapPtr map);
  static bool containsTwoOrMoreAreas(ConstOsmMapPtr map);
  static bool containsOnePolygonAndOnePoi(ConstOsmMapPtr map);
  static bool containsPolys(ConstOsmMapPtr map);
  static bool containsAreas(ConstOsmMapPtr map);
  static bool containsBuildings(ConstOsmMapPtr map);
  static bool containsPois(ConstOsmMapPtr map);

};

}

#endif // MAP_UTILS_H
