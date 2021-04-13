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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef LINEAR_KEEP_REF1_MERGER_H
#define LINEAR_KEEP_REF1_MERGER_H

// Hoot
#include <hoot/core/conflate/merging/LinearMergerAbstract.h>
#include <hoot/core/algorithms/subline-matching/SublineStringMatcher.h>

namespace hoot
{

/**
 * TODO
 */
class LinearKeepRef1Merger : public LinearMergerAbstract
{

public:

  static QString className() { return "hoot::LinearKeepRef1Merger"; }

  LinearKeepRef1Merger();
  LinearKeepRef1Merger(
    const std::set<std::pair<ElementId, ElementId>>& pairs,
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher);
  virtual ~LinearKeepRef1Merger() = default;

  virtual void apply(const OsmMapPtr& map, std::vector<std::pair<ElementId, ElementId>>& replaced);

  virtual QString getDescription() const { return "TODO"; }
  virtual QString getName() const override { return className(); }
  virtual QString getClassName() const override { return className(); }

protected:

  virtual bool _mergePair(const OsmMapPtr& map, ElementId eid1, ElementId eid2,
                          std::vector<std::pair<ElementId, ElementId>>& replaced);

private:

  static int logWarnCount;

  std::shared_ptr<SublineStringMatcher> _sublineMatcher;

  static const bool WRITE_DETAILED_DEBUG_MAPS;
};

typedef std::shared_ptr<LinearKeepRef1Merger> LinearKeepRef1MergerPtr;

}

#endif // LINEAR_KEEP_REF1_MERGER_H
