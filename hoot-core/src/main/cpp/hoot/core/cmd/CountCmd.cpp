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
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/visitors/FeatureCountVisitor.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/ElementCriterionVisitorInputStream.h>
#include <hoot/core/io/ElementVisitorInputStream.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/visitors/FilteredVisitor.h>

// Qt
#include <QFileInfo>
#include <QElapsedTimer>

namespace hoot
{

class CountCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::CountCmd"; }

  CountCmd() :
  _total(0),
  _taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
  {
  }

  QString getName() const override { return "count"; }

  QString getDescription() const override
  { return "Counts the number of features in a map that meet specified criteria"; }

  int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

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

    const QStringList inputs = args[0].trimmed().split(";");
    LOG_VART(inputs.size());
    for (int i = 0; i < inputs.size(); i++)
    {
      const QString input = inputs.at(i);
      QFileInfo fileInfo(input);
      if (!fileInfo.exists())
      {
        throw IllegalArgumentException("Input file does not exist: " + input);
      }
    }

    QString criterionClassName = "";
    if (args.size() > 1)
    {
      criterionClassName = args[1].trimmed();
    }
    LOG_VARD(criterionClassName);

    // test crit here to see if I/O can be streamed or not; if it requires a map, then it can't be
    // streamed
    bool isStreamableCrit = false;
    ElementCriterionPtr crit =
      _getCriterion(
        criterionClassName, ConfigOptions().getElementCriterionNegate(), isStreamableCrit);

    const QString dataType = countFeaturesOnly ? "features" : "elements";
    if (isStreamableCrit)
    {
      for (int i = 0; i < inputs.size(); i++)
      {
        LOG_INFO(
          "Counting " << dataType << " satisfying " << criterionClassName << " from ..." <<
          inputs.at(i).right(25) << "...");
        _total += _countStreaming(inputs.at(i), countFeaturesOnly, crit);
      }
    }
    else
    {
      _total += _countMemoryBound(inputs, countFeaturesOnly, crit);
    }

    LOG_STATUS(
      "Features counted in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    //putting a preceding endline in here since PROGRESS_INFO doesn't clear itself out at the end
    QString displayStr = "Total count ";
    if (!criterionClassName.isEmpty())
    {
      displayStr += "(" + criterionClassName + ")";
    }
    displayStr += ": " + StringUtils::formatLargeNumber(_total);
    std::cout << std::endl << displayStr << std::endl;

    return 0;
  }

private:

  long _total;
  int _taskStatusUpdateInterval;

  std::shared_ptr<PartialOsmMapReader> _getStreamingReader(const QString& input)
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

  ElementCriterionPtr _getCriterion(const QString& criterionClassName, const bool negate,
                                    bool& isStreamable)
  {
    LOG_TRACE("Getting criterion: " << criterionClassName << "...");

    if (criterionClassName.isEmpty())
    {
      return ElementCriterionPtr();
    }

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

    OsmMapConsumer* omc = dynamic_cast<OsmMapConsumer*>(crit.get());
    if (omc)
    {
      isStreamable = false;
    }
    else
    {
      isStreamable = true;
    }

    if (negate)
    {
      crit.reset(new NotCriterion(crit));
    }
    LOG_VART(crit.get());

    std::shared_ptr<Configurable> critConfig;
    if (crit.get())
    {
      critConfig = std::dynamic_pointer_cast<Configurable>(crit);
    }
    LOG_VART(critConfig.get());
    if (critConfig.get())
    {
      critConfig->setConfiguration(conf());
    }

    return crit;
  }

  ElementInputStreamPtr _getFilteredInputStream(ElementInputStreamPtr inputStream,
                                                const ElementCriterionPtr& criterion,
                                                ConstElementVisitorPtr countVis)
  {
    LOG_TRACE("Getting filtered input stream...");
    if (criterion)
    {
      LOG_VARD(criterion->toString());
    }

    ElementInputStreamPtr filteredInputStream;

    LOG_TRACE("Creating stream...");
    if (criterion)
    {
      filteredInputStream.reset(
        new ElementCriterionVisitorInputStream(inputStream, criterion, countVis));
    }
    else
    {
      filteredInputStream.reset(new ElementVisitorInputStream(inputStream, countVis));
    }

    return filteredInputStream;
  }

  ConstElementVisitorPtr _getCountVis(const bool countFeaturesOnly)
  {
    LOG_TRACE("Getting count vis...");

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

  long _countMemoryBound(const QStringList& inputs, const bool countFeaturesOnly,
                         const ElementCriterionPtr& criterion)
  {
    OsmMapPtr map(new OsmMap());
    IoUtils::loadMaps(map, inputs, true);

    OsmMapConsumer* omc = dynamic_cast<OsmMapConsumer*>(criterion.get());
    if (omc)
    {
      omc->setOsmMap(map.get());
    }

    ConstElementVisitorPtr countVis = _getCountVis(countFeaturesOnly);
    ConstElementVisitorPtr vis;
    if (criterion)
    {
      vis.reset(new FilteredVisitor(criterion, countVis));
    }
    else
    {
      vis = countVis;
    }
    map->visitRo(*vis);

    std::shared_ptr<SingleStatistic> counter =
      std::dynamic_pointer_cast<SingleStatistic>(countVis);
    return (long)counter->getStat();
  }

  long _countStreaming(const QString& input, const bool countFeaturesOnly,
                       const ElementCriterionPtr& criterion)
  {
    long inputTotal = 0;

    std::shared_ptr<PartialOsmMapReader> reader = _getStreamingReader(input);

    ConstElementVisitorPtr countVis = _getCountVis(countFeaturesOnly);

    ElementInputStreamPtr filteredInputStream =
      _getFilteredInputStream(
        std::dynamic_pointer_cast<ElementInputStream>(reader),
        criterion,
        countVis);

    std::shared_ptr<SingleStatistic> counter =
      std::dynamic_pointer_cast<SingleStatistic>(countVis);
    LOG_VART(counter.get());

    while (filteredInputStream->hasMoreElements())
    {
      /*ConstElementPtr element = */filteredInputStream->readNextElement();
      inputTotal = (int)counter->getStat();

      // It would be nice if we could display information on the total elements processed, as well
      // as the ones counted, but since the element stream is already filtered by this point that
      // would take some extra work.
      const long runningTotal = _total + inputTotal;
      if (runningTotal > 0 && runningTotal % (_taskStatusUpdateInterval * 10) == 0)
      {
        QString msg = "Counted " + StringUtils::formatLargeNumber(runningTotal);
        if (countFeaturesOnly)
        {
          msg += " features";
        }
        else
        {
          msg += " elements";
        }
        msg += " total.";
        // TODO: We could do a sliding interval here, like we do for poi/poly match counting. Would
        // help give better status for datasets with sparser number of features satisfying the crit.
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
