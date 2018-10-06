/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef HOOT_SERVICES_TRANSLATOR_CLIENT_H
#define HOOT_SERVICES_TRANSLATOR_CLIENT_H

// hoot
#include <hoot/core/language/ToEnglishTranslator.h>
#include <hoot/rnd/language/TranslationInfoProvider.h>

// Qt
#include <QCache>

namespace hoot
{

class HootNetworkCookieJar;

struct TranslationResult
{
  QString translatedText;
  QString detectedLang;
};

/**
 * Translates text from a single source language to English using translation/detection
 * technologies integrated into the Hootenanny web services.
 */
class HootServicesTranslatorClient : public ToEnglishTranslator
{

public:

  static std::string className() { return "hoot::HootServicesTranslatorClient"; }

  HootServicesTranslatorClient();
  virtual ~HootServicesTranslatorClient();

  virtual QStringList getSourceLanguages() const { return _sourceLangs; }
  virtual void setSourceLanguages(const QStringList langCodes);

  /**
   * Translates text given a source language
   *
   * @param textToTranslate the text to translate
   * @return translated text or an empty string if a translation could not be made
   */
  virtual QString translate(const QString textToTranslate);

  virtual QString getDetectedLanguage() const { return _detectedLang; }

  QString getTranslatedText() const { return _translatedText; }

  virtual void setConfiguration(const Settings& conf);

  virtual void setId(const QString id) { _id = id; }

protected:

  //the source languages to tell the translator which languages to translate to English from; at
  //least one must be populated, or use "detect" to force the service to try to detect the language
  //(can be expensive)
  QStringList _sourceLangs;

  QString _translatedText;
  QString _detectedLang;
  QString _detectorUsed;
  bool _detectedLangAvailableForTranslation;

  boost::shared_ptr<TranslationInfoProvider> _infoClient;

private:

  friend class HootServicesTranslatorClientTest;

  //translator implementation to use server side
  QString _translator;
  //detector implementations to use server side; if left empty, the server will try to use as many
  //of them as necessary to detect the language
  QStringList _detectors;
  QString _translationUrl;
  //if true and a detected lang doesn't match a specified lang, then the detected lang takes
  //priority
  bool _detectedLangOverrides;
  //if true and no lang can be detected, an exhaustive search for the correct translation is made
  //against all available translatable languages (expensive)
  bool _performExhaustiveSearch;

  boost::shared_ptr<QCache<QString, TranslationResult>> _translateCache;

  int _statusUpdateInterval;

  long _numTranslationsMade;
  long _numTranslationsAttempted;
  long _untranslatableWords;
  long _numDetectionsMade;
  long _numEnglishWordsSkipped;
  long _translationCacheHits;
  long _translationCacheMaxSize;
  long _translationCacheSize;

  bool _skipWordsInEnglishDict;

  static bool _loggedCacheMaxReached;

  QString _id;

  boost::shared_ptr<HootNetworkCookieJar> _cookies;

  /**
   * Verifies that every language specified for this translator is supported by the server
   *
   * @param type type of check; either "detectable" or "translatable"; a failure to verify a
   * detectable language results in a warning, while a failure to verify a translatable language
   * results in an error
   */
  void _checkLangsAvailable(const QString type);

  void _validateAvailableLangs(boost::shared_ptr<boost::property_tree::ptree> replyObj,
                               const QString type);

  QString _getTranslateRequestData(const QString text);
  void _parseTranslateResponse(boost::shared_ptr<boost::property_tree::ptree> replyObj);

  bool _getTranslationFromCache(const QString textToTranslate);
  void _insertTranslationIntoCache(const QString textToTranslate, const QString translatedText,
                                   const QString detectedLang);

  bool _textIsTranslatable(const QString textToTranslate) const;
  bool _textIsEnglish(const QString textToTranslate) const;
};

}

#endif // HOOT_SERVICES_TRANSLATOR_CLIENT_H
