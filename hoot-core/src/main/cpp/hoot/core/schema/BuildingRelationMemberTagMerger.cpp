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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "BuildingRelationMemberTagMerger.h"

// hoot
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/util/Factory.h>

// Qt
#include <QStringBuilder>

namespace hoot
{

QSet<QString> BuildingRelationMemberTagMerger::_buildingPartTagNames;

HOOT_FACTORY_REGISTER(TagMerger, BuildingRelationMemberTagMerger)

BuildingRelationMemberTagMerger::BuildingRelationMemberTagMerger(
  const QSet<QString>& ignoreTagKeys) :
_ignoreTagKeys(ignoreTagKeys)
{
}

QSet<QString> BuildingRelationMemberTagMerger::getBuildingPartTagNames()
{
  if (_buildingPartTagNames.empty())
  {
    const std::vector<SchemaVertex>& buildingPartTags =
      OsmSchema::getInstance().getAssociatedTagsAsVertices(MetadataTags::BuildingPart() + "=yes");
    for (size_t i = 0; i < buildingPartTags.size(); i++)
    {
      _buildingPartTagNames.insert(buildingPartTags[i].name.split("=")[0]);
    }
    _buildingPartTagNames.insert(MetadataTags::BuildingPart());
  }
  return _buildingPartTagNames;
}

Tags BuildingRelationMemberTagMerger::mergeTags(
  const Tags& relationTags, const Tags& constituentBuildingTags, ElementType /*elementType*/) const
{
  LOG_VART(relationTags);
  LOG_VART(constituentBuildingTags);

  Tags mergedTags = relationTags;
  Tags relationTagsCopy = relationTags;
  Tags constituentBuildingTagsCopy = constituentBuildingTags;

  Tags names;
  TagComparator::getInstance().mergeNames(relationTagsCopy, constituentBuildingTagsCopy, names,
                                          QStringList(), _caseSensitive);
  mergedTags.set(names);
  LOG_VART(mergedTags);

  // go through all the tags
  OsmSchema& schema = OsmSchema::getInstance();
  for (Tags::const_iterator it = constituentBuildingTags.begin();
       it != constituentBuildingTags.end(); ++it)
  {
    // ignore any specified ignore keys
    if (_ignoreTagKeys.find(it.key()) == _ignoreTagKeys.end())
    {
      // If the tag isn't already in the relation, add it.
      if (mergedTags.contains(it.key()) == false)
      {
        mergedTags[it.key()] = it.value();
      }
      // If this is an arbitrary text value, then concatenate the values.
      else if (schema.isTextTag(it.key()))
      {
        mergedTags.appendValueIfUnique(it.key(), it.value());
      }
      // If the tag is in the relation and the tags differ, mark it empty.
      else if (mergedTags[it.key()] != it.value())
      {
        mergedTags[it.key()] = "";
      }
    }
  }
  LOG_VART(mergedTags);

  // go through all the keys that were consistent for each of the parts and move them into the
  // relation.

  relationTagsCopy = mergedTags;
  for (Tags::const_iterator it = relationTagsCopy.begin(); it != relationTagsCopy.end(); ++it)
  {
    // If the value is empty, then the tag isn't needed, or it wasn't consistent between multiple
    // parts...remove it.
    if (it.value() == "")
    {
      mergedTags.remove(it.key());
    }
  }

  LOG_VART(mergedTags);

  return mergedTags;
}

}
