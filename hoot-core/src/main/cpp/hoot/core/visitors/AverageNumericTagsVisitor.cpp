/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#include "AverageNumericTagsVisitor.h"

#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

int AverageNumericTagsVisitor::logWarnCount = 0;

HOOT_FACTORY_REGISTER(ElementVisitor, AverageNumericTagsVisitor)

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
  _keys = ConfigOptions(conf).getTagsVisitorKeys();
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
