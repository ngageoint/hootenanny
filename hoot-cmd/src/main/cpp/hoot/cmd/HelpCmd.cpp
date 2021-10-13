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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/info/Version.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QString>
#include <QStringList>
#include <QVector>

using namespace std;

namespace hoot
{

class HelpCmd : public BaseCommand
{
public:

  static QString className() { return "HelpCmd"; }

  QString getName() const override { return "help"; }
  QString getDescription() const override { return "Displays help documentation"; }

  HelpCmd() = default;

  static bool commandCompare(const QString& n1, const QString& n2)
  {
    std::shared_ptr<Command> c1 = Factory::getInstance().constructObject<Command>(n1);
    std::shared_ptr<Command> c2 =  Factory::getInstance().constructObject<Command>(n2);
    return c1->getName() < c2->getName();
  }

  static bool commandCompare2(const std::shared_ptr<Command>& c1, const std::shared_ptr<Command>& c2)
  {
    return c1->getName() < c2->getName();
  }

  virtual int runSimple(QStringList& args) override
  {
    if (args.size() == 0 || args.size() > 1)
    {
      return _printSummary();
    }
    else if (args.size() == 1 && args[0] == "--verbose")
    {
      return _printVerbose();
    }
    else
    {
      return _printDetails(args[0]);
    }
  }

private:

  int _printDetails(const QString& command)
  {
    int result = 0;
    vector<QString> cmds =
      Factory::getInstance().getObjectNamesByBase(Command::className());

    bool foundIt = false;
    for (size_t i = 0; i < cmds.size(); i++)
    {
      std::shared_ptr<Command> c = Factory::getInstance().constructObject<Command>(cmds[i]);

      if (c->getName() == command)
      {
        foundIt = true;
        cout << endl;
        cout << c->getHelp().toStdString() << endl;
      }
    }

    if (!foundIt)
    {
      cout << "Unable to find command: " << command << endl;
      result = -1;
    }

    return result;
  }

  int _printSummary()
  {
    cout << endl << "For detailed command help type: hoot help (command name)" << endl << endl;

    // Please update the asciidoc user documentation if you change this usage.
    cout << "Usage:" << endl << endl;
    cout << "hoot <command> [--logLevel] [--color] [-C configFile ] [-D optionName=optionValue] \\" << endl <<
            "  [-D optionName=\"<optionValueEntry 1>;<optionValueEntry 2>;...\"] [args]"
         << endl << endl;

    const vector<QString> cmds = Factory::getInstance().getObjectNamesByBase(Command::className());
    vector<std::shared_ptr<Command>> coreCmds;
    vector<std::shared_ptr<Command>> advancedCmds;
    for (size_t i = 0; i < cmds.size(); i++)
    {
      const QString cmdClassName = cmds[i];
      LOG_VARD(cmdClassName);
      std::shared_ptr<Command> command =
        Factory::getInstance().constructObject<Command>(cmdClassName);
      if (command->displayInHelp())
      {
        const QString commandName = command->getName();
        // Currently, there's only one josm command and we want it in the main list. So, just lump
        // it in with the core commands.
        if ((command->getType() == "core" || command->getType() == "josm"))
        {
          coreCmds.push_back(command);
        }
        else if (command->getType() == "advanced")
        {
          advancedCmds.push_back(command);
        }
        else
        {
          throw IllegalArgumentException("Invalid command type: " + command->getType());
        }
      }

    }
    sort(coreCmds.begin(), coreCmds.end(), commandCompare2);
    sort(advancedCmds.begin(), advancedCmds.end(), commandCompare2);
    LOG_VART(coreCmds.size());
    LOG_VART(advancedCmds.size());

    cout << "Basic Commands:" << endl << endl;
    _printCommands(coreCmds);
    cout << endl << "Advanced Commands:" << endl << endl;
    _printCommands(advancedCmds);

    cout << endl << "Log Levels:" << endl << endl;
    cout << "  --error" << endl;
    cout << "  --warn" << endl;
    cout << "  --status (default)" << endl;
    cout << "  --info" << endl;
    cout << "  --debug" << endl;
    cout << "  --trace" << endl;

    cout << endl << "Miscellaneous:" << endl << endl;
    cout << "  --color                     Colorize the output logs using ANSI colors" << endl << endl;

    cout << "List Option Operations:" << endl << endl;
    cout << "  Append:  [-D optionName+=optionValue]" << endl;
    cout << "  Prepend: [-D optionName++=optionValue]" << endl;
    cout << "  Remove:  [-D optionName-=optionValue]" << endl;
    cout << "  Replace: [-D optionName=\"[old optionValue 1]->[new optionValue 1];\\"
         << endl << "                           [old optionValue 2]->[new optionValue 2]\"...]"
         << endl;

    return 0;
  }

  void _printCommands(const vector<std::shared_ptr<Command>>& cmds)
  {
    for (size_t i = 0; i < cmds.size(); i++)
    {
      std::shared_ptr<Command> command = cmds[i];
      LOG_VART(command->getName());
      //spacing here is roughly the size of the longest command name plus a small buffer
      const int spaceSize = 30 - command->getName().size();
      const QString line = command->getName() + QString(spaceSize, ' ') + command->getDescription();
      cout << "  " << line << endl;
    }
  }

  int _printVerbose()
  {
    vector<QString> cmds = Factory::getInstance().getObjectNamesByBase(Command::className());
    LOG_VART(cmds);
    sort(cmds.begin(), cmds.end(), commandCompare);
    for (size_t i = 0; i < cmds.size(); i++)
    {
      std::shared_ptr<Command> c(Factory::getInstance().constructObject<Command>(cmds[i]));
      if (c->displayInHelp())
      {
        cout << endl;
        cout << c->getHelp().toStdString() << endl;
      }
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, HelpCmd)

}
