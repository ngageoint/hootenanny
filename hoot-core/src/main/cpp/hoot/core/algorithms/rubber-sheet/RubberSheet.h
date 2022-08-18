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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef RUBBERSHEET_H
#define RUBBERSHEET_H

// hoot
#include <hoot/core/conflate/ConflateInfoCacheConsumer.h>
#include <hoot/core/conflate/matching/NodeMatcher.h>
#include <hoot/core/criterion/OrCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/StringUtils.h>

// tgs
#include <tgs/Interpolation/Interpolator.h>

// geos
#include <ogr_spatialref.h>

namespace hoot
{

class SearchRadiusCalculatorTest;
class RubberSheetTest;
class OsmMap;
class Status;

class RubberSheet : public OsmMapOperation, public Configurable, public ConflateInfoCacheConsumer
{
public:

  static QString className() { return "RubberSheet"; }

  /**
   * @brief refKey If this configuration setting is set to true then the first layer is treated as
   * the reference layer and will not be moved.
   */
  static QString refKey() { return "rubber.sheet.ref"; }
  /**
   * @brief debugKey If this is true then debug tags will be added to the output file.
   */
  static QString debugKey() { return "rubber.sheet.debug"; }

  struct Match
  {
    long nid1, nid2;
    double score;
    double p;
  };

  RubberSheet();
  ~RubberSheet() = default;

  std::shared_ptr<RubberSheet> clone() const { return std::make_shared<RubberSheet>(*this); }

  /**
   * @brief applyTransform applies a perviously calculated or loaded transform to the specified map.
   * @param map the map to apply the transform to
   * @return true if the operation succeeded; false otherwise
   */
  bool applyTransform(const std::shared_ptr<OsmMap>& map);
  /**
   * @brief calculateTransform calculates an appropriate transform for the specified map, but does
   * not change any data.
   * @param map the map to calculate the transform for
   * @return true if the operation succeeded; false otherwise
   */
  bool calculateTransform(const std::shared_ptr<OsmMap>& map);

  /**
   * @brief readTransform1to2 reads the data necessary to perform a transform from unknown1 to
   * unknown2.
   */
  void readTransform1to2(QIODevice& is) { _interpolator1to2 = _readInterpolator(is); }
  /**
   * @brief readTransform2to1 reads the data necessary to perform a transform from unknown1 to
   * unknown2.
   */
  void readTransform2to1(QIODevice& is) { _interpolator2to1 = _readInterpolator(is); }
  /**
   * @brief writeTransform1to2 writes out the data necessary to perform a transform from unknown1 to
   * unknown2.
   */
  void writeTransform1to2(QIODevice& os) const { _writeInterpolator(_interpolator1to2, os); }
  /**
   * @brief writeTransform2to1 writes out the data necessary to perform a transform from unknown1 to
   * unknown2.
   */
  void writeTransform2to1(QIODevice& os) const { _writeInterpolator(_interpolator2to1, os); }

  /**
   * @brief calculateTiePointDistances calculates the distances between each of the rubber sheet's
   * tie points.
   * @return a collection of distance values
   * @throws HootException if the tie points have not been created
   */
   std::vector<double> calculateTiePointDistances() const;

  void setConflateInfoCache(const std::shared_ptr<ConflateInfoCache>& cache) override
  { _conflateInfoCache = cache; }

  /**
   * @see OsmMapOperation
   */
  void apply(std::shared_ptr<OsmMap>& map) override;

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

   /**
    * @see FilteredByGeometryTypeCriteria
    */
   QStringList getCriteria() const override;

   QString getName() const override { return className(); }
   QString getClassName() const override { return className(); }
   QString getDescription() const override { return "Applies rubber sheeting to a map"; }

   /**
    * @see OperationStatus
    */
   QString getInitStatusMessage() const override { return "Rubbersheeting data..."; }

   /**
    * @see OperationStatus
    */
   QString getCompletedStatusMessage() const override
   {
     return "Rubbersheeted " + StringUtils::formatLargeNumber(_numAffected) + " / " +
            StringUtils::formatLargeNumber(_numProcessed) + " linear features.";
   }

   void setReference(bool ref) { _ref = ref; }
   void setDebug(bool debug) { _debug = debug; }
   void setMinimumTies(int minTies) { _minimumTies = minTies; }
   void setFailWhenMinimumTiePointsNotFound(bool fail) { _failWhenMinTiePointsNotFound = fail; }
   void setLogWarningWhenRequirementsNotFound(bool logWarning)
   { _logWarningWhenRequirementsNotFound = logWarning; }
   void setMaxAllowedWays(int max) { _maxAllowedWays = max; }
   void setCriteria(const QStringList& criteria, OsmMapPtr map = OsmMapPtr());

private:

  static int logWarnCount;

  using MatchList = std::map<long, std::list<Match>>;

  class Tie
  {
  public:

    // Unknown1 coordinate
    geos::geom::Coordinate c1;
    // Unknown2 coordinate
    geos::geom::Coordinate c2;

    double dx() const { return c1.x - c2.x; }
    double dy() const { return c1.y - c2.y; }

    QString toString() const
    { return "dx: " + QString::number(dx()) + ", dy: " + QString::number(dy()); }
  };

  std::shared_ptr<OsmMap> _map;

  // A map of nids to the list of matches.
  MatchList _matches;
  std::vector<Match> _finalPairs;

  // Set this to true if Unknown1 is a reference dataset and Unknown2 should be moved toward it. If
  // this is false, then the two data sets are moved toward one another.
  bool _ref;
  bool _debug;

  // The minimum number of tie points that will be used when calculating a rubber sheeting solution.
  int _minimumTies;
  std::vector<Tie> _ties;
  std::shared_ptr<Tgs::Interpolator> _interpolator1to2, _interpolator2to1;
  // used as a temporary in interpolating.
  std::vector<double> _matchPoint;
  std::shared_ptr<OGRSpatialReference> _projection;
  QString _interpolatorClassName;

  double _searchRadius;

  NodeMatcher _nm;

  bool _failWhenMinTiePointsNotFound;
  bool _logWarningWhenRequirementsNotFound;

  Match _emptyMatch;

  int _maxAllowedWays;

  OrCriterionPtr _criteria;

  // Existence of this cache tells us that elements must be individually checked to see that they
  // are conflatable given the current configuration before modifying them.
  std::shared_ptr<ConflateInfoCache> _conflateInfoCache;

  bool _calcAndApplyTransform(const OsmMapPtr& map);
  void _filterCalcAndApplyTransform(OsmMapPtr& map);

  bool _findTies();
  void _addIntersection(long nid, const std::set<long>& wids);

  /**
   * Build a data frame for use with interpolators.
   */
  std::shared_ptr<Tgs::DataFrame> _buildDataFrame(Status s) const;

  std::shared_ptr<Tgs::Interpolator> _buildInterpolator(Status s) const;
  std::shared_ptr<Tgs::Interpolator> _readInterpolator(QIODevice& is);
  void _writeInterpolator(const std::shared_ptr<const Tgs::Interpolator>& interpolator,
                          QIODevice& os) const;

  const Match& _findMatch(long nid1, long nid2);

  geos::geom::Coordinate _translate(const geos::geom::Coordinate& c, Status s);

  friend class SearchRadiusCalculatorTest;
  friend class RubberSheetTest;
};

}

#endif // RUBBERSHEET_H
