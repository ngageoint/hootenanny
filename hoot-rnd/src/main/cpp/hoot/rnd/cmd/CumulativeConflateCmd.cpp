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
#include <hoot/core/cmd/BoundedCommand.h>
#include <hoot/core/util/Factory.h>
#include <hoot/rnd/conflate/CumulativeConflator2.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

class ConflateCumulativeCmd : public BoundedCommand
{
public:

  static QString className() { return "ConflateCumulativeCmd"; }

  ConflateCumulativeCmd() :
  BoundedCommand()
  {
  }

  QString getName() const override { return "conflate-cumulative"; }
  QString getDescription() const override
  { return "Conflates multiple maps in a cumulative fashion (experimental)"; }
  QString getType() const override { return "rnd"; }

  int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    CumulativeConflator2 conflator;
    conflator.setArgs(_rawArgs);
    LOG_VARD(_rawArgs);

    BoundedCommand::runSimple(args);

    bool reverseInputs = false;
    if (args.contains("--reverse-inputs"))
    {
      reverseInputs = true;
      args.removeAll("--reverse-inputs");
    }
    QString transferTagsInput;
    if (args.contains("--transfer-tags"))
    {
      const int optionNameIndex = args.indexOf("--transfer-tags");
      transferTagsInput = args.at(optionNameIndex + 1).trimmed();
      args.removeAt(optionNameIndex + 1);
      args.removeAt(optionNameIndex);
    }
    bool leaveTransferredTags = false;
    if (args.contains("--leave-transferred-tags"))
    {
      leaveTransferredTags = true;
      args.removeAll("--leave-transferred-tags");
    }
    int maxIterations = -1;
    if (args.contains("--max-iterations"))
    {
      const int optionNameIndex = args.indexOf("--max-iterations");
      bool ok = false;
      maxIterations = args.at(optionNameIndex + 1).trimmed().toInt(&ok);
      if (!ok)
      {
        throw IllegalArgumentException();
      }
      args.removeAt(optionNameIndex + 1);
      args.removeAt(optionNameIndex);
    }
    bool keepIntermediateOutputs = false;
    if (args.contains("--keep-intermediate-outputs"))
    {
      keepIntermediateOutputs = true;
      args.removeAll("--keep-intermediate-outputs");
    }
    QString inputSortScoreType;
    if (args.contains("--sort-inputs"))
    {
      const int optionNameIndex = args.indexOf("--sort-inputs");
      inputSortScoreType = args.at(optionNameIndex + 1).trimmed().toLower();
      args.removeAt(optionNameIndex + 1);
      args.removeAt(optionNameIndex);
    }
    bool sortIncreasing = false;
    if (args.contains("--sort-increasing"))
    {
      sortIncreasing = true;
      args.removeAll("--sort-increasing");
    }

    if (args.size() != 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two input parameters.").arg(getName()));
    }

    QDir input(args[0]);
    if (!input.exists())
    {
      throw IllegalArgumentException("Input directory does not exist.");
    }

    conflator.setReverseInputs(reverseInputs);
    conflator.setTransferTagsInput(transferTagsInput);
    conflator.setLeaveTransferredTags(leaveTransferredTags);
    conflator.setMaxIterations(maxIterations);
    conflator.setKeepIntermediateOutputs(keepIntermediateOutputs);
    if (!inputSortScoreType.isEmpty())
    {
      conflator.setInputSortScoreType(inputSortScoreType);
    }
    conflator.setSortIncreasing(sortIncreasing);
    conflator.conflate(input, args[1]);

    LOG_STATUS(
      "Cumulative conflation ran in " << StringUtils::millisecondsToDhms(timer.elapsed()) <<
      " total.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ConflateCumulativeCmd)

}

