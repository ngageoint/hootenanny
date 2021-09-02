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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef PERTY_H
#define PERTY_H

// hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/OpenCv.h>
#include <hoot/core/util/Units.h>

namespace hoot
{

class PermuteGridCalculator;

/**
 * @brief The PertyOp class performs a perty style permutation on the data.
 *
 * The specifics of which operations are performed can be specified via the "perty.ops"
 * configuration setting. The geometry permutations are done in accordance with [1].
 *
 * 1. Evaluating conflation methods using uncertainty modeling - Peter Doucette, et al. 2013
 *    https://github.com/ngageoint/hootenanny/files/609201/2013.Evaluating.conflation.methods.using.uncertainty.modeling.pdf
 *    http://proceedings.spiedigitallibrary.org/proceeding.aspx?articleid=1691369
 * @todo implement OperationStatus
 */
class PertyOp : public OsmMapOperation, public Configurable
{
public:

  static QString className() { return "PertyOp"; }

  PertyOp();
  ~PertyOp() = default;

  void setConfiguration(const Settings& conf) override;

  /**
   * @brief Permute the map and then apply all "perty.ops" to the map as well.
   * @see OsmMapOperation
   */
  void apply(std::shared_ptr<OsmMap>& map) override;

  /**
   * @brief generateDebugMap generates a map of all the grid offset vectors and permutes the given
   * map.
   */
  std::shared_ptr<OsmMap> generateDebugMap(const std::shared_ptr<OsmMap>& map);

  void permute(const std::shared_ptr<OsmMap>& map);

  void setCsmParameters(double D) { _D = D; }
  void setGridSpacing(Meters gridSpacing) { _gridSpacing = gridSpacing; }
  /**
   * @brief setNamedOps sets a list of operations that should be run after the permute method is
   * called.
   */
  void setNamedOps(QStringList namedOps) { _namedOps = namedOps; }
  /**
   * @brief setSeed seeds the permutation process. By default a seed is generated based on time. The
   * seed should be non-negative or -1 to generate a seed based on time.
   */
  void setSeed(int seed) { _seed = seed; }
  /**
   * @brief setSystematicError sets the systematic error. This is the sigma value for Sx and Sy. The
   * same sigma value is used for all values in each matrix. See [1] for more information.
   */
  void setSystematicError(Meters sigmaX, Meters sigmaY) { _sigmaSx = sigmaX; _sigmaSy = sigmaY; }

  QString getDescription() const override { return "Perturbs map data"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  Meters _D;

  Meters _gridSpacing;
  int _seed;
  Meters _sigmaSx, _sigmaSy;
  /**
   * Previously the full covariance method was also supported as described in Doucette et al. However,
   * the newer DirectSequentialSimulation is more efficient and produces similar results. By removing
   * the full covariance method we were able to simplify the build process and reduce maintenance cost.
   */
  QString _permuteAlgorithm;
  std::shared_ptr<PermuteGridCalculator> _gridCalculator;
  QStringList _namedOps;

  Settings& _settings;

  void _configure();

  cv::Mat _calculatePermuteGrid(const geos::geom::Envelope& env, int& rows, int& cols);
};

}

#endif // PERTY_H
