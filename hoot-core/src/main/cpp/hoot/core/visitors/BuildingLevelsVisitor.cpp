
#include "BuildingLevelsVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, BuildingLevelsVisitor)

BuildingLevelsVisitor::BuildingLevelsVisitor() :
_totalLevels(0),
_minLevels(0),
_maxLevels(0)
{
}

void BuildingLevelsVisitor::visit(const ConstElementPtr& e)
{
  if (_crit.isSatisfied(e))
  {
    const int numLevels = _getNumLevels(e);
    // num levels = 0 denotes no levels tag. We're only going to return stats for buildings with
    // the tag.
    if (numLevels > 0)
    {
      _totalLevels += numLevels;
      if (_minLevels == 0 || numLevels < _minLevels)
      {
        _minLevels = numLevels;
      }
      if (numLevels > _maxLevels)
      {
        _maxLevels = numLevels;
      }
      _numAffected++;
    }
  }
}

int BuildingLevelsVisitor::_getNumLevels(const ConstElementPtr& e) const
{
  // just handling the levels above ground for now
  if (e->getTags().contains("building:levels"))
  {
    bool ok = false;
    const int numLevels = e->getTags().get("building:levels").toInt(&ok);
    if (ok)
    {
      return numLevels;
    }
  }
  return 0;
}

}
