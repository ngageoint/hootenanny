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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CHANGESET_REPLACEMENT_CREATOR_H
#define CHANGESET_REPLACEMENT_CREATOR_H

// Hoot
#include <hoot/core/algorithms/changeset/ChangesetCutOnlyCreator.h>

namespace hoot
{

/**
 * Single geometry pass version of ChangesetCutOnlyCreator, which solves the bug in handling
 * relations with children of mixed geometry types. This drops support for overlapping only
 * replacement and strict bounds handling, as they are not useful for replacements within a task
 * grid. This temporarily drops support for the additional filters (they were broken anyway), and
 * they will be restored as part of #4267. Eventually this class will be refactored and renamed.
 */
class ChangesetReplacementCreator : public ChangesetCutOnlyCreator
{

public:

  static std::string className() { return "hoot::ChangesetReplacementCreator"; }

  ChangesetReplacementCreator();

  /**
   * @see ChangesetReplacement
   */
  virtual void create(
    const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
    const QString& output) override;
  /**
   * @see ChangesetReplacement
   */
  virtual void create(
    const QString& input1, const QString& input2,
    const std::shared_ptr<geos::geom::Polygon>& bounds, const QString& output) override;

  // Currently, this only supports geometry filters (additional filters are broken right now
  // anyway: #4267).
  virtual void setGeometryFilters(const QStringList& filterClassNames) override;
  virtual void setReplacementFilters(const QStringList& /*filterClassNames*/) override {}
  virtual void setChainReplacementFilters(const bool /*chain*/) override {}
  virtual void setReplacementFilterOptions(const QStringList& /*optionKvps*/) override {}
  virtual void setRetainmentFilters(const QStringList& /*filterClassNames*/) override {}
  virtual void setChainRetainmentFilters(const bool /*chain*/) override {}
  virtual void setRetainmentFilterOptions(const QStringList& /*optionKvps*/) override {}

  virtual QString toString() const override
    { return QString::fromStdString(className()).remove("hoot::"); }

protected:

  ElementCriterionPtr _geometryTypeFilter;

  virtual void _setGlobalOpts() override;

private:

  OsmMapPtr _loadAndFilterRefMap(QMap<ElementId, long>& refIdToVersionMappings);
  OsmMapPtr _loadAndFilterSecMap();

  void _snapUnconnectedPreChangesetMapCropping(OsmMapPtr& combinedMap);
  void _snapUnconnectedPostChangesetMapCropping(
    OsmMapPtr& refMap, OsmMapPtr& combinedMap, OsmMapPtr& immediatelyConnectedOutOfBoundsWays);

  /*
   * Performs cropping to prepare a map for changeset derivation. This is potentially different
   * cropping than done during initial load and cookie cutting.
   */
  void _cropMapForChangesetDerivation(
    OsmMapPtr& map, const bool keepEntireFeaturesCrossingBounds,
    const bool keepOnlyFeaturesInsideBounds, const QString& debugFileName);

  void _generateChangeset(OsmMapPtr& refMap, OsmMapPtr& combinedMap);
};

}

#endif // CHANGESET_REPLACEMENT_CREATOR_H
