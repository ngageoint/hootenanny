
#ifndef REVIEWRELATIONCRITERION_H
#define REVIEWRELATIONCRITERION_H

// Hoot
#include "ElementCriterion.h"
#include <hoot/core/OsmMap.h>
#include <hoot/core/ConstOsmMapConsumer.h>

namespace hoot
{

/**
 * Matches conflation review relations
 */
class ReviewRelationCriterion : public ElementCriterion, public ConstOsmMapConsumer
{
public:

  static std::string className() { return "hoot::ReviewRelationCriterion"; }

  ReviewRelationCriterion() {}

  ReviewRelationCriterion(ConstOsmMapPtr& map) : _map(map) { }

  virtual bool isSatisfied(const boost::shared_ptr<const Element> &e) const;

  virtual ElementCriterionPtr clone()
  { return ElementCriterionPtr(new ReviewRelationCriterion(_map)); }

  virtual void setOsmMap(const OsmMap* map) { _map = map->shared_from_this(); }

  virtual QString getDescription() const
  { return "Identifies review relations created during conflation"; }

private:

  ConstOsmMapPtr _map;

};

}

#endif // REVIEWRELATIONCRITERION_H
