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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef POIPOLYGONMERGER_H
#define POIPOLYGONMERGER_H

// hoot
#include <hoot/core/conflate/merging/MergerBase.h>

namespace hoot
{

class ElementCriterion;
class TagMerger;

/**
 * Merges POIs with polygons
 *
 * A note about a tag merging inconsistency when auto-merging poi matches:
 *
 * The default tag merging behavior is to keep all poly type tags over poi type tags when there are
 * conflicts (OverwriteTagMerger), so by default poi type tags may be dropped.  When
 * _autoMergeManyPoiToOnePolyMatches is set to true tag merging uses the PreserveTypesTagMerger
 * instead, which will put all poi type tags that are unrelated to the poly type tag into an
 * alt_tags field, so none of the types are lost. The exception to this is when the Attribute
 * Conflation workflow is being run. For AC, we favor the POI tags during conflicts instead of the
 * building tags (with some configurable exceptions).
 *
 * See
 * https://github.com/ngageoint/hootenanny/files/607197/Hootenanny.-.POI.to.Polygon.2016-11-15.pptx
 * for more details.
 */
class PoiPolygonMerger : public MergerBase
{

public:

  static QString className() { return "PoiPolygonMerger"; }

  PoiPolygonMerger();
  ~PoiPolygonMerger() = default;
  /**
   * Constructed with a set of element matching pairs. The pairs are generally Unknown1 as first
   * and Unknown2 as second.
   */
  explicit PoiPolygonMerger(const std::set<std::pair<ElementId, ElementId>>& pairs);

  void apply(const OsmMapPtr& map,
             std::vector<std::pair<ElementId, ElementId>>& replaced) override;

  QString toString() const override;

  /**
   * Utility method for merging a single POI with a single polygon
   *
   * The map passed may or may not contain constituent elements (way nodes, relation members).

   * @param map a map containing the two features to merge
   * @return the ID of the element that was merged into
   */
  static ElementId mergeOnePoiAndOnePolygon(OsmMapPtr map);

  QString getDescription() const override { return "Merges POIs into polygons"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  void setTagMergerClass(const QString& className) { _tagMergerClass = className; }

private:

  static int logWarnCount;

  // This will cause multiple poi matches to get auto-merged vs being reviewed against the poly.
  bool _autoMergeManyPoiToOnePolyMatches;
  // Allows for setting the tag merger from unit tests. Mergers are set up to support Configurable,
  // so easier to specify the tag merge this way for now.
  QString _tagMergerClass;
  // this is the tag merger that is used throughout poi/poly merging
  std::shared_ptr<TagMerger> _tagMerger;

  bool _writeDebugMaps;

  ElementId _mergeBuildings(const OsmMapPtr& map, std::vector<ElementId>& buildings1,
                            std::vector<ElementId>& buildings2,
                            std::vector<std::pair<ElementId, ElementId>>& replaced) const;

  Tags _mergePoiTags(const OsmMapPtr& map, Status s);
  std::shared_ptr<TagMerger> _getTagMerger();

  std::vector<ElementId> _getBuildingParts(const OsmMapPtr& map, Status s) const;

  static ElementId _getElementIdByType(OsmMapPtr map, const ElementCriterion& typeCrit);

  /*
   * This attempts to get replace Invalid and Conflated statuses with Unknown statuses as those are
   * the only types of statuses that can be merged by anything that calls the BuildingMerger.
   */
  static void _fixStatuses(OsmMapPtr map, const ElementId& poiId, const ElementId& polyId);
};

}

#endif // POIPOLYGONMERGER_H
