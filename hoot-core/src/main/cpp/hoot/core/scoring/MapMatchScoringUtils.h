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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 Maxar (http://www.maxar.com/)
 */
#ifndef MAP_MATCH_SCORING_UTILS_H
#define MAP_MATCH_SCORING_UTILS_H

// Qt
#include <QString>

namespace hoot
{

class MatchComparator;

/**
 * Various utilities to be used when scoring matches after conflation
 */
class MapMatchScoringUtils
{
  public:

    MapMatchScoringUtils();

    /**
      Returns a printable string with match scoring results

      @param matchComparator a match comparator populated during scoring of conflation results
      @return a printable string with scoring results
      */
    static QString getMatchScoringString(const std::shared_ptr<const MatchComparator>& matchComparator);

};

}

#endif // MAP_MATCH_SCORING_UTILS_H
