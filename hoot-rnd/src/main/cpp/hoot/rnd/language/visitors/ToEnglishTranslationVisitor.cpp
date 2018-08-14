
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
_detectedLangOverrides(false),
_performExhaustiveSearch(false),
_numTotalElements(0),
_skipWordsInEnglishDict(true),
_numTranslationsMade(0),
_numProcessedElements(0),
_numDetectionsMade(0)
{
}

ToEnglishTranslationVisitor::~ToEnglishTranslationVisitor()
{
  LOG_INFO("Language detections made: " << _numDetectionsMade);
  LOG_INFO("To English tag value translations made: " << _numTranslationsMade);
  LOG_INFO("Elements processed: " << _numProcessedElements);
  LOG_INFO("Elements encountered: " << _numTotalElements);
}

void ToEnglishTranslationVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  _translatorClient.reset(
    Factory::getInstance().constructObject<ToEnglishTranslator>(
      opts.getLanguageTranslationTranslatorClient()));
  boost::shared_ptr<QObject> qObj = boost::dynamic_pointer_cast<QObject>(_translatorClient);
  if (qObj.get())
  {
    qObj->setParent(this);
  }
  _translatorClient->setConfiguration(conf);
  _translatorClient->setSourceLanguages(opts.getLanguageTranslationSourceLanguages());

  _toTranslateTagKeys = opts.getLanguageTranslationToTranslateTagKeys();
  _skipPreTranslatedTags = opts.getLanguageTranslationSkipPreTranslatedTags();
  _skipWordsInEnglishDict = opts.getLanguageTranslationSkipWordsInEnglishDictionary();
}

void ToEnglishTranslationVisitor::visit(const boost::shared_ptr<Element>& e)
{
  LOG_VART(e);
  const Tags& tags = e->getTags();
  for (int i = 0; i < _toTranslateTagKeys.size(); i++)
  {
    const QString toTranslateTagKey = _toTranslateTagKeys.at(i);
    if ((tags.contains(toTranslateTagKey)))
    {
      _translate(e, toTranslateTagKey);
    }
  }

  _numTotalElements++;
  if (_numTotalElements % 10 == 0)
  {
    LOG_VARD(_numTotalElements);
  }
}

void ToEnglishTranslationVisitor::_translate(const ElementPtr& e,
                                             const QString toTranslateTagKey)
{
  const Tags& tags = e->getTags();
  _toTranslateTagKey = toTranslateTagKey;
  _element = e;
  _toTranslateVal = tags.get(toTranslateTagKey).trimmed();
  //We're just translating single phrases, not entire texts, so remove newlines.
  _toTranslateVal = _toTranslateVal.replace("\n", "");
  LOG_VART(_toTranslateVal);

  const QString preTranslatedTagKey = _toTranslateTagKey + ":en";
  if (_skipPreTranslatedTags && tags.contains(preTranslatedTagKey))
  {
    LOG_DEBUG(
      "Skipping element with pre-translated tag: " << preTranslatedTagKey << "=" <<
      _toTranslateVal);
    return;
  }

  //TODO: should this be replaced with lang detect, or should the two be combined for this
  //purpose?
  if (_skipWordsInEnglishDict)
  {
    const double englishNameScore = MostEnglishName::getInstance()->scoreName(_toTranslateVal);
    LOG_TRACE("English name score: " << englishNameScore << " for: " << _toTranslateVal);
    if (englishNameScore == 1.0)
    {
      LOG_DEBUG(
        "Tag value to be translated determined to already be in English.  Skipping " <<
        "translation; text: " << _toTranslateVal);
      return;
    }
  }

  _translatorClient->translate(_toTranslateVal);

  _numProcessedElements++;
  if (_numProcessedElements % 10 == 0)
  {
    LOG_VARD(_numProcessedElements);
  }
}

void ToEnglishTranslationVisitor::translationComplete()
{
  const QString translatedVal = _translatorClient->getTranslatedText();
  LOG_VART(translatedVal);
  if (_translatorClient->detectionMade())
  {
    _numDetectionsMade++;
  }
  const int strComparison = translatedVal.compare(_toTranslateVal, Qt::CaseInsensitive);
  LOG_VART(strComparison);
  //If the translator merely returned the same string we passed in, then no point in using it.
  if (strComparison != 0)
  {
    LOG_DEBUG("Translated: " << _toTranslateVal << " to: " << translatedVal);
    _element->getTags().appendValue("hoot:translated:" + _toTranslateTagKey + ":en", translatedVal);
    _numTranslationsMade++;
    if (_numTranslationsMade % 10 == 0)
    {
      LOG_VARD(_numTranslationsMade);
    }
  }
  else
  {
    LOG_DEBUG(
      "Translator returned translation with same value as text passed in.  Discarding " <<
      "translation; text: " << translatedVal);
  }
}

}
