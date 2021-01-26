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
 * @copyright Copyright (C) 2015, 2017, 2018, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef TAGCATEGORYDIFFERENCER_H
#define TAGCATEGORYDIFFERENCER_H

#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TagFilteredDifferencer.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Compare tags that have the specified category. Ignore the other tags that don't have the
 * same category.
 */
class TagCategoryDifferencer : public TagFilteredDifferencer, public Configurable
{
public:
  static QString className() { return "hoot::TagCategoryDifferencer"; }

  TagCategoryDifferencer() = default;
  TagCategoryDifferencer(OsmSchemaCategory category);
  virtual ~TagCategoryDifferencer() = default;

  virtual void setConfiguration(const Settings& conf);

protected:

  OsmSchemaCategory _category;

  virtual bool isValidTag(const SchemaVertex& sv) const;
};

}

#endif // TAGCATEGORYDIFFERENCER_H
