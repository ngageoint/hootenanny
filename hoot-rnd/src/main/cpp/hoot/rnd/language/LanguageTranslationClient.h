
#ifndef LANGUAGE_TRANSLATION_COMPARISON_VISITOR_H
#define LANGUAGE_TRANSLATION_COMPARISON_VISITOR_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/algorithms/StringDistance.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>

// Qt
#include <QTcpSocket>

namespace hoot
{

/**
 * This is used to check hoot tag language translations against existing known (assumed?)
 * correct translations; e.g. compare hoot's English translation of a tag to that of name:en
 *
 * //TODO: move this to separate Joshua translator later
 * @article{post2015joshua,
    Author = {Post, Matt and Cao, Yuan and Kumar, Gaurav},
    Journal = {The Prague Bulletin of Mathematical Linguistics},
    Title = {Joshua 6: A phrase-based and hierarchical statistical machine translation system},
    Year = {2015}
  }
 */
class LanguageTranslationComparisonVisitor : public ElementOsmMapVisitor, public Configurable
{

public:

  static std::string className() { return "hoot::LanguageTranslationComparisonVisitor"; }

  LanguageTranslationComparisonVisitor();
  ~LanguageTranslationComparisonVisitor();

  virtual void visit(const boost::shared_ptr<Element>& e);

  virtual void setConfiguration(const Settings& conf);

  virtual QString getDescription() const
  { return "Scores tag language translations against known translated tags"; }

private:

  StringDistancePtr _translationScorer;
  boost::shared_ptr<QTcpSocket> _translationClient;
  QStringList _preTranslatedTagKeys;
  QStringList _toTranslateTagKeys;

  void _translate(Tags& tags, const QString preTranslatedNameKey, const QString toTranslateNameKey);
};

}

#endif // LANGUAGE_TRANSLATION_COMPARISON_VISITOR_H
