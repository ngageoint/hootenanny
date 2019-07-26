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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CHANGESET_REPLACEMENT_CREATOR_H
#define CHANGESET_REPLACEMENT_CREATOR_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/changeset/ChangesetCreator.h>

//GEOS
#include <geos/geom/Envelope.h>

namespace hoot
{

/**
 * TODO
 */
class ChangesetReplacementCreator
{

public:

  ChangesetReplacementCreator(const bool printStats = false, const QString osmApiDbUrl = "");

  /**
   * TODO
   *
   * @param input1
   * @param input2
   * @param bounds
   * @param featureTypeFilterClassName
   * @param lenientBounds
   * @param output
   * @todo support empty feature filters and filter with multiple types
   */
  void create(
    const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
    const QString& featureTypeFilterClassName, const bool lenientBounds, const QString& output);

private:

  std::shared_ptr<ChangesetCreator> _changesetCreator;

  bool _convertRefKeepEntireCrossingBounds;
  bool _convertRefKeepOnlyInsideBounds;
  bool _convertRefKeepImmediateConnectedWaysOutsideBounds;
  bool _convertSecKeepEntireCrossingBounds;
  bool _convertSecKeepOnlyInsideBounds;
  bool _cropKeepEntireCrossingBounds;
  bool _cropKeepOnlyInsideBounds;
  bool _changesetRefKeepEntireCrossingBounds;
  bool _changesetSecKeepEntireCrossingBounds;
  bool _changesetRefKeepOnlyInsideBounds;
  bool _changesetSecKeepOnlyInsideBounds;
  bool _changesetAllowDeletingRefOutsideBounds;
  bool _inBoundsStrict;

  bool _isPointCrit(const QString& critClassName) const;
  bool _isLinearCrit(const QString& critClassName) const;
  bool _isPolyCrit(const QString& critClassName) const;

  bool _isNetworkConflate() const;

  void _parseConfigOpts(const bool lenientBounds, const QString& critClassName);
};

}

#endif // CHANGESET_REPLACEMENT_CREATOR_H
