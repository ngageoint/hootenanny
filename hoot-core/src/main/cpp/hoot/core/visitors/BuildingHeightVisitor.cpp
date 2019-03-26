
#include "BuildingHeightVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>

#include <float.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, BuildingHeightVisitor)

BuildingHeightVisitor::BuildingHeightVisitor() :
_totalHeight(0.0),
_numHeightsProcessed(0),
_minHeight(DBL_MAX),
_maxHeight(0.0)
{
}

void BuildingHeightVisitor::visit(const ConstElementPtr& e)
{
  if (_crit.isSatisfied(e))
  {
    const int height = _getHeight(e);
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
      _numHeightsProcessed++;
    }
    _numAffected++;
  }
}

double BuildingHeightVisitor::_getHeight(const ConstElementPtr& e) const
{
  if (e->getTags().contains("height"))
  {
    return QString::number(e->getTags().getLength("height").value(), 'g', 2).toDouble();
  }
  return 0.0;
}

}
