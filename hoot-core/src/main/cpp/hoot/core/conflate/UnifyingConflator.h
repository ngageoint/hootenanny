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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef UNIFYINGCONFLATOR_H
#define UNIFYINGCONFLATOR_H

// hoot
#include <hoot/core/conflate/matching/MatchGraph.h>
#include <hoot/core/conflate/merging/Merger.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/info/SingleStat.h>
#include <hoot/core/io/Serializable.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Boundable.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/ProgressReporter.h>
#include <hoot/core/util/Settings.h>

// tgs
#include <tgs/HashMap.h>

// Qt
#include <QString>

namespace hoot
{

class Match;
class MatchClassification;
class MatchFactory;
class MatchThreshold;
class MergerFactory;
class ElementId;

/**
 * A different conflation algorithm compared to the original greedy conflation alg. This is named
 * Unifying because it unifies different feature type conflation approaches (building, road, etc.)
 * that were originally separated from each other.
 *
 * Re-entrant but not thread safe.
 *
 * While this object is serializable it doesn't maintain state across serialization. It simply
 * uses the default configuration. This should probably be made more robust in the future, but
 * works fine for now.
 */
class UnifyingConflator : public OsmMapOperation, public Serializable, public Boundable,
  public Configurable, public ProgressReporter
{
public:

  static std::string className() { return "hoot::UnifyingConflator"; }

  UnifyingConflator();
  UnifyingConflator(const std::shared_ptr<MatchThreshold>& matchThreshold);

  virtual ~UnifyingConflator();

  /**
   * Conflates the specified map. If the map is not in a planar projection it is reprojected. The
   * map is not reprojected back to the original projection when conflation is complete.
   */
  virtual void apply(OsmMapPtr& map) override;

  virtual std::string getClassName() const override { return className(); }

  QList<SingleStat> getStats() const { return _stats; }

  virtual void readObject(QDataStream& /*is*/) override {}

  virtual void setBounds(const geos::geom::Envelope& bounds) override { _bounds = bounds; }

  virtual void setConfiguration(const Settings &conf) override;

  /**
   * Set the factory to use when creating mergers. This method is likely only useful when testing.
   */
  void setMergerFactory(const std::shared_ptr<MergerFactory>& mf) { _mergerFactory = mf; }

  virtual void writeObject(QDataStream& /*os*/) const override {}

  virtual QString getDescription() const override
  { return "Conflates two inputs maps into one with Unifying Conflation"; }

  virtual void setProgress(Progress progress) override { _progress = progress; }
  virtual unsigned int getNumSteps() const override { return 3; }

  /**
   * Determines if conflation is configured with the Network Roads algorithm
   *
   * @return true if conflation is configured with the Network Roads algorithm; false otherwise
   */
  static bool isNetworkConflate();

private:

  geos::geom::Envelope _bounds;
  const MatchFactory& _matchFactory;
  std::shared_ptr<MatchThreshold> _matchThreshold;
  std::shared_ptr<MergerFactory> _mergerFactory;
  Settings _settings;   // Why is this needed?
  HashMap<ElementId, std::vector<MergerPtr>> _e2m;
  std::vector<ConstMatchPtr> _matches;
  std::vector<MergerPtr> _mergers;
  QList<SingleStat> _stats;
  int _taskStatusUpdateInterval;
  Progress _progress;

  void _addReviewTags(const OsmMapPtr &map, const std::vector<ConstMatchPtr> &matches);
  void _addScoreTags(const ElementPtr& e, const MatchClassification& mc);

  /**
   * Populates the _e2m map with a mapping of ElementIds to their respective Merger objects. This
   * is helpful when replacing element ids with new ids.
   *
   * @optimize It may make sense to write a custom multi-map for storing values. Check out this
   * for an example:
   * http://stackoverflow.com/questions/10064422/java-on-memory-efficient-key-value-store
   */
  void _mapElementIdsToMergers();

  void _removeWholeGroups(std::vector<ConstMatchPtr> &matches, MatchSetVector &matchSets,
    const OsmMapPtr &map);

  void _replaceElementIds(const std::vector<std::pair<ElementId, ElementId>>& replaced);

  /**
   * Cleans up any resources used by the object during conflation. This also makes exceptions that
   * might be thrown during apply() clean up the leftovers nicely (albeit delayed).
   */
  void _reset();

  void _printMatches(std::vector<ConstMatchPtr> matches);
  void _printMatches(std::vector<ConstMatchPtr> matches, const MatchType& typeFilter);
  QString _matchSetToString(const MatchSet& matchSet) const;

  void _updateProgress(const int currentStep, const QString message);
};

}

#endif // UNIFYINGCONFLATOR_H

