
#ifndef IN_BOUNDS_CRITERION_H
#define IN_BOUNDS_CRITERION_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/ops/Boundable.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * TODO
 */
class InBoundsCriterion : public ElementCriterion, public Boundable, public ConstOsmMapConsumer,
  public Configurable
{
public:

  static std::string className() { return "hoot::InBoundsCriterion"; }

  InBoundsCriterion();
  InBoundsCriterion(const bool mustCompletelyContain);

  virtual bool isSatisfied(const ConstElementPtr& e) const;

  virtual void setBounds(const geos::geom::Envelope& bounds) { _bounds = bounds; }

  virtual void setConfiguration(const Settings& conf);

  virtual void setOsmMap(const OsmMap* map);

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new InBoundsCriterion()); }

  virtual QString getDescription() const { return "TODO"; }

private:

  geos::geom::Envelope _bounds;
  ConstOsmMapPtr _map;
  std::shared_ptr<ElementConverter> _elementConverter;
  bool _mustCompletelyContain;
};

}

#endif // IN_BOUNDS_CRITERION_H
