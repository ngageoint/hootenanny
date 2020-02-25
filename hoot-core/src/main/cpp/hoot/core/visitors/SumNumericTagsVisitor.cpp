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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "SumNumericTagsVisitor.h"

#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

int SumNumericTagsVisitor::logWarnCount = 0;

HOOT_FACTORY_REGISTER(ElementVisitor, SumNumericTagsVisitor)

SumNumericTagsVisitor::SumNumericTagsVisitor() :
_sum(0.0)
{
}

SumNumericTagsVisitor::SumNumericTagsVisitor(const QStringList keys) :
_keys(keys),
_sum(0.0)
{
}

void SumNumericTagsVisitor::setConfiguration(const Settings& conf)
{
  _keys = ConfigOptions(conf).getTagsVisitorKeys();
  LOG_VART(_keys);
}

void SumNumericTagsVisitor::visit(const ConstElementPtr& e)
{
  for (int i = 0; i < _keys.size(); i++)
  {
    const QString key = _keys.at(i);
    LOG_VART(key);

    if (e->getTags().contains(key))
    {
      bool parsed = false;
      const QString strValue = e->getTags().get(key);
      const long value = strValue.toLong(&parsed);
      if (parsed)
      {
        _sum += value;
        _numAffected++;
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
  _numProcessed++;
}

}
