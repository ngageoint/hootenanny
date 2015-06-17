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
 * @copyright Copyright (C) 2012, 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef PROBABILITYOFMATCH_H
#define PROBABILITYOFMATCH_H

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/ml/LogisticRegression.h>

// TGS
#include <tgs/SharedPtr.h>

namespace hoot
{
  using namespace geos::geom;

class ProbabilityOfMatch
{
public:
  ProbabilityOfMatch();

  static ProbabilityOfMatch& getInstance() { return _default; }

  double attributeScore(const ConstOsmMapPtr &map, const shared_ptr<const Way>& w1,
    const shared_ptr<const Way> &w2);

  map<QString, double> calculateStats(const ConstOsmMapPtr &map,
    const shared_ptr<const Way>& w1, const shared_ptr<const Way> &w2, double portionScore);

  double distanceScore(const ConstOsmMapPtr& map, const shared_ptr<const Way>& w1,
    const shared_ptr<const Way>& w2);

  double distanceScore(const hoot::ConstOsmMapPtr &map, const shared_ptr<const Way> &w1,
    const shared_ptr<const LineString> &ls2, Meters circularError);

  double evaluateSample(const map<QString, double>& sample) { return _model.evaluate(sample); }

  double lengthScore(const ConstOsmMapPtr& map, const shared_ptr<const Way>& w1,
    const shared_ptr<const Way>& w2);

  double modelProbability(const ConstOsmMapPtr& map, const shared_ptr<const Way>& w1,
    const shared_ptr<const Way>& w2, double portionScore);

  double parallelScore(const ConstOsmMapPtr& map, const shared_ptr<const Way>& w1,
    const shared_ptr<const Way>& w2);

  double portionScore(const ConstOsmMapPtr& map, const shared_ptr<const Way>& w1,
    const shared_ptr<const Way>& w2);

  double probability(const ConstOsmMapPtr& map, const shared_ptr<const Way>& w1,
    const shared_ptr<const Way>& w2, double portionScore);

  double expertProbability(const ConstOsmMapPtr &map, const shared_ptr<const Way>& w1,
    const shared_ptr<const Way> &w2);

  bool useModel() const { return _useModel; }

  double zipperScore(const ConstOsmMapPtr &map, const shared_ptr<const Way>& w1,
    const shared_ptr<const Way>& w2);

  static bool debug;

private:
  static ProbabilityOfMatch _default;
  bool _useModel;
  LogisticRegression _model;
  double _parallelExp;
  double _dMax;
  double _dMedian;

};

}

#endif // PROBABILITYOFMATCH_H
