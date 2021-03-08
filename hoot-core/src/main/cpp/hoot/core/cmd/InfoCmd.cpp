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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/info/ApiEntityDisplayInfo.h>
#include <hoot/core/info/ConfigOptionsDisplayer.h>
#include <hoot/core/info/FormatsDisplayer.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QUrl>

namespace hoot
{

/*
 * Some time needs to be spent exploring ways to make the classes containing the information
 * printed out by this command auto-register themselves in a more extensible way.
 */
class InfoCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::InfoCmd"; }

  InfoCmd() = default;

  virtual QString getName() const override { return "info"; }

  virtual QString getDescription() const override
  { return "Displays information about capabilities"; }

  virtual int runSimple(QStringList& args) override
  {
    // only allowing one option per command
    const QStringList supportedOpts = _getSupportedOptions();
    QStringList specifiedOpts;
    for (int i = 0; i < args.size(); i++)
    {
      const QString arg = args.at(i);
      if (specifiedOpts.contains(arg) || (supportedOpts.contains(arg) && !specifiedOpts.empty()))
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw IllegalArgumentException(QString("%1 takes a single option.").arg(getName()));
      }
      specifiedOpts.append(arg);
    }
    if (specifiedOpts.empty())
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(QString("%1 takes a single option.").arg(getName()));
    }
    LOG_VARD(specifiedOpts.size());

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
        std::cout << getHelp() << std::endl << std::endl;
        throw IllegalArgumentException(
          "Only one option can be used, either --option-names or --option-details.");
      }

      if (args.size() > 1)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw IllegalArgumentException(
          QString("%1 with the --config-options option takes zero to one parameters.")
            .arg(getName()));
      }

      if (args.empty())
      {
        std::cout << ConfigOptionsDisplayer::getAllOptionNames(getDetails).toStdString();
      }
      else
      {
        std::cout << ConfigOptionsDisplayer::getOptionName(args[0], getDetails).toStdString();
      }
    }
    else if (specifiedOpts.contains("--formats"))
    {
      args.removeAt(args.indexOf("--formats"));
      if (args.size() > 2)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw IllegalArgumentException(
          QString("%1 with the --formats option takes between zero and two parameters.")
            .arg(getName()));
      }

      QStringList formatSubOptions;
      formatSubOptions.append("--input");
      formatSubOptions.append("--input-streamable");
      formatSubOptions.append("--input-bounded");
      formatSubOptions.append("--output");
      formatSubOptions.append("--output-streamable");
      formatSubOptions.append("--ogr");
      for (int i = 0; i < args.size(); i++)
      {
        const QString arg = args.at(i);
        if (!formatSubOptions.contains(arg))
        {
          std::cout << getHelp() << std::endl << std::endl;
          throw IllegalArgumentException("Invalid parameter: " + arg + " passed to " + getName());
        }
      }

      bool displayOgrOnly = false;
      if (args.contains("--ogr"))
      {
        displayOgrOnly = true;
        args.removeAt(args.indexOf("--ogr"));
      }

      bool displayInputs = false;
      if (args.contains("--input"))
      {
        displayInputs = true;
        args.removeAt(args.indexOf("--input"));
      }

      bool displayInputsSupportingBounds = false;
      if (args.contains("--input-bounded"))
      {
        displayInputsSupportingBounds = true;
        args.removeAt(args.indexOf("--input-bounded"));
      }

      bool displayInputsSupportingStreaming = false;
      if (args.contains("--input-streamable"))
      {
        displayInputsSupportingStreaming = true;
        args.removeAt(args.indexOf("--input-streamable"));
      }

      bool displayOutputs = false;
      if (args.contains("--output"))
      {
        displayOutputs = true;
        args.removeAt(args.indexOf("--output"));
      }

      bool displayOutputsSupportingStreaming = false;
      if (args.contains("--output-streamable"))
      {
        displayOutputsSupportingStreaming = true;
        args.removeAt(args.indexOf("--output-streamable"));
      }

      // If none were specified, show them all, except OGR.
      // This is getting a little messy...maybe pass in an object with the settings instead...
      if (!displayInputs && !displayInputsSupportingBounds && !displayInputsSupportingStreaming &&
          !displayOutputs && !displayOutputsSupportingStreaming &&
          (args.empty() || (args.size() == 1 && displayOgrOnly)))
      {
        displayInputs = true;
        displayInputsSupportingStreaming = true;
        displayInputsSupportingBounds = true;
        displayOutputs = true;
        displayOutputsSupportingStreaming = true;
      }

      std::cout <<
        FormatsDisplayer::display(
          displayInputs, displayInputsSupportingStreaming, displayInputsSupportingBounds,
          displayOutputs, displayOutputsSupportingStreaming, displayOgrOnly)
        .toStdString();
    }
    else if (specifiedOpts.contains("--cleaning-operations"))
    {
      std::cout << ApiEntityDisplayInfo::getDisplayInfoOps("map.cleaner.transforms").toStdString();
    }
    else if (specifiedOpts.contains("--conflate-post-operations"))
    {
      std::cout << ApiEntityDisplayInfo::getDisplayInfoOps("conflate.post.ops").toStdString();
    }
    else if (specifiedOpts.contains("--conflate-pre-operations"))
    {
      std::cout << ApiEntityDisplayInfo::getDisplayInfoOps("conflate.pre.ops").toStdString();
    }
    // everything else
    else if (specifiedOpts.size() == 1)
    {
      QString apiEntityType;
      for (int i = 0; i < supportedOpts.size(); i++)
      {
        QString supportedOpt = supportedOpts.at(i);
        if (args.contains(supportedOpt))
        {
          //should only be one of these
          args.removeAt(args.indexOf(supportedOpt));
          apiEntityType = supportedOpt.replace("--", "");
        }
      }
      LOG_VARD(apiEntityType);
      if (!args.empty())
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw IllegalArgumentException(
          QString("%1 with the --operators option takes zero parameters.").arg(getName()));
      }

      std::cout << ApiEntityDisplayInfo::getDisplayInfo(apiEntityType).toStdString();
    }
    else
    {
      return 1;
    }

    return 0;
  }

private:

  QStringList _getSupportedOptions() const
  {
    QStringList options;
    options.append("--cleaning-operations");
    options.append("--config-options");
    options.append("--conflatable-criteria");
    options.append("--conflate-post-operations");
    options.append("--conflate-pre-operations");
    options.append("--criterion-consumers");
    options.append("--feature-extractors");
    options.append("--filters");
    options.append("--formats");
    options.append("--geometry-type-criteria");
    options.append("--matchers");
    options.append("--match-creators");
    options.append("--mergers");
    options.append("--merger-creators");
    options.append("--operators");
    options.append("--string-comparators");
    options.append("--subline-matchers");
    options.append("--subline-string-matchers");
    options.append("--tag-mergers");
    options.append("--value-aggregators");
    options.append("--way-joiners");
    return options;
  }
};

HOOT_FACTORY_REGISTER(Command, InfoCmd)
}

