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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/info/Version.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QStringList>
#include <QVector>

using namespace std;

namespace hoot
{

class HelpCmd : public BaseCommand
{
public:

  static string className() { return "hoot::HelpCmd"; }

  HelpCmd()
  {
    _forceToRndList.append("build-model");
  }

  static bool commandCompare(const std::string& n1, const std::string& n2)
  {
    std::shared_ptr<Command> c1(Factory::getInstance().constructObject<Command>(n1));
    std::shared_ptr<Command> c2(Factory::getInstance().constructObject<Command>(n2));

    return c1->getName() < c2->getName();
  }

  static bool commandCompare2(const std::shared_ptr<Command>& c1, const std::shared_ptr<Command>& c2)
  {
    return c1->getName() < c2->getName();
  }

  virtual QString getName() const { return "help"; }

  virtual QString getDescription() const { return "Displays help documentation"; }

  int runSimple(QStringList args)
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

  // display names of commands that we want to appear in the advanced rnd list despite the fact
  // they are part of core
  QStringList _forceToRndList;

  int _printDetails(const QString& command)
  {
    int result = 0;
    vector<string> cmds = Factory::getInstance().getObjectNamesByBase(Command::className());

    bool foundIt = false;
    for (size_t i = 0; i < cmds.size(); i++)
    {
      std::shared_ptr<Command> c(Factory::getInstance().constructObject<Command>(cmds[i]));

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
    const vector<string> cmds = Factory::getInstance().getObjectNamesByBase(Command::className());
    vector<std::shared_ptr<Command>> coreCmds;
    vector<std::shared_ptr<Command>> rndCmds;
    for (size_t i = 0; i < cmds.size(); i++)
    {
      const string cmdClassName = cmds[i];
      std::shared_ptr<Command> command(
        Factory::getInstance().constructObject<Command>(cmdClassName));
      if (command->displayInHelp())
      {
        const QString commandName = command->getName();
        if (command->getType() == "core" && !_forceToRndList.contains(commandName))
        {
          coreCmds.push_back(command);
        }
        else if (command->getType() == "rnd" || _forceToRndList.contains(commandName))
        {
          rndCmds.push_back(command);
        }
      }

    }
    sort(coreCmds.begin(), coreCmds.end(), commandCompare2);
    sort(rndCmds.begin(), rndCmds.end(), commandCompare2);

    // Please update the asciidoc user documentation if you change this usage.
    cout << "usage: hoot <command> [--trace] [--debug] [--status] [--warn] [--error] " <<
            "[-D name=value] [--conf <path>] [<args>]\n";
    cout << endl;
    cout << "For detailed help on the following commands type: hoot help (command name)\n"
            "\n";
    _printCommands(coreCmds);
    cout << endl << "Advanced:" << endl << endl;
    _printCommands(rndCmds);

    return 0;
  }

  void _printCommands(const vector<std::shared_ptr<Command>>& cmds)
  {
    for (size_t i = 0; i < cmds.size(); i++)
    {
      std::shared_ptr<Command> command = cmds[i];
      //spacing here is roughly the size of the longest command name plus a small buffer
      const int spaceSize = 30 - command->getName().size();
      const QString line = command->getName() + QString(spaceSize, ' ') + command->getDescription();
      cout << "  " << line << endl;
    }
  }

  int _printVerbose()
  {
    vector<string> cmds = Factory::getInstance().getObjectNamesByBase(Command::className());
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
