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
#ifndef CHANGESET_CUT_ONLY_CREATOR_H
#define CHANGESET_CUT_ONLY_CREATOR_H

// Hoot
#include <hoot/core/criterion/GeometryTypeCriterion.h>
#include <hoot/core/algorithms/changeset/ChangesetReplacementCreatorAbstract.h>

namespace hoot
{

class ChainCriterion;
class Settings;

/**
 * This is the cut only version of C&R.
 */
class ChangesetCutOnlyCreator : public ChangesetReplacementCreatorAbstract
{

public:

  static std::string className() { return "hoot::ChangesetCutOnlyCreator"; }

  ChangesetCutOnlyCreator();

  /**
   * Creates a changeset that replaces features in the first input
   *
   * @param input1 the target data file path for the changeset in which to replace features; must
   * support Boundable
   * @param input2 ignored
   * @param bounds the rectangular bounds over which features are to be replaced
   * @param output the changeset file output location
   */
  virtual void create(
    const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
    const QString& output);

  /**
   * Creates a changeset that replaces features in the first input
   *
   * @param input1 the target data file path for the changeset in which to replace features; must
   * support Boundable
   * @param input2 ignored
   * @param bounds the bounds over which features are to be replaced
   * @param output the changeset file output location
   */
  virtual void create(
    const QString& input1, const QString& input2,
    const std::shared_ptr<geos::geom::Polygon>& bounds, const QString& output);

  virtual int getNumChanges() const { return _numChanges; }

  virtual void setGeometryFilters(const QStringList& filterClassNames);
  virtual void setReplacementFilters(const QStringList& filterClassNames);
  virtual void setChainReplacementFilters(const bool chain) { _chainReplacementFilters = chain; }
  virtual void setReplacementFilterOptions(const QStringList& optionKvps);
  virtual void setRetainmentFilters(const QStringList& filterClassNames);
  virtual void setChainRetainmentFilters(const bool chain) { _chainRetainmentFilters = chain; }
  virtual void setRetainmentFilterOptions(const QStringList& optionKvps);

  virtual QString toString() const override
    { return QString::fromStdString(className()).remove("hoot::"); }

protected:

  virtual void _setGlobalOpts() override;

private:

  friend class ChangesetReplacementCreatorTest;

  void _parseConfigOpts(const GeometryTypeCriterion::GeometryType& geometryType);

  /*
   * Returns the default geometry filters (point, line, poly) to use when no other geometry filters
   * are specified
   */
  QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr>
    _getDefaultGeometryFilters() const;
  /*
   * Combines filters in _geometryTypeFilters with _replacementFilter.
   */
  QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr> _getCombinedFilters(
    std::shared_ptr<ChainCriterion> nonGeometryFilter);
  bool _roadFilterExists() const;
  void _setInputFilter(
    std::shared_ptr<ChainCriterion>& inputFilter, const QStringList& filterClassNames,
    const bool chainFilters);
  void _setInputFilterOptions(Settings& opts, const QStringList& optionKvps);

  OsmMapPtr _loadRefMap(const GeometryTypeCriterion::GeometryType& geometryType);
  OsmMapPtr _loadSecMap(const GeometryTypeCriterion::GeometryType& geometryType);

  /*
   * Populates a reference and a conflated map based on the geometry type being replaced. The maps
   * can then used to derive the replacement changeset.
   */
  void _processMaps(
    OsmMapPtr& refMap, OsmMapPtr& conflatedMap, const ElementCriterionPtr& refFeatureFilter,
    const ElementCriterionPtr& secFeatureFilter,
    const GeometryTypeCriterion::GeometryType& geometryType,
    const QStringList& linearFilterClassNames = QStringList());
};

}

#endif // CHANGESET_CUT_ONLY_CREATOR_H
