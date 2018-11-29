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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "TagValueNumericRangeCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/Element.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, TagValueNumericRangeCriterion)

TagValueNumericRangeCriterion::TagValueNumericRangeCriterion()
{
  setConfiguration(conf());
}

TagValueNumericRangeCriterion::TagValueNumericRangeCriterion(const QStringList tagKeys,
                                                             const long rangeMin,
                                                             const long rangeMax) :
_tagKeys(tagKeys),
_rangeMin(rangeMin),
_rangeMax(rangeMax)
{
}

void TagValueNumericRangeCriterion::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  _tagKeys = configOptions.getTagValueNumericRangeCriterionKeys();
  _rangeMin = configOptions.getTagValueNumericRangeCriterionMin();
  _rangeMax = configOptions.getTagValueNumericRangeCriterionMax();
}

bool TagValueNumericRangeCriterion::isSatisfied(const ConstElementPtr& e) const
{
  //The element must have tags for all keys passed in, and all the values for those keys must be
  //within the specified range.
  for (int i = 0; i < _tagKeys.size(); i++)
  {
    const QString tagKey = _tagKeys.at(i);
    if (!e->getTags().contains(tagKey))
    {
      return false;
    }
    bool ok = false;
    const long val = e->getTags().get(tagKey).toLong(&ok);
    if (!ok || val < _rangeMin || val > _rangeMax)
    {
      return false;
    }
  }
  return true;
}

}
