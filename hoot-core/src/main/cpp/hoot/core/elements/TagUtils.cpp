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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "TagUtils.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

bool TagUtils::allElementsHaveAnyTagKey(const QStringList& tagKeys,
                                        const std::vector<ElementPtr>& elements)
{
  for (std::vector<ElementPtr>::const_iterator it = elements.begin(); it != elements.end(); ++it)
  {
    ElementPtr element = *it;
    bool elementHasTagKey = false;
    for (int i = 0; i < tagKeys.size(); i++)
    {
      if (element->getTags().contains(tagKeys.at(i)))
      {
        elementHasTagKey = true;
        break;
      }
    }
    if (!elementHasTagKey)
    {
      return false;
    }
  }
  return true;
}

bool TagUtils::allElementsHaveAnyKvp(const QStringList& kvps,
                                     const std::vector<ElementPtr>& elements)
{

  for (std::vector<ElementPtr>::const_iterator it = elements.begin(); it != elements.end(); ++it)
  {
    ElementPtr element = *it;
    bool elementHasKvp = false;
    for (int i = 0; i < kvps.size(); i++)
    {
      const QString kvp = kvps.at(i);
      const QStringList kvpParts = kvp.split("=");
      if (kvpParts.size() != 2)
      {
        throw IllegalArgumentException("Invalid kvp: " + kvp);
      }
      const QString key = kvpParts[0];
      const QString val = kvpParts[1];
      if (element->getTags()[key] == val)
      {
        elementHasKvp = true;
        break;
      }
    }
    if (!elementHasKvp)
    {
      return false;
    }
  }
  return true;
}

bool TagUtils::anyElementsHaveAnyTagKey(const QStringList& tagKeys,
                                        const std::vector<ElementPtr>& elements)
{
  for (std::vector<ElementPtr>::const_iterator it = elements.begin(); it != elements.end(); ++it)
  {
    ElementPtr element = *it;
    for (int i = 0; i < tagKeys.size(); i++)
    {
      if (element->getTags().contains(tagKeys.at(i)))
      {
        return true;
      }
    }
  }
  return false;
}

bool TagUtils::anyElementsHaveAnyKvp(const QStringList& kvps,
                                     const std::vector<ElementPtr>& elements)
{

  for (std::vector<ElementPtr>::const_iterator it = elements.begin(); it != elements.end(); ++it)
  {
    ElementPtr element = *it;
    for (int i = 0; i < kvps.size(); i++)
    {
      const QString kvp = kvps.at(i);
      const QStringList kvpParts = kvp.split("=");
      if (kvpParts.size() != 2)
      {
        throw IllegalArgumentException("Invalid kvp: " + kvp);
      }
      const QString key = kvpParts[0];
      const QString val = kvpParts[1];
      if (element->getTags()[key] == val)
      {
        return true;
      }
    }
  }
  return false;
}

bool TagUtils::allElementsHaveAnyTagKey(const QStringList& tagKeys,
                                        const std::set<ElementId>& elementIds, OsmMapPtr& map)
{
  std::vector<ElementPtr> elements;
  for (std::set<ElementId>::const_iterator it = elementIds.begin(); it != elementIds.end(); ++it)
  {
    elements.push_back(map->getElement(*it));
  }
  return allElementsHaveAnyTagKey(tagKeys, elements);
}

bool TagUtils::allElementsHaveAnyKvp(const QStringList& kvps,
                                     const std::set<ElementId>& elementIds, OsmMapPtr& map)
{
  std::vector<ElementPtr> elements;
  for (std::set<ElementId>::const_iterator it = elementIds.begin(); it != elementIds.end(); ++it)
  {
    elements.push_back(map->getElement(*it));
  }
  return allElementsHaveAnyKvp(kvps, elements);
}

bool TagUtils::anyElementsHaveAnyTagKey(const QStringList& tagKeys,
                                        const std::set<ElementId>& elementIds, OsmMapPtr& map)
{
  std::vector<ElementPtr> elements;
  for (std::set<ElementId>::const_iterator it = elementIds.begin(); it != elementIds.end(); ++it)
  {
    elements.push_back(map->getElement(*it));
  }
  return anyElementsHaveAnyTagKey(tagKeys, elements);
}

bool TagUtils::anyElementsHaveAnyKvp(const QStringList& kvps,
                                     const std::set<ElementId>& elementIds, OsmMapPtr& map)
{
  std::vector<ElementPtr> elements;
  for (std::set<ElementId>::const_iterator it = elementIds.begin(); it != elementIds.end(); ++it)
  {
    elements.push_back(map->getElement(*it));
  }
  return anyElementsHaveAnyKvp(kvps, elements);
}

}
