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
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/algorithms/rubber-sheet/RubberSheeter.h>
#include <hoot/core/algorithms/rubber-sheet/RubberSheetDeriver.h>
#include <hoot/core/algorithms/rubber-sheet/RubberSheetApplier.h>

namespace hoot
{

class RubberSheetCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::RubberSheetCmd"; }

  RubberSheetCmd() { }

  virtual QString getName() const { return "rubber-sheet"; }

  virtual QString getDescription() const { return "Brings two maps into alignment"; }

  virtual int runSimple(QStringList args)
  {
    if (args.contains("--derive"))
    {
      args.removeAt(args.indexOf("--derive"));
      if (args.size() < 3 || args.size() > 5)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw HootException(
          QString("%1 with the --derive option takes three to five parameters.").arg(getName()));
      }

      bool ref = false;
      if (args[0] == "--ref")
      {
        ref = true;
        args.pop_front();
      }

      QString transform1to2Path;
      if (args.size() == 4)
      {
        transform1to2Path = args[3];
      }
      else if (ref == false)
      {
        throw HootException(QString("You must specify a transform1to2.rs argument if --ref isn't "
          "specified."));
      }

      RubberSheetDeriver().derive(args[0], args[1], args[2], transform1to2Path, ref);
    }
    else if (args.contains("--apply"))
    {
      args.removeAt(args.indexOf("--apply"));
      if (args.size() != 3)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw HootException(
          QString("%1 with the --apply option takes three parameters.").arg(getName()));
      }

      RubberSheetApplier().apply(args[0], args[1], args[2]);
    }
    else
    {
      if (args.size() != 3)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw HootException(QString("%1 takes three parameters.").arg(getName()));
      }

      RubberSheeter().rubberSheet(args[0], args[1], args[2]);
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, RubberSheetCmd)

}

