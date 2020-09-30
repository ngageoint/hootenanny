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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CHANGESET_REPLACEMENT_CREATOR_6_H
#define CHANGESET_REPLACEMENT_CREATOR_6_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/GeometryTypeCriterion.h>
#include <hoot/core/ops/ElementIdRemapper.h>
#include <hoot/core/algorithms/changeset/ChangesetReplacementCreator.h>

namespace hoot
{

class ChangesetCreator;
class ChainCriterion;
class Settings;
class Change;
class ElementDeduplicator;
class ConstOsmMapConsumer;

/**
 * simplified version that doesn't support filtering or conflation and only supports lenient bounds
 *
 * @todo add filtering
 */
class ChangesetReplacementCreator6 : public ChangesetReplacementCreator
{

public:

  static std::string className() { return "hoot::ChangesetReplacementCreator6"; }

  ChangesetReplacementCreator6();

  /**
   * Creates a changeset that replaces features in the first input with features from the second
   * input
   *
   * @param input1 the target data file path for the changeset in which to replace features; must
   * support Boundable
   * @param input2 the source data file path for the changeset to get replacement features from;
   * must support Boundable
   * @param bounds the bounds over which features are to be replaced
   * @param output the changeset file output location
   */
  virtual void create(
    const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
    const QString& output);

  void createSimple(
    const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
    const QString& output);

  virtual int getNumChanges() const { return _numChanges; }

  virtual void setFullReplacement(const bool /*full*/) {}
  virtual void setBoundsInterpretation(const BoundsInterpretation& /*interpretation*/) {}
  virtual void setGeometryFilters(const QStringList& /*filterClassNames*/) {}
  virtual void setReplacementFilters(const QStringList& /*filterClassNames*/) {}
  virtual void setChainReplacementFilters(const bool /*chain*/) {}
  virtual void setReplacementFilterOptions(const QStringList& /*optionKvps*/) {}
  virtual void setRetainmentFilters(const QStringList& /*filterClassNames*/) {}
  virtual void setChainRetainmentFilters(const bool /*chain*/) {}
  virtual void setRetainmentFilterOptions(const QStringList& /*optionKvps*/) {}
  virtual void setConflationEnabled(const bool /*enabled*/) {}
  virtual void setChangesetId(const QString& id) { _changesetId = id; }
  /**
   * Sets changeset options
   *
   * @param printStats prints statistics for the output changeset
   * @param outputStatsFile optional file to output the changeset statistics to
   * @param osmApiDbUrl URL to an OSM API database used to calculate element IDs; required only if
   * the output changeset is of type .osc.sql.
   */
  virtual void setChangesetOptions(
    const bool printStats, const QString& statsOutputFile, const QString osmApiDbUrl);

  virtual QString toString() const override
    { return QString::fromStdString(className()).remove("hoot::"); }

protected:

  // path to the input with data being replaced; overrides use of _input1Map
  QString _input1;

  // path to the input with data used for replacement; overrides use of _input2Map
  QString _input2;

  // path to the changeset output file
  QString _output;

  // the AOI over which the replacement is being performed
  QString _replacementBounds;

  // helpful to name the debug map files when doing successive replacements
  QString _changesetId;

  // used to keep log messages with urls in them shorter
  int _maxFilePrintLength;

  // handles changeset generation and output
  std::shared_ptr<ChangesetCreator> _changesetCreator;
  int _numChanges;

  void _setGlobalOpts();
  void _validateInputs();
  void _printJobDescription() const;

  OsmMapPtr _loadInputMap(
    const QString& mapName, const QString& inputUrl, const bool useFileIds, const Status& status,
    const bool keepEntireFeaturesCrossingBounds, const bool keepOnlyFeaturesInsideBounds,
    const bool keepImmediatelyConnectedWaysOutsideBounds, const bool warnOnZeroVersions);

  void _removeMetadataTags(const OsmMapPtr& map);
  void _markElementsWithMissingChildren(OsmMapPtr& map);

  void _cut(
    OsmMapPtr& map, const bool keepEntireFeaturesCrossingBounds,
    const bool keepOnlyFeaturesInsideBounds);
  void _cut(
    OsmMapPtr mapToReplace, OsmMapPtr replacementMap,
    const bool keepEntireFeaturesCrossingBounds, const bool keepOnlyFeaturesInsideBounds);

  void _cleanup(OsmMapPtr& map);

  void _dedupeMaps(OsmMapPtr& refMap, OsmMapPtr& secMap);
  void _intraDedupeMap(OsmMapPtr& map);
};

}

#endif // CHANGESET_REPLACEMENT_CREATOR_6_H
