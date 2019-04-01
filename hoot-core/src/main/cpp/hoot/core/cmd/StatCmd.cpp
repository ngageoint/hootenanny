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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/info/SingleStatistic.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/util/Exception.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/ElementVisitorInputStream.h>

namespace hoot
{

class StatCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::StatCmd"; }

  StatCmd() :
  _taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
  {
  }

  virtual QString getName() const { return "stat"; }

  virtual QString getDescription() const
  { return "Calculates a specified statistic for a map"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 2)
    {
      LOG_VAR(args);
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes two parameters.").arg(getName()));
    }

    const QString input = args[0];
    LOG_VART(input);

    const QString visClassName = args[1];
    LOG_VARD(visClassName);

    const double stat = _calcStat(input, visClassName);
    LOG_VART(stat);

    // see note in CountCmd about the preceding endline
    std::cout << std::endl << "Calculated statistic: " << stat << std::endl;

    return 0;
  }

private:

  int _taskStatusUpdateInterval;

  boost::shared_ptr<PartialOsmMapReader> _getReader(const QString input)
  {
    LOG_TRACE("Getting reader...");

    boost::shared_ptr<PartialOsmMapReader> reader =
      boost::dynamic_pointer_cast<PartialOsmMapReader>(
        OsmMapReaderFactory::createReader(input));
    reader->setUseDataSourceIds(true);
    reader->open(input);
    reader->initializePartial();
    return reader;
  }

  ElementVisitorPtr _getStatCollector(const QString visClassName)
  {
    boost::shared_ptr<ElementVisitor> statsCollector;

    try
    {
      statsCollector.reset(
        Factory::getInstance().constructObject<ElementVisitor>(visClassName));
    }
    catch (const boost::bad_any_cast&)
    {
      throw IllegalArgumentException("Invalid visitor: " + visClassName);
    }

    boost::shared_ptr<Configurable> visConfig =
      boost::dynamic_pointer_cast<Configurable>(statsCollector);
    LOG_VART(visConfig.get());
    if (visConfig.get())
    {
      visConfig->setConfiguration(conf());
    }

    return statsCollector;
  }

  double _calcStat(const QString input, const QString visClassName)
  {
    double stat;

    boost::shared_ptr<PartialOsmMapReader> reader = _getReader(input);

    ElementVisitorPtr statCollector = _getStatCollector(visClassName);

    ElementInputStreamPtr filteredInputStream(
      new ElementVisitorInputStream(
        boost::dynamic_pointer_cast<ElementInputStream>(reader), statCollector));

    boost::shared_ptr<SingleStatistic> counter =
      boost::dynamic_pointer_cast<SingleStatistic>(statCollector);
    LOG_VART(counter.get());

    LOG_TRACE("Calculating statistic...");
    long numElementsParsed = 0;
    while (filteredInputStream->hasMoreElements())
    {
      /*ConstElementPtr element = */filteredInputStream->readNextElement();
      numElementsParsed++;

      if (numElementsParsed % _taskStatusUpdateInterval == 0)
      {
        PROGRESS_INFO("Calculated statistic for: " << numElementsParsed << " elements.");
      }
    }

    stat = counter->getStat();

    reader->finalizePartial();
    reader->close();
    filteredInputStream->close();

    return stat;
  }
};

HOOT_FACTORY_REGISTER(Command, StatCmd)

}

