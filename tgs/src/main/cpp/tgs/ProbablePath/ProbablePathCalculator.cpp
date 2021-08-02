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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#include "ProbablePathCalculator.h"

// Standard Includes
#include <algorithm>
#include <iostream>
#include <math.h>
#include <map>
#include <sstream>

// Tgs
#include <tgs/StreamUtils.h>
#include <tgs/TgsException.h>
#include <tgs/Heap/JHeap.h>

using namespace std;

namespace Tgs
{

  std::ostream & operator << (std::ostream & o, const PpPoint& pp)
  {
    o << "<row: " << pp.row << ", col: " << pp.col;
    if (pp.name != "")
    {
      o << ", name: " << pp.name;
    }
    o << ">";
    return o;
  }

  std::ostream & operator << (std::ostream & o, const ProbablePathCalculator::Destination& d)
  {
    o << "<row: " << d.row << ", col: " << d.col;
    if (d.maxCost >= 0)
    {
      o << ", max cost: " << d.maxCost;
    }
    o << ">";
    return o;
  }

  class MyHeap : public TgsJHeap<float> {};

  PpPoint::PpPoint() 
  {
    cost = -1;
  }

  const static float sqrt2 = sqrt(2.0f);
  ProbablePathCalculator::ProbablePathCalculator()
    : _rows(-1),
      _cols(-1),
      _variation(0.0),
      _verbose(Warning),
      _randomGenerator(Random::instance())
  {
  }

  ProbablePathCalculator::ProbablePathCalculator(const RandomPtr& random)
    : _rows(-1),
      _cols(-1),
      _variation(0.0),
      _verbose(Warning),
      _randomGenerator(random)
  {
  }

  bool ProbablePathCalculator::_addReturnPaths(const PpPoint& source,
    const Destination& destination)
  {
    PpPoint pos = destination;
    _result[_toIndex(pos)]++;

    PpRoute route;
    route.waypoints.push_back(destination);
    float length = 0.0;
    float cost = 0.0;

    while (pos != source)
    {

      float minVal = _costSurface[_toIndex(pos)];
      PpPoint minPoint = pos;
      for (int dr = -1; dr <= 1; dr++)
      {
        for (int dc = -1; dc <= 1; dc++)
        {
          int r = pos.row + dr;
          int c = pos.col + dc;
          if (r >= 0 && r < _rows && c >= 0 && c <_cols)
          {
            float v = _costSurface[_toIndex(r, c)];
            if (v < minVal && v >= 0.0)
            {
              minVal = v;
              minPoint = PpPoint(r, c);
            }
          }
        }
      }
      // invalid route, no solution
      if (minPoint == pos)
      {
        _v() << "Found invalid route -- no solution." << endl;
        return true;
      }
      
      float fromCost = _baseValues[_toIndex(pos)];
      float toCost = _baseValues[_toIndex(minPoint)];
      float distance = _calculateDistance(minPoint, pos);
      cost += distance * ((fromCost + toCost) / 2.0f);
      // make sure we copy the name over
      if (minPoint == source)
      {
        minPoint = source;
      }
      route.waypoints.push_back(minPoint);
      length += distance;

      pos = minPoint;
      _result[_toIndex(pos)]++;
    }

    if (!route.waypoints.empty())
    {
      reverse(route.waypoints.begin(), route.waypoints.end());
      float wayPointCost = 0.0;
      route.waypoints[0].cost = 0.0;
      PpPoint from, to;
      for (unsigned int i = 1; i < route.waypoints.size(); i++)
      {
        from = route.waypoints[i - 1];
        to = route.waypoints[i];
        float fromCost = _baseValues[_toIndex(from)];
        float toCost = _baseValues[_toIndex(to)];
        float distance = _calculateDistance(from, to);
        wayPointCost += distance * ((fromCost + toCost) / 2.0f);
        route.waypoints[i].cost = wayPointCost;
      }
    }

    if (cost <= destination.maxCost || destination.maxCost < 0.0)
    {
      route.cost = cost;
      route.length = length;
      _routes.push_back(route);
      return true;
    }

    return false;
  }

  float ProbablePathCalculator::_calculateDistance(const PpPoint& p1, const PpPoint& p2) const
  {
    int dy = p1.row - p2.row;
    int dx = p1.col - p2.col;
    return sqrt((float)(dx * dx + dy * dy));
  }

  void ProbablePathCalculator::calculateProbablePaths(int iterations)
  {
    _v(Debug) << "Sources: " << _sources << endl;
    if (_result.size() != _baseValues.size())
    {
      _routes.clear();
      _result.clear();
      _result.resize(_baseValues.size(), 0);
    }
    if (iterations <= 0)
    {
      throw Exception("iterations must be > 0.");
    }
    if (iterations > 1000)
    {
      _v(Warning) << "Iterations is unreasonably large, continuing anyways..." << endl;
    }

    _v(Debug) << "Starting...\r";

    for (int i = 0; i < iterations; i++)
    {
      _v(Debug) << "Randomizing...\n";
      _randomizeCurrentValues();

      // go from each source...
      for (unsigned int s = 0; s < _sources.size(); s++)
      {
        // ...to every destination
        _resetRemainingDestinations();
        int attempt = 0;
        while (!_remainingDest.empty())
        {
          attempt++;
          // if we can't find a path in 10 tries the two are probably too far away with the given
          // time constraints and friction surface.
          if (attempt > 10)
          {
            stringstream strm;
            strm << "Made too many attempts to find destination. Too far away? " << _remainingDest
              << endl;
            throw Exception(strm.str());
          }
          _calculateShortestPath(_sources[s]);

          // we only need to randomize if the current random surface doesn't reach the 
          // destination
          if (!_remainingDest.empty())
          {
            _v(Debug) << "Randomizing...\n";
            _randomizeCurrentValues();
          }

          _v() << i + 1 << " of " << iterations << " (" << (i + 1) * 100 / iterations << "%)   \r";
        }
      }
    }

    _v() << endl;
  }

  void ProbablePathCalculator::_calculateShortestPath(const PpPoint& source)
  {
    _checkBounds(source);
    _v(Debug) << "Create Cost Surface for " << source << "...\n";
    // calculate the cost surface from the source to each destination
    _createCostSurface(source);
    int watchDog = 0;
    // add return paths from each destination to this source.
    list<Destination>::iterator it = _remainingDest.begin();
    while (it != _remainingDest.end())
    {
      watchDog++;
      if (watchDog > 1000)
      {
        throw Exception("Internal Error: Watchdog exceeded.");
      }
      _checkBounds(*it);
      _v(Debug) << "Add return paths...\n";
      if (_addReturnPaths(source, *it))
      {
        it = _remainingDest.erase(it);
      }
      else
      {
        ++it;
      }
    }
  }

  void ProbablePathCalculator::_checkBounds(const PpPoint& p) const
  {
    if (p.col < 0)
    {
      stringstream strm;
      strm << "Column is less than zero (" << p.col << ")";
      throw Exception(strm.str());
    }
    if (p.col >= _cols)
    {
      stringstream strm;
      strm << "Column is greater than " << _cols << " (" << p.col << ")";
      throw Exception(strm.str());
    }
    if (p.row < 0)
    {
      stringstream strm;
      strm << "Row is less than zero (" << p.row << ")";
      throw Exception(strm.str());
    }
    if (p.row >= _rows)
    {
      stringstream strm;
      strm << "Row is greater than " << _rows << " (" << p.row << ")";
      throw Exception(strm.str());
    }
  }

  void ProbablePathCalculator::_checkForInconsistency(const PpPoint& p, MyHeap& q)
  {
    static const int dc[] = { 0, 1, 1,  1,  0, -1, -1, -1 };
    static const int dr[] = { 1, 1, 0, -1, -1, -1,  0,  1 };
    static const float dist[] = { 1, sqrt2, 1, sqrt2, 1, sqrt2, 1, sqrt2 };

    const int row = p.row;
    const int col = p.col;
    const int index = _toIndex(p);
    const float f = _currentValues[index];
    vector<MyHeap::JHeapNode>& nodes = q.getNodes();

    // this pixel is impassible
    if (_baseValues[index] < 0)
    {
      return;
    }

    for (unsigned int i = 0; i < 8; i++)
    {
      int r = row + dr[i];
      int c = col + dc[i];
      // if it is in bounds
      if (r >= 0 && r < _rows && c >= 0 && c < _cols)
      {
        int dIndex = _toIndex(r, c);
        float dv = _costSurface[dIndex];
        // if this pixel is impassible or not calculated, ignore it
        if (_baseValues[dIndex] < 0 || dv < 0)
        {
          continue;
        }
        float df = _currentValues[dIndex];

        float newCost = dv + (f + df) / 2.0f * dist[i];
        if (newCost < f && 
          (newCost < nodes[index].getValue() || nodes[index].getValue() < 0.0))
        {
          _tmpCost[index] = newCost;
          q.push(index, newCost);
        }
      }
    }
  }

  void ProbablePathCalculator::clearRoutes()
  {
    _result.clear();
    _costSurface.clear();
    _routes.clear();
  }

  float ProbablePathCalculator::_heuristic(int start)
  {
    float result = 0.0;
//     if (_minCost > 0 && _destinations.size() == 1)
//     {
//       float d = _calculateDistance(_toPoint(start), _destinations[0]);
//       result = d * _minCost;
//     }
    if (_minCost > 0 && !_destPending.empty())
    {
      float minD = 1e10f;
      for (HashSet<int>::const_iterator it = _destPending.begin(); it != _destPending.end(); 
        ++it)
      {
        float d = _calculateDistance(_toPoint(start), _destinations[0]);
        minD = std::min(minD, d);
      }
      result = minD * _minCost;
    }
    return result;
  }

  void ProbablePathCalculator::_randomizeCurrentValues()
  {
    _minCost = 1e10f;
    if (_variation == 0)
    {
      _currentValues = _baseValues;
    }
    else
    {
      switch (_randType)
      {
      case UniformNoise:
        {
          _randomizeUniformNoise();
          break;
        }
      case Patches:
        {
          _randomizePatches();
          break;
        }
      default: break;
      }
    }
    for (unsigned int i = 0; i < _currentValues.size(); i++)
    {
      if (_currentValues[i] >= 0.0)
      {
        _minCost = std::min(_minCost, _currentValues[i]);
      }
    }
  }

  void ProbablePathCalculator::_randomizePatches()
  {
    map<int, float> patch;
    for (int i = 0; i < _patchCount; i++)
    {
      float r = 1.0f + ((float)_randomGenerator->generateInt() / (float)RAND_MAX * 2.0f - 1.0f) * _variation;
      patch[_randomGenerator->generateInt(_rows * _cols)] = r;
    }

    float maxValue = -1e10f;
    for (unsigned int i = 0; i < _baseValues.size(); i++)
    {
      maxValue = std::max(maxValue, _baseValues[i]);
    }

    _currentValues.resize(_baseValues.size());

    for (unsigned int i = 0; i < _currentValues.size(); i++)
    {
      if (_baseValues[i] < 0)
      {
        _currentValues[i] = _baseValues[i];
      }
      else
      {
        
        PpPoint c = _toPoint(i);
        float w = 0.0;
        float r = 0.0;
        for (map<int, float>::iterator it = patch.begin(); it != patch.end(); ++it)
        {
          PpPoint p = _toPoint(it->first);
          float d = _calculateDistance(c, p);
          w += 1 / d;
          r += 1 / d * it->second;
          if (fabs(d) < 1e-6)
          {
            r = it->second;
            w = 1.0;
            break;
          }
        }

        r = r / w;

        _currentValues[i] = _baseValues[i] + (r - 1) * maxValue;
        if (_currentValues[i] < 1e-6)
        {
          _currentValues[i] = 1e-6f;
        }
      }
    }
  }

  void ProbablePathCalculator::_randomizeUniformNoise()
  {
    _currentValues.resize(_baseValues.size());
    for (unsigned int i = 0; i < _currentValues.size(); i++)
    {
      if (_baseValues[i] < 0)
      {
        _currentValues[i] = _baseValues[i];
      }
      else
      {
        float r = 1.0f + ((float)_randomGenerator->generateInt() / (float)RAND_MAX * 2.0f - 1.0f) * _variation;
        _currentValues[i] = r * _baseValues[i];
        if (_currentValues[i] < 1e-6)
        {
          _currentValues[i] = 1e-6f;
        }
      }
    }
  }

  void ProbablePathCalculator::_resetRemainingDestinations()
  {
    _remainingDest.clear();
    for (unsigned int d = 0; d < _destinations.size(); d++)
    {
      Destination tmp;
      tmp.row = _destinations[d].row;
      tmp.col = _destinations[d].col;
      tmp.name = _destinations[d].name;
      if (!_destinationMaxCost.empty())
      {
        tmp.maxCost = _destinationMaxCost[d];
      }
      else
      {
        tmp.maxCost = -1;
      }
      _remainingDest.push_back(tmp);
    }
  }

  void ProbablePathCalculator::setFriction(int rows, int cols, const std::vector<float>& values) 
  {
    if (_rows != rows || _cols != cols)
    {
      clearRoutes();
    }
    _rows = rows;
    _cols = cols;
    if (_rows < 1 || _cols < 1)
    {
      throw Exception("Rows and cols must be > 1");
    }
    if (_rows * _cols != (int)values.size())
    {
      throw Exception("Row and Column count doesn't match array size");
    }
    _baseValues = values;
    for (unsigned int i = 0; i < values.size(); i++)
    {
      if (values[i] == 0.0)
      {
        throw Exception("No cell in the friction surface may be zero.");
      }
    }
  }

  void ProbablePathCalculator::setFrictionAsImage(const Tgs::Image<float>& friction)
  {
    setFriction(friction.getHeight(), friction.getWidth(), friction.getVector());
  }

  void ProbablePathCalculator::setRandomNoise(float variation) 
  { 
    _randType = UniformNoise; 
    _variation = variation; 
    if (_variation < 0)
    {
      throw Exception("Variation must be greater than zero.");
    }
    else if (_variation > 100)
    {
      _v(Warning) << "Variation is unreasonably large, continuing anyway..." << endl;
    }
  }

  void ProbablePathCalculator::setRandomPatches(float variation, int patchCount) 
  { 
    _randType = Patches; 
    _variation = variation; 
    _patchCount = patchCount; 
    if (_variation < 0)
    {
      throw Exception("Variation must be greater than zero.");
    }
    else if (_variation > 100)
    {
      _v(Warning) << "Variation is unreasonably large, continuing anyway..." << endl;
    }
    if (_patchCount < 1)
    {
      throw Exception("Patch count must be >= 1.");
    }
    if (_patchCount > 1000)
    {
      _v(Warning) << "Patch count is unreasonably high, continuing anyway..." << endl;
    }
  }

  void ProbablePathCalculator::setVerbose(int level) 
  { 
    _verbose = level; 
  }

  void ProbablePathCalculator::_surround(int index, float cost, MyHeap& q)
  {    
    vector<MyHeap::JHeapNode>& nodes = q.getNodes();

    int row = index / _cols;
    int col = index % _cols;
    float v = _currentValues[index];
    for (int dr = -1; dr <= 1; dr++)
    {
      for (int dc = -1; dc <= 1; dc++)
      {
        int r = row + dr;
        int c = col + dc;
        if (r >= 0 && r < _rows && c >= 0 && c < _cols)
        {
          int dIndex = r * _cols + c;
          if (_costSurface[dIndex] < 0.0)
          {
            float v2 = _currentValues[dIndex];
            if (v2 < 0)
            {
              _costSurface[dIndex] = -1;
            }
            else
            {
              float dist;
              if (dr != 0 && dc != 0)
              {
                dist = sqrt2;
              }
              else
              {
                dist = 1.0;
              }
              float newCost = cost + ((v + v2) / 2.0f) * dist;
              float h = _heuristic(dIndex);
              if ((nodes[dIndex].getValue() > newCost + h || nodes[dIndex].getValue() < 0.0) &&
                _baseValues[dIndex] >= 0.0)
              {
                q.push(dIndex, newCost + h);
                _tmpCost[dIndex] = newCost;
              }
            }
          }
        }
      }
    }
  }

  void ProbablePathCalculator::_createCostSurface(const PpPoint& source)
  {
    MyHeap q;
    _v(Debug) << "Resizing Cost Surface...\n";
    _costSurface.clear();
    _costSurface.resize(_rows * _cols, -1);
    _v(Debug) << "Resizing Tmp Cost Surface...\n";
    _tmpCost.clear();
    _tmpCost.resize(_rows * _cols, -1);
    _v(Debug) << "Resizing Heap...\n";
    q.resize(_costSurface.size(), -1);
    int idxs = _toIndex(source.row, source.col);
    // if the source is on impassable terrain, there are no routes to or from it
    if (_baseValues[idxs] < 0)
    {
      return;
    }
    q.push(idxs, 0.0);
    _tmpCost[idxs] = 0.0;

    _destPending.clear();
    for (list<Destination>::iterator it = _remainingDest.begin(); it != _remainingDest.end();
      ++it)
    {
      _destPending.insert(_toIndex(it->row, it->col));
    }

    while (q.hasNodes())
    {
      int index = q.pop();
      
      _costSurface[index] = _tmpCost[index];

      _destPending.erase(index);
      if (_destPending.empty())
      {
        return;
      }

      _surround(index, _costSurface[index], q);
    }
  }

  Tgs::Image<float> ProbablePathCalculator::updateCostSurface(const Tgs::Image<float>& surface)
  {
//     _v(Debug) << "Here\n";
//     _v(Debug) << "Friction Surface: " << endl;
    Image<float> tmp(_cols, _rows);
    tmp.setVector(_baseValues);
//     _v(Debug) << tmp;
//     _v(Debug) << "Cost Surface: " << endl;
//     _v(Debug) << surface;
//     _v(Debug).flush();
    if (surface.getHeight() != _rows || surface.getWidth() != _cols)
    {
      stringstream strm;
      strm << "Cost surface must be the same size as friction surface. (cost: " << 
        surface.getWidth() << " x " << surface.getHeight() << 
        " vs. friction: " << _cols << " x " << _rows << ")" << endl;
      throw Exception(strm.str());
    }
    _randomizeCurrentValues();
    _costSurface = surface.getVector();

    MyHeap q;
    _v(Debug) << "Resizing Heap...\n";
    q.resize(_costSurface.size(), -1);

    _destPending.clear();

    for (unsigned int i = 0; i < _costSurface.size(); i++)
    {
      if (_costSurface[i] >= 0)
      {
        q.push(i, _costSurface[i]);
        //_updateSurround(i, q);
      }
    }
    _v(Debug) << "Heap size: " << q.getChildCount() << endl;
    
    while (q.hasNodes())
    {
      int index = q.pop();
      _updateSurround(index, q);
    }

    Tgs::Image<float> result;
    result.resize(surface.getWidth(), surface.getHeight());
    result.setVector(_costSurface);
    //result.setVector(_currentValues);
//     _v(Debug) << result;
    return result;
  }

  void ProbablePathCalculator::_updateSurround(int index, MyHeap& q)
  {    
    int row = index / _cols;
    int col = index % _cols;
    float cost = _costSurface[index];
    float v = _currentValues[index];
    if (cost < 0)
    {
      return;
    }
    for (int dr = -1; dr <= 1; dr++)
    {
      for (int dc = -1; dc <= 1; dc++)
      {
        int r = row + dr;
        int c = col + dc;
        if (r >= 0 && r < _rows && c >= 0 && c < _cols)
        {
          int dIndex = r * _cols + c;
          float v2 = _currentValues[dIndex];
          if (v2 < 0)
          {
            _costSurface[dIndex] = -1;
          }
          else
          {
            float dist;
            float dCost = _costSurface[dIndex];
            if (dr != 0 && dc != 0)
            {
              dist = sqrt2;
            }
            else
            {
              dist = 1.0;
            }
            float newCost = cost + ((v + v2) / 2.0f) * dist;
            if (newCost < dCost || dCost < 0.0)
            {
              q.push(dIndex, newCost);
              _costSurface[dIndex] = newCost;
            }
          }
        }
      }
    }
  }


  ostream& ProbablePathCalculator::_v(int level)
  {
    if (level <= _verbose)
    {
      return cout;
    }
    else
    {
      _null.clear();
      return _null;
    }
  }
}

