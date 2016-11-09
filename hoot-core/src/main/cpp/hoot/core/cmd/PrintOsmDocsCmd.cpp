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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Boost
#include <boost/shared_ptr.hpp>

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/ScriptTranslator.h>
#include <hoot/core/io/ScriptTranslatorFactory.h>
#include <hoot/core/util/ConfigOptions.h>


namespace hoot
{

class PrintOsmDocsCmd : public BaseCommand
{
public:

  static string className() { return "hoot::PrintOsmDocsCmd"; }

  PrintOsmDocsCmd() {}

  virtual QString getName() const { return "print-tags"; }

  virtual int runSimple(QStringList args)
  {
    QString printScript(ConfigOptions().getTagPrintingScript());

    auto_ptr<ScriptTranslator> translator;

    if (args.size() == 1)
    {
      printScript = args[0];
    }
    else if (args.size() > 1)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes one optional parameter.").arg(getName()));
    }

    // Great bit of code taken from TranslatedTagDifferencer.cpp
    shared_ptr<ScriptTranslator> uut(ScriptTranslatorFactory::getInstance().createTranslator(printScript));

    if (!uut)
    {
      throw HootException("Unable to find a valid translation format for: " + printScript);
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, PrintOsmDocsCmd)

}
