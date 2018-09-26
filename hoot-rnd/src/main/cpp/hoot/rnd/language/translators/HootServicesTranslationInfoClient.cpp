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

#include "HootServicesTranslationInfoClient.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/io/HootNetworkRequest.h>

// Qt
#include <QVariant>

// std
#include <iostream>
#include <sstream>

namespace hoot
{

HOOT_FACTORY_REGISTER(TranslationInfoProvider, HootServicesTranslationInfoClient)

HootServicesTranslationInfoClient::HootServicesTranslationInfoClient()
{
}

void HootServicesTranslationInfoClient::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  _translator = opts.getLanguageTranslationHootServicesTranslator();
  _detectors = opts.getLanguageTranslationHootServicesDetectors();
  _detectableUrl = opts.getLanguageTranslationHootServicesDetectableLanguagesEndpoint();
  _translatableUrl = opts.getLanguageTranslationHootServicesTranslatableLanguagesEndpoint();
  _detectorsUrl = opts.getLanguageTranslationHootServicesDetectorsEndpoint();
  _translatorsUrl = opts.getLanguageTranslationHootServicesTranslatorsEndpoint();
}

boost::shared_ptr<boost::property_tree::ptree> HootServicesTranslationInfoClient::getAvailableApps(
 const QString type)
{
 LOG_DEBUG("Checking apps available for: " << type << "...");

 QString urlStr;
 if (type == "translators")
 {
   urlStr = _translatorsUrl;
 }
 else
 {
   urlStr = _detectorsUrl;
 }
 LOG_VARD(urlStr);

 //create and execute the request
 QUrl url(urlStr);
 HootNetworkRequest request;
 request.networkRequest(url);

 //check for a response error
 if (request.getHttpStatus() != 200)
 {
   throw HootException(QString("Reply error:\n%1").arg(request.getErrorString()));
 }

 //parse the response data
 return StringUtils::jsonStringToPropTree(request.getResponseContent());
}

 boost::shared_ptr<boost::property_tree::ptree> HootServicesTranslationInfoClient::getAvailableLanguages(
  const QString type)
{
  LOG_DEBUG("Checking languages available for: " << type << "...");

  QString urlStr;
  QStringList apps;
  if (type == "translatable")
  {
    urlStr = _translatableUrl;
    apps = QStringList(_translator);
  }
  else
  {
    urlStr = _detectableUrl;
    apps = _detectors;
  }
  LOG_VARD(urlStr);
  LOG_VARD(apps);

  //create and execute the request
  QUrl url(urlStr);
  QMap<QNetworkRequest::KnownHeaders, QVariant> headers;
  headers[QNetworkRequest::ContentTypeHeader] = "application/json";
  HootNetworkRequest request;
  request.networkRequest(
    url, headers, QNetworkAccessManager::Operation::PostOperation,
    _getAvailableLanguagesRequestData(apps).toUtf8());

  //check for a response error
  if (request.getHttpStatus() != 200)
  {
    throw HootException(QString("Reply error:\n%1").arg(request.getErrorString()));
  }

  //parse the response data
  return StringUtils::jsonStringToPropTree(request.getResponseContent());
}

QString HootServicesTranslationInfoClient::_getAvailableLanguagesRequestData(const QStringList apps)
{ 
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
  return QString::fromStdString(requestStrStrm.str());
}

}
