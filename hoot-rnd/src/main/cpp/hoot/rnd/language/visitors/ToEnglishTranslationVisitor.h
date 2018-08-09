
#ifndef TO_ENGLISH_TRANSLATION_VISITOR_H
#define TO_ENGLISH_TRANSLATION_VISITOR_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/rnd/language/JoshuaTranslator.h>
#include <hoot/core/algorithms/string/StringTokenizer.h>
#include <hoot/rnd/language/LanguageDetector.h>
#include <hoot/rnd/language/SupportedTranslationLanguages.h>

namespace hoot
{

/**
 * Translates tag values to English
 */
class ToEnglishTranslationVisitor : public QObject, public ElementOsmMapVisitor,
   public Configurable
{

  Q_OBJECT

public:

  static std::string className() { return "hoot::ToEnglishTranslationVisitor"; }

  ToEnglishTranslationVisitor();
  ~ToEnglishTranslationVisitor();

  virtual void visit(const boost::shared_ptr<Element>& e);

  virtual void setConfiguration(const Settings& conf);

  virtual QString getDescription() const
  { return "Translates selected tag values to English"; }

private slots:

  void _translationComplete();

private:

  boost::shared_ptr<JoshuaTranslator> _translator;
  QList<boost::shared_ptr<LanguageDetector>> _langDetectors;
  StringTokenizer _strTokenizer;
  boost::shared_ptr<SupportedTranslationLanguages> _supportedLangs;

  QStringList _toTranslateTagKeys;

  QString _toTranslateVal;
  QString _toTranslateTagKey;
  ElementPtr _element;

  bool _skipPreTranslatedTags;
  bool _skipWordsInEnglishDict;
  bool _detectedLangOverrides;
  bool _performExhaustiveSearch;
  long _numTranslationsMade;
  long _numTotalElements;
  long _numProcessedElements;

  void _translate(const ElementPtr& e, const QString toTranslateTagKey);
};

}

#endif // TO_ENGLISH_TRANSLATION_VISITOR_H
