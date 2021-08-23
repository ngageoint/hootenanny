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

#include "HootServicesLanguageInfoResponseParser.h"

// Qt

#include <QUrl>

namespace hoot
{

QString HootServicesLanguageInfoResponseParser::parseAvailableLanguagesResponse(
    const QString& type,  const std::shared_ptr<boost::property_tree::ptree>& response)
{
  QString displayStr;

  displayStr += type + " languages:\n\n";
  int langCtr = 0;
  int availableCtr = 0;
  for (const boost::property_tree::ptree::value_type& language : response->get_child("languages"))
  {
    LOG_VART(language.second.get<std::string>("name"));
    LOG_VART(QString::fromStdString(language.second.get<std::string>("name")));
    const QString name =
      QUrl::fromPercentEncoding(
        QString::fromStdString(language.second.get<std::string>("name")).toUtf8());
    LOG_VART(name);
    const bool available = language.second.get<bool>("available");
    const QString availableStr = available ? "yes" : "no";

    displayStr += "Code: " +
      QString::fromStdString(language.second.get<std::string>("iso6391Code")) + "\n";
    displayStr += "Name: " + name + "\n";
    displayStr += "Available: " + availableStr + "\n";
    displayStr += "\n";
    langCtr++;
    if (available)
    {
      availableCtr++;
    }
  }

  QString descriptor = "translation";
  if (type != "translatable")
  {
    descriptor = "detection";
  }
  displayStr += QString::number(langCtr) + " languages are supported for " + descriptor + ".\n";
  displayStr +=
    "Currently, " + QString::number(availableCtr) +
    " of those languages are available for " + descriptor + ".";

  return displayStr.trimmed();
}

QString HootServicesLanguageInfoResponseParser::parseAvailableAppsResponse(
    const QString& type, const std::shared_ptr<boost::property_tree::ptree>& response)
{
  QString displayStr;

  displayStr += "Available language " + type + ":\n\n";
  int appCtr = 0;
  for (const boost::property_tree::ptree::value_type& app : response->get_child("apps"))
  {
    displayStr += "Name: " + QString::fromStdString(app.second.get<std::string>("name")) + "\n";
    displayStr += "Description: " +
      QString::fromStdString(app.second.get<std::string>("description")) + "\n";
    displayStr += "URL: " + QString::fromStdString(app.second.get<std::string>("url")) + "\n";
    if (type == "detectors")
    {
      const bool supportsConfidence = app.second.get<bool>("supportsConfidence");
      const QString supportsConfidenceStr = supportsConfidence ? "yes" : "no";
      displayStr += "Supports confidence: " + supportsConfidenceStr + "\n";
    }
    displayStr += "\n";
    appCtr++;
  }

  displayStr += QString::number(appCtr) + " " + type + " are available.";

  return displayStr.trimmed();
}

QMap<QString, QString> HootServicesLanguageInfoResponseParser::getLangCodesToLangs(
  const std::shared_ptr<boost::property_tree::ptree>& response)
{
  QMap<QString, QString> langCodesToLangs;
  for (const boost::property_tree::ptree::value_type& language : response->get_child("languages"))
  {
    const QString langCode =
      QString::fromStdString(language.second.get<std::string>("iso6391Code"));
    LOG_VART(langCode);
    const QString name =
      QUrl::fromPercentEncoding(
        QString::fromStdString(language.second.get<std::string>("name")).toUtf8());
    LOG_VART(name);
    langCodesToLangs[langCode] = name;
  }
  return langCodesToLangs;
}

}
