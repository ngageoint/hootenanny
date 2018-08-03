
#ifndef TO_ENGLISH_TRANSLATION_COMPARISON_VISITOR_H
#define TO_ENGLISH_TRANSLATION_COMPARISON_VISITOR_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/algorithms/LevenshteinDistance.h>

#include "ElementOsmMapVisitor.h"

// Qt
#include <QTcpSocket>

namespace hoot
{

/**
 * This is used to check hoot to-English name translations against existing English name
 * translations; e.g. compare hoot's translation to name:en
 *
 * //TODO: move this to separate Joshua translator later
 * @article{post2015joshua,
    Author = {Post, Matt and Cao, Yuan and Kumar, Gaurav},
    Journal = {The Prague Bulletin of Mathematical Linguistics},
    Title = {Joshua 6: A phrase-based and hierarchical statistical machine translation system},
    Year = {2015}
  }
 */
class ToEnglishTranslationComparisonVisitor : public ElementOsmMapVisitor, public Configurable
{

public:

  static std::string className() { return "hoot::ToEnglishTranslationComparisonVisitor"; }

  ToEnglishTranslationComparisonVisitor();
  ~ToEnglishTranslationComparisonVisitor();

  virtual void visit(const boost::shared_ptr<Element>& e);

  virtual void setConfiguration(const Settings& conf);

  virtual QString getDescription() const
  { return "TODO"; }

private:

  LevenshteinDistance _levDist;
  boost::shared_ptr<QTcpSocket> _translationClient;

  void _translateName(Tags& tags, const QString preTranslatedNameKey,
                      const QString toTranslateNameKey);
};

}

#endif // TO_ENGLISH_TRANSLATION_COMPARISON_VISITOR_H
