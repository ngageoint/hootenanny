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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ConstrainedMatches.h"

// hoot
#include <hoot/core/algorithms/IntegerProgrammingSolver.h>
#include <hoot/core/conflate/Match.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

//  Set EPSILON to non-zero so that no matches are dropped
double ConstrainedMatches::EPSILON = 1e-9;

ConstrainedMatches::ConstrainedMatches(const ConstOsmMapPtr &map) :
  _map(map)
{
  _score = -1;
  _timeLimit = -1;
  setConfiguration(conf());
}

void ConstrainedMatches::_calculateMatchConflicts()
{
  MatchConflicts(_map).calculateMatchConflicts(_matches, _conflicts);
}

}
