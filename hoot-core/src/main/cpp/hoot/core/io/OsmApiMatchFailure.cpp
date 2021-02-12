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
 * @copyright Copyright (C) 2020 Maxar (http://www.maxar.com/)
 */

#include "OsmApiMatchFailure.h"

namespace hoot
{

OsmApiMatchFailure::OsmApiMatchFailure()
  :
    //  Placeholder node not found for reference -145213 in way -5687
    //  Placeholder Way not found for reference -12257 in Relation -51
    _placeholderFailure(
      "Placeholder (node|way|relation) not found for reference (-?[0-9]+) in (node|way|relation) (-?[0-9]+)",
      QRegularExpression::CaseInsensitiveOption),
    //  Relation with id  cannot be saved due to Relation with id 1707699
    _relationFailure(
      "Relation with id (-?[0-9]+)? cannot be saved due to (nodes|way|relation) with id (-?[0-9]+)",
      QRegularExpression::CaseInsensitiveOption),
    //  Relation with id -2 requires the relations with id in 1707148,1707249, which either do not exist, or are not visible.
    _multiElementFailure(
      "(Relation|Way) (-?[0-9]+) requires the (nodes|ways|relations) with id in ((-?[0-9]+,)+) (.*)",
      QRegularExpression::CaseInsensitiveOption),
    //  Precondition failed: Node 55 is still used by ways 123
    _deletePreconditionFailure(
      "Precondition failed: (?:The )?(Node|Way|Relation) (-?[0-9]+) is (?:still )?used (?:by|in) (node|way|relation)s ((-?[0-9]+,?)+)",
      QRegularExpression::CaseInsensitiveOption),
    //  Version mismatch: Provided 2, server had: 1 of Node 4869875616
    _conflictVersionFailure(
      "Version mismatch: Provided ([0-9]+), server had: ([0-9]+) of (Node|Way|Relation) ([0-9]+)",
      QRegularExpression::CaseInsensitiveOption),
    //  Changeset conflict: The changeset 49514098 was closed at 2020-01-08 16:28:56 UTC
    _changesetClosedFailure(
      ".*The changeset ([0-9]+) was closed.*",
      QRegularExpression::CaseInsensitiveOption),
    //  Gone: The node with the id 12345 has already been deleted
    _elementGoneDeletedFailure(
      ".*The (node|way|relation) with the id ([0-9]+) has already been deleted",
      QRegularExpression::CaseInsensitiveOption)
{
  _placeholderFailure.optimize();
  _relationFailure.optimize();
  _multiElementFailure.optimize();
  _deletePreconditionFailure.optimize();
  _conflictVersionFailure.optimize();
  _changesetClosedFailure.optimize();
  _elementGoneDeletedFailure.optimize();
}

bool OsmApiMatchFailure::matchesPlaceholderFailure(
    const QString& hint,
    long& member_id, ElementType::Type& member_type,
    long& element_id, ElementType::Type& element_type) const
{
  //  Placeholder node not found for reference -145213 in way -5687
  //  Placeholder Way not found for reference -12257 in Relation -51
  QRegularExpressionMatch match = _placeholderFailure.match(hint);
  if (match.hasMatch())
  {
    //  Get the node/way/relation type and id that caused the failure
    member_type = ElementType::fromString(match.captured(1).toLower());
    bool success = false;
    member_id = match.captured(2).toLong(&success);
    if (!success)
      return success;
    //  Get the node/way/relation type and id that failed
    element_type = ElementType::fromString(match.captured(3));
    element_id = match.captured(4).toLong(&success);
    return success;
  }
  return false;
}

bool OsmApiMatchFailure::matchesRelationFailure(
    const QString& hint,
    long& element_id,
    long& member_id, ElementType::Type& member_type) const
{
  //  Relation with id  cannot be saved due to Relation with id 1707699
  QRegularExpressionMatch match = _relationFailure.match(hint);
  if (match.hasMatch())
  {
    QString error = match.captured(1);
    if (error != "")
      element_id = error.toLong();
    //  Get the node/way/relation type and id that failed
    member_type = ElementType::fromString(match.captured(2));
    bool success = false;
    member_id = match.captured(3).toLong(&success);
    return success;
  }
  return false;
}

bool OsmApiMatchFailure::matchesMultiElementFailure(
    const QString& hint,
    long& element_id, ElementType::Type& element_type,
    std::vector<long>& member_ids, ElementType::Type& member_type) const
{
  //  Relation with id -2 requires the relations with id in 1707148,1707249, which either do not exist, or are not visible.
  QRegularExpressionMatch match = _multiElementFailure.match(hint);
  if (match.hasMatch())
  {
    element_type = ElementType::fromString(match.captured(1));
    QString error = match.captured(2);
    if (error != "")
      element_id = error.toLong();
    //  Get the node/way/relation type (remove the 's') and id that failed
    member_type = ElementType::fromString(match.captured(3).left(match.captured(3).length() - 1));
    bool success = false;
    QStringList ids = match.captured(4).split(",", QString::SkipEmptyParts);
    for (int i = 0; i < ids.size(); ++i)
    {
      long id = ids[i].toLong(&success);
      if (success)
        member_ids.push_back(id);
    }
    return success;
  }
  return false;
}

bool OsmApiMatchFailure::matchesChangesetDeletePreconditionFailure(
    const QString& hint,
    long& element_id, ElementType::Type& element_type,
    std::vector<long>& member_ids, ElementType::Type& member_type) const
{
  //  Precondition failed: Node 55 is still used by ways 123
  //  Precondition failed: The relation 2203673 is used in relations 1590286,1897938
  QRegularExpressionMatch match = _deletePreconditionFailure.match(hint);
  if (match.hasMatch())
  {
    bool success = false;
    element_type = ElementType::fromString(match.captured(1).toLower());
    element_id = match.captured(2).toLong(&success);
    if (!success)
      return success;
    member_type = ElementType::fromString(match.captured(3).toLower());
    QStringList ids = match.captured(4).split(",", QString::SkipEmptyParts);
    for (int i = 0; i < ids.size(); ++i)
    {
      long id = ids[i].toLong(&success);
      if (success)
        member_ids.push_back(id);
    }
    return success;
  }
  return false;
}

bool OsmApiMatchFailure::matchesChangesetConflictVersionMismatchFailure(
    const QString& hint,
    long& element_id, ElementType::Type& element_type,
    long& version_old, long& version_new) const
{
  //  Version mismatch: Provided 2, server had: 1 of Node 4869875616
  QRegularExpressionMatch match = _conflictVersionFailure.match(hint);
  if (match.hasMatch())
  {
    bool success = false;
    version_old = match.captured(1).toLong(&success);
    if (!success)
      return success;
    version_new = match.captured(2).toLong(&success);
    if (!success)
      return success;
    element_type = ElementType::fromString(match.captured(3).toLower());
    element_id = match.captured(4).toLong(&success);
    return success;
  }
  return false;
}

bool OsmApiMatchFailure::matchesChangesetClosedFailure(const QString& hint) const
{
  //  Changeset conflict: The changeset 49514098 was closed at 2020-01-08 16:28:56 UTC
  QRegularExpressionMatch match = _changesetClosedFailure.match(hint);
  return match.hasMatch();
}

bool OsmApiMatchFailure::matchesElementGoneDeletedFailure(
    const QString& hint,
    long& element_id, ElementType::Type& element_type) const
{
  //  The node with the id 12345 has already been deleted
  QRegularExpressionMatch match = _elementGoneDeletedFailure.match(hint);
  if (match.hasMatch())
  {
    bool success = false;
    element_type = ElementType::fromString(match.captured(1).toLower());
    element_id = match.captured(2).toLong(&success);
    return success;
  }
  return false;
}

}
