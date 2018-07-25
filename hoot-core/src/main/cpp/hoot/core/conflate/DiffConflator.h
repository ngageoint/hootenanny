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
#include <hoot/core/io/MemChangesetProvider.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/ops/Boundable.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/conflate/matching/MatchGraph.h>
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
 *
 * The diff conflator also supports calculating a tag differential, but again this is approached
 * from a, "What updates are in the second dataset?" perspective. To this end, the conflator
 * iterates through all of the match pairs, looking for new or different tags in the input2
 * elements. If a difference is found, the input2 element tag set is presumed to be an update, and
 * a change is recorded, in which no element geometry is changed, but the tags from the input2
 * element replace the tags from the input1 element. The output from the tag-differencing
 * will always be an osm changeset (*.osc).
 *
 */
class DiffConflator : public OsmMapOperation, public Serializable, public Boundable,
    public Configurable
{
public:

  /**
   * @brief className - Get a string that represents this class name
   * @return - class name
   */
  static std::string className() { return "hoot::DiffConflator"; }

  /**
   * @brief DiffConflator - Default constructor
   */
  DiffConflator();

  /**
   * @brief DiffConflator - Construct & set a match threshold
   * @param matchThreshold - Match threshold
   */
  DiffConflator(boost::shared_ptr<MatchThreshold> matchThreshold);

  ~DiffConflator();

  /**
   * Conflates the specified map. If the map is not in a planar projection it is reprojected. The
   * map is not reprojected back to the original projection when conflation is complete.
   */

  /**
   * @brief apply - Applies the differential conflation operation to the supplied
   * map. If the map is not in a planar projection it is reprojected. The map
   * is not reprojected back to the original projection when conflation is complete.
   * @param pMap - The map to operate on
   */
  virtual void apply(OsmMapPtr& pMap);

  /**
   * @brief getClassName - Gets the class name
   * @return - The class name string
   */
  virtual std::string getClassName() const { return className(); }

  /**
   * @brief getStats - Gets a list of stats recorded during the conflation process,
   * like number of matches found, time spent finding matches, etc.
   * @return - List of stats
   */
  QList<SingleStat> getStats() const { return _stats; }

  virtual void readObject(QDataStream& /*is*/) {}

  virtual void setBounds(const geos::geom::Envelope& bounds) { _bounds = bounds; }

  virtual void setConfiguration(const Settings &conf);

  virtual void writeObject(QDataStream& /*os*/) const {}

  virtual QString getDescription() const
  { return "Conflates two maps into a single map based on the difference between the inputs"; }

  // Gets the tag differential between the maps. To do this, we look through all
  // of the matches, and compare tags. A set of newer tags is returned as a
  // changeset (because updating the tags requires a modify operation)

  /**
   * @brief getTagDiff - Gets the tag differential that was calculated during the
   * conflation. This will be a list of 'modify' changes that contain tag updates
   * for elements that matched between the input maps.
   * @return - A changeset provider that can be used with ChangesetWriter classes
   */
  MemChangesetProviderPtr getTagDiff();

  /**
   * @brief storeOriginalIDs - Stores the ElementIDs for the original Input1
   * elements. This is necessary for calculating the tag differential, and it's
   * important to call this after loading the Input1 map, and before loading the
   * Input2 map.
   * @param pMap - Map that should be holding only the original "Input1" elements
   */
  void storeOriginalIDs(OsmMapPtr& pMap);

private:

  OsmMapPtr _pMap;
  geos::geom::Envelope _bounds;
  const MatchFactory& _matchFactory;
  boost::shared_ptr<MatchThreshold> _matchThreshold;
  Settings _settings;

  // Stores the matches we found
  std::vector<const Match*> _matches;

  // Stores stats calcuated during conflation
  QList<SingleStat> _stats;

  // Stores the changes we calculate when doing the tag differential
  MemChangesetProviderPtr _pTagChanges;

  // Set of the elements that came from the "Input1" map
  // Used when calculating the tag differential
  // This is important, because elements get modified by map cleaning
  // operations prior to conflation - and we need this list of IDs to help
  // us generate a clean changeset output for the tag diff.
  std::set<ElementId> _originalIds;

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

  // Calculates and stores the tag differential as a set of change objects
  void _calcAndStoreTagChanges();

  // Decides if the newTags should replace the oldTags. Among other things,
  // it checks the differential.tag.ignore.list
  bool _compareTags (const Tags &oldTags, const Tags &newTags);

  // Creates a change object using the original element and new tags
  Change _getChange(ConstElementPtr pOldElement, ConstElementPtr pNewElement);
};

}

#endif // DIFFCONFLATOR_H

