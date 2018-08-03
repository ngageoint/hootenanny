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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/visitors/SingleStatistic.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/util/Exception.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

class StatCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::StatCmd"; }

  StatCmd() { }

  virtual QString getName() const { return "stat"; }

  virtual QString getDescription() const
  { return "Calculates a statistic on features"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 2)
    {
      LOG_VAR(args);
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes two parameters.").arg(getName()));
    }

    const QStringList inputs = args[0].split(";");
    LOG_VART(inputs.size());
    OsmMapPtr map(new OsmMap());
    _readInputs(inputs, map);

    LOG_VARD(args[1]);
    _applyOperator(args[1], map);

    return 0;
  }

private:

  void _readInputs(const QStringList inputs, OsmMapPtr map)
  {
    for (int i = 0; i < inputs.size(); i++)
    {
      const QString input = inputs.at(i);
      boost::shared_ptr<OsmMapReader> reader =
        OsmMapReaderFactory::getInstance().createReader(input, true, Status::Unknown1);
      reader->open(input);
      reader->read(map);
    }
  }

  void _applyOperator(const QString visClassName, OsmMapPtr map)
  {
    double total = 0;

    //filter the elements

    boost::shared_ptr<ConstElementVisitor> statsCollector;
    boost::shared_ptr<SingleStatistic> singleStat;
    try
    {
      statsCollector.reset(
        Factory::getInstance().constructObject<ConstElementVisitor>(visClassName));
      singleStat = boost::dynamic_pointer_cast<SingleStatistic>(statsCollector);
    }
    catch (const boost::bad_any_cast&)
    {
      throw IllegalArgumentException("Invalid visitor: " + visClassName);
    }
    LOG_VART(statsCollector.get());
    LOG_VART(singleStat.get());

    boost::shared_ptr<Configurable> visConfig =
      boost::dynamic_pointer_cast<Configurable>(statsCollector);
    LOG_VART(visConfig.get());
    if (visConfig.get())
    {
      visConfig->setConfiguration(conf());
    }

    LOG_TRACE("Using visitor with SingleStatistic...");
    map->visitRo(*statsCollector);
    total = singleStat->getStat();
    LOG_VART(total);

    std::cout << "Total: " <<
      total/*QString::number(total, 'f', ConfigOptions().getWriterPrecision())*/ << std::endl;
  }
};

HOOT_FACTORY_REGISTER(Command, StatCmd)

}

