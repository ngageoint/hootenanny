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

#include "HootServicesTranslatorClient.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QByteArray>
#include <QEventLoop>
#include <QVariant>
#include <QMap>
#include <QNetworkRequest>

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

// std
#include <iostream>
#include <sstream>

namespace hoot
{

HOOT_FACTORY_REGISTER(ToEnglishTranslator, HootServicesTranslatorClient)

HootServicesTranslatorClient::HootServicesTranslatorClient() :
_detectedLangAvailableForTranslation(false),
_detectedLangOverrides(false),
_performExhaustiveSearch(false)
{
  _client.reset(new QNetworkAccessManager(this));
}

void HootServicesTranslatorClient::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  _detectedLangOverrides =
    opts.getLanguageTranslationDetectedLanguageOverridesSpecifiedSourceLanguages();
  _performExhaustiveSearch = opts.getLanguageTranslationPerformExhaustiveSearchWithNoDetection();
  _translator = opts.getLanguageTranslationHootServicesTranslator();
  _detectors = opts.getLanguageTranslationHootServicesDetectors();
  _translationUrl = opts.getLanguageTranslationHootServicesTranslationEndpoint();

  //allowing null here is primarily for testing purposes; a null parent will cause runtime errors
  //outside of tests anyway
  if (parent() != NULL)
  {
    connect(this, SIGNAL(translationComplete()), parent(), SLOT(translationComplete()));
    connect(
      this, SIGNAL(translationError(QString, QString)), parent(),
      SLOT(translationError(QString, QString)));
  }
  else
  {
    LOG_DEBUG("HootServicesTranslatorClient parent null.")
  }

  _infoClient.reset(
    Factory::getInstance().constructObject<TranslationInfoProvider>(
      opts.getLanguageTranslationInfoProvider()));
  _infoClient->setConfiguration(conf);
}

void HootServicesTranslatorClient::setSourceLanguages(const QStringList langCodes)
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

void HootServicesTranslatorClient::_checkLangsAvailable(const QString type)
{
  //request the supported langs info from the service
  boost::shared_ptr<boost::property_tree::ptree> replyObj;
  try
  {
    replyObj = _infoClient->getAvailableLanguages(type);
  }
  catch (const HootException& e)
  {
    emit translationError("Supported languages request error", e.getWhat());
    return;
  }

  //check the supported langs against our specified langs
  _validateAvailableLangs(replyObj, type);
}

void HootServicesTranslatorClient::_validateAvailableLangs(
  boost::shared_ptr<boost::property_tree::ptree> replyObj, const QString type)
{
  QMap<QString, bool> returnedLangs;
  BOOST_FOREACH (boost::property_tree::ptree::value_type& language, replyObj->get_child("languages"))
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
    LOG_DEBUG("All languages available for " << type);
  }
}

boost::shared_ptr<QNetworkRequest> HootServicesTranslatorClient::_getTranslateRequest(
  const QString text, std::stringstream& requestStrStrm)
{
  QUrl url(_translationUrl);
  boost::shared_ptr<QNetworkRequest> request(new QNetworkRequest(url));
  request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
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
  boost::property_tree::ptree detectors;
  for (int i = 0; i < _detectors.size(); i++)
  {
    boost::property_tree::ptree detector;
    detector.put("", _detectors.at(i).toStdString());
    detectors.push_back(std::make_pair("", detector));
  }
  requestObj.add_child("detectors", detectors);
  boost::property_tree::ptree sourceLangCodes;
  for (int i = 0; i < _sourceLangs.size(); i++)
  {
    boost::property_tree::ptree sourceLangCode;
    sourceLangCode.put("", _sourceLangs.at(i).toStdString());
    sourceLangCodes.push_back(std::make_pair("", sourceLangCode));
  }
  requestObj.add_child("sourceLangCodes", sourceLangCodes);
  boost::property_tree::json_parser::write_json(requestStrStrm, requestObj);
  LOG_VART(requestStrStrm.str());

  return request;
}

void HootServicesTranslatorClient::_parseTranslateResponse(
  boost::shared_ptr<boost::property_tree::ptree> replyObj)
{
  _translatedText =
    QUrl::fromPercentEncoding(
      QString::fromStdString(replyObj->get<std::string>("translatedText")).toUtf8());
  LOG_VARD(_translatedText);
  _detectedLang = QString::fromStdString(replyObj->get<std::string>("detectedLang"));
  if (!_detectedLang.isEmpty())
  {
    _detectionMade = true;
    LOG_VARD(_detectedLang);
    _detectorUsed = QString::fromStdString(replyObj->get<std::string>("detectorUsed"));
    LOG_VARD(_detectorUsed);
    _detectedLangAvailableForTranslation =
      replyObj->get<bool>("detectedLangAvailableForTranslation");
    LOG_VARD(_detectedLangAvailableForTranslation);
  }

  emit translationComplete();
}

void HootServicesTranslatorClient::translate(const QString textToTranslate)
{
  if (_sourceLangs.size() == 0)
  {
    throw HootException("Cannot determine source language.");
  }

  LOG_DEBUG(
    "Translating to English with specified source languages: " <<
     _sourceLangs.join(",") << "; text: " << textToTranslate);

  //create the request
  std::stringstream requestStrStrm;
  boost::shared_ptr<QNetworkRequest> request =
    _getTranslateRequest(textToTranslate, requestStrStrm);

  //send the request and wait for the response
  QNetworkReply* reply =
    _client->post(*request, QString::fromStdString(requestStrStrm.str()).toUtf8());
  QEventLoop loop;
  QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  loop.exec();

  //check for a response error
  if (reply->error() != QNetworkReply::NoError)
  {
    emit translationError(textToTranslate, reply->errorString());
    return;
  }

  //get and parse the response data
  _parseTranslateResponse(StringUtils::jsonStringToPropTree(reply->readAll()));
}

}
