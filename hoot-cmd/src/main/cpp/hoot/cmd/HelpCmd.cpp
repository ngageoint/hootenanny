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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
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
    // Add hoot-core commands to this list that are in production use but either are typically
    // used by developers only during advanced tasks like model training, etc. or commands that
    // admittedly may need some testing against real world data before they're ready for the prime
    // time. This makes the command list display a little cleaner and less confusing.
    _forceToRndList.append("build-model");
    _forceToRndList.append("de-duplicate");
    _forceToRndList.append("login");
    _forceToRndList.append("logout");
    _forceToRndList.append("node-density-plot");
    _forceToRndList.append("optimize-network-conf");
    _forceToRndList.append("perty");
    _forceToRndList.append("score-matches");
    _forceToRndList.append("score-matches-diff");
    _forceToRndList.append("sync-element-ids");
    _forceToRndList.append("type-similarity");
    _forceToRndList.append("type-tagger-rules");
    _forceToRndList.append("write-name-counts");
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

  virtual QString getName() const override { return "help"; }

  virtual QString getDescription() const override { return "Displays help documentation"; }

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
      LOG_VART(cmdClassName);
      std::shared_ptr<Command> command(
        Factory::getInstance().constructObject<Command>(cmdClassName));
      if (command->displayInHelp())
      {
        const QString commandName = command->getName();
        LOG_VART(commandName);
        // Currently, there's only one josm command and we want it in the main list. So, just lump
        // it in with the core commands.
        if ((command->getType() == "core" || command->getType() == "josm") &&
            !_forceToRndList.contains(commandName))
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
    LOG_VART(coreCmds.size());
    LOG_VART(rndCmds.size());

    // Please update the asciidoc user documentation if you change this usage.
    cout << "Usage: hoot <command> [--logLevel] [-C configFile ] [-D optionName=optionValue] " <<
            "[-D optionName=\"<optionValueEntry 1>;<optionValueEntry 2>;...\"] [args]"
         << endl << endl;

    cout << "Log Levels:" << endl;
    cout << "  --trace" << endl;
    cout << "  --debug" << endl;
    cout << "  --info" << endl;
    cout << "  --status" << endl;
    cout << "  --warn" << endl;
    cout << "  --error" << endl;
    cout << endl;

    cout << "List Option Operations:" << endl;
    cout << "  Append:  [-D optionName+=optionValueEntry]" << endl;
    cout << "  Prepend: [-D optionName++=optionValueEntry]" << endl;
    cout << "  Remove:  [-D optionName-=optionValueEntry]" << endl;
    cout << "  Replace: [-D optionName=\"<old optionValueEntry 1>-><new optionValueEntry 1>;<old optionValueEntry 2>-><new optionValueEntry 2>\"...]"
         << endl;
    cout << endl;

    cout << "For detailed command help type: hoot help (command name)" << endl << endl;

    cout << endl << "Basic Commands:" << endl << endl;
    _printCommands(coreCmds);
    cout << endl << "Advanced Commands:" << endl << endl;
    _printCommands(rndCmds);

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
    vector<string> cmds = Factory::getInstance().getObjectNamesByBase(Command::className());
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
