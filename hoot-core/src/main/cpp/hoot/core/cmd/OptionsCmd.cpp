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
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>

// Qt
#include <QFile>

using namespace std;

namespace hoot
{

class OptionsCmd : public BaseCommand
{

public:

  static string className() { return "hoot::OptionsCmd"; }

  OptionsCmd() {}

  virtual QString getName() const { return "options"; }

  virtual QString getDescription() const
  { return "Prints configuration options"; }

  virtual int runSimple(QStringList args)
  {
    LOG_VARD(args.size());
    if (args.size() < 1 || args.size() > 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes one to two parameters.").arg(getName()));
    }

    const QString configOptionsFile = ConfigOptions().getConfigOptionsFile();
    QString cmd;
    if (args.size() == 1)
    {
      LOG_VARD(args[0].toLower());
      //Had to use "--option-names" here instead of just "--names", b/c there is a "names" command
      //and we still haven't removed backward compatibility for the "--" in front of commands yet.
      if (args[0].toLower() == "--option-names")
      {
        cmd = "cat " + configOptionsFile + " | grep '^===' | sed 's/=== //g'";
      }
      else if (args[0].toLower() == "--option-details")
      {
        cmd = "cat " + configOptionsFile + " | grep -v '//'";
      }
      else
      {
        cout << getHelp() << endl << endl;
        throw HootException(QString("Invalid single input parameter: " + args[0]).arg(getName()));
      }
    }
    else
    {
      LOG_VARD(args[1].toLower());
      if (args[1].toLower() == "--option-names")
      {
        cmd =
          "cat " + configOptionsFile + " | grep '^=== " + args[0].toLower().trimmed() +
          "' | sed 's/=== //g'";
      }
      else if (args[1].toLower() == "--option-details")
      {
        _printAllConfigOptionsDetails(args, configOptionsFile);
        return 0;
      }
      else
      {
        cout << getHelp() << endl << endl;
        throw HootException(
          QString("Invalid input parameters: " + args[0] + " " + args[1]).arg(getName()));
      }
    }

    if (!cmd.isEmpty() && std::system(cmd.toStdString().c_str()) != 0)
    {
      throw HootException("Unable to list configuration options.");
    }

    return 0;
  }

private:

  void _printAllConfigOptionsDetails(const QStringList args, const QString configOptionsFile)
  {
    QFile file(configOptionsFile);
    if (!file.open(QFile::ReadOnly))
    {
      throw HootException("Error opening file for reading: " + configOptionsFile);
    }

    //TODO: replace this w/ some fancy lookahead regex called by grep...doing it by brute force for
    //now
    QString output;
    try
    {
      const QString optionName = args[0].toLower().trimmed();
      bool foundOption = false;
      while (!file.atEnd())
      {
        const QString line = QString::fromUtf8(file.readLine().constData());

        if (line.startsWith("=== " + optionName))
        {
          foundOption = true;
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
    }
    catch (...)
    {
      file.close();
    }
    file.close();

    std::cout << output.trimmed() << std::endl;
  }
};

HOOT_FACTORY_REGISTER(Command, OptionsCmd)

}

