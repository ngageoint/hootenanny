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
 * @copyright Copyright (C) 2015, 2017, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef SAMPLESTATS_H
#define SAMPLESTATS_H

// Qt
#include <QString>
#include <qnumeric.h>

// Standard
#include <vector>

namespace tbs
{

class SampleStats
{
public:
  /**
   * Uses a reference to samples. If samples changes while performing calculations the results are
   * undefined.
   */
  SampleStats(const std::vector<double>& samples);

  double calculateCi90Upper();
  double calculateCi90Lower();

  double calculateMax();

  double calculateMin();

  double calculateMean();

  double calculateMedian();

  double calculateQ1() { calculateMedian(); return _q1; }
  double calculateQ3() { calculateMedian(); return _q3; }

  /**
   * Calculates the unbiased standard deviation. (1 / (N - 1))
   * As defined here: http://en.wikipedia.org/wiki/Standard_deviation
   */
  double calculateUnbiasedStandardDeviation();

  double calculateSum();

  QString toString();

private:
  const std::vector<double> &_samples;
  double _max;
  double _min;
  double _mean;
  double _median;
  double _q1, _q3;
  double _standardDeviation;
  double _sum;

  bool _isPopulated(double v) const;

};

}

#endif // SAMPLESTATS_H
