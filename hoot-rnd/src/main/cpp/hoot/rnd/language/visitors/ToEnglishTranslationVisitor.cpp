
#include "ToEnglishTranslationVisitor.h"

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

  _translationClient.reset(
    new JoshuaTranslator(
      opts.getLanguageTranslationServiceHost(), opts.getLanguageTranslationServicePort(), this));
  connect(_translationClient.get(), SIGNAL(translationComplete()), this,
          SLOT(_translationComplete()));
  _translationClient->setSourceLanguage(opts.getLanguageTranslationSourceLanguage());
  _toTranslateTagKeys = opts.getLanguageTranslationToTranslateTagKeys();
  _skipPreTranslatedTags = opts.getLanguageTranslationSkipPreTranslatedTags();
  _skipWordsInEnglishDict = opts.getLanguageTranslationSkipWordsInEnglishDictionary();
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
    _toTranslateVal = _toTranslateVal.replace("\n", "");
    LOG_VART(_toTranslateVal);

    if (_skipWordsInEnglishDict)
    {
      const double englishNameScore = MostEnglishName::getInstance()->scoreName(_toTranslateVal);
      LOG_TRACE("English name score: " << englishNameScore << " for: " << _toTranslateVal);
      //const bool valInEngDict = MostEnglishName::getInstance()->isInDictionary(_toTranslateVal);
      //const bool valInEngDict =
        //MostEnglishName::getInstance()->areAllInDictionary(_strTokenizer.tokenize(_toTranslateVal));
  //    QString msg = "Tag value ";
  //    if (valInEngDict)
  //    {
  //      msg += "in ";
  //    }
  //    else
  //    {
  //      msg += "not in ";
  //    }
  //    msg += "English dictionary; val: " + _toTranslateVal;
  //    LOG_DEBUG(msg);
      if (englishNameScore == 1.0)
      //if (valInEngDict)
      {
        LOG_DEBUG(
          "Tag value to be translated determined to already be in English: " << _toTranslateVal);
        return;
      }
    }

    const QString preTranslatedTagKey = _toTranslateTagKey + ":en";
    if (!_skipPreTranslatedTags || !tags.contains(preTranslatedTagKey))
    {
      _translationClient->translate(_toTranslateVal);
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
  const QString translatedVal = _translationClient->getTranslatedText();
  LOG_VART(translatedVal);
  const int strComparison = translatedVal.compare(_toTranslateVal, Qt::CaseInsensitive);
  LOG_VART(strComparison);
  //If the translator merely returned the same string we passed in, then no point in recording
  //it.
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
