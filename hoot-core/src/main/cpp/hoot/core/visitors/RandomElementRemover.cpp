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
#include "RandomElementRemover.h"

// boost
#include <boost/random/uniform_real.hpp>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/RandomNumberUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RandomElementRemover)

RandomElementRemover::RandomElementRemover() :
_localRng(std::make_shared<boost::minstd_rand>())
{
  _rng = _localRng.get();
}

void RandomElementRemover::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  setProbability(configOptions.getRandomElementRemoverProbability());
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

void RandomElementRemover::visit(const ConstElementPtr& e)
{
  boost::uniform_real<> uni(0.0, 1.0);
  if (uni(*_rng) <= _p)
  {
    RecursiveElementRemover(
      ElementId(e->getElementType(), e->getId())).apply(_map->shared_from_this());
    _numAffected++;
  }
}

}
