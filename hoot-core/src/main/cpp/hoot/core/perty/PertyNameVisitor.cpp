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
#include "PertyNameVisitor.h"

// boost
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/RandomNumberUtils.h>

// Standard
#include <algorithm>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, PertyNameVisitor)

PertyNameVisitor::PertyNameVisitor()
{
  _localRng.reset(new boost::minstd_rand());
  _rng = _localRng.get();

  setConfiguration(conf());
}

QString PertyNameVisitor::permuteName(const QString& s)
{
  boost::uniform_real<> uni(0.0, 1.0);
  boost::uniform_int<> uniChange(0, 1);
  boost::uniform_int<> uniLetter('a', 'z' + 1);
  QString result = s;
  for (int i = 0; i < result.size(); i++)
  {
    if (uni(*_rng) <= _changeP)
    {
      int type = uniChange(*_rng);
      switch (type)
      {
      // replace the letter with a random letter a-z or space
      case 0:
        {
          char letter = uniLetter(*_rng);
          if (letter == 'z' + 1)
          {
            letter = ' ';
          }
          result[i] = letter;
        } break;
      // randomly swap a letter.
      case 1:
        {
          int pos = max<int>(uni(*_rng) * result.size(), result.size() - 1);
          QChar c1 = result[i];
          result[i] = result[pos];
          result[pos] = c1;
        } break;
      }
    }
  }

  return result;
}

void PertyNameVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  setProbability(configOptions.getPertyNameProbability());
  setChangeProbability(configOptions.getPertyNameChangeProbability());
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

QString PertyNameVisitor::toString()
{
  return "_p: " + QString::number(_p) + ", _changeP: " + QString::number(_changeP);
}

void PertyNameVisitor::visit(const boost::shared_ptr<Element>& e)
{
  boost::uniform_real<> uni(0.0, 1.0);
  QStringList keys = e->getTags().getMatchingKeys(Tags::getNameKeys());

  for (int i = 0; i < keys.size(); i++)
  {
    bool change = false;
    QString k = keys[i];
    QStringList vl = e->getTags().getList(k);
    for (int j = 0; j < vl.size(); j++)
    {
      if (uni(*_rng) <= _p)
      {
        vl[j] = permuteName(vl[j]);
        change = true;
      }
    }

    if (change)
    {
      e->getTags().setList(k, vl);
    }
  }
}

}
