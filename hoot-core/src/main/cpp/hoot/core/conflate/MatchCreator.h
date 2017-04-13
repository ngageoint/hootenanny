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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MATCHCREATOR_H
#define MATCHCREATOR_H

// Hoot
#include <hoot/core/conflate/MatchThreshold.h>

// Standard
#include <string>
#include <vector>

// QT
#include <QString>

namespace hoot
{

using namespace std;
class Match;
class ElementCriterion;

class MatchCreator
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
    Unknown = 4 // Unknown must always be last
  };

  enum FeatureCalcType
  {
    CalcTypeNone = 0,
    CalcTypeLength = 1,
    CalcTypeArea = 2
  };

  static QString BaseFeatureTypeToString(BaseFeatureType t);

  static BaseFeatureType StringToBaseFeatureType(QString s);

  /* These two functions, getFeatureCalcType & getElementCriterion could be
   * pushed down into the classes that are derived from MatchCreator, and
   * that would seem logical and clean - BUT ScriptMatchCreator becomes
   * problematic, as those match creators are generated at runtime. So you'd
   * have to hard code the mappings between featureTypes and FeatureCalcTypes
   * and the ElementCriterions for each script down in the ScriptMatchCreator
   * class. SO, rather than that - we'll just keep all of this feature type
   * stuff grouped together in one place.
   */
  static FeatureCalcType getFeatureCalcType (BaseFeatureType t);

  static ElementCriterion* getElementCriterion (BaseFeatureType t, ConstOsmMapPtr map);

  class Description
  {
  public:
    Description() : experimental() {}
    Description(string className, QString description, BaseFeatureType featureType,
                bool experimental)
    {
      this->className = className;
      this->experimental = experimental;
      this->description = description;
      this->baseFeatureType = featureType;
    }

    bool experimental;
    string className;
    QString description;
    BaseFeatureType baseFeatureType;
  };

  static string className() { return "hoot::MatchCreator"; }

  virtual ~MatchCreator() {}

  /**
   * Given two elements, create a match if it is appropriate. If it is not appropriate then return
   * null.
   */
  virtual Match* createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2) = 0;

  /**
   * Search the provided map for building matches and add the matches to the matches vector.
   */
  virtual void createMatches(const ConstOsmMapPtr& map, vector<const Match*>& matches,
    ConstMatchThresholdPtr threshold) = 0;

  /**
   * Generally this just returns the class name of this creator. However, creators that take
   * arguments to specify scripts such as the ScriptMatchCreator may return multiple results.
   */
  virtual vector<Description> getAllCreators() const = 0;

  /**
   * Determines whether an element is a candidate for matching for this match creator
   *
   * @param element element to determine the match candidate status of
   * @param map the map the element whose candidacy is being determined belongs to
   * @return true if the element is a match candidate; false otherwise
   */
  virtual bool isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& map) = 0;

  virtual boost::shared_ptr<MatchThreshold> getMatchThreshold() = 0;

  /**
   * Arguments are passed in by the MatchFactory.
   */
  virtual void setArguments(QStringList /*args*/)
  {
    throw HootException("This match creator takes no argument.");
  }

  /*
   * This is actually being done in order to track the script name in ScriptMatchCreator, so we
   * need to do some refactoring to get rid of this.  Redundant with Description class above.
   */
  QString getDescription() { return _description; }
  void setDescription(QString description) { _description = description; }

protected:

  QString _description;

};

}

#endif // MATCHCREATOR_H
