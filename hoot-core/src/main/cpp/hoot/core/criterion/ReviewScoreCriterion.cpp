
#include "ReviewScoreCriterion.h"

// Hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, ReviewScoreCriterion)

ReviewScoreCriterion::ReviewScoreCriterion() :
ReviewRelationCriterion(),
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
      "Review score threshold outside of allowed range of 0.0 to 1.0: " +
      QString::number(threshold));
  }
  _minScoreThreshold = threshold;
}

void ReviewScoreCriterion::setMaxScoreThreshold(const double threshold)
{
  if (threshold < 0.0 || threshold > 1.0)
  {
    throw IllegalArgumentException(
      "Review score threshold outside of allowed range of 0.0 to 1.0: " +
      QString::number(threshold));
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
  if (ReviewRelationCriterion::isSatisfied(e))
  {
    bool ok = false;
    const double score = e->getTags().get(MetadataTags::HootReviewScore()).toDouble(&ok);
    LOG_VART(score);
    if (!ok || score > 1.0 || score < 0.0)
    {
      LOG_TRACE("Skipping invalid score: " << score << " for: " << e->getElementId() << "...");
//      if (_invertThresholding)
//      {
//        LOG_DEBUG("Passed filter.");
//        return true;
//      }
//      else
//      {
//        LOG_DEBUG("Did not pass filter.");
//        return false;
//      }
      return _invertThresholding ? true : false;
    }
    LOG_TRACE("Filtering review: " << e->getElementId() << "...");
//    if (_invertThresholding &&
//        ((_minScoreThreshold == 0.0 && _maxScoreThreshold == 1.0) ||
//         (_minScoreThreshold == 0.0 && score == 0.0) ||
//         (_maxScoreThreshold == 1.0 && score == 1.0) ||
//         score < _minScoreThreshold || score > _maxScoreThreshold))
//    {
//      LOG_DEBUG("Passed filter.");
//      return true;
//    }
//    else if (!_invertThresholding && (score >= _minScoreThreshold && score <= _maxScoreThreshold))
//    {
//      LOG_DEBUG("Passed filter.");
//      return true;
//    }
//    else
//    {
//      LOG_DEBUG("Did not pass filter.");
//      return false;
//    }
    return
      _invertThresholding ?
        // inverted; handle boundary conditions
        ((_minScoreThreshold == 0.0 && _maxScoreThreshold == 1.0) ||
         (_minScoreThreshold == 0.0 && score == 0.0) ||
         (_maxScoreThreshold == 1.0 && score == 1.0) ||
         score < _minScoreThreshold ||
         score > _maxScoreThreshold) :
        // not inverted
        (score >= _minScoreThreshold && score <= _maxScoreThreshold);
  }
  return false;
}

}
