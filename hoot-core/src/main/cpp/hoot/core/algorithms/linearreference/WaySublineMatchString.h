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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef WAYSUBLINEMATCHSTRING_H
#define WAYSUBLINEMATCHSTRING_H

#include "WaySublineMatch.h"
#include "WaySublineCollection.h"

namespace hoot
{

/**
 * Contains multiple WaySublineMatches. The matches must not overlap.
 *
 * @todo this is a brittle way of handling the problem. Changing any way will break the matches.
 * In the future we should entertain switching to WayMatchStringMapping.
 */
class WaySublineMatchString
{
public:
  static std::string className() { return "hoot::WaySublineMatchString"; }

  typedef vector<WaySublineMatch> MatchCollection;

  WaySublineMatchString() {}

  WaySublineMatchString(const WaySublineMatchString& other);

  /**
   * Makes a new WaySublineMatchString where all the WayLocations are remapped to reference the
   * new map.
   */
  WaySublineMatchString(const WaySublineMatchString& other, const OsmMapPtr& newMap);

  WaySublineMatchString(const MatchCollection& m);

  /**
   * Returns true if one or more sublines in all the matches are contained by this
   * WaySublineMatchString. This is most useful when calculating conflicts.
   */
  bool contains(const WaySublineMatchString& other) const;

  /**
   * Returns true if one or more of the sublines in the match are contained by this
   * WaySublineMatchString. This is most useful when calculating conflicts.
   */
  bool contains(const WaySublineMatch& other) const;

  /**
   * Returns the mean of the length of getSublineString1().getLength() and
   * getSublineString2().getLength().
   */
  Meters getLength() const;

  const MatchCollection& getMatches() const { return _matches; }

  /// @todo move reverse vector into the way subline string, but keep the interface identical
  vector<bool> getReverseVector1() const;
  vector<bool> getReverseVector2() const;

  /**
   * Returns the string of sublines that represent the first match.
   */
  WaySublineCollection getSublineString1() const;

  /**
   * Returns the string of sublines that represent the second match.
   */
  WaySublineCollection getSublineString2() const;

  /**
   * Returns true if there is no match.
   */
  bool isEmpty() const { return _matches.size() == 0; }

  /**
   * Returns true if there is more than one match and they're all non-zero length.
   */
  bool isValid() const;

  /**
   * This method really shouldn't be necessary. It removes any matches between zero-length ways
   * and other ways. See #4593
   */
  void removeEmptyMatches();

  /**
   * Returns true if any part of this matches other.
   */
  bool touches(const WaySublineMatchString& other) const;

  QString toString() const
  {
    QString result;
    result += QString("matches:\n");
    QStringList l;
    for (size_t i = 0; i < _matches.size(); i++)
    {
      l.append(_matches[i].toString());
    }
    result += l.join("\n");
    return result;
  }

private:
  MatchCollection _matches;
};

HOOT_DEFINE_EXCEPTION(OverlappingMatchesException)

typedef boost::shared_ptr<WaySublineMatchString> WaySublineMatchStringPtr;
typedef boost::shared_ptr<const WaySublineMatchString> ConstWaySublineMatchStringPtr;

}

#endif // WAYSUBLINEMATCHSTRING_H
