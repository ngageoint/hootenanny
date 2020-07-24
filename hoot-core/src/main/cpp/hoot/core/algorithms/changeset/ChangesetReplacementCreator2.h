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
#ifndef CHANGESET_REPLACEMENT_CREATOR_2_H
#define CHANGESET_REPLACEMENT_CREATOR_2_H

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
class ChangesetReplacementCreator2
{

  /**
   * Options used to control cropping at various stages of the replacement changeset workflow
   */
  struct BoundsOptions
  {
    // Determines whether features crossing the bounds should be kept when loading reference data.
    bool loadRefKeepEntireCrossingBounds;
    // Determines whether only features completely inside the bounds should be kept when loading
    // reference data.
    bool loadRefKeepOnlyInsideBounds;
    // Determines whether ways immediately connected to other ways being kept but completely outside
    // of the bounds should also be kept
    bool loadRefKeepImmediateConnectedWaysOutsideBounds;
    // Determines whether features crossing the bounds should be kept when loading secondary data.
    bool loadSecKeepEntireCrossingBounds;
    // Determines whether only features completely inside the bounds should be kept when loading
    // secondary data.
    bool loadSecKeepOnlyInsideBounds;

    // Determines whether features crossing the bounds should be kept when cookie cutting reference
    // data.
    bool cookieCutKeepEntireCrossingBounds;
    // Determines whether only features completely inside the bounds should be kept when cookie
    // cutting reference data.
    bool cookieCutKeepOnlyInsideBounds;

    // Determines whether reference features crossing the bounds should be kept when deriving a
    // changeset.
    bool changesetRefKeepEntireCrossingBounds;
    // Determines whether secondary features crossing the bounds should be kept when deriving a
    // changeset.
    bool changesetSecKeepEntireCrossingBounds;
    // Determines whether only reference features completely inside the bounds should be kept when
    // deriving a changeset.
    bool changesetRefKeepOnlyInsideBounds;
    // Determines whether only secondary features completely inside the bounds should be kept when
    // deriving a changeset.
    bool changesetSecKeepOnlyInsideBounds;
    // Determines whether deleting reference features existing either partially of completely outside
    // of the bounds is allowed during changeset generation
    bool changesetAllowDeletingRefOutsideBounds;
    // the strictness of the bounds calculation used in conjunction with
    // _changesetAllowDeletingRefOutsideBounds
    bool inBoundsStrict;
  };

public:

  /**
   * Constructor
   *
   * @param printStats prints statistics for the output changeset
   * @param outputStatsFile optional file to output the changeset statistics to
   * @param osmApiDbUrl URL to an OSM API database used to calculate element IDs; required only if
   * the output changeset is of type .osc.sql.
   */
  ChangesetReplacementCreator2(
    const bool printStats = false, const QString& statsOutputFile = "",
    const QString osmApiDbUrl = "");

  /**
   * Creates a changeset that replaces features in the first input with features from the second
   * input.
   *
   * @param input1 the target data for the changeset in which to replace features; must support
   * Boundable
   * @param input2 the source data for the changeset to get replacement features from; must support
   * Boundable
   * @param bounds the bounds over which features are to be replaced
   * @param output the changeset file output locationn
   */
  void create(
    const QString& input1, const QString& input2, const geos::geom::Envelope& bounds,
    const QString& output);

private:

  // controls cropping
  BoundsOptions _boundsOpts;

  // handles changeset generation and output
  std::shared_ptr<ChangesetCreator> _changesetCreator;

  void _validateInputs(const QString& input1, const QString& input2);

  QString _getJobDescription(
    const QString& input1, const QString& input2, const QString& bounds,
    const QString& output) const;

  void _setGlobalOpts(const QString& boundsStr);
  void _parseConfigOpts();

  OsmMapPtr _loadRefMap(const QString& input);
  OsmMapPtr _loadSecMap(const QString& input);

  /*
   * Adds a custom tag to any element from the input with a missing child. This is primarily useful
   * in repairing relations manually that were passed in without some of their child elements after
   * the replacement changeset is written.
   */
  void _markElementsWithMissingChildren(OsmMapPtr& map);

  /*
   * Cut out of the reference map what you don't want, and if there is anything in the secondary
   * map, add that data in (not applicable in the cut only scenario).
   */
  OsmMapPtr _getCookieCutMap(OsmMapPtr doughMap, OsmMapPtr cutterMap);

  /*
   * Combines two maps into one; throwOutDupes ignores any elements in the second map with the ID
   * as an element in the first map
   */
  void _combineMaps(
    OsmMapPtr& map1, OsmMapPtr& map2, const bool throwOutDupes, const QString& debugFileName);

  void _cleanup(OsmMapPtr& map);
};

}

#endif // CHANGESET_REPLACEMENT_CREATOR_H
