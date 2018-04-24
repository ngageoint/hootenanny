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
#include "PreserveTypesTagMerger.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/OsmSchema.h>

#include "TagComparator.h"

// Qt
#include <QStringBuilder>

namespace hoot
{

HOOT_FACTORY_REGISTER(TagMerger, PreserveTypesTagMerger)

PreserveTypesTagMerger::PreserveTypesTagMerger()
{
}

Tags PreserveTypesTagMerger::mergeTags(const Tags& t1, const Tags& t2, ElementType /*et*/) const
{
  Tags result;
  Tags t1Copy = t1;
  Tags t2Copy = t2;

  TagComparator::getInstance().mergeNames(t1Copy, t2Copy, result);
  TagComparator::getInstance().mergeText(t1Copy, t2Copy, result);

  //combine the rest of the tags together; if two tags with the same key are found, use the most
  //specific one or use both if they aren't related in any way
  for (Tags::ConstIterator it = t1Copy.constBegin(); it != t1Copy.constEnd(); ++it)
  {
    LOG_VART(it.key());
    LOG_VART(it.value());
    if (!t2Copy[it.key()].trimmed().isEmpty())
    {
      LOG_VART(t2Copy[it.key()]);
      //if one is more specific than the other, add it, but then remove both tags so we don't
      //try to add them again
      if (OsmSchema::getInstance().isAncestor(it.key() % "=" % t2Copy[it.key()],
                                              it.key() % "=" % it.value()))
      {
        LOG_TRACE(
          it.key() % "=" % t2Copy[it.key()] << " is more specific than " <<
          it.key() % "=" % it.value() << ".  Using more specific tag.");
        result[it.key()] = t2Copy[it.key()];
      }
      else if (OsmSchema::getInstance().isAncestor(it.key() % "=" % it.key(),
                                                   it.key() % "=" % t2Copy[it.value()]))
      {
        LOG_TRACE(
          it.key() % "=" % it.value() << " is more specific than " <<
          it.key() % "=" % t2Copy[it.key()] << ".  Using more specific tag.");
        result[it.key()] = it.value();
      }
      else
      {
        //TODO: This is actually going to bring in any tags into alt_types and not just schema type
        //tags.  Probably need to either rename alt_types or have a more strict check on which tags
        //can be merged into it.

        //arbitrarily use first one and add second to an alt_types field
        LOG_TRACE(
          "Both tag sets contain same type: " << it.key() <<
          " but neither is more specific.  Keeping both...");
        result[it.key()] = it.value();
        if (!result["alt_types"].trimmed().isEmpty())
        {
          const QString altTypes = result["alt_types"];
          result["alt_types"] = altTypes % ";" + it.key() % "=" % t2Copy[it.key()];
        }
        else
        {
          result["alt_types"] = it.key() % "=" % t2Copy[it.key()];
        }
        LOG_VART(result["alt_types"]);
      }
    }
    else if (!it.value().isEmpty())
    {
      result[it.key()] = it.value();
    }
  }
  LOG_VART(result);

  for (Tags::ConstIterator it = t2Copy.constBegin(); it != t2Copy.constEnd(); ++it)
  {
    LOG_VART(it.key());
    LOG_VART(it.value());
    if (!result.contains(it.key()) && !it.value().isEmpty())
    {
      result[it.key()] = it.value();
    }
  }
  LOG_VART(result);

  return result;
}

}
