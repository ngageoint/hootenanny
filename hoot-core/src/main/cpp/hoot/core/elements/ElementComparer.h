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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ELEMENTCOMPARER_H
#define ELEMENTCOMPARER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/OsmMapConsumer.h>

namespace hoot
{

/**
 * This class compares two elements to determine if they are identical.
 *
 * It can be configured to be ID agnostic. When element IDs are ignored, a map is required and
 * way/relation comparisons could potentially be slower than when they are not ignored as each child
 * element must be accessed.
 *
 * The node distance comparison tolerance threshold is controlled via the
 * node.comparison.coordinate.sensitivity configuration option.
 */
class ElementComparer : public OsmMapConsumer
{

public:

  static const long DEBUG_ID = 0;

  ElementComparer();

  /**
   * Determines if two elements are the same
   *
   * The inputs aren't const b/c we auto update elements with a hash if they don't already have one
   * in order to avoid computing the hash more than once.
   *
   * @param e1 the first element to compare
   * @param e2 the second element to compare
   * @return true if they are the same; false otherwise
   */
  bool isSame(ElementPtr e1, ElementPtr e2) const;

  /**
   * @see OsmMapConsumer
   *
   * This only needs to be set if _ignoreElementId = true. The reason a const map isn't used here
   * is for the same reason isSame doesn't take in const elements.
   */
  virtual void setOsmMap(OsmMap* map) { _map = map->shared_from_this(); }

  /**
   * Wrapper around ElementHashVisitor::toHashString
   */
  QString toHashString(const ConstElementPtr& e) const;

  void setIgnoreElementId(bool ignore) { _ignoreElementId = ignore; }
  void setIgnoreVersion(bool ignore) { _ignoreVersion = ignore; }

private:

  // enabling this allows for element comparisons to ignore the element ID; requires a map, so
  // default it to false to support callers that don't have a map
  bool _ignoreElementId;
  // allows for ignoring element versions during comparison
  bool _ignoreVersion;
  // a map is needed when comparing child elements if ignoring element IDs
  OsmMapPtr _map;

  void _setHash(ElementPtr element) const;
  bool _haveSameHash(ElementPtr re, ElementPtr e) const;

  bool _compareNode(ElementPtr re, ElementPtr e) const;
  bool _compareWay(ElementPtr re, ElementPtr e) const;
  bool _compareRelation(ElementPtr re, ElementPtr e) const;
};

}

#endif // ELEMENTCOMPARER_H
