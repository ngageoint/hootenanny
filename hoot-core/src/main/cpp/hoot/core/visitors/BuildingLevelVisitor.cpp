
#include "BuildingLevelVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, BuildingLevelVisitor)

BuildingLevelVisitor::BuildingLevelVisitor() :
_totalLevels(0),
_numLevelsProcessed(0),
_minLevels(INT_MAX),
_maxLevels(0)
{
}

void BuildingLevelVisitor::visit(const ConstElementPtr& e)
{
  if (_crit.isSatisfied(e))
  {
    const int numLevels = _getNumLevels(e);
    if (numLevels > 0)
    {
      _totalLevels += numLevels;
      if (numLevels < _minLevels)
      {
        _minLevels = numLevels;
      }
      if (numLevels > _maxLevels)
      {
        _maxLevels = numLevels;
      }
      _numLevelsProcessed++;
    }
    _numAffected++;
  }
}

int BuildingLevelVisitor::_getNumLevels(const ConstElementPtr& e) const
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
