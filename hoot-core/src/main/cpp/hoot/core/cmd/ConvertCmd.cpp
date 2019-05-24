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
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/DataConverter.h>

// Qt
#include <QElapsedTimer>
#include <QStringList>

using namespace std;

namespace hoot
{

class ConvertCmd : public BaseCommand
{
public:

  static string className() { return "hoot::ConvertCmd"; }

  ConvertCmd() {}

  virtual QString getName() const { return "convert"; }

  virtual QString getDescription() const { return "Converts between map formats"; }

  virtual int runSimple(QStringList args) override
  {
    if (args.size() != 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two parameters.").arg(getName()));
    }

    QElapsedTimer timer;
    timer.start();

    LOG_VART(args.size());
    LOG_VART(args);
    QStringList inputs;
    QString output;
    int argIndex = 0;
    for (int i = 0; i < args.size(); i++)
    {
      const QString arg = args[i];
      LOG_VART(arg);
      // Formerly, "--" options existed and were required to all be at the end of the command, so
      // you could break here once you reached them, and you knew you were done parsing
      // inputs/outputs. Now, the command doesn't take any command line options (uses all
      // configuration options), so let's throw if we see a command line option. If we add any
      // command line options back in at some point, then we can switch this logic back to how
      // it originally was.
      if (arg.startsWith("--"))
      {
        //break;
        throw IllegalArgumentException(
          QString("The convert command takes no inline options starting with '--'. All options ") +
          QString("are passed in as configuration options (-D)."));
      }
      argIndex++;
      inputs.append(arg);
    }
    LOG_VART(inputs.size());
    LOG_VART(argIndex);
    output = inputs.at(argIndex - 1);
    inputs.removeAt(argIndex - 1);
    LOG_VART(inputs.size());
    LOG_VART(inputs);
    LOG_VART(output);  

    DataConverter converter;
    converter.setConfiguration(conf());
    converter.convert(inputs, output);

    QString msg = "Convert operation completed in ";
    const qint64 timeElapsed = timer.elapsed();
    if (timeElapsed > 60000)
    {
      msg += QString::number(timeElapsed / 1000 / 60) + " minutes";
    }
    else
    {
      msg += QString::number(timeElapsed / 1000) + " seconds";
    }
    LOG_DEBUG(msg);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ConvertCmd)

}

