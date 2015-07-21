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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MatchCandidateCountVisitor.h"

namespace hoot
{

MatchCandidateCountVisitor::MatchCandidateCountVisitor(
  const vector< shared_ptr<MatchCreator> >& matchCreators) :
_matchCreators(matchCreators),
_candidateCount(0)
{
  for (size_t i = 0; i < _matchCreators.size(); ++i)
  {
    vector<MatchCreator::Description> matchCreatorDescriptions = _matchCreators[i]->getAllCreators();
    sort(matchCreatorDescriptions.begin(), matchCreatorDescriptions.end(), _matchDescriptorCompare);
    //TODO: Grabbing the first element doesn't seem right here, but its behavior has been expected
    //so far.
    LOG_VARD(matchCreatorDescriptions.at(0).className);
     _matchCreatorDescriptions.append(
       QString::fromStdString(matchCreatorDescriptions.at(0).className));
  }

}

void MatchCandidateCountVisitor::visit(const shared_ptr<const Element>& e)
{
  for (size_t i = 0; i < _matchCreators.size(); ++i)
  {
    shared_ptr<MatchCreator> matchCreator = _matchCreators[i];
    if (matchCreator->isMatchCandidate(e, _map->shared_from_this()))
    {
     _candidateCount++;

     const QString matchCreatorName = _matchCreatorDescriptions.at(i);
     LOG_VARD(matchCreatorName);
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
    }
  }
}

bool MatchCandidateCountVisitor::_matchDescriptorCompare(const MatchCreator::Description& m1,
                                                         const MatchCreator::Description& m2)
{
  return m1.className > m2.className;
}

}
