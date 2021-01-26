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
 * @copyright Copyright (C) 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/rnd/language/LanguageInfoProvider.h>
#include <hoot/rnd/language/HootServicesLanguageInfoResponseParser.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QUrl>

namespace hoot
{

class InfoRndCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::InfoRndCmd"; }

  InfoRndCmd() = default;

  virtual QString getName() const override { return "info-rnd"; }

  virtual QString getDescription() const override
  { return "Displays information about capabilities specific to the hoot-rnd module"; }

  virtual QString getType() const override { return "rnd"; }

  virtual int runSimple(QStringList& args) override
  {
    // only allowing one option per command
    const QStringList supportedOpts = _getSupportedOptions();
    QStringList specifiedOpts;
    for (int i = 0; i < args.size(); i++)
    {
      const QString arg = args.at(i);
      if (specifiedOpts.contains(arg) || (supportedOpts.contains(arg) && specifiedOpts.size() > 0))
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw IllegalArgumentException(QString("%1 takes a single option.").arg(getName()));
      }
      specifiedOpts.append(arg);
    }
    if (specifiedOpts.size() == 0)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(QString("%1 takes a single option.").arg(getName()));
    }
    LOG_VARD(specifiedOpts.size());

    if (specifiedOpts.contains("--languages"))
    {
      args.removeAt(args.indexOf("--languages"));
      if (args.size() != 1)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw IllegalArgumentException(
          QString("%1 with the --languages option takes one parameter.").arg(getName()));
      }

      // only allowing one option per command
      const QStringList supportedOpts = _getSupportedLanguageOptions();
      QStringList specifiedOpts;
      for (int i = 0; i < args.size(); i++)
      {
        const QString arg = args.at(i);
        if (specifiedOpts.contains(arg) ||
            (supportedOpts.contains(arg) && specifiedOpts.size() > 0))
        {
          std::cout << getHelp() << std::endl << std::endl;
          throw IllegalArgumentException(QString("%1 takes a single option.").arg(getName()));
        }
        specifiedOpts.append(arg);
      }
      if (specifiedOpts.size() == 0)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw IllegalArgumentException(
          QString("%1 with the --languages option takes a single option.").arg(getName()));
      }
      LOG_VARD(specifiedOpts.size());

      ConfigOptions opts = ConfigOptions(conf());

      try
      {
        std::shared_ptr<LanguageInfoProvider> client;
        client.reset(
          Factory::getInstance().constructObject<LanguageInfoProvider>(
            opts.getLanguageInfoProvider()));
        client->setConfiguration(conf());

        const QString type = args[0].replace("--", "").toLower();

        QString displayStr;
        if (type == "translatable" || type == "detectable")
        {
          displayStr =
            HootServicesLanguageInfoResponseParser::parseAvailableLanguagesResponse(
              type, client->getAvailableLanguages(type));
        }
        else
        {
          displayStr =
            HootServicesLanguageInfoResponseParser::parseAvailableAppsResponse(
              type, client->getAvailableApps(type));
        }
        std::cout << displayStr << std::endl;
      }
      catch (const HootException& e)
      {
        LOG_VART(e.getWhat());
        if (e.getWhat().contains("Access tokens for user"))
        {
          std::cout <<
            "You must log in to the Hootenanny Web Services before displaying supported language information." <<
            std::endl;
        }
      }
    }
    else
    {
      return 1;
    }

    return 0;
  }

private:

  QStringList _getSupportedLanguageOptions() const
  {
    QStringList options;
    options.append("--detectable");
    options.append("--detectors");
    options.append("--translatable");
    options.append("--translators");
    return options;
  }

  QStringList _getSupportedOptions() const
  {
    QStringList options;
    options.append("--languages");
    return options;
  }
};

HOOT_FACTORY_REGISTER(Command, InfoRndCmd)
}

