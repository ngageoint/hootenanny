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
}

//TODO: return as pointer?
boost::property_tree::ptree HootServicesTranslationInfoClient::getAvailableLanguages(
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
    throw HootException(QString("Languages reply error:\n%1").arg(reply->error()));
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

  return replyObj;
}

}
