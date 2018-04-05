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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef UNIFYINGCONFLATOR_H
#define UNIFYINGCONFLATOR_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/Serializable.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/ops/Boundable.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/conflate/match-graph/MatchGraph.h>
#include <hoot/core/ops/stats/SingleStat.h>

// tgs
#include <tgs/HashMap.h>

namespace hoot
{

class Match;
class MatchClassification;
class MatchFactory;
class MatchThreshold;
class Merger;
class MergerFactory;
class Settings;
class ElementId;

/**
 * A different form of conflation from the standard greedy conflation. This is a work in progress
 * see the developer documentation for details and #2376 and #2452 for details.
 *
 * This is named unifying because it unifies the building conflation and road conflation approaches.
 *
 * Re-entrant but not thread safe.
 *
 * While this object is serializable it doesn't maintain state across serialization. It simply
 * uses the default configuration. This should probably be made more robust in the future, but
 * works fine for now.
 */
class UnifyingConflator : public OsmMapOperation, public Serializable, public Boundable,
    public Configurable
{
public:

  static std::string className() { return "hoot::UnifyingConflator"; }

  UnifyingConflator();

  UnifyingConflator(boost::shared_ptr<MatchThreshold> matchThreshold);

  ~UnifyingConflator();

  /**
   * Conflates the specified map. If the map is not in a planar projection it is reprojected. The
   * map is not reprojected back to the original projection when conflation is complete.
   */
  virtual void apply(OsmMapPtr& map);

  virtual std::string getClassName() const { return className(); }

  QList<SingleStat> getStats() const { return _stats; }

  virtual void readObject(QDataStream& /*is*/) {}

  virtual void setBounds(const geos::geom::Envelope& bounds) { _bounds = bounds; }

  virtual void setConfiguration(const Settings &conf);

  /**
   * Set the factory to use when creating mergers. This method is likely only useful when testing.
   */
  void setMergerFactory(boost::shared_ptr<MergerFactory> mf) { _mergerFactory = mf; }

  virtual void writeObject(QDataStream& /*os*/) const {}

  virtual QString getDescription() const
  { return "Conflates two inputs maps into one with Unifying Conflation"; }

private:

  geos::geom::Envelope _bounds;
  const MatchFactory& _matchFactory;
  boost::shared_ptr<MatchThreshold> _matchThreshold;
  boost::shared_ptr<MergerFactory> _mergerFactory;
  Settings _settings;
  HashMap<ElementId, std::vector<Merger*> > _e2m;
  std::vector<const Match*> _matches;
  std::vector<Merger*> _mergers;
  QList<SingleStat> _stats;

  void _addReviewTags(const OsmMapPtr &map, const std::vector<const Match *> &matches);
  void _addScoreTags(const ElementPtr& e, const MatchClassification& mc);

  template <typename InputCollection>
  void _deleteAll(InputCollection& ic)
  {
    for (typename InputCollection::const_iterator it = ic.begin(); it != ic.end(); ++it)
    {
      delete *it;
    }
    ic.clear();
  }

  /**
   * Populates the _e2m map with a mapping of ElementIds to their respective Merger objects. This
   * is helpful when replacing element ids with new ids.
   *
   * @optimize It may make sense to write a custom multi-map for storing values. Check out this
   * for an example:
   * http://stackoverflow.com/questions/10064422/java-on-memory-efficient-key-value-store
   */
  void _mapElementIdsToMergers();

  void _removeWholeGroups(std::vector<const Match *> &matches, MatchSetVector &matchSets,
    const OsmMapPtr &map);

  void _replaceElementIds(const std::vector< std::pair<ElementId, ElementId> >& replaced);

  /**
   * Cleans up any resources used by the object during conflation. This also makes exceptions that
   * might be thrown during apply() clean up the leftovers nicely (albeit delayed).
   */
  void _reset();

  void _validateConflictSubset(const ConstOsmMapPtr& map, std::vector<const Match *> matches);

  void _printMatches(std::vector<const Match*> matches);
  void _printMatches(std::vector<const Match*> matches, const MatchType& typeFilter);

  /*
   * This method is required for the preserve.unknown1.element.id.when.modifying.features setting
   * to work.  It is run after conflation to prevent ID's of unknown1 features from being lost.
   */
  void _mapUnknown1IdsBackToModifiedElements(OsmMapPtr& map);
};

}

#endif // UNIFYINGCONFLATOR_H

