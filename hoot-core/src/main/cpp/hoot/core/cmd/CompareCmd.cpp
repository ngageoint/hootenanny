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
#include <hoot/core/criterion/CriterionUtils.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/SuperfluousWayRemover.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/scoring/MapCompareUtils.h>

// Qt
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

class CompareCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::CompareCmd"; }

  CompareCmd() :
  _disableAttributeScoring(false),
  _disableRasterScoring(false),
  _disableGraphScoring(false),
  _aic(0),
  _aim(0),
  _rasterScore(0),
  _gic(0),
  _gim(0)
  {
  }

  QString getName() const override { return "compare"; }
  QString getDescription() const override { return "Compares maps using metrics"; }

  int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    if (args.contains("--disable-attribute"))
    {
      _disableAttributeScoring = true;
      args.removeAt(args.indexOf("--disable-attribute"));
    }
    if (args.contains("--disable-raster"))
    {
      _disableRasterScoring = true;
      args.removeAt(args.indexOf("--disable-raster"));
    }
    if (args.contains("--disable-graph"))
    {
      _disableGraphScoring = true;
      args.removeAt(args.indexOf("--disable-graph"));
    }
    if (_disableAttributeScoring && _disableRasterScoring && _disableGraphScoring)
    {
      throw IllegalArgumentException("No scoring method selected.");
    }
    LOG_VARD(args);

    if (args.size() < 2 || args.size() > 4)
    {
      LOG_VAR(args);
      cout << getHelp() << endl << endl;
      throw IllegalArgumentException(
        QString("%1 takes at two to four parameters.").arg(getName()));
    }

    const QString base1 = args[0];
    QString base2;
    QStringList criteria;
    QString uut;
    if (args.size() == 2)
    {
      uut = args[1];
    }
    else if (args.size() == 3)
    {
      // The last parameter could be either the second base map input or a list of criteria.
      if (OsmMapReaderFactory::isSupportedFormat(args[2]))
      {
        uut = args[2];
      }
      else
      {
        uut = args[1];
        criteria = args[2].split(";");
      }
    }
    else if (args.size() == 4)
    {
      base2 = args[1];
      uut = args[2];
      criteria = args[3].split(";");
    }

    LOG_VARD(base1);
    LOG_VARD(base2);
    LOG_VARD(criteria);
    LOG_VARD(uut);

    _compareMaps(base1, base2, uut, _getCrit(criteria));

    LOG_STATUS(
      "Maps compared in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }

private:

  bool _disableAttributeScoring;
  bool _disableRasterScoring;
  bool _disableGraphScoring;

  int _aic;
  int _aim;

  int _rasterScore;

  int _gic;
  int _gim;

  void _compareMaps(
    const QString& in1, const QString& in2, const QString& out,
    const ElementCriterionPtr& filteringCrit)
  {
    QString msg = "Comparing maps ..." + FileUtils::toLogFormat(in1, 25);
    if (!in2.isEmpty())
    {
      msg += ", ..." + FileUtils::toLogFormat(in2, 25);
    }
    msg += ", ..." + FileUtils::toLogFormat(out, 25);
    if (filteringCrit)
    {
      msg += ". Filtering with: " + filteringCrit->toString();
    }
    msg += "...";
    LOG_STATUS(msg);

    OsmMapPtr map1 = _loadMap(in1, filteringCrit);
    if (map1->isEmpty())
    {
      throw EmptyMapInputException();
    }
    OsmMapPtr map2;
    if (in2 != "")
    {
      map2 = _loadMap(in2, filteringCrit);
      if (map2->isEmpty())
      {
        throw EmptyMapInputException();
      }
    }
    OsmMapPtr outMap = _loadMap(out, filteringCrit);

    int numScores = 3;

    if (!_disableAttributeScoring)
    {
      _calculateAttributeScore(map1, map2, outMap, 600);
    }
    else
    {
      numScores--;
    }

    if (!_disableRasterScoring)
    {
      _calculateRasterScore(map1, map2, outMap);
    }
    else
    {
      numScores--;
    }

    if (!_disableGraphScoring)
    {
      // The graph score only makes sense for linear features, so we'll further filter the input
      // maps here to ensure only linear features are present.
      OsmMapPtr filteredMap1 = _filterToLinearOnly(map1);
      OsmMapPtr filteredMap2;
      if (map2)
      {
        filteredMap2 = _filterToLinearOnly(map2);
      }
      _calculateGraphScore(filteredMap1, filteredMap2, outMap);
    }
    else
    {
      numScores--;
    }

    const int overall = (_aim + _gim + _rasterScore) / numScores;
    const int overallConf = (_aic + _gic) / numScores;
    cout << "Overall: " << overall << " +/-" << overallConf << " (" << overall - overallConf <<
        " to " << overall + overallConf << ")" << endl;
  }

  ElementCriterionPtr _getCrit(QStringList& names) const
  {
    if (!names.isEmpty())
    {
      ConfigOptions opts;
      return
        CriterionUtils::constructCriterion(
          names, opts.getElementCriteriaChain(), opts.getElementCriteriaNegate());
    }
    return ElementCriterionPtr();
  }

  OsmMapPtr _loadMap(const QString& path, const ElementCriterionPtr& filteringCrit) const
  {
    LOG_STATUS("Loading input map: ..." << path << "...");
    OsmMapPtr map = std::make_shared<OsmMap>();
    IoUtils::loadMap(map, path, false);

    SuperfluousWayRemover::removeWays(map);
    LOG_VARD(map->size());

    if (filteringCrit)
    {
      LOG_STATUS("Filtering input map...");

      OsmMapConsumer* omc = dynamic_cast<OsmMapConsumer*>(filteringCrit.get());
      if (omc)
      {
        omc->setOsmMap(map.get());
      }

      OsmMapPtr filteredMap = std::make_shared<OsmMap>();
      CopyMapSubsetOp op(map, filteringCrit);
      op.apply(filteredMap);
      map = filteredMap;
      LOG_VARD(map->size());
    }

    OsmMapWriterFactory::writeDebugMap(map, className(), "after-input-filtering");

    return map;
  }

  OsmMapPtr _filterToLinearOnly(const ConstOsmMapPtr& map)
  {
    LOG_STATUS("Filtering input map to linear features only...");
    OsmMapPtr filteredMap = std::make_shared<OsmMap>();
    CopyMapSubsetOp op(map, std::make_shared<LinearCriterion>());
    op.setCopyChildren(true);
    op.apply(filteredMap);
    LOG_VARD(filteredMap->size());
    return filteredMap;
  }

  void _calculateAttributeScore(
    const OsmMapPtr& map1, const OsmMapPtr& map2, const OsmMapPtr& outMap, const int numIterations)
  {
    MapCompareUtils::getAttributeComparisonFinalScores(map1, outMap, _aim, _aic, numIterations);
    if (map2 != nullptr)
    {
      int aic2, aim2;
      MapCompareUtils::getAttributeComparisonFinalScores(map2, outMap, aim2, aic2, numIterations);
      _aic += aic2;
      _aim += aim2;
      _aic /= 2;
      _aim /= 2;
    }
    cout << "Attribute Score: " << _aim << " +/-" << _aic << " (" << _aim - _aic << " to " <<
            _aim + _aic << ")" << endl;
  }

  void _calculateRasterScore(const OsmMapPtr& map1, const OsmMapPtr& map2, const OsmMapPtr& outMap)
  {
    double rMean = 0.0;
    MapCompareUtils::getRasterComparisonRawScores(map1, outMap, rMean);
    if (map2 != nullptr)
    {
      double rMean2 = 0.0;
      MapCompareUtils::getRasterComparisonRawScores(map2, outMap, rMean2);
      LOG_STATUS(""); // Clear out the line after the progress logging with this.
      cout << "Raster Score 1: " << MapCompareUtils::convertRawScoreToFinalScore(rMean) << endl;
      cout << "Raster Score 2: " << MapCompareUtils::convertRawScoreToFinalScore(rMean2) << endl;
      rMean = (rMean + rMean2) / 2.0;
    }
    _rasterScore = MapCompareUtils::convertRawScoreToFinalScore(rMean);
    cout << "Raster Score: " << _rasterScore << endl;
  }

  void _calculateGraphScore(const OsmMapPtr& map1, const OsmMapPtr& map2, const OsmMapPtr& outMap)
  {
    double gMean = 0.0;
    double gConfidence = 0.0;
    MapCompareUtils::getGraphComparisonRawScores(map1, outMap, gMean, gConfidence);
    if (map2 != nullptr)
    {
      double gMean2 = 0.0;
      double gConfidence2 = 0.0;
      MapCompareUtils::getGraphComparisonRawScores(map2, outMap, gMean2, gConfidence2);
      _gic = MapCompareUtils::convertRawScoreToFinalScore(gConfidence);
      _gim = MapCompareUtils::convertRawScoreToFinalScore(gMean);
      cout << "Graph Score 1: " << _gim <<
              " +/-" << _gic << " (" << _gim - _gic << " to " << _gim + _gic << ")" << endl;
      _gic = MapCompareUtils::convertRawScoreToFinalScore(gConfidence2);
      _gim = MapCompareUtils::convertRawScoreToFinalScore(gMean2);
      cout << "Graph Score 2: " << _gim <<
              " +/-" << _gic << " (" << _gim - _gic << " to " << _gim + _gic << ")" << endl;

      gConfidence = (gConfidence + gConfidence2) / 2.0;
      gMean = (gMean + gMean2) / 2.0;
    }
    _gic = MapCompareUtils::convertRawScoreToFinalScore(gConfidence);
    _gim = MapCompareUtils::convertRawScoreToFinalScore(gMean);

    cout << "Graph Score: " << _gim <<
            " +/-" << _gic << " (" << _gim - _gic << " to " << _gim + _gic << ")" << endl;
  }
};

HOOT_FACTORY_REGISTER(Command, CompareCmd)

}
