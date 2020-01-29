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
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>

#include <hoot/core/io/ArffToRfConverter.h>
#include <hoot/core/scoring/RandomForestModelBuilder.h>

namespace hoot
{

class BuildModelCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::BuildModelCmd"; }

  BuildModelCmd() { }

  virtual QString getName() const override { return "build-model"; }

  virtual QString getDescription() const override
  { return "Builds a random forest model to be used in conflation"; }

  virtual int runSimple(QStringList& args) override
  {
    // This argument is only valid when converting to .rf from training data.
    bool exportArffOnly = false;
    if (args.contains("--export-arff-only"))
    {
      args.removeAll("--export-arff-only");
      exportArffOnly = true;
    }

    if (args.size() == 2 && !exportArffOnly)
    {
      //must be a .arff to .rf conversion
      ArffToRfConverter::convert(args[0], args[1]);
    }
    //must be a conversion to .rf from training data
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
      QStringList inputs;
      for (int i = 0; i < args.size() - 1; i++)
      {
        inputs.append(args.at(i));
      }

      RandomForestModelBuilder::build(inputs, args.last(), exportArffOnly);
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, BuildModelCmd)

}

