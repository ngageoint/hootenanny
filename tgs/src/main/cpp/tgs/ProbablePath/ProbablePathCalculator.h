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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef __PROBABLE_PATH_CALCULATOR_H__
#define __PROBABLE_PATH_CALCULATOR_H__

// Standard Includes
#include <iostream>
#include <list>
#include <sstream>
#include <vector>

#include <tgs/HashMap.h>
#include <tgs/TgsExport.h>
#include <tgs/RasterOps/Image.hpp>
#include <tgs/Statistics/Random.h>

namespace Tgs
{
  class MyHeap;

  /**
   * I had to put this class outside ProbablePathCalculator to make SWIG happy.
   */
  class TGS_EXPORT PpPoint
  {
  public:
    int row, col;
    std::string name;
    /// The cumulative cost up to this point.
    float cost;

    PpPoint();

    PpPoint(int row, int col)
      : row(row), col(col), cost(0.0f) { }

    PpPoint(int row, int col, std::string name)
      : row(row), col(col), name(name), cost(0.0f) { }

    bool operator!=(const PpPoint& p)
    {
      return row != p.row || col != p.col;
    }

    bool operator==(const PpPoint& p)
    {
      return row == p.row && col == p.col;
    }
  };

  /**
  * I had to put this class outside ProbablePathCalculator to make SWIG happy.
  */
  class TGS_EXPORT PpRoute
  {
  public:
    float cost;
    float length;
    std::vector<PpPoint> waypoints;
  };

  class TGS_EXPORT ProbablePathCalculator
  {
  public:

    enum
    {
      Quiet = 0,
      Warning = 1,
      Verbose = 2,
      Debug = 3
    };

    class Destination : public PpPoint
    {
    public:
      float maxCost;
    };

    ProbablePathCalculator();
    ProbablePathCalculator(const RandomPtr& random);

    virtual ~ProbablePathCalculator() = default;

    /**
     * Calculates the most probable paths with the specified number of iterations.
     */
    void calculateProbablePaths(int iterations);

    /**
     * Clears all the routes that have been generated to this point. The other setting
     * (friction surface, variation, etc.) all stay the same.
     */
    void clearRoutes();

    /**
     * Returns a vector of all the routes calculated by the last calculateProbablePaths run.
     */
    const std::vector<PpRoute>& getAllRoutes() const { return _routes; }

    const std::vector<float>& getCostSurface() const { return _costSurface; }

    const std::vector<int>& getRouteCounts() const { return _result; }

    /**
     * Set the destination locations as points. It is assumed that all points and rasters are in
     * the same projection.
     */
    void setDestinations(const std::vector<PpPoint>& points) { _destinations = points; }

    /**
     * Sets the maximum allowable cost for any route that ends at this destination.
     */
    void setDestinationMaxCost(const std::vector<float>& maxCost) { _destinationMaxCost = maxCost; }

    /**
     * Set the friction surface for calculating destination as a raster input. Values in the
     * friction surface must be >= 0.
     */
    void setFriction(int rows, int cols, const std::vector<float>& values);

    /**
     * Similar to above, provided for convenience
     */
    void setFrictionAsImage(const Tgs::Image<float>& friction);

    void setRandomNoise(float variation);

    void setRandomPatches(float variation, int patchCount);

    /**
     * Set the source locations as points. It is assumed that all points and rasters are in
     * the same projection.
     */
    void setSources(const std::vector<PpPoint>& points) { _sources = points; }

    void setVerbose(int level = Verbose);

    /**
     * Given a partially calculated cost, updated the values in the cost surface to make them
     * consistent. The cost surface should have either null values (-1) or values that are >=
     * actual cost. This function is primarily useful when distributing cost surface calculation
     * over multiple processors or machines.
     */
    Tgs::Image<float> updateCostSurface(const Tgs::Image<float>& surface);

  private:

    friend std::ostream& operator<<(std::ostream & o, const Destination& d);

    class Cell
    {
    public:
      int index;
      float cost;
      Cell(int index, float cost) { this->index = index; this->cost = cost; }
      bool operator<(const Cell& c) const
      {
        return cost > c.cost;
      }
    };

    enum
    {
      UniformNoise,
      Patches
    };

    int _rows, _cols;
    std::vector<PpPoint> _sources;
    std::vector<PpPoint> _destinations;
    std::vector<float> _destinationMaxCost;

    /// This list is used to maintain a list of destinations that have not been found. We may
    /// have to look for a destination multiple times if the maxCost is exceeded.
    std::list<Destination> _remainingDest;

    std::vector<float> _baseValues;
    std::vector<float> _currentValues;
    std::vector<float> _costSurface;
    std::vector<float> _tmpCost;
    std::vector<int> _result;
    float _variation;
    float _minCost;
    int _randType;
    int _patchCount;
    HashSet<int> _destPending;
    std::vector<PpRoute> _routes;
    int _verbose;
    std::stringstream _null;
    /** Pointer to the random number generator object, defaults to Tgs::Random::instance() */
    RandomPtr _randomGenerator;

    bool _addReturnPaths(const PpPoint& source, const Destination& destination);

    /**
     * Checks the _costSurface point for an inconsistency (there is a shorter path to
     * get there based on the immediate neighbors). If there is an inconsistency, it is updated
     * and added to the heap.
     */
    void _checkForInconsistency(const PpPoint& p, MyHeap& q);

    float _calculateDistance(const PpPoint& p1, const PpPoint& p2);

    /**
     * Calculates the shortest path from the given source to all the _remainingDestinations.
     * As valid paths are found the destinations are removed from _remainingDestinations, if a
     * valid path isn't found for a destination then it is left in _remainingDestinations. This
     * uses the _currentValues for the friction surface.
     */
    void _calculateShortestPath(const PpPoint& source);

    void _calculateWithWaypoints(int iterations);

    void _calculateWithoutWaypoints(int iterations);

    void _checkBounds(const PpPoint& p);

    float _heuristic(int start);

    void _randomizeCurrentValues();

    void _randomizePatches();

    void _randomizeUniformNoise();

    /**
     * Reset remaining destinations so it contains all the destinations.
     */
    void _resetRemainingDestinations();

    void _surround(int index, float cost, MyHeap& q);

    void _createCostSurface(const PpPoint& source);

    int _toIndex(int row, int col) const { return col + row * _cols; }

    int _toIndex(const PpPoint& p) const { return _toIndex(p.row, p.col); }

    PpPoint _toPoint(int index) const { return PpPoint(index / _cols, index % _cols); }

    void _updateSurround(int index, MyHeap& q);

    /**
     * Returns the appropriate output stream for the given verbosity. E.g. If verbosity is
     * Warning and I call _v(Verbose) I will get a null stream, but if verbosity is Debug I
     * will get cout. This is a bit slow so don't call on frequent messages.
     */
    std::ostream& _v(int level = Verbose);
  };
}

#endif
