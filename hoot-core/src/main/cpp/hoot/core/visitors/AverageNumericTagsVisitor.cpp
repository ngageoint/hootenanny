
#include "AverageNumericTagsVisitor.h"

#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

unsigned int AverageNumericTagsVisitor::logWarnCount = 0;

HOOT_FACTORY_REGISTER(ConstElementVisitor, AverageNumericTagsVisitor)

AverageNumericTagsVisitor::AverageNumericTagsVisitor() :
_sum(0.0),
_tagCount(0)
{
  setConfiguration(conf());
}

AverageNumericTagsVisitor::AverageNumericTagsVisitor(const QStringList keys) :
_keys(keys),
_sum(0.0),
_tagCount(0)
{
}

void AverageNumericTagsVisitor::setConfiguration(const Settings& conf)
{
  _keys = ConfigOptions(conf).getAverageNumericTagsVisitorKeys();
  LOG_VART(_keys);
}

void AverageNumericTagsVisitor::visit(const ConstElementPtr& e)
{
  for (int i = 0; i < _keys.size(); i++)
  {
    const QString key = _keys.at(i);
    LOG_VART(key);
    if (e->getTags().contains(key))
    {
      bool parsed = false;
      const QString strValue = e->getTags().get(key);
      LOG_VART(strValue);
      const double value = strValue.toDouble(&parsed);
      LOG_VART(value);
      LOG_VART(parsed);
      if (parsed)
      {
        _tagCount++;
        _sum += value;
        LOG_VART(_tagCount);
        LOG_VART(_sum);
      }
      else
      {
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN(
            "Unsuccessfully attempted to convert tag with key: " << key << " and value: " <<
            strValue << " to number.");
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
      }
    }
  }
}

double AverageNumericTagsVisitor::getStat() const
{
  LOG_VART(_sum);
  LOG_VART(_tagCount);
  if (_sum > 0.0 && _tagCount > 0)
  {
    return _sum / _tagCount;
  }
  return 0.0;
}

}
