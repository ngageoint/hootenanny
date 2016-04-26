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
#ifndef MATCH_CANDIDATE_COUNT_VISITOR_H
#define MATCH_CANDIDATE_COUNT_VISITOR_H

// hoot
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>
#include <hoot/core/util/DataProducer.h>
#include <hoot/core/conflate/MatchCreator.h>
#include "SingleStatistic.h"

namespace hoot
{

/**
 * Calculates a count of all elements that are candidates for matches given a set
 * of match creators.
 */
class MatchCandidateCountVisitor : public ElementConstOsmMapVisitor, public SingleStatistic,
                                   public DataProducer
{
public:

  static string className() { return "hoot::MatchCandidateCountVisitor"; }

  MatchCandidateCountVisitor(const vector< shared_ptr<MatchCreator> >& matchCreators);

  virtual void visit(const shared_ptr<const Element>& e);

  double getStat() const { return _candidateCount; }

  any getData() const { return _matchCandidateCountsByMatchCreator; }

private:

  QMap<QString, shared_ptr<MatchCreator> > _matchCreatorsByName;
  long _candidateCount;
  QMap<QString, long> _matchCandidateCountsByMatchCreator;

  void _setupCreators(const vector< shared_ptr<MatchCreator> >& matchCreators);
};

}

#endif // MATCH_CANDIDATE_COUNT_VISITOR_H
