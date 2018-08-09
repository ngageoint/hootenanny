
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
_detectedLangOverrides(false),
_performExhaustiveSearch(false),
_numTranslationsMade(0),
_numTotalElements(0),
_numProcessedElements(0)
{
}

ToEnglishTranslationVisitor::~ToEnglishTranslationVisitor()
{
  LOG_INFO("Total number of to English tag value translations made: " << _numTranslationsMade);
  LOG_INFO("Total number of elements processed: " << _numProcessedElements);
  LOG_INFO("Total number of elements encountered: " << _numTotalElements);
}

void ToEnglishTranslationVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  _supportedLangs.reset(
    new SupportedTranslationLanguages(opts.getLanguageTranslationSupportedLanguagesFile()));
  const QStringList sourceLanguages = opts.getLanguageTranslationSourceLanguages();
  for (int i = 0; i < sourceLanguages.size(); i++)
  {
    const QString langCode = sourceLanguages.at(i);
    if (!_supportedLangs->isSupportedLanguage(langCode))
    {
      throw HootException("Specified unsupported language: " + langCode);
    }
  }

  _translator.reset(new JoshuaTranslator(this));
  connect(_translator.get(), SIGNAL(translationComplete()), this, SLOT(_translationComplete()));
  _translator->setConfiguration(conf);
  _translator->setSourceLanguages(sourceLanguages);

  _toTranslateTagKeys = opts.getLanguageTranslationToTranslateTagKeys();
  _skipPreTranslatedTags = opts.getLanguageTranslationSkipPreTranslatedTags();
  _skipWordsInEnglishDict = opts.getLanguageTranslationSkipWordsInEnglishDictionary();
  _detectedLangOverrides =
    opts.getLanguageTranslationDetectedLanguageOverridesSpecifiedSourceLanguages();
  LOG_VARD(_detectedLangOverrides);

  LOG_VART(opts.getLanguageTranslationLanguageDetector());
  _langDetector.reset(
    Factory::getInstance().constructObject<LanguageDetector>(
      opts.getLanguageTranslationLanguageDetector()));
  _langDetector->setConfiguration(conf);
  _performExhaustiveSearch = opts.getLanguageTranslationPerformExhaustiveSearchWithNoDetection();
}

void ToEnglishTranslationVisitor::visit(const boost::shared_ptr<Element>& e)
{
  LOG_VART(e);
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

    const QStringList specifiedSourceLangs = _translator->getSourceLanguages();
    assert(specifiedSourceLangs.size() > 0);
    LOG_VART(specifiedSourceLangs.size());
    QString sourceLang;
    if (specifiedSourceLangs.contains("detect", Qt::CaseInsensitive) ||
        specifiedSourceLangs.size() > 1)
    {
      sourceLang = _supportedLangs->getIso6391Code(_langDetector->detect(_toTranslateVal));
      if (sourceLang.isEmpty() || !specifiedSourceLangs.contains(sourceLang, Qt::CaseInsensitive))
      {
        if (_performExhaustiveSearch)
        {
          LOG_DEBUG(
            "Unable to detect language.  Performing translation against each specified " <<
            "language until a translation is found...");
          for (int i = 0; i < specifiedSourceLangs.size(); i++)
          {
            _translator->translate(specifiedSourceLangs.at(i), _toTranslateVal);
          }
        }
        else
        {
          LOG_DEBUG("Unable to detect language.  Skipping translation; " << _toTranslateVal);
          return;
        }
      }
      else if (!_detectedLangOverrides && specifiedSourceLangs.size() > 1 &&
               !specifiedSourceLangs.contains(sourceLang, Qt::CaseInsensitive))
      {
        LOG_DEBUG(
          "Detected language: " << sourceLang << " not in specified source languages: " <<
          specifiedSourceLangs.join(";") << ".  Skipping translation; text: " << _toTranslateVal);
        return;
      }
      else
      {
        if (!specifiedSourceLangs.contains(sourceLang, Qt::CaseInsensitive))
        {
          assert(_detectedLangOverrides);
          LOG_DEBUG(
            "Detected language: " << sourceLang << " overrides specified language(s) for text: " <<
            _toTranslateVal)
        }
        else
        {
          LOG_DEBUG("Detected language: " << sourceLang << " for text: " << _toTranslateVal);
        }

        _translator->translate(sourceLang, _toTranslateVal);
      }
    }
    else
    {
      sourceLang = specifiedSourceLangs.at(0);
      LOG_VARD(sourceLang);
      _translator->translate(sourceLang, _toTranslateVal);
    }

    _numProcessedElements++;
    if (_numProcessedElements % 10 == 0)
    {
      LOG_VARD(_numProcessedElements);
    }
  }
  _numTotalElements++;
  if (_numTotalElements % 10 == 0)
  {
    LOG_VARD(_numTotalElements);
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
