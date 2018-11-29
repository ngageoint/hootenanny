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
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Exception.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/visitors/FeatureCountVisitor.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/ConstElementCriterionVisitorInputStream.h>
#include <hoot/core/io/ConstElementVisitorInputStream.h>

namespace hoot
{

class CountCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::CountCmd"; }

  CountCmd() :
  _total(0),
  _taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
  {
  }

  virtual QString getName() const { return "count"; }

  virtual QString getDescription() const
  { return "Counts the number of features in a map that meet a criterion"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() < 1 || args.size() > 3)
    {
      LOG_VAR(args);
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes one to three parameters.").arg(getName()));
    }

    bool countFeaturesOnly = true;
    if (args.contains("--all-elements"))
    {
      countFeaturesOnly = false;
      args.removeAt(args.indexOf("--all-elements"));
      LOG_VART(args);
    }
    LOG_VARD(countFeaturesOnly);

    const QStringList inputs = args[0].split(";");
    LOG_VART(inputs.size());

    QString criterionClassName = "";
    if (args.size() > 1)
    {
      criterionClassName = args[1];
    }
    LOG_VARD(criterionClassName);

    for (int i = 0; i < inputs.size(); i++)
    {
      _total += _count(inputs.at(i), countFeaturesOnly, criterionClassName);
    }
    LOG_VART(_total);

    //putting a preceding endline in here since PROGRESS_INFO doesn't clear itself out at the end
    std::cout << std::endl << "Total: " << _total << std::endl;

    return 0;
  }

private:

  long _total;
  int _taskStatusUpdateInterval;

  boost::shared_ptr<PartialOsmMapReader> _getReader(const QString input)
  {
    LOG_TRACE("Getting reader...");

    boost::shared_ptr<PartialOsmMapReader> reader =
      boost::dynamic_pointer_cast<PartialOsmMapReader>(
        OsmMapReaderFactory::getInstance().createReader(input));
    reader->setUseDataSourceIds(true);
    reader->open(input);
    reader->initializePartial();
    return reader;
  }

  ElementCriterionPtr _getCriterion(const QString criterionClassName, const bool negate)
  {
    LOG_TRACE("Getting criterion...");

    ElementCriterionPtr crit;

    try
    {
      crit.reset(
        Factory::getInstance().constructObject<ElementCriterion>(criterionClassName));
    }
    catch (const boost::bad_any_cast&)
    {
      throw IllegalArgumentException("Invalid criterion: " + criterionClassName);
    }

    if (negate)
    {
      crit.reset(new NotCriterion(crit));
    }
    LOG_VART(crit.get());

    boost::shared_ptr<Configurable> critConfig;
    if (crit.get())
    {
      critConfig = boost::dynamic_pointer_cast<Configurable>(crit);
    }
    LOG_VART(critConfig.get());
    if (critConfig.get())
    {
      critConfig->setConfiguration(conf());
    }

    return crit;
  }

  ElementInputStreamPtr _getFilteredInputStream(ElementInputStreamPtr inputStream,
                                                const QString criterionClassName,
                                                ConstElementVisitorPtr countVis)
  {
    LOG_TRACE("Getting filtered input stream...");

    ElementInputStreamPtr filteredInputStream;

    LOG_TRACE("Creating stream...");
    if (!criterionClassName.trimmed().isEmpty())
    {
      ElementCriterionPtr crit =
        _getCriterion(criterionClassName, ConfigOptions().getElementCriterionNegate());
      filteredInputStream.reset(
        new ConstElementCriterionVisitorInputStream(inputStream, crit, countVis));
    }
    else
    {
      filteredInputStream.reset(new ConstElementVisitorInputStream(inputStream, countVis));
    }

    return filteredInputStream;
  }

  ConstElementVisitorPtr _getCountVis(const bool countFeaturesOnly)
  {
    ConstElementVisitorPtr countVis;
    if (countFeaturesOnly)
    {
      countVis.reset(new FeatureCountVisitor());
    }
    else
    {
      countVis.reset(new ElementCountVisitor());
    }
    return countVis;
  }

  long _count(const QString input, const bool countFeaturesOnly, const QString criterionClassName)
  {
    long inputTotal = 0;

    boost::shared_ptr<PartialOsmMapReader> reader = _getReader(input);

    ConstElementVisitorPtr countVis = _getCountVis(countFeaturesOnly);

    ElementInputStreamPtr filteredInputStream =
      _getFilteredInputStream(
        boost::dynamic_pointer_cast<ElementInputStream>(reader),
        criterionClassName,
        countVis);

    boost::shared_ptr<SingleStatistic> counter =
      boost::dynamic_pointer_cast<SingleStatistic>(countVis);
    LOG_VART(counter.get());

    LOG_TRACE("Counting...");
    while (filteredInputStream->hasMoreElements())
    {
      /*ConstElementPtr element = */filteredInputStream->readNextElement();
      inputTotal = (int)counter->getStat();

      // It would be nice if we could display information on the total elements processed, as well
      // as the ones counted, but since the element stream is already filtered by this point would
      // take some extra work.
      const long runningTotal = _total + inputTotal;
      if (runningTotal > 0 && runningTotal % _taskStatusUpdateInterval == 0)
      {
        QString msg = "Counted " + QString::number(runningTotal);
        if (countFeaturesOnly)
        {
          msg += " features.";
        }
        else
        {
          msg += " elements.";
        }
        PROGRESS_INFO(msg);
      }
    }
    LOG_VART(inputTotal);

    reader->finalizePartial();
    reader->close();
    filteredInputStream->close();

    return inputTotal;
  }
};

HOOT_FACTORY_REGISTER(Command, CountCmd)

}
