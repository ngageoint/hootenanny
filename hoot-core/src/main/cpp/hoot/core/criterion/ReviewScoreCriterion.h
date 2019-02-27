
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

  static std::string className() { return "hoot::ReviewScoreCriterion"; }

  ReviewScoreCriterion();

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual void setConfiguration(const Settings& conf);

  virtual ElementCriterionPtr clone()
  { return ElementCriterionPtr(new ReviewScoreCriterion()); }

  virtual QString getDescription() const { return "Allows for filtering reviews by score"; }

  void setMinScoreThreshold(const double threshold);
  void setMaxScoreThreshold(const double threshold);
  void setInvertThresholding(const bool invert) { _invertThresholding = invert; }

private:

  double _minScoreThreshold;
  double _maxScoreThreshold;
  bool _invertThresholding;
};

}

#endif // REVIEW_SCORE_CRITERION_H
