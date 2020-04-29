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
 * TODO
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
   * TODO
   *
   * @param map
   * @param way1
   * @param way2
   * @return
   */
  bool isLongWayPair(ConstOsmMapPtr map, ConstWayPtr way1, ConstWayPtr way2);

  virtual QString getDescription() const { return "TODO"; }

  virtual QString getName() const { return QString::fromStdString(className()); }

protected:

  /*
   * TODO
   */
  virtual WaySublineMatchString _matchSubline(OsmMapPtr map, ElementPtr e1, ElementPtr e2);

private:

  int _nodeCountThreshold;
  int _lengthThreshold;

  std::shared_ptr<SublineStringMatcher> _sublineMatcher2;
};

typedef std::shared_ptr<RiverSnapMerger> RiverSnapMergerPtr;

}

#endif // HIGHWAYSNAPMERGER_H
