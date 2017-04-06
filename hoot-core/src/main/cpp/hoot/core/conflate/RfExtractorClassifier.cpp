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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RfExtractorClassifier.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/conflate/MatchType.h>
#include <hoot/core/conflate/extractors/FeatureExtractor.h>
#include <hoot/core/conflate/MatchClassification.h>

// Standard
#include <sstream>

// tgs
#include <tgs/RandomForest/RandomForest.h>

namespace hoot
{

unsigned int RfExtractorClassifier::logWarnCount = 0;

RfExtractorClassifier::RfExtractorClassifier()
{
}

MatchClassification RfExtractorClassifier::classify(const ConstOsmMapPtr& map,
  ElementId eid1, ElementId eid2) const
{
  MatchClassification result;

  std::map<QString, double> features = getFeatures(map, eid1, eid2);
  vector<double> row;
  row.reserve(features.size());
  for (std::map<QString, double>::const_iterator it = features.begin(); it != features.end(); it++)
  {
    row.push_back(it->second);
  }
  std::map<string, double> scores;
  _rf->classifyVector(row, scores);

  result.setMissP(scores["miss"]);
  result.setMatchP(scores["match"]);
  result.setReviewP(scores["review"]);

  return result;
}

const vector< boost::shared_ptr<const FeatureExtractor> >& RfExtractorClassifier::_getExtractors()
  const
{
  if (_extractors.size() == 0)
  {
    _createExtractors();
  }

  return _extractors;
}

map<QString, double> RfExtractorClassifier::getFeatures(const boost::shared_ptr<const OsmMap>& m,
  ElementId eid1, ElementId eid2) const
{
  map<QString, double> result;

  const boost::shared_ptr<const Element>& e1 = m->getElement(eid1);
  const boost::shared_ptr<const Element> e2 = m->getElement(eid2);

  _getExtractors();
  for (size_t i = 0; i < _extractors.size(); i++)
  {
    double v = _extractors[i]->extract(*m, e1, e2);
    // if it isn't null then include it.
    if (!FeatureExtractor::isNull(v))
    {
      QString factorName = QString::fromStdString(_extractors[i]->getName()).
          replace(QRegExp("[^\\w]"), "_");
      result[factorName] = v;
    }
  }

  return result;
}

void RfExtractorClassifier::import(QDomElement& docRoot)
{
  _getExtractors();

  _rf.reset(new RandomForest());
  _rf->importModel(docRoot);
  vector<string> factorLabels = _rf->getFactorLabels();

  QStringList extractorNames;
  for (size_t i = 0; i < _extractors.size(); i++)
  {
    extractorNames.append(QString::fromUtf8(_extractors[i]->getName().data()).
                          replace(QRegExp("[^\\w]"), "_"));
  }

  QStringList missingExtractors;
  for (size_t i = 0; i < factorLabels.size(); i++)
  {
    QString fn = QString::fromUtf8(factorLabels[i].data());
    if (extractorNames.contains(fn) == false)
    {
      missingExtractors.append(fn);
    }
    _rfFactorLabels.append(fn);
  }

  if (missingExtractors.size() > 0)
  {
    if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN(
        "An extractor used by the model is not being calculated. We will still try, but this will "
        "undoubtably result in poor quality matches. Missing extractors:");
    }
    else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
    LOG_TRACE("Missing extractors: " << missingExtractors);
    LOG_TRACE("Available extractors: " << extractorNames);
  }
}

}
