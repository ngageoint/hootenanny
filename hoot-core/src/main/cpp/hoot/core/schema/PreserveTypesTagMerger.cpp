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
#include "PreserveTypesTagMerger.h"

// hoot
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/OverwriteTagMerger.h>

// Qt
#include <QStringBuilder>

namespace hoot
{

// haven't seen any need to make this configurable yet
QString PreserveTypesTagMerger::ALT_TYPES_TAG_KEY = "alt_types";

HOOT_FACTORY_REGISTER(TagMerger, PreserveTypesTagMerger)

PreserveTypesTagMerger::PreserveTypesTagMerger(const std::set<QString>& skipTagKeys,
                                               const OsmSchemaCategory& categoryFilter) :
_overwrite1(ConfigOptions().getTagMergerDefault() ==
            QString::fromStdString(OverwriteTag1Merger::className())),
_skipTagKeys(skipTagKeys),
_categoryFilter(categoryFilter)
{
}

Tags PreserveTypesTagMerger::mergeTags(const Tags& t1, const Tags& t2, ElementType /*et*/) const
{
  Tags result;
  Tags tagsToOverwriteWith;
  Tags tagsToBeOverwritten;
  if (_overwrite1)
  {
    tagsToOverwriteWith = t2;
    tagsToBeOverwritten = t1;
  }
  else
  {
    tagsToOverwriteWith = t1;
    tagsToBeOverwritten = t2;
  }

  TagComparator::getInstance().mergeNames(tagsToOverwriteWith, tagsToBeOverwritten, result);
  TagComparator::getInstance().mergeText(tagsToOverwriteWith, tagsToBeOverwritten, result);
  LOG_VART(result);

  //retain any previously set alt_types
  if (!tagsToOverwriteWith[ALT_TYPES_TAG_KEY].trimmed().isEmpty())
  {
    result = _preserveAltTypes(tagsToOverwriteWith, result);
  }
  if (!tagsToBeOverwritten[ALT_TYPES_TAG_KEY].trimmed().isEmpty())
  {
    result = _preserveAltTypes(tagsToBeOverwritten, result);
  }
  LOG_VART(result);

  //combine the rest of the tags together; if two tags with the same key are found, use the most
  //specific one or use both if they aren't related in any way
  OsmSchema& schema = OsmSchema::getInstance();
  for (Tags::ConstIterator it = tagsToOverwriteWith.constBegin();
       it != tagsToOverwriteWith.constEnd(); ++it)
  {
    LOG_VART(it.key());
    LOG_VART(it.value());

    if (_skipTagKeys.find(it.key()) != _skipTagKeys.end())
    {
      LOG_TRACE("Explicitly skipping tag: " << it.key() << "=" <<  it.value() << "...");
      continue;
    }
    if (schema.isMetaData(it.key(), it.value()))
    {
      LOG_TRACE("Skipping metadata tag: " << it.key() << "=" <<  it.value() << "...");
      continue;
    }
    if (!_passesSchemaFilter(it.key(), it.value()))
    {
      LOG_TRACE(
        "Skipping tag not passing category filter: " << it.key() << "=" << it.value() << "...");
      continue;
    }

    if (!tagsToBeOverwritten[it.key()].trimmed().isEmpty())
    {
      LOG_VART(tagsToBeOverwritten[it.key()]);
      //if one is more specific than the other, add it, but then remove both tags so we don't
      //try to add them again
      if (schema.isAncestor(
            it.key() % "=" % tagsToBeOverwritten[it.key()], it.key() % "=" % it.value()))
      {
        LOG_TRACE(
          it.key() % "=" % tagsToBeOverwritten[it.key()] << " is more specific than " <<
          it.key() % "=" % it.value() << ".  Using more specific tag.");
        result[it.key()] = tagsToBeOverwritten[it.key()];
      }
      else if (schema.isAncestor(
                 it.key() % "=" % it.key(), it.key() % "=" % tagsToBeOverwritten[it.value()]))
      {
        LOG_TRACE(
          it.key() % "=" % it.value() << " is more specific than " <<
          it.key() % "=" % tagsToBeOverwritten[it.key()] << ".  Using more specific tag.");
        result[it.key()] = it.value();
      }
      else if (it.key() != ALT_TYPES_TAG_KEY)
      {
        //arbitrarily use first one and add second to an alt_types field
        LOG_TRACE(
          "Both tag sets contain same type: " << it.key() <<
          " but neither is more specific.  Keeping both...");
        result[it.key()] = it.value();
        LOG_VART(result[ALT_TYPES_TAG_KEY]);
        if (!result[ALT_TYPES_TAG_KEY].trimmed().isEmpty())
        {
          const QString altType = it.key() % "=" % tagsToBeOverwritten[it.key()];
          if (!result[ALT_TYPES_TAG_KEY].contains(altType))
          {
            result[ALT_TYPES_TAG_KEY] =
              result[ALT_TYPES_TAG_KEY] % ";" + it.key() % "=" % tagsToBeOverwritten[it.key()];
          }
        }
        else
        {
          result[ALT_TYPES_TAG_KEY] = it.key() % "=" % tagsToBeOverwritten[it.key()];
        }
        LOG_VART(result[ALT_TYPES_TAG_KEY]);
      }
    }
    else if (!it.value().isEmpty())
    {
      result[it.key()] = it.value();
    }
  }
  LOG_VART(result);

  for (Tags::ConstIterator it = tagsToBeOverwritten.constBegin();
       it != tagsToBeOverwritten.constEnd(); ++it)
  {
    LOG_VART(it.key());
    LOG_VART(it.value());

    if (!result.contains(it.key()) && !it.value().isEmpty() &&
        _skipTagKeys.find(it.key()) == _skipTagKeys.end() /*&&
        // TODO: should this be here?
        _passesSchemaFilter(it.key(), it.value())*/)
    {
      result[it.key()] = it.value();
    }
  }
  LOG_VART(result);

  _removeRedundantAltTypeTags(result);
  LOG_VART(result);

  return result;
}

bool PreserveTypesTagMerger::_passesSchemaFilter(const QString& key, const QString& val) const
{
  OsmSchema& schema = OsmSchema::getInstance();
  LOG_VART(_categoryFilter.toString());

  if (_categoryFilter == OsmSchemaCategory::Empty ||
      (OsmSchemaCategory::building().intersects(_categoryFilter) && key == "building") ||
      (OsmSchemaCategory::poi().intersects(_categoryFilter) && key == "poi"))
  {
    return true;
  }

  LOG_VART(schema.getCategories(key, val));

  // TODO: Should we also check for tag similarity as well?
  return schema.getCategories(key, val).intersects(_categoryFilter);
}

void PreserveTypesTagMerger::_removeRedundantAltTypeTags(Tags& tags) const
{
  LOG_VART(tags.contains(ALT_TYPES_TAG_KEY));
  if (tags.contains(ALT_TYPES_TAG_KEY))
  {
    // Remove anything in alt_types that's also in the building (may be able to handle this within
    // PreserveTypesTagMerger instead). So far, this has primarily been done to keep building=yes
    // out of alt_types.
    const QStringList altTypes = tags.get(ALT_TYPES_TAG_KEY).split(";");
    LOG_VART(altTypes);
    QStringList altTypesCopy = altTypes;
    for (int i = 0; i < altTypes.size(); i++)
    {
      LOG_VART(altTypes[i]);
      assert(altTypes[i].contains("="));
      const QStringList tagParts = altTypes[i].split("=");
      assert(tagParts.size() == 2);
      LOG_VART(tagParts);
      LOG_VART(tags.get(tagParts[0]) == tagParts[1]);
      if (tags.get(tagParts[0]) == tagParts[1])
      {
        altTypesCopy.removeAll(tagParts[0] + "=" + tagParts[1]);
      }
    }
    LOG_VART(altTypesCopy);
    if (altTypes.size() != altTypesCopy.size())
    {
      LOG_VART(altTypesCopy.join(";"));
      tags.set(ALT_TYPES_TAG_KEY, altTypesCopy.join(";"));
    }
  }
  //tags.removeEmptyTags(); // why does this crash?
  if (tags[ALT_TYPES_TAG_KEY].trimmed().isEmpty())
  {
    tags.remove(ALT_TYPES_TAG_KEY);
  }
}

Tags PreserveTypesTagMerger::_preserveAltTypes(const Tags& source, const Tags& target) const
{
  LOG_TRACE("Preserving alt_types tag...");

  Tags updatedTags = target;
  const QStringList altTypes = source[ALT_TYPES_TAG_KEY].split(";");
  for (int i = 0; i < altTypes.size(); i++)
  {
    const QString altType = altTypes.at(i);
    if (updatedTags[ALT_TYPES_TAG_KEY].trimmed().isEmpty())
    {
      updatedTags[ALT_TYPES_TAG_KEY] = altType;
    }
    else
    {
      if (!updatedTags[ALT_TYPES_TAG_KEY].contains(altType))
      {
        updatedTags[ALT_TYPES_TAG_KEY] = updatedTags[ALT_TYPES_TAG_KEY] + ";" + altType;
      }
    }
  }
  LOG_VART(updatedTags);
  return updatedTags;
}

}
