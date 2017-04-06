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

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/conflate/splitter/DualWaySplitter.h>
#include <hoot/core/conflate/SuperfluousWayRemover.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/scoring/AttributeComparator.h>
#include <hoot/core/scoring/GraphComparator.h>
#include <hoot/core/scoring/RasterComparator.h>
#include <hoot/core/visitors/KeepHighwaysVisitor.h>
#include <hoot/core/util/ConfigOptions.h>

// tgs
#include <tgs/Optimization/NelderMead.h>

namespace hoot
{

class ScoreCmd : public BaseCommand
{
public:

  static string className() { return "hoot::ScoreCmd"; }

  ScoreCmd() { }

  virtual QString getName() const { return "score"; }

  void attributeCompare(boost::shared_ptr<OsmMap> map1, boost::shared_ptr<OsmMap> map2, boost::shared_ptr<OsmMap> outMap,
                        int& mean, int& confidence)
  {
    Tgs::Random::instance()->seed(100);

    int iterations = 600;
    {
      AttributeComparator attr(map1, outMap);
      attr.setIterations(iterations);
      attr.compareMaps();
      int thisConfidence = attr.getConfidenceInterval() * 1000.0 + 0.5;
      int thisMean = attr.getMeanScore() * 1000.0 + 0.5;
      if (map2 != 0)
      {
        cout << "Attribute Score 1: " << thisMean << " +/-" << thisConfidence << endl;
        cout.flush();
      }

      confidence = thisConfidence;
      mean = thisMean;
    }

    if (map2 != 0)
    {
      AttributeComparator attr(map2, outMap);
      attr.setIterations(iterations);
      attr.compareMaps();
      int thisConfidence = attr.getConfidenceInterval() * 1000.0 + 0.5;
      int thisMean = attr.getMeanScore() * 1000.0 + 0.5;
      cout << "Attribute Score 2: " << thisMean << " +/-" << thisConfidence << endl;

      confidence += thisConfidence;
      mean += thisMean;

      confidence /= 2;
      mean /= 2;
    }
  }

  void graphCompare(boost::shared_ptr<OsmMap> map1, boost::shared_ptr<OsmMap> map2, double& mean,
                    double& confidence)
  {
    Tgs::Random::instance()->seed(0);
    GraphComparator graph(map1, map2);
    graph.setDebugImages(ConfigOptions().getScoreGraphDebugImages());
    graph.setIterations(1000);
    graph.setPixelSize(10);
    graph.compareMaps();
    double thisConfidence = graph.getConfidenceInterval();
    double thisMean = graph.getMeanScore();

    confidence += thisConfidence;
    mean += thisMean;
  }

  void rasterCompare(boost::shared_ptr<OsmMap> map1, boost::shared_ptr<OsmMap> map2, double& mean)
  {
    RasterComparator raster(map1, map2);
    raster.setPixelSize(5);
    mean += raster.compareMaps();
  }

  int compareMaps(QString in1, QString in2, QString out)
  {
    boost::shared_ptr<OsmMap> map1 = loadMap(in1);
    boost::shared_ptr<OsmMap> map2;
    if (in2 != "")
    {
      map2 = loadMap(in2);
    }
    boost::shared_ptr<OsmMap> outMap = loadMap(out);

    int aic, aim;
    attributeCompare(map1, map2, outMap, aim, aic);
    cout << "Attribute Score: " << aim <<
            " +/-" << aic << " (" << aim - aic << " to " << aim + aic << ")" << endl;

    double rMean = 0.0;
    rasterCompare(map1, outMap, rMean);
    if (map2 != 0)
    {
      double rMean2 = 0.0;
      rasterCompare(map2, outMap, rMean2);
      cout << "Raster Score 1: " << int(rMean * 1000.0 + 0.5) << endl;
      cout << "Raster Score 2: " << int(rMean2 * 1000.0 + 0.5) << endl;
      rMean = (rMean + rMean2) / 2.0;
    }
    int rasterScore = int(rMean * 1000.0 + 0.5);
    cout << "Raster Score: " << rasterScore << endl;

    double gMean = 0.0;
    double gConfidence = 0.0;
    graphCompare(map1, outMap, gMean, gConfidence);
    if (map2 != 0)
    {
      double gMean2 = 0.0;
      double gConfidence2 = 0.0;
      graphCompare(map2, outMap, gMean2, gConfidence2);
      int gic = gConfidence * 1000.0 + 0.5;
      int gim = gMean * 1000.0 + 0.5;
      cout << "Graph Score 1: " << gim <<
              " +/-" << gic << " (" << gim - gic << " to " << gim + gic << ")" << endl;
      gic = gConfidence2 * 1000.0 + 0.5;
      gim = gMean2 * 1000.0 + 0.5;
      cout << "Graph Score 2: " << gim <<
              " +/-" << gic << " (" << gim - gic << " to " << gim + gic << ")" << endl;

      gConfidence = (gConfidence + gConfidence2) / 2.0;
      gMean = (gMean + gMean2) / 2.0;
    }
    int gic = gConfidence * 1000.0 + 0.5;
    int gim = gMean * 1000.0 + 0.5;

    cout << "Graph Score: " << gim <<
            " +/-" << gic << " (" << gim - gic << " to " << gim + gic << ")" << endl;

    int overall = (aim + gim + rasterScore) / 3;
    int overallConf = (aic + gic) / 3;
    cout << "Overall: " << overall << " +/-" << overallConf << " (" << overall - overallConf <<
        " to " << overall + overallConf << ")" << endl;

    return 0;
  }

  boost::shared_ptr<OsmMap> loadMap(QString p)
  {
    boost::shared_ptr<OsmMap> result(new OsmMap());
    BaseCommand::loadMap(result, p, false);

    SuperfluousWayRemover(result).removeWays();
    // drop everything that isn't a highway.
    KeepHighwaysVisitor keepHighways;
    result->visitRw(keepHighways);
    result->visitRw(keepHighways);
    result->visitRw(keepHighways);

    return result;
  }

  virtual int runSimple(QStringList args)
  {
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

    return compareMaps(base1, base2, uut);
  }
};

HOOT_FACTORY_REGISTER(Command, ScoreCmd)

}

