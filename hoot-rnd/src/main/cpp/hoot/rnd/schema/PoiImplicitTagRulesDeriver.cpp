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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiImplicitTagRulesDeriver.h"

// hoot
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/algorithms/string/StringTokenizer.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/rnd/io/ImplicitTagRulesWriter.h>
#include <hoot/rnd/io/ImplicitTagRulesWriterFactory.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QStringBuilder>

namespace hoot
{

PoiImplicitTagRulesDeriver::PoiImplicitTagRulesDeriver() :
_avgTagsPerRule(0),
_avgWordsPerRule(0),
_statusUpdateInterval(ConfigOptions().getApidbBulkInserterFileOutputStatusUpdateInterval())
{
}

QStringList PoiImplicitTagRulesDeriver::_getPoiKvps(const Tags& tags) const
{
  LOG_TRACE("Retrieving POI kvps...");

  QStringList poiKvps;
  for (Tags::const_iterator tagItr = tags.begin(); tagItr != tags.end(); ++tagItr)
  {
    const QString kvp = tagItr.key() % "=" % tagItr.value();
    LOG_VART(kvp);
    LOG_VART(OsmSchema::getInstance().getCategories(kvp).intersects(OsmSchemaCategory::poi()));
    if (kvp != QLatin1String("poi=yes") &&
        OsmSchema::getInstance().getCategories(kvp).intersects(OsmSchemaCategory::poi()))
    {
      poiKvps.append(kvp);
    }
  }
  return poiKvps;
}

void PoiImplicitTagRulesDeriver::_updateForNewWord(QString word, const QString kvp)
{
  LOG_TRACE("Updating word: " << word << " with kvp: " << kvp << "...");

  const QString lowerCaseWord = word.toLower();
  if (_wordCaseMappings.contains(lowerCaseWord))
  {
    word = _wordCaseMappings[lowerCaseWord];
  }
  else
  {
    _wordCaseMappings[lowerCaseWord] = word;
  }

  const QString wordKvp = word % ";" % kvp;
  LOG_VART(wordKvp);
  if (!_wordKvpsToOccuranceCounts.contains(wordKvp))
  {
    _wordKvpsToOccuranceCounts[wordKvp] = 1;
  }
  else
  {
    _wordKvpsToOccuranceCounts[wordKvp]++;
  }
  LOG_VART( _wordKvpsToOccuranceCounts[wordKvp]);

  const QStringList kvpParts = kvp.split("=");
  const QString kvpKey = kvpParts[0];
  const QString kvpVal = kvpParts[1];
  const QString wordKvpKey = word % ";" % kvpKey;
  LOG_VART(wordKvpKey);
  if (!_wordTagKeysToTagValues.contains(wordKvpKey))
  {
    QStringList valsList;
    valsList.append(kvpVal);
    _wordTagKeysToTagValues[wordKvpKey] = valsList;
  }
  else
  {
    _wordTagKeysToTagValues[wordKvpKey].append(kvpVal);
  }
  LOG_VART(_wordTagKeysToTagValues[wordKvpKey]);
}

void PoiImplicitTagRulesDeriver::deriveRules(const QStringList inputs, const QStringList outputs,
                                             const QStringList typeKeys,
                                             const int minOccurancesThreshold)
{
  LOG_INFO(
    "Deriving POI implicit tag rules for inputs: " << inputs << " type keys: " << typeKeys <<
    " with minimum occurance threshold: " << minOccurancesThreshold <<
    ".  Writing to outputs: " << outputs << "...");

  QStringList typeKeysAllowed;
  for (int i = 0; i < typeKeys.size(); i++)
  {
    typeKeysAllowed.append(typeKeys.at(i).toLower());
  }

  long poiCount = 0;
  long nodeCount = 0;
  for (int i = 0; i < inputs.size(); i++)
  {
    boost::shared_ptr<PartialOsmMapReader> inputReader =
      boost::dynamic_pointer_cast<PartialOsmMapReader>(
        OsmMapReaderFactory::getInstance().createReader(inputs.at(i)));
    inputReader->open(inputs.at(i));
    boost::shared_ptr<ElementInputStream> inputStream =
      boost::dynamic_pointer_cast<ElementInputStream>(inputReader);

    StringTokenizer tokenizer;
    while (inputStream->hasMoreElements())
    {
      ElementPtr element = inputStream->readNextElement();
      const Tags& tags = element->getTags();
      LOG_VART(element);
      LOG_VART(typeKeys.isEmpty());
      LOG_VART(tags.hasAnyKey(typeKeys));

      if (element->getElementType() == ElementType::Node &&
          (typeKeys.isEmpty() || tags.hasAnyKey(typeKeys)))
      {
        const QStringList names = tags.getNames();
        LOG_VART(names);
        if (names.size() > 0)
        {
          const QStringList kvps = _getPoiKvps(tags);
          LOG_VART(kvps.size());
          if (!kvps.isEmpty())
          {
            for (int i = 0; i < names.size(); i++)
            {
              const QString name = names.at(i);

              for (int j = 0; j < kvps.size(); j++)
              {
                _updateForNewWord(name, kvps.at(j));
              }

              const QStringList nameTokens = tokenizer.tokenize(name);
              LOG_VART(nameTokens.size());
              for (int j = 0; j < nameTokens.size(); j++)
              {
                for (int k = 0; k < kvps.size(); k++)
                {
                  _updateForNewWord(nameTokens.at(j), kvps.at(k));
                }
              }
            }

            poiCount++;

            if (poiCount % _statusUpdateInterval == 0)
            {
              PROGRESS_INFO(
                "Derived implicit tags for " << StringUtils::formatLargeNumber(poiCount) <<
                " POIs.");
            }
          }
        }

        nodeCount++;

        if (nodeCount % _statusUpdateInterval == 0)
        {
          PROGRESS_INFO(
            "Parsed " << StringUtils::formatLargeNumber(nodeCount) << " nodes from input.");
        }
      }
    }
    inputReader->finalizePartial();
  }
  _wordCaseMappings.clear();

  //TODO: try to reduce these mutiple passes over the data down to a single pass
  _removeKvpsBelowOccuranceThreshold(minOccurancesThreshold);
  _removeDuplicatedKeyTypes();
  _removeIrrelevantKeyTypes(typeKeysAllowed);
  _tagRulesByWord = _generateTagRulesByWord();
  _tagRules = _rulesByWordToRules(_tagRulesByWord);

  LOG_INFO(
    "Generated " << StringUtils::formatLargeNumber(_tagRules.size()) <<
    " implicit tag rules for " << StringUtils::formatLargeNumber(_tagRulesByWord.size()) <<
    " unique words and " << StringUtils::formatLargeNumber(poiCount) << " POIs (" <<
    StringUtils::formatLargeNumber(nodeCount) << "nodes parsed).");

  for (int i = 0; i < outputs.size(); i++)
  {
    const QString output = outputs.at(i);
    LOG_INFO("Writing implicit tag rules to " << output << "...");
    boost::shared_ptr<ImplicitTagRulesWriter> rulesWriter =
      ImplicitTagRulesWriterFactory::getInstance().createWriter(output);
    rulesWriter->open(output);
    rulesWriter->write(_tagRules);
    rulesWriter->write(_tagRulesByWord);
    rulesWriter->close();
  }
}

void PoiImplicitTagRulesDeriver::_removeKvpsBelowOccuranceThreshold(const int minOccurancesThreshold)
{
  LOG_DEBUG(
    "Removing duplicated kvp below mininum occurance threshold: " << minOccurancesThreshold <<
    "...");

  QMap<QString, long> updatedCounts; //*
  //Tgs::BigMap<QString, long> updatedCounts;
//  fixed_name_map myFixedMap(
//    (fixed_name_map::node_block_type::raw_size)*5, (fixed_name_map::leaf_block_type::raw_size)*5);
  QMap<QString, QStringList> updatedValues; //*

  long kvpRemovalCount = 0;
  for (QMap<QString, long>::const_iterator kvpCountsItr = _wordKvpsToOccuranceCounts.begin();
       kvpCountsItr != _wordKvpsToOccuranceCounts.end(); ++kvpCountsItr)
  {
    const long count = kvpCountsItr.value();
    LOG_VART(count);
    if (count >= minOccurancesThreshold)
    {
      const QStringList keyParts = kvpCountsItr.key().split(";");
      const QString word = keyParts[0];
      const QString kvp = keyParts[1];
      const QStringList kvpParts = kvp.split("=");
      const QString kvpKey = kvpParts[0];
      const QString kvpVal = kvpParts[1];
      const QString wordKvpKey = word % ";" % kvpKey;

      if (!kvpCountsItr.key().contains(";"))
      {
        LOG_VARE(kvpCountsItr.key());
      }
      updatedCounts[kvpCountsItr.key()] = count;
      if (!updatedValues.contains(wordKvpKey))
      {
        updatedValues[wordKvpKey] = QStringList();
      }
      updatedValues[wordKvpKey].append(kvpVal);
      LOG_VART(updatedValues[wordKvpKey]);
    }
    else
    {
      kvpRemovalCount++;
    }
  }

  _wordKvpsToOccuranceCounts = updatedCounts;
  _wordTagKeysToTagValues= updatedValues;

  LOG_DEBUG(
    "Removed " << StringUtils::formatLargeNumber(kvpRemovalCount) << " tag instances that " <<
    "fell below the minimum occurrance threshold of " << minOccurancesThreshold);
}

void PoiImplicitTagRulesDeriver::_removeIrrelevantKeyTypes(const QStringList typeKeysAllowed)
{
  if (typeKeysAllowed.size() == 0)
  {
    return;
  }

  LOG_DEBUG("Removing irrelevant kvps...");

  QMap<QString, long> updatedCounts; //*
  QMap<QString, QStringList> updatedValues; //*

  long irrelevantKvpRemovalCount = 0;
  for (QMap<QString, long>::const_iterator kvpCountsItr = _wordKvpsToOccuranceCounts.begin();
       kvpCountsItr != _wordKvpsToOccuranceCounts.end(); ++kvpCountsItr)
  {
    const QStringList keyParts = kvpCountsItr.key().split(";");
    const QString word = keyParts[0];
    const QString key = keyParts[1].split("=")[0];

    if (typeKeysAllowed.contains(key.toLower()))
    {
      updatedCounts[kvpCountsItr.key()] = kvpCountsItr.value();
      const QString wordKvpKey = word % ";" % key;
      updatedValues[wordKvpKey] = _wordTagKeysToTagValues[wordKvpKey];
    }
    else
    {
      irrelevantKvpRemovalCount++;
    }
  }

  _wordKvpsToOccuranceCounts = updatedCounts;
  _wordTagKeysToTagValues= updatedValues;

  LOG_DEBUG(
    "Removed " << StringUtils::formatLargeNumber(irrelevantKvpRemovalCount) <<
    " tag instances that did not belong to the specified types list.");
}

void PoiImplicitTagRulesDeriver::_removeDuplicatedKeyTypes()
{
  LOG_DEBUG("Removing duplicated kvp types...");

  QMap<QString, long> updatedCounts; //*
  QMap<QString, QStringList> updatedValues; //*

  long duplicatedKeyTypeRemovalCount = 0;
  for (QMap<QString, QStringList>::const_iterator valsItr = _wordTagKeysToTagValues.begin();
       valsItr != _wordTagKeysToTagValues.end(); ++valsItr)
  {
    const QString wordKvpKey = valsItr.key();
    LOG_VART(wordKvpKey);
    const QStringList vals = valsItr.value();
    LOG_VART(vals.size());

    assert(vals.size() != 0);
    if (vals.size() > 1)
    {
      LOG_TRACE(vals.size() << " values mapped to wordKvpKey: " << wordKvpKey);

      QString highestOccurranceKvp;
      long highestOccurranceCount = 0;

      for (int i = 0; i < vals.size(); i++)
      {
        const QString wordKvp = wordKvpKey % "=" % vals.at(i);
        LOG_VART(wordKvp);
        const long occurranceCount = _wordKvpsToOccuranceCounts[wordKvp];
        LOG_VART(occurranceCount);
        if (occurranceCount > highestOccurranceCount)
        {
          highestOccurranceCount = occurranceCount;
          LOG_VART(highestOccurranceCount);
          highestOccurranceKvp = wordKvp;
          LOG_VART(highestOccurranceKvp);
        }
      }

      if (highestOccurranceCount > 0)
      {
        if (!highestOccurranceKvp.contains(";"))
        {
          LOG_VARE(highestOccurranceKvp);
        }
        updatedCounts[highestOccurranceKvp] = highestOccurranceCount;
        LOG_VART(updatedCounts[highestOccurranceKvp]);
        const QString highestOccurranceVal = highestOccurranceKvp.split("=")[1];
        if (!updatedValues.contains(wordKvpKey))
        {
          updatedValues[wordKvpKey] = QStringList();
        }
        updatedValues[wordKvpKey].append(highestOccurranceVal);
        LOG_VART(updatedValues[wordKvpKey]);
      }

      //removed all but one of the tag values for the same tag key
      duplicatedKeyTypeRemovalCount += vals.size() - 1;
    }
    else //size == 1 - tag key has only one tag value associated with it, so no removal necessary
    {
      LOG_TRACE("One value mapped to wordKvpKey: " << wordKvpKey);

      const QString wordKvp = wordKvpKey % "=" % vals.at(0);
      if (!wordKvp.contains(";"))
      {
        LOG_VARE(wordKvp);
      }
      updatedCounts[wordKvp] = _wordKvpsToOccuranceCounts[wordKvp];
      LOG_VART(updatedCounts[wordKvp]);
      updatedValues[wordKvpKey] = _wordTagKeysToTagValues[wordKvpKey];
      LOG_VART(updatedValues[wordKvpKey]);
    }
  }

  _wordKvpsToOccuranceCounts = updatedCounts;
  _wordTagKeysToTagValues= updatedValues;

  LOG_DEBUG(
    "Removed " << StringUtils::formatLargeNumber(duplicatedKeyTypeRemovalCount) <<
    " tag values belonged to the same tag key for a give word.");
}

ImplicitTagRulesByWord PoiImplicitTagRulesDeriver::_generateTagRulesByWord()
{
  LOG_DEBUG("Generating rules by word output...");

  //key=<word>, value=map: key=<kvp>, value=<kvp occurance count>
  ImplicitTagRulesByWord wordsToKvpsWithCounts;

  for (QMap<QString, long>::const_iterator kvpsWithCountsItr = _wordKvpsToOccuranceCounts.begin();
       kvpsWithCountsItr != _wordKvpsToOccuranceCounts.end(); ++kvpsWithCountsItr)
  {
    const QString wordKvp = kvpsWithCountsItr.key();
    const QStringList wordKvpParts = wordKvp.split(";");
    QString word = wordKvpParts[0];
    if (word.contains("="))
    {
      LOG_VARE(word);
    }
    const QString kvp = wordKvpParts[1];
    const long kvpCount = kvpsWithCountsItr.value();

    if (!wordsToKvpsWithCounts.contains(word))
    {
      wordsToKvpsWithCounts[word] = QMap<QString, long>();
    }
    QMap<QString, long> kvpsWithCounts = wordsToKvpsWithCounts[word];
    kvpsWithCounts[kvp] = kvpCount;
    wordsToKvpsWithCounts[word] = kvpsWithCounts;
  }

  return wordsToKvpsWithCounts;
}

ImplicitTagRules PoiImplicitTagRulesDeriver::_rulesByWordToRules(
  const ImplicitTagRulesByWord& rulesByWord)
{
  LOG_DEBUG("Generating rules output...");

  ImplicitTagRules tagRules;

  long totalWordInstances = 0;
  long totalTagInstances = 0;
  QMap<QString, ImplicitTagRulePtr> tagsToRules;
  //key=<word>, value=map: key=<kvp>, value=<kvp occurance count>
  for (ImplicitTagRulesByWord::const_iterator rulesByWordItr = rulesByWord.begin();
       rulesByWordItr != rulesByWord.end(); ++rulesByWordItr)
  {
    const QString word = rulesByWordItr.key();
    LOG_VART(word);

    QMap<QString, long> kvpsWithCounts = rulesByWordItr.value();
    LOG_VART(kvpsWithCounts);
    ImplicitTagRulePtr rule;
    for (QMap<QString, long>::const_iterator kvpsWithCountsItr = kvpsWithCounts.begin();
         kvpsWithCountsItr != kvpsWithCounts.end(); ++kvpsWithCountsItr)
    {
      const QString kvp =  kvpsWithCountsItr.key();
      LOG_VART(kvp);
      //not concerned with counts here, b/c we've already eliminated all rules below the min
      //allowed threshold
      if (tagsToRules.contains(kvp))
      {
        LOG_TRACE("Tag: " << kvp << " already exists for rule.");
        rule = tagsToRules[kvp];
      }
      else
      {
        LOG_TRACE("Creating new rule for tag: " << kvp << "...");
        rule.reset(new ImplicitTagRule());
        tagsToRules[kvp] = rule;
        tagRules.append(rule);
        LOG_VART(tagRules.size());
        rule->getTags().appendValue(kvp);
      }
      rule->getWords().insert(word);
    }

    LOG_VART(rule->getWords());
    LOG_VART(rule->getTags());
  }

  _avgWordsPerRule = 0;
  _avgTagsPerRule = 0;

  return tagRules;
}

}
