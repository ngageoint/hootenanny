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

#ifndef RELATION_MEMBER_UTILS_H
#define RELATION_MEMBER_UTILS_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/GeometryTypeCriterion.h>

namespace hoot
{

class ElementCriterion;

/**
 * Various utilities useful when working with relation members but kept out of the Relation class
 * to avoid circular dependencies
 */
class RelationMemberUtils
{

public:

  /**
   * Get a detailed string representing a relation's members
   *
   * @param relation relation to get info from
   * @param map map owning the relation
   * @return a detailed relations members string
   */
  static QString getRelationMembersDetailedString(const ConstRelationPtr& relation,
                                                  const ConstOsmMapPtr& map);

  /**
   * Returns the first way ID from a set of relation members
   *
   * @param relation relation to check way ID for
   * @param map map owning the relation
   * @return a way ID
   */
  static long getFirstWayIdFromRelation(const ConstRelationPtr& relation, const OsmMapPtr& map);

  /**
   * Determines if an element is a member of a relation
   *
   * @param map map owning the input element
   * @param childId element ID of the input element
   * @return true if the element is a member of a relation
   */
  static bool isMemberOfRelation(const ConstOsmMapPtr& map, const ElementId& childId);

  /**
   * Determines if an element is a member of a relation of a given type
   *
   * @param map map owning the input element
   * @param childId element ID of the input element
   * @param relationType type of relation to search for; if left blank, relation types are not
   * checked
   * @return true if the element is a member of a relation of the specified type OR if the element
   * is simply a member of a relation and the type is not specified; false otherwise
   */
  static bool isMemberOfRelationType(const ConstOsmMapPtr& map, const ElementId& childId,
                                     const QString& relationType = "");

  /**
   * Determines if an element is a member of a relation that is in a given schema category
   *
   * @param map map owning the input element
   * @param childId element ID of the input element
   * @param schemaCategory schema category to search for relation membership in; if left blank,
   * category membership for the relations is not checked
   * @return true if the element is a member of a relation that is in the specified schema category
   * OR if the element is simply a member of a relation and the category is not specified; false
   * otherwise
   */
  static bool isMemberOfRelationInCategory(const ConstOsmMapPtr& map, const ElementId& childId,
                                           const QString& schemaCategory = "");

  /**
   * Determines if an element is a member of a relation that has a given tag key
   *
   * @param map map owning the input element
   * @param childId element ID of the input element
   * @param tagKey tag key to search relations for; if left blank, tag keys on the relations are not
   * checked
   * @return true if the element is a member of a relation with the specified tag key OR if the
   * element is simply a member of a relation and a tag key is not specified
   */
  static bool isMemberOfRelationWithTagKey(const ConstOsmMapPtr& map, const ElementId& childId,
                                           const QString& tagKey = "");

  /**
   * Determines if an element is contained by any relation in a map
   *
   * @param elementId the ID of the element to search for
   * @param map the map containing the element
   * @return true if any relation in the map contains the element; false otherwise
   */
  static bool elementContainedByAnyRelation(const ElementId& elementId, const ConstOsmMapPtr& map);

  /**
   * Determines if a relation contains any member that satisifies a specified criterion
   *
   * @param relation the relation to examine
   * @param criterion the criterion which must be satisified by member elements
   * @param map the map owning the relation
   * @return true if at least one member element satisfies the criterion; false otherwise
   */
  static bool containsMemberWithCriterion(
    const ConstRelationPtr& relation, const ElementCriterion& criterion, const ConstOsmMapPtr& map);

  /**
   * Determines if a relation contains only members that satisfy specified criterion
   *
   * @param relation the relation to examine
   * @param criterion the criterion which must be satisified by member elements
   * @param map the map owning the relation
   * @return true if all member elements satisfy the criterion; false otherwise
   */
  static bool containsOnlyMembersWithCriterion(
    const ConstRelationPtr& relation, const ElementCriterion& criterion, const ConstOsmMapPtr& map);

  /**
   * Detemines if a relation has members all with the same geometry type
   *
   * @param relation the relation to examine
   * @param map the map owning the relation
   * @param geometryType optional geometry type filter
   * @return if no geometry type is specified, returns true if all member elements have the same
   * geometry type and false otherwise; if a geometry type is specified, returns true if all
   * member elements have the specified geometry type and false otherwise
   */
  static bool membersHaveHomogenousGeometryType(
    const ConstRelationPtr& relation, const ConstOsmMapPtr& map,
    const GeometryTypeCriterion::GeometryType& geometryType =
      GeometryTypeCriterion::GeometryType::Unknown);

  /**
   * Determines if a relation has any member with a specified geometry type
   *
   * @param relation the relation to examine
   * @param map the map owning the relation
   * @param geometryType geometry type filter
   * @return true if the relation has any member with the specified geometry type; false otherwise
   */
  static bool hasMemberWithGeometryType(
    const ConstRelationPtr& relation, const ConstOsmMapPtr& map,
    const GeometryTypeCriterion::GeometryType& geometryType);

  /**
   * Counts the number of way nodes in way members belonging to a relation
   *
   * @param relation the relation containing the way members
   * @param map the map owning the relation
   * @return a count
   */
  static int getMemberWayNodeCount(const ConstRelationPtr& relation, const ConstOsmMapPtr& map);
};

}

#endif // RELATION_MEMBER_UTILS_H
