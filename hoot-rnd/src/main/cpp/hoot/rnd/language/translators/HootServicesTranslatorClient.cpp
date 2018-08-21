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
  _detectableUrl = opts.getLanguageTranslationHootServicesDetectableLanguagesEndpoint();
  _translatableUrl = opts.getLanguageTranslationHootServicesTranslatableLanguagesEndpoint();

  connect(this, SIGNAL(translationComplete()), parent(), SLOT(translationComplete()));
  connect(
    this, SIGNAL(translationError(QString, QString)), parent(),
    SLOT(translationError(QString, QString)));
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

  _checkLangsAvailable("detect");
  _checkLangsAvailable("translate");
}

void HootServicesTranslatorClient::_checkLangsAvailable(const QString type)
{
  LOG_DEBUG("Checking languages available for: " << type << "...");

  QString urlStr;
  QStringList apps;
  if (type == "translate")
  {
    urlStr = _translatableUrl;
    apps = QStringList(_translator);
  }
  else
  {
    urlStr = _detectableUrl;
    apps = _detectors;
  }

  QUrl url(urlStr);
  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");

  boost::property_tree::ptree requestObj;
  boost::property_tree::ptree appsObj;
  for (int i = 0; i < apps.size(); i++)
  {
    boost::property_tree::ptree appObj;
    appObj.put("", apps.at(i).toStdString());
    appsObj.push_back(std::make_pair("", appObj));
  }
  requestObj.add_child("apps", appsObj);
  std::stringstream requestStrStrm;
  boost::property_tree::json_parser::write_json(requestStrStrm, requestObj);
  LOG_VART(requestStrStrm.str());

  QNetworkReply* reply =
    _client->post(request, QString::fromStdString(requestStrStrm.str()).toUtf8());
  QEventLoop loop;
  QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  loop.exec();

  QVariant status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
  LOG_VARD(status.isValid());
  LOG_VARD(status.toInt());
  if (reply->error() != QNetworkReply::NoError)
  {
    emit translationError("Supported languages request error", reply->errorString());
    return;
  }

  QString replyData(reply->readAll());
  LOG_VART(replyData);
  std::stringstream replyStrStrm(replyData.toUtf8().constData(), std::ios::in);
  if (!replyStrStrm.good())
  {
    throw HootException(QString("Error reading from reply string:\n%1").arg(replyData));
  }
  boost::property_tree::ptree replyObj;
  try
  {
    boost::property_tree::read_json(replyStrStrm, replyObj);
  }
  catch (boost::property_tree::json_parser::json_parser_error& e)
  {
    QString reason = QString::fromStdString(e.message());
    QString line = QString::number(e.line());
    throw HootException(QString("Error parsing JSON: %1 (line %2)").arg(reason).arg(line));
  }
  catch (const std::exception& e)
  {
    QString reason = e.what();
    throw HootException("Error parsing JSON " + reason);
  }
  QMap<QString, bool> returnedLangs;
  BOOST_FOREACH (boost::property_tree::ptree::value_type& language, replyObj.get_child("languages"))
  {
    const QString sourceLangCode =
      QString::fromStdString(language.second.get<std::string>("iso6391code"));
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
      QString msg =
        "Requested source language code: " + sourceLangCode + " not available for apps: " +
        apps.join(",");
      if (type == "translate")
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

void HootServicesTranslatorClient::translate(const QString textToTranslate)
{
  if (_sourceLangs.size() == 0)
  {
    throw HootException("Cannot determine source language.");
  }

  LOG_DEBUG(
    "Translating to English with specified source languages: " <<
     _sourceLangs.join(",") << "; text: " << textToTranslate);

  QUrl url(_translationUrl);
  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  boost::property_tree::ptree requestObj;
  requestObj.put("translator", _translator.toStdString());
  QByteArray textData = QUrl::toPercentEncoding(textToTranslate);
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
  std::stringstream ss;
  boost::property_tree::json_parser::write_json(ss, requestObj);
  LOG_VART(ss.str());

  QNetworkReply* reply = _client->post(request, QString::fromStdString(ss.str()).toUtf8());
  QEventLoop loop;
  QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  loop.exec();

  QVariant status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
  LOG_VARD(status.isValid());
  LOG_VARD(status.toInt());
  if (reply->error() != QNetworkReply::NoError)
  {
    emit translationError(textToTranslate, reply->errorString());
    return;
  }

  QString replyData(reply->readAll());
  LOG_VART(replyData);
  std::stringstream replyStrStrm(replyData.toUtf8().constData(), std::ios::in);
  if (!replyStrStrm.good())
  {
    throw HootException(QString("Error reading reply string:\n%1").arg(replyData));
  }
  boost::property_tree::ptree replyObj;
  try
  {
    boost::property_tree::read_json(replyStrStrm, replyObj);
  }
  catch (boost::property_tree::json_parser::json_parser_error& e)
  {
    QString reason = QString::fromStdString(e.message());
    QString line = QString::number(e.line());
    throw HootException(QString("Error parsing JSON: %1 (line %2)").arg(reason).arg(line));
  }
  catch (const std::exception& e)
  {
    QString reason = e.what();
    throw HootException("Error parsing JSON " + reason);
  }
  _translatedText =
    QUrl::fromPercentEncoding(
      QString::fromStdString(replyObj.get<std::string>("translatedText")).toUtf8());
  LOG_VARD(_translatedText);
  const QString detectedLang =
    QString::fromStdString(replyObj.get<std::string>("detectedLang"));
  if (!detectedLang.isEmpty())
  {
    _detectionMade = true;
    LOG_VARD(detectedLang);
    const QString detectorUsed =
      QString::fromStdString(replyObj.get<std::string>("detectorUsed"));
    LOG_VARD(detectorUsed);
    const bool detectedLangAvailableForTranslation =
      replyObj.get<bool>("detectedLangAvailableForTranslation");
    LOG_VARD(detectedLangAvailableForTranslation);
  }

  emit translationComplete();
}

}
