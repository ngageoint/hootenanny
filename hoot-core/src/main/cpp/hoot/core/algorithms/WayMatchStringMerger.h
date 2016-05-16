#ifndef WAYSTRINGMERGER_H
#define WAYSTRINGMERGER_H

// hoot
#include <hoot/core/algorithms/linearreference/WaySublineMatchString.h>
#include <hoot/core/algorithms/linearreference/WayMatchStringMapping.h>
#include <hoot/core/schema/TagMerger.h>

namespace hoot
{

/**
 * Provides methods for merging features/geometries in one string of ways into another string of
 * ways.
 */
class WayMatchStringMerger
{
public:
  WayMatchStringMerger(WaySublineMatchStringPtr matchString, WayMatchStringMappingPtr mapping);

  /**
   * Merge all the tags from the scrap into the associated keeper ways.
   */
  void mergeTags(const TagMerger& merger);

  /**
   * Set the status on all the keeper ways.
   */
  void setKeeperStatus(Status s);

  /**
   * Merge scrapNode into the keeper way.
   */
  void mergeNode(ElementId scrapNode);


};

typedef boost::shared_ptr<WayMatchStringMerger> WayMatchStringMergerPtr;
typedef boost::shared_ptr<const WayMatchStringMerger> ConstWayMatchStringMergerPtr;

}

#endif // WAYSTRINGMERGER_H
