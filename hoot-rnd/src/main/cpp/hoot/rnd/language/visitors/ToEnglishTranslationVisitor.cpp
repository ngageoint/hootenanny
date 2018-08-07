
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
_skipPreTranslatedTags(true)
{
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
    LOG_VARD(_toTranslateVal);

    if (MostEnglishName::getInstance()->scoreName(_toTranslateVal) == 1.0)
    {
      LOG_DEBUG("Tag value to be translated determined to already be in English.");
      return;
    }

    const QString preTranslatedTagKey = _toTranslateTagKey + ":en";
    if (!_skipPreTranslatedTags || !tags.contains(preTranslatedTagKey))
    {
      _translationClient->translate(_toTranslateVal);
    }
    else
    {
      LOG_DEBUG("Skipping pre-translated tag: " << preTranslatedTagKey << "=" << _toTranslateVal);
    }
  }
}

void ToEnglishTranslationVisitor::_translationComplete()
{
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
  if (strComparison != 0)
  {
    _element->getTags().appendValue("hoot:translated:" + _toTranslateTagKey + ":en", translatedVal);
  }
  else
  {
    LOG_DEBUG("Translator returned translation with same value as text passed in.");
  }
}

}
