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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/info/ConfigOptionsDisplayer.h>
#include <hoot/core/info/FormatsDisplayer.h>
#include <hoot/core/info/ApiEntityDisplayer.h>

namespace hoot
{

class InfoCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::InfoCmd"; }

  InfoCmd() { }

  virtual QString getName() const { return "info"; }

  virtual QString getDescription() const
  { return "Displays information about Hootenanny capabilities"; }

  virtual int runSimple(QStringList args)
  {
    //only allowing one option per command
    const QString supportedOpts = _getSupportedOptions();
    QStringList specifiedOpts;
    for (int i = 0; i < args.size(); i++)
    {
      const QString arg = args.at(i);
      if (specifiedOpts.contains(arg) || (supportedOpts.contains(arg) && specifiedOpts.size() > 0))
      {
        throw HootException(QString("%1 takes a single option.").arg(getName()));
      }
    }
    if (specifiedOpts.size() == 0)
    {
      throw HootException(QString("%1 takes a single option.").arg(getName()));
    }
    assert(specifiedOpts.size() == 1);

    if (specifiedOpts.contains("--config-options"))
    {
      args.removeAt(args.indexOf("--config-options"));

      bool getNamesOnly = false;
      if (args.contains("--option-names"))
      {
        getNamesOnly = true;
        args.removeAt(args.indexOf("--option-names"));
      }

      bool getDetails = false;
      if (args.contains("--option-details"))
      {
        getDetails = true;
        args.removeAt(args.indexOf("--option-details"));
      }

      if (getNamesOnly && getDetails)
      {
        throw HootException(
          "Only one option can be used, either --option-names or --option-details.");
      }

      if (args.size() > 1)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw HootException(
          QString("%1 with the --config-options option takes zero to one parameters.")
            .arg(getName()));
      }

      if (args.size() == 0)
      {
        ConfigOptionsDisplayer::displayAllOptionNames(getDetails);
      }
      else
      {
        ConfigOptionsDisplayer::displayOptionName(args[0], getDetails);
      }
    }
    else if (specifiedOpts.contains("--formats"))
    {
      args.removeAt(args.indexOf("--formats"));
      if (args.size() > 2)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw HootException(
          QString("%1 with the --formats option takes between zero and two parameters.")
            .arg(getName()));
      }

      for (int i = 0; i < args.size(); i++)
      {
        const QString arg = args.at(i);
        if (arg != "--input" && arg != "--output")
        {
          throw IllegalArgumentException("Invalid parameter: " + arg + " passed to " + getName());
        }
      }

      bool displayInputs = false;
      if (args.contains("--input"))
      {
        displayInputs = true;
        args.removeAt(args.indexOf("--input"));
      }

      bool displayOutputs = false;
      if (args.contains("--output"))
      {
        displayOutputs = true;
        args.removeAt(args.indexOf("--output"));
      }

      if (args.size() == 0)
      {
        displayInputs = true;
        displayOutputs = true;
      }

      FormatsDisplayer::display(displayInputs, displayOutputs);
    }
    else //feature-extractors, operators, matchers, mergers, or tag mergers
    {
      QString apiEntityType;
      for (int i = 0; i < supportedOpts.size(); i++)
      {
        const QString supportedOpt = supportedOpts.at(i);
        if (args.contains(supportedOpt))
        {
          //should only be one of these
          apiEntityType = apiEntityType.replace("--", "");
          args.removeAt(args.indexOf(supportedOpt));
        }
      }
      if (args.size() != 0)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw HootException(
          QString("%1 with the --feature-extractors option takes zero parameters.").arg(getName()));
      }

      ApiEntityDisplayer::display(apiEntityType);
    }

    return 0;
  }

private:

  QStringList _getSupportedOptions() const
  {
    QStringList options;
    options.append("--config-options");
    options.append("--feature-extractors");
    options.append("--formats");
    options.append("--matchers");
    options.append("--mergers");
    options.append("--operators");
    options.append("--tag-mergers");
    return options;
  }
};

HOOT_FACTORY_REGISTER(Command, InfoCmd)
}

