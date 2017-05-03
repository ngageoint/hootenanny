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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PertyWayGeneralizeVisitor.h"

// boost
#include <boost/random/uniform_real.hpp>
#include <boost/random/uniform_int.hpp>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/RandomNumberUtils.h>
#include <hoot/core/util/OsmUtils.h>
#include <hoot/core/algorithms/RdpWayGeneralizer.h>

// Qt
#include <QSet>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, PertyWayGeneralizeVisitor)

PertyWayGeneralizeVisitor::PertyWayGeneralizeVisitor() :
_epsilon(-1.0)
{
  _localRng.reset(new boost::minstd_rand());
  _rng = _localRng.get();

  setConfiguration(conf());
}

void PertyWayGeneralizeVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  setWayGeneralizeProbability(configOptions.getPertyWayGeneralizeProbability());
  setEpsilon(configOptions.getPertyWayGeneralizeEpsilon());
  const int seed = configOptions.getPertySeed();
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

QString PertyWayGeneralizeVisitor::toString()
{
  return "_wayGeneralizeProbability: " + QString::number(_wayGeneralizeProbability) +
    ", _epsilon: " + QString::number(_epsilon);
}

void PertyWayGeneralizeVisitor::setOsmMap(OsmMap* map)
{
  _map = map;
  assert(_epsilon != -1.0);
  _generalizer.reset(new RdpWayGeneralizer(_map->shared_from_this(), _epsilon));
}

void PertyWayGeneralizeVisitor::visit(const boost::shared_ptr<Element>& element)
{
  if (element->getElementType() == ElementType::Way)
  {
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
      _generalizer->generalize(dynamic_pointer_cast<Way>(element));
    }
    else
    {
      logMsg = logMsg.replace("will", "will not");
      LOG_TRACE(logMsg);
    }
  }
}

}
