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
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/info/SingleStatistic.h>
#include <hoot/core/info/NumericStatistic.h>
#include <hoot/core/io/ElementVisitorInputStream.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/ElementVisitor.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

class StatCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::StatCmd"; }

  StatCmd() :
  _taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
  {
  }

  QString getName() const override { return "stat"; }

  QString getDescription() const override
  { return "Calculates a single statistic for a map"; }

  int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    if (args.size() < 2 || args.size() > 3)
    {
      LOG_VAR(args);
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes two or three parameters.").arg(getName()));
    }

    const QString input = args[0].trimmed();
    LOG_VART(input);

    const QString visClassName = args[1].trimmed();
    LOG_VARD(visClassName);

    QString statType = "total";
    if (args.size() == 3)
    {
      statType = args[2].trimmed();
    }
    LOG_VARD(statType);
    if (!_isValidStatType(statType))
    {
      throw IllegalArgumentException("Invalid statistic type: " + statType);
    }

    LOG_INFO(
      "Calculating statistic of type: " << statType << ", with visitor: " << visClassName <<
      ", for input: " << FileUtils::toLogFormat(input, 25) << "...")
    const double stat = _calcStat(input, visClassName, statType);
    LOG_VART(stat);

    // see note in CountCmd about the preceding endline
    std::cout << std::endl << "Calculated statistic: " <<
                 QString::number(stat, 'g', 3) << std::endl;

    LOG_STATUS(
      "Statistic calculated in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }

private:

  int _taskStatusUpdateInterval;

  bool _isValidStatType(const QString& statType) const
  {
    return statType == "total" || statType == "min" || statType == "max" || statType == "average";
  }

  std::shared_ptr<PartialOsmMapReader> _getReader(const QString& input)
  {
    LOG_TRACE("Getting reader...");

    std::shared_ptr<PartialOsmMapReader> reader =
      std::dynamic_pointer_cast<PartialOsmMapReader>(
        OsmMapReaderFactory::createReader(input));
    reader->setUseDataSourceIds(true);
    reader->open(input);
    reader->initializePartial();
    return reader;
  }

  ElementVisitorPtr _getStatCollector(const QString& visClassName)
  {
    std::shared_ptr<ElementVisitor> statsCollector;

    try
    {
      statsCollector.reset(Factory::getInstance().constructObject<ElementVisitor>(visClassName));
    }
    catch (const boost::bad_any_cast&)
    {
      throw IllegalArgumentException("Invalid visitor: " + visClassName);
    }

    std::shared_ptr<Configurable> visConfig =
      std::dynamic_pointer_cast<Configurable>(statsCollector);
    LOG_VART(visConfig.get());
    if (visConfig.get())
    {
      visConfig->setConfiguration(conf());
    }

    return statsCollector;
  }

  double _calcStat(const QString& input, const QString& visClassName, const QString& statType)
  {
    double stat;

    std::shared_ptr<PartialOsmMapReader> reader = _getReader(input);

    ElementVisitorPtr statCollector = _getStatCollector(visClassName);

    ElementInputStreamPtr filteredInputStream(
      new ElementVisitorInputStream(
        std::dynamic_pointer_cast<ElementInputStream>(reader), statCollector));

    std::shared_ptr<SingleStatistic> singleStatCtr =
      std::dynamic_pointer_cast<SingleStatistic>(statCollector);
    LOG_VART(singleStatCtr.get());
    if (!singleStatCtr.get())
    {
      throw IllegalArgumentException(
        "Visitors passed to the stat command must support the SingleStatistic interface.");
    }
    std::shared_ptr<NumericStatistic> numericStatCtr;
    if (statType != "total")
    {
      numericStatCtr = std::dynamic_pointer_cast<NumericStatistic>(singleStatCtr);
      if (!numericStatCtr.get())
      {
        throw IllegalArgumentException(
          "Visitors passed to the stat command with a statistic type other than \"total\" must "
           "support the NumericStatistic interface.");
      }
    }

    LOG_TRACE("Calculating statistic...");
    long numElementsParsed = 0;
    while (filteredInputStream->hasMoreElements())
    {
      /*ConstElementPtr element = */filteredInputStream->readNextElement();
      numElementsParsed++;

      if (numElementsParsed % (_taskStatusUpdateInterval * 10) == 0)
      {
        PROGRESS_INFO(
          "Calculated statistic for: " << StringUtils::formatLargeNumber(numElementsParsed) <<
          " elements.");
      }
    }

    if (numericStatCtr)
    {
      if (statType == "min")
      {
        stat = numericStatCtr->getMin();
      }
      else if (statType == "max")
      {
        stat = numericStatCtr->getMax();
      }
      else if (statType == "average")
      {
        stat = numericStatCtr->getAverage();
      }
      else
      {
        // won't get here
        throw HootException("");
      }
    }
    else
    {
      stat = singleStatCtr->getStat();
    }

    reader->finalizePartial();
    reader->close();
    filteredInputStream->close();

    return stat;
  }
};

HOOT_FACTORY_REGISTER(Command, StatCmd)

}

