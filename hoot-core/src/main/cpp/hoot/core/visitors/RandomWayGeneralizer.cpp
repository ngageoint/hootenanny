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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RandomWayGeneralizer.h"

// boost
#include <boost/random/uniform_real.hpp>
#include <boost/random/uniform_int.hpp>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/RandomNumberUtils.h>
#include <hoot/core/algorithms/RdpWayGeneralizer.h>
#include <hoot/core/util/MapProjector.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RandomWayGeneralizer)

RandomWayGeneralizer::RandomWayGeneralizer() :
_epsilon(1.0)
{
  _localRng.reset(new boost::minstd_rand());
  _rng = _localRng.get();
}

void RandomWayGeneralizer::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  setWayGeneralizeProbability(configOptions.getRandomWayGeneralizerProbability());
  setEpsilon(configOptions.getRandomWayGeneralizerEpsilon());
  const int seed = configOptions.getRandomSeed();
  LOG_VARD(seed);
  if (seed == -1)
  {
    _rng->seed(RandomNumberUtils::generateSeed());
  }
  else
  {
    _rng->seed(seed);
  }
}

void RandomWayGeneralizer::setOsmMap(OsmMap* map)
{
  _map = map;
  MapProjector::projectToPlanar(_map->shared_from_this());

  assert(_epsilon != -1.0);
  _generalizer.reset(new RdpWayGeneralizer(_epsilon));
  _generalizer->setOsmMap(_map);
}

void RandomWayGeneralizer::visit(const std::shared_ptr<Element>& element)
{
  if (element->getElementType() == ElementType::Way)
  {
    if (!_map)
    {
      throw IllegalArgumentException("No map passed to way generalizer.");
    }
    else if (_map->getProjection()->IsGeographic())
    {
      throw IllegalArgumentException("Input map must be projected to planar.");
    }

    //randomly select ways to generalize
    boost::uniform_real<> randomGeneralizeDistribution(0.0, 1.0);
    const double randomNum = randomGeneralizeDistribution(*_rng);
    QString logMsg =
      QString("element: %1 %2 be generalized based on a probability of: %3 and a randomly generated number: %4 \n")
        .arg(element->getElementId().toString())
        .arg("*will*")
        .arg(QString::number(_wayGeneralizeProbability))
        .arg(QString::number(randomNum));
    if (randomNum <= _wayGeneralizeProbability)
    {
      LOG_TRACE(logMsg);
      _generalizer->generalize(std::dynamic_pointer_cast<Way>(element));
    }
    else
    {
      logMsg = logMsg.replace("will", "will not");
      LOG_TRACE(logMsg);
    }
  }
}

}
