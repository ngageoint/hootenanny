
#include "BuildingHeightVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>

#include <float.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, BuildingHeightVisitor)

BuildingHeightVisitor::BuildingHeightVisitor() :
_totalHeight(0.0),
_minHeight(DBL_MAX),
_maxHeight(0.0)
{
}

void BuildingHeightVisitor::visit(const ConstElementPtr& e)
{
  if (_crit.isSatisfied(e))
  {
    const Meters height = _getHeight(e);
    LOG_VART(height);
    if (height > 0)
    {
      _totalHeight += height;
      if (height < _minHeight)
      {
        _minHeight = height;
      }
      if (height > _maxHeight)
      {
        _maxHeight = height;
      }
      // We're only going to return stats for buildings with the tag.
      _numAffected++;
    }
  }
}

Meters BuildingHeightVisitor::_getHeight(const ConstElementPtr& e) const
{
  if (e->getTags().contains("height"))
  {
    const Meters height =
      QString::number(e->getTags().getLength("height").value(), 'g', 2).toDouble();
    LOG_VART(height);
    return height;
  }
  return 0.0;
}

}
