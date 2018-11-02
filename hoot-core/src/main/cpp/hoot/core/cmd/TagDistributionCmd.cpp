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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/ElementCriterionInputStream.h>
#include <hoot/core/util/CollectionUtils.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QMap>

namespace hoot
{

class TagDistributionCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::TagDistributionCmd"; }

  TagDistributionCmd() :
  _total(0),
  _taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
  {
  }

  virtual QString getName() const { return "tag-distribution"; }

  virtual QString getDescription() const
  { return "Calculates the distribution of specified tag values for a map"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() < 2 || args.size() > 4)
    {
      LOG_VAR(args);
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes two to four parameters.").arg(getName()));
    }

    bool sortByFrequency = true;
    if (args.contains("--sort-by-value"))
    {
      sortByFrequency = false;
      args.removeAt(args.indexOf("--sort-by-value"));
    }
    LOG_VARD(sortByFrequency);

    const QStringList inputs = args[0].split(";");
    LOG_VART(inputs.size());
    const QString tagKey = args[1];
    QString criterionClassName;
    if (args.size() == 3)
    {
      criterionClassName = args[2];
    }

    std::map<QString, int> tagCounts;
    for (int i = 0; i < inputs.size(); i++)
    {
      _count(inputs.at(i), tagKey, criterionClassName, tagCounts);
    }

    //putting a preceding endline in here since PROGRESS_INFO doesn't clear itself out at the end
    std::cout << std::endl;
    if (tagCounts.size() == 0)
    {
      std::cout << "No tags with key: " << tagKey << " were found." << std::endl;
    }
    else
    {
      _printResults(tagCounts, sortByFrequency);
    }

    return 0;
  }

private:

  long _total;
  int _taskStatusUpdateInterval;

  void _printResults(const std::map<QString, int>& tagCounts, const bool sortByFrequency)
  {
    if (!sortByFrequency)
    {
      for (std::map<QString, int>::const_iterator itr = tagCounts.begin(); itr != tagCounts.end();
           ++itr)
      {
        const QString tagValue = itr->first;
        const int count = itr->second;
        const double percentageOfTotal = (double)count / (double)_total;
        std::cout << tagValue << " : " << QString::number(count) << " (" <<
          QString::number(percentageOfTotal, 'g', 4) << ")" << std::endl;
      }
    }
    else
    {
      std::multimap<int, QString> sortedMap = CollectionUtils::flipMap(tagCounts);
      for (std::multimap<int, QString>::reverse_iterator itr = sortedMap.rbegin();
           itr != sortedMap.rend(); ++itr)
      {
        const QString tagValue = itr->second;
        const int count = itr->first;
        const double percentageOfTotal = (double)count / (double)_total;
        std::cout << tagValue << " : " << QString::number(count) << " (" <<
          QString::number(percentageOfTotal, 'g', 4) << ")" << std::endl;
      }
    }
  }

  void _count(const QString input, const QString tagKey, const QString criterionClassName,
              std::map<QString, int>& tagCounts)
  {
    long inputTotal = 0;

    boost::shared_ptr<PartialOsmMapReader> reader = _getReader(input);

    ElementInputStreamPtr filteredInputStream =
      _getFilteredInputStream(
        boost::dynamic_pointer_cast<ElementInputStream>(reader), criterionClassName);

    LOG_TRACE("Counting...");
    long elementCtr = 0;
    while (filteredInputStream->hasMoreElements())
    {
      ElementPtr element = filteredInputStream->readNextElement();
      elementCtr++;
      _total++;

      const Tags& tags = element->getTags();
      if (tags.contains(tagKey))
      {
        const QString tagValue = tags.get(tagKey);
        if (tagCounts.find(tagValue) != tagCounts.end())
        {
          tagCounts[tagValue]++;
        }
        else
        {
          tagCounts[tagValue] = 1;
        }
      }

      // see status logging note in corresponding location in CountCmd::_count
      const long runningTotal = _total + elementCtr;
      if (runningTotal > 0 && runningTotal % _taskStatusUpdateInterval == 0)
      {
        QString msg = "Processed " + QString::number(runningTotal) + " elements.";
        PROGRESS_INFO(msg);
      }
    }
    LOG_VART(inputTotal);

    reader->finalizePartial();
    reader->close();
    filteredInputStream->close();
  }

  ElementInputStreamPtr _getFilteredInputStream(ElementInputStreamPtr inputStream,
                                                const QString criterionClassName)
  {
    LOG_TRACE("Getting filtered input stream...");

    ElementInputStreamPtr filteredInputStream;

    if (!criterionClassName.trimmed().isEmpty())
    {
      ElementCriterionPtr crit =
        _getCriterion(criterionClassName, ConfigOptions().getElementCriterionNegate());
      filteredInputStream.reset(new ElementCriterionInputStream(inputStream, crit));
    }
    else
    {
      filteredInputStream = inputStream;
    }

    return filteredInputStream;
  }

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
};

HOOT_FACTORY_REGISTER(Command, TagDistributionCmd)

}

