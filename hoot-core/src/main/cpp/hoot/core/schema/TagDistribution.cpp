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
#include <hoot/core/criterion/CriterionUtils.h>
#include <hoot/core/io/ElementCriterionInputStream.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/CollectionUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QTextStream>

namespace hoot
{

TagDistribution::TagDistribution() :
_processAllTagKeys(false),
_countOnlyMatchingElementsInTotal(false),
_sortByFrequency(true),
_tokenize(false),
_limit(-1),
_totalTagsProcessed(0),
_totalElementsProcessed(0),
_taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
  _nonWord.setPattern("[^\\w\\s]");
}

void TagDistribution::setCriteria(QStringList& names)
{
  if (!names.isEmpty())
  {
    for (int i = 0; i < names.size(); i++)
    {
      if (!names.at(i).startsWith(MetadataTags::HootNamespacePrefix()))
      {
        QString className = names[i];
        className.prepend(MetadataTags::HootNamespacePrefix());
        names[i] = className;
      }
    }

    ConfigOptions opts;
    bool isStreamableCrit = false;
    _crit =
      CriterionUtils::constructCriterion(
        names, opts.getElementCriteriaChain(), opts.getElementCriteriaNegate(), isStreamableCrit);
//    if (!isStreamableCrit)
//    {
//      throw IllegalArgumentException("Criteria for TagDistribution must be streamable.");
//    }
  }
}

std::map<QString, int> TagDistribution::getTagCounts(const QStringList& inputs)
{
  LOG_VART(_processAllTagKeys);
  LOG_VART(_tagKeys.size());
  LOG_VART(_sortByFrequency);
  LOG_VART(_tokenize);
  LOG_VART(_limit);

  if (!_processAllTagKeys && _tagKeys.isEmpty())
  {
    throw IllegalArgumentException("No tag keys specified.");
  }

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

  QString msg;
  if (tagCounts.empty())
  {
    if (!_tagKeys.isEmpty())
    {
      msg =
        "No tags with keys: " + _tagKeys.join(",") + " were found out of " +
        StringUtils::formatLargeNumber(_totalTagsProcessed) + " total tags processed.";
      if (_crit)
      {
        msg += " Filtered by: " + _crit->toString() + ".";
      }
      ts << msg << endl;
    }
    else
    {
      ts << "No tags were found." << endl;
    }
  }
  else
  {
    msg =
      "Total elements processed: " +
      StringUtils::formatLargeNumber(_totalElementsProcessed) + ". Total tags processed: " +
      StringUtils::formatLargeNumber(_totalTagsProcessed) + ".";
    if (_crit)
    {
      msg += " Filtered by: " + _crit->toString() + ".";
    }
    ts << msg << endl;

    int ctr = 0;
    if (!_sortByFrequency)
    {
      for (std::map<QString, int>::const_iterator itr = tagCounts.begin(); itr != tagCounts.end();
           ++itr)
      {
        const QString tagValue = itr->first;
        const int count = itr->second;
        const double percentageOfTotal = (double)count / (double)_totalElementsProcessed;
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
        const double percentageOfTotal = (double)count / (double)_totalElementsProcessed;
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
  // Keep a per input total for reporting purposes.
  long inputElementTotal = 0;
  long inputTagsTotal = 0;

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
      // If we're processing any tag or not just counting elements with matching tags, go ahead
      // and this element to the total.
      if (_processAllTagKeys || !_countOnlyMatchingElementsInTotal)
      {
        //LOG_VART(element);
        _totalElementsProcessed++;
      }
      inputElementTotal++;

      const Tags& tags = element->getTags();
      _totalTagsProcessed += tags.size();
      inputTagsTotal+= tags.size();
      if (!_processAllTagKeys)
      {
        assert(!_tagKeys.isEmpty());
        for (int i = 0; i < _tagKeys.size(); i++)
        {
          const QString tagKey = _tagKeys.at(i);
          if (tags.contains(tagKey))
          {
            // We didn't count this element earlier in the total, so do it now.
            if (_countOnlyMatchingElementsInTotal)
            {
              //LOG_VART(element);
              _totalElementsProcessed++;
            }
            _processTagKey(tagKey, tags, tagCounts);
          }
        }
      }
      else
      {
        for (Tags::const_iterator tagItr = tags.begin(); tagItr != tags.end(); ++tagItr)
        {
          _processTagKey(tagItr.key(), tags, tagCounts);
        }
      }

      // See status logging note in corresponding location in CountCmd::_count.
      const long runningTotal = _totalElementsProcessed + elementCtr;
      if (runningTotal > 0 && runningTotal % (_taskStatusUpdateInterval * 10) == 0)
      {
        PROGRESS_INFO("Processed " << StringUtils::formatLargeNumber(runningTotal) << " elements.");
      }
    }
  }

  if (inputElementTotal > 0)
  {
    LOG_INFO(
      "Processed " << StringUtils::formatLargeNumber(inputTagsTotal) << " tags from " <<
      StringUtils::formatLargeNumber(inputElementTotal) << " elements from " <<
      FileUtils::toLogFormat(input, 25) << ".");
  }

  reader->finalizePartial();
  reader->close();
  filteredInputStream->close();
}

void TagDistribution::_processTagKey(
  const QString& tagKey, const Tags& tags, std::map<QString, int>& tagCounts) const
{
  const QString tagValue = tags.get(tagKey);
  QStringList tagValues;
  if (_tokenize)
  {
    tagValues = tagValue.split(" ");
  }
  else
  {
    tagValues.append(tagValue);
  }

  OsmSchema& schema = OsmSchema::getInstance();
  for (int j = 0; j < tagValues.size(); j++)
  {
    // See wikipedia for a description of what this does, but we're trying to group words
    // that are effectively the same together in the same hash bin:
    // http://en.wikipedia.org/wiki/Unicode_equivalence
    QString tagToken =
      tagValues.at(j)
        .toLower().normalized(QString::NormalizationForm_C).simplified().trimmed();
    LOG_VART(tagToken);
    // Remove all the non-word characters (e.g. "(", "-", etc.).
    QString noSymbols = tagToken;
    noSymbols.replace(_nonWord, "");
    LOG_VART(noSymbols);
    // If there is still a word left then use the word w/o symbols. e.g. don't remove "&"
    // if it is all by itself.
    if (!noSymbols.trimmed().isEmpty())
    {
      tagToken = noSymbols;
    }
    LOG_VART(tagToken);

    // Make this a KVP so that we see differences between tags that have the same value but
    // different keys.
    tagToken = schema.toKvp(tagKey, tagToken);

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

ElementInputStreamPtr TagDistribution::_getFilteredInputStream(
  const ElementInputStreamPtr& inputStream) const
{
  ElementInputStreamPtr filteredInputStream;

  if (_crit)
  {
    filteredInputStream.reset(new ElementCriterionInputStream(inputStream, _crit));
  }
  else
  {
    filteredInputStream = inputStream;
  }

  return filteredInputStream;
}

std::shared_ptr<PartialOsmMapReader> TagDistribution::_getReader(const QString& input) const
{
  // See related note in TagInfo::_getInfo.
  if (!OsmMapReaderFactory::hasElementInputStream(input))
  {
    throw IllegalArgumentException("Inputs to TagDistribution must be streamable.");
  }

  std::shared_ptr<PartialOsmMapReader> reader =
    std::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::createReader(input));
  reader->setUseDataSourceIds(true);
  reader->open(input);
  reader->initializePartial();
  return reader;
}

}
