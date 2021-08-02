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
 * @copyright Copyright (C) 2015, 2017, 2018, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef CONFLATE_STATS_HELPER_H
#define CONFLATE_STATS_HELPER_H

// Qt
#include <QList>
#include <QString>

namespace hoot
{

class SingleStat;

/**
 * Used to calculate conflation stats that require the conflation input *and* output data
 */
class ConflateStatsHelper
{
public:

  ConflateStatsHelper(const QList<SingleStat> input1Stats, const QList<SingleStat> input2Stats,
                      const QList<SingleStat> outputStats);

  /**
   * Appends additional conflation stats to a set of existing stats.
   *
   * @param statsToUpdate the set of stats to update
   */
  void updateStats(QList<SingleStat>& statsToUpdate) const;
  /**
   * Insert additional conflation stats into a set of existing stats at the specified location.
   *
   * @param statsToUpdate the set of stats to update
   * @param insertIndex the location in statsToUpdate where the stats should be insert
   */
  void updateStats(QList<SingleStat>& statsToUpdate, long insertIndex) const;

  /**
   * Retrieves a stat by name from a set of stats
   *
   * O(n)
   *
   * @param statName the name of the stat to retrieve
   * @param stats the list of stats to retrieve the stat from
   * @return the stat's value
   */
  static double getSingleStat(const QString& statName, const QList<SingleStat> stats);

  /**
   * Determines if a named stat exists in a a set of stats
   *
   * O(n)
   *
   * @param statName the name of the stat to retrieve
   * @param stats the list of stats to retrieve the stat from
   * @return true if the stat exists; false otherwise
   */
  static bool hasSingleStat(const QString& statName, const QList<SingleStat> stats);

private:

  QList<SingleStat> _input1Stats;
  QList<SingleStat> _input2Stats;
  QList<SingleStat> _outputStats;

  /*
   * These are useful stats if the reference map (map 1) is being treated as ground truth.
   * Eventually, may want to add the option to turn this on/off.
   */
  void _addRefAsGroundTruthStats(QList<SingleStat>& statsToUpdate, long insertIndex) const;
};

}

#endif // CONFLATE_STATS_HELPER_H
