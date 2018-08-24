
#include "HootServicesTranslationInfoResponseParser.h"

// Qt
#include <QString>
#include <QUrl>

namespace hoot
{

QString HootServicesTranslationInfoResponseParser::parseAvailableLanguagesResponse(
  const QString type,  boost::shared_ptr<boost::property_tree::ptree> response)
{
  QString displayStr;

  std::cout << type << " languages: " << std::endl << std::endl;
  int langCtr = 0;
  int availableCtr = 0;
  BOOST_FOREACH (boost::property_tree::ptree::value_type& language, response->get_child("languages"))
  {
    const QString name =
      QUrl::fromPercentEncoding(
        QString::fromStdString(language.second.get<std::string>("name")).toUtf8());
    const bool available = language.second.get<bool>("available");
    const QString availableStr = available ? "yes" : "no";

    displayStr += "Code: " +
      QString::fromStdString(language.second.get<std::string>("iso6391code")) + "\n";
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

  return displayStr;
}

QString HootServicesTranslationInfoResponseParser::parseAvailableAppsResponse(const QString type,
  boost::shared_ptr<boost::property_tree::ptree> response)
{
  QString displayStr;
  std::cout << "Available language detectors: " << std::endl << std::endl;
  int appCtr = 0;
  BOOST_FOREACH (boost::property_tree::ptree::value_type& app, response->get_child("apps"))
  {
    displayStr += "Name: " + QString::fromStdString(app.second.get<std::string>("name")) + "\n";
    displayStr += "Description: " + QString::fromStdString(app.second.get<std::string>("description")) + "\n";
    displayStr += "URL: " + QString::fromStdString(app.second.get<std::string>("url")) + "\n";
    displayStr += "\n";
    appCtr++;
  }

  displayStr += QString::number(appCtr) + " " + type + " are available.  " + "\n";

  return displayStr;
}

}
