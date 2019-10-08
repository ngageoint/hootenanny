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
      const QString errorMsg = "Empty REF1 tag";
      LOG_VART(errorMsg);
      _errors[element->getElementId()] = errorMsg;
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
      const QString errorMsg = "Empty REF2 tag";
      LOG_VART(errorMsg);
      _errors[element->getElementId()] = errorMsg;
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
      const QString errorMsg = "Empty REVIEW tag";
      LOG_VART(errorMsg);
      _errors[element->getElementId()] = errorMsg;
      return;
    }
  }
  LOG_VART(review.size());

  // REF2 and review can be multiple IDs (many to one match), but REF1 is always a single ID.
  if (ref1.split(";").size() > 1)
  {
    const QString errorMsg = "REF1 ID must be singular. REF1=" + ref1;
    LOG_VART(errorMsg);
    _errors[element->getElementId()] = errorMsg;
  }
  // validate manual match ids
  else if (!ref1.isEmpty() && !_isValidRef1Id(ref1))
  {
     const QString errorMsg = "Invalid REF1=" + ref1;
     LOG_VART(errorMsg);
    _errors[element->getElementId()] = errorMsg;
  }
  // can't have both ref1 and ref2/review on the same element
  else if (!ref1.isEmpty() && (!ref2.isEmpty() || !review.isEmpty()))
  {
    const QString errorMsg = "Element has both REF1 and either a REF2 or REVIEW tag";
    LOG_VART(errorMsg);
    _errors[element->getElementId()] = errorMsg;
  }
  // an unknown1 element can't have a ref2 or review tag
  else if (element->getStatus() == Status::Unknown1 && (!ref2.isEmpty() || !review.isEmpty()))
  {
    const QString errorMsg = "Unknown1 element with REF2 or REVIEW tag";
    LOG_VART(errorMsg);
    _errors[element->getElementId()] = errorMsg;
  }
  // an unknown2 element can't have a ref1 tag
  else if (element->getStatus() == Status::Unknown2 && !ref1.isEmpty())
  {
    const QString errorMsg = "Unknown2 element with REF1 tag";
    LOG_VART(errorMsg);
    _errors[element->getElementId()] = errorMsg;
  }
  // If a ref2 or review has multiple ID's, they should all be hex.
  else if (!ref2.isEmpty() && ref2.size() > 1 &&
           (ref2.contains("todo", Qt::CaseInsensitive) ||
            ref2.contains("none", Qt::CaseInsensitive)))
  {
    const QString errorMsg = "Invalid many to one REF2=" + ref2.join(";");
    LOG_VART(errorMsg);
    _errors[element->getElementId()] = errorMsg;
  }
  else if (!review.isEmpty() && review.size() > 1 &&
           (review.contains("todo") || review.contains("none")))
  {
    const QString errorMsg = "Invalid many to one REVIEW=" + review.join(";");
    LOG_VART(errorMsg);
    _errors[element->getElementId()] = errorMsg;
  }
  // check for dupes
  else if (!ref2.isEmpty() && ref2.toSet().size() != ref2.size() )
  {
    const QString errorMsg = "Duplicate ID found in REF2=" + ref2.join(";");
    LOG_VART(errorMsg);
    _errors[element->getElementId()] = errorMsg;
  }
  else if (!review.isEmpty() && review.toSet().size() != review.size())
  {
    const QString errorMsg = "Duplicate ID found in REVIEW=" + review.join(";");
    LOG_VART(errorMsg);
    _errors[element->getElementId()] = errorMsg;
  }
  else if (!ref2.isEmpty())
  {
    for (int i = 0; i < ref2.size(); i++)
    {
      const QString ref2Id = ref2.at(i);

      if (!_isValidRef2OrReviewId(ref2Id))
      {
        const QString errorMsg = "Invalid REF2=" + ref2Id;
        LOG_VART(errorMsg);
        _errors[element->getElementId()] = errorMsg;
        break;
      }
      // make sure a ref1 exists for each ref2
      else if (!_isValidNonUniqueMatchId(ref2Id) &&
               !_ref1Mappings.getIdToTagValueMappings().values().contains(ref2Id))
      {
        const QString errorMsg = "No REF1 exists for REF2=" + ref2Id;
        LOG_VART(errorMsg);
        _errors[element->getElementId()] = errorMsg;
        break;
      }
      // same id can't be on both ref2 and review for the same element
      else if (!review.isEmpty() && !_isValidNonUniqueMatchId(ref2Id) && review.contains(ref2Id))
      {
        const QString errorMsg = "Invalid repeated ID: REF2=" + ref2Id + ", REVIEW=" + ref2Id;
        LOG_VART(errorMsg);
        _errors[element->getElementId()] = errorMsg;
        break;
      }
    }
  }
  // same as previous, except for review
  else if (!review.isEmpty())
  {
    for (int i = 0; i < review.size(); i++)
    {
      const QString reviewId = review.at(i);

      if (!_isValidRef2OrReviewId(reviewId))
      {
        const QString errorMsg = "Invalid REVIEW=" + reviewId;
        LOG_VART(errorMsg);
        _errors[element->getElementId()] = errorMsg;
        break;
      }
      else if (!_isValidNonUniqueMatchId(reviewId) &&
               !_ref1Mappings.getIdToTagValueMappings().values().contains(reviewId))
      {
        const QString errorMsg = "No REF1 exists for REVIEW=" + reviewId;
        LOG_VART(errorMsg);
        _errors[element->getElementId()] = errorMsg;
        break;
      }
    }
  }

  _numAffected++;
}

bool ManualMatchValidator::_isValidRef2OrReviewId(const QString& matchId) const
{
  const QString matchIdTemp = matchId.trimmed().toLower();
  return
    !matchIdTemp.isEmpty() &&
    (_isValidNonUniqueMatchId(matchIdTemp) || _isValidUniqueMatchId(matchIdTemp));
}

bool ManualMatchValidator::_isValidRef1Id(const QString& matchId) const
{
  const QString matchIdTemp = matchId.trimmed().toLower();
  return
    !matchIdTemp.isEmpty() && !_isValidNonUniqueMatchId(matchIdTemp) &&
    _isValidUniqueMatchId(matchIdTemp);
}

bool ManualMatchValidator::_isValidUniqueMatchId(const QString& matchId) const
{
  // This corresponds with how AddRef1Visitor creates the ids.
  return matchId.size() >= 6 && StringUtils::isAlphaNumeric(matchId.right(6));
}

bool ManualMatchValidator::_isValidNonUniqueMatchId(const QString& matchId) const
{
  const QString matchIdTemp = matchId.toLower();
  return matchIdTemp == "none" || matchIdTemp == "todo";
}

}
