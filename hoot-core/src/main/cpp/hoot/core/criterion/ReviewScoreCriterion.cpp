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
 * @copyright Copyright (C) 2019, 2021 Maxar (http://www.maxar.com/)
 */

#include "ReviewScoreCriterion.h"

// Hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, ReviewScoreCriterion)

ReviewScoreCriterion::ReviewScoreCriterion()
  : ReviewRelationCriterion(),
    _minScoreThreshold(0.0),
    _maxScoreThreshold(1.0),
    _invertThresholding(false)
{
}

void ReviewScoreCriterion::setMinScoreThreshold(const double threshold)
{
  if (threshold < 0.0 || threshold > 1.0)
  {
    throw IllegalArgumentException(
      "Review score threshold outside of allowed range of 0.0 to 1.0: " + QString::number(threshold));
  }
  _minScoreThreshold = threshold;
}

void ReviewScoreCriterion::setMaxScoreThreshold(const double threshold)
{
  if (threshold < 0.0 || threshold > 1.0)
  {
    throw IllegalArgumentException(
      "Review score threshold outside of allowed range of 0.0 to 1.0: " + QString::number(threshold));
  }
  _maxScoreThreshold = threshold;
}

void ReviewScoreCriterion::setConfiguration(const Settings& conf)
{
  ConfigOptions opts = ConfigOptions(conf);
  _minScoreThreshold = opts.getReviewScoreCriterionMinThreshold();
  _maxScoreThreshold = opts.getReviewScoreCriterionMaxThreshold();
  // When combining this with RemoveElementsVisitor, using the element crit negate option
  // didn't work for what I was trying to do with score filtering here, so I added this negate
  // option instead.
  _invertThresholding = opts.getReviewScoreCriterionInvertThresholding();
  LOG_VART(_minScoreThreshold);
  LOG_VART(_maxScoreThreshold);
  LOG_VART(_invertThresholding);
}

bool ReviewScoreCriterion::isSatisfied(const ConstElementPtr& e) const
{
  QString status = "did not meet";
  bool criterionMet = false;
  if (ReviewRelationCriterion::isSatisfied(e))
  {
    bool ok = false;
    const double score = e->getTags().get(MetadataTags::HootReviewScore()).toDouble(&ok);
    LOG_VART(score);

    if (!ok || score > 1.0 || score < 0.0)
    {
      LOG_TRACE("Skipping invalid score: " << score << " for: " << e->getElementId() << "...");
      criterionMet = true ? _invertThresholding : false;
    }
    else
    {
      if (!_invertThresholding)
        criterionMet = score >= _minScoreThreshold && score <= _maxScoreThreshold;
      else
      {
        // boundary conditions
        if ((_minScoreThreshold == 0.0 && _maxScoreThreshold == 1.0) ||
            (_minScoreThreshold == 0.0 && score == 0.0) ||
            (_maxScoreThreshold == 1.0 && score == 1.0))
        {
          criterionMet = false;
        }
        else
          criterionMet = score < _minScoreThreshold || score > _maxScoreThreshold;
      }
    }
  }
  if ((!_invertThresholding && criterionMet) || (_invertThresholding && !criterionMet))
    status = "met";
  LOG_TRACE("Review " << status << " criterion: " << e->getElementId() << "...");
  return criterionMet;
}

}
