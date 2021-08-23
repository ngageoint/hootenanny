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

#include "HootServicesLanguageInfoClient.h"

// hoot
#include <hoot/rnd/auth/HootServicesLoginManager.h>
#include <hoot/core/io/HootNetworkRequest.h>
#include <hoot/rnd/io/NetworkIoUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/HootNetworkUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QVariant>

// std

#include <sstream>

namespace hoot
{

HOOT_FACTORY_REGISTER(LanguageInfoProvider, HootServicesLanguageInfoClient)

QString HootServicesLanguageInfoClient::_getBaseUrl()
{
  return HootServicesLoginManager::getBaseUrl() + "/language/toEnglishTranslation";
}

QString HootServicesLanguageInfoClient::_getDetectableUrl()
{
  return _getBaseUrl() + "/detectable";
}

QString HootServicesLanguageInfoClient::_getTranslatableUrl()
{
  return _getBaseUrl() + "/translatable";
}

QString HootServicesLanguageInfoClient::_getDetectorsUrl()
{
  return _getBaseUrl() + "/detectors";
}

QString HootServicesLanguageInfoClient::_getTranslatorsUrl()
{
  return _getBaseUrl() + "/translators";
}

void HootServicesLanguageInfoClient::setConfiguration(const Settings& conf)
{
  LOG_DEBUG("Setting configuration options...");

  ConfigOptions opts(conf);

  _translator = opts.getLanguageTranslationHootServicesTranslator();
  _detectors = opts.getLanguageHootServicesDetectors();
  if (_useCookies)
  {
    // get a session cookie associated with the user information passed into the command calling
    // this class
    _cookies =
      NetworkIoUtils::getUserSessionCookie(
        opts.getHootServicesAuthUserName(), opts.getHootServicesAuthAccessToken(),
        opts.getHootServicesAuthAccessTokenSecret(), _getDetectableUrl());
  }
}

std::shared_ptr<boost::property_tree::ptree> HootServicesLanguageInfoClient::getAvailableApps(const QString& type)
{
 LOG_DEBUG("Checking apps available for: " << type << "...");

 QString urlStr;
 if (type == "translators")
 {
   urlStr = _getTranslatorsUrl();
 }
 else
 {
   urlStr = _getDetectorsUrl();
 }
 LOG_VARD(urlStr);

 QUrl url(urlStr);
 HootNetworkRequest request;
 if (_useCookies)
 {
   //Hoot OAuth requires that session state be maintained for the authenticated user
   request.setCookies(_cookies);
 }
 try
 {
   request.networkRequest(url, _timeout);
 }
 catch (const HootException& e)
 {
   const QString errorMsg = e.what();
   throw HootException("Error getting available translation apps. error: " + errorMsg);
 }
 if (request.getHttpStatus() != HttpResponseCode::HTTP_OK)
 {
   throw HootException(QString("Reply error:\n%1").arg(request.getErrorString()));
 }

 return StringUtils::jsonStringToPropTree(request.getResponseContent());
}

std::shared_ptr<boost::property_tree::ptree> HootServicesLanguageInfoClient::getAvailableLanguages(const QString& type)
{
  LOG_DEBUG("Checking languages available for: " << type << "...");

  QString urlStr;
  QStringList apps;
  if (type == "translatable")
  {
    urlStr = _getTranslatableUrl();
    apps = QStringList(_translator);
  }
  else
  {
    urlStr = _getDetectableUrl();
    apps = _detectors;
  }
  LOG_VARD(urlStr);
  LOG_VARD(apps);

  QUrl url(urlStr);
  QMap<QNetworkRequest::KnownHeaders, QVariant> headers;
  headers[QNetworkRequest::ContentTypeHeader] = HootNetworkUtils::CONTENT_TYPE_JSON;
  HootNetworkRequest request;
  if (_useCookies)
  {
    request.setCookies(_cookies);
  }
  try
  {
    request.networkRequest(
      url, _timeout, headers, QNetworkAccessManager::Operation::PostOperation,
      _getAvailableLanguagesRequestData(apps).toUtf8());
  }
  catch (const HootException& e)
  {
    const QString errorMsg = e.what();
    throw HootException("Error getting available translation languages. error: " + errorMsg);
  }
  if (request.getHttpStatus() != HttpResponseCode::HTTP_OK)
  {
    throw HootException(QString("Reply error:\n%1").arg(request.getErrorString()));
  }

  return StringUtils::jsonStringToPropTree(request.getResponseContent());
}

QString HootServicesLanguageInfoClient::_getAvailableLanguagesRequestData(const QStringList& apps) const
{ 
  boost::property_tree::ptree requestObj;
  requestObj.add_child("apps", *StringUtils::stringListToJsonStringArray(apps));

  std::stringstream requestStrStrm;
  boost::property_tree::json_parser::write_json(requestStrStrm, requestObj);
  return QString::fromStdString(requestStrStrm.str());
}

}
