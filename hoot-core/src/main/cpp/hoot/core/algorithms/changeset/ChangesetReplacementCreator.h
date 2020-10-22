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
    const QString& output) override;

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

  virtual void _setGlobalOpts();
};

}

#endif // CHANGESET_REPLACEMENT_CREATOR_H
