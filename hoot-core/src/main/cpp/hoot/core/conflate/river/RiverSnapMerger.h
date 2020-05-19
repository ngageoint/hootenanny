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
#ifndef RIVER_SNAP_MERGER_H
#define RIVER_SNAP_MERGER_H

// Hoot
#include <hoot/core/conflate/highway/HighwaySnapMerger.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

class WaySublineMatchString;

/**
 * Merges river features
 *
 * This class primarily exists so that we can select a subline matcher based on properties of the
 * input data for runtime performance reasons.
 */
class RiverSnapMerger : public HighwaySnapMerger, public Configurable
{

public:

  static std::string className() { return "hoot::RiverSnapMerger"; }

  RiverSnapMerger();
  RiverSnapMerger(
    const std::set<std::pair<ElementId, ElementId>>& pairs,
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher,
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher2);

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

  /**
   * Determines if either of two features is considered "long" by the standards of River Conflation.
   * Both way length and node count are examined.
   *
   * Note that relations are allowed here to despite the fact relation isn't an element type
   * conflatable by River Conflation. This is due to the fact that relations are passed to matching
   * during match conflict resolution and trying to prevent that causes regression test failures
   * for an unknown reason. See comments in ScriptMatch::_isOrderedConflicting.
   *
   * @param map map owning the input elements
   * @param element1 the first input element
   * @param element2 the second input element
   * @return true if either of the elements is considered "long"; false otherwise
   */
  bool isLongPair(ConstOsmMapPtr map, ConstElementPtr element1, ConstElementPtr element2);

  virtual QString getDescription() const { return "Merges rivers"; }

  virtual QString getName() const { return QString::fromStdString(className()); }

protected:

  /*
   * @see HighwaySnapMerger
   */
  virtual WaySublineMatchString _matchSubline(OsmMapPtr map, ElementPtr e1, ElementPtr e2);

private:

  // way node count above which an element is considered "long"
  int _nodeCountThreshold;
  // way length above  which an elementis considered "long"
  int _lengthThreshold;

  // This is our backup matcher to use for long ways for runtime performance reasons. It may be
  // a little less accurate but prevents extremely long ways from slowing things down too much.
  std::shared_ptr<SublineStringMatcher> _sublineMatcher2;
};

typedef std::shared_ptr<RiverSnapMerger> RiverSnapMergerPtr;

}

#endif // HIGHWAYSNAPMERGER_H
