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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef HOOT_SERVICES_TRANSLATOR_CLIENT_H
#define HOOT_SERVICES_TRANSLATOR_CLIENT_H

// hoot
#include <hoot/core/language/ToEnglishTranslator.h>
#include <hoot/rnd/language/LanguageInfoProvider.h>

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

  static QString className() { return "hoot::HootServicesTranslatorClient"; }

  HootServicesTranslatorClient();
  virtual ~HootServicesTranslatorClient();

  QStringList getSourceLanguages() const override { return _sourceLangs; }
  void setSourceLanguages(const QStringList& langCodes) override;

  /**
   * Translates text given a source language
   *
   * @param text the text to translate
   * @return translated text or an empty string if a translation could not be made
   */
  QString translate(const QString& text) override;

  QString getDetectedLanguage() const override { return _detectedLang; }

  QString getTranslatedText() const { return _translatedText; }

  void setConfiguration(const Settings& conf) override;

  void setId(const QString& id) override { _id = id; }

protected:

  //the source languages to tell the translator which languages to translate to English from; at
  //least one must be populated, or use "detect" to force the service to try to detect the language
  //(can be expensive)
  QStringList _sourceLangs;

  QString _translatedText;
  QString _detectedLang;
  QString _detectorUsed;
  bool _detectedLangAvailableForTranslation;

  //determines whether cookies are used in web requests; should be set to true to support auth,
  //except when running tests as they don't make web requests that require auth
  bool _useCookies;

  long _numTranslationsMade;
  long _numTranslationsAttempted;
  long _numEnglishTextsSkipped;

  //uses the local English dict to prevent unnecessary translation server calls
  bool _skipWordsInEnglishDict;

  //used to retrieve info about what languages are translatable, etc.
  std::shared_ptr<LanguageInfoProvider> _infoClient;

private:

  friend class HootServicesTranslatorClientTest;

  //translator implementation to use server side
  QString _translator;
  //detector implementations to use server side; if left empty, the server will try to use as many
  //of them as necessary to detect the language
  QStringList _detectors;
  //if true and a detected lang doesn't match a specified lang, then the detected lang takes
  //priority
  bool _detectedLangOverrides;
  //if true and no lang can be detected, an exhaustive search for the correct translation is made
  //against all available translatable languages (expensive)
  bool _performExhaustiveSearch;

  std::shared_ptr<QCache<QString, TranslationResult>> _cache;

  int _statusUpdateInterval;

  //number of words that were deemed untranslatable (contained numbers, etc.)
  long _untranslatableWords;
  long _numDetectionsMade;

  long _cacheHits;
  long _cacheMaxSize;
  long _cacheSize;

  //if true, we've hit our cache limit and it will start flushing older entries
  static bool _loggedCacheMaxReached;

  //useful when trying to figure out who invoked this translator throughout the lifecycle of the app
  QString _id;

  //persistent session is required by hoot services
  std::shared_ptr<HootNetworkCookieJar> _cookies;

  static QString _getTranslateUrl();

  /**
   * Verifies that every language specified for this translator is supported by the server
   *
   * @param type type of check; either "detectable" or "translatable"; a failure to verify a
   * detectable language results in a warning, while a failure to verify a translatable language
   * results in an error
   */
  void _checkLangsAvailable(const QString& type);

  void _validateAvailableLangs(const std::shared_ptr<boost::property_tree::ptree>& replyObj,
                               const QString& type);

  QString _getRequestData(const QString& text);
  void _parseResponse(const std::shared_ptr<boost::property_tree::ptree>& replyObj);

  bool _getTranslationFromCache(const QString& text);
  void _insertTranslationIntoCache(const QString& text, const QString& translatedText,
                                   const QString& detectedLang);

  bool _textIsTranslatable(const QString& text) const;
  bool _textIsEnglish(const QString& text) const;
  /** Timeout for HTTP requests in seconds */
  int _timeout;
};

}

#endif // HOOT_SERVICES_TRANSLATOR_CLIENT_H
