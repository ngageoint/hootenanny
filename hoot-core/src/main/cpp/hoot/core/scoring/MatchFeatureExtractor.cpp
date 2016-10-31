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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "MatchFeatureExtractor.h"

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/conflate/Match.h>
#include <hoot/core/conflate/MatchCreator.h>
#include <hoot/core/conflate/MatchDetails.h>
#include <hoot/core/conflate/MatchFactory.h>
#include <hoot/core/util/HootException.h>

// Standard
#include <numeric>

// Tgs
#include <tgs/Statistics/Random.h>

namespace hoot
{

MatchFeatureExtractor::MatchFeatureExtractor(bool evenClasses)
{
  _evenClasses = evenClasses;
  _matchFactory = &MatchFactory::getInstance();
}

void MatchFeatureExtractor::addMatchCreator(const shared_ptr<MatchCreator> &m)
{
  _creators.push_back(m);
}

MatchType MatchFeatureExtractor::_getActualMatchType(const set<ElementId> &eids,
  const shared_ptr<const OsmMap>& map) const
{
  set<QString> ref1, ref2, review;

  if (eids.size() > 2)
  {
    LOG_WARN("More than two eids. This may not give the intended result. As written now it only "
             "needs one match to call the whole set good.");
  }

  // go through the set of provided ways
  for (set<ElementId>::const_iterator it = eids.begin(); it != eids.end(); ++it)
  {
    const shared_ptr<const Element>& e = map->getElement(*it);
    if (e->getStatus() == Status::Unknown1)
    {
      QString r = e->getTags()["REF1"];
      // ignore all features that haven't been matched.
      if (r != "todo")
      {
        ref1.insert(r);
      }
    }
    else if (e->getStatus() == Status::Unknown2)
    {
      QStringList list;
      e->getTags().readValues("REF2", list);

      for (int i = 0; i < list.size(); i++)
      {
        QString r = list[i];
        // ignore all features that haven't been matched.
        if (r != "todo")
        {
          ref2.insert(r);
        }
      }

      QStringList reviewList;
      e->getTags().readValues("REVIEW", reviewList);

      for (int i = 0; i < reviewList.size(); i++)
      {
        QString r = reviewList[i];
        // ignore all features that haven't been matched.
        if (r != "todo")
        {
          review.insert(r);
        }
      }
    }
    else
    {
      LOG_WARN("Expected the element to be either Unknown1 or Unknown2. " << e->getStatus() <<
               " element: " << e->toString());
    }
  }

  // if at least one of the ways matches at least one of the other ways then this is a good match.
  MatchType result = MatchType::Miss;
  for (set<QString>::const_iterator it = ref1.begin(); it != ref1.end(); ++it)
  {
    if (ref2.find(*it) != ref2.end())
    {
      if (result == MatchType::Review)
      {
        LOG_WARN("Feature with multiple REF/REVIEW meanings. Bad data? " << eids);
      }
      result = MatchType::Match;
    }
    if (review.find(*it) != review.end())
    {
      if (result == MatchType::Match)
      {
        LOG_WARN("Feature with multiple REF/REVIEW meanings. Bad data? " << eids);
      }
      result = MatchType::Review;
    }
  }

  return result;
}

QString MatchFeatureExtractor::getResults(bool useNulls)
{
  set<QString> attributes;

  for (vector<Sample>::const_iterator it = _samples.begin(); it != _samples.end(); ++it)
  {
    const Sample& s = *it;
    for (Sample::const_iterator jt = s.begin(); jt != s.end(); ++jt)
    {
      if (jt->first != "class")
      {
        attributes.insert(jt->first);
      }
    }
  }

  QString result;

  result += QString("@RELATION manipulations\n\n");

  for (set<QString>::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
  {
    result += QString("@ATTRIBUTE %1 NUMERIC\n").arg(*it);
  }
  result += QString("@ATTRIBUTE class {match,miss,review}\n\n");

  result += QString("@DATA\n");

  if (_evenClasses)
  {
    _resampleClasses();
  }

  for (vector<Sample>::const_iterator st = _samples.begin(); st != _samples.end(); ++st)
  {
    const Sample& s = *st;

    QStringList l;
    for (set<QString>::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
    {
      if (s.find(*it) != s.end())
      {
        double v = s.find(*it)->second;
        l.append(QString("%1").arg(v));
      }
      else
      {
        if (useNulls)
        {
          l.append("?");
        }
        else
        {
          l.append("-1");
        }
      }
    }
    MatchType type = MatchType(s.find("class")->second);
    result += l.join(",") + "," + type.toString().toLower() + "\n";
  }

  return result;
}

void MatchFeatureExtractor::processMap(const shared_ptr<const OsmMap>& map)
{
  vector<const Match*> matches;
  Envelope bounds;
  bounds.setToNull();
  shared_ptr<const MatchThreshold> mt(new MatchThreshold(0, 0));
  _matchFactory->createMatches(map, matches, bounds, mt);
  // go through all the manipulators
  LOG_INFO("Processing " << matches.size() << " matches...");
  for (size_t i = 0; i < matches.size(); i++)
  {
    const MatchDetails* d = dynamic_cast<const MatchDetails*>(matches[i]);
    if (d == 0)
    {
      LOG_WARN("Match does not implement MatchDetails. " << matches[i]->toString());
    }
    else
    {
      try
      {
        Sample s = d->getFeatures(map);

        set< pair<ElementId, ElementId> > pairs = matches[i]->getMatchPairs();
        if (pairs.size() != 1)
        {
          LOG_WARN("Got more than one match pair, this case is not handled.");
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
      catch (const NeedsReviewException& e)
      {
        // pass don't include the bad match pairs. A classifier won't impact those situations.
      }
    }
  }
  LOG_INFO("Collected " << _samples.size() << " samples.");
}

void MatchFeatureExtractor::_resampleClasses()
{
  vector<Sample> classes[3];

  size_t maxAttrs = 0;
  for (size_t i = 0; i < _samples.size(); i++)
  {
    maxAttrs = max(maxAttrs, _samples[i].size());
  }

  for (size_t i = 0; i < _samples.size(); i++)
  {
    if (_samples[i].size() == maxAttrs)
    {
      int classId = round(_samples[i]["class"]);
      if (classId > MatchType::Review || classId < MatchType::Miss)
      {
        LOG_VAR(_samples[i]);
        LOG_WARN("Got class id: " << _samples[i]["class"]);
        throw HootException("Unexpected class id");
      }

      classes[classId].push_back(_samples[i]);
    }
  }

  size_t minSize = classes[0].size();

  for (size_t i = 0; i < 3; i++)
  {
    minSize = min(minSize, classes[i].size());
  }

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
