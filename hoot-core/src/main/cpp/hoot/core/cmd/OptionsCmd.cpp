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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>

// Qt
#include <QFile>

using namespace std;

namespace hoot
{

/**
 * Lists Hootenanny config options
 */
class OptionsCmd : public BaseCommand
{
public:

  static string className() { return "hoot::OptionsCmd"; }

  OptionsCmd() { }

  virtual QString getName() const { return "options"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() > 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes zero to two parameters.").arg(getName()));
    }

    const QString configOptionsFile = ConfigOptions(conf()).getConfigOptionsFile();
    QString cmd;
    if (args.size() == 0)
    {
      cmd = "cat " + configOptionsFile + " | grep '^===' | sed 's/=== //g'";
    }
    else if (args.size() == 1)
    {
      if (args[0].toLower() == "--detail")
      {
        cmd = "cat " + configOptionsFile + " | grep -v '//'";
      }
      else
      {
        cmd =
          "cat " + configOptionsFile + " | grep '^=== " + args[0].toLower().trimmed() +
          "' | sed 's/=== //g'";
      }
    }
    else    //size = 2
    {
      if (args[0].toLower() == "--detail" || args[1].toLower() != "--detail")
      {
        cout << getHelp() << endl << endl;
        throw HootException(QString("%1 requires --detail specifier at end of command.").arg(getName()));
      }

      //TODO: replace this w/ some fancy lookahead regex called by grep...doing it by brute force for now
      //const QString configFileContents = FileUtils::readFully(configOptionsFile);
      QFile file(configOptionsFile);
      if (!file.open(QFile::ReadOnly))
      {
        throw HootException("Error opening file for reading: " + configOptionsFile);
      }

      QString output;
      const QString optionName = args[0].toLower().trimmed();
      bool foundOption = false;
      while (!file.atEnd())
      {
        const QString line = QString::fromUtf8(file.readLine().constData());

        if (line.startsWith("=== " + optionName))
        {
          foundOption = true;
          output += line;
        }
        else if (line.startsWith("=== "))
        {
          foundOption = false;
        }

        if (foundOption)
        {
          output += line ;
        }
      }

      std::cout << output;
    }
    if (!cmd.isEmpty() && std::system(cmd.toStdString().c_str()) != 0)
    {
      throw HootException("Unable to list configuration options.");
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, OptionsCmd)


}

