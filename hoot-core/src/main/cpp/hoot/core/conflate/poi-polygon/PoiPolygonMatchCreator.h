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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POIPOLYGONMATCHCREATOR_H
#define POIPOLYGONMATCHCREATOR_H

// hoot
#include <hoot/core/algorithms/subline-matching/SublineStringMatcher.h>
#include <hoot/core/conflate/highway/HighwayClassifier.h>
#include <hoot/core/conflate/matching/MatchCreator.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonRfClassifier.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPoiCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPolyCriterion.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonInfoCache.h>

namespace hoot
{

class PoiPolygonMatchCreator : public MatchCreator
{

public:

  static std::string className() { return "hoot::PoiPolygonMatchCreator"; }

  PoiPolygonMatchCreator();

  virtual MatchPtr createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2) override;

  /**
   * Search the provided map for POI/Polygon matches and add the matches to the matches vector.
   */
  virtual void createMatches(const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& matches,
                             ConstMatchThresholdPtr threshold) override;

  virtual std::vector<CreatorDescription> getAllCreators() const override;

  /**
   * Determines whether an element is a candidate for matching for this match creator
   *
   * @param element element to determine the match candidate status of
   * @param map the map the element whose candidacy is being determined belongs to
   * @return true if the element is a match candidate; false otherwise
   */
  virtual bool isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& map) override;

  virtual std::shared_ptr<MatchThreshold> getMatchThreshold() override;

  virtual QString getName() const { return QString::fromStdString(className()); }

  /**
   * @see FilteredByCriteria
   */
  virtual QStringList getCriteria() const;

private:

  std::shared_ptr<MatchThreshold> _matchThreshold;
  std::shared_ptr<PoiPolygonRfClassifier> _getRf();

  std::shared_ptr<PoiPolygonRfClassifier> _rf;

  PoiPolygonPoiCriterion _poiCrit;
  PoiPolygonPolyCriterion _polyCrit;

  PoiPolygonInfoCachePtr _infoCache;

  /*
   * For any 2:1 matches/reviews, will only keep the match with the smallest distance between
   * matched features
   */
  int _retainClosestDistanceMatchesOnly(
    std::vector<ConstMatchPtr>& matches, const ConstOsmMapPtr& map);
  /*
   * Called by _retainClosestDistanceMatchesOnly; 2:1 POI to poly and 2:1 polyl to POI matches are
   * processed separately
   */
  int _retainClosestDistanceMatchesOnlyByType(
    std::vector<ConstMatchPtr>& matches, const ConstOsmMapPtr& map, const bool processPois);

  /*
   * Organizes matches with key=element's ID and value=match its associated with; one
   * element may be involved in more than one match
   */
  QMultiMap<ElementId, ConstMatchPtr> _indexMatchesById(
    const std::vector<ConstMatchPtr>& matches, const QString& matchTypeStr);

  /*
   * Finds all instances where an element is involved in more than one match; returns a collection
   * with the element's ID and all the matches its involved with
   */
  QMap<ElementId, QList<ConstMatchPtr>> _getOverlappingMatches(
    const QMultiMap<ElementId, ConstMatchPtr>& matchesById, const QString& matchTypeStr);

  /*
   * Cycles through each overlapping match and keeps only the single match associated with each
   * element that has the shortest distance between POI and polygon
   */
  std::vector<ConstMatchPtr> _filterOutNonClosestMatches(
    const QMap<ElementId, QList<ConstMatchPtr>>& overlappingMatches,
    const std::vector<ConstMatchPtr>& allMatches, const ConstOsmMapPtr& map,
    const QString& matchTypeStr);

  //debugging only methods
  bool _containsMatch(
    const ElementId& elementId1, const ElementId& elementId2,
    const std::vector<ConstMatchPtr>& matches) const;
  int _numMatchesContainingElement(const ElementId& elementId,
                                   const std::vector<ConstMatchPtr>& matches) const;
};

}

#endif // POIPOLYGONMATCHCREATOR_H
