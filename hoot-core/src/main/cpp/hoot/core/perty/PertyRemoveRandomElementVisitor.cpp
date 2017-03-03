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
#include "PertyRemoveRandomElementVisitor.h"

// boost
#include <boost/random/uniform_real.hpp>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/RandomNumberUtils.h>
#include <hoot/core/OsmMap.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, PertyRemoveRandomElementVisitor)

PertyRemoveRandomElementVisitor::PertyRemoveRandomElementVisitor()
{
  _localRng.reset(new boost::minstd_rand());
  _rng = _localRng.get();

  setConfiguration(conf());
}

QString PertyRemoveRandomElementVisitor::toString()
{
  return "_p: " + QString::number(_p);
}

void PertyRemoveRandomElementVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  setProbability(configOptions.getPertyRemoveRandomProbability());
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

void PertyRemoveRandomElementVisitor::visit(const ConstElementPtr& e)
{
  boost::uniform_real<> uni(0.0, 1.0);
  if (uni(*_rng) <= _p)
  {
    RecursiveElementRemover(ElementId(e->getElementType(), e->getId())).apply(_map->shared_from_this());
  }
}

}
