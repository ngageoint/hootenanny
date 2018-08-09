
#ifndef TO_ENGLISH_TRANSLATION_COMPARISON_VISITOR_H
#define TO_ENGLISH_TRANSLATION_COMPARISON_VISITOR_H

// hoot
#include <hoot/rnd/language/visitors/ToEnglishTranslationVisitor.h>
#include <hoot/core/algorithms/StringDistance.h>

namespace hoot
{

/**
 * This is used to check hoot tag language translations against existing known (assumed?)
 * correct translations; e.g. compare hoot's English translation of a tag to that of name:en
 */
class ToEnglishTranslationComparisonVisitor : public ToEnglishTranslationVisitor
{

public:

  static std::string className() { return "hoot::ToEnglishTranslationComparisonVisitor"; }

  ToEnglishTranslationComparisonVisitor();

  virtual void visit(const boost::shared_ptr<Element>& e);

  virtual void setConfiguration(const Settings& conf);

  virtual QString getDescription() const
  { return "Scores selected tag value English translations against known translated tags"; }

public slots:

  virtual void translationComplete();

private:

  StringDistancePtr _translationScorer;

  QStringList _preTranslatedTagKeys;
  QString _preTranslatedVal;

  void _translate(const ElementPtr& e, const QString preTranslatedTagKey,
                  const QString toTranslateTagKey);
};

}

#endif // TO_ENGLISH_TRANSLATION_COMPARISON_VISITOR_H
