
#include "ToEnglishTranslationComparisonVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ConstElementVisitor, ToEnglishTranslationComparisonVisitor)

ToEnglishTranslationComparisonVisitor::ToEnglishTranslationComparisonVisitor() :
ToEnglishTranslationVisitor()
{
}

void ToEnglishTranslationComparisonVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  if (opts.getLanguageTranslationSourceLanguages().size() > 1 ||
      opts.getLanguageTranslationSourceLanguages().contains("detect", Qt::CaseInsensitive))
  {
    throw HootException(
      QString("ToEnglishTranslationComparisonVisitor supports only one source language ") +
      QString("and does not support detect mode."));
  }

  ToEnglishTranslationVisitor::setConfiguration(conf);

  if (_skipPreTranslatedTags ||
      opts.getLanguageTranslationDetectedLanguageOverridesSpecifiedSourceLanguages() ||
      opts.getLanguageTranslationPerformExhaustiveSearchWithNoDetection())
  {
    throw IllegalArgumentException(
      "ToEnglishTranslationComparisonVisitor does not support enabling any of the following options:\n"
      "language.translation.detected.language.overrides.specified.source.languages\n"
      "language.translation.perform.exhaustive.search.with.no.detection\n"
      "language.translation.skip.pre.translated.tags\nDisabling the options.");
  }

  _translationScorer.reset(
    Factory::getInstance().constructObject<StringDistance>(
      opts.getLanguageTranslationComparisonScorer()));

  _preTranslatedTagKeys = opts.getLanguageTranslationComparisonPretranslatedTagKeys();
  if (_preTranslatedTagKeys.size() != opts.getLanguageTranslationToTranslateTagKeys().size())
  {
    throw HootException(
      QString("When preforming language translation comparison, the number of pre-translated ") +
      QString("tag keys must match that of the keys of the tags to be translated."));
  }
}

void ToEnglishTranslationComparisonVisitor::visit(const boost::shared_ptr<Element>& e)
{
  const Tags& tags = e->getTags();
  for (int i = 0; i < _preTranslatedTagKeys.size(); i++)
  {
    const QString toTranslateTagKey = _toTranslateTagKeys.at(i);
    const QString preTranslatedTagKey = _preTranslatedTagKeys.at(i);
    //only care about features that have both the pre-translated tag and the tag we want to compare
    //our translation to it with
    if ((tags.contains(toTranslateTagKey) && tags.contains(preTranslatedTagKey)))
    {
      _preTranslatedVal = tags.get(preTranslatedTagKey).trimmed();
      //TODO: Should this be moved to JoshuaTranslator?
      _preTranslatedVal = _preTranslatedVal.replace("\n", "");
      LOG_VARD(_preTranslatedVal);

      ToEnglishTranslationVisitor::_translate(e, toTranslateTagKey);
    }
  }

  _numTotalElements++;
  if (_numTotalElements % 10 == 0)
  {
    LOG_VARD(_numTotalElements);
  }
}

void ToEnglishTranslationComparisonVisitor::translationComplete()
{
  const QString translatedVal = _translatorClient->getTranslatedText();
  LOG_VARD(translatedVal);
  Tags& tags = _element->getTags();
  tags.appendValue("hoot:translated:" + _toTranslateTagKey + ":en", translatedVal);
  const double similarityScore = _translationScorer->compare(_preTranslatedVal, translatedVal);
  LOG_VARD(similarityScore);
  tags.appendValue(
    "hoot:translated:similarity:score:" + _toTranslateTagKey + ":en",
    QString::number(similarityScore));
}

}
