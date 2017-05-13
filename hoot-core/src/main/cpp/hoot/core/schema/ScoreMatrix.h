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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SCOREMATRIX_H
#define SCOREMATRIX_H

// Qt
#include <QString>

// Standard
#include <cassert>
#include <vector>

namespace hoot
{

/**
 * A matrix of scores where the scores usually represent tag or name combinations. E.g. if you have
 * two names "Hotel California" and "California Hostel in Hollywood". You may assign the values s/t
 *
 *            | California | Hostel | in | Hollywood
 * Hotel      | 0.02       | 0.9    | 0  | 0.2
 * California | 1.0        | 0.02   | 0  | 0.01
 */
class ScoreMatrix
{
public:
  ScoreMatrix(int width, int height);

  double get(size_t i, size_t j) const { return _v[i][j]; }

  size_t getHeight() const { return _v[0].size(); }

  size_t getWidth() const { return _v.size(); }

  void set(size_t i, size_t j, double v) { assert(i < _v.size() && j < _v[0].size()); _v[i][j] = v; }

  /**
   * Uses each row or col at most one time to calculate a reasonable estimate of the maximum
   * mean score. In the above example:
   * California & California = 1.0
   * Hotel & Hostel = 0.9
   *
   * Mean = 0.95. In this case we only do two values since the largest dimension is two. A greedy
   * search is used to determine the mean.
   *
   * @param portion The portion of the scores that should be evaluated. A value of 0 will give you
   *  just the max score. A value of 1.0 will give you the mean of all scores. 0.5 will just give
   *  you the top half of scores.
   */
  double meanScore(double portion=1.0) const;

  /**
   * Uses each row or col at most one time to calculate a reasonable estimate of the minimum sum of
   * values. Typically this is useful if the score represents distance instead of similarity.
   * The first row and column are special in that they represent the unassigned weight of a row/col.
   * A word may be matched to unassigned multiple times.
   *
   * In this example:
   *
   *              | <unassigned> | California | Hostel | in  | Hollywood
   * <unassigned> | -            | .8         | 1      | 0.5 | 1
   * Hotel        | 1            | 0.98       | 0.1    | 1   | 0.8
   * California   | 0.8          | 0.0        | 0.98   | 1   | 0.99
   *
   * California & California = 0.0
   * Hotel & Hostel = 0.1
   * in (unassigned) = 0.5
   * Hollywood (unassigned) = 1.0
   *
   * Result: 1.6
   */
  double minSumScore() const;

  /**
   * Similar to above, but returns a matrix where there is a 1 in each cell that represents a chosen
   * element.
   */
  ScoreMatrix minSumMatrix() const;

  /**
   * This is not matrix multiplication!!
   *
   * This multiplies the values in this cells against the corresponding values in the other cells.
   * In other words:
   *
   * result(i,j) = this(i,j) * other(i,j)
   */
  ScoreMatrix multiplyCells(const ScoreMatrix& other) const;

  /**
   * @brief Return the sum of all cells.
   */
  double sumCells() const;

  QString toTableString() const;

private:
  struct Entry
  {
    size_t i, j;
    double score;

    bool operator()(const Entry& left, const Entry& right) const
    {
      return left.score < right.score;
    }

    QString toString() const
    {
      return QString("%1, %2: %3").arg(i).arg(j).arg(score);
    }
  };

  struct EntryMax
  {
    bool operator()(const Entry& left, const Entry& right) const
    {
      return right.score < left.score;
    }
  };

  std::vector< std::vector<double> > _v;

};

}

#endif // SCOREMATRIX_H
