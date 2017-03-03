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
#include "ScoreMatrix.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Qt
#include <hoot/core/scoring/TextTable.h>

// Standard
#include <assert.h>
#include <deque>
#include <queue>
#include <set>

namespace hoot
{

ScoreMatrix::ScoreMatrix(int width, int height)
{
  _v.resize(width);

  for (int i = 0; i < width; i++)
  {
    _v[i].resize(height, 0.0);
  }
}

double ScoreMatrix::meanScore(double portion) const
{
  priority_queue<Entry, deque<Entry>, Entry> heap;

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

  int scoreCount = max(1, (int)(min(_v.size(), _v[0].size()) * portion));
  int weight = scoreCount;
  double score = 0.0;

  std::set<int> used1;
  std::set<int> used2;

  while (scoreCount > 0)
  {
    assert(heap.size() > 0);

    e = heap.top();
    heap.pop();
    if (used1.find(e.i) == used1.end() && used2.find(e.j) == used2.end())
    {
      LOG_TRACE("  " << e.i << ", " << e.j << " " << e.score);
      score += e.score;
      used1.insert(e.i);
      used2.insert(e.j);
      scoreCount--;
    }
  }

  if (weight > 0)
  {
    score /= (double)weight;
  }
  return score;
}

double ScoreMatrix::minSumScore() const
{
  return this->multiplyCells(this->minSumMatrix()).sumCells();
}

ScoreMatrix ScoreMatrix::minSumMatrix() const
{
  priority_queue<Entry, deque<Entry>, EntryMax> heap;

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

  double score = 0.0;

  std::set<int> used1;
  std::set<int> used2;

  ScoreMatrix result(getWidth(), getHeight());
  while (heap.size() > 0)
  {
    e = heap.top();
    heap.pop();
    if (used1.find(e.i) == used1.end() && used2.find(e.j) == used2.end())
    {
      score += e.score;
      result.set(e.i, e.j, 1.0);

      // the first row and column can be used multiple times.
      if (e.i != 0)
      {
        used1.insert(e.i);
      }
      if (e.j != 0)
      {
        used2.insert(e.j);
      }
    }
  }

  return result;
}

ScoreMatrix ScoreMatrix::multiplyCells(const ScoreMatrix& other) const
{
  assert(getWidth() == other.getWidth() && getHeight() == other.getHeight());
  ScoreMatrix result(getWidth(), getHeight());

  for (size_t i = 0; i < getWidth(); i++)
  {
    for (size_t j = 0; j < getHeight(); j++)
    {
      result.set(i, j, get(i, j) * other.get(i, j));
    }
  }

  return result;
}

double ScoreMatrix::sumCells() const
{
  double result = 0.0;

  for (size_t i = 0; i < getWidth(); i++)
  {
    for (size_t j = 0; j < getHeight(); j++)
    {
      result += get(i, j);
    }
  }

  return result;
}

QString ScoreMatrix::toTableString() const
{
  TextTable::Data d;
  for (size_t i = 0; i < getWidth(); i++)
  {
    for (size_t j = 0; j < getHeight(); j++)
    {
      d[QString::number(i)][QString::number(j)] = get(i, j);
    }
  }

  return TextTable(d).toWikiString();
}

}
