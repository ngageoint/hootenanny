
#include "ToEnglishTranslationVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/algorithms/string/MostEnglishName.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ConstElementVisitor, ToEnglishTranslationVisitor)

ToEnglishTranslationVisitor::ToEnglishTranslationVisitor() :
_skipPreTranslatedTags(false),
_skipWordsInEnglishDict(true),
_numTranslations(0)
{
}

ToEnglishTranslationVisitor::~ToEnglishTranslationVisitor()
{
  LOG_INFO("Total number of to English tag value translations made: " << _numTranslations);
}

void ToEnglishTranslationVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  _translator.reset(new JoshuaTranslator(this));
  connect(_translator.get(), SIGNAL(translationComplete()), this, SLOT(_translationComplete()));
  _translator->setConfiguration(conf);
  _translator->setSourceLanguage(opts.getLanguageTranslationSourceLanguage());

  _toTranslateTagKeys = opts.getLanguageTranslationToTranslateTagKeys();
  _skipPreTranslatedTags = opts.getLanguageTranslationSkipPreTranslatedTags();
  _skipWordsInEnglishDict = opts.getLanguageTranslationSkipWordsInEnglishDictionary();

  LOG_VART(opts.getLanguageTranslationLanguageDetector());
  _langDetector.reset(
    Factory::getInstance().constructObject<LanguageDetector>(
      opts.getLanguageTranslationLanguageDetector()));
  _langDetector->setConfiguration(conf);
}

void ToEnglishTranslationVisitor::visit(const boost::shared_ptr<Element>& e)
{
  for (int i = 0; i < _toTranslateTagKeys.size(); i++)
  {
    _translate(e, _toTranslateTagKeys.at(i));
  }
}

void ToEnglishTranslationVisitor::_translate(const ElementPtr& e,
                                             const QString toTranslateTagKey)
{
  const Tags& tags = e->getTags();
  if ((tags.contains(toTranslateTagKey)))
  {
    _element = e;
    _toTranslateTagKey = toTranslateTagKey;
    _toTranslateVal = tags.get(toTranslateTagKey).trimmed();
    //TODO: Should this be moved to JoshuaTranslator?
    _toTranslateVal = _toTranslateVal.replace("\n", "");
    LOG_VART(_toTranslateVal);

//    const QString detectedLang = _langDetector->detect(_toTranslateVal);
//    if (detectedLang.isEmpty())
//    {
//      LOG_DEBUG("Unable to detect language for text: " << _toTranslateVal);
//    }
//    else
//    {
//      LOG_DEBUG("Detected language: " << detectedLang << " for text: " << _toTranslateVal);
//    }

    //TODO: should this be replaced with lang detect, or should the two be combined for this
    //purpose?
    if (_skipWordsInEnglishDict)
    {
      const double englishNameScore = MostEnglishName::getInstance()->scoreName(_toTranslateVal);
      LOG_TRACE("English name score: " << englishNameScore << " for: " << _toTranslateVal);
      if (englishNameScore == 1.0)
      {
        LOG_DEBUG(
          "Tag value to be translated determined to already be in English: " << _toTranslateVal);
        return;
      }
    }

    const QString preTranslatedTagKey = _toTranslateTagKey + ":en";
    if (!_skipPreTranslatedTags || !tags.contains(preTranslatedTagKey))
    {
      _translator->translate(_toTranslateVal);
    }
    else
    {
      LOG_DEBUG(
        "Skipping element with pre-translated tag: " << preTranslatedTagKey << "=" <<
        _toTranslateVal);
    }
  }
}

void ToEnglishTranslationVisitor::_translationComplete()
{
  const QString translatedVal = _translator->getTranslatedText();
  LOG_VART(translatedVal);
  const int strComparison = translatedVal.compare(_toTranslateVal, Qt::CaseInsensitive);
  LOG_VART(strComparison);
  //If the translator merely returned the same string we passed in, then no point in using it.
  if (strComparison != 0)
  {
    LOG_DEBUG("Translated: " << _toTranslateVal << " to: " << translatedVal);
    _element->getTags().appendValue("hoot:translated:" + _toTranslateTagKey + ":en", translatedVal);
    _numTranslations++;
  }
  else
  {
    LOG_DEBUG(
      "Translator returned translation with same value as text passed in: " << translatedVal);
  }
}

}
