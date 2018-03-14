
#ifndef POIMERGERJS_H
#define POIMERGERJS_H

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
class PoiMergerJs
{

public:

  /**
   * an unlimited number of pois may be merged
   *
   * @param map
   * @param mergeTargetId
   * @param current
   */
  static void mergePois(OsmMapPtr map, const ElementId& mergeTargetId, Isolate* current);

};

}

#endif // POIMERGERJS_H
