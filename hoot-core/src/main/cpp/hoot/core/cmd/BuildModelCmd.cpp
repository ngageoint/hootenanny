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
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/ArffToRfConverter.h>
#include <hoot/core/scoring/RandomForestModelBuilder.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

class BuildModelCmd : public BaseCommand
{
public:

  static QString className() { return "BuildModelCmd"; }

  BuildModelCmd() = default;

  QString getName() const override { return "build-model"; }
  QString getDescription() const override
  { return "Builds a random forest model to be used by conflation"; }

  int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    // This argument is only valid when converting to .rf from training data.
    bool exportArffOnly = false;
    if (args.contains("--export-arff-only"))
    {
      args.removeAll("--export-arff-only");
      exportArffOnly = true;
    }

    QString output;
    if (args.size() == 2 && !exportArffOnly)
    {
      // It must be a .arff to .rf conversion.
      output = args[1];
      ArffToRfConverter::convert(args[0], args[1]);
    }
    else if (args.size() < 3 || args.size() % 2 == 0)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(
        QString(
          "%1 takes an odd number of parameters and at least three parameters when converting to .rf from training data.")
        .arg(getName()));
    }
    else
    {
      // It must be a conversion to .rf from training data.
      QStringList inputs;
      for (int i = 0; i < args.size() - 1; i++)
      {
        inputs.append(args.at(i));
      }
      output = args.last();
      RandomForestModelBuilder::build(inputs, args.last(), exportArffOnly);
    }

    LOG_STATUS(
      "Model: ..." << output.right(25) << " built in " <<
      StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, BuildModelCmd)

}

