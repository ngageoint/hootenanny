
#include "ToEnglishTranslationComparisonVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/algorithms/string/MostEnglishName.h>

// Qt
//#include <QLocale>

namespace hoot
{

HOOT_FACTORY_REGISTER(ConstElementVisitor, ToEnglishTranslationComparisonVisitor)

ToEnglishTranslationComparisonVisitor::ToEnglishTranslationComparisonVisitor()
{
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

  _translationClient.reset(
    new JoshuaTranslator(
      opts.getLanguageTranslationServiceHost(), opts.getLanguageTranslationServicePort(), this));
  connect(_translationClient.get(), SIGNAL(translationComplete()), this,
          SLOT(_translationComplete()));
  _translationClient->setSourceLanguage(opts.getLanguageTranslationSourceLanguage());
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

    if (MostEnglishName::getInstance()->scoreName(_toTranslateVal) == 1.0)
    {
      LOG_DEBUG("Tag value to be translated determined to already be in English.");
      return;
    }

    _translationClient->translate(_toTranslateVal);
  }
}

void ToEnglishTranslationComparisonVisitor::_translationComplete()
{
  const QString translatedVal = _translationClient->getTranslatedText();
  LOG_VARD(translatedVal);
  Tags& tags = _element->getTags();
  //not sure if locale is needed for the comparison yet
  //QLocale::setDefault(QLocale(QLocale::German, QLocale::Germany));
  //QLocale::setDefault(QLocale(QLocale::German));
  //const int nameComparison = translatedName.localeAwareCompare(name);
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
