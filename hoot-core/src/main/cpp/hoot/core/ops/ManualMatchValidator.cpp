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

  // Get all the REF1 values, as we'll need them when examining REF2/REVIEW tags.
  map->visitRo(_ref1Mappings);

  const NodeMap& nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    _validate(*it);
  }

  const WayMap& ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    _validate(*it);
  }

  // TODO: Do we need to parse relations?
  const RelationMap& relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    _validate(*it);
  }
}

void ManualMatchValidator::_validate(const ConstElementPtr& element)
{
  _numAffected++;

  const Tags& tags = element->getTags();
  Tags::const_iterator tagRef1Itr = tags.find(MetadataTags::Ref1());
  Tags::const_iterator tagRef2Itr = tags.find(MetadataTags::Ref2());
  Tags::const_iterator tagReviewItr = tags.find(MetadataTags::Review());

  // an unknown1 element can never have a ref2 or review tag
  if (element->getStatus() == Status::Unknown1 &&
      (tagRef2Itr != tags.end() || tagReviewItr != tags.end()))
  {
    _errors[element->getElementId].append("Unknown1 element with REF2 or REVIEW tag");
  }
  // an unknown2 element can never have a ref1 tag
  else if (element->getStatus() == Status::Unknown2 && tagRef1Itr != tags.end())
  {
    _errors[element->getElementId].append("Unknown2 element with REF1 tag");
  }
  // make sure a REF1 exists for each REF2 or REVIEW hex
  else if (tagRef2Itr != tags.end() && !_ref1Mappings.contains(tagRef2Itr.value()))
  {
    _errors[element->getElementId].append("No REF1 exists for REF2=" + tagRef2Itr.value());
  }
  else if (tagReviewItr != tags.end() && !_ref1Mappings.contains(tagReviewItr.value()))
  {
    _errors[element->getElementId].append("No REF1 exists for REVIEW=" + tagReviewItr.value());
  }
  // validate REF2 and REVIEW tag values (hex, todo, or none)
  else if (tagRef2Itr != tags.end() && _isValidManualMatchId(tagRef2Itr.value()))
  {
    _errors[element->getElementId].append("Invalid REF2: " + tagRef2Itr.value());
  }
  else if (tagReviewItr != tags.end() && _isValidManualMatchId(tagReviewItr.value()))
  {
    _errors[element->getElementId].append("Invalid REVIEW: " + tagReviewItr.value());
  }
  // same ID can't be on both REF2 and REVIEW for the same element
  else if (tagRef2Itr != tags.end() && tagReviewItr != tags.end() &&
           tagRef2Itr.value().toLower() == tagReviewItr.value().toLower())
  {
    _errors[element->getElementId].append("Invalid REVIEW: " + tagReviewItr.value());
  }
}

bool ManualMatchValidator::_isValidManualMatchId(const QString& matchId) const
{
  return
    matchId.toLower() == "todo" || matchId.toLower() == "none" ||
    StringUtils::isAlphaNumeric(matchId);
}

}
