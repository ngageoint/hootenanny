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

#include "TagDistribution.h"

// Hoot
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/io/ElementCriterionInputStream.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/CollectionUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

#include <QTextStream>

namespace hoot
{

TagDistribution::TagDistribution() :
_sortByFrequency(true),
_tokenize(false),
_limit(-1),
_total(0),
_taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
  _nonWord.setPattern("[^\\w\\s]");
}

std::map<QString, int> TagDistribution::getTagCounts(const QStringList& inputs)
{
  std::map<QString, int> tagCounts;
  for (int i = 0; i < inputs.size(); i++)
  {
    _countTags(inputs.at(i), tagCounts);
  }
  return tagCounts;
}

QString TagDistribution::_getPercentageStr(const double percentage) const
{
  const QString percentageStr =
    percentage >= 0.01 ? QString::number(percentage * 100, 'g', 3) : "<1";
  return percentageStr;
}

QString TagDistribution::getTagCountsString(const std::map<QString, int>& tagCounts) const
{
  QString buffer;
  QTextStream ts(&buffer);
  QLocale locale(QLocale::English);

  if (tagCounts.empty())
  {
    ts << "No tags with keys: " << _tagKeys.join(",") << " were found." << endl;
  }
  else
  {
    ts << "Total tag count: " << StringUtils::formatLargeNumber(_total) << endl;

    int ctr = 0;
    if (!_sortByFrequency)
    {
      for (std::map<QString, int>::const_iterator itr = tagCounts.begin(); itr != tagCounts.end();
           ++itr)
      {
        const QString tagValue = itr->first;
        const int count = itr->second;
        const double percentageOfTotal = (double)count / (double)_total;
        ts << StringUtils::formatLargeNumber(count) << "\t(" <<
              _getPercentageStr(percentageOfTotal) << "%)\t" << tagValue << endl;

        ctr++;
        if (ctr == _limit)
        {
          break;
        }
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
        ts << StringUtils::formatLargeNumber(count) << "\t(" <<
           _getPercentageStr(percentageOfTotal) << "%)\t" << tagValue << endl;

        ctr++;
        if (ctr == _limit)
        {
          break;
        }
      }
    }
  }

  // See related note towards the end of TagDistributionCmd::runSimple.
  return ts.readAll();
}

void TagDistribution::_countTags(const QString& input, std::map<QString, int>& tagCounts)
{
  long inputTotal = 0;

  std::shared_ptr<PartialOsmMapReader> reader = _getReader(input);

  ElementInputStreamPtr filteredInputStream =
    _getFilteredInputStream(std::dynamic_pointer_cast<ElementInputStream>(reader));

  long elementCtr = 0;
  while (filteredInputStream->hasMoreElements())
  {
    ElementPtr element = filteredInputStream->readNextElement();
    if (element)
    {
      elementCtr++;
      _total++;

      const Tags& tags = element->getTags();
      for (int i = 0; i < _tagKeys.size(); i++)
      {
        const QString tagKey = _tagKeys.at(i);
        if (tags.contains(tagKey))
        {
          QString tagValue = tags.get(tagKey);
          QStringList tagValues;
          if (_tokenize)
          {
            tagValues = tagValue.split(" ");
          }
          else
          {
            tagValues.append(tagValue);
          }

          for (int j = 0; j < tagValues.size(); j++)
          {
            // See wikipedia for a description of what this does, but we're trying to group words
            // that are effectively the same together in the same hash bin.
            // http://en.wikipedia.org/wiki/Unicode_equivalence
            QString tagToken =
              tagValues.at(j)
                .toLower().normalized(QString::NormalizationForm_C).simplified().trimmed();
            LOG_VART(tagToken);
            // remove all the non-word characters (e.g. "(", "-", etc.)
            QString noSymbols = tagToken;
            noSymbols.replace(_nonWord, "");
            LOG_VART(noSymbols);
            // if there is still a word left then use the word w/o symbols.
            // E.g. don't remove "&" if it is all by itself.
            if (!noSymbols.trimmed().isEmpty())
            {
              tagToken = noSymbols;
            }
            LOG_VART(tagToken);

            if (!tagToken.trimmed().isEmpty())
            {
              if (tagCounts.find(tagToken) != tagCounts.end())
              {
                tagCounts[tagToken]++;
              }
              else
              {
                tagCounts[tagToken] = 1;
              }
            }
          }
        }
      }

      // see status logging note in corresponding location in CountCmd::_count
      const long runningTotal = _total + elementCtr;
      if (runningTotal > 0 && runningTotal % (_taskStatusUpdateInterval * 10) == 0)
      {
        PROGRESS_INFO("Processed " << StringUtils::formatLargeNumber(runningTotal) << " elements.");
      }
    }
  }

  if (_total > 0)
  {
    LOG_INFO(
      "Processed " << StringUtils::formatLargeNumber(_total) << " elements from " <<
      FileUtils::toLogFormat(input, 25) << ".");
  }

  LOG_VART(inputTotal);

  reader->finalizePartial();
  reader->close();
  filteredInputStream->close();
}

ElementInputStreamPtr TagDistribution::_getFilteredInputStream(const ElementInputStreamPtr& inputStream) const
{
  ElementInputStreamPtr filteredInputStream;

  if (!_criterionClassName.trimmed().isEmpty())
  {
    ElementCriterionPtr crit = _getCriterion();
    filteredInputStream.reset(new ElementCriterionInputStream(inputStream, crit));
  }
  else
  {
    filteredInputStream = inputStream;
  }

  return filteredInputStream;
}

std::shared_ptr<PartialOsmMapReader> TagDistribution::_getReader(const QString& input) const
{
  std::shared_ptr<PartialOsmMapReader> reader =
    std::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::createReader(input));
  reader->setUseDataSourceIds(true);
  reader->open(input);
  reader->initializePartial();
  return reader;
}

ElementCriterionPtr TagDistribution::_getCriterion() const
{
  ElementCriterionPtr crit;

  try
  {
    crit.reset(
      Factory::getInstance().constructObject<ElementCriterion>(_criterionClassName));
  }
  catch (const boost::bad_any_cast&)
  {
    throw IllegalArgumentException("Invalid criterion: " + _criterionClassName);
  }

  if (ConfigOptions().getElementCriterionNegate())
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

}