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
 * @copyright Copyright (C) 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef DIFFCONFLATOR_H
#define DIFFCONFLATOR_H

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
class Settings;
class ElementId;

/**
 * Re-entrant but not thread safe.
 *
 * While this object is serializable it doesn't maintain state across serialization. It simply
 * uses the default configuration. This should probably be made more robust in the future, but
 * works fine for now.
 *
 * The idea behind the Differential Conflator is to do the "easy" conflation - to essentially
 * ignore anything that would be a possible conflict. The proposed conops are to conflate
 * input1 and input2, and have hoot spit out everything from input2, with anything that would
 * conflict with input1 removed. Is it really a conflation operation? No, because nothing gets
 * merged. It's not a true diff, either, because it won't tell you stuff that was in input1, but
 * not input2. It's basically answering the question: what can I super-easily merge from dataset2
 * into dataset1?
 */
class DiffConflator : public OsmMapOperation, public Serializable, public Boundable,
    public Configurable
{
public:
  static std::string className() { return "hoot::DiffConflator"; }

  DiffConflator();

  DiffConflator(boost::shared_ptr<MatchThreshold> matchThreshold);

  ~DiffConflator();

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

  virtual void writeObject(QDataStream& /*os*/) const {}

  virtual QString getDescription() const
  { return "Conflates two maps into a single map based on the difference between the inputs"; }

private:

  geos::geom::Envelope _bounds;
  const MatchFactory& _matchFactory;
  boost::shared_ptr<MatchThreshold> _matchThreshold;
  Settings _settings;
  std::vector<const Match*> _matches;
  QList<SingleStat> _stats;

  template <typename InputCollection>
  void _deleteAll(InputCollection& ic)
  {
    for (typename InputCollection::const_iterator it = ic.begin(); it != ic.end(); ++it)
    {
      delete *it;
    }
    ic.clear();
  }

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
};

}

#endif // DIFFCONFLATOR_H

