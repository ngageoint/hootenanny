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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef RUBBERSHEET_H
#define RUBBERSHEET_H

// hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/conflate/matching/NodeMatcher.h>

// tgs
#include <tgs/Interpolation/Interpolator.h>

// geos
#include <geos/geom/Coordinate.h>

#include <ogr_spatialref.h>

namespace hoot
{

class SearchRadiusCalculatorTest;
class RubberSheetTest;
class OsmMap;
class Status;

class RubberSheet : public OsmMapOperation
{
public:

  static std::string className() { return "hoot::RubberSheet"; }

  static int logWarnCount;

  /**
   * If this configuration setting is set to true then the first layer is treated as the reference
   * layer and will not be moved.
   */
  static QString refKey() { return "rubber.sheet.ref"; }
  /**
   * If this is true then debug tags will be added to the output file.
   */
  static QString debugKey() { return "rubber.sheet.debug"; }

  struct Match
  {
    long nid1, nid2;
    double score;
    double p;
  };

  RubberSheet();

  void apply(std::shared_ptr<OsmMap>& map) override;

  /**
   * Applies a perviously calculated or loaded transform to the specified map.
   */
  void applyTransform(std::shared_ptr<OsmMap>& map);

  /**
   * Calculates an appropriate transform for the specified map, but does not change any data.
   */
  void calculateTransform(std::shared_ptr<OsmMap>& map);

  /**
   * Set this to true if Unknown1 is a reference dataset and Unknown2 should be moved toward it.
   * If this is false, then the two data sets are moved toward one another.
   */
  void setReference(bool ref) { _ref = ref; }

  /**
   * The minimum number of tie points that will be used when calculating a rubber sheeting solution.
   */
  void setMinimumTies(int minTies) { _minimumTies = minTies; }

  /**
   * Reads the data necessary to perform a transform from unknown1 to unknown2.
   */
  void readTransform1to2(QIODevice& is) { _interpolator1to2 = _readInterpolator(is); }

  /**
   * Reads the data necessary to perform a transform from unknown1 to unknown2.
   */
  void readTransform2to1(QIODevice& is) { _interpolator2to1 = _readInterpolator(is); }

  /**
   * Writes out the data necessary to perform a transform from unknown1 to unknown2.
   */
  void writeTransform1to2(QIODevice& os) const { _writeInterpolator(_interpolator1to2, os); }

  /**
   * Writes out the data necessary to perform a transform from unknown1 to unknown2.
   */
  void writeTransform2to1(QIODevice& os) const { _writeInterpolator(_interpolator2to1, os); }

  /**
   * Calculates the distances between each of the rubber sheet's tie points
   *
   * @return a collection of distance values
   * @throws HootException if the tie points have not been created
   */
   std::vector<double> calculateTiePointDistances();

   void setFailWhenMinimumTiePointsNotFound(bool fail) { _failWhenMinTiePointsNotFound = fail; }

   void setLogWarningWhenRequirementsNotFound(bool logWarning)
   { _logWarningWhenRequirementsNotFound = logWarning; }

   virtual QString getDescription() const override { return "Applies rubber sheeting to a map"; }

   /**
    * @see FilteredByCriteria
    */
   virtual QStringList getCriteria() const;

   virtual std::string getClassName() const { return className(); }

private:

  typedef std::map<long, std::list<Match>> MatchList;

  class Tie
  {
    public:

      // Unknown1 coordinate
      geos::geom::Coordinate c1;
      // Unknown2 coordinate
      geos::geom::Coordinate c2;

      double dx() const { return c1.x - c2.x; }

      double dy() const { return c1.y - c2.y; }

      QString toString() { return "dx: " + QString::number(dx()) + ", dy: " + QString::number(dy()); }
  };

  std::shared_ptr<OsmMap> _map;

  // A map of nids to the list of matches.
  MatchList _matches;
  std::vector<Match> _finalPairs;
  bool _ref;
  bool _debug;
  int _minimumTies;
  std::vector<Tie> _ties;
  std::shared_ptr<Tgs::Interpolator> _interpolator1to2, _interpolator2to1;
  // used as a temporary in interpolating.
  std::vector<double> _matchPoint;
  std::shared_ptr<OGRSpatialReference> _projection;
  std::string _interpolatorClassName;

  double _searchRadius;

  NodeMatcher _nm;

  bool _failWhenMinTiePointsNotFound;
  bool _logWarningWhenRequirementsNotFound;

  Match _emptyMatch;

  int _maxAllowedWays;

  void _findTies();

  void _addIntersection(long nid, const std::set<long>& wids);

  /**
   * Build a data frame for use with interpolators.
   */
  std::shared_ptr<Tgs::DataFrame> _buildDataFrame(Status s) const;

  std::shared_ptr<Tgs::Interpolator> _buildInterpolator(Status s) const;

  const Match& _findMatch(long nid1, long nid2);

  std::shared_ptr<Tgs::Interpolator> _readInterpolator(QIODevice& is);

  geos::geom::Coordinate _translate(const geos::geom::Coordinate& c, Status s);

  void _writeInterpolator(const std::shared_ptr<const Tgs::Interpolator>& interpolator,
                          QIODevice& os) const;

  friend class SearchRadiusCalculatorTest;
  friend class RubberSheetTest;
};

}

#endif // RUBBERSHEET_H
