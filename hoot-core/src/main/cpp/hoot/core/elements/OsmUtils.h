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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OSM_UTILS_H
#define OSM_UTILS_H

// Hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Utilities for use with the OSM data.
 *
 * Put any Element related utilities here that either don't fit into any of the existing OSM related
 * utils classes, would clutter up any of the core Element classes, or are isolated enough not to
 * warrant making a new utils class for them yet.
 */
class OsmUtils
{
public:

  /**
   * Get a more detailed string representation of a relation
   *
   * @param relation relation to get info from
   * @param map map owning the relation
   * @return a detailed relation string
   */
  static QString getRelationDetailedString(const ConstRelationPtr& relation,
                                           const ConstOsmMapPtr& map);

  /**
   * Constructs a detailed string for an element suitable for trace logging
   *
   * @param element the element to create a string for
   * @param map map owning the element
   * @return a string describing the element
   */
  static QString getElementDetailString(const ConstElementPtr& element, const ConstOsmMapPtr& map);

  /**
   *  Constructs a detailed string for collection of elements suitable for trace logging
   *
   * @param elements the elements to create a string for
   * @param map map owning the elements
   * @return a string describing the elements
   */
  static QString getElementsDetailString(const std::vector<ElementPtr>& elements,
                                         const ConstOsmMapPtr& map);

  /**
   * Determines if two elements have conflicting one way street tags
   *
   * @param element1 the first element to examine
   * @param element2 the second element to examine
   * @return true if their one way tags conflict; false otherwise
   */
  static bool oneWayConflictExists(const ConstElementPtr& element1, const ConstElementPtr& element2);

  /**
   * Determines if a way has an explicitly negative one way tag (oneway=no, etc.)
   *
   * @param element the element to examine
   * @return true if the element contains a tag indicating it is not a one way street; false
   * otherwise
   */
  static bool explicitlyNotAOneWayStreet(const ConstElementPtr& element);

  /**
   * Determines if two elements have conflicting name tags
   *
   * @param element1 the first element to examine
   * @param element2 the second element to examine
   * @return true if their name tags conflict; false otherwise
   */
  static bool nameConflictExists(const ConstElementPtr& element1, const ConstElementPtr& element2);

  /**
   * Determines if there are specific highway type conflicts between elements
   *
   * @param element1 the first element to examine
   * @param element2 the second element to examine
   * @return true if both have specific highway tags (other than highway=road) and they disagree;
   * false otherwise
   */
  static bool nonGenericHighwayConflictExists(const ConstElementPtr& element1,
                                              const ConstElementPtr& element2);

  /**
   * Determines if one element a child of another; e.g. way node or relation memeber
   *
   * @param parent the parent element
   * @param memberId the element ID of the child
   * @return true if parent has the element with memberId as a child; false otherwise
   */
  static bool containsMember(const ConstElementPtr& parent, const ElementId& memberId);

  /**
   * Determines if an element is contained by any way or relation in a map
   *
   * @param elementId the ID of the element to search for
   * @param map the map containing the element
   * @return true if any way or relation in the map contains the element; false otherwise
   */
  static bool isChild(const ElementId& elementId, const ConstOsmMapPtr& map);
};

}

#endif // OSM_UTILS_H
