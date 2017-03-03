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
#include "TagCategoryDifferencer.h"

#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(TagDifferencer, TagCategoryDifferencer)

TagCategoryDifferencer::TagCategoryDifferencer(OsmSchemaCategory category) :
  _category(category)
{
  if (_category.toStringList().size() != 1)
  {
    throw HootException("The TagCategoryDifferencer can only filter on a single category.");
  }
}

TagCategoryDifferencer::~TagCategoryDifferencer()
{
}

bool TagCategoryDifferencer::isValidTag(const SchemaVertex& sv) const
{
  if (_category.getEnum() == OsmSchemaCategory::Empty)
  {
    throw IllegalArgumentException("You must specify exactly one category to the "
      "TagCategoryDifferencer.");
  }
  return sv.categories.contains(_category.toString());
}

void TagCategoryDifferencer::setConfiguration(const Settings& conf)
{
  _category = OsmSchemaCategory::fromString(ConfigOptions(conf).getTagCategoryDifferencerName());
  if (_category.toStringList().size() != 1)
  {
    throw HootException("The TagCategoryDifferencer can only filter on a single category.");
  }
}

}
