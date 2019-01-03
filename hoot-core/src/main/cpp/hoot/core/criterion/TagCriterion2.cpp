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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "TagCriterion2.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/Element.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, TagCriterion2)

TagCriterion2::TagCriterion2()
{
  setConfiguration(conf());
}

TagCriterion2::TagCriterion2(const QString filter)
{
  _parseFilterString(filter);
}

void TagCriterion2::setConfiguration(const Settings& s)
{
  _parseFilterString(ConfigOptions(s).getTagFilter());
}

void TagCriterion2::_parseFilterString(const QString /*filterStr*/)
{
  /*
   * non-sensical example, but illustrates all the possible filter inputs
   *
   * {
       "feature_filter":
       {
         "must":
         [
           {
             "tourism=hotel",
             "allowAliases": "true"
           }
         ]
         "should":
         [
           {
             "amenity=restaurant",
             "similarityThreshold": "0.8"
           },
           {
             "amenity=place_of_worship"
           },
           {
             "*address*=*"
           },
           {
             "poi*=*"
           },
           {
             "*building=*"
           },
           {
             "*=*address*"
           },
           {
             "*=poi*"
           },
           {
             "*=*building"
           }
         ],
         "must_not":
         [
           {
             "amenity=chapel"
           }
         ]
       }
     }
   */


}

bool TagCriterion2::isSatisfied(const ConstElementPtr& /*e*/) const
{

  return false;
}

}
