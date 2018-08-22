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
#include <hoot/core/util/NetUtils.h>

// Qt
#include <QEventLoop>
#include <QVariant>
#include <QNetworkRequest>
#include <QNetworkReply>

// std
#include <iostream>
#include <sstream>

namespace hoot
{

HootServicesTranslationInfoClient::HootServicesTranslationInfoClient()
{
  _client.reset(new QNetworkAccessManager());
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

 //create the request
 QUrl url(urlStr);
 QNetworkRequest request(url);

 //send the request and wait for a response
 QNetworkReply* reply = _client->get(request);
 QEventLoop loop;
 QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
 loop.exec();

 //check for a response error
 NetUtils::checkWebReplyForError(reply);

 //parse and return the response
 return NetUtils::replyToPropTree(reply);
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

  //create the request
  std::stringstream requestStrStrm;
  boost::shared_ptr<QNetworkRequest> request =
    _getAvailableLanguagesRequest(urlStr, apps, requestStrStrm);

  //send the request and wait for a response
  QNetworkReply* reply =
    _client->post(*request, QString::fromStdString(requestStrStrm.str()).toUtf8());
  QEventLoop loop;
  QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  loop.exec();

  //check for a response error
  NetUtils::checkWebReplyForError(reply);

  //parse and return the response
  return NetUtils::replyToPropTree(reply);
}

boost::shared_ptr<QNetworkRequest> HootServicesTranslationInfoClient::_getAvailableLanguagesRequest(
  const QString urlStr, const QStringList apps, std::stringstream& requestStrStrm)
{
  QUrl url(urlStr);
  boost::shared_ptr<QNetworkRequest> request(new QNetworkRequest(url));

  request->setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");
  boost::property_tree::ptree requestObj;
  boost::property_tree::ptree appsObj;
  for (int i = 0; i < apps.size(); i++)
  {
    boost::property_tree::ptree appObj;
    appObj.put("", apps.at(i).toStdString());
    appsObj.push_back(std::make_pair("", appObj));
  }
  requestObj.add_child("apps", appsObj);
  boost::property_tree::json_parser::write_json(requestStrStrm, requestObj);
  LOG_VART(requestStrStrm.str());

  return request;
}

}
