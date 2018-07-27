
#ifndef CREATOR_DESCRIPTON_H
#define CREATOR_DESCRIPTON_H

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>

// Standard
#include <string>
#include <vector>

// QT
#include <QString>

namespace hoot
{

/**
 *
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
    Unknown = 9 // Unknown must always be last
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

  static QString BaseFeatureTypeToString(BaseFeatureType t);

  static BaseFeatureType StringToBaseFeatureType(QString s);

  /* These two functions, getFeatureCalcType & getElementCriterion could be pushed down into the
   * classes that are derived from MatchCreator, and that would seem logical and clean - BUT
   * ScriptMatchCreator becomes problematic, as those match creators are generated at runtime.
   * So you'd have to hard code the mappings between featureTypes and FeatureCalcTypes and the
   * ElementCriterions for each script down in the ScriptMatchCreator class. SO, rather than
   * that - we'll just keep all of this feature type stuff grouped together in one place.
   */
  static FeatureCalcType getFeatureCalcType(BaseFeatureType t);

  static boost::shared_ptr<ElementCriterion> getElementCriterion(BaseFeatureType t,
                                                                 ConstOsmMapPtr map);

  QString toString() const;
};

}

#endif // CREATOR_DESCRIPTON_H
