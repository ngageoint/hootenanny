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
#include <hoot/core/schema/ScriptSchemaTranslator.h>
#include <hoot/core/schema/ScriptSchemaTranslatorFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

using namespace std;

namespace hoot
{

class TagSchemaCmd : public BaseCommand
{
public:

  static string className() { return "hoot::TagSchemaCmd"; }

  TagSchemaCmd() {}

  virtual QString getName() const { return "schema"; }

  virtual QString getDescription() const { return "Displays the tag schema that Hootenanny uses"; }

  virtual int runSimple(QStringList args)
  {
    QString printScript(ConfigOptions().getTagPrintingScript());

    if (args.size() == 1)
    {
      printScript = args[0];
    }
    else if (args.size() > 1)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes one optional parameter.").arg(getName()));
    }

    // Great bit of code taken from TranslatedTagDifferencer
    std::shared_ptr<ScriptSchemaTranslator> schemaPrinter(
      ScriptSchemaTranslatorFactory::getInstance().createTranslator(printScript));

    if (!schemaPrinter)
    {
      throw HootException("Unable to find a valid translation format for: " + printScript);
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, TagSchemaCmd)

}
