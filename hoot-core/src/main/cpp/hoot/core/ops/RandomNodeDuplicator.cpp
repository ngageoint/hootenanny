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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "RandomNodeDuplicator.h"

// boost
#include <boost/random/normal_distribution.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

// hoot
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/RandomNumberUtils.h>
#include <hoot/core/util/Settings.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RandomNodeDuplicator)

RandomNodeDuplicator::RandomNodeDuplicator()
  : _localRng(std::make_shared<boost::minstd_rand>())
{
  _rng =_localRng.get();
}

void RandomNodeDuplicator::apply(OsmMapPtr& map)
{
  _numAffected = 0;

  MapProjector::projectToPlanar(map);

  boost::uniform_real<> uni(0.0, 1.0);
  boost::normal_distribution<> nd;
  boost::variate_generator<boost::minstd_rand&, boost::normal_distribution<>> N(*_rng, nd);

  // make a copy since we'll be modifying the map as we go.
  NodeMap nm = map->getNodes();
  for (auto it = nm.begin(); it != nm.end(); ++it)
  {
    if (uni(*_rng) < _p)
    {
      const NodePtr& n = it->second;
      int copies = static_cast<int>(round(fabs(N() * _duplicateSigma)) + 1);

      for (int i = 0; i < copies; i++)
        duplicateNode(n, map);
    }
  }
}

void RandomNodeDuplicator::duplicateNode(const NodePtr& n, const OsmMapPtr& map)
{
  boost::normal_distribution<> nd;
  boost::variate_generator<boost::minstd_rand&, boost::normal_distribution<>> N(*_rng, nd);

  LOG_VART(n->getCircularError());
  Meters sigma = n->getCircularError() / 2.0;
  double x = n->getX() + N() * sigma * _moveMultiplier;
  double y = n->getY() + N() * sigma * _moveMultiplier;

  NodePtr newNode = std::make_shared<Node>(n->getStatus(), map->createNextNodeId(), x, y, n->getCircularError());
  map->addNode(newNode);

  _numAffected++;
}

void RandomNodeDuplicator::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  setDuplicateSigma(configOptions.getRandomNodeDuplicatorSigma());
  setProbability(configOptions.getRandomNodeDuplicatorProbability());
  setMoveMultiplier(configOptions.getRandomNodeDuplicatorMoveMultiplier());
  const int seed = configOptions.getRandomSeed();
  LOG_VARD(seed);
  if (seed == -1)
    _rng->seed(RandomNumberUtils::generateSeed());
  else
    _rng->seed(seed);
}

}
