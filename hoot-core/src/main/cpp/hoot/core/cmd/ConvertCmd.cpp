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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
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

  virtual QString getDescription() const
  { return "Converts map data from one input format to another"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() < 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes at least two parameters.").arg(getName()));
    }

    QElapsedTimer timer;
    timer.start();

    DataConverter converter;

    LOG_VART(args.size());
    LOG_VART(args);
    QStringList inputs;
    QString output;
    int argIndex = 0;
    for (int i = 0; i < args.size(); i++)
    {
      const QString arg = args[i];
      LOG_VART(arg);
      //-- options are assumed to be all at the end of the command, so we're done parsing
      //inputs/outputs once we reach one of them
      if (arg.startsWith("--"))
      {
        break;
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

    if (args.contains("--trans"))
    {
      const QString translation = args.at(args.indexOf("--trans") + 1).trimmed();
      if (translation.isEmpty())
      {
        throw HootException("Invalid translation specified.");
      }
      converter.setTranslation(translation);
    }    

    if (args.contains("--cols"))
    {
      converter.setColsArgSpecified(true);
      const QStringList cols =
        args.at(args.indexOf("--cols") + 1).trimmed().split(",", QString::SkipEmptyParts);
      converter.setColumns(cols);
    }

    if (args.contains("--limit"))
    {
      bool ok;
      const int featureReadLimit = args.at(args.indexOf("--limit") + 1).trimmed().toInt(&ok);
      if (!ok)
      {
        throw HootException("Invalid input specified for limit: " +
                            args.at(args.indexOf("--limit") + 1));
      }
      converter.setFeatureReadLimit(featureReadLimit);
    }

    converter.setConfiguration(conf());

    converter.convert(inputs, output);

    LOG_DEBUG("Convert operation complete.");
    QString msg = "Convert operation took ";
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

