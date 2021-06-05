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
#ifndef STAT_CALCULATOR_H
#define STAT_CALCULATOR_H

// Hoot
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

class PartialOsmMapReader;

/**
 * Calculates a single statistic against elements
 */
class StatCalculator
{
public:

  StatCalculator();
  ~StatCalculator() = default;

  /**
   * Calculates a statistic given an element visitor
   *
   * @param input path to input map containing the elements to examine
   * @param visitorClassName name of an ElementVisitor class that implements either SingleStatistic
   * or NumericStatistic
   * @param statType the type of statistic to calculate; valid types are: total, min, max, or
   * average
   * @return a numeric statistic
   */
  double calculateStat(
    const QString& input, QString& visitorClassName, const QString& statType) const;

private:

  int _taskStatusUpdateInterval;

  bool _isValidStatType(const QString& statType) const;
  std::shared_ptr<PartialOsmMapReader> _getReader(const QString& input) const;
  ElementVisitorPtr _getStatCollector(const QString& visClassName) const;
};

}

#endif // STAT_CALCULATOR_H
