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
  ~NameExtractor() = default;

  static QString className() { return "hoot::NameExtractor"; }

  double extract(
    const OsmMap& map, const std::shared_ptr<Element const>& target,
    const std::shared_ptr<Element const>& candidate) const override;
  virtual double extract(const ConstElementPtr& target, const ConstElementPtr& candidate) const;

  void setStringDistance(const StringDistancePtr &sd) override { _d = sd; }

  QString getDescription() const override { return "Calculates name similarity between features"; }
  QString getClassName() const override { return className(); }
  QString getName() const override;

  long getNamesProcessed() const { return _namesProcessed; }
  bool getMatchAttemptMade() const { return _matchAttemptMade; }

private:

  StringDistancePtr _d;
  mutable long _namesProcessed;
  mutable bool _matchAttemptMade;
};

}

#endif // OVERLAPEXTRACTOR_H
