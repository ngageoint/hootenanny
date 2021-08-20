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
#include "PreserveTypesTagMerger.h"

// hoot
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/OverwriteTagMerger.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/CollectionUtils.h>

// Qt
#include <QStringBuilder>

namespace hoot
{

// haven't seen any need to make this configurable yet
QString PreserveTypesTagMerger::ALT_TYPES_TAG_KEY = "alt_types";

HOOT_FACTORY_REGISTER(TagMerger, PreserveTypesTagMerger)

PreserveTypesTagMerger::PreserveTypesTagMerger(const QSet<QString>& skipTagKeys) :
_overwrite1(ConfigOptions().getTagMergerDefault() == OverwriteTag1Merger::className()),
_skipTagKeys(skipTagKeys)
{
  setConfiguration(conf());
}

void PreserveTypesTagMerger::setConfiguration(const Settings& conf)
{
  TagMerger::setConfiguration(conf);
  ConfigOptions config = ConfigOptions(conf);
  setOverwriteExcludeTagKeys(config.getTagMergerOverwriteExclude());
}

Tags PreserveTypesTagMerger::mergeTags(const Tags& t1, const Tags& t2, ElementType /*et*/) const
{
  LOG_VART(t1);
  LOG_VART(t2);

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

  // handle name and text tags in the standard overwrite merge fashion
  //const QStringList excludeKeysAsList = QStringList::fromSet(_skipTagKeys);
  TagComparator::getInstance().mergeNames(tagsToOverwriteWith, tagsToBeOverwritten, result,
                                          _overwriteExcludeTagKeys, _caseSensitive);
  TagComparator::getInstance().mergeText(tagsToOverwriteWith, tagsToBeOverwritten, result,
                                         _overwriteExcludeTagKeys, _caseSensitive);
  LOG_TRACE("Tags after name/text merging: " << result);

  // retain any previously set alt_types
  if (!tagsToOverwriteWith[ALT_TYPES_TAG_KEY].trimmed().isEmpty())
  {
    result = _preserveAltTypes(tagsToOverwriteWith, result);
  }
  if (!tagsToBeOverwritten[ALT_TYPES_TAG_KEY].trimmed().isEmpty())
  {
    result = _preserveAltTypes(tagsToBeOverwritten, result);
  }
  LOG_TRACE("Tags after alt_types handling: " << result);

  // Combine the rest of the tags together. If two tags with the same key are found, use the most
  // specific one. Keep both if they aren't related in any way.
  OsmSchema& schema = OsmSchema::getInstance();
  for (Tags::ConstIterator it = tagsToOverwriteWith.constBegin();
       it != tagsToOverwriteWith.constEnd(); ++it)
  {
    LOG_VART(it.key());
    LOG_VART(it.value());

    bool skippingTagPreservation = false;
    // ignore type tags that either were specified to be explicitly skipped or general tags that
    // we don't want to be overwritten
    if (_skipTagKeys.find(it.key()) != _skipTagKeys.end() ||
        _overwriteExcludeTagKeys.contains(it.key()))
    {
      LOG_TRACE(
        "Explicitly skipping type handling for tag: " << it.key() << "=" <<  it.value() << "...");
      skippingTagPreservation = true;
    }
    // ignore metadata tags
    else if (schema.isMetaData(it.key(), it.value()))
    {
      LOG_TRACE(
        "Skipping type handling for metadata tag: " << it.key() << "=" <<  it.value() << "...");
      skippingTagPreservation = true;
    }
    // If a tag is not part of any type category, there is no point in trying to preserve its type
    // since we have no category record of it in the schema. We'll skip the type perservation and
    // just add it to the output regardless (if its value isn't empty).
    else if (!schema.hasAnyCategory(it.key(), it.value()))
    {
      LOG_TRACE(
        "Skipping type handling for tag not belonging to any type category: " << it.key() << "=" <<
        it.value() << "...");
      skippingTagPreservation = true;
    }

    const QString tagsToBeOverwrittenOfKey = tagsToBeOverwritten[it.key()].trimmed();
    if (!skippingTagPreservation && !tagsToBeOverwrittenOfKey.isEmpty())
    {
      LOG_VART(tagsToBeOverwrittenOfKey);
      // If one is more specific than the other, add it, but then remove both tags so we don't
      // try to add them again.
      if (_isAncestor(it.key(), tagsToBeOverwrittenOfKey, it.key(), it.value()))
      {
        LOG_TRACE(
          it.key() % "=" % tagsToBeOverwrittenOfKey << " is more specific than " <<
          it.key() % "=" % it.value() << ".  Using more specific tag.");
        result[it.key()] = tagsToBeOverwrittenOfKey;
      }
      else if (_isAncestor(it.key(), it.value(), it.key(), tagsToBeOverwrittenOfKey))
      {
        LOG_TRACE(
          it.key() % "=" % it.value() << " is more specific than " <<
          it.key() % "=" % tagsToBeOverwrittenOfKey << ".  Using more specific tag.");
        result[it.key()] = it.value();
      }
      else if (it.key() != ALT_TYPES_TAG_KEY)
      {
        if (tagsToBeOverwrittenOfKey == tagsToOverwriteWith[it.key()])
        {
          LOG_TRACE("Merging duplicate tag: " << it.key() << "=" << it.value() << "...");
        }
        else
        {
          // arbitrarily use first tag and add the second to an alt_types field
          LOG_TRACE(
            "Both tag sets contain same type: " << it.key() <<
            " but neither is more specific.  Keeping both...");
        }
        result[it.key()] = it.value();
        const QString resultAltType = result[ALT_TYPES_TAG_KEY].trimmed();
        LOG_VART(resultAltType);
        if (!resultAltType.isEmpty())
        {
          const QString altType = it.key() % "=" % tagsToBeOverwrittenOfKey;
          if (!resultAltType.contains(altType))
          {
            result[ALT_TYPES_TAG_KEY] =
              resultAltType % ";" + it.key() % "=" % tagsToBeOverwrittenOfKey;
          }
        }
        else
        {
          result[ALT_TYPES_TAG_KEY] = it.key() % "=" % tagsToBeOverwrittenOfKey;
        }
        LOG_VART(result[ALT_TYPES_TAG_KEY]);
      }
    }
    // None of the special cases for skipping or type merging apply to this tag, so let's just add
    // it as is.
    else if (!it.value().isEmpty())
    {
      LOG_TRACE("Adding tag: " << it.key() << "=" << it.value() << "...");
      result[it.key()] = it.value();
    }
  }
  LOG_TRACE("Tags after type handling: " << result);

  // handle all the remaining tags that don't conflict
  for (Tags::ConstIterator it = tagsToBeOverwritten.constBegin();
       it != tagsToBeOverwritten.constEnd(); ++it)
  {
    LOG_VART(it.key());
    LOG_VART(it.value());

    if (!result.contains(it.key()) && !it.value().isEmpty() &&
        _skipTagKeys.find(it.key()) == _skipTagKeys.end())
    {
      result[it.key()] = it.value();
    }
  }
  LOG_TRACE("Tags after processing remaining in t2: " << result);

  _removeRedundantAltTypeTags(result);
  LOG_TRACE("Tags after removing redundant (final): " << result);

  return result;
}

bool PreserveTypesTagMerger::_isAncestor(const QString& childKey, const QString& childVal,
                                         const QString& parentKey, const QString& parentVal) const
{
  // It seems like OsmSchema::isAncestor doesn't handle generic type tags correctly, e.g.
  // building=yes. Its also possible that it does handle them correctly and our schema is not
  // correct somehow. Until that's determined, I'm adding this custom logic to make sure that if,
  // for example, building=yes is compared to building=mosque we always end up with the more
  // specific building=mosque in the output.

  // If the tags have the same key, the parent is generic, and the child is not, the parent is
  // an ancestor of the child. not completely sure this is covering all possible cases yet...
  if (childKey == parentKey && (parentVal == "yes" || parentVal == "true") && childVal != "yes" &&
      childVal != "true")
  {
    return true;
  }
  return
    OsmSchema::getInstance().isAncestor(childKey % "=" % childVal, parentKey % "=" % parentVal);
}

void PreserveTypesTagMerger::_removeRedundantAltTypeTags(Tags& tags) const
{
  LOG_VART(tags.contains(ALT_TYPES_TAG_KEY));
  Tags::ConstIterator itr = tags.find(ALT_TYPES_TAG_KEY);
  if (itr != tags.end())
  {
    // remove anything in alt_types that's also in the tags being merged
    const QStringList altTypes = itr.value().split(";");
    LOG_VART(altTypes);
    QStringList altTypesCopy = altTypes;
    for (int i = 0; i < altTypes.size(); i++)
    {
      LOG_VART(altTypes[i]);
      const QString altType = altTypes[i];
      if (altType.contains("="))
      {
        const QStringList tagParts = altTypes[i].split("=");
        if (tagParts.size() == 2)
        {
          LOG_VART(tagParts);
          LOG_VART(tags.get(tagParts[0]) == tagParts[1]);
          if (tags.get(tagParts[0]) == tagParts[1])
          {
            altTypesCopy.removeAll(tagParts[0] + "=" + tagParts[1]);
          }
        }
      }
    }
    LOG_VART(altTypesCopy);
    if (altTypes.size() != altTypesCopy.size())
    {
      LOG_VART(altTypesCopy.join(";"));
      tags.set(ALT_TYPES_TAG_KEY, altTypesCopy.join(";"));
    }
  }
  //tags.removeEmpty(); // why does this crash?
  if (tags[ALT_TYPES_TAG_KEY].trimmed().isEmpty())
  {
    tags.remove(ALT_TYPES_TAG_KEY);
  }
}

Tags PreserveTypesTagMerger::_preserveAltTypes(const Tags& source, const Tags& target) const
{
  Tags updatedTags = target;
  const QStringList altTypes = source[ALT_TYPES_TAG_KEY].split(";");
  for (int i = 0; i < altTypes.size(); i++)
  {
    const QString altType = altTypes.at(i);
    const QString updatedAlType = updatedTags[ALT_TYPES_TAG_KEY].trimmed();
    if (updatedAlType.isEmpty())
    {
      updatedTags[ALT_TYPES_TAG_KEY] = altType;
    }
    else
    {
      if (!updatedAlType.contains(altType))
      {
        updatedTags[ALT_TYPES_TAG_KEY] = updatedAlType + ";" + altType;
      }
    }
  }
  return updatedTags;
}

}
