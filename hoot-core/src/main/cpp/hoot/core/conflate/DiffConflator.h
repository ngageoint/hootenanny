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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef DIFFCONFLATOR_H
#define DIFFCONFLATOR_H

// hoot
#include <hoot/core/algorithms/changeset/ChangesetDeriver.h>
#include <hoot/core/algorithms/changeset/MemChangesetProvider.h>
#include <hoot/core/conflate/AbstractConflator.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/io/ChangesetStatsFormat.h>
#include <hoot/core/ops/UnconnectedWaySnapper.h>

namespace hoot
{

class MatchThreshold;

/**
 * @brief The DiffConflator class calculates the difference between two inputs and adds to the
 * output only features from the second output which match with nothing in the first input.
 *
 * The diff can be set up to only exclude the partial matching portions of linear features involved
 * in a partial match (default) or remove all portions of the feature.
 *
 * This conflator also supports calculating a tag differential, The conflator iterates through all
 * of the match pairs, looking for new or different tags in the input2 elements. If a difference is
 * found, the input2 element tag set is presumed to be an update, and a change is recorded, in which
 * no element geometry is changed, but the tags from the input2 element replace the tags from the
 * input1 element. The output from the tag-differencing will always be an osm changeset (*.osc).
 *
 * This is re-entrant but not thread safe. While this object is serializable, it doesn't maintain
 * state across serialization. It simply uses the default configuration.
 *
 * Workflow Steps:
 *
 * - Store off original map and ref1
 * - Mark ref1 inputs
 * - Remove unconflatable elements (optional and enabled by default; generally only turned off for
 * debugging)
 * - Find matches
 * - Calc tag diff against the matches (optional; used when conflate.differential.separate.output is true)
 * - Optimize linear matches (only happens when differential.remove.linear.partial.matches.as.whole
 * is disabled and a linear feature matcher has been specified in the configuration)
 * - Remove secondary match elements
 * - Snap secondary roads back to ref roads (optional; not enabled by default)
 * - Remove ref match elements
 * - Remove some metadata tags
 * - Get the tag diff (optional; used when conflate.differential.separate.output is true)
 * - Add tag changes to back to the map (optional; used when conflate.differential.separate.output is true)
 */
class DiffConflator : public AbstractConflator
{
public:

  /**
   * @brief className returns a string that represents this class name.
   * @return class name
   */
  static QString className() { return "DiffConflator"; }

  /**
   * @brief DiffConflator Constructor
   */
  DiffConflator();
  /**
   * @brief DiffConflator Constructor that sets a match threshold.
   * @param matchThreshold Match threshold
   */
  DiffConflator(const std::shared_ptr<MatchThreshold>& matchThreshold);

  /**
   * @see OsmMapOperation
   */
  void apply(OsmMapPtr& map) override;

  /**
   * @brief markInputElements uniquely mark input1 elements.
   * @param map Map to add the changes to
   */
  void markInputElements(OsmMapPtr map) const;

  /**
   * @brief storeOriginalMap Stores the original map.
   *
   * This is necessary for calculating the tag differential, and it's important to call this after
   * loading the Input1 map, and before loading the Input2 map.
   * @param map Map that should be holding only the original "Input1" elements
   */
  void storeOriginalMap(const OsmMapPtr& map);
  /**
   * @brief addChangesToMap adds the changes to a map, as regular elements.
   *
   * This is useful for visualizing tag-diff output in JOSM and the hoot UI.
   * @param map Map to add the changes to
   * @param pChanges Changeset provider
   */
  void addChangesToMap(OsmMapPtr map, ChangesetProviderPtr pChanges);

  /**
   * @brief writeChangeset writes a changeset with just the data from the input map.
   * @param pResultMap the input map
   * @param output the output changeset path
   * @param separateOutput if true, separates geometry and tag changeset output
   * @param changesetStatsFormat changeset statistics format
   * @param osmApiDbUrl specifies the target OSM API database, if SQL changeset output is specified
   */
  void writeChangeset(OsmMapPtr pResultMap, const QString& output, bool separateOutput,
                      const ChangesetStatsFormat& changesetStatsFormat =
                        ChangesetStatsFormat(ChangesetStatsFormat::Unknown),
                      const QString& osmApiDbUrl = "");

  /**
   * @see ProgressReporter
   */
  unsigned int getNumSteps() const override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Creates a map with features from the second input which are not in the first"; }

  void enableTags() { _conflateTags = true; }

  /**
   * @brief getTagDiff returns the tag differential that was calculated during the conflation.
   *
   * This will be a list of 'modify' changes that contain tag updates for elements that matched
   * between the input maps. To calculate these changes we look through all of the matches, and
   * compare tags. A set of newer tags is returned as a changeset (because updating the tags
   * requires a modify operation).
   * @return A changeset provider that can be used with ChangesetWriter classes
   */
  MemChangesetProviderPtr getTagDiff() const { return _tagChanges; }
  bool conflatingTags() const { return _conflateTags;}
  QString getGeometryChangesetStats() const { return _geometryChangesetStats; }
  QString getTagChangesetStats() const { return _tagChangesetStats; }
  QString getUnifiedChangesetStats() const { return _unifiedChangesetStats; }
  long getNumUnconflatableElementsDiscarded() const { return _numUnconflatableElementsDiscarded; }

  void setRemoveLinearPartialMatchesAsWhole(bool remove)
  { _removeLinearPartialMatchesAsWhole = remove; }
  void setRemoveRiverPartialMatchesAsWhole(bool remove)
  { _removeRiverPartialMatchesAsWhole = remove; }

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
  bool _removeRiverPartialMatchesAsWhole; // overrides the linear only option
  // These are only used when _removeLinearMatchesPartially returns true.
  std::vector<ConstMatchPtr> _matchesToRemoveAsPartial;
  std::vector<ConstMatchPtr> _matchesToRemoveAsWhole;

  UnconnectedWaySnapper _linearFeatureSnapper;
  QStringList _originalSnapWayStatuses;
  QStringList _originalSnapToWayStatuses;

  long _numSnappedWays;
  long _numUnconflatableElementsDiscarded;

  QString _geometryChangesetStats;
  QString _tagChangesetStats;
  QString _unifiedChangesetStats;

  static int logWarnCount;

  void _init();

  /*
   * Cleans up any resources used by the object during conflation. This also makes exceptions that
   * might be thrown during apply() clean up the leftovers nicely (albeit delayed).
   */
  void _reset() override;

  /*
   * If its not recognized by the schema or we don't have a matcher for it, we don't want to
   * consider it for being in the output. After recent changes, there should be very few elements
   * that can't be conflated.
   */
  void _discardUnconflatableElements();

  /*
   * Should we remove linear partial matches partially?
   */
  bool _removeLinearMatchesPartially() const;
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
  Change _getChange(ConstElementPtr pOldElement, ConstElementPtr pNewElement) const;
  std::shared_ptr<ChangesetDeriver> _sortInputs(OsmMapPtr map1, OsmMapPtr map2) const;
  ChangesetProviderPtr _getChangesetFromMap(OsmMapPtr map) const;

  /**
   * @brief _snapSecondaryLinearFeaturesBackToRef snaps secondary features that are part of the diff
   * back to reference features before the reference features are dropped from the output.
   * @return the number of ways snapped
   */
  long _snapSecondaryLinearFeaturesBackToRef();
  /**
   * @brief _snapSecondaryLinearFeaturesBackToTagChangedRef snaps secondary features that are part
   * of the diff back to reference features that had tag changes only applied to them (status=4)
   * before the reference features are dropped from the output.
   *
   * This is necessary due to the fact that diff w/ tags may introduce different reference features
   * into the diff after the snapping done with _snapSecondaryLinearFeaturesBackToRef if those
   * reference features had tag changes.
   * @return the number of ways snapped
   */
  long _snapSecondaryLinearFeaturesBackToTagChangedRef();

  QSet<ElementId> _getElementIdsInvolvedInOnlyIntraDatasetMatches(const std::vector<ConstMatchPtr>& matches);

  /**
   * @brief _removeRefData removes reference data from the differential output.
   */
  void _removeRefData(const bool removeSnapped = false);
  /**
   * @brief _cleanSecData cleans secondary ways out of the differential output based on specified
   * criteria.
   * @param baseCriteria criteria for determining which secondary ways to remove; Generally, this
   * would be a ConflatableElementCriterion (HighwayCriterion, etc.)
   * @param maxSize only ways up to this size are removed
   */
  void _cleanSecData(const QStringList& baseCriteria, const double maxSize) const;
  /**
   * The element criteria that must be met in order for an element involved in a match to be
   * completely removed
   */
  bool _satisfiesCompleteElementRemovalCondition(const ConstElementPtr& element, const Status& status,
                                                 const ConstMatchPtr& match) const;
  /**
   * Removes match elements from a match completely regardless of whether only part of their
   * geometry is involved in a match (if linear).
   */
  void _removeMatchElementsCompletely(const Status& status);
  void _removeMatchElementPairCompletely(const ConstMatchPtr& match,
                                         const std::pair<ElementId, ElementId>& elementPair,
                                         const Status& status) const;
  /**
   * Removes only the geometric portions of linear match elements that were involved in a match.
   */
  void _removePartialSecondaryMatchElements();
  void _cleanupAfterPartialMatchRemoval();
  void _removeMetadataTags() const;
};

}

#endif // DIFFCONFLATOR_H

