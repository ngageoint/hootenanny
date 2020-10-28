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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "HootServicesTranslatorClient.h"

// hoot
#include <hoot/core/auth/HootServicesLoginManager.h>
#include <hoot/core/io/HootNetworkRequest.h>
#include <hoot/core/io/NetworkIoUtils.h>
#include <hoot/core/language/LanguageUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/HootNetworkUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QByteArray>
#include <QVariant>
#include <QMap>

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

// std
#include <iostream>
#include <sstream>

namespace hoot
{

HOOT_FACTORY_REGISTER(ToEnglishTranslator, HootServicesTranslatorClient)

bool HootServicesTranslatorClient::_loggedCacheMaxReached = false;

HootServicesTranslatorClient::HootServicesTranslatorClient() :
_detectedLang(""),
_detectedLangAvailableForTranslation(false),
_useCookies(true),
_numTranslationsMade(0),
_numTranslationsAttempted(0),
_numEnglishTextsSkipped(0),
_skipWordsInEnglishDict(true),
_detectedLangOverrides(false),
_performExhaustiveSearch(false),
_statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
_untranslatableWords(0),
_numDetectionsMade(0),
_cacheHits(0),
_cacheMaxSize(-1),
_cacheSize(0)
{
}

HootServicesTranslatorClient::~HootServicesTranslatorClient()
{
  LOG_INFO("ID: " << _id);
  LOG_INFO(
    "Made " << _numTranslationsMade << " successful translations on " <<
    _numTranslationsAttempted << " attempts.");
  LOG_INFO(_untranslatableWords << " words were not translatable.");
  LOG_INFO(_numEnglishTextsSkipped << " English words were skipped.");
  LOG_INFO("Language detections made: " << _numDetectionsMade);
  if (_cache)
  {
    LOG_DEBUG("Translation cache hits: " << _cacheHits);
    LOG_DEBUG("Translation cache size: " << _cacheSize);
    LOG_DEBUG("Translation cache max possible size: " << _cacheMaxSize);
  }
}

QString HootServicesTranslatorClient::_getTranslateUrl()
{
  return HootServicesLoginManager::getBaseUrl() + "/language/toEnglishTranslation/translate";
}

void HootServicesTranslatorClient::setConfiguration(const Settings& conf)
{
  LOG_DEBUG("Setting configuration...");

  ConfigOptions opts(conf);

  _detectedLangOverrides =
    opts.getLanguageTranslationDetectedLanguageOverridesSpecifiedSourceLanguages();
  _performExhaustiveSearch = opts.getLanguageTranslationPerformExhaustiveSearchWithNoDetection();
  _translator = opts.getLanguageTranslationHootServicesTranslator();
  _detectors = opts.getLanguageHootServicesDetectors();
  _skipWordsInEnglishDict = opts.getLanguageSkipWordsInEnglishDictionary();

  _infoClient.reset(
    Factory::getInstance().constructObject<LanguageInfoProvider>(
      opts.getLanguageInfoProvider()));
  _infoClient->setConfiguration(conf);

  _cacheMaxSize = opts.getLanguageMaxCacheSize();
  if (_cacheMaxSize != -1)
  {
    _cache.reset(new QCache<QString, TranslationResult>(_cacheMaxSize));
  }

  if (_useCookies)
  {
    // get a session cookie associated with the user information passed into the command calling
    // this class
    _cookies =
      NetworkIoUtils::getUserSessionCookie(
        opts.getHootServicesAuthUserName(), opts.getHootServicesAuthAccessToken(),
        opts.getHootServicesAuthAccessTokenSecret(), _getTranslateUrl());
  }
}

void HootServicesTranslatorClient::setSourceLanguages(const QStringList& langCodes)
{    
  LOG_DEBUG("Setting source languages...");

  _sourceLangs = langCodes;
  if (_sourceLangs.contains("detect", Qt::CaseInsensitive))
  {
    if (_sourceLangs.size() != 1)
    {
      throw HootException(
        "When specifying 'detect' in source languages, no other languages may be specified.");
    }
    else
    {
      return;
    }
  }

  _checkLangsAvailable("detectable");
  _checkLangsAvailable("translatable");
}

void HootServicesTranslatorClient::_checkLangsAvailable(const QString& type)
{
  //request the supported langs info from the service and check the supported langs against our
  //specified source langs
  _validateAvailableLangs(_infoClient->getAvailableLanguages(type), type);
}

void HootServicesTranslatorClient::_validateAvailableLangs(
  const std::shared_ptr<boost::property_tree::ptree>& replyObj, const QString& type)
{
  QMap<QString, bool> returnedLangs;
  for (boost::property_tree::ptree::value_type& language : replyObj->get_child("languages"))
  {
    const QString sourceLangCode =
      QString::fromStdString(language.second.get<std::string>("iso6391Code"));
    LOG_VART(sourceLangCode);
    const bool available = language.second.get<bool>("available");
    LOG_VART(available);
    returnedLangs[sourceLangCode] = available;
  }
  bool langNotFound = false;
  for (int i = 0; i < _sourceLangs.size(); i++)
  {
    QString sourceLangCode = _sourceLangs.at(i);
    LOG_VART(sourceLangCode);
    if (!returnedLangs[sourceLangCode])
    {
      QString msg = "Requested source language code: " + sourceLangCode + " not available.";
      if (type == "translatable")
      {
        throw HootException(msg);
      }
      else
      {
        msg += "; Skipping detection for language: " + _sourceLangs.at(i);
        LOG_WARN(msg);
        langNotFound = true;
      }
    }
  }

  if (!langNotFound)
  {
    LOG_DEBUG("All languages available for operation: " << type);
  }
}

QString HootServicesTranslatorClient::_getRequestData(const QString& text)
{
  boost::property_tree::ptree requestObj;
  requestObj.put("translator", _translator.toStdString());
  QByteArray textData = QUrl::toPercentEncoding(text);
  requestObj.put("text", QString(textData).toStdString());
  const QString detectedLangOverridesStr = _detectedLangOverrides ? "true" : "false";
  requestObj.put(
    "detectedLanguageOverridesSpecifiedSourceLanguages", detectedLangOverridesStr.toStdString());
  const QString performExhaustiveSearchStr = _performExhaustiveSearch ? "true" : "false";
  requestObj.put("performExhaustiveTranslationSearchWithNoDetection",
    performExhaustiveSearchStr.toStdString());
  requestObj.add_child("detectors", *StringUtils::stringListToJsonStringArray(_detectors));
  requestObj.add_child("sourceLangCodes", *StringUtils::stringListToJsonStringArray(_sourceLangs));

  std::stringstream requestStrStrm;
  boost::property_tree::json_parser::write_json(requestStrStrm, requestObj);
  return QString::fromStdString(requestStrStrm.str());
}

void HootServicesTranslatorClient::_parseResponse(const std::shared_ptr<boost::property_tree::ptree>& replyObj)
{
  _translatedText =
    QUrl::fromPercentEncoding(
      QString::fromStdString(replyObj->get<std::string>("translatedText")).toUtf8());
  LOG_VART(_translatedText);
  //detected lang won't be populated if no detection was performed
  if (replyObj->count("detectedLang") > 0)
  {
    _numDetectionsMade++;
    _detectedLang =
      QUrl::fromPercentEncoding(
        QString::fromStdString(replyObj->get<std::string>("detectedLang")).toUtf8());
    LOG_VART(_detectedLang);
    //detector used should always be here if a detection was made
    _detectorUsed = QString::fromStdString(replyObj->get<std::string>("detectorUsed"));
    LOG_VART(_detectorUsed);
    _detectedLangAvailableForTranslation =
      replyObj->get<bool>("detectedLangAvailableForTranslation");
    LOG_VART(_detectedLangAvailableForTranslation);
  }
}

bool HootServicesTranslatorClient::_getTranslationFromCache(const QString& text)
{
  TranslationResult* cachedTranslation = _cache->object(text.toLower());
  if (cachedTranslation != 0)
  {
    _translatedText = cachedTranslation->translatedText;
    _detectedLang = cachedTranslation->detectedLang;
    LOG_TRACE(
      "Found cached translation: " << _translatedText << " for: " << text);
    _cacheHits++;

    _numTranslationsMade++;
    if (_numTranslationsMade % _statusUpdateInterval == 0)
    {
      PROGRESS_DEBUG("Made " << _numTranslationsMade << " translations.");
    }
    _numTranslationsAttempted++;
    if (_numTranslationsAttempted % _statusUpdateInterval == 0)
    {
      PROGRESS_DEBUG("Attempted " << _numTranslationsAttempted << " translations.");
    }

    return true;
  }
  return false;
}

bool HootServicesTranslatorClient::_textIsTranslatable(const QString& text) const
{
  return !StringUtils::isNumber(text) && StringUtils::hasAlphabeticCharacter(text);
}

void HootServicesTranslatorClient::_insertTranslationIntoCache(const QString& text,
                                                               const QString& translatedText,
                                                               const QString& detectedLang)
{
  TranslationResult* translationResult = new TranslationResult();
  translationResult->detectedLang = detectedLang;
  translationResult->translatedText = translatedText;
  LOG_TRACE(
    "Inserting: " << translationResult->translatedText << " for: " << text.toLower() <<
    " into translation cache...");
  _cache->insert(text.toLower(), translationResult);
  _cacheSize = _cache->size();

  if (!_loggedCacheMaxReached && _cache->size() >= _cacheMaxSize)
  {
    LOG_DEBUG("Translation cache max size of: " << _cacheMaxSize << " has been reached.");
  }
}

QString HootServicesTranslatorClient::translate(const QString& text)
{
  if (_sourceLangs.size() == 0)
  {
    throw HootException("Cannot determine source language.");
  }

  LOG_TRACE(
    "Translating to English with specified source languages: " <<
     _sourceLangs.join(",") << "; text: " << text << "...");

  _translatedText = "";
  _detectedLang = "";
  _detectorUsed = "";

  if (_cache && _getTranslationFromCache(text))
  {
    return _translatedText;
  }

  if (!_textIsTranslatable(text))
  {
    LOG_TRACE("Text to be translated is not translatable; text: " << text);
    _translatedText = "";
    _untranslatableWords++;
    return "";
  }

  //This is an attempt to cut back on translation service requests for text that may already
  //in English.  Leaving it optional, b/c MostEnglishName's ability to determine if a word is
  //English is still a little suspect at this point.
  if (_skipWordsInEnglishDict && LanguageUtils::normalizeAndDetermineIfTextIsEnglish(text))
  {
    LOG_TRACE(
      "Text to be translated determined to already be in English.  Skipping " <<
      "translation for text: " << text);
    _numEnglishTextsSkipped++;
    _translatedText = "";
    return "";
  }

  QUrl url(_getTranslateUrl());
  QMap<QNetworkRequest::KnownHeaders, QVariant> headers;
  headers[QNetworkRequest::ContentTypeHeader] = HootNetworkUtils::CONTENT_TYPE_JSON;
  HootNetworkRequest request;
  if (_useCookies)
  {
    //Hoot OAuth requires that session state be maintained for the authenticated user
    request.setCookies(_cookies);
  }
  try
  {
    request.networkRequest(
      url, headers, QNetworkAccessManager::Operation::PostOperation,
      _getRequestData(text).toUtf8());
  }
  catch (const HootException& e)
  {
    throw HootException("Error translating text: " + text + ". error: " + e.what());
  }
  if (request.getHttpStatus() != HttpResponseCode::HTTP_OK)
  {
    throw HootException("Error translating text: " + text + ". error: " + request.getErrorString());
  }

  _parseResponse(StringUtils::jsonStringToPropTree(request.getResponseContent()));

  if (text.toLower() == _translatedText.toLower())
  {
    _translatedText = "";
  }

  //update the cache
  if (_cache && !_cache->contains(text))
  {
    _insertTranslationIntoCache(text, _translatedText, _detectedLang);
  }

  if (!_translatedText.isEmpty())
  {
    LOG_TRACE("Translated: " << text << " to English as: " << _translatedText);
    _numTranslationsMade++;
    if (_numTranslationsMade % _statusUpdateInterval == 0)
    {
      PROGRESS_DEBUG("Made " << _numTranslationsMade << " translations.");
    }
  }
  else
  {
    LOG_TRACE("Text: " << text << " could not be translated.");
  }

  _numTranslationsAttempted++;
  if (_numTranslationsAttempted % _statusUpdateInterval == 0)
  {
    PROGRESS_DEBUG("Attempted " << _numTranslationsAttempted << " translations.");
  }

  return _translatedText;
}

}
