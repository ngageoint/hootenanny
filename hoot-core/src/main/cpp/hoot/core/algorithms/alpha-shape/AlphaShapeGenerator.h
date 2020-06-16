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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef ALPHASHAPEGENERATOR_H
#define ALPHASHAPEGENERATOR_H

// Hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Generates an alpha shape
 *
 * @see AlphaShape
 */
class AlphaShapeGenerator
{
public:

  static std::string className() { return "hoot::AlphaShapeGenerator"; }

  /**
   * TODO
   *
   * @param alpha
   * @param buffer
   */
  AlphaShapeGenerator(const double alpha, const double buffer = 0.0);

  /**
   * Generates an alpha shape based on the geometry of an input map
   *
   * @param inputMap the geometry to use for generating the alpha shape
   * @return a map containing the alpha shape's points
   */
  OsmMapPtr generateMap(OsmMapPtr inputMap);

  /**
   * TODO
   *
   * @param inputMap
   * @return
   */
  std::shared_ptr<geos::geom::Geometry> generateGeometry(OsmMapPtr inputMap);

  void setRetryOnTooSmallInitialAlpha(bool retry) { _retryOnTooSmallInitialAlpha = retry; }
  void setManuallyCoverSmallPointClusters(bool cover) { _manuallyCoverSmallPointClusters = cover; }

private:

  // TODO
  double _alpha;
  // TODO
  double _buffer;
  // TODO
  bool _manuallyCoverSmallPointClusters;
  // TODO
  bool _retryOnTooSmallInitialAlpha;
  // TODO
  bool _maxTries;

  /*
   * This is a bit of hack to the alg, if you will, that will alow for covering small groups of
   * features when a smaller alpha value is selected. This is desirable in certain situations when
   * using the alpha shape to feed a tasking grid.
   */
  void _coverStragglers(std::shared_ptr<geos::geom::Geometry>& geometry, const ConstOsmMapPtr& map);
};

}

#endif // ALPHASHAPEGENERATOR_H
