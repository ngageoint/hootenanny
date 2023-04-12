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
 * @copyright Copyright (C) 2015-2023 Maxar (http://www.maxar.com/)
 */

// hoot
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/conflate/polygon/BuildingMatch.h>
#include <hoot/core/conflate/polygon/BuildingRfClassifier.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/visitors/ConstElementVisitor.h>

// tgs
#include <tgs/RStarTree/HilbertRTree.h>

namespace hoot
{

/**
 * Searches the specified map for any building match potentials.
 */
class BuildingMatchVisitor : public ConstElementVisitor
{
public:

  BuildingMatchVisitor(const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& result,
                       ElementCriterionPtr filter = ElementCriterionPtr());
  /**
   * @param matchStatus If the element's status matches this status then it is checked for a match.
   */
  BuildingMatchVisitor(const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& result,
                       std::shared_ptr<BuildingRfClassifier> rf, ConstMatchThresholdPtr threshold,
                       ElementCriterionPtr filter = ElementCriterionPtr(), Status matchStatus = Status::Invalid);

  ~BuildingMatchVisitor() override;

  QString getDescription() const override { return ""; }
  QString getName() const override { return ""; }
  QString getClassName() const override { return ""; }

  void checkForMatch(const std::shared_ptr<const Element>& e);

  std::shared_ptr<BuildingMatch> createMatch(ElementId eid1, ElementId eid2) const;

  static bool isRelated(ConstElementPtr e1, ConstElementPtr e2);

  Meters getSearchRadius(const std::shared_ptr<const Element>& e) const;

  void visit(const ConstElementPtr& e) override;

  bool isMatchCandidate(ConstElementPtr element) const;

  std::shared_ptr<Tgs::HilbertRTree>& getIndex();

  ConstOsmMapPtr getMap() const { return _map; }

  long getNumMatchCandidatesFound() const { return _numMatchCandidatesVisited; }

private:

  const ConstOsmMapPtr& _map;
  std::vector<ConstMatchPtr>& _result;
  std::set<ElementId> _empty;
  std::shared_ptr<BuildingRfClassifier> _rf;
  ConstMatchThresholdPtr _mt;
  ElementCriterionPtr _filter;
  Status _matchStatus;
  int _neighborCountMax;
  int _neighborCountSum;
  int _elementsEvaluated;
  size_t _maxGroupSize;
  Meters _searchRadius;

  // Used for finding neighbors
  std::shared_ptr<Tgs::HilbertRTree> _index;
  std::deque<ElementId> _indexToEid;

  long _numElementsVisited;
  long _numMatchCandidatesVisited;
  int _taskStatusUpdateInterval;
  int _memoryCheckUpdateInterval;

  void _markNonOneToOneMatchesAsReview(const std::vector<MatchPtr>& matches) const;

  void _adjustForOverlappingAdjoiningBuildingMatches(std::vector<MatchPtr>& matches) const;

};

}
