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
#ifndef NAMEEXTRACTOR_H
#define NAMEEXTRACTOR_H

// hoot
#include <hoot/core/algorithms/string/StringDistanceConsumer.h>
#include <hoot/core/algorithms/extractors/FeatureExtractorBase.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

class NameExtractor : public FeatureExtractorBase, public StringDistanceConsumer
{
public:

  NameExtractor();
  NameExtractor(StringDistancePtr d);

  static std::string className() { return "hoot::NameExtractor"; }

  virtual std::string getClassName() const { return NameExtractor::className(); }

  virtual std::string getName() const;

  virtual double extract(const OsmMap& map, const std::shared_ptr<const Element>& target,
    const std::shared_ptr<const Element>& candidate) const;

  virtual double extract(const ConstElementPtr& target,
    const ConstElementPtr& candidate) const;

  virtual void setStringDistance(const StringDistancePtr &sd) { _d = sd; }

  virtual QString getDescription() const
  { return "Calculates name similarity between features"; }

  long getNamesProcessed() const { return _namesProcessed; }
  bool getMatchAttemptMade() const { return _matchAttemptMade; }

private:

  StringDistancePtr _d;
  mutable long _namesProcessed;
  mutable bool _matchAttemptMade;
};

}

#endif // OVERLAPEXTRACTOR_H
