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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef DIFFCONFLATOR_H
#define DIFFCONFLATOR_H

// hoot
#include <hoot/core/algorithms/changeset/ChangesetDeriver.h>
#include <hoot/core/algorithms/changeset/MemChangesetProvider.h>
#include <hoot/core/io/ChangesetStatsFormat.h>
#include <hoot/core/conflate/AbstractConflator.h>

namespace hoot
{

class MatchThreshold;

/**
 * The idea behind the Differential Conflator is to do the "easy" conflation - to essentially
 * ignore anything that would be a possible conflict. The proposed conops are to conflate
 * input1 and input2, and spit out everything from input2, with anything that would conflict with
 * input1 removed. Is it really a conflation operation? No, because nothing gets merged. It's not a
 * true diff, either, because it won't tell you stuff that was in input1, but not input2. It's
 * basically answering the question: what can I super-easily merge from dataset2 into dataset1?
 *
 * The diff conflator also supports calculating a tag differential, but again this is approached
 * from a, "What updates are in the second dataset?" perspective. To this end, the conflator
 * iterates through all of the match pairs, looking for new or different tags in the input2
 * elements. If a difference is found, the input2 element tag set is presumed to be an update, and
 * a change is recorded, in which no element geometry is changed, but the tags from the input2
 * element replace the tags from the input1 element. The output from the tag-differencing
 * will always be an osm changeset (*.osc).
 *
 * Workflow Steps:
 *
 * - Store off original map and ref1
 * - Mark ref1 inputs
 * - Remove unconflatable elements (optional and enabled by default; generally only turned off for
 * debugging)
 * - Find matches
 * - Calc tag diff against the matches (optional; used when --separate-output is specified)
 * - Optimize linear matches (only happens when differential.remove.linear.partial.matches.as.whole
 * is disabled and a linear feature matcher has been specified in the configuration)
 * - Remove secondary match elements
 * - Snap secondary roads back to ref roads (optional; not enabled by default)
 * - Remove ref match elements
 * - Remove some metadata tags
 * - Get the tag diff (optional; used when --separate-output is specified)
 * - Add tag changes to back to the map (optional; used when --separate-output is specified)
 *
 * Re-entrant but not thread safe. While this object is serializable, it doesn't maintain state
 * across serialization. It simply uses the default configuration. This should probably be made
 * more robust in the future, but works fine for now.
 */
class DiffConflator : public AbstractConflator
{
public:

  /**
   * Returns a string that represents this class name
   *
   * @return class name
   */
  static QString className() { return "hoot::DiffConflator"; }

  /**
   * Default constructor
   */
  DiffConflator();

  /**
   * Construct & set a match threshold
   *
   * @param matchThreshold - Match threshold
   */
  DiffConflator(const std::shared_ptr<MatchThreshold>& matchThreshold);

  /**
   * Applies the differential conflation operation to the supplied
   * map. If the map is not in a planar projection it is reprojected. The map
   * is not reprojected back to the original projection when conflation is complete.
   *
   * @param map - The map to operate on
   */
  void apply(OsmMapPtr& map) override;

  /**
   * Stores the original map. This is necessary for calculating the tag differential, and it's
   * important to call this after loading the Input1 map, and before loading the Input2 map.
   *
   * @param map - Map that should be holding only the original "Input1" elements
   */
  void storeOriginalMap(OsmMapPtr& map);

  /**
   * Uniquely mark input1 elements
   *
   * @param map - Map to add the changes to
   */
  void markInputElements(OsmMapPtr map);

  /**
   * Adds the changes to a map, as regular elements. This is useful for visualizing tag-diff output
   * in JOSM and the hoot UI.
   *
   * @param map - Map to add the changes to
   * @param pChanges - Changeset provider
   */
  void addChangesToMap(OsmMapPtr map, ChangesetProviderPtr pChanges);

  /**
   * Writes a changeset with just the data from the input map
   *
   * @param pResultMap the input map
   * @param output the output changeset path
   * @param separateOutput if true, separates geometry and tag changeset output
   * @param changesetStatsFormat changeset statistics format
   * @param osmApiDbUrl specifies the target OSM API database, if SQL changeset output is specified
   */
  void writeChangeset(
    OsmMapPtr pResultMap, QString& output, bool separateOutput,
    const ChangesetStatsFormat& changesetStatsFormat =
      ChangesetStatsFormat(ChangesetStatsFormat::Unknown),
    const QString& osmApiDbUrl = "");

  /**
   * Calculates statistics unique to Differential Conflation
   *
   * @param pResultMap map containing the features to calculate the statistics from
   * @param stats the statistics to populate
   */
  void calculateStats(OsmMapPtr pResultMap, QList<SingleStat>& stats);

  /**
   * @see ProgressReporter
   */
  unsigned int getNumSteps() const override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Creates a map with features from the second input which are not in the first"; }

  void enableTags() { _conflateTags = true; }
  bool conflatingTags() const { return _conflateTags;}

  /**
   * Returns the tag differential that was calculated during the conflation. This will be a list of
   * 'modify' changes that contain tag updates for elements that matched between the input maps.
   *
   * To calculate these changes we look through all of the matches, and compare tags. A set of newer
   * tags is returned as a changeset (because updating the tags requires a modify operation).
   *
   * @return A changeset provider that can be used with ChangesetWriter classes
   */
  MemChangesetProviderPtr getTagDiff() { return _tagChanges; }

  QString getGeometryChangesetStats() const { return _geometryChangesetStats; }
  QString getTagChangesetStats() const { return _tagChangesetStats; }
  QString getUnifiedChangesetStats() const { return _unifiedChangesetStats; }
  long getNumUnconflatableElementsDiscarded() const { return _numUnconflatableElementsDiscarded; }

  void setRemoveLinearPartialMatchesAsWhole(bool remove)
  { _removeLinearPartialMatchesAsWhole = remove; }

private:

  // A copy of the "Input1" map. This is used when calculating the tag differential. It's important,
  // because elements get modified by map cleaning operations prior to conflation - and we need this
  // as a reference for original IDs and original geometry, so that we can generate a clean
  // changeset output for the tag diff.
  OsmMapPtr _originalMap;
  // keep a copy of the original map with only ref1 data to calc the diff when any roads are snapped
  OsmMapPtr _originalRef1Map;

  bool _conflateTags = false;
  // Stores the changes we calculate when doing the tag differential
  MemChangesetProviderPtr _tagChanges;

  // IDs of all elements involved in only intra-dataset matches
  QSet<ElementId> _intraDatasetMatchOnlyElementIds;
  bool _intraDatasetElementIdsPopulated;

  // Determines whether matching linear portions from the secondary are partially removed (just
  // portions that matched) or completely removed (entire element); see
  // differential.remove.linear.partial.matches.as.whole
  bool _removeLinearPartialMatchesAsWhole;
  // These are only used when _removeLinearPartialMatchesAsWhole=false.
  std::vector<ConstMatchPtr> _matchesToRemoveAsPartial;
  std::vector<ConstMatchPtr> _matchesToRemoveAsWhole;

  long _numSnappedWays;
  long _numUnconflatableElementsDiscarded;

  QString _geometryChangesetStats;
  QString _tagChangesetStats;
  QString _unifiedChangesetStats;

  static int logWarnCount;

  /*
   * Cleans up any resources used by the object during conflation. This also makes exceptions that
   * might be thrown during apply() clean up the leftovers nicely (albeit delayed).
   */
  void _reset() override;

  void _discardUnconflatableElements();

  static bool _isMatchToRemovePartially(const ConstMatchPtr& match);
  /*
   * Separates matches with linear features that we are removing partially from other matches that
   * we're removing completely
   */
  void _separateMatchesToRemoveAsPartial();
  int _countMatchesToRemoveAsPartial() const;

  // Calculates and stores the tag differential as a set of change objects
  void _calcAndStoreTagChanges();
  // Decides if the newTags should replace the oldTags. Among other things, it checks the
  // differential.tag.ignore.list
  bool _tagsAreDifferent(const Tags& oldTags, const Tags& newTags) const;

  // Creates a change object using the original element and new tags
  Change _getChange(ConstElementPtr pOldElement, ConstElementPtr pNewElement);
  std::shared_ptr<ChangesetDeriver> _sortInputs(OsmMapPtr map1, OsmMapPtr map2);
  ChangesetProviderPtr _getChangesetFromMap(OsmMapPtr map);

  long _snapSecondaryRoadsBackToRef();

  QSet<ElementId> _getElementIdsInvolvedInOnlyIntraDatasetMatches(
    const std::vector<ConstMatchPtr>& matches);

  void _removeRefData();
  /*
   * The element criteria that must be met in order for an element involved in a match to be
   * completely removed
   */
  bool _satisfiesCompleteElementRemovalCondition(
    const ConstElementPtr& element, const Status& status, const ConstMatchPtr& match) const;
  /*
   * Removes match elements from a match completely regardless of whether only part of their
   * geometry is involved in a match (if linear).
   */
  void _removeMatchElementsCompletely(const Status& status);
  void _removeMatchElementPairCompletely(
    const ConstMatchPtr& match, const std::pair<ElementId, ElementId>& elementPair,
    const Status& status);
  /*
   * Removes only the geometric portions of linear match elements that were involved in a match.
   */
  void _removePartialSecondaryMatchElements();
  void _removeMetadataTags();
};

}

#endif // DIFFCONFLATOR_H

