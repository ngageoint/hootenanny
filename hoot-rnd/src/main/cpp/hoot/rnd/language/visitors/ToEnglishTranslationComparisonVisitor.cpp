
#include "ToEnglishTranslationComparisonVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QLocale>

namespace hoot
{

HOOT_FACTORY_REGISTER(ConstElementVisitor, ToEnglishTranslationComparisonVisitor)

ToEnglishTranslationComparisonVisitor::ToEnglishTranslationComparisonVisitor()
{
}

ToEnglishTranslationComparisonVisitor::~ToEnglishTranslationComparisonVisitor()
{
}

void ToEnglishTranslationComparisonVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  _translationScorer.reset(
    Factory::getInstance().constructObject<StringDistance>(
      opts.getLanguageTranslationComparisonScorer()));

  _preTranslatedTagKeys = opts.getLanguageTranslationComparisonPretranslatedTagKeys();
  _toTranslateTagKeys = opts.getLanguageTranslationComparisonToTranslateTagKeys();
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
                                                       const QString preTranslatedNameKey,
                                                       const QString toTranslateNameKey)
{
  //only care about features that have both the pre-translated tag and the tag we want to compare
  //our translation to it with
  const Tags& tags = e->getTags();
  if ((tags.contains(toTranslateNameKey) && tags.contains(preTranslatedNameKey)))
  {
    _element = e;
    _toTranslateNameKey = toTranslateNameKey;
    _toTranslateVal = tags.get(toTranslateNameKey).trimmed();
    _toTranslateVal = _toTranslateVal.replace("\n", "");
    LOG_VARD(_toTranslateVal);
    _preTranslatedVal = tags.get(preTranslatedNameKey).trimmed();
    _preTranslatedVal = _preTranslatedVal.replace("\n", "");
    LOG_VARD(_preTranslatedVal);
    //TODO: add english common word lookup
    //If the tag to be translated already matches the previously translated tag, there's no point
    //in translating it.
    //if (!toTranslateValue.isEmpty() && name != preTranslatedVal)
    //{
      _translationClient->translate(_toTranslateVal);
    //}
  }
}

void ToEnglishTranslationComparisonVisitor::_translationComplete()
{
  LOG_DEBUG("translationComplete");
  const QString translatedVal = _translationClient->getTranslatedText();
  LOG_VARD(translatedVal);
  //not sure if locale is needed for the comparison yet
  //QLocale::setDefault(QLocale(QLocale::German, QLocale::Germany));
  //QLocale::setDefault(QLocale(QLocale::German));
  //const int nameComparison = translatedName.localeAwareCompare(name);
  const int strComparison = translatedVal.compare(_toTranslateVal, Qt::CaseInsensitive);
  LOG_VARD(strComparison);
  //If the translator merely returned the same string we passed in, then no point in recording
  //it.
  //if (strComparison != 0)
  //{
    _element->getTags().appendValue("hoot:translated:" + _toTranslateNameKey + ":en", translatedVal);
    const double similarityScore = _translationScorer->compare(_preTranslatedVal, translatedVal);
    LOG_VARD(similarityScore);
    _element->getTags().appendValue(
      "hoot:translated:similarity:score:" + _toTranslateNameKey + ":en",
      QString::number(similarityScore));
  //}
//}
}

}
