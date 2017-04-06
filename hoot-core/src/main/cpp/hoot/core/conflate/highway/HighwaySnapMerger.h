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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef HIGHWAYSNAPMERGER_H
#define HIGHWAYSNAPMERGER_H

#include <hoot/core/conflate/MergerBase.h>

namespace hoot
{
class SublineStringMatcher;
class WaySublineCollection;

class HighwaySnapMerger : public MergerBase
{
public:

  static std::string className() { return "hoot::HighwaySnapMerger"; }

  static unsigned int logWarnCount;

  HighwaySnapMerger(Meters minSplitSize,
    const set< pair<ElementId, ElementId> >& pairs,
    const boost::shared_ptr<SublineStringMatcher>& sublineMatcher);

  virtual void apply(const OsmMapPtr& map, vector< pair<ElementId, ElementId> >& replaced)
    const;

  virtual QString toString() const;

protected:

  virtual PairsSet& getPairs() { return _pairs; }
  virtual const PairsSet& getPairs() const { return _pairs; }

private:

  double _minSplitSize;
  set< pair<ElementId, ElementId> > _pairs;
  boost::shared_ptr<SublineStringMatcher> _sublineMatcher;

  void _addScrapsToMap(const OsmMapPtr& map, vector< pair<ElementId, ElementId> >& replaced,
    ElementId originalId, vector<WayPtr>& scraps) const;

  /**
   * Returns true if the way directly connects the left and right ways. There is some tolerance
   * for "directly". See ticket #951 for details.
   */
  bool _directConnect(const ConstOsmMapPtr &map, boost::shared_ptr<Way> w) const;

  void _markNeedsReview(const OsmMapPtr& map, ElementPtr e1, ElementPtr e2, QString note,
                        QString reviewType) const;

  void _mergePair(const OsmMapPtr& map, ElementId eid1, ElementId eid2,
                   vector< pair<ElementId, ElementId> >& replaced) const;

  void _removeSpans(boost::shared_ptr<OsmMap> map, const ElementPtr& w1, const ElementPtr& w2) const;
  void _removeSpans(boost::shared_ptr<OsmMap> map, const WayPtr& w1, const WayPtr& w2) const;

  /**
   * Snap the ends of snapee that match with either end point of middle to snapTo's end points.
   */
  void _snapEnds(const OsmMapPtr &map, ElementPtr snapee, ElementPtr snapTo) const;
  void _snapEnds(WayPtr snapee, WayPtr middle, WayPtr snapTo) const;

  /**
   * Splits the splitee up into a match element and a scrap element. All the tags are assigned
   * appropriately and the match and scrap are added to the replaced list and added to the map.
   * The original elements are deleted.
   */
  void _splitElement(const OsmMapPtr& map, const WaySublineCollection& s, const vector<bool>& reverse,
    vector< pair<ElementId, ElementId> >& replaced,
    const ConstElementPtr& splitee, ElementPtr& match, ElementPtr& scrap) const;

  bool _doesWayConnect(long node1, long node2, const ConstWayPtr& w) const;

  // for white box testing.
  friend class HighwaySnapMergerTest;
};

typedef boost::shared_ptr<HighwaySnapMerger> HighwaySnapMergerPtr;

}

#endif // HIGHWAYSNAPMERGER_H
