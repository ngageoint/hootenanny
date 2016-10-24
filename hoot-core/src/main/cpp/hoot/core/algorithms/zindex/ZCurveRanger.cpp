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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ZCurveRanger.h"

//Std Includes
#include <math.h>
#include <queue>

//Hoot Inlcudes
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

//Boost Includes
#include <boost/make_shared.hpp>

namespace hoot
{
class LongBox;
/**
* Used when experimenting with various range decomposition approaches.
*/
class LongBoxContainer
{
public:

  LongBoxContainer() {}

  LongBoxContainer(LongBox box, long int excess)
  {
    _box = box;
    _excess = excess;
  }

  bool operator=(LongBoxContainer bc) const
  {
    if (_excess != bc.getExcess())
    {
       return false;
    }
    if (_box.getMin().size() != bc.getBox().getMin().size() || _box.getMax().size() != bc.getBox().getMax().size())
    {
      return false;
    }
    vector<long int> bcMin = bc.getBox().getMin();
    vector<long int> bcMax = bc.getBox().getMax();
    for (uint i = 0; i < _box.getMin().size(); i++)
    {
      long int value = _box.getMin()[i];
      if (std::find(bcMin.begin(), bcMin.end(), value) == bcMin.end() )
      {
        return false;
      }
    }
    for (uint i = 0; i < _box.getMax().size(); i++)
    {
      long int value = _box.getMax()[i];
      if (std::find(bcMax.begin(), bcMax.end(), value) == bcMax.end() )
      {
        return false;
      }
    }
    return true;
  }

  int hashCode()
  {
    return (int) (_excess ^ (_excess >> 32));
  }

  bool operator<(const LongBoxContainer& o) const
  {
    return getExcess() < o.getExcess();
  }

  LongBox getBox() const { return _box; }

  long int getExcess() const { return _excess; }

  bool isPerfectFit() { return getExcess() == 0; }

  QString toString()
  {
    return QString::number(_excess) + _box.toString();
  }

private:
  LongBox _box;
  long int _excess;
};

struct range_sort
{
  bool operator() (Range a, Range b) { return (a.getMin() < b.getMin());}
} range_object;

ZCurveRanger::ZCurveRanger(const ZValue& zv)
{
  _zv = zv;
  //completely arbitrary.
  _slop = 5;
}

vector<shared_ptr<LongBox> > ZCurveRanger::breakBox(shared_ptr<LongBox> box)
{
  int bestMaxBit = 0;
  int bestD = 0;

  // go through all the dimensions and find the one that has the most
  // significant binary breaks from min to max.
  for (int d = 0; d < _zv.getDimensions(); d++)
  {
    long int v1 = box->getMin()[d];
    long int v2 = box->getMax()[d];

    int maxBit = getMaxBitColumn(v1 ^ v2);
    // the later dimensions have a more significant bits so we'll split
    // on >=
    if ((maxBit >= bestMaxBit) && (v1 < v2))
    {
       bestMaxBit = maxBit;
       bestD = d;
     }
   }

   long int splitPoint = getSplitValue(box->getMin()[bestD], box->getMax()[bestD]);

   // if there aren't any good splits
   vector<shared_ptr<LongBox> > result;
   if (splitPoint == -1)
   {
     result.push_back(box->copy());
   }
   else
   {
     result.reserve(2);
     result.push_back(box->copy());
     vector<long int> rMax = result[0]->getMax();
     rMax[bestD] = splitPoint - 1;
     result[0]->setMax(rMax);
     result.push_back(box->copy());
     vector<long int> rMin = result[1]->getMin();
     rMin[bestD] = splitPoint;
     result[1]->setMin(rMin);
   }

   return result;
}

long int ZCurveRanger::calculateExcess(shared_ptr<LongBox> box)
{
  long int startSize = _toRange(box).calculateSize();
  return startSize - box->calculateVolume();
}

vector<shared_ptr<LongBox> > ZCurveRanger::decomposeBox(shared_ptr<LongBox> box, int level)
{
  vector<shared_ptr<LongBox> > result;

  // break the input box on major z-curve boundaries
  vector<shared_ptr<LongBox> > children = breakBox(box);

  // if there is only one child we're done.
  if (children.size() == 1)
  {
    result.push_back(children[0]);
  }
  // if we've decomposed enough (eh' stinky!)
  else if (level <= 0) {
    result = children;
  }
  // if we've still got some decomposition to do
  else {
    for (uint i = 0; i < children.size(); i++)
    {
      vector<shared_ptr<LongBox> > aChildren = decomposeBox(children[i], level - 1);
      result.insert(result.end(),aChildren.begin(),aChildren.end());
    }
  }

  return result;
}

int ZCurveRanger::getMaxBitColumn(long int v)
{
  int column = -1;
  while ((v != 0) && (column < 100))
  {
     column++;
     v = v >> 1;
  }

  if (column >= 100)
  {
    throw HootException("MaxBitColumn >= 100!");
  }
  return column;
}

long int ZCurveRanger::getSplitValue(long int v1, long int v2)
{
  if ((v2 - v1) < 1)
  {
    return -1;
  }

  long int d = v1 ^ v2;

  int maxBit = getMaxBitColumn(d);

  long mask = 0;
  for (int i = 0; i < maxBit; i++)
  {
    mask = (mask << 1) | 1;
  }
  mask = ~mask;
  return v2 & mask;
}

vector<Range> ZCurveRanger::decomposeRange(BBox box, BBox focusBox, int levels)
{
  return _decomposeRange(_toLongBox(box), _toLongBox(focusBox), levels);
}

vector<Range> ZCurveRanger::decomposeRange(BBox box, int levels)
{
   return decomposeRange(_toLongBox(box), levels);
}

vector<Range> ZCurveRanger::decomposeRange(LongBox box, int levels)
{
  vector<shared_ptr<LongBox> > boxes = decomposeBox(make_shared<LongBox>(_clipBox(box)), levels);

  vector<Range> result;
  result.reserve((boxes.size()));
  for (uint i = 0; i < boxes.size(); i++)
  {
    result.push_back(_toRange(boxes[i]));
  }

  return _condenseRanges(result);
}

vector<Range> ZCurveRanger::decomposeRangeIterative(BBox box, int count)
{
  return _decomposeRangeIterative(_toLongBox(box), count);
}

bool ZCurveRanger::rangeCoversIdentity(Range r)
{
  vector<long int> min;
  min.reserve(_zv.getDimensions());

  vector<long int> max;
  max.reserve(_zv.getDimensions());

  _zv.decompose(r.getMin(), min);
  _zv.decompose(r.getMax(), max);

  long int int1 = min[0];
  long int int2 = max[0];

  for (uint i = 1; i < min.size(); i++)
  {
    int1 = std::max(int1, min[i]);
    int2 = std::min(int2, max[i]);
  }

  return int2 >= int1;
}

vector<Range> ZCurveRanger::_decomposeRange(LongBox box, LongBox focusBox, int levels)
{
  vector<shared_ptr<LongBox> > boxes = decomposeBox(make_shared<LongBox>(box), levels/2);

  for (int i = 0; i < (levels * 2); i++)
  {
    vector<shared_ptr<LongBox> > newBoxes;
    for (uint j = 0; j < boxes.size(); j++)
    {
      shared_ptr<LongBox> b = boxes[i];
      if (b->intersects(focusBox) && (calculateExcess(b) > 0))
      {
        vector<shared_ptr<LongBox> > rv = decomposeBox(b, 0);
        newBoxes.insert(newBoxes.end(), rv.begin(), rv.end());
      }
      else
      {
        newBoxes.push_back(b);
      }
    }
    boxes = newBoxes;
  }

  vector<Range> result;
  result.reserve(boxes.size());
  for (uint i = 0; i < boxes.size(); i++)
  {
    result[i] = _toRange(boxes[i]);
  }

  return _condenseRanges(result);
}

vector<Range> ZCurveRanger::_decomposeRangeIterative(LongBox box, int count)
{
  priority_queue<LongBoxContainer> pq;
  pq.push(LongBoxContainer(box, calculateExcess(make_shared<LongBox>(box))));

  vector<LongBox> completed;
  while ((!pq.empty()) && (((int)pq.size() + (int)completed.size()) < count))
  {
    LongBoxContainer lbc = pq.top();
    pq.pop();

    if (lbc.isPerfectFit())
    {
      completed.push_back(lbc.getBox());
    }
    else
    {
      vector<shared_ptr<LongBox> > boxes = decomposeBox(make_shared<LongBox>(lbc.getBox()), 0);

      if (boxes.size() == 1)
      {
        completed.push_back(*boxes[0].get());
      }
      else if (boxes.size() == 2)
      {
        pq.push(LongBoxContainer(*boxes[0].get(), calculateExcess(boxes[0])));
        pq.push(LongBoxContainer(*boxes[1].get(), calculateExcess(boxes[1])));
      }
      else
      {
        throw new HootException("Invalid boxes.size = " + boxes.size());
      }
    }
  }

  while (!pq.empty())
  {
    LongBoxContainer lbc = pq.top();
    completed.push_back(lbc.getBox());
  }

  vector<Range> result;
  result.reserve(completed.size());
  for (uint i = 0; i < completed.size(); i++)
  {
    result[i] = _toRange(make_shared<LongBox>(completed[i]));
  }

  return _condenseRanges(result);
}

LongBox ZCurveRanger::_clipBox(LongBox box)
{
  LongBox result = *box.copy().get();

  for (int i = 0; i < result.getDimensions(); i++)
  {
    result.getMin()[i] = max(0, (int)result.getMin()[i]);
    result.getMax()[i] = min(_zv.getMaxDimensionRange(), result.getMax()[i]);
  }
  return result;
}

vector<Range> ZCurveRanger::_condenseRanges(vector<Range> r)
{
  sort(r.begin(), r.end(), range_object);

  vector<Range> result;
  result.push_back(r[0]);
  for (uint i = 1; i < r.size(); i++)
  {
    if ((r[i].getMin() - result[result.size() - 1].getMax()) <= _slop)
    {
      result[(result.size() - 1)].set(result[(result.size() - 1)].getMin(), r[i].getMax());
    }
    else
    {
      result.push_back(r[i]);
    }
  }
  return result;
}

Range ZCurveRanger::_toRange(shared_ptr<LongBox> box)
{
  vector<long int> scratch;
  scratch.reserve(_zv.getDimensions());
  for (int i = 0; i < _zv.getDimensions(); i++)
  {
    if (i < (int)box->getMin().size())
    {
      scratch.push_back(box->getMin()[i]);
    }
  }
  long int min = _zv.calculate(scratch);

  for (int i = 0; i < _zv.getDimensions(); i++)
  {
    if (i < (int)box->getMax().size())
    {
      scratch[i] = box->getMax()[i];
    }
  }

  Range result(min, _zv.calculate(scratch));
  return result;
}

LongBox ZCurveRanger::_toLongBox(BBox box)
{
  vector<long int> min;
  min.reserve(box.getDimensions());

  vector<long int> max;
  max.reserve(box.getDimensions());

  for (int i = 0; i < box.getDimensions(); i++)
  {
    min.push_back(_zv.calculateComponent(box.getMin()[i], i));
    max.push_back(_zv.calculateComponent(box.getMax()[i], i));
  }

  return LongBox(min, max);
}

}

