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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MapStatsWriter.h"

// hoot
#include <hoot/core/ops/CalculateStatsOp.h>
#include <hoot/core/info/SingleStat.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/ConfigOptions.h>

//Qt
#include <QFile>
#include <QTextStream>
#include <QProcess>

// Boost
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
namespace pt = boost::property_tree;

using namespace std;

namespace hoot
{
void MapStatsWriter::_appendUnique(QList<SingleStat>& stats, QStringList& names)
{
  for (int i = 0; i < stats.size(); i++)
  {
    if (names.contains(stats[i].name) == false)
    {
      names.append(stats[i].name);
    }
  }
}

void MapStatsWriter::writeStatsToJson(QList<QList<SingleStat>>& stats,
                                      const QString& statsOutputFilePath)
{
  try
  {
    pt::ptree pt;
    QStringList allStats = statsToString(stats, "\t").split("\n");
    for (int i = 0; i < allStats.size(); i++)
    {
      QStringList statrow = allStats.at(i).split("\t");
      if (statrow.size() > 0 && !statrow[0].isEmpty())
      {
        QStringList tmpValues;
        //filter out empty values, first one in array is key, so the loop starts with 1
        for (int j = 1; j < statrow.size(); j++)
        {
          if (!statrow.at(j).trimmed().isEmpty())
          {
            tmpValues << statrow.at(j).trimmed();
          }
        }
        //if only one value in the array, do not use array in json file
        if (tmpValues.size() == 1)
        {
          pt::ptree child;
          child.put("", tmpValues.at(0).toStdString());
          pt.add_child(statrow.at(0).toStdString(), child);
        }
        else
        {
          pt::ptree children;
          for (int j = 0; j < tmpValues.size(); j++)
          {
            pt::ptree child;
            child.put("", tmpValues.at(j).toStdString());
            children.push_back(std::make_pair("", child));
          }
          pt.add_child(statrow.at(0).toStdString(), children);
        }
      }
    }
    pt::write_json(statsOutputFilePath.toStdString(), pt);
  }
  catch (const std::exception& e)
  {
    QString reason = e.what();
    LOG_ERROR("Error writing JSON " + reason);
  }
}

void MapStatsWriter::writeStatsToText(QList<QList<SingleStat>> &stats,
                                      const QString &statsOutputFilePath)
{
  LOG_INFO("Writing stats to file: " << statsOutputFilePath);

  //  Write to the text file
  QFile outputFile(statsOutputFilePath);
  if (outputFile.exists())
  {
    outputFile.remove();
  }
  if (outputFile.open(QFile::WriteOnly | QFile::Text))
  {
    QTextStream out(&outputFile);
    out << statsToString(stats, "\t");
    outputFile.close();
  }
  else
  {
    LOG_ERROR("Unable to write to output file.");
  }
}

void MapStatsWriter::writeStats(const QString& mapInputPath, const QString& statsOutputFilePath,
                                QString sep)
{
  LOG_INFO("Writing stats for map in file: " << mapInputPath << " to file: " << statsOutputFilePath);

  // read the conflation status from the file.
  conf().set(ConfigOptions::getReaderUseFileStatusKey(), true);
  OsmMapPtr map(new OsmMap());
  IoUtils::loadMap(map, mapInputPath, true, Status::Invalid);
  MapProjector::projectToPlanar(map);

  QList<QList<SingleStat>> allStats;
  std::shared_ptr<CalculateStatsOp> cso(new CalculateStatsOp());
  cso->apply(map);
  allStats.append(cso->getStats());

  QFile outputFile(statsOutputFilePath);
  if (outputFile.exists())
  {
    outputFile.remove();
  }
  if (outputFile.open(QFile::WriteOnly | QFile::Text))
  {
    QTextStream out(&outputFile);
    //out << "Stat Name\t" << args.join(sep) << endl;
    out << statsToString(allStats, sep);
    outputFile.close();
  }
  else
  {
    LOG_ERROR("Unable to write to output file.");
  }
}

QString MapStatsWriter::statsToString(QList<QList<SingleStat>>& stats, QString sep)
{
  QStringList allStatNames;

  for (int i = 0; i < stats.size(); i++)
  {
    _appendUnique(stats[i], allStatNames);
  }

  int precision = ConfigOptions().getWriterPrecision();
  QString result;
  // reserve a ballpark size to limit reallocs
  result.reserve(20 + 10 * allStatNames.size() * stats.size());
  for (int i = 0; i < allStatNames.size(); i++)
  {
    QStringList l;
    l << allStatNames[i];

    for (int j = 0; j < stats.size(); j++)
    {
      bool foundIt = false;

      for (int k = 0; k < stats[j].size(); k++)
      {
        if (stats[j][k].name == allStatNames[i])
        {
          QString value = QString::number(stats[j][k].value, 'g', precision);
          l << ((value != "nan") ? value : "-");
          foundIt = true;
        }
      }

      if (!foundIt)
      {
        l << "";
      }
    }

    result.append(l.join(sep) + "\n");
  }

  return result;
}

}
