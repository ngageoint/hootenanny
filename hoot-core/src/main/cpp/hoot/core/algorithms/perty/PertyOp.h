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
#ifndef PERTY_H
#define PERTY_H

// geos
#include <geos/geom/Envelope.h>

// hoot
#include <hoot/core/util/Units.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/OpenCv.h>

// Qt
#include <QString>
#include <QStringList>

namespace hoot
{

class PermuteGridCalculator;

/**
 * Performs a perty style permutation on the data. The specifics of which operations are performed
 * can be specified via the "perty.ops" configuration setting.
 *
 * The geometry permutations are done in accordance with [1].
 *
 * 1. Evaluating conflation methods using uncertainty modeling - Peter Doucette, et al. 2013
 *    https://insightcloud.digitalglobe.com/redmine/attachments/download/1667/2013%20Evaluating%20conflation%20methods%20using%20uncertainty%20modeling.pdf
 *    http://proceedings.spiedigitallibrary.org/proceeding.aspx?articleid=1691369
 *
 * TODO: implement OperationStatusInfo
 */
class PertyOp : public OsmMapOperation, public Configurable
{
public:

  static std::string className() { return "hoot::PertyOp"; }

  PertyOp();

  virtual ~PertyOp() {}

  virtual void setConfiguration(const Settings& conf);

  /**
   * Permute the map and then apply all "perty.ops" to the map as well.
   */
  virtual void apply(std::shared_ptr<OsmMap>& map);

  /**
   * Generates a map of all the grid offset vectors and permutes the given map.
   */
  std::shared_ptr<OsmMap> generateDebugMap(std::shared_ptr<OsmMap>& map);

  void permute(const std::shared_ptr<OsmMap>& map);

  void setCsmParameters(double D) { _D = D; }

  void setGridSpacing(Meters gridSpacing) { _gridSpacing = gridSpacing; }

  /**
   * Sets a list of operations that should be run after the permute method is called.
   */
  void setNamedOps(QStringList namedOps) { _namedOps = namedOps; }

  /**
   * Seeds the permutation process. By default a seed is generated based on time. The seed should
   * be non-negative or -1 to generate a seed based on time.
   */
  void setSeed(int seed) { _seed = seed; }

  /**
   * Sets the systematic error. This is the sigma value for Sx and Sy. The same sigma value is used
   * for all values in each matrix. See [1] for more information.
   */
  void setSystematicError(Meters sigmaX, Meters sigmaY) { _sigmaSx = sigmaX; _sigmaSy = sigmaY; }

  virtual QString getDescription() const { return "Perturbs map data"; }

  virtual std::string getClassName() const { return className(); }

private:

  Meters _D;

  Meters _gridSpacing;
  int _seed;
  Meters _sigmaRx, _sigmaRy;
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

  cv::Mat _calculatePermuteGrid(geos::geom::Envelope env, int& rows, int& cols);
};

}

#endif // PERTY_H
