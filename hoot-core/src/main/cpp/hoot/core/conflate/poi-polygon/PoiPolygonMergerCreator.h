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
#ifndef POIPOLYGONMERGERCREATOR_H
#define POIPOLYGONMERGERCREATOR_H

// hoot
#include <hoot/core/conflate/merging/MergerCreator.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPoiCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPolyCriterion.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/conflate/matching/MatchGraph.h>

namespace hoot
{

/**
 * Creates mergers to handle matches found by POI to Polygon Conflation
 */
class PoiPolygonMergerCreator : public MergerCreator, public ConstOsmMapConsumer
{

public:

  static std::string className() { return "hoot::PoiPolygonMergerCreator"; }

  PoiPolygonMergerCreator();
  virtual ~PoiPolygonMergerCreator() = default;

  /**
   * If there is one match and it is a PoiPolygonMatch then a PoiPolygonMerger is created and
   * appended. If there is more than one match and at least one is a PoiPolygonMatch then a
   * MarkForReviewMerger is created.
   */
  virtual bool createMergers(const MatchSet& matches,
                             std::vector<MergerPtr>& mergers) const override;

  virtual std::vector<CreatorDescription> getAllCreators() const override;

  virtual bool isConflicting(
    const ConstOsmMapPtr& map, ConstMatchPtr m1, ConstMatchPtr m2,
    const QHash<QString, ConstMatchPtr>& matches = QHash<QString, ConstMatchPtr>()) const override;

  virtual void setOsmMap(const OsmMap* map) override { _map = map; }

  // POI/Polygon matching is unique in that it is the only non-generic geometry type matcher that
  // can duplicate matches with other non-generic geometry type matchers (namely POI and Building).
  // One way to deal with this could be that if there are POI/Polygon matches sharing elements with
  // a POI matcher, then we could mark them as reviews before having each MergerCreator create
  // Mergers. The reason we would only care about overlapping POI matches in this situation, and not
  // building or area matches, is that PoiPolygonMerger attempts to remove a POI completely once it
  // is merged with a polygon, which could result in an orphaned node
  // (PoiPolygonInvalidReviewNodeRemover specifically deals with this issue). Since PoiPolygonMerger
  // doesn't know about the existence of any POI to POI matches which reference the POI its
  // removing, we could handle this in this method before merging features with PoiPolygonMerger.
  //
  // HOWEVER, we're favoring getting POI/Polygon matches preserved at the expense of potentially
  // losing some POI/POI reviews for now. This method is being left in here as an alternative
  // workflow from now and could be called in UnifyingConflator before the merging process, if
  // desired.

  //static void convertSharedMatchesToReviews(
    //MatchSetVector& matchSets, std::vector<MergerPtr>& mergers);

  void setAllowCrossConflationMerging(bool allow) { _allowCrossConflationMerging = allow; }

private:

  const OsmMap* _map;

  // see PoiPolygonMerger::_autoMergeManyPoiToOnePolyMatches
  bool _autoMergeManyPoiToOnePolyMatches;
  // If enabled, this prevents reviews from being generated when another type of conflation
  // (currently could only be Building Conflation) has a match that involves a polygon also
  // involved in a POI/Poly Conflation match.
  bool _allowCrossConflationMerging;

  PoiPolygonPoiCriterion _poiCrit;
  PoiPolygonPolyCriterion _polyCrit;

  MatchPtr _createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2) const;

  /*
   * Returns true if one or more matches are conflicting matches.
   */
  bool _isConflictingSet(const MatchSet& matches) const;
};

}

#endif // POIPOLYGONMERGERCREATOR_H
