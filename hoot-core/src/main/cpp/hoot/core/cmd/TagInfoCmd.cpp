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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/schema/TagInfo.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QDir>
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

class TagInfoCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::TagInfoCmd"; }

  TagInfoCmd() = default;

  QString getName() const override { return "tag-info"; }
  QString getDescription() const override { return "Displays tag information for a map"; }

  int runSimple(QStringList& args) override
  {
    if (args.size() < 1)
    {
      cout << getHelp() << endl << endl;
      throw IllegalArgumentException(QString("%1 takes at least one parameter.").arg(getName()));
    }

    int tagValuesPerKeyLimit = INT_MAX;
    if (args.contains("--tag-values-limit"))
    {
      const int limitIndex = args.indexOf("--tag-values-limit");
      bool ok;
      tagValuesPerKeyLimit = args.at(limitIndex + 1).trimmed().toInt(&ok);
      if (!ok)
      {
        throw HootException("Invalid input specified for limit: " +
                            args.at(args.indexOf("--tag-values-limit") + 1));
      }
      args.removeAt(limitIndex + 1);
      args.removeAt(limitIndex);
    }

    QStringList keys;
    if (args.contains("--keys"))
    {
      const int keysIndex = args.indexOf("--keys");
      keys = args.at(keysIndex + 1).trimmed().split(";");
      args.removeAt(keysIndex + 1);
      args.removeAt(keysIndex);
    }

    bool keysOnly = false;
    if (args.contains("--keys-only"))
    {
      keysOnly = true;
      args.removeAt(args.indexOf("--keys-only"));
    }

    bool caseSensitive = true;
    if (args.contains("--case-insensitive"))
    {
      caseSensitive = false;
      args.removeAt(args.indexOf("--case-insensitive"));
    }

    bool exactKeyMatch = true;
    if (args.contains("--partial-key-match"))
    {
      exactKeyMatch = false;
      args.removeAt(args.indexOf("--partial-key-match"));
    }

    bool delimitedTextOutput = false;
    if (args.contains("--delimited-text"))
    {
      if (!keysOnly)
      {
        throw IllegalArgumentException(
          QString("%1 --delimited-text option is only valid when used with --keys-only.")
            .arg(getName()));
      }
      delimitedTextOutput = true;
      args.removeAt(args.indexOf("--delimited-text"));
    }

    bool recursive = false;
    const QStringList inputFilters = _parseRecursiveInputParameter(args, recursive);

    if (args.size() < 1)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at least one parameter. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }

    // Everything left is an input.
    QStringList inputs;
    if (!recursive)
    {
      inputs = args;
    }
    else
    {
      inputs = IoUtils::getSupportedInputsRecursively(args, inputFilters);
    }

    QElapsedTimer timer;
    timer.start();

    LOG_STATUS("Displaying tag information for " << inputs.size() << "inputs...");
    TagInfo tagInfo(
      tagValuesPerKeyLimit, keys, keysOnly, caseSensitive, exactKeyMatch, delimitedTextOutput);
    cout << tagInfo.getInfo(inputs) << endl;
    LOG_STATUS(
      "Tag information collected in " << StringUtils::millisecondsToDhms(timer.elapsed()) <<
      " total.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, TagInfoCmd)

}
