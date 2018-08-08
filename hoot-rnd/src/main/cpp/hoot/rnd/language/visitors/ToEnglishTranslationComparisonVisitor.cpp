
#include "ToEnglishTranslationComparisonVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/algorithms/string/MostEnglishName.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ConstElementVisitor, ToEnglishTranslationComparisonVisitor)

ToEnglishTranslationComparisonVisitor::ToEnglishTranslationComparisonVisitor() :
_skipWordsInEnglishDict(true),
_numTranslations(0)
{
}

ToEnglishTranslationComparisonVisitor::~ToEnglishTranslationComparisonVisitor()
{
  LOG_INFO("Total number of to English tag value translations made: " << _numTranslations);
}

void ToEnglishTranslationComparisonVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  _translationScorer.reset(
    Factory::getInstance().constructObject<StringDistance>(
      opts.getLanguageTranslationComparisonScorer()));

  _preTranslatedTagKeys = opts.getLanguageTranslationComparisonPretranslatedTagKeys();
  _toTranslateTagKeys = opts.getLanguageTranslationToTranslateTagKeys();
  if (_preTranslatedTagKeys.size() != _toTranslateTagKeys.size())
  {
    throw HootException(
      QString("When preforming language translation comparison, the number of pre-translated ") +
      QString("tag keys must match that of the keys of the tags to be translated."));
  }

  _skipWordsInEnglishDict = opts.getLanguageTranslationSkipWordsInEnglishDictionary();

  _translator.reset(new JoshuaTranslator(this));
  connect(_translator.get(), SIGNAL(translationComplete()), this,
          SLOT(_translationComplete()));
  _translator->setConfiguration(conf);
  _translator->setSourceLanguage(opts.getLanguageTranslationSourceLanguage());
}

void ToEnglishTranslationComparisonVisitor::visit(const boost::shared_ptr<Element>& e)
{
  for (int i = 0; i < _preTranslatedTagKeys.size(); i++)
  {
    _translate(e, _preTranslatedTagKeys.at(i), _toTranslateTagKeys.at(i));
  }
}

void ToEnglishTranslationComparisonVisitor::_translate(const ElementPtr& e,
                                                       const QString preTranslatedTagKey,
                                                       const QString toTranslateTagKey)
{
  //only care about features that have both the pre-translated tag and the tag we want to compare
  //our translation to it with
  const Tags& tags = e->getTags();
  if ((tags.contains(toTranslateTagKey) && tags.contains(preTranslatedTagKey)))
  {
    _element = e;
    _toTranslateTagKey = toTranslateTagKey;
    _toTranslateVal = tags.get(toTranslateTagKey).trimmed();
    _toTranslateVal = _toTranslateVal.replace("\n", "");
    LOG_VARD(_toTranslateVal);
    _preTranslatedVal = tags.get(preTranslatedTagKey).trimmed();
    _preTranslatedVal = _preTranslatedVal.replace("\n", "");
    LOG_VARD(_preTranslatedVal);

    if (_skipWordsInEnglishDict &&
        MostEnglishName::getInstance()->scoreName(_toTranslateVal) == 1.0)
    {
      LOG_DEBUG(
        "Tag value to be translated determined to already be in English: " << _toTranslateVal);
      return;
    }

    _translator->translate(_toTranslateVal);
  }
}

void ToEnglishTranslationComparisonVisitor::_translationComplete()
{
  const QString translatedVal = _translator->getTranslatedText();
  LOG_VARD(translatedVal);
  _numTranslations++;
  Tags& tags = _element->getTags();
  const int strComparison = translatedVal.compare(_toTranslateVal, Qt::CaseInsensitive);
  LOG_VARD(strComparison);
  tags.appendValue("hoot:translated:" + _toTranslateTagKey + ":en", translatedVal);
  const double similarityScore = _translationScorer->compare(_preTranslatedVal, translatedVal);
  LOG_VARD(similarityScore);
  tags.appendValue(
    "hoot:translated:similarity:score:" + _toTranslateTagKey + ":en",
    QString::number(similarityScore));
}

}
