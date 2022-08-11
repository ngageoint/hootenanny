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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "MatchFeatureExtractor.h"

// Hoot
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/conflate/matching/MatchCreator.h>
#include <hoot/core/conflate/matching/MatchDetails.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/StringUtils.h>

// Standard
#include <numeric>

// Tgs
#include <tgs/Statistics/Random.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

int MatchFeatureExtractor::logWarnCount = 0;

MatchFeatureExtractor::MatchFeatureExtractor(bool evenClasses)
  : _evenClasses(evenClasses),
    _matchFactory(&MatchFactory::getInstance())
{
}

void MatchFeatureExtractor::addMatchCreator(const std::shared_ptr<MatchCreator>& m)
{
  _creators.push_back(m);
}

MatchType MatchFeatureExtractor::_getActualMatchType(const set<ElementId> &eids,
                                                     const std::shared_ptr<const OsmMap>& map) const
{
  set<QString> ref1, ref2, review;

  if (eids.size() > 2)
  {
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN("More than two eids. This may not give the intended result. As written now it only "
               "needs one match to call the whole set good.");
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }

  // go through the set of provided elements
  for (const auto& eid : eids)
  {
    const std::shared_ptr<const Element>& e = map->getElement(eid);
    const Tags& tags = e->getTags();
    if (e->getStatus() == Status::Unknown1)
    {
      QString r = tags[MetadataTags::Ref1()];
      if (r != "todo")  //  ignore all features that haven't been matched.
        ref1.insert(r);
    }
    else if (e->getStatus() == Status::Unknown2)
    {
      QStringList list;
      tags.readValues(MetadataTags::Ref2(), list);

      for (const auto& r : qAsConst(list))
      {
        if (r != "todo")  //  ignore all features that haven't been matched.
          ref2.insert(r);
      }

      QStringList reviewList;
      tags.readValues("REVIEW", reviewList);

      for (const auto& r : qAsConst(reviewList))
      {
        if (r != "todo")  //  ignore all features that haven't been matched.
          review.insert(r);
      }
    }
    else
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Expected the element to be either " << MetadataTags::Unknown1() << " or " <<
                 MetadataTags::Unknown2() << ". " << e->getStatus() << " element: " <<
                 e->toString());
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
  }

  // if at least one of the ways matches at least one of the other ways then this is a good match.
  MatchType result = MatchType::Miss;
  for (const auto& tag : ref1)
  {
    if (ref2.find(tag) != ref2.end())
    {
      if (result == MatchType::Review)
      {
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN("Feature with multiple REF/REVIEW meanings. Bad data? " << eids);
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
      }
      result = MatchType::Match;
    }
    if (review.find(tag) != review.end())
    {
      if (result == MatchType::Match)
      {
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN("Feature with multiple REF/REVIEW meanings. Bad data? " << eids);
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
      }
      result = MatchType::Review;
    }
  }
  return result;
}

QString MatchFeatureExtractor::getResults(bool useNulls)
{
  set<QString> attributes;
  for (const auto& s : _samples)
  {
    for (auto jt = s.begin(); jt != s.end(); ++jt)
    {
      if (jt->first != "class")
        attributes.insert(jt->first);
    }
  }

  QStringList result;

  result += QString("@RELATION manipulations\n\n");

  for (set<QString>::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
    result += QString("@ATTRIBUTE %1 NUMERIC\n").arg(*it);

  result += QString("@ATTRIBUTE class {match,miss,review}\n\n");
  result += QString("@DATA\n");

  if (_evenClasses)
    _resampleClasses();

  for (const auto& s : _samples)
  {
    QStringList l;
    for (const auto& attribute : attributes)
    {
      if (s.find(attribute) != s.end())
        l.append(QString("%1").arg(s.find(attribute)->second));
      else
      {
        if (useNulls)
          l.append("?");
        else
          l.append("-1");
      }
    }
    MatchType type = MatchType(s.find("class")->second);
    result += QString("%1,%2\n").arg(l.join(","), type.toString().toLower());
  }
  return result.join("");
}

void MatchFeatureExtractor::processMap(const std::shared_ptr<const OsmMap>& map)
{
  vector<ConstMatchPtr> matches;
  // We don't want to validate the thresholds here in order to do the full match feature extraction.
  // Reviews aren't applicable here, so just use the default value.
  std::shared_ptr<const MatchThreshold> mt = std::make_shared<MatchThreshold>(0.0, 0.0, 1.0, false);
  _matchFactory->createMatches(map, matches, std::shared_ptr<geos::geom::Geometry>(), mt);
  size_t matchCount = 0;
  for (const auto& match : matches)
  {
    const MatchDetails* d = dynamic_cast<const MatchDetails*>(match.get());
    if (d == nullptr)
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Match does not implement MatchDetails. " << match->toString());
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
    else
    {
      try
      {
        Sample s = d->getFeatures(map);
        LOG_VART(s.size());

        set<pair<ElementId, ElementId>> pairs = match->getMatchPairs();
        if (pairs.size() != 1)
        {
          if (logWarnCount < Log::getWarnMessageLimit())
          {
            LOG_WARN("Got more than one match pair, this case is not handled.");
          }
          else if (logWarnCount == Log::getWarnMessageLimit())
          {
            LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
          }
          logWarnCount++;
        }
        // no need to store the record if there is no information
        else if (s.size() > 0)
        {
          set<ElementId> eids;
          eids.insert(pairs.begin()->first);
          eids.insert(pairs.begin()->second);
          LOG_VART(eids);
          s["class"] = _getActualMatchType(eids, map).toEnum();
          LOG_VART(s["class"]);
          _samples.push_back(s);
        }
      }
      catch (const NeedsReviewException&)
      {
        // pass - don't include the bad match pairs. A classifier won't impact those situations.
      }

      matchCount++;
      if (matchCount % 10 == 0)
      {
        PROGRESS_INFO(
          "Processed " << StringUtils::formatLargeNumber(matchCount) << " of " <<
          StringUtils::formatLargeNumber(matches.size()) << " matches; samples collected: " <<
          StringUtils::formatLargeNumber(_samples.size()));
      }
    }
  }
  LOG_INFO(
    "Collected " << StringUtils::formatLargeNumber(_samples.size()) << " samples from " <<
    StringUtils::formatLargeNumber(matches.size()) << " matches.");
}

void MatchFeatureExtractor::_resampleClasses()
{
  vector<Sample> classes[3];

  size_t maxAttrs = 0;
  for (const auto& sample : _samples)
    maxAttrs = max(maxAttrs, sample.size());

  for (const auto& sample : _samples)
  {
    if (sample.size() == maxAttrs)
    {
      int classId = static_cast<int>(round(sample.at("class")));
      if (classId > MatchType::Review || classId < MatchType::Miss)
      {
        LOG_VART(sample);
        LOG_TRACE("Got class id: " << sample.at("class"));
        throw HootException("Unexpected class id");
      }
      classes[classId].push_back(sample);
    }
  }

  size_t minSize = classes[0].size();
  for (size_t i = 0; i < 3; i++)
    minSize = min(minSize, classes[i].size());

  _samples.clear();
  _samples.reserve(minSize * 3);

  for (size_t i = 0; i < 3; i++)
  {
    Tgs::Random::randomizeVector(classes[i]);
    classes[i].resize(minSize);
    _samples.insert(_samples.end(), classes[i].begin(), classes[i].end());
  }
}

}
