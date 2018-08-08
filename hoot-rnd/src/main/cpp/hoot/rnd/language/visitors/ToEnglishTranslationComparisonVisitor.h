
#ifndef TO_ENGLISH_TRANSLATION_COMPARISON_VISITOR_H
#define TO_ENGLISH_TRANSLATION_COMPARISON_VISITOR_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/algorithms/StringDistance.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/rnd/language/JoshuaTranslator.h>

namespace hoot
{

/**
 * This is used to check hoot tag language translations against existing known (assumed?)
 * correct translations; e.g. compare hoot's English translation of a tag to that of name:en
 */
class ToEnglishTranslationComparisonVisitor : public QObject, public ElementOsmMapVisitor,
   public Configurable
{

  Q_OBJECT

public:

  static std::string className() { return "hoot::ToEnglishTranslationComparisonVisitor"; }

  ToEnglishTranslationComparisonVisitor();
  ~ToEnglishTranslationComparisonVisitor();

  virtual void visit(const boost::shared_ptr<Element>& e);

  virtual void setConfiguration(const Settings& conf);

  virtual QString getDescription() const
  { return "Scores selected tag value English translations against known translated tags"; }

private slots:

  void _translationComplete();

private:

  StringDistancePtr _translationScorer;
  boost::shared_ptr<JoshuaTranslator> _translator;

  QStringList _preTranslatedTagKeys;
  QStringList _toTranslateTagKeys;

  QString _toTranslateVal;
  QString _preTranslatedVal;
  QString _toTranslateTagKey;
  ElementPtr _element;

  bool _skipWordsInEnglishDict;
  long _numTranslations;

  void _translate(const ElementPtr& e, const QString preTranslatedTagKey,
                  const QString toTranslateTagKey);
};

}

#endif // TO_ENGLISH_TRANSLATION_COMPARISON_VISITOR_H
