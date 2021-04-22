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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef ABSTRACT_CONFLATOR_H
#define ABSTRACT_CONFLATOR_H

// hoot
#include <hoot/core/conflate/matching/MatchGraph.h>
#include <hoot/core/conflate/merging/Merger.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/info/SingleStat.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Boundable.h>
#include <hoot/core/util/ProgressReporter.h>

// tgs
#include <tgs/HashMap.h>
#include <tgs/System/Timer.h>

namespace hoot
{

class MatchFactory;
class MatchThreshold;
class MergerFactory;

/**
 * Base class for conflators
 */
class AbstractConflator : public OsmMapOperation, public Boundable, public ProgressReporter
{
public:

  static QString className() { return "hoot::AbstractConflator"; }

  AbstractConflator();
  AbstractConflator(const std::shared_ptr<MatchThreshold>& matchThreshold);
  virtual ~AbstractConflator();

  QList<SingleStat> getStats() const { return _stats; }

  /**
   * Set the factory to use when creating mergers. This method is likely only useful when testing.
   */
  void setMergerFactory(const std::shared_ptr<MergerFactory>& mf) { _mergerFactory = mf; }

  void setProgress(const Progress& progress) override { _progress = progress; }

protected:

  OsmMapPtr _map;

  const MatchFactory& _matchFactory;
  std::shared_ptr<MatchThreshold> _matchThreshold;
  std::vector<ConstMatchPtr> _matches;
  MatchSetVector _matchSets;

  std::shared_ptr<MergerFactory> _mergerFactory;
  HashMap<ElementId, std::vector<MergerPtr>> _e2m;
  std::vector<MergerPtr> _mergers;

  QList<SingleStat> _stats;

  int _taskStatusUpdateInterval;
  int _currentStep;
  Tgs::Timer _timer;

  /*
   * Cleans up any resources used by the object during conflation. This also makes exceptions that
   * might be thrown during apply() clean up the leftovers nicely (albeit delayed).
   */
  virtual void _reset();

  void _createMatches();
  MatchSetVector _optimizeMatches();

  /*
   * Populates the _e2m map with a mapping of ElementIds to their respective Merger objects. This
   * is helpful when replacing element ids with new ids.
   *
   * @todo It may make sense to write a custom multi-map for storing values. Check out this
   * for an example:
   * http://stackoverflow.com/questions/10064422/java-on-memory-efficient-key-value-store
   */
  // TODO:
  void _createMergers(std::vector<MergerPtr>& relationMergers);
  void _mergeFeatures(const std::vector<MergerPtr>& relationMergers);
  void _applyMergers(const std::vector<MergerPtr>& mergers, OsmMapPtr& map);
  void _mapElementIdsToMergers();
  void _replaceElementIds(const std::vector<std::pair<ElementId, ElementId>>& replaced);

  void _addConflateScoreTags();
  void _addConflateScoreTags(
    const ElementPtr& e, const MatchClassification& matchClassification,
    const MatchThreshold& matchThreshold) const;

  void _updateProgress(const int currentStep, const QString message);

private:

  Progress _progress;

  static const bool WRITE_DETAILED_DEBUG_MAPS;

  void _removeWholeGroups(MatchSetVector& matchSets);

  QString _matchSetToString(const MatchSet& matchSet) const;
};

}

#endif // ABSTRACT_CONFLATOR_H

