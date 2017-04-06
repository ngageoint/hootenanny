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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MapStatsWriter.h"

// hoot
#include <hoot/core/ops/CalculateStatsOp.h>
#include <hoot/core/ops/stats/SingleStat.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/OsmUtils.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/conflate/StatsComposer.h>

//Qt
#include <QFile>
#include <QTextStream>
#include <QProcess>

// Boost
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
namespace pt = boost::property_tree;

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

void MapStatsWriter::writeStats(QList< QList<SingleStat> >& stats, QStringList names)
{
  ConfigOptions configOptions;
  QString statsFormat = configOptions.getStatsFormat();
  QString statsOutput = configOptions.getStatsOutput();
  QString statsClassName = configOptions.getStatsClass();
  QString statsScript = configOptions.getStatsScript();
  LOG_DEBUG("stats format = " << statsFormat << endl);
  LOG_DEBUG("stats outfile= " << statsOutput << endl);
  LOG_DEBUG("stats className= " << statsClassName << endl);
  LOG_DEBUG("stats script= " << statsScript << endl);

  if(statsOutput=="") return; // just need to specify only output file, rest of args have default values

  // create map for additional args
  QVariantMap args;

  // get the translation schema
  std::string schemaPath = configOptions.getStatsTranslateScript().toStdString();
  std::string baseSchema = schemaPath.substr(schemaPath.find_last_of("/\\") + 1);
  args["Translation Schema"] = QString::fromUtf8(baseSchema.c_str());

  // get the conflation types
  args["Conflation Type"] = configOptions.getConflateStatsTypes().replace("\"","");

  // get the conflation match thresholds
  args["Building Match Threshold"] = configOptions.getBuildingMatchThreshold();
  args["Highway Match Threshold"] = configOptions.getHighwayMatchThreshold();
  args["POI Match Threshold"] = configOptions.getPoiMatchThreshold();

  // get the conflation miss thresholds
  args["Building Miss Threshold"] = configOptions.getBuildingMissThreshold();
  args["Highway Miss Threshold"] = configOptions.getHighwayMissThreshold();
  args["POI Miss Threshold"] = configOptions.getPoiMissThreshold();

  // get the conflation review thresholds
  args["Building Review Threshold"] = configOptions.getBuildingReviewThreshold();
  args["Highway Review Threshold"] = configOptions.getHighwayReviewThreshold();
  args["POI Review Threshold"] = configOptions.getPoiReviewThreshold();

  // get the highway search radius
  args["Highway Search Radius"] = configOptions.getSearchRadiusHighway();
  args["All Other Search Radius"] = configOptions.getSearchRadiusDefault();

  // initialize the plugin
  StatsComposer* sc = Factory::getInstance().constructObject<StatsComposer>(statsClassName);
  sc->initialize( statsScript, statsFormat, statsOutput, args);

  // compose the stats object
  QString asciidoc = sc->compose( stats, names );

  // write the file out (it will always generate asciidoc because needed for pdf and html)
  QFile outputFile(statsOutput+".asciidoc");
  if (outputFile.exists())
  {
    outputFile.remove();
  }
  if (outputFile.open(QFile::WriteOnly | QFile::Text))
  {
    QTextStream out(&outputFile);
    out << asciidoc;
    outputFile.close();
  }
  else
  {
    LOG_ERROR("Unable to write to output file.");
  }

  // if PDF then convert asciidoc
  if(statsFormat=="pdf") {
    QString program = "/usr/bin/a2x";
    QStringList arguments;
    arguments << "-v" <<
      "--dblatex-opts \"-P latex.output.revhistory=0 -P 'doc.layout=mainmatter' -P doc.publisher.show=0\"" <<
      "-f pdf" << statsOutput+".asciidoc";
    QString prog = program + QString(" ") + arguments.join(" ");

    int result = system(prog.toStdString().c_str());
    if(result != 0)
    {
      LOG_ERROR("The system command for generating PDF file created a return value of"+result);
    }
  }
  else if(statsFormat=="html") {
    QString program = "/usr/bin/asciidoc";
    QStringList arguments;
    arguments << "-a" << "data-uri" << "-a" << "icons" << statsOutput+".asciidoc";
    QProcess myProcess;
    myProcess.start(program, arguments);
    myProcess.waitForFinished(-1);
  }
}

void MapStatsWriter::writeStatsToJson(QList< QList<SingleStat> >& stats, const QString& statsOutputFilePath)
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
  catch (std::exception e)
  {
    QString reason = e.what();
    LOG_ERROR("Error writing JSON " + reason);
  }
}

void MapStatsWriter::writeStatsToText(QList<QList<SingleStat> > &stats, const QString &statsOutputFilePath)
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
  conf().set(ConfigOptions().getReaderUseFileStatusKey(), true);
  boost::shared_ptr<OsmMap> map(new OsmMap());
  OsmUtils::loadMap(map, mapInputPath, true, Status::Invalid);
  MapProjector::projectToPlanar(map);

  QList< QList<SingleStat> > allStats;
  boost::shared_ptr<CalculateStatsOp> cso(new CalculateStatsOp());
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

QString MapStatsWriter::statsToString(QList< QList<SingleStat> >& stats, QString sep)
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
          l << QString::number(stats[j][k].value, 'g', precision);
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
