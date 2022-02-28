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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
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

  static QString className() { return "MatchCandidateCountVisitor"; }

  MatchCandidateCountVisitor(const std::vector<std::shared_ptr<MatchCreator>>& matchCreators);
  ~MatchCandidateCountVisitor() override = default;

  /**
   * @see ElementVisitor
   */
  void visit(const std::shared_ptr<Element const>& e) override;

  double getStat() const override { return _totalCandidateCount; }

  boost::any getData() const override { return _matchCandidateCountsByMatchCreator; }

  QString getInitStatusMessage() const override { return "Counting match candidates..."; }
  QString getCompletedStatusMessage() const override
  {
    return "Counted " + StringUtils::formatLargeNumber(_totalCandidateCount) + " match candidates.";
  }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Counts all elements that are candidates for matches given a set of match creators"; }

private:

  QMap<QString, std::shared_ptr<MatchCreator>> _matchCreatorsByName;
  long _totalCandidateCount;
  QMap<QString, long> _matchCandidateCountsByMatchCreator;

  void _setupCreators(const std::vector<std::shared_ptr<MatchCreator>>& matchCreators);
};

}

#endif // MATCH_CANDIDATE_COUNT_VISITOR_H
