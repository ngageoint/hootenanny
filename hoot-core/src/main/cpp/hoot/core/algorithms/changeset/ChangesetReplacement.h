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
#ifndef CHANGESET_REPLACEMENT_H
#define CHANGESET_REPLACEMENT_H

//GEOS
#include <geos/geom/Polygon.h>

namespace hoot
{

/**
 * @brief The ChangesetReplacement class is the interface to implement for classes generating
 * changesets where one dataset replaces another
 *
 * The interface supports a Polygon geometry instead of any geometry as input due to the fact the
 * bounds option string has to be set by the creators at the beginning of the replacement
 * operation, and there only exist sa string parsing implementation for Polygon. Its possible for
 * this support additional geometry types at some point going forward, but there's no need for it
 * currently.
 */
class ChangesetReplacement
{

public:

  static QString className() { return "ChangesetReplacement"; }

  /**
   * The manner in which replacement boundary conditions are handled. See the
   * changeset-derive CLI doc for more detail.
   * @todo remove Hybrid; not used anywhere
   */
  enum BoundsInterpretation
  {
    Strict = 0, // only features completely inside or lines crossing that get cut at the boundary
    Lenient,    // features inside and overlapping
    Hybrid      // points inside, polys inside and overlapping, lines cut at the boundary
  };

  /**
   * @brief create creates a changeset that replaces data.
   * @param input1 the target data file path
   * @param input2 the source data file path
   * @param bounds the rectangular bounds over which features are to be replaced
   * @param output the changeset file output location
   */
  virtual void create(
    const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
    const QString& output) = 0;
  /**
   * @brief create creates a changeset that replaces data.
   * @param input1 the target data file path
   * @param input2 the source data file path
   * @param bounds the bounds over which features are to be replaced
   * @param output the changeset file output location
   */
  virtual void create(
    const QString& input1, const QString& input2,
    const std::shared_ptr<geos::geom::Polygon>& bounds, const QString& output) = 0;

  virtual int getNumChanges() const = 0;

  virtual void setFullReplacement(const bool full) = 0;
  virtual void setBoundsInterpretation(const BoundsInterpretation& interpretation) = 0;
  virtual void setEnableWaySnapping(const bool enable) = 0;
  virtual void setChangesetId(const QString& id) = 0;

  /**
   * @brief setChangesetOptions sets changeset options.
   * @param printStats prints statistics for the output changeset
   * @param outputStatsFile optional file to output the changeset statistics to
   * @param osmApiDbUrl URL to an OSM API database used to calculate element IDs; required only if
   * the output changeset is of type .osc.sql.
   */
  virtual void setChangesetOptions(
    const bool printStats, const QString& statsOutputFile, const QString osmApiDbUrl) = 0;

  virtual QString toString() const  = 0;
};

}

#endif // CHANGESET_REPLACEMENT_H
