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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef REVIEW_SCORE_CRITERION_H
#define REVIEW_SCORE_CRITERION_H

// Hoot
#include <hoot/core/criterion/ReviewRelationCriterion.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Allows for filtering reviews by score
 */
class ReviewScoreCriterion : public ReviewRelationCriterion, public Configurable
{
public:

  static QString className() { return "hoot::ReviewScoreCriterion"; }

  ReviewScoreCriterion();
  virtual ~ReviewScoreCriterion() = default;

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual void setConfiguration(const Settings& conf);

  virtual ElementCriterionPtr clone()
  { return ElementCriterionPtr(new ReviewScoreCriterion()); }

  virtual QString getDescription() const { return "Allows for filtering reviews by score"; }

  void setMinScoreThreshold(const double threshold);
  void setMaxScoreThreshold(const double threshold);
  void setInvertThresholding(const bool invert) { _invertThresholding = invert; }

  virtual QString getName() const override { return className(); }

  virtual QString getClassName() const override { return className(); }

private:

  double _minScoreThreshold;
  double _maxScoreThreshold;
  bool _invertThresholding;
};

}

#endif // REVIEW_SCORE_CRITERION_H
