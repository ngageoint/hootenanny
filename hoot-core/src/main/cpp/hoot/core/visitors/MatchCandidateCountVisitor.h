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
#ifndef MATCH_CANDIDATE_COUNT_VISITOR_H
#define MATCH_CANDIDATE_COUNT_VISITOR_H

// hoot
#include <hoot/core/conflate/stats/DataProducer.h>
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>
#include <hoot/core/info/SingleStatistic.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

class MatchCreator;

/**
 * Calculates a count of all elements that are candidates for matches given a set
 * of match creators.
 */
class MatchCandidateCountVisitor : public ElementConstOsmMapVisitor, public SingleStatistic,
                                   public DataProducer
{
public:

  static std::string className() { return "hoot::MatchCandidateCountVisitor"; }

  MatchCandidateCountVisitor(const std::vector<std::shared_ptr<MatchCreator>>& matchCreators);

  virtual void visit(const std::shared_ptr<const Element>& e);

  double getStat() const { return _totalCandidateCount; }

  boost::any getData() const { return _matchCandidateCountsByMatchCreator; }

  virtual QString getDescription() const
  { return "Counts all elements that are candidates for matches given a set of match creators"; }

  virtual QString getInitStatusMessage() const { return "Counting match candidates..."; }

  virtual QString getCompletedStatusMessage() const
  {
    return "Counted " + StringUtils::formatLargeNumber(_totalCandidateCount) + " match candidates.";
  }

  virtual std::string getClassName() const { return className(); }

private:

  QMap<QString, std::shared_ptr<MatchCreator>> _matchCreatorsByName;
  long _totalCandidateCount;
  QMap<QString, long> _matchCandidateCountsByMatchCreator;

  void _setupCreators(const std::vector<std::shared_ptr<MatchCreator>>& matchCreators);
};

}

#endif // MATCH_CANDIDATE_COUNT_VISITOR_H
