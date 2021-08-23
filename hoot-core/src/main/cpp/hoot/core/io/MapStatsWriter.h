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
 * @copyright Copyright (C) 2015, 2016, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef MAP_STATS_WRITER
#define MAP_STATS_WRITER

// Qt

#include <QStringList>

namespace hoot
{
  class SingleStat;

/**
  Writes map stats
  */
class MapStatsWriter
{
public:

  /**
    Writes map stats to a file

    @param mapInputPath path of the input map
    @param statsOutputFilePath path of the output stats file
    @param sep stats string separator
    */
  void writeStats(
    const QString& mapInputPath, const QString& statsOutputFilePath, QString sep) const;

  /**
    Writes map stats to a json file

    @param map stats
    @param statsOutputFilePath path of the output stats file
    */
  void writeStatsToJson(QList<QList<SingleStat>>& stats, const QString& statsOutputFilePath) const;

  /**
    Write map stats to a text file

    @param map stats
    @param statsOutputFilePath path of the output stats file
    */
  void writeStatsToText(QList<QList<SingleStat>>& stats, const QString& statsOutputFilePath) const;

  /**
    Creates a string for map stats

    @param stats the stats to create the string for
    @param stats string separator
    */
  QString statsToString(QList<QList<SingleStat>>& stats, QString sep) const;

private:

  void _appendUnique(QList<SingleStat>& stats, QStringList& names) const;

};

}

#endif // MAP_STATS_WRITER
