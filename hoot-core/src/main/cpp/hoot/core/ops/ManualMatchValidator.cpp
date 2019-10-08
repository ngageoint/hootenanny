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
  _ref1Mappings.getIdToTagValueMappings().clear();
  _ref1Mappings.setTagKey(MetadataTags::Ref1());

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
  // Just recording one error for each element for performance reasons, even if there are multiple.

  const Tags& tags = element->getTags();

  // if the tag key for the id exists, it can't be empty
  QString ref1;
  Tags::const_iterator tagRef1Itr = tags.find(MetadataTags::Ref1());
  if (tagRef1Itr != tags.end())
  {
    ref1 = tagRef1Itr.value().trimmed().toLower();
    if (ref1.isEmpty())
    {
      _errors[element->getElementId()] = "Empty REF1 tag";
      return;
    }
  }
  LOG_VART(ref1);

  QStringList ref2;
  Tags::const_iterator tagRef2Itr = tags.find(MetadataTags::Ref2());
  if (tagRef2Itr != tags.end())
  {
    ref2 = tagRef2Itr.value().trimmed().toLower().split(";");
    if (ref2.isEmpty() || (ref2.size() == 1 && ref2.at(0).isEmpty()))
    {
      _errors[element->getElementId()] = "Empty REF2 tag";
      return;
    }
  }
  LOG_VART(ref2.size());

  QStringList review;
  Tags::const_iterator tagReviewItr = tags.find(MetadataTags::Review());
  if (tagReviewItr != tags.end())
  {
    review = tagReviewItr.value().trimmed().toLower().split(";");
    if (review.isEmpty() || (review.size() == 1 && review.at(0).isEmpty()))
    {
      _errors[element->getElementId()] = "Empty REVIEW tag";
      return;
    }
  }
  LOG_VART(review.size());

  // REF2 and review can be multiple IDs (many to one match), but REF1 is always a single ID.
  if (ref1.split(";").size() > 1)
  {
    _errors[element->getElementId()] = "REF1 ID must be singular. REF1=" + ref1;
  }
  // validate manual match ids
  else if (!ref1.isEmpty() && !_isValidRef1Id(ref1))
  {
    _errors[element->getElementId()] = "Invalid REF1=" + ref1;
  }
  // can't have both ref1 and ref2/review on the same element
  else if (!ref1.isEmpty() && (!ref2.isEmpty() || !review.isEmpty()))
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
  // If a ref2 or review has multiple ID's, they should all be hex.
  else if (!ref2.isEmpty() && ref2.size() > 1 && (ref2.contains("todo") || ref2.contains("none")))
  {
    _errors[element->getElementId()] = "Invalid many to one REF2=" + ref2.join(";");
  }
  else if (!review.isEmpty() && review.size() > 1 &&
           (review.contains("todo") || review.contains("none")))
  {
    _errors[element->getElementId()] = "Invalid many to one REVIEW=" + review.join(";");
  }
  // check for dupes
  else if (!ref2.isEmpty() && ref2.toSet().size() != ref2.size() )
  {
    _errors[element->getElementId()] = "Duplicate ID found in REF2=" + ref2.join(";");
  }
  else if (!review.isEmpty() && review.toSet().size() != review.size())
  {
    _errors[element->getElementId()] = "Duplicate ID found in REVIEW=" + review.join(";");
  }
  else if (!ref2.isEmpty())
  {
    for (int i = 0; i < ref2.size(); i++)
    {
      const QString ref2Id = ref2.at(i).trimmed().toLower();

      // check for empty
      if (ref2Id.isEmpty())
      {
        _errors[element->getElementId()] = "Empty REF2 tag";
        break;
      }
      // make sure a each ref2 id is valid; no need to check for 'todo' or 'none' here, as
      // we've previously handled that
      else if (!_isValidRef2OrReviewId(ref2Id))
      {
        _errors[element->getElementId()] = "Invalid REF2=" + ref2Id;
        break;
      }
      // make sure a ref1 exists for each ref2
      else if (ref2Id != "none" && ref2Id != "todo" &&
               !_ref1Mappings.getIdToTagValueMappings().values().contains(ref2Id))
      {
        _errors[element->getElementId()] = "No REF1 exists for REF2=" + ref2Id;
        break;
      }
      // same id can't be on both ref2 and review for the same element
      else if (!review.isEmpty() && ref2Id != "todo" && ref2Id != "none" && review.contains(ref2Id))
      {
        _errors[element->getElementId()] =
          "Invalid repeated ID: REF2=" + ref2Id + ", REVIEW=" + ref2Id;
        break;
      }
    }
  }
  // same as previous, except for review
  else if (!review.isEmpty())
  {
    for (int i = 0; i < review.size(); i++)
    {
      const QString reviewId = review.at(i).trimmed().toLower();

      if (reviewId.isEmpty())
      {
        _errors[element->getElementId()] = "Empty REVIEW tag";
        break;
      }
      else if (!_isValidRef2OrReviewId(reviewId))
      {
        _errors[element->getElementId()] = "Invalid REVIEW=" + reviewId;
        break;
      }
      else if (reviewId != "none" && reviewId != "todo" &&
               !_ref1Mappings.getIdToTagValueMappings().values().contains(reviewId))
      {
        _errors[element->getElementId()] = "No REF1 exists for REVIEW=" + reviewId;
        break;
      }
    }
  }

  _numAffected++;
}

// TODO: something to validate actual hex here instead?

bool ManualMatchValidator::_isValidRef2OrReviewId(const QString& matchId) const
{
  return matchId == "todo" || matchId == "none" || StringUtils::isAlphaNumeric(matchId);
}

bool ManualMatchValidator::_isValidRef1Id(const QString& matchId) const
{
  return matchId != "todo" && matchId != "none" && StringUtils::isAlphaNumeric(matchId);
}

}
