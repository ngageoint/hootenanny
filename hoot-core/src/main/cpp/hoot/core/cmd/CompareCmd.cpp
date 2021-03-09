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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/ops/SuperfluousWayRemover.h>
#include <hoot/core/visitors/KeepHighwaysVisitor.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/scoring/MapCompareUtils.h>

// tgs
#include <tgs/Optimization/NelderMead.h>

// Qt
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

class CompareCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::CompareCmd"; }

  CompareCmd() = default;

  virtual QString getName() const override { return "compare"; }

  virtual QString getDescription() const override { return "Compares maps using metrics"; }

  int compareMaps(QString in1, QString in2, QString out)
  {
    OsmMapPtr map1 = loadMap(in1);
    if (map1->isEmpty())
    {
      return -1;
    }
    OsmMapPtr map2;
    if (in2 != "")
    {
      map2 = loadMap(in2);
      if (map2->isEmpty())
      {
        return -1;
      }
    }
    OsmMapPtr outMap = loadMap(out);

    int aic, aim;
    MapCompareUtils::getAttributeComparisonFinalScores(map1, outMap, aim, aic, 600);
    if (map2 != nullptr)
    {
      int aic2, aim2;
      MapCompareUtils::getAttributeComparisonFinalScores(map2, outMap, aim2, aic2, 600);
      aic += aic2;
      aim += aim2;
      aic /= 2;
      aim /= 2;
    }
    cout << "Attribute Score: " << aim <<
            " +/-" << aic << " (" << aim - aic << " to " << aim + aic << ")" << endl;

    double rMean = 0.0;
    MapCompareUtils::getRasterComparisonRawScores(map1, outMap, rMean);
    if (map2 != nullptr)
    {
      double rMean2 = 0.0;
      MapCompareUtils::getRasterComparisonRawScores(map2, outMap, rMean2);
      cout << "Raster Score 1: " << MapCompareUtils::convertRawScoreToFinalScore(rMean) << endl;
      cout << "Raster Score 2: " << MapCompareUtils::convertRawScoreToFinalScore(rMean2) << endl;
      rMean = (rMean + rMean2) / 2.0;
    }
    int rasterScore = MapCompareUtils::convertRawScoreToFinalScore(rMean);
    cout << "Raster Score: " << rasterScore << endl;

    double gMean = 0.0;
    double gConfidence = 0.0;
    MapCompareUtils::getGraphComparisonRawScores(map1, outMap, gMean, gConfidence);
    if (map2 != nullptr)
    {
      double gMean2 = 0.0;
      double gConfidence2 = 0.0;
      MapCompareUtils::getGraphComparisonRawScores(map2, outMap, gMean2, gConfidence2);
      int gic = MapCompareUtils::convertRawScoreToFinalScore(gConfidence);
      int gim = MapCompareUtils::convertRawScoreToFinalScore(gMean);
      cout << "Graph Score 1: " << gim <<
              " +/-" << gic << " (" << gim - gic << " to " << gim + gic << ")" << endl;
      gic = MapCompareUtils::convertRawScoreToFinalScore(gConfidence2);
      gim = MapCompareUtils::convertRawScoreToFinalScore(gMean2);
      cout << "Graph Score 2: " << gim <<
              " +/-" << gic << " (" << gim - gic << " to " << gim + gic << ")" << endl;

      gConfidence = (gConfidence + gConfidence2) / 2.0;
      gMean = (gMean + gMean2) / 2.0;
    }
    int gic = MapCompareUtils::convertRawScoreToFinalScore(gConfidence);
    int gim = MapCompareUtils::convertRawScoreToFinalScore(gMean);

    cout << "Graph Score: " << gim <<
            " +/-" << gic << " (" << gim - gic << " to " << gim + gic << ")" << endl;

    int overall = (aim + gim + rasterScore) / 3;
    int overallConf = (aic + gic) / 3;
    cout << "Overall: " << overall << " +/-" << overallConf << " (" << overall - overallConf <<
        " to " << overall + overallConf << ")" << endl;

    return 0;
  }

  OsmMapPtr loadMap(QString p)
  {
    OsmMapPtr result(new OsmMap());
    IoUtils::loadMap(result, p, false);

    SuperfluousWayRemover().removeWays(result);
    // drop everything that isn't a highway.
    KeepHighwaysVisitor keepHighways;
    result->visitRw(keepHighways);
    result->visitRw(keepHighways);
    result->visitRw(keepHighways);

    return result;
  }

  virtual int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    if (args.size() < 2 || args.size() > 3)
    {
      LOG_VAR(args);
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes at two or three parameters.").
                          arg(getName()));
    }

    QString base1 = args[0];
    QString base2 = args.size() == 3 ? args[1] : QString();
    QString uut = args.last();

    const int status = compareMaps(base1, base2, uut);

    LOG_STATUS(
      "Maps compared in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return status;
  }
};

HOOT_FACTORY_REGISTER(Command, CompareCmd)

}

