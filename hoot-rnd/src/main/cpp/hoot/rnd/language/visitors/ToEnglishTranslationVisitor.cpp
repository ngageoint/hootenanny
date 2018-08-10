
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
_numProcessedElements(0)
{
}

ToEnglishTranslationVisitor::~ToEnglishTranslationVisitor()
{
//  long numDetections = 0;
//  for (QList<boost::shared_ptr<LanguageDetector>>::const_iterator itr = _langDetectors.begin();
//       itr != _langDetectors.end(); ++itr)
//  {
//    boost::shared_ptr<LanguageDetector> langDetector = *itr;
//    numDetections += langDetector->getDetectionsMade();
//  }
//  LOG_INFO("Number of language detections made: " << _langDetector);
//  LOG_INFO("Total number of to English tag value translations made: " << _numTranslationsMade);
//  LOG_INFO("Total number of elements processed: " << _numProcessedElements);
//  LOG_INFO("Total number of elements encountered: " << _numTotalElements);
}

void ToEnglishTranslationVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  _translator.reset(
    Factory::getInstance().constructObject<ToEnglishTranslator>(
      opts.getLanguageTranslationTranslator()));
  boost::shared_ptr<QObject> qObj = boost::dynamic_pointer_cast<Configurable>(_translator);
  if (qObj.get())
  {
    qObj->setParent(this);
  }
  _translator->setConfiguration(conf);
  _translator->setSourceLanguages(opts.getLanguageTranslationSourceLanguages());

  _toTranslateTagKeys = opts.getLanguageTranslationToTranslateTagKeys();
  _skipPreTranslatedTags = opts.getLanguageTranslationSkipPreTranslatedTags();
  _skipWordsInEnglishDict = opts.getLanguageTranslationSkipWordsInEnglishDictionary();
  _detectedLangOverrides =
    opts.getLanguageTranslationDetectedLanguageOverridesSpecifiedSourceLanguages();
  _performExhaustiveSearch = opts.getLanguageTranslationPerformExhaustiveSearchWithNoDetection();
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
  //TODO: Should this be moved to JoshuaTranslator?
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

//  const QStringList specifiedSourceLangs = _translator->getSourceLanguages();
//  assert(specifiedSourceLangs.size() > 0);
//  LOG_VART(specifiedSourceLangs.size());
//  QString sourceLang;
//  if (specifiedSourceLangs.contains("detect", Qt::CaseInsensitive) ||
//      specifiedSourceLangs.size() > 1)
//  {
//    for (QList<boost::shared_ptr<LanguageDetector>>::const_iterator itr = _langDetectors.begin();
//         itr != _langDetectors.end(); ++itr)
//    {
//      boost::shared_ptr<LanguageDetector> langDetector = *itr;
//      sourceLang =
//        _translator->getSupportedLanguages()->getIso6391Code(langDetector->detect(_toTranslateVal));
//      if (!sourceLang.isEmpty())
//      {
//        break;
//      }
//    }

//    if (sourceLang.isEmpty())
//    {
//      if (_performExhaustiveSearch)
//      {
//        LOG_DEBUG(
//          "Unable to detect language.  Performing translation against each specified " <<
//          "language until a translation is found...");
//        for (int i = 0; i < specifiedSourceLangs.size(); i++)
//        {
//          _translator->translate(specifiedSourceLangs.at(i), _toTranslateVal);
//        }
//      }
//      else
//      {
//        LOG_DEBUG("Unable to detect language.  Skipping translation; " << _toTranslateVal);
//        return;
//      }
//    }
//    else if (!_detectedLangOverrides && specifiedSourceLangs.size() > 1 &&
//             !specifiedSourceLangs.contains(sourceLang, Qt::CaseInsensitive))
//    {
//      QString msg =
//        "Detected language: " + _translator->getSupportedLanguages()->getCountryName(sourceLang) +
//        " not in specified source languages: " + specifiedSourceLangs.join(";") + ".  ";
//      if (_performExhaustiveSearch)
//      {
//        msg +=
//          "Performing translation against each specified language until a translation is found...";
//        LOG_DEBUG(msg);
//        for (int i = 0; i < specifiedSourceLangs.size(); i++)
//        {
//          _translator->translate(specifiedSourceLangs.at(i), _toTranslateVal);
//        }
//      }
//      else
//      {
//        LOG_DEBUG(
//          "Detected language: " <<
//          _translator->getSupportedLanguages()->getCountryName(sourceLang) <<
//          " not in specified source languages: " << specifiedSourceLangs.join(";") <<
//          ".  Skipping translation; text: " << _toTranslateVal);
//        return;
//      }
//    }
//    else
//    {
//      if (!specifiedSourceLangs.contains(sourceLang, Qt::CaseInsensitive))
//      {
//        assert(_detectedLangOverrides);
//        LOG_DEBUG(
//          "Detected language: " <<
//          _translator->getSupportedLanguages()->getCountryName(sourceLang) <<
//          " overrides specified language(s) for text: " <<
//          _toTranslateVal)
//      }
//      else
//      {
//        LOG_DEBUG(
//          "Detected language: " <<
//          _translator->getSupportedLanguages()->getCountryName(sourceLang) << " for text: " <<
//          _toTranslateVal);
//      }

//      _translator->translate(sourceLang, _toTranslateVal);
//    }
//  }
//  else
//  {
//    sourceLang = specifiedSourceLangs.at(0);
//    LOG_DEBUG(
//      "Using specified language: " <<
//      _translator->getSupportedLanguages()->getCountryName(sourceLang));
//    _translator->translate(sourceLang, _toTranslateVal);
//  }
  _translator->translate(sourceLang, _toTranslateVal);

  _numProcessedElements++;
  if (_numProcessedElements % 10 == 0)
  {
    LOG_VARD(_numProcessedElements);
  }
}

void ToEnglishTranslationVisitor::translationComplete()
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
