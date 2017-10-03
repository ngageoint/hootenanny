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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/rnd/schema/PoiImplicitTagRulesDeriver.h>
#include <hoot/rnd/io/ImplicitTagRulesJsonWriter.h>
#include <hoot/rnd/io/ImplicitTagRulesSqliteWriter.h>

namespace hoot
{

/**
 *
 */
class DeriveImplicitTagRulesCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::DeriveImplicitTagRulesCmd"; }

  virtual QString getName() const { return "derive-implicit-tag-rules"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 4)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes four parameters.").arg(getName()));
    }

    bool ok = false;
    const int minOccurances = args[1].toInt(&ok);
    if (!ok || minOccurances < 1)
    {
      throw IllegalArgumentException(
        "Invalid value for the minimum number of type occurrances allowed: " + args[1]);
    }

    QMap<QString, QMap<QString, long> > rules =
      PoiImplicitTagRulesDeriver().deriveRules(
        args[0].trimmed().split(";"), args[1].trimmed().split(";"), minOccurances);

    const QStringList outputs = args[3].trimmed().split(";");
    for (int i = 0; i < outputs.size(); i++)
    {
      if (outputs.at(i).trimmed().endsWith(".json")) //TODO: cleanup
      {
        ImplicitTagRulesJsonWriter rulesWriter;
        rulesWriter.open(outputs.at(i));
        rulesWriter.write(rules);
        rulesWriter.close();
      }
      else
      {
        ImplicitTagRulesSqliteWriter rulesWriter;
        rulesWriter.open(outputs.at(i));
        rulesWriter.write(rules);
        rulesWriter.close();
      }
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, DeriveImplicitTagRulesCmd)

}
