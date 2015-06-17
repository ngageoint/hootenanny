/***************************************************************************
* Copyright (c) 2005-2007 by SPADAC Inc. (formerly Spatial Data Analytics Corporation).  All rights reserved.
****************************************************************************/

%{
#include "../Tgs/src/ProbablePath/ProbablePathCalculator.h"
%}

namespace Tgs
{
  /**
   * I had to put this class outside ProbablePathCalculator to make SWIG happy.
   */
  class PpPoint
  {
  public:
    int row, col;
    std::string name;
    float cost;
    
    PpPoint();
    
    PpPoint(const PpPoint& p);

    PpPoint(int row, int col);

    PpPoint(int row, int col, const char* name);
  };
}

namespace std
{
  %template(tgsVectorPpPoint) vector<Tgs::PpPoint>;
}

namespace Tgs
{
  /**
   * I had to put this class outside ProbablePathCalculator to make SWIG happy.
   */
  class PpRoute
  {
  public:
    float cost;
    float length;
    std::vector<PpPoint> waypoints;
  };
}

namespace std
{
  %template(tgsVectorPpRoute) vector<Tgs::PpRoute>;
}

namespace Tgs
{
  class ProbablePathCalculator
  {
  public:
    enum
    {
      Quiet = 0,
      Warning = 1,
      Verbose = 2,
      Debug = 3
    };

    ProbablePathCalculator();

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
    const std::vector<PpRoute>& getAllRoutes() const;

    const std::vector<float>& getCostSurface() const;
    
    const std::vector<int>& getRouteCounts() const;

    /**
     * Set the destination locations as points. It is assumed that all points and rasters are in
     * the same projection.
     */
    void setDestinations(const std::vector<PpPoint>& points);
    
    void setDestinationMaxCost(const std::vector<float>& maxCost);

    /**
     * Set the friction surface for calculating destination as a raster input. Values in the 
     * friction surface must be >= 0.
     */
    void setFriction(int rows, int cols, const std::vector<float>& values);
    
    void setFrictionAsImage(const Tgs::Image<float>& friction);

    void setRandomNoise(float variation);

    void setRandomPatches(float variation, int patchCount);

    /**
     * Set the source locations as points. It is assumed that all points and rasters are in
     * the same projection.
     */
    void setSources(const std::vector<PpPoint>& points);
    
    void setVerbose(int level);
    
    Image<float> updateCostSurface(const Image<float>& surface);
  };
}

