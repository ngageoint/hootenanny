
#ifndef TO_ENGLISH_TRANSLATION_COMPARISON_VISITOR_H
#define TO_ENGLISH_TRANSLATION_COMPARISON_VISITOR_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/algorithms/LevenshteinDistance.h>

#include "ElementOsmMapVisitor.h"

namespace hoot
{

/**
 *
 */
class ToEnglishTranslationComparisonVisitor : public ElementOsmMapVisitor, public Configurable
{

public:

  static std::string className() { return "hoot::ToEnglishTranslationComparisonVisitor"; }

  ToEnglishTranslationComparisonVisitor();

  virtual void visit(const boost::shared_ptr<Element>& e);

  virtual void setConfiguration(const Settings& conf);

  virtual QString getDescription() const
  { return "TODO"; }

private:

  LevenshteinDistance _levDist;

  void _translateName(Tags& tags, const QString preTranslatedNameKey,
                      const QString toTranslateNameKey);
};

}

#endif // TO_ENGLISH_TRANSLATION_COMPARISON_VISITOR_H
