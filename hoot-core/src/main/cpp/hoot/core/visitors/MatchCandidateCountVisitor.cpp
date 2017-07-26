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
#include "MatchCandidateCountVisitor.h"

#include <hoot/core/conflate/MatchCreator.h>
#include <hoot/core/util/Log.h>

using namespace std;

namespace hoot
{

MatchCandidateCountVisitor::MatchCandidateCountVisitor(
  const vector<boost::shared_ptr<MatchCreator> >& matchCreators) :
_totalCandidateCount(0)
{
  _setupCreators(matchCreators);
}

void MatchCandidateCountVisitor::_setupCreators(
  const vector<boost::shared_ptr<MatchCreator> >& matchCreators)
{
  LOG_VARD(matchCreators.size());
  for (size_t i = 0; i < matchCreators.size(); i++)
  {
    boost::shared_ptr<MatchCreator> matchCreator = matchCreators[i];
    QString matchCreatorName;
    const QString matchCreatorDescription = matchCreator->getDescription();
    if (matchCreatorDescription.isEmpty())
    {
      matchCreatorName = QString::fromStdString(matchCreator->getAllCreators().at(0).className);
    }
    else
    {
      matchCreatorName = matchCreatorDescription;
    }
    LOG_VART(matchCreatorName);
    _matchCreatorsByName.insert(matchCreatorName, matchCreator);
  }
  LOG_VART(_matchCreatorsByName.size());
}

void MatchCandidateCountVisitor::visit(const boost::shared_ptr<const Element>& e)
{
  _totalCandidateCount = 0;

  for (QMap<QString, boost::shared_ptr<MatchCreator> >::const_iterator iterator =
       _matchCreatorsByName.begin(); iterator != _matchCreatorsByName.end(); ++iterator)
  {
    const QString matchCreatorName = iterator.key();
    LOG_VART(matchCreatorName);

    boost::shared_ptr<MatchCreator> matchCreator = iterator.value();
    if (matchCreator->isMatchCandidate(e, _map->shared_from_this()))
    {
      LOG_TRACE("is match candidate");
      if (_matchCandidateCountsByMatchCreator.contains(matchCreatorName))
      {
        _matchCandidateCountsByMatchCreator[matchCreatorName] =
          _matchCandidateCountsByMatchCreator[matchCreatorName] + 1;
      }
      else
      {
        _matchCandidateCountsByMatchCreator[matchCreatorName] = 1;
      }
      LOG_VART(_matchCandidateCountsByMatchCreator[matchCreatorName]);
    }
  }
  LOG_VART(_matchCandidateCountsByMatchCreator.size());

  for (QMap<QString, long>::const_iterator iterator = _matchCandidateCountsByMatchCreator.begin();
       iterator != _matchCandidateCountsByMatchCreator.end(); ++iterator)
  {
    //We'll handle poi and building counts separately, since there could be overlap between poi,
    //building, and poi/poly match creators.
    const QString matchCreatorName = iterator.key().toLower();
    LOG_VART(matchCreatorName);
    if (!matchCreatorName.contains("poi") && !matchCreatorName.contains("building"))
    {
      _totalCandidateCount += iterator.value();
    }
  }
  //If poi/poly match creator isn't present, then use each of the counts from the poi and building
  //match creators, if they're present.
  //This is a little brittle by not using the class name methods of the match creators, but was
  //trying to avoid a dependency on specific match creator child classes.
  if (!_matchCandidateCountsByMatchCreator.contains("hoot::PoiPolygonMatchCreator"))
  {
    if (_matchCandidateCountsByMatchCreator.contains("hoot::BuildingMatchCreator"))
    {
      _totalCandidateCount += _matchCandidateCountsByMatchCreator["hoot::BuildingMatchCreator"];
    }
    if (_matchCandidateCountsByMatchCreator.contains("hoot::ScriptMatchCreator,PoiGeneric.js"))
    {
      _totalCandidateCount +=
        _matchCandidateCountsByMatchCreator["hoot::ScriptMatchCreator,PoiGeneric.js"];
    }
  }
  //otherwise, use the poi/poly count only to avoid overlap.  it will always be >= the sum of
  //the poi and building counts
  else
  {
    _totalCandidateCount += _matchCandidateCountsByMatchCreator["hoot::PoiPolygonMatchCreator"];
  }
}

}
