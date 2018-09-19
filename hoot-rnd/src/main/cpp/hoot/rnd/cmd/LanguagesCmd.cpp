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
#include <hoot/rnd/language/translators/HootServicesTranslationInfoResponseParser.h>

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
    QString displayStr;
    if (type == "translatable" || type == "detectable")
    {
      displayStr =
        HootServicesTranslationInfoResponseParser::parseAvailableLanguagesResponse(
          type, _client.getAvailableLanguages(type));
    }
    else
    {
      displayStr =
        HootServicesTranslationInfoResponseParser::parseAvailableAppsResponse(
          type, _client.getAvailableApps(type));
    }
    std::cout << displayStr << std::endl;

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
};

HOOT_FACTORY_REGISTER(Command, LanguagesCmd)

}

