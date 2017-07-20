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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "SumNumericTagsVisitor.h"

#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

unsigned int SumNumericTagsVisitor::logWarnCount = 0;

HOOT_FACTORY_REGISTER(ElementVisitor, SumNumericTagsVisitor)

SumNumericTagsVisitor::SumNumericTagsVisitor(const QString key) :
_key(key),
_sum(0)
{
}

void SumNumericTagsVisitor::visit(const ConstElementPtr& e)
{
  if (e->getTags().contains(_key))
  {
    bool parsed = false;
    const QString strValue = e->getTags().get(_key);
    const long value = strValue.toLong(&parsed);
    if (parsed)
    {
      _sum += value;
    }
    else
    {
      if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN(
          "Unsuccessfully attempted to convert tag with key: " << _key << " and value: " <<
          strValue << " to number.");
      }
      else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
  }
}

}
