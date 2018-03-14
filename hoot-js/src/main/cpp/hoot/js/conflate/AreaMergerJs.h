
#ifndef AREAMERGERJS_H
#define AREAMERGERJS_H

// Hoot
#include <hoot/js/HootJsStable.h>
#include <hoot/js/SystemNodeJs.h>
#include <hoot/js/PluginContext.h>
#include <hoot/core/OsmMap.h>

using namespace v8;

namespace hoot
{

/**
 *
 */
class AreaMergerJs
{

public:

  /**
   * an unlimited number of areas may be merged
   *
   * @param map
   * @param mergeTargetId
   * @param current
   */
  static void mergeAreas(OsmMapPtr map, const ElementId& mergeTargetId, Isolate* current);
};

}

#endif // AREAMERGERJS_H
