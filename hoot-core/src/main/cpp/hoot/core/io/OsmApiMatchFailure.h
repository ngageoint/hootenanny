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

#ifndef OSM_API_MATCH_FAILURE_H
#define OSM_API_MATCH_FAILURE_H

//  Qt
#include <QString>

//  Standard
#include <vector>

//  Hoot
#include <hoot/core/io/OsmApiChangesetElement.h>

namespace hoot
{

class OsmApiMatchFailure
{
public:
  /** Constructor */
  OsmApiMatchFailure();
  /**
   * @brief matchesPlaceholderFailure Checks the return from the API to see if it is similar to the following error message:
   *        "Placeholder node not found for reference -145213 in way -5687"
   * @param hint Error message from OSM API
   * @param member_id ID of the member element that caused the element to fail
   * @param member_type Type of the member element that caused the element to fail
   * @param element_id ID of the element that failed
   * @param element_type Type of the element that failed
   * @return True if the message matches and was parsed
   */
  bool matchesPlaceholderFailure(const QString& hint,
                                        long& member_id, ElementType::Type& member_type,
                                        long& element_id, ElementType::Type& element_type) const;
  /**
   * @brief matchesRelationFailure Checks the return from the API to see if it is similar to the following error message:
   *        "Relation with id  cannot be saved due to Relation with id 1707699"
   * @param hint Error message from OSM API
   * @param element_id ID of the element that failed
   * @param member_id ID of the member element that caused the element to fail
   * @param member_type Type of the member element that caused the element to fail
   * @return True if the message matches and was parsed
   */
  bool matchesRelationFailure(const QString& hint, long& element_id,
                                     long& member_id, ElementType::Type& member_type) const;
  /**
   * @brief matchesMultiElementFailure Checks the return from the API to see if it is similar to the following error message:
   *        "Relation with id -2 requires the relations with id in 1707148,1707249, which either do not exist, or are not visible."
   * @param hint Error message from OSM API
   * @param element_id ID of the element that failed
   * @param element_type Type of the element that failed
   * @param member_ids IDs of the member elements that caused the element to fail
   * @param member_type Type of the member elements that caused the element to fail
   * @return True if the message matches and was parsed
   */
  bool matchesMultiElementFailure(const QString& hint, long& element_id, ElementType::Type& element_type,
                                         std::vector<long>& member_ids, ElementType::Type& member_type) const;
  /**
   * @brief matchesChangesetDeletePreconditionFailure Checks the return from the API to see if it is similar to the following error message:
   *        "Precondition failed: Node 55 is still used by ways 123"
   * @param hint Error message from OSM API
   * @param element_id ID of the element that failed
   * @param element_type Type of the element that failed
   * @param member_ids IDs of the member elements that caused the element to fail
   * @param member_type Type of the member elements that caused the element to fail
   * @return True if the message matches and was parsed
   */
  bool matchesChangesetDeletePreconditionFailure(const QString& hint,
                                                 long& element_id, ElementType::Type& element_type,
                                                 std::vector<long>& member_ids, ElementType::Type& member_type) const;
  /**
   * @brief matchesChangesetConflictVersionMismatchFailure Checks the return from the API to see if it is similar to the following error message:
   *        "Changeset conflict: Version mismatch: Provided 2, server had: 1 of Node 4869875616"
   * @param hint Error message from OSM API
   * @param member_id ID of the member element that caused the element to fail
   * @param member_type Type of the member element that caused the element to fail
   * @param element_id ID of the element that failed
   * @param element_type Type of the element that failed
   * @return True if the message matches and was parsed
   */
  bool matchesChangesetConflictVersionMismatchFailure(const QString& hint,
                                                      long& element_id, ElementType::Type& element_type,
                                                      long& version_old, long& version_new) const;
  /**
   * @brief matchesChangesetClosed FailureChecks the return from the API to see if it is similar to the following error message:
   *        "Changeset conflict: The changeset 49514098 was closed at 2020-01-08 16:28:56 UTC"
   * @param hint Error message from OSM API
   * @return True if the message matches
   */
  bool matchesChangesetClosedFailure(const QString& hint) const;

private:
  /** Precompiled regular expressions */
  QRegularExpression _placeholderFailure;
  QRegularExpression _relationFailure;
  QRegularExpression _multiElementFailure;
  QRegularExpression _deletePreconditionFailure;
  QRegularExpression _conflictVersionFailure;
  QRegularExpression _changesetClosedFailure;
};

}

#endif  //  OSM_API_MATCH_FAILURE_H
