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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "BuildingPartTagMerger.h"

// hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/PreserveTypesTagMerger.h>

// Qt
#include <QStringBuilder>

namespace hoot
{

QString BuildingPartTagMerger::ALT_TYPES_TAG_KEY = "alt_types";

HOOT_FACTORY_REGISTER(TagMerger, BuildingPartTagMerger)

BuildingPartTagMerger::BuildingPartTagMerger()
{
}

BuildingPartTagMerger::BuildingPartTagMerger(std::set<QString> buildingPartTagNames) :
_buildingPartTagNames(buildingPartTagNames)
{
}

Tags BuildingPartTagMerger::mergeTags(const Tags& buildingTags, const Tags& buildingPartTags,
                                      ElementType /*et*/) const
{
  LOG_VART(buildingTags);
  LOG_VART(buildingPartTags);

  Tags mergedTags = buildingTags;
  Tags buildingTagsCopy = buildingTags;
  Tags buildingPartTagsCopy = buildingPartTags;

  Tags names;
  TagComparator::getInstance().mergeNames(buildingTagsCopy, buildingPartTagsCopy, names);
  mergedTags.set(names);
  LOG_VART(mergedTags);

  // go through all the tags
  OsmSchema& schema = OsmSchema::getInstance();
  for (Tags::const_iterator it = buildingPartTags.begin(); it != buildingPartTags.end(); ++it)
  {
    // ignore all keys that are building:part specific.
    if (_buildingPartTagNames.find(it.key()) == _buildingPartTagNames.end())
    {
      // if the tag isn't already in the relation
      if (mergedTags.contains(it.key()) == false)
      {
        mergedTags[it.key()] = it.value();
      }
      // if this is an arbitrary text value, then concatenate the values.
      else if (schema.isTextTag(it.key()))
      {
        mergedTags.appendValueIfUnique(it.key(), it.value());
      }
      // if the tag is in the relation and the tags differ.
      else if (mergedTags[it.key()] != it.value())
      {
        mergedTags[it.key()] = "";  // TODO: this breaks type preservation
      }
    }
  }
  LOG_VART(mergedTags);

  // go through all the keys that were consistent for each of the parts and move them into the
  // relation.
  buildingTagsCopy = mergedTags;
  for (Tags::const_iterator it = buildingTagsCopy.begin(); it != buildingTagsCopy.end(); ++it)
  {
    // if the value is empty, then the tag isn't needed, or it wasn't consistent between multiple
    // parts.
    if (it.value() == "")   // TODO: this breaks type preservation
    {
      mergedTags.remove(it.key());
    }
    LOG_VART(mergedTags);
  }

  if (!mergedTags.contains("building"))
  {
    mergedTags["building"] = "yes";
  }

  LOG_VART(mergedTags);

  return mergedTags;
}

}
