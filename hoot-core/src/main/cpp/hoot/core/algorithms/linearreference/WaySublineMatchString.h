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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef WAYSUBLINEMATCHSTRING_H
#define WAYSUBLINEMATCHSTRING_H


// Hoot
#include <hoot/core/algorithms/linearreference/WaySublineMatch.h>
#include <hoot/core/elements/OsmMap.h>

// Std



namespace hoot
{

class WaySublineCollection;

/**
 * @brief The WaySublineMatchString class contains multiple WaySublineMatches. The matches must not
 * overlap.
 *
 * @todo this is a brittle way of handling the problem. Changing any way will break the matches.
 * In the future we should entertain switching to WayMatchStringMapping.
 */
class WaySublineMatchString
{
public:

  static QString className() { return "hoot::WaySublineMatchString"; }

  using MatchCollection = std::vector<WaySublineMatch>;

  WaySublineMatchString() = default;
  /**
   * @brief WaySublineMatchString Constructor - Makes a new WaySublineMatchString where all the
   * WayLocations are remapped to reference the new map.
   */
  WaySublineMatchString(const WaySublineMatchString& other, const OsmMapPtr& newMap);
  WaySublineMatchString(const MatchCollection& m);

  /**
   * @brief contains returns true if one or more sublines in all the matches are contained by this
   * WaySublineMatchString. This is most useful when calculating conflicts.
   */
  bool contains(const WaySublineMatchString& other) const;
  /**
   * @brief contains returns true if one or more of the sublines in the match are contained by this
   * WaySublineMatchString. This is most useful when calculating conflicts.
   */
  bool contains(const WaySublineMatch& other) const;

  /**
   * @brief getLength returns the mean of the length of getSublineString1().getLength() and
   * getSublineString2().getLength().
   */
  Meters getLength() const;

  const MatchCollection& getMatches() const { return _matches; }

  // TODO: move reverse vector into the way subline string, but keep the interface identical
  std::vector<bool> getReverseVector1() const;
  std::vector<bool> getReverseVector2() const;

  /**
   * @brief getSublineString1 returns the string of sublines that represent the first match.
   */
  WaySublineCollection getSublineString1() const;
  /**
   * @brief getSublineString2 returns the string of sublines that represent the second match.
   */
  WaySublineCollection getSublineString2() const;

  /**
   * @brief isEmpty returns true if there is no match.
   */
  bool isEmpty() const { return _matches.size() == 0; }

  /**
   * @brief isValid returns true if there is more than one match and they're all non-zero length.
   */
  bool isValid() const;

  /**
   * @brief removeEmptyMatches really shouldn't be necessary. It removes any matches between
   * zero-length ways and other ways. See Redmine #4593
   */
  void removeEmptyMatches();

  /**
   * @brief touches returns true if any part of this matches other.
   */
  bool touches(const WaySublineMatchString& other) const;

  QString toString() const;

private:

  MatchCollection _matches;
};

HOOT_DEFINE_EXCEPTION(OverlappingMatchesException)

using WaySublineMatchStringPtr = std::shared_ptr<WaySublineMatchString>;
using ConstWaySublineMatchStringPtr = std::shared_ptr<const WaySublineMatchString>;

}

#endif // WAYSUBLINEMATCHSTRING_H
