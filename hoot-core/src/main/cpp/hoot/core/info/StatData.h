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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#ifndef STAT_DATA_H
#define STAT_DATA_H

#include <hoot/core/info/SingleStat.h>

namespace hoot
{

/**
 * Represents a single statistic that is part of generic statistics calculation processed by
 * CalculateStatsOp.
 */
class StatData
{
public:

  // Enum defining what stat value of the SingleStatistic or NumericStatistic
  // implementation of the specific visitor is being used.
  enum class StatCall
  {
    Stat,           // SingleStatistic::getStat()
    Min,            // NumericStatistic::getMin()
    Max,            // NumericStatistic::getMax()
    Average,        // NumericStatistic::getAverage()
    InfoCount,      // NumericStatistic::getInformationCount()
    InfoMin,        // NumericStatistic::getInformationMin()
    InfoMax,        // NumericStatistic::getInformationMax()
    InfoAverage,    // NumericStatistic::getInformationAverage()
    InfoDiff,       // NumericStatistic::getInformationCountDiff()
  };

  StatData() = default;

  QString getName() const { return _name; }
  QString getVisitor() const { return _visitor; }
  QString getCriterion() const { return _criterion; }
  QString getFilterCriterion() const { return _filterCriterion; }
  StatCall getStatCall() const { return _statCall; }

  void setName(const QString& name) { _name = name; }
  void setVisitor(const QString& visitor) { _visitor = visitor; }
  void setCriterion(const QString& criterion) { _criterion = criterion; }
  void setFilterCriterion(const QString& criterion) { _filterCriterion = criterion; }
  void setStatCall(const StatCall& statCall) { _statCall = statCall; }

private:

  QString _name;            // displayable name of the statistic
  QString _visitor;         // class name of the visitor used to collect the data
  QString _criterion;       // optional criterion class name used to filter the data processed by
                            // _visitor
  QString _filterCriterion; // optional filter populated with the class name of a criterion used to
                            // determine whether the statistic is generated; e.g. ConflateExecutor
                            // requires CalculateStatsOp check against this criterion to suppress
                            // generation of statistics that do not map to data types being
                            // conflated.
  StatCall _statCall;       // defines how the visitor data is being interpreted
};

}

#endif // STAT_DATA_H
