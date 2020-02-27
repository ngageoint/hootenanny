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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MATCHCREATOR_H
#define MATCHCREATOR_H

// Hoot
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/info/CreatorDescription.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/criterion/FilteredByCriteria.h>

// Standard
#include <string>
#include <vector>

// Qt
#include <QString>

namespace hoot
{

class MatchCreator : public FilteredByCriteria
{
public:

  static std::string className() { return "hoot::MatchCreator"; }

  virtual ~MatchCreator() {}

  /**
   * Given two elements, create a match if it is appropriate. If it is not appropriate then return
   * null.
   */
  virtual MatchPtr createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2) = 0;

  /**
   * Search the provided map for building matches and add the matches to the matches vector.
   */
  virtual void createMatches(const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& matches,
    ConstMatchThresholdPtr threshold) = 0;

  /**
   * Generally this just returns the class name of this creator. However, creators that take
   * arguments to specify scripts such as the ScriptMatchCreator may return multiple results.
   */
  virtual std::vector<CreatorDescription> getAllCreators() const = 0;

  /**
   * Determines whether an element is a candidate for matching for this match creator
   *
   * @param element element to determine the match candidate status of
   * @param map the map the element whose candidacy is being determined belongs to
   * @return true if the element is a match candidate; false otherwise
   */
  virtual bool isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& map) = 0;

  virtual std::shared_ptr<MatchThreshold> getMatchThreshold() = 0;

  virtual void setCriterion(const ElementCriterionPtr& filter) { _filter = filter; }

  /**
   * Arguments are passed in by the MatchFactory.
   */
  virtual void setArguments(QStringList /*args*/)
  {
    throw HootException("This match creator takes no arguments.");
  }

  /*
   * This is actually being done in order to track the script name in ScriptMatchCreator, so we
   * need to do some refactoring to get rid of this.  Could be redundant with the
   * CreatorDescription class.
   */
  QString getDescription() const { return _description; }

  /**
   * Returns the name of the match creator
   *
   * @return a name string
   */
  virtual QString getName() const = 0;

  /**
   * @see FilteredByCriteria
   */
  virtual QStringList getCriteria() const = 0;

protected:

  QString _description;
  // allows for matching only a subset of features in the input data
  // Unfortunately, each match creator will need to add explicit logic to use this filter in their
  // isMatchCandidate method (if they call into a match visitor, then in that class's
  // isMatchCandidate method).  Couldn't find any clean way to add that to this base class, given
  // the way that each match creator calls into a visitor to parse potential matches
  ElementCriterionPtr _filter;
};

typedef std::shared_ptr<MatchCreator> MatchCreatorPtr;
typedef std::shared_ptr<const MatchCreator> ConstMatchCreatorPtr;

}

#endif // MATCHCREATOR_H
