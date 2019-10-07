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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ManualMatchValidator.h"

// hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, ManualMatchValidator)

ManualMatchValidator::ManualMatchValidator()
{
}

void ManualMatchValidator::apply(const OsmMapPtr& map)
{
  _numAffected = 0;
  _errors.clear();
  _ref1Mappings.clear();

  // Get all the REF1 values, as we'll need them when examining REF2/REVIEW tags.
  map->visitRo(_ref1Mappings);

  const NodeMap& nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    ConstNodePtr node = it->second;
    _validate(node);
  }

  const WayMap& ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    ConstWayPtr way = it->second;
    _validate(way);
  }

  // Can't remember right now if relations are ever manually matched...maybe?
  const RelationMap& relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    ConstRelationPtr relation = it->second;
    _validate(relation);
  }
}

void ManualMatchValidator::_validate(const ConstElementPtr& element)
{
  _numAffected++;

  // Just recording one error for each element, even if there are multiple.

  const Tags& tags = element->getTags();
  QString ref1;
  Tags::const_iterator tagRef1Itr = tags.find(MetadataTags::Ref1());
  if (tagRef1Itr != tags.end())
  {
    ref1 = tagRef1Itr.value().trimmed().toLower();
    // values can't be blank
    if (ref1.isEmpty())
    {
      _errors[element->getElementId()] = "Empty REF1 tag";
      return;
    }
  }
  QString ref2;
  Tags::const_iterator tagRef2Itr = tags.find(MetadataTags::Ref2());
  if (tagRef2Itr != tags.end())
  {
    ref2 = tagRef2Itr.value().trimmed().toLower();
    if (ref2.isEmpty())
    {
      _errors[element->getElementId()] = "Empty REF2 tag";
      return;
    }
  }
  QString review;
  Tags::const_iterator tagReviewItr = tags.find(MetadataTags::Review());
  if (tagReviewItr != tags.end())
  {
    review = tagReviewItr.value().trimmed().toLower();
    if (review.isEmpty())
    {
      _errors[element->getElementId()] = "Empty REVIEW tag";
      return;
    }
  }

  // can't have both ref1 and ref2/review on the same element
  if (!ref1.isEmpty() && (!ref2.isEmpty() || !review.isEmpty()))
  {
    _errors[element->getElementId()] = "Element has both REF1 and either a REF2 or REVIEW tag";
  }
  // an unknown1 element can't have a ref2 or review tag
  else if (element->getStatus() == Status::Unknown1 && (!ref2.isEmpty() || !review.isEmpty()))
  {
    _errors[element->getElementId()] = "Unknown1 element with REF2 or REVIEW tag";
  }
  // an unknown2 element can't have a ref1 tag
  else if (element->getStatus() == Status::Unknown2 && !ref1.isEmpty())
  {
    _errors[element->getElementId()] = "Unknown2 element with REF1 tag";
  }
  // make sure a ref1 exists for each ref2 or review
  else if (!ref2.isEmpty() && !_ref1Mappings.getIdToTagValueMappings().values().contains(ref2))
  {
    _errors[element->getElementId()] = "No REF1 exists for REF2=" + ref2;
  }
  else if (!review.isEmpty() && !_ref1Mappings.getIdToTagValueMappings().values().contains(review))
  {
    _errors[element->getElementId()] = "No REF1 exists for REVIEW=" + review;
  }
  // validate manual match ids
  else if (!ref1.isEmpty() && !_isValidRef1Id(ref1))
  {
    _errors[element->getElementId()] = "Invalid REF1: " + ref1;
  }
  else if (!ref2.isEmpty() && !_isValidRef2OrReviewId(ref2))
  {
    _errors[element->getElementId()] = "Invalid REF2: " + ref2;
  }
  else if (!review.isEmpty() && !_isValidRef2OrReviewId(review))
  {
    _errors[element->getElementId()] = "Invalid REVIEW: " + review;
  }
  // same id can't be on both ref2 and review for the same element
  else if (!ref2.isEmpty() && !review.isEmpty() && ref2 == review && ref2 != "todo" &&
           ref2 != "none")
  {
    _errors[element->getElementId()] = "Invalid REF2=" + ref2 + " or REVIEW=" + review;
  }
}

bool ManualMatchValidator::_isValidRef2OrReviewId(const QString& matchId) const
{
  const QString matchIdTemp = matchId.toLower();
  return
    matchIdTemp == "todo" || matchIdTemp == "none" || StringUtils::isAlphaNumeric(matchIdTemp);
}

bool ManualMatchValidator::_isValidRef1Id(const QString& matchId) const
{
  return StringUtils::isAlphaNumeric(matchId);
}

}
