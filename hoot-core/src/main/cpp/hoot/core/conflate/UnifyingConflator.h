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
#ifndef UNIFYINGCONFLATOR_H
#define UNIFYINGCONFLATOR_H

// hoot
#include <hoot/core/conflate/AbstractConflator.h>

namespace hoot
{

class MatchThreshold;

/**
 * @brief The UnifyingConflator class is a conflator implementing Unifying Confaltion.
 *
 * This is a different conflation algorithm compared to the original greedy conflation alg. This is
 * named Unifying because it unifies different feature type conflation approaches (building, road,
 * etc.) that were originally separated from each other.
 *
 * This class is re-entrant but not thread safe.
 */
class UnifyingConflator : public AbstractConflator
{
public:

  static QString className() { return "UnifyingConflator"; }

  UnifyingConflator();
  UnifyingConflator(const std::shared_ptr<MatchThreshold>& matchThreshold);

  /**
   * @brief apply conflates the specified map.
   *
   * If the map is not in a planar projection it is reprojected. The map is not reprojected back to
   * the original projection when conflation is complete.
   * @see OsmMapOperation
   */
  void apply(OsmMapPtr& map) override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Conflates two inputs maps into one with Unifying Conflation"; }

  /**
   * @see ProgressReporter
   */
  unsigned int getNumSteps() const override;
};

}

#endif // UNIFYINGCONFLATOR_H

