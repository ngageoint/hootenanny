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
 * @copyright Copyright (C) 2015, 2016, 2018, 2021 Maxar (http://www.maxar.com/)
 */
#include "ConstrainedMatches.h"

// hoot
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

//  Set EPSILON to non-zero so that no matches are dropped
double ConstrainedMatches::EPSILON = 1e-9;

ConstrainedMatches::ConstrainedMatches(const ConstOsmMapPtr& map)
  : _map(map),
    _score(-1),
    _timeLimit(-1)
{
}

void ConstrainedMatches::_calculateMatchConflicts()
{
  MatchConflicts(_map).calculateMatchConflicts(_matches, _conflicts);
}

}
