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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef HIGHWAY_MERGER_ABSTRACT_H
#define HIGHWAY_MERGER_ABSTRACT_H

// Hoot
#include <hoot/core/conflate/merging/MergerBase.h>
#include <hoot/core/conflate/review/ReviewMarker.h>

namespace hoot
{

/**
 * Base class for road merging
 */
class HighwayMergerAbstract : public MergerBase
{

public:

  static std::string className() { return "hoot::HighwayMergerAbstract"; }

  static unsigned int logWarnCount;

  virtual ~HighwayMergerAbstract() {}

  virtual void apply(const OsmMapPtr& map,
                     std::vector< std::pair<ElementId, ElementId> >& replaced) = 0;

  virtual QString toString() const;

protected:

  virtual PairsSet& getPairs() { return _pairs; }
  virtual const PairsSet& getPairs() const { return _pairs; }

  std::set<std::pair<ElementId, ElementId>> _pairs;

  /*
   * Return true if pair needs review.
   */
  virtual bool _mergePair(const OsmMapPtr& map, ElementId eid1, ElementId eid2,
    std::vector<std::pair<ElementId, ElementId>>& replaced);

  virtual void _markNeedsReview(const OsmMapPtr& map, ElementPtr e1, ElementPtr e2, QString note,
                                QString reviewType);

  ReviewMarker _reviewMarker;
};

}

#endif // HIGHWAY_MERGER_ABSTRACT_H
