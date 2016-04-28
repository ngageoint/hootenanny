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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MatchCandidateCountVisitor.h"

namespace hoot
{

MatchCandidateCountVisitor::MatchCandidateCountVisitor(
  const vector< shared_ptr<MatchCreator> >& matchCreators) :
_candidateCount(0)
{
  _setupCreators(matchCreators);
}

void MatchCandidateCountVisitor::_setupCreators(const vector< shared_ptr<MatchCreator> >& matchCreators)
{
  LOG_VARD(matchCreators.size());
  for (size_t i = 0; i < matchCreators.size(); i++)
  {
    shared_ptr<MatchCreator> matchCreator = matchCreators[i];
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
     LOG_DEBUG("Appending: " + matchCreatorName);
    _matchCreatorsByName.insert(matchCreatorName, matchCreator);
  }
  LOG_VARD(_matchCreatorsByName.size());
}

void MatchCandidateCountVisitor::visit(const shared_ptr<const Element>& e)
{
  for (QMap<QString, shared_ptr<MatchCreator> >::const_iterator iterator = _matchCreatorsByName.begin();
       iterator != _matchCreatorsByName.end(); ++iterator)
  {
    const QString matchCreatorName = iterator.key();
    //LOG_VARD(matchCreatorName);
    shared_ptr<MatchCreator> matchCreator = iterator.value();
    if (matchCreator->isMatchCandidate(e, _map->shared_from_this()))
    {
      //LOG_DEBUG("is match candidate");
      if (_matchCandidateCountsByMatchCreator.contains(matchCreatorName))
      {
        _matchCandidateCountsByMatchCreator[matchCreatorName] =
          _matchCandidateCountsByMatchCreator[matchCreatorName] + 1;
      }
      else
      {
        _matchCandidateCountsByMatchCreator[matchCreatorName] = 1;
      }
      //LOG_VARD(_matchCandidateCountsByMatchCreator[matchCreatorName]);
      _candidateCount++;
    }
  }
  //LOG_VARD(_matchCandidateCountsByMatchCreator.size());
}

}
