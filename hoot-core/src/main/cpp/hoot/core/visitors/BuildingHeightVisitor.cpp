
#include "BuildingHeightVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, BuildingHeightVisitor)

BuildingHeightVisitor::BuildingHeightVisitor() :
_totalHeight(0.0),
_minHeight(0.0),
_maxHeight(0.0)
{
}

void BuildingHeightVisitor::visit(const ConstElementPtr& e)
{
  if (_crit.isSatisfied(e))
  {
    const Meters height = _getHeight(e);
    LOG_VART(height);
    // height = 0 denotes no height tag present. We're only going to return stats for buildings
    // with the tag.
    if (height > 0.0)
    {
      _totalHeight += height;
      if (_minHeight == 0 || height < _minHeight)
      {
        _minHeight = height;
      }
      if (height > _maxHeight)
      {
        _maxHeight = height;
      }
      _numAffected++;
    }
  }
}

Meters BuildingHeightVisitor::_getHeight(const ConstElementPtr& e)
{
  QString heightStr = e->getTags().get("height").trimmed();
  if (!heightStr.isEmpty())
  {
    _cleanAndStandardizeLengthString(heightStr);
    bool ok = false;
    const Meters height = heightStr.toDouble(&ok);
    if (ok)
    {
      LOG_VART(height);
      return height;
    }
  }
  return 0.0;
}

void BuildingHeightVisitor::_cleanAndStandardizeLengthString(QString& value)
{
  value = value.toLower().trimmed();
  // convert feet to meters

  value.replace("m", "");
  value.replace(",", ".");
  value = value.simplified();
}

}
