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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
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
 * Contains descriptive information about MatchCreators and MergerCreators. This class bridges
 * several concepts used through out conflation and statistics generation.
 */
class CreatorDescription
{
public:

  // This is how we map various match creators to the feature they operate on. Helpful when
  // generating stats later.
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
    Relation = 11,
    Unknown = 12 // Unknown must always be last
  };

  enum FeatureCalcType
  {
    CalcTypeNone = 0,
    CalcTypeLength = 1,
    CalcTypeArea = 2
  };

  CreatorDescription();
  CreatorDescription(const QString& className, const QString& description, bool experimental);
  CreatorDescription(
    const QString& className, const QString& description, BaseFeatureType featureType,
    bool experimental);

  /**
   * Converts a base feature type to a string representation
   *
   * @param t the feature type to convert
   * @return a string
   */
  static QString baseFeatureTypeToString(BaseFeatureType t);

  /**
   * Converts the string representation of a base feature type to an enum
   *
   * @param s the string to convert
   * @return a feature type enumeration value
   */
  static BaseFeatureType stringToBaseFeatureType(QString s);

  /*
   * These two functions, getFeatureCalcType and getElementCriterion, could be pushed down into the
   * classes that are derived from MatchCreator, and that would seem logical and clean - BUT
   * ScriptMatchCreator becomes problematic, as those match creators are generated at runtime.
   * So you'd have to hard code the mappings between featureTypes and FeatureCalcTypes and the
   * ElementCriterions for each script down in the ScriptMatchCreator class. SO, rather than
   * that - we'll just keep all of this feature type stuff grouped together in one place.
   */

  /**
   * Returns the physical measurement feature stat calculation type for a base feature type e.g.
   * an area calc type is returned for polygon features
   *
   * @param t the feature type to retrieve the statistics calculation type for
   * @return a feature statistics calculation type enumeration value
   */
  static FeatureCalcType getFeatureCalcType(BaseFeatureType t);

  /**
   * Returns a GeometryTypeCriterion instantiation associated with the given feature type
   *
   * @param t a feature type
   * @param map map used to initialize the criterion
   * @return a GeometryTypeCriterion
   * @note It would be better to return a ConflatableElementCriterion here instead, but returning
   * NonBuildingAreaCriterion doesn't allow for that. Doing so also makes this inconsistent with
   * getElementCriterionName. Think if we change to AreaCriterion, however, that will throw the
   * stats off.
   */
  static std::shared_ptr<GeometryTypeCriterion> getElementCriterion(
    BaseFeatureType t, ConstOsmMapPtr map);

  /**
   * Returns the name of a ConflatableElementCriterion associated with the given feature type
   *
   * @param t a feature type
   * @return the class name of a ConflatableElementCriterion
   */
  static QString getElementCriterionName(BaseFeatureType t);

  QString toString() const;

  bool getExperimental() const { return _experimental; }
  QString getClassName() const { return _className; }
  QString getDescription() const { return _description; }
  BaseFeatureType getBaseFeatureType() const { return _baseFeatureType; }
  GeometryTypeCriterion::GeometryType getGeometryType() const { return _geometryType; }
  QStringList getMatchCandidateCriteria() const { return _matchCandidateCriteria; }

  void setExperimental(bool experimental) { _experimental = experimental; }
  void setClassName(const QString& className) { _className = className; }
  void setDescription(const QString& description) { _description = description; }
  void setBaseFeatureType(const BaseFeatureType& baseFeatureType)
  { _baseFeatureType = baseFeatureType; }
  void setGeometryType(const GeometryTypeCriterion::GeometryType& geometryType)
  { _geometryType = geometryType; }
  void setMatchCandidateCriteria(const QStringList& matchCandidateCriteria)
  { _matchCandidateCriteria = matchCandidateCriteria; }

private:

  bool _experimental;
  QString _className;
  QString _description;
  BaseFeatureType _baseFeatureType;
  GeometryTypeCriterion::GeometryType _geometryType;
  QStringList _matchCandidateCriteria;
};

}

#endif // CREATOR_DESCRIPTON_H
