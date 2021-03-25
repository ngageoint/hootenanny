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

  static QString className() { return "hoot::ChangesetCutOnlyCreator"; }

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
  void create(const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
    const QString& output) override;

  /**
   * Creates a changeset that replaces features in the first input
   *
   * @param input1 the target data file path for the changeset in which to replace features; must
   * support Boundable
   * @param input2 ignored
   * @param bounds the bounds over which features are to be replaced
   * @param output the changeset file output location
   */
  void create(const QString& input1, const QString& input2,
    const std::shared_ptr<geos::geom::Polygon>& bounds, const QString& output) override;

  QString toString() const override { return className().remove("hoot::"); }

protected:

  void _setGlobalOpts() override;

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
  QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr> _getFilters();
  bool _roadFilterExists() const;

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
