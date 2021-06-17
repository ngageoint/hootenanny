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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/info/ElementCounter.h>

namespace hoot
{

class CountCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::CountCmd"; }

  CountCmd() = default;

  QString getName() const override { return "count"; }
  QString getDescription() const override
  { return "Counts the number of features in a map that meet specified criteria"; }

  int runSimple(QStringList& args) override
  {
    bool countFeaturesOnly = true;
    if (args.contains("--all-elements"))
    {
      countFeaturesOnly = false;
      args.removeAt(args.indexOf("--all-elements"));
    }

    bool recursive = false;
    const QStringList inputFilters = _parseRecursiveInputParameter(args, recursive);

    if (args.size() < 1 || args.size() > 2)
    {
      LOG_VAR(args);
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes one or two parameters.").arg(getName()));
    }

    QStringList inputs;
    if (!recursive)
    {
      inputs = args[0].trimmed().split(";");
    }
    else
    {
      inputs = IoUtils::getSupportedInputsRecursively(args[0].trimmed().split(";"), inputFilters);
    }

    QStringList criteriaClassNames;
    if (args.size() > 1)
    {
      criteriaClassNames = args[1].trimmed().split(";");
    }
    LOG_VARD(criteriaClassNames);

    ElementCounter counter;
    counter.setCountFeaturesOnly(countFeaturesOnly);
    counter.setCriteria(criteriaClassNames);
    const long totalCount = counter.count(inputs);

    // putting a preceding endline in here since PROGRESS_INFO doesn't clear itself out at the end
    QString displayStr = "Total count ";
    if (!criteriaClassNames.isEmpty())
    {
      displayStr += "(" + criteriaClassNames.join(";") + ")";
    }
    displayStr += ": " + StringUtils::formatLargeNumber(totalCount);
    std::cout << std::endl << displayStr << std::endl;

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, CountCmd)

}
