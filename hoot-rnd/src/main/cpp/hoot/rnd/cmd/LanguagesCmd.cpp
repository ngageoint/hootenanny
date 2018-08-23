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

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Log.h>
#include <hoot/rnd/language/translators/HootServicesTranslationInfoClient.h>

// Boost
#include <boost/foreach.hpp>

// Qt
#include <QUrl>

namespace hoot
{

/**
 * Displays information about languages supported by Hootenanny to English translation
 *
 * If language translation is moved out of hoot-rnd to hoot-core, this command could be replaced
 * by an add-on to the InfoCmd, rather than being a separate command itself.
 */
class LanguagesCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::LanguagesCmd"; }

  LanguagesCmd() { }

  virtual QString getName() const { return "languages"; }

  virtual QString getDescription() const
  { return "Displays information about translatable/detectable spoken languages"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 1)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes one parameter.").arg(getName()));
    }

    //only allowing one option per command
    const QStringList supportedOpts = _getSupportedOptions();
    QStringList specifiedOpts;
    for (int i = 0; i < args.size(); i++)
    {
      const QString arg = args.at(i);
      if (specifiedOpts.contains(arg) || (supportedOpts.contains(arg) && specifiedOpts.size() > 0))
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw HootException(QString("%1 takes a single option.").arg(getName()));
      }
      specifiedOpts.append(arg);
    }
    if (specifiedOpts.size() == 0)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes a single option.").arg(getName()));
    }
    LOG_VARD(specifiedOpts.size());

    const QString type = args[0].replace("--", "").toLower();

    _client.setConfiguration(conf());
    if (type == "translatable" || type == "detectable")
    {
      _printAvailableLanguages(type);
    }
    else
    {
      _printAvailableApps(type);
    }

    return 0;
  }

private:

  HootServicesTranslationInfoClient _client;

  QStringList _getSupportedOptions() const
  {
    QStringList options;
    options.append("--detectable");
    options.append("--detectors");
    options.append("--translatable");
    options.append("--translators");
    return options;
  }

  void _printAvailableLanguages(const QString type)
  {
    boost::shared_ptr<boost::property_tree::ptree> replyObj = _client.getAvailableLanguages(type);

    std::cout << type << " languages: " << std::endl << std::endl;
    int langCtr = 0;
    int availableCtr = 0;
    BOOST_FOREACH (boost::property_tree::ptree::value_type& language, replyObj->get_child("languages"))
    {
      const QString name =
        QUrl::fromPercentEncoding(
          QString::fromStdString(language.second.get<std::string>("name")).toUtf8());
      const bool available = language.second.get<bool>("available");
      const QString availableStr = available ? "yes" : "no";

      std::cout << "Code: " << QString::fromStdString(language.second.get<std::string>("iso6391code")) << std::endl;
      std::cout << "Name: " << name << std::endl;
      std::cout << "Available: " << availableStr << std::endl;
      std::cout << std::endl;
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
    std::cout << QString::number(langCtr) << " languages are supported for " << descriptor << ".  " << std::endl;
    std::cout << "Currently, " << QString::number(availableCtr) << " of those languages are available for " << descriptor << ".  " << std::endl;
  }

  void _printAvailableApps(const QString type)
  {
    boost::shared_ptr<boost::property_tree::ptree> replyObj = _client.getAvailableApps(type);

    std::cout << "Available language detectors: " << std::endl << std::endl;
    int appCtr = 0;
    BOOST_FOREACH (boost::property_tree::ptree::value_type& app, replyObj->get_child("apps"))
    {
      std::cout << "Name: " << QString::fromStdString(app.second.get<std::string>("name")) << std::endl;
      std::cout << "Description: " << QString::fromStdString(app.second.get<std::string>("description")) << std::endl;
      std::cout << "URL: " << QString::fromStdString(app.second.get<std::string>("url")) << std::endl;
      std::cout << std::endl;
      appCtr++;
    }

    std::cout << QString::number(appCtr) << " " << type << " are available.  " << std::endl;
  }
};

HOOT_FACTORY_REGISTER(Command, LanguagesCmd)

}

