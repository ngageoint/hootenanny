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

#include "HootServicesLanguageDetectorClient.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/io/HootNetworkRequest.h>
#include <hoot/core/util/NetworkUtils.h>
#include "LanguageUtils.h"

// Qt
#include <QByteArray>
#include <QVariant>
#include <QMap>

// std
#include <iostream>
#include <sstream>

namespace hoot
{

HOOT_FACTORY_REGISTER(LanguageDetector, HootServicesLanguageDetectorClient)

bool HootServicesLanguageDetectorClient::_loggedCacheMaxReached = false;

HootServicesLanguageDetectorClient::HootServicesLanguageDetectorClient() :
_useCookies(true),
_statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
_numDetectionsMade(0),
_numEnglishWordsSkipped(0),
_skipWordsInEnglishDict(true)
{
}

HootServicesLanguageDetectorClient::~HootServicesLanguageDetectorClient()
{
  //TODO: finish
  LOG_INFO(
    "Made " << _numDetectionsMade << " successful language detections on " <<
    _numDetectionsAttempted << " attempts.");
  LOG_INFO(_undetectableWords << " words were not detectable.");
  LOG_INFO(_numEnglishWordsSkipped << " English words were skipped.");
  LOG_INFO("Language detections made: " << _numDetectionsMade);
  if (_cache)
  {
    LOG_DEBUG("Language detection cache hits: " << _cacheHits);
    LOG_DEBUG("Language detection cache size: " << _cacheSize);
    LOG_DEBUG("Language detection cache max possible size: " << _cacheMaxSize);
  }
}

void HootServicesLanguageDetectorClient::setConfiguration(const Settings& conf)
{
  LOG_DEBUG("Setting configuration...");

  ConfigOptions opts(conf);

  _detectors = opts.getLanguageHootServicesDetectors();
  _url = opts.getLanguageHootServicesDetectionEndpoint();
  _skipWordsInEnglishDict = opts.getLanguageSkipWordsInEnglishDictionary();

  _cacheMaxSize = opts.getLanguageMaxCacheSize();
  if (_cacheMaxSize != -1)
  {
    _cache.reset(new QCache<QString, DetectionResult>(_cacheMaxSize));
  }

  if (_useCookies)
  {
    // get a session cookie associated with the user information passed into the command calling
    // this class
    _cookies =
      NetworkUtils::getUserSessionCookie(
        opts.getHootServicesAuthUserName(), opts.getHootServicesAuthAccessToken(),
        opts.getHootServicesAuthAccessTokenSecret(), _url);
  }
}

QString HootServicesLanguageDetectorClient::detect(const QString text)
{
  LOG_TRACE("Detecting source language for: " << text);

  QString detectedLanguage;
  if (_cache)
  {
    detectedLanguage = _getLangFromCache(text);
    if (!detectedLanguage.trimmed().isEmpty())
    {
      return detectedLanguage;
    }
  }

  if (!_textIsDetectable(text))
  {
    LOG_TRACE(
      "Text for language being detected is not detectable; text: " << text);
    return "";
  }

  // see related note in HootServicesTranslatorClient::translate
  if (_skipWordsInEnglishDict && LanguageUtils::normalizeAndDetermineIfTextIsEnglish(text))
  {
    LOG_TRACE(
      "Text for language being detected determined to already be in English.  Skipping " <<
      "language detection for text: " << text);
    _numEnglishWordsSkipped++;
    return "";
  }

  //create and execute the request
  QUrl url(_url);
  QMap<QNetworkRequest::KnownHeaders, QVariant> headers;
  headers[QNetworkRequest::ContentTypeHeader] = "application/json";
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
    throw HootException("Error detecting language for text: " + text + ". error: " + e.what());
  }

  //check for a response error
  if (request.getHttpStatus() != 200)
  {
    throw HootException(
      "Error detecting language for text: " + text + ". error: " + request.getErrorString());
  }

  //parse the response data
  QString detectedLangCode =
    _parseResponse(StringUtils::jsonStringToPropTree(request.getResponseContent()));

  // update the cache
  if (!detectedLangCode.isEmpty() && _cache && !_cache->contains(text))
  {
    _insertLangIntoCache(text, detectedLangCode);
  }

  if (!detectedLangCode.isEmpty())
  {
    LOG_TRACE("Detected language: " << detectedLangCode << " for text: " << text);
    _numDetectionsMade++;
    if (_numDetectionsMade % _statusUpdateInterval == 0)
    {
      PROGRESS_DEBUG("Made " << _numDetectionsMade << " language detections.");
    }
  }
  else
  {
    LOG_TRACE("Source language for text: " << text << " could not be detected.");
  }

  _numDetectionsAttempted++;
  if (_numDetectionsAttempted % _statusUpdateInterval == 0)
  {
    PROGRESS_DEBUG("Attempted " << _numDetectionsAttempted << " language detections.");
  }

  return detectedLangCode;
}

QString HootServicesLanguageDetectorClient::_getRequestData(const QString text) const
{
  boost::property_tree::ptree requestObj;
  QByteArray textData = QUrl::toPercentEncoding(text);
  requestObj.put("text", QString(textData).toStdString());
  requestObj.add_child("detectors", *StringUtils::stringListToJsonStringArray(_detectors));

  std::stringstream requestStrStrm;
  boost::property_tree::json_parser::write_json(requestStrStrm, requestObj);
  return QString::fromStdString(requestStrStrm.str());
}

QString HootServicesLanguageDetectorClient::_parseResponse(
  boost::shared_ptr<boost::property_tree::ptree> replyObj) const
{
  return QString::fromStdString(replyObj->get<std::string>("detectedLangCode"));
}

QString HootServicesLanguageDetectorClient::_getLangFromCache(const QString text)
{
  QString detectedLangCode = "";
  DetectionResult* cachedDetection = _cache->object(text.toLower());
  if (cachedDetection != 0)
  {
    const QString detectedLangCode = cachedDetection->detectedLangCode;
    LOG_TRACE("Found cached detection: " << detectedLangCode << " for: " << text);
    _cacheHits++;

    _numDetectionsMade++;
    if (_numDetectionsMade % _statusUpdateInterval == 0)
    {
      PROGRESS_DEBUG("Made " << _numDetectionsMade << " detections.");
    }
    _numDetectionsAttempted++;
    if (_numDetectionsAttempted % _statusUpdateInterval == 0)
    {
      PROGRESS_DEBUG("Attempted " << _numDetectionsAttempted << " detections.");
    }
  }
  return detectedLangCode;
}

void HootServicesLanguageDetectorClient::_insertLangIntoCache(const QString text,
                                                              const QString detectedLangCode)
{
  DetectionResult* detectionResult = new DetectionResult();
  detectionResult->detectedLangCode = detectedLangCode;
  detectionResult->sourceText = text;
  LOG_TRACE(
    "Inserting: " << detectionResult->detectedLangCode << " for: " << text.toLower() <<
    " into detection cache...");
  _cache->insert(text.toLower(), detectionResult);
  _cacheSize = _cache->size();

  if (!_loggedCacheMaxReached && _cache->size() >= _cacheMaxSize)
  {
    LOG_DEBUG("Detection cache max size of: " << _cacheMaxSize << " has been reached.");
  }
}

bool HootServicesLanguageDetectorClient::_textIsDetectable(const QString text) const
{
  return !StringUtils::isNumber(text) && StringUtils::hasAlphabeticCharacter(text);
}

}
