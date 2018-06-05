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
#include <hoot/core/util/Settings.h>
#include <hoot/core/scoring/AttributeCount.h>

// QT
#include <QDir>

using namespace std;

namespace hoot
{

class TagValuesCmd : public BaseCommand
{
public:

  static string className() { return "hoot::TagValuesCmd"; }

  TagValuesCmd() { }

  virtual QString getName() const { return "tag-values"; }

  virtual QString getDescription() const
  { return "Prints tag names and unique values for map data."; }

  virtual int runSimple(QStringList args)
  {
    AttributeCount attrcount;

    QString finalText;

    if (args.size() < 1)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes at least one parameter.").arg(getName()));
    }

    int tagValuePerKeyLimit = INT_MAX;
    if (args.contains("--tag-values-limit"))
    {
      const int limitIndex = args.indexOf("--tag-values-limit");
      LOG_VART(limitIndex);
      LOG_VART(args.at(limitIndex));
      LOG_VART(args.at(limitIndex + 1));
      bool ok;
      tagValuePerKeyLimit = args.at(limitIndex + 1).trimmed().toInt(&ok);
      if (!ok)
      {
        throw HootException("Invalid input specified for limit: " +
                            args.at(args.indexOf("--tag-values-limit") + 1));
      }
      args.removeAt(limitIndex + 1);
      args.removeAt(limitIndex);
      LOG_VART(args);
    }

    finalText += "{\n";

    for (int i = 0; i < args.size(); i++)
    {
      finalText += QString("  \"%1\":{\n").arg(QFileInfo(args[i]).fileName()); // Lazy :-)

      finalText += attrcount.Count(QString(args[i]), tagValuePerKeyLimit) ;
      finalText += "\n  }";

      // Dont add a comma to the last dataset
      if (i != (args.size() - 1))
      {
        finalText += ",\n";
      }
    }
    finalText += "\n}";

    cout << finalText << endl;

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, TagValuesCmd)

}
