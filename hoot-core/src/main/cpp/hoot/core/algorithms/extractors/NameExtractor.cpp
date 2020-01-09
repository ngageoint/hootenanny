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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "NameExtractor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/algorithms/string/LevenshteinDistance.h>
#include <hoot/core/util/Log.h>

using namespace std;

namespace hoot
{

QHash<ElementId, QStringList> NameExtractor::_nameCache;
int NameExtractor::_nameCacheHits;
bool NameExtractor::_cacheInitialized = false;

HOOT_FACTORY_REGISTER(FeatureExtractor, NameExtractor)

NameExtractor::NameExtractor():
_d(new LevenshteinDistance()),
_namesProcessed(0),
_matchAttemptMade(false)
{
}

NameExtractor::NameExtractor(StringDistancePtr d):
_d(d),
_namesProcessed(0),
_matchAttemptMade(false)
{
}

double NameExtractor::extract(const OsmMap& /*map*/, const std::shared_ptr<const Element>& target,
  const std::shared_ptr<const Element>& candidate) const
{
//  if (!_cacheInitialized)
//  {
//    _nameCache.reserve(map.size());
//    _cacheInitialized = true;
//  }

  return extract(target, candidate);
}

double NameExtractor::extract(const ConstElementPtr& target, const ConstElementPtr& candidate) const
{
  QStringList targetNames, candidateNames;

//  QHash<ElementId, QStringList>::const_iterator targetNameItr =
//    _nameCache.find(target->getElementId());
//  if (targetNameItr != _nameCache.end())
//  {
//    _nameCacheHits++;
//    targetNames = targetNameItr.value();
//    LOG_TRACE("Found " << targetNames.size() << " names for: " << target->getElementId());
//  }
//  else
//  {
    targetNames = target->getTags().getNames();
    targetNames.append(target->getTags().getPseudoNames());
    //_nameCache[target->getElementId()] = targetNames;
  //}
  _namesProcessed += targetNames.size();

//  QHash<ElementId, QStringList>::const_iterator candidateNameItr =
//    _nameCache.find(candidate->getElementId());
//  if (candidateNameItr != _nameCache.end())
//  {
//    _nameCacheHits++;
//    candidateNames = candidateNameItr.value();
//    LOG_TRACE("Found " << candidateNames.size() << " names for: " << candidate->getElementId());
//  }
//  else
//  {
    candidateNames = candidate->getTags().getNames();
    candidateNames.append(candidate->getTags().getPseudoNames());
    //_nameCache[candidate->getElementId()] = candidateNames;
  //}
  _namesProcessed += candidateNames.size();

  double score = -1;

  for (int i = 0; i < targetNames.size(); i++)
  {
    const QString targetName = targetNames[i];
    for (int j = 0; j < candidateNames.size(); j++)
    {
      _matchAttemptMade = true;
      const QString candidateName = candidateNames[j];
      LOG_VART(targetName);
      LOG_VART(candidateName);
      const double thisScore = _d->compare(targetName, candidateName);
      score = max(thisScore, score);
      LOG_VART(score);
      if (score == 1.0)
      {
        return score;
      }
    }
  }

  if (score == -1)
  {
    return nullValue();
  }

  return score;
}

string NameExtractor::getName() const
{
  return "name " + _d->toString().toStdString();
}

}
