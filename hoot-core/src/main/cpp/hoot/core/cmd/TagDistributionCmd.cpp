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
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TagDistribution.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

class TagDistributionCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::TagDistributionCmd"; }

  TagDistributionCmd() = default;

  QString getName() const override { return "tag-distribution"; }
  QString getDescription() const override
  { return "Calculates the distribution of values for specified tags in a map"; }

  int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    bool typeKeysOnly = false;
    if (args.contains("--types"))
    {
      typeKeysOnly = true;
      args.removeAt(args.indexOf("--types"));
    }

    bool nameKeysOnly = false;
    if (args.contains("--names"))
    {
      nameKeysOnly = true;
      args.removeAt(args.indexOf("--names"));
    }

    bool countOnlyMatchingElementsInTotal = false;
    if (args.contains("--percentage-of-matching"))
    {
      countOnlyMatchingElementsInTotal = true;
      args.removeAt(args.indexOf("--percentage-of-matching"));
    }

    bool sortByFrequency = true;
    if (args.contains("--sort-by-value"))
    {
      sortByFrequency = false;
      args.removeAt(args.indexOf("--sort-by-value"));
    }

    bool tokenize = false;
    if (args.contains("--tokenize"))
    {
      tokenize = true;
      args.removeAt(args.indexOf("--tokenize"));
    }

    int limit = -1;
    if (args.contains("--limit"))
    {
      int limitIndex = args.indexOf("--limit");
      if (args.size() < limitIndex + 1)
      {
        throw HootException(
          QString("No limit value specified with --limit option.").arg(getName()));
      }
      bool ok;
      limit = args.at(limitIndex + 1).toInt(&ok);
      if (!ok)
      {
        throw IllegalArgumentException(
          QString("Invalid limit value specified: " +  args.at(limitIndex + 1)).arg(getName()));
      }
      args.removeAt(limitIndex + 1);
      args.removeAt(limitIndex);
    }

    bool recursive = false;
    const QStringList inputFilters = _parseRecursiveInputParameter(args, recursive);

    if (typeKeysOnly && nameKeysOnly)
    {
      throw IllegalArgumentException(
        "Only either --names or --types may be specified as an option to " + getName() + ".");
    }

    if (!nameKeysOnly && !typeKeysOnly && (args.size() < 2 || args.size() > 3))
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes two to three parameters when --names is not specified.").arg(getName()));
    }
    else if ((nameKeysOnly || typeKeysOnly) && (args.size() < 1 || args.size() > 2))
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes one to two parameters when --names is specified.").arg(getName()));
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
    QStringList tagKeys;
    if (typeKeysOnly)
    {
      tagKeys = OsmSchema::getInstance().getAllTypeKeys().toList();
    }
    else if (nameKeysOnly)
    {
      tagKeys = Tags::getNameKeys();
    }
    else
    {
      tagKeys = args[1].split(";", QString::SkipEmptyParts);
    }
    QStringList criteriaClassNames;
    if (args.size() == 3)
    {
      criteriaClassNames = args[2].trimmed().split(";");
    }

    LOG_STATUS("Calculating tag distribution for " << inputs.size() << " input(s)...");

    TagDistribution tagDist;
    tagDist.setCriteria(criteriaClassNames);
    tagDist.setLimit(limit);
    tagDist.setSortByFrequency(sortByFrequency);
    tagDist.setTagKeys(tagKeys);
    tagDist.setProcessAllTagKeys(tagKeys.empty());
    tagDist.setCountOnlyMatchingElementsInTotal(countOnlyMatchingElementsInTotal);
    tagDist.setTokenize(tokenize);

    std::cout << tagDist.getTagCountsString(tagDist.getTagCounts(inputs));

    LOG_STATUS(
      "Tag distribution calculated in " << StringUtils::millisecondsToDhms(timer.elapsed()) <<
      " total.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, TagDistributionCmd)

}

