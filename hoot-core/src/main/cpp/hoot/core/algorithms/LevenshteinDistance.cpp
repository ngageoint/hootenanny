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

#include "LevenshteinDistance.h"

// Hoot
#include <hoot/core/algorithms/Translator.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/HootException.h>

// Standard
#include <iostream>
#include <math.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(StringDistance, LevenshteinDistance)

LevenshteinDistance::LevenshteinDistance(double alpha)
{
  if (alpha == -1)
  {
    setAlpha(ConfigOptions().getLevenshteinDistanceAlpha());
  }
  else
  {
    setAlpha(alpha);
  }
}

double LevenshteinDistance::compare(const QString &s1, const QString &s2) const
{
  return pow(score(s1, s2), _alpha);
}

void LevenshteinDistance::setAlpha(double alpha)
{
  if (alpha <= 0)
  {
    throw IllegalArgumentException("Expected alpha to be > 0.");
  }
  _alpha = alpha;
}

void LevenshteinDistance::setConfiguration(const Settings& conf)
{
  setAlpha(ConfigOptions(conf).getLevenshteinDistanceAlpha());
}

QString LevenshteinDistance::toEnglish(const QString& s)
{
  QString result = Translator::getInstance().toEnglish(s);
  return result;
}


}
