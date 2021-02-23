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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#ifndef LINEAR_AVERAGE_MERGER_H
#define LINEAR_AVERAGE_MERGER_H

// Hoot
#include <hoot/core/conflate/merging/LinearMergerAbstract.h>
#include <hoot/core/algorithms/subline-matching/SublineStringMatcher.h>

namespace hoot
{

class WaySublineCollection;

/**
 * TODO
 */
class LinearAverageMerger : public LinearMergerAbstract
{

public:

  static QString className() { return "hoot::LinearAverageMerger"; }

  LinearAverageMerger();
  LinearAverageMerger(
    const std::set<std::pair<ElementId, ElementId>>& pairs,
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher);
  virtual ~LinearAverageMerger() = default;

  virtual void apply(const OsmMapPtr& map, std::vector<std::pair<ElementId, ElementId>>& replaced);

  void setMatchedBy(const QString& matchedBy) { _matchedBy = matchedBy; }

  virtual QString getDescription() const { return "Merges road geometries and tags"; }

  virtual QString getName() const override { return className(); }

  virtual QString getClassName() const override { return className(); }

protected:

  std::shared_ptr<SublineStringMatcher> _sublineMatcher;

  virtual bool _mergePair(const OsmMapPtr& map, ElementId eid1, ElementId eid2,
                          std::vector<std::pair<ElementId, ElementId>>& replaced);

  /*
   * Finds a matching subline between two elements with the configured subline matcher
   */
  virtual WaySublineMatchString _matchSubline(OsmMapPtr map, ElementPtr e1, ElementPtr e2);

private:

  static int logWarnCount;

  // indicates which matcher matched the elements being processed by this merger
  QString _matchedBy;

  static const bool WRITE_DETAILED_DEBUG_MAPS;
};

typedef std::shared_ptr<LinearAverageMerger> LinearAverageMergerPtr;

}

#endif // LINEAR_SNAP_MERGER_H
