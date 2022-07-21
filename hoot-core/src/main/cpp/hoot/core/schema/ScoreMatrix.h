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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef SCOREMATRIX_H
#define SCOREMATRIX_H

// Standard
#include <cassert>
#include <queue>

//  Hoot
#include <hoot/core/scoring/TextTable.h>

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
template<typename _type>
class ScoreMatrix
{
public:

  ScoreMatrix() = default;
  ScoreMatrix(int width, int height) { resize(width, height); }

  void resize(size_t i, size_t j);

  _type get(size_t i, size_t j) const { return _v[i][j]; }
  _type& operator() (size_t i, size_t j) { return _v[i][j]; }
  _type& operator() (size_t i, size_t j) const { return _v[i][j]; }

  size_t getHeight() const { return _v[0].size(); }

  size_t getWidth() const { return _v.size(); }

  void set(size_t i, size_t j, _type v) { assert(i < _v.size() && j < _v[0].size()); _v[i][j] = v; }

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
  double meanScore(double portion = 1.0) const;

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
  _type minSumScore() const;

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
  _type sumCells() const;

  void setLabels(const std::vector<QString>& labelRows, const std::vector<QString>& labelCols)
  {
    _labelRows = labelRows;
    _labelCols = labelCols;
  }

  /**
   * Clears the underlying data from the matrix
   */
  void clear()
  {
    _v.clear();
    _labelCols.clear();
    _labelRows.clear();
  }

  /**
   * Returns a text table representation of the matrix
   *
   * @return a text table
   */
  TextTable toTextTable() const;

  /**
   * Returns a text table string representation of the matrix
   *
   * @return a string
   */
  QString toTableString() const;

  /**
   * Returns a text table JSON string representation of the matrix
   *
   * @return a JSON string
   */
  QString toJsonString() const;

private:

  struct Entry
  {
    size_t i, j;
    _type score;

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

  std::vector<std::vector<_type>> _v;
  std::vector<QString> _labelRows;
  std::vector<QString> _labelCols;
};

template<typename _type>
void ScoreMatrix<_type>::resize(size_t rows, size_t cols)
{
  _v.resize(rows);

  for (size_t i = 0; i < rows; i++)
    _v[i].resize(cols, (_type)0.0);
}

template<typename _type>
double ScoreMatrix<_type>::meanScore(double portion) const
{
  std::priority_queue<Entry, std::deque<Entry>, Entry> heap;

  if (_v.size() == 0)
  {
    QString msg("Attempting to calculate the mean score on an empty matrix.");
    throw HootException(msg);
  }

  Entry e;
  for (size_t i = 0; i < _v.size(); i++)
  {
    e.i = i;
    for (size_t j = 0; j < _v[0].size(); j++)
    {
      e.j = j;
      e.score = _v[i][j];
      heap.push(e);
    }
  }

  int scoreCount = std::max(1, static_cast<int>(static_cast<double>(std::min(_v.size(), _v[0].size())) * portion));
  int weight = scoreCount;
  double score = 0.0;

  std::set<int> used1;
  std::set<int> used2;

  while (scoreCount > 0)
  {
    assert(heap.size() > 0);

    e = heap.top();
    heap.pop();
    if (used1.find(static_cast<int>(e.i)) == used1.end() && used2.find(static_cast<int>(e.j)) == used2.end())
    {
      LOG_TRACE("  " << e.i << ", " << e.j << " " << e.score);
      score += e.score;
      used1.insert(static_cast<int>(e.i));
      used2.insert(static_cast<int>(e.j));
      scoreCount--;
    }
  }

  if (weight > 0)
    score /= (double)weight;

  return score;
}

template<typename _type>
_type ScoreMatrix<_type>::minSumScore() const
{
  return this->multiplyCells(this->minSumMatrix()).sumCells();
}

template<typename _type>
ScoreMatrix<_type> ScoreMatrix<_type>::minSumMatrix() const
{
  std::priority_queue<Entry, std::deque<Entry>, EntryMax> heap;

  Entry e;
  for (size_t i = 0; i < _v.size(); i++)
  {
    e.i = i;
    for (size_t j = 0; j < _v[0].size(); j++)
    {
      e.j = j;
      if (i != 0 || j != 0)
      {
        e.score = _v[i][j];
        heap.push(e);
      }
    }
  }

  _type score = (_type)0.0;

  std::set<int> used1;
  std::set<int> used2;

  ScoreMatrix<_type> result(static_cast<int>(getWidth()), static_cast<int>(getHeight()));
  while (heap.size() > 0)
  {
    e = heap.top();
    heap.pop();
    if (used1.find(static_cast<int>(e.i)) == used1.end() && used2.find(static_cast<int>(e.j)) == used2.end())
    {
      score += e.score;
      result.set(e.i, e.j, 1.0);
      // the first row and column can be used multiple times.
      if (e.i != 0)
        used1.insert(static_cast<int>(e.i));
      if (e.j != 0)
        used2.insert(static_cast<int>(e.j));
    }
  }
  return result;
}

template<typename _type>
ScoreMatrix<_type> ScoreMatrix<_type>::multiplyCells(const ScoreMatrix& other) const
{
  assert(getWidth() == other.getWidth() && getHeight() == other.getHeight());
  ScoreMatrix result(static_cast<int>(getWidth()), static_cast<int>(getHeight()));
  for (size_t i = 0; i < getWidth(); i++)
  {
    for (size_t j = 0; j < getHeight(); j++)
      result.set(i, j, get(i, j) * other.get(i, j));
  }
  return result;
}

template<typename _type>
_type ScoreMatrix<_type>::sumCells() const
{
  _type result = (_type)0.0;
  for (size_t i = 0; i < getWidth(); i++)
  {
    for (size_t j = 0; j < getHeight(); j++)
      result += get(i, j);
  }
  return result;
}

template<typename _type>
TextTable ScoreMatrix<_type>::toTextTable() const
{
  TextTable::Data d;
  for (size_t i = 0; i < getWidth(); i++)
  {
    QString x = QString::number(i);
    if (_labelRows.size() > i)
      x = _labelRows[i];
    for (size_t j = 0; j < getHeight(); j++)
    {
      QString y = QString::number(j);
      if (_labelCols.size() > j)
        y = _labelCols[j];
      d[x][y] = QVariant(QString("%1").arg(get(i, j)));
    }
  }

  return TextTable(d);
}

template<typename _type>
QString ScoreMatrix<_type>::toTableString() const
{
  return toTextTable().toWikiString();
}

template<typename _type>
QString ScoreMatrix<_type>::toJsonString() const
{
  return toTextTable().toJsonString();
}

}

#endif // SCOREMATRIX_H
