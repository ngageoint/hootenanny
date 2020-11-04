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
#ifndef CHANGESET_REPLACEMENT_H
#define CHANGESET_REPLACEMENT_H

//GEOS
#include <geos/geom/Envelope.h>
#include <geos/geom/Polygon.h>

// Qt
#include <QString>

namespace hoot
{

/**
 * Interface for classes generating changesets where one dataset replaces another
 */
class ChangesetReplacement
{

public:

  static std::string className() { return "hoot::ChangesetReplacement"; }

  /**
   * The manner in which replacement boundary conditions are handled. See the
   * changeset-derive-replacement CLI doc for more detail.
   *
   * @todo Hybrid may go away
   */
  enum BoundsInterpretation
  {
    Strict = 0, // only features completely inside or lines crossing that get cut at the boundary
    Lenient,    // features inside and overlapping
    Hybrid      // points inside, polys inside and overlapping, lines cut at the boundary
  };

  /**
   * Creates a changeset that replaces features in the first input with features from the second
   * input
   *
   * @param input1 the target data file path for the changeset in which to replace features; must
   * support Boundable
   * @param input2 the source data file path for the changeset to get replacement features from;
   * must support Boundable
   * @param bounds the rectangular bounds over which features are to be replaced
   * @param output the changeset file output location
   */
  virtual void create(
    const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
    const QString& output) = 0;

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
    const QString& input1, const QString& input2,
    const std::shared_ptr<geos::geom::Polygon>& bounds, const QString& output) = 0;

  virtual int getNumChanges() const = 0;

  virtual void setFullReplacement(const bool full) = 0;
  virtual void setBoundsInterpretation(const BoundsInterpretation& interpretation) = 0;
  virtual void setGeometryFilters(const QStringList& filterClassNames) = 0;
  virtual void setReplacementFilters(const QStringList& filterClassNames) = 0;
  virtual void setChainReplacementFilters(const bool chain)  = 0;
  virtual void setReplacementFilterOptions(const QStringList& optionKvps) = 0;
  virtual void setRetainmentFilters(const QStringList& filterClassNames) = 0;
  virtual void setChainRetainmentFilters(const bool chain) = 0;
  virtual void setRetainmentFilterOptions(const QStringList& optionKvps) = 0;
  virtual void setConflationEnabled(const bool enabled) = 0;
  virtual void setChangesetId(const QString& id) = 0;
  virtual void setChangesetOptions(
    const bool printStats, const QString& statsOutputFile, const QString osmApiDbUrl) = 0;

  virtual QString toString() const  = 0;
};

}

#endif // CHANGESET_REPLACEMENT_H
