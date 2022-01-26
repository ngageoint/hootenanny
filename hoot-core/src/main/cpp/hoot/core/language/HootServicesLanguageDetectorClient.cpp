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

#include "HootServicesLanguageDetectorClient.h"

// hoot
#include <hoot/core/auth/HootServicesLoginManager.h>
#include <hoot/core/language/LanguageUtils.h>
#include <hoot/core/io/HootNetworkRequest.h>
#include <hoot/core/io/NetworkIoUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/HootNetworkUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QByteArray>
#include <QVariant>

// std
#include <sstream>

namespace hoot
{

HOOT_FACTORY_REGISTER(LanguageDetector, HootServicesLanguageDetectorClient)

bool HootServicesLanguageDetectorClient::_loggedCacheMaxReached = false;

HootServicesLanguageDetectorClient::HootServicesLanguageDetectorClient()
  : _useCookies(true),
    _numDetectionsAttempted(0),
    _numDetectionsMade(0),
    _numEnglishTextsSkipped(0),
    _skipWordsInEnglishDict(true),
    _statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
    _undetectableWords(0),
    _minConfidence(LanguageDetectionConfidenceLevel::Level::None),
    _cacheHits(0),
    _cacheSize(0),
    _cacheMaxSize(100),
    _timeout(500)
{
}

HootServicesLanguageDetectorClient::~HootServicesLanguageDetectorClient()
{
  LOG_INFO("Minimum confidence threshold: " << _minConfidence.toString());
  LOG_INFO(
    "Made " << _numDetectionsMade << " successful language detections on " <<
    _numDetectionsAttempted << " attempts.");
  LOG_INFO(_undetectableWords << " words were not detectable.");
  LOG_INFO(_numEnglishTextsSkipped << " English words were skipped.");
  LOG_INFO("Language detections made: " << _numDetectionsMade);
  if (_cache)
  {
    LOG_DEBUG("Language detection cache hits: " << _cacheHits);
    LOG_DEBUG("Language detection cache size: " << _cacheSize);
    LOG_DEBUG("Language detection cache max possible size: " << _cacheMaxSize);
  }
  if (!_langCodesWithNoLangNamesAvailable.empty())
  {
    LOG_INFO(_getUnvailableLangNamesStr());
  }
  else
  {
    LOG_INFO("No language codes returned without language names.");
  }
  if (!_confidenceCounts.empty())
  {
    LOG_INFO(_getCountsStr("Detection confidence counts", _confidenceCounts));
  }
  LOG_INFO(_getCountsStr("Detectors used", _detectorUsedCounts));
}

QString HootServicesLanguageDetectorClient::_getDetectUrl()
{
  return HootServicesLoginManager::getBaseUrl() + "/language/toEnglishTranslation/detect";
}

void HootServicesLanguageDetectorClient::setConfiguration(const Settings& conf)
{
  LOG_DEBUG("Setting configuration...");

  ConfigOptions opts(conf);

  _detectors = opts.getLanguageHootServicesDetectors();
  _skipWordsInEnglishDict = opts.getLanguageSkipWordsInEnglishDictionary();
  const QString minConfidenceStr = opts.getLanguageHootServicesDetectionMinConfidenceThreshold();
  if (!minConfidenceStr.trimmed().isEmpty())
    _minConfidence = LanguageDetectionConfidenceLevel::fromString(minConfidenceStr);

  _cacheMaxSize = opts.getLanguageMaxCacheSize();
  if (_cacheMaxSize != -1)
    _cache = std::make_shared<QCache<QString, DetectionResult>>(_cacheMaxSize);

  if (_useCookies)
  {
    // get a session cookie associated with the user information passed into the command calling
    // this class
    _cookies =
      NetworkIoUtils::getUserSessionCookie(
        opts.getHootServicesAuthUserName(), opts.getHootServicesAuthAccessToken(),
        opts.getHootServicesAuthAccessTokenSecret(), _getDetectUrl());
  }
}

QString HootServicesLanguageDetectorClient::_getUnvailableLangNamesStr() const
{
  QString str = "Language codes for which no name was available:\n";

  for (auto itr = _langCodesWithNoLangNamesAvailable.begin(); itr != _langCodesWithNoLangNamesAvailable.end(); ++itr)
  {
    const QString detector = itr.key();
    str += detector + ":\n";
    const QSet<QString> unvailableLangNames = itr.value();
    for (auto itr2 = unvailableLangNames.begin(); itr2 != unvailableLangNames.end(); ++itr2)
    {
      const QString langCode = *itr2;
      str += langCode + "\n";
    }
  }
  str.chop(1);

  return str;
}

QString HootServicesLanguageDetectorClient::_getCountsStr(const QString& title,
                                                          const QMap<QString, int>& data) const
{
  QString str = title + ":\n";

  for (auto itr = data.begin(); itr != data.end(); ++itr)
  {
    const QString confidence = itr.key();
    const int count = itr.value();
    str += confidence + ": " + QString::number(count) + "\n";
  }
  str.chop(1);

 return str;
}

QString HootServicesLanguageDetectorClient::detect(const QString& text)
{
  LOG_TRACE("Detecting source language for: " << text);

  QString detectedLanguage;
  if (_cache)
  {
    detectedLanguage = _getLangFromCache(text);
    if (!detectedLanguage.trimmed().isEmpty())
      return detectedLanguage;
  }

  if (!_textIsDetectable(text))
  {
    _undetectableWords++;
    LOG_TRACE(
      "Text for language being detected is not detectable; text: " << text);
    return "";
  }

  // see related note in HootServicesTranslatorClient::translate
  bool englishTextSkipped = false;
  if (_skipWordsInEnglishDict && LanguageUtils::normalizeAndDetermineIfTextIsEnglish(text))
  {
    LOG_TRACE(
      "Text for language being detected determined to already be in English.  Skipping " <<
      "language detection for text: " << text);
    _numEnglishTextsSkipped++;
    return "";
  }

  QUrl url(_getDetectUrl());
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
      url, _timeout, headers, QNetworkAccessManager::Operation::PostOperation,
      _getRequestData(text).toUtf8());
  }
  catch (const HootException& e)
  {
    throw HootException("Error detecting language for text: " + text + ". error: " + e.what());
  }
  if (request.getHttpStatus() != HttpResponseCode::HTTP_OK)
  {
    throw HootException(
      "Error detecting language for text: " + text + ".  HTTP status: " +
      QString::number(request.getHttpStatus()) + "; error: " + request.getErrorString());
  }

  QString detectorUsed;
  QString detectedLangCode =
    _parseResponse(StringUtils::jsonStringToPropTree(request.getResponseContent()), detectorUsed);
  LOG_VART(detectorUsed);
  assert(!detectorUsed.trimmed().isEmpty());
  if (_detectorUsedCounts.contains(detectorUsed))
    _detectorUsedCounts[detectorUsed] = _detectorUsedCounts[detectorUsed] + 1;
  else
    _detectorUsedCounts[detectorUsed] = 1;

  // update the cache
  if (!detectedLangCode.isEmpty() && _cache && !_cache->contains(text))
    _insertLangIntoCache(text, detectedLangCode);

  if (detectedLangCode == "en")
  {
    LOG_TRACE("Source language for text: " << text << " detected as English.");
    _numEnglishTextsSkipped++;
    englishTextSkipped = true;
    detectedLangCode = "";
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
  else if (!englishTextSkipped)
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

QString HootServicesLanguageDetectorClient::_getRequestData(const QString& text) const
{
  boost::property_tree::ptree requestObj;
  QByteArray textData = QUrl::toPercentEncoding(text);
  requestObj.put("text", QString(textData).toStdString());
  requestObj.add_child("detectors", *StringUtils::stringListToJsonArray(_detectors));

  std::stringstream requestStrStrm;
  boost::property_tree::json_parser::write_json(requestStrStrm, requestObj);
  return QString::fromStdString(requestStrStrm.str());
}

QString HootServicesLanguageDetectorClient::_parseResponse(const std::shared_ptr<boost::property_tree::ptree>& replyObj,
                                                           QString& detectorUsed)
{
  QString detectedLangCode =
    QString::fromStdString(replyObj->get<std::string>("detectedLangCode")).trimmed();
  if (!detectedLangCode.isEmpty())
  {
    detectorUsed = QString::fromStdString(replyObj->get<std::string>("detectorUsed"));
    const QString langName = QString::fromStdString(replyObj->get<std::string>("detectedLang"));
    LOG_VART(langName);
    if (langName.toLower() == "unvailable")
      _langCodesWithNoLangNamesAvailable[detectorUsed].insert(detectedLangCode);

    const QString detectionConfidenceStr =
      QString::fromStdString(replyObj->get<std::string>("detectionConfidence"));
    LOG_VART(detectionConfidenceStr);
    if (!detectionConfidenceStr.isEmpty() && detectionConfidenceStr != "none available" &&
        detectionConfidenceStr != "none")
    {
      const LanguageDetectionConfidenceLevel detectionConfidence =
        LanguageDetectionConfidenceLevel::fromString(detectionConfidenceStr);
      if (detectionConfidence.getEnum() < _minConfidence.getEnum())
      {
        LOG_TRACE(
          "Detected language with confidence threshold: " << detectionConfidenceStr << " did " <<
          "not meet the minimum threshold of " << _minConfidence.toString() << ".");
        detectedLangCode = "";
      }
      if (_confidenceCounts.contains(detectionConfidenceStr))
        _confidenceCounts[detectionConfidenceStr] = _confidenceCounts[detectionConfidenceStr] + 1;
      else
        _confidenceCounts[detectionConfidenceStr] = 1;
    }
  }
  return detectedLangCode;
}

QString HootServicesLanguageDetectorClient::_getLangFromCache(const QString& text)
{
  QString detectedLangCode = "";
  const DetectionResult* cachedDetection = _cache->object(text.toLower());
  if (cachedDetection != nullptr)
  {
    detectedLangCode = cachedDetection->detectedLangCode;
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

void HootServicesLanguageDetectorClient::_insertLangIntoCache(const QString& text,
                                                              const QString& detectedLangCode)
{
  // The cache takes ownership of this object.
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

bool HootServicesLanguageDetectorClient::_textIsDetectable(const QString& text) const
{
  return !StringUtils::isNumber(text) && StringUtils::hasAlphabeticCharacter(text);
}

}
