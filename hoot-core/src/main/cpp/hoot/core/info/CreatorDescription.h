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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef CREATOR_DESCRIPTON_H
#define CREATOR_DESCRIPTON_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/GeometryTypeCriterion.h>

// Standard
#include <string>
#include <vector>

// QT
#include <QString>

namespace hoot
{

/**
 * Contains descriptive information about MatchCreators and MergerCreators
 */
class CreatorDescription
{
public:

  // This is how we map various match creators to the feature
  // they operate on. Helpful when generating stats later.
  enum BaseFeatureType
  {
    POI = 0,
    Highway = 1,
    Building = 2,
    Waterway = 3,
    PoiPolygonPOI = 4, //this is a superset of POI specific to poi/poly conflation
    Polygon = 5, //polygon is a superset of building
    Area = 6,
    Railway = 7,
    PowerLine = 8,
    Point = 9,
    Line = 10,
    CollectionRelation = 11,
    Unknown = 12 // Unknown must always be last
  };

  enum FeatureCalcType
  {
    CalcTypeNone = 0,
    CalcTypeLength = 1,
    CalcTypeArea = 2
  };

  CreatorDescription();
  CreatorDescription(std::string className, QString description, bool experimental);
  CreatorDescription(std::string className, QString description, BaseFeatureType featureType,
                     bool experimental);

  bool experimental;
  std::string className;
  QString description;
  BaseFeatureType baseFeatureType;
  GeometryTypeCriterion::GeometryType geometryType;
  QStringList matchCandidateCriteria;

  static QString baseFeatureTypeToString(BaseFeatureType t);

  static BaseFeatureType stringToBaseFeatureType(QString s);

  /* These two functions, getFeatureCalcType & getElementCriterion could be pushed down into the
   * classes that are derived from MatchCreator, and that would seem logical and clean - BUT
   * ScriptMatchCreator becomes problematic, as those match creators are generated at runtime.
   * So you'd have to hard code the mappings between featureTypes and FeatureCalcTypes and the
   * ElementCriterions for each script down in the ScriptMatchCreator class. SO, rather than
   * that - we'll just keep all of this feature type stuff grouped together in one place.
   */
  static FeatureCalcType getFeatureCalcType(BaseFeatureType t);

  static std::shared_ptr<ElementCriterion> getElementCriterion(BaseFeatureType t,
                                                               ConstOsmMapPtr map);

  QString toString() const;
};

}

#endif // CREATOR_DESCRIPTON_H
