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
#include "HighwayRfClassifier.h"

// hoot
#include <hoot/core/algorithms/aggregator/MeanAggregator.h>
#include <hoot/core/algorithms/aggregator/RmseAggregator.h>
#include <hoot/core/algorithms/aggregator/SigmaAggregator.h>
#include <hoot/core/algorithms/linearreference/WaySublineCollection.h>
#include <hoot/core/algorithms/splitter/MultiLineStringSplitter.h>
#include <hoot/core/algorithms/extractors/AngleHistogramExtractor.h>
#include <hoot/core/algorithms/extractors/EdgeDistanceExtractor.h>
#include <hoot/core/algorithms/extractors/WeightedMetricDistanceExtractor.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

// Standard
#include <fstream>

// Qt
#include <QFile>

using namespace std;
using namespace Tgs;

namespace hoot
{

int HighwayRfClassifier::logWarnCount = 0;

HOOT_FACTORY_REGISTER(HighwayClassifier, HighwayRfClassifier)

MatchClassification HighwayRfClassifier::classify(const ConstOsmMapPtr& map,
  ElementId eid1, ElementId eid2, const WaySublineMatchString& match)
{
  _init();

  MatchClassification p;

  if (match.isValid() == false)
  {
    p.setMiss();
  }
  else
  {
    std::map<QString, double> features = getFeatures(map, eid1, eid2, match);

    if (features.empty())
    {
      p.setMiss();
    }
    else
    {
      vector<double> row;
      row.reserve(_rfFactorLabels.size());
      for (size_t i = 0; i < (size_t)_rfFactorLabels.size(); i++)
      {
        std::map<QString, double>::const_iterator it = features.find(_rfFactorLabels[i]);
        if (it != features.end())
        {
          row.push_back(it->second);
        }
        else
        {
          // for now we're using -1 instead of null.
          row.push_back(-1);
        }
      }
      std::map<string, double> scores;
      _rf->classifyVector(row, scores);

      p.setMissP(scores["miss"]);
      p.setMatchP(scores["match"]);
      p.setReviewP(scores["review"]);
    }
  }

  return p;
}

void HighwayRfClassifier::_createTestExtractors() const
{
  _extractors.clear();

  _extractors.push_back(
    std::make_shared<EdgeDistanceExtractor>(std::make_shared<RmseAggregator>()));
  _extractors.push_back(
     std::make_shared<EdgeDistanceExtractor>(std::make_shared<SigmaAggregator>()));
  _extractors.push_back(std::make_shared<AngleHistogramExtractor>());

  _extractors.push_back(
    std::make_shared<WeightedMetricDistanceExtractor>(
      std::make_shared<MeanAggregator>(), std::make_shared<RmseAggregator>(),
      ConfigOptions().getSearchRadiusHighway()));

  // TODO: At some point names will make sense, but for now there isn't enough name data (#4874).
}

map<QString, double> HighwayRfClassifier::getFeatures(const ConstOsmMapPtr& m,
  ElementId eid1, ElementId eid2, const WaySublineMatchString& match) const
{
  _init();

  map<QString, double> result;

  // convert match into elements in a new map.
  set<ElementId> eids;
  eids.insert(eid1);
  eids.insert(eid2);
  OsmMapPtr copiedMap = std::make_shared<OsmMap>(m->getProjection());
  CopyMapSubsetOp(m, eids).apply(copiedMap);
  WaySublineMatchString copiedMatch(match, copiedMap);

  // split the shared line based on the matching subline
  ElementPtr match1, scraps1;
  ElementPtr match2, scraps2;
  WaySublineCollection string1 = copiedMatch.getSublineString1();
  WaySublineCollection string2 = copiedMatch.getSublineString2();

  MultiLineStringSplitter().split(
    copiedMap, string1, copiedMatch.getReverseVector1(), match1, scraps1);
  MultiLineStringSplitter().split(
    copiedMap, string2, copiedMatch.getReverseVector2(), match2, scraps2);

  if (!match1 || !match2)
  {
    LOG_VART(eid1);
    LOG_VART(eid2);
    LOG_VART(match.toString());
    throw NeedsReviewException(
      "Internal Error: Found a situation where the match after copy is invalid. Marking as needs "
      "review.  Expected a matching subline, but got an empty match. Please report this to "
      " https://github.com/ngageoint/hootenanny.");
  }
  else
  {
    for (size_t i = 0; i < _extractors.size(); i++)
    {
      double v = _extractors[i]->extract(*copiedMap, match1, match2);
      // if it isn't null then include it.
      if (!FeatureExtractor::isNull(v))
      {
        QString factorName = _extractors[i]->getName().replace(QRegExp("[^\\w]"), "_");
        result[factorName] = v;
      }
    }
  }

  return result;
}

void HighwayRfClassifier::_init() const
{
  if (!_rf)
  {
    _createTestExtractors();

    QString path = ConfPath::search(ConfigOptions().getConflateMatchHighwayModel());
    LOG_DEBUG("Loading highway model from: " << path);

    QFile file(path.toLatin1().data());
    if (!file.open(QIODevice::ReadOnly))
    {
      throw HootException("Error opening file: " + path);
    }
    _rf = std::make_shared<RandomForest>();
    try
    {
      _rf->importModel(file);
      file.close();
    }
    catch (const Exception&)
    {
      file.close();
      throw;
    }

    vector<string> factorLabels = _rf->getFactorLabels();
    LOG_VART(factorLabels);

    QStringList extractorNames;
    for (size_t i = 0; i < _extractors.size(); i++)
    {
      extractorNames.append(_extractors[i]->getName().replace(QRegExp("[^\\w]"), "_"));
    }

    QStringList missingExtractors;
    for (size_t i = 0; i < factorLabels.size(); i++)
    {
      QString fn = QString::fromStdString(factorLabels[i]);
      if (extractorNames.contains(fn) == false)
      {
        missingExtractors.append(fn);
      }
      _rfFactorLabels.append(fn);
    }
    LOG_VART(extractorNames);
    LOG_VART(missingExtractors);
    LOG_VART(_rfFactorLabels);

    if (!missingExtractors.empty())
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN(
          "An extractor used by the model is not being calculated. We will still try, but this " <<
          "will undoubtably result in poor quality matches. Missing extractors: " <<
          missingExtractors);
        LOG_TRACE("Available extractors: " << extractorNames);
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
  }
}

}
