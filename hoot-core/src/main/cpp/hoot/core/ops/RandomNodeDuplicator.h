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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef RANDOM_NODE_DUPLICATOR_H
#define RANDOM_NODE_DUPLICATOR_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/RngConsumer.h>

namespace hoot
{

/**
 * Duplicates a node with some random error.  This is not part of the original PERTY paper.
 *
 * @todo This should really change the node after it has been duplicated.
 */
class RandomNodeDuplicator : public OsmMapOperation, public Configurable, public RngConsumer
{
public:

  static QString className() { return "RandomNodeDuplicator"; }

  RandomNodeDuplicator();
  ~RandomNodeDuplicator() = default;

  void apply(OsmMapPtr& map) override;

  void duplicateNode(const NodePtr& n, const OsmMapPtr& map);

  void setConfiguration(const Settings& conf) override;

  void setRng(boost::minstd_rand& rng) override { _rng = &rng; }

  QString getInitStatusMessage() const override
  { return "Randomly duplicating nodes..."; }
  QString getCompletedStatusMessage() const override
  { return "Randomly duplicated " + QString::number(_numAffected) + " nodes"; }

  QString getDescription() const override
  { return "Duplicates a node with some random error"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  /**
   * The number of duplicates is set as round(abs(N(0, sigma^2))) + 1. Setting sigma to 0 will
   * guarantee that there will always be exactly one duplicate.
   */
  void setDuplicateSigma(double sigma) { _duplicateSigma = sigma; }

  /**
   * The distance that a feature is moved is based on the circular error of the source point. The
   * new point will be put within N(0, sigma^2) * moveMultiplier meters of the source point where
   * sigma is the standard deviation associated with the source point.
   */
  void setMoveMultiplier(double moveMultiplier) { _moveMultiplier = moveMultiplier; }

  /**
   * The probability of at least one duplicate being created. See setDuplicateSigma to determine
   * how many duplicates will be created.
   */
  void setProbability(double p) { _p = p; }

private:

  double _moveMultiplier;
  double _p;
  double _duplicateSigma;
  boost::minstd_rand* _rng;
  std::shared_ptr<boost::minstd_rand> _localRng;
};

}

#endif // RANDOM_NODE_DUPLICATOR_H
