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
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/schema/ScriptSchemaTranslator.h>
#include <hoot/core/schema/ScriptSchemaTranslatorFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

using namespace std;

namespace hoot
{

/**
 * @brief The SchemaCmd class runs the schema command.
 *
 * Note that no command line tests exists for this class since its code is exercised when the
 * documentation is built.
 *
 * neato -Tsvg -Gsize=100,100\! -GK=3 -Gratio=.5 -Gmindist=1 -Gmclimit=10 -Gnodesep=4 -Granksep=4 \
      -Gsplines=true -Glen=0.5 -Gdpi=100 -Goverlap=false -Elen=3 -o tmp/schema.png tmp/schema.dot
 */
class SchemaCmd : public BaseCommand
{
public:

  static QString className() { return "SchemaCmd"; }

  SchemaCmd() = default;

  QString getName() const override { return "schema"; }
  QString getDescription() const override
  { return "Generates documentation for the tag schema in use"; }

  int runSimple(QStringList& args) override
  {
    if (!args.empty())
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes one parameter. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }

    const QString outputFormat = QFileInfo(args[0]).suffix().toLower();
    if (_getSupportedTextualFormats.contains(outputFormat))
    {
      std::shared_ptr<ScriptSchemaTranslator> schemaPrinter =
        ScriptSchemaTranslatorFactory::getInstance().createTranslator(
          ConfigOptions().getTagPrintingScript());
      if (!schemaPrinter)
      {
        throw IllegalArgumentException(
          "Unable to find printing script: " + ConfigOptions().getTagPrintingScript());
      }
    }
    else if (_getSupportedImageFormats.contains(outputFormat))
    {
      // TODO
    }
    else
    {
      // TODO
    }

    return 0;
  }

private:

  QStringList _getSupportedTextualFormats() const
  {
    QStringList formats;
    formats.append("asciidoc");
    formats.append("csv");
    formats.append("html");
    return formats;
  }

  QStringList _getSupportedImageFormats() const
  {
    QStringList formats;
    formats.append("svg");
    return formats;
  }
};

HOOT_FACTORY_REGISTER(Command, SchemaCmd)

}
