
#include "InBoundsCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Element.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, InBoundsCriterion)

InBoundsCriterion::InBoundsCriterion(const bool mustCompletelyContain) :
_mustCompletelyContain(mustCompletelyContain)
{
}

void InBoundsCriterion::setOsmMap(const OsmMap* map)
{
  _map = map->shared_from_this();
  _elementConverter.reset(new ElementConverter(_map));
}

bool InBoundsCriterion::isSatisfied(const ConstElementPtr& e) const
{
  LOG_VART(e->getElementId());
  std::shared_ptr<geos::geom::Geometry> geom = _elementConverter->convertToGeometry(e);
  LOG_VART(geom.get());
  LOG_VART(_bounds);
  if (_mustCompletelyContain)
  {
    LOG_VART(_bounds.contains(geom->getEnvelopeInternal()));
    return _bounds.contains(geom->getEnvelopeInternal());
  }
  else
  {
    LOG_VART(_bounds.intersects(geom->getEnvelopeInternal()));
    return _bounds.intersects(geom->getEnvelopeInternal());
  }
}

}
