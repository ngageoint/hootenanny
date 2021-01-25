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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/rnd/schema/ImplicitTagRawRulesDeriver.h>
#include <hoot/rnd/schema/ImplicitTagRulesDatabaseDeriver.h>
#include <hoot/rnd/io/ImplicitTagRulesSqliteReader.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

class ImplicitTypeTaggingRulesCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::ImplicitTypeTaggingRulesCmd"; }

  ImplicitTypeTaggingRulesCmd()  = default;

  virtual QString getName() const override { return "type-tagger-rules"; }

  virtual QString getDescription() const override
  { return "Creates rules for adding missing type tags to a map"; }

  virtual QString getType() const { return "rnd"; }

  virtual int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    if (args.contains("--create-raw"))
    {
      args.removeAt(args.indexOf("--create-raw"));
      if (args.size() != 3)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw HootException(
          QString("%1 with the --create-raw option takes three parameters.").arg(getName()));
      }

      ImplicitTagRawRulesDeriver rawRulesDeriver;
      rawRulesDeriver.setConfiguration(conf());
      rawRulesDeriver.deriveRawRules(
        args[0].trimmed().split(";"), args[1].trimmed().split(";"), args[2].trimmed());
    }
    else if (args.contains("--create-db"))
    {
      args.removeAt(args.indexOf("--create-db"));
      if (args.size() != 2)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw HootException(
          QString("%1 with the --create-db option takes two parameters.").arg(getName()));
      }

      ImplicitTagRulesDatabaseDeriver rulesDatabaseDeriver;
      rulesDatabaseDeriver.setConfiguration(conf());
      rulesDatabaseDeriver.deriveRulesDatabase(args[0].trimmed(), args[1].trimmed());
    }
    else if (args.contains("--db-stats"))
    {
      args.removeAt(args.indexOf("--db-stats"));
      if (args.size() != 1)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw HootException(
          QString("%1 with the --db-stats option takes one parameter.").arg(getName()));
      }

      ImplicitTagRulesSqliteReader dbReader;
      dbReader.open(args[0].trimmed());
      std::cout << dbReader.getStats();
      dbReader.close();
    }
    else
    {
      LOG_WARN(
        "Must supply one of the option parameters: --create-raw, --create-db, --db-stats");
      std::cout << getHelp() << std::endl << std::endl;
      return 1;
    }

    LOG_STATUS(
      "Implicit type tagging rules generated in " <<
       StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ImplicitTypeTaggingRulesCmd)

}
