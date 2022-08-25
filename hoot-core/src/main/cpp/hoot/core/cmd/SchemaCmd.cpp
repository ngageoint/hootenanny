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
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/ScriptSchemaTranslator.h>
#include <hoot/core/schema/ScriptSchemaTranslatorFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

using namespace std;

namespace hoot
{

/**
 * @brief The SchemaCmd class runs the schema command.
 */
class SchemaCmd : public BaseCommand
{
public:

  static QString className() { return "SchemaCmd"; }

  SchemaCmd() = default;

  QString getName() const override { return "schema"; }
  QString getDescription() const override
  { return "Generates documentation for the schema in use"; }

  int runSimple(QStringList& args) override
  {
    if (args.empty())
    {
      if (!_getSupportedTextualFormats().contains(ConfigOptions().getTagPrintingFormat()))
        throw IllegalArgumentException("Unsupported textual output format: " + ConfigOptions().getTagPrintingFormat() + ".");

      std::shared_ptr<ScriptSchemaTranslator> schemaPrinter =
        ScriptSchemaTranslatorFactory::getInstance().createTranslator(ConfigOptions().getTagPrintingScript());
      if (!schemaPrinter)
        throw IllegalArgumentException("Unable to find schema printing script: " + ConfigOptions().getTagPrintingScript());
    }
    else if (args.size() == 1)
    {
      QFileInfo outputFileInfo(args[0]);
      const QString outputFormat = outputFileInfo.suffix().toLower();
      if (_getSupportedImageFormats().contains(outputFormat))
        _writeGraphImage(args[0]);
      else
        throw IllegalArgumentException("Unsupported image output format: " + outputFormat + ".");
    }
    else
    {
      if (args.size() > 1)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw IllegalArgumentException(
          QString("%1 takes zero or one parameters. You provided %2: %3")
            .arg(getName())
            .arg(args.size())
            .arg(args.join(",")));
      }
    }

    return 0;
  }

private:

  void _writeGraphImage(const QString& outputPath) const
  {
    QFileInfo outputFileInfo(outputPath);
    const QString outputFormat = outputFileInfo.suffix().toLower();
    const QString dotFilePath = outputFileInfo.absoluteDir().absolutePath() + "/schema.dot";

    const OsmSchema& uut = OsmSchema::getInstance();
    QFile fp(dotFilePath);
    fp.open(QFile::WriteOnly);
    fp.write(uut.toGraphvizString().toUtf8());
    fp.close();

    QString cmd = "neato -T" + outputFormat + " -Gsize=100,100\\! -GK=3 -Gratio=.5 -Gmindist=1";
    cmd += " -Gmclimit=10 -Gnodesep=4 -Granksep=4 -Gsplines=true -Glen=0.5 -Gdpi=100";
    cmd += " -Goverlap=false -Elen=3 -o " + outputPath + " " + dotFilePath;
    const int retval = system(cmd.toStdString().c_str());
    if (retval != 0)
      throw HootException("Failed creating schema graph image. Status: " + QString::number(retval));
  }

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
    formats.append("png");
    formats.append("svg");
    return formats;
  }
};

HOOT_FACTORY_REGISTER(Command, SchemaCmd)

}
