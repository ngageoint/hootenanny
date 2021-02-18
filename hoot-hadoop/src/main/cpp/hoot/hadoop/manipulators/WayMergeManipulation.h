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
 * @copyright Copyright (C) 2015, 2017, 2018 Maxar (http://www.maxar.com/)
 */

#ifndef WAYMERGEMANIPULATION_H
#define WAYMERGEMANIPULATION_H

#include <hoot/core/manipulators/Manipulation.h>
#include <hoot/core/manipulators/ManipulationDetails.h>

// Hoot
#include <hoot/core/util/Units.h>

namespace hoot
{

class Way;

class WayMergeManipulation : public Manipulation, public ManipulationDetails
{
public:
  /**
   * Left and right aren't truly the left and right, it is just easer for me to think about than
   * v1 & v2.
   * @param threshold Error threshold of the input data in meters.
   */
  WayMergeManipulation(long leftId, long rightId, ConstOsmMapPtr map, Meters minSplitSize);

  virtual void addBogusReviewTags(const OsmMapPtr& map) const;

  virtual void applyManipulation(OsmMapPtr wm, std::set<ElementId>& impactedElements,
    std::set<ElementId>& newElements) const;

  virtual double calculateProbability(ConstOsmMapPtr map) const;

  virtual double calculateScore(ConstOsmMapPtr map) const;

  virtual const std::set<ElementId>& getImpactedElementIds(const ConstOsmMapPtr& map) const;

  virtual const std::set<long>& getImpactedWayIds(const ConstOsmMapPtr& map) const;

  virtual std::set<ElementId> getMatchedElements() const;

  virtual double getProbabilityEstimate() const { return _p; }

  double getBogusReviewScore() const { return std::max(0.0, 1.0 - (getProbabilityEstimate() / 0.5)); }

  virtual double getScoreEstimate() const { return _estimate; }

  virtual bool isValid(ConstOsmMapPtr map) const;

  virtual QString toString() const;

protected:
  mutable std::set<ElementId> _impactedElements;
  mutable std::set<long> _impactedWays;
  long _left, _right;
  mutable double _p;
  Meters _minSplitSize;

  virtual double _calculateExpertProbability(ConstOsmMapPtr map) const;

  /**
   * Returns true if the way directly connects the left and right ways. There is some tolerance
   * for "directly". See ticket #951 for details.
   */
  bool _directConnect(const OsmMapPtr& map, WayPtr w) const;

  void _removeSpans(OsmMapPtr map, std::set<ElementId> &ids) const;

  /**
   * Calculates the MNS for left and right and updates the map accordingly. The new left and
   * right are put into the appropriate boost::shared_ptr.
   */
  void _splitWays(OsmMapPtr map, WayPtr& left, WayPtr& right) const;

};

}

#endif // WAYMERGEMANIPULATION_H
