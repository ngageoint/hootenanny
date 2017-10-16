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
#include <hoot/core/algorithms/string/StringTokenizer.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/ElementVisitorInputStream.h>
#include <hoot/core/visitors/TranslationVisitor.h>
#include <hoot/rnd/io/ImplicitTagRulesSqliteReader.h>

// Qt
#include <QStringBuilder>

namespace hoot
{

PoiImplicitTagRulesDeriver::PoiImplicitTagRulesDeriver() :
_avgTagsPerRule(0),
_avgWordsPerRule(0),
_statusUpdateInterval(ConfigOptions().getApidbBulkInserterFileOutputStatusUpdateInterval()),
_highestRuleWordCount(0),
_highestRuleTagCount(0)
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

//void PoiImplicitTagRulesDeriver::_updateForNewWord(QString word, const QString kvp)
//{
//  LOG_TRACE("Updating word: " << word << " with kvp: " << kvp << "...");

//  //'=' is used as a map key for kvps, so it needs to be escaped in the word
//  if (word.contains("="))
//  {
//    word = word.replace("=", "%3D");
//  }

//  const QString lowerCaseWord = word.toLower();
//  if (_wordCaseMappings.contains(lowerCaseWord))
//  {
//    word = _wordCaseMappings[lowerCaseWord];
//  }
//  else
//  {
//    _wordCaseMappings[lowerCaseWord] = word;
//  }

//  const QString wordKvp = word % ";" % kvp;
//  LOG_VART(wordKvp);
//  if (!_wordKvpsToOccuranceCounts.contains(wordKvp))
//  {
//    _wordKvpsToOccuranceCounts[wordKvp] = 1;
//  }
//  else
//  {
//    _wordKvpsToOccuranceCounts[wordKvp]++;
//  }
//  LOG_VART( _wordKvpsToOccuranceCounts[wordKvp]);

//  const QStringList kvpParts = kvp.split("=");
//  const QString kvpKey = kvpParts[0];
//  const QString kvpVal = kvpParts[1];
//  const QString wordKvpKey = word % ";" % kvpKey;
//  LOG_VART(wordKvpKey);
//  if (!_wordTagKeysToTagValues.contains(wordKvpKey))
//  {
//    QStringList valsList;
//    valsList.append(kvpVal);
//    _wordTagKeysToTagValues[wordKvpKey] = valsList;
//  }
//  else
//  {
//    _wordTagKeysToTagValues[wordKvpKey].append(kvpVal);
//  }
//  LOG_VART(_wordTagKeysToTagValues[wordKvpKey]);
//}

QString PoiImplicitTagRulesDeriver::_getSqliteOutput(const QStringList outputs)
{
  for (int i = 0; i < outputs.size(); i++)
  {
    if (outputs.at(i).endsWith(".sqlite"))
    {
      return outputs.at(i);
    }
  }
  return "";
}

void PoiImplicitTagRulesDeriver::deriveRules(const QStringList inputs,
                                             const QStringList translationScripts,
                                             const QStringList outputs,
                                             const QStringList typeKeys,
                                             const int minOccurancesThreshold)
{
  if (inputs.isEmpty())
  {
    throw HootException("No inputs were specified.");
  }

  if (outputs.isEmpty())
  {
    throw HootException("No outputs were specified.");
  }
  const QString sqliteOutput = _getSqliteOutput(outputs);
  if (sqliteOutput.isEmpty())
  {
    throw HootException("Outputs must contain at least one Sqlite database.");
  }

  if (inputs.size() != translationScripts.size())
  {
    LOG_VARD(inputs.size());
    LOG_VARD(translationScripts.size());
    throw HootException(
      "The size of the input datasets list must equal the size of the list of translation scripts.");
  }

  LOG_INFO(
    "Deriving POI implicit tag rules for inputs: " << inputs << ", translation scripts: " <<
    translationScripts << ", type keys: " << typeKeys << ", and minimum occurance threshold: " <<
    minOccurancesThreshold << ".  Writing to outputs: " << outputs << "...");

  QStringList typeKeysAllowed;
  for (int i = 0; i < typeKeys.size(); i++)
  {
    typeKeysAllowed.append(typeKeys.at(i).toLower());
  }
  LOG_VART(typeKeysAllowed.isEmpty());

  _ruleWriter.open(sqliteOutput);

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
    boost::shared_ptr<TranslationVisitor> translationVisitor(new TranslationVisitor());
    translationVisitor->setPath(translationScripts.at(i));
    inputStream.reset(new ElementVisitorInputStream(inputReader, translationVisitor));

    StringTokenizer tokenizer;
    while (inputStream->hasMoreElements())
    {
      ElementPtr element = inputStream->readNextElement();
      if (element->getElementType() == ElementType::Node)
      {
        LOG_VART(element->getTags());
        const QStringList names = element->getTags().getNames();
        LOG_VART(names.size());
        if (names.size() > 0)
        {
          LOG_VART(names);
          Tags relevantTags;
          if (typeKeysAllowed.isEmpty())
          {
            relevantTags = element->getTags();
          }
          else
          {
            for (Tags::const_iterator tagsItr = element->getTags().begin();
                 tagsItr != element->getTags().end(); ++tagsItr)
            {
              const QString tagKey = tagsItr.key();
              if (typeKeysAllowed.contains(tagKey))
              {
                relevantTags.appendValue(tagKey, tagsItr.value());
              }
            }
          }
          LOG_VART(relevantTags);

          const QStringList kvps = _getPoiKvps(relevantTags);
          LOG_VART(kvps.size());
          if (!kvps.isEmpty())
          {
            for (int i = 0; i < names.size(); i++)
            {
              QString name = names.at(i);
              //'=' is used as a map key for kvps, so it needs to be escaped in the word
              if (name.contains("="))
              {
                name = name.replace("=", "%3D");
              }
              for (int j = 0; j < kvps.size(); j++)
              {
                _ruleWriter.write(name, kvps.at(j));
              }

              const QStringList nameTokens = tokenizer.tokenize(name);
              LOG_VART(nameTokens.size());
              for (int j = 0; j < nameTokens.size(); j++)
              {
                for (int k = 0; k < kvps.size(); k++)
                {
                  _ruleWriter.write(nameTokens.at(j), kvps.at(k));
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

        if (nodeCount % (_statusUpdateInterval * 100) == 0)
        {
          PROGRESS_INFO(
            "Parsed " << StringUtils::formatLargeNumber(nodeCount) << " nodes from input.");
        }
      }
    }
    _ruleWriter.close();
    inputReader->finalizePartial();
  }
//  LOG_VARD(_wordCaseMappings.size());
//  _wordCaseMappings.clear();

  _removeKvpsBelowOccuranceThreshold(minOccurancesThreshold);
  _removeDuplicatedKeyTypes();
//  LOG_VARD(_wordTagKeysToTagValues.size());
//  _wordTagKeysToTagValues.clear();
//  _generateTagRulesByWord();
//  _rulesByWordToRules(_tagRulesByWord);
//  _unescapeRuleWords();

//  LOG_INFO(
//    "Generated " << StringUtils::formatLargeNumber(_tagRules.size()) <<
//    " implicit tag rules for " << StringUtils::formatLargeNumber(_tagRulesByWord.size()) <<
//    " unique words and " << StringUtils::formatLargeNumber(poiCount) << " POIs (" <<
//    StringUtils::formatLargeNumber(nodeCount) << " nodes parsed).");
  LOG_INFO("Average words per rule: " << _avgWordsPerRule);
  LOG_INFO("Average tags per rule: " << _avgTagsPerRule);
  LOG_INFO("Highest rule word count: " << _highestRuleWordCount);
  LOG_INFO("Highest rule tag count: " << _highestRuleTagCount);

  QList<boost::shared_ptr<ImplicitTagRuleWordPartWriter> > ruleWordPartWriters =
    _getNonSqliteOutputWriters(outputs);
  if (!ruleWordPartWriters.isEmpty())
  {
    _writeToNonSqliteOutputs(ruleWordPartWriters, sqliteOutput);
  }
}

QList<boost::shared_ptr<ImplicitTagRuleWordPartWriter> > PoiImplicitTagRulesDeriver::_getNonSqliteOutputWriters(
  const QStringList outputs)
{
  QList<boost::shared_ptr<ImplicitTagRuleWordPartWriter> > ruleWordPartWriters;
  for (int i = 0; i < outputs.size(); i++)
  {
    const QString output = outputs.at(i);
    if (!output.endsWith(".sqlite"))
    {
      boost::shared_ptr<ImplicitTagRuleWordPartWriter> ruleWordPartWriter =
        ImplicitTagRuleWordPartWriterFactory::getInstance().createWriter(output);
      ruleWordPartWriter->open(output);
      ruleWordPartWriters.append(ruleWordPartWriter);
    }
  }
  return ruleWordPartWriters;
}

void PoiImplicitTagRulesDeriver::_writeToNonSqliteOutputs(
  QList<boost::shared_ptr<ImplicitTagRuleWordPartWriter> >& ruleWordPartWriters,
  const QString sqliteOutput)
{
  ImplicitTagRulesSqliteReader rulesReader;
  rulesReader.open(sqliteOutput);
  while (rulesReader.hasMoreRuleWordParts())
  {
    const ImplicitTagRuleWordPartPtr ruleWordPart = rulesReader.getNextRuleWordPart();
    for (QList<boost::shared_ptr<ImplicitTagRuleWordPartWriter> >::iterator writersItr = ruleWordPartWriters.begin();
       writersItr != ruleWordPartWriters.end();
       ++writersItr)
    {
      boost::shared_ptr<ImplicitTagRuleWordPartWriter> rulesWriter = *writersItr;
      rulesWriter->write(*ruleWordPart);
    }
  }
  rulesReader.close();
  for (QList<boost::shared_ptr<ImplicitTagRuleWordPartWriter> >::iterator writersItr = ruleWordPartWriters.begin();
       writersItr != ruleWordPartWriters.end();
       ++writersItr)
  {
    boost::shared_ptr<ImplicitTagRuleWordPartWriter> rulesWriter = *writersItr;
    rulesWriter->close();
  }
}

void PoiImplicitTagRulesDeriver::_removeKvpsBelowOccuranceThreshold(const int minOccurancesThreshold)
{
  if (minOccurancesThreshold == 1)
  {
    return;
  }

  LOG_DEBUG("Removing tags below mininum occurance threshold: " << minOccurancesThreshold << "...");

//  QMap<QString, long> updatedCounts; //*
//  //Tgs::BigMap<QString, long> updatedCounts;
////  fixed_name_map myFixedMap(
////    (fixed_name_map::node_block_type::raw_size)*5, (fixed_name_map::leaf_block_type::raw_size)*5);
//  QMap<QString, QStringList> updatedValues; //*

//  long kvpRemovalCount = 0;
//  for (QMap<QString, long>::const_iterator kvpCountsItr = _wordKvpsToOccuranceCounts.begin();
//       kvpCountsItr != _wordKvpsToOccuranceCounts.end(); ++kvpCountsItr)
//  {
//    const long count = kvpCountsItr.value();
//    LOG_VART(count);
//    if (count >= minOccurancesThreshold)
//    {
//      const QStringList keyParts = kvpCountsItr.key().split(";");
//      const QString word = keyParts[0];
//      const QString kvp = keyParts[1];
//      const QStringList kvpParts = kvp.split("=");
//      const QString kvpKey = kvpParts[0];
//      const QString kvpVal = kvpParts[1];
//      const QString wordKvpKey = word % ";" % kvpKey;

//      if (!kvpCountsItr.key().contains(";"))
//      {
//        LOG_VARE(kvpCountsItr.key());
//      }
//      updatedCounts[kvpCountsItr.key()] = count;
//      if (!updatedValues.contains(wordKvpKey))
//      {
//        updatedValues[wordKvpKey] = QStringList();
//      }
//      updatedValues[wordKvpKey].append(kvpVal);
//      LOG_VART(updatedValues[wordKvpKey]);
//    }
//    else
//    {
//      kvpRemovalCount++;
//    }
//  }

//  _wordKvpsToOccuranceCounts = updatedCounts;
//  _wordTagKeysToTagValues = updatedValues;

//  LOG_DEBUG(
//    "Removed " << StringUtils::formatLargeNumber(kvpRemovalCount) << " tags that " <<
//    "fell below the minimum occurrance threshold of " << minOccurancesThreshold);

//TODO: finish
}

void PoiImplicitTagRulesDeriver::_removeDuplicatedKeyTypes()
{
//  LOG_DEBUG("Removing duplicated tag types...");

//  QMap<QString, long> updatedCounts; //*
//  QMap<QString, QStringList> updatedValues; //*

//  long duplicatedKeyTypeRemovalCount = 0;
//  for (QMap<QString, QStringList>::const_iterator valsItr = _wordTagKeysToTagValues.begin();
//       valsItr != _wordTagKeysToTagValues.end(); ++valsItr)
//  {
//    const QString wordKvpKey = valsItr.key();
//    LOG_VART(wordKvpKey);
//    const QStringList vals = valsItr.value();
//    LOG_VART(vals.size());

//    assert(vals.size() != 0);
//    if (vals.size() > 1)
//    {
//      LOG_TRACE(vals.size() << " values mapped to wordKvpKey: " << wordKvpKey);

//      QString highestOccurranceKvp;
//      long highestOccurranceCount = 0;

//      for (int i = 0; i < vals.size(); i++)
//      {
//        const QString wordKvp = wordKvpKey % "=" % vals.at(i);
//        LOG_VART(wordKvp);
//        const long occurranceCount = _wordKvpsToOccuranceCounts[wordKvp];
//        LOG_VART(occurranceCount);
//        if (occurranceCount > highestOccurranceCount)
//        {
//          highestOccurranceCount = occurranceCount;
//          LOG_VART(highestOccurranceCount);
//          highestOccurranceKvp = wordKvp;
//          LOG_VART(highestOccurranceKvp);
//        }
//      }

//      if (highestOccurranceCount > 0)
//      {
//        if (!highestOccurranceKvp.contains(";"))
//        {
//          LOG_VARE(highestOccurranceKvp);
//        }
//        updatedCounts[highestOccurranceKvp] = highestOccurranceCount;
//        LOG_VART(updatedCounts[highestOccurranceKvp]);
//        const QString highestOccurranceVal = highestOccurranceKvp.split("=")[1];
//        if (!updatedValues.contains(wordKvpKey))
//        {
//          updatedValues[wordKvpKey] = QStringList();
//        }
//        updatedValues[wordKvpKey].append(highestOccurranceVal);
//        LOG_VART(updatedValues[wordKvpKey]);
//      }

//      //removed all but one of the tag values for the same tag key
//      duplicatedKeyTypeRemovalCount += vals.size() - 1;
//    }
//    else //size == 1 - tag key has only one tag value associated with it, so no removal necessary
//    {
//      LOG_TRACE("One value mapped to wordKvpKey: " << wordKvpKey);

//      const QString wordKvp = wordKvpKey % "=" % vals.at(0);
//      if (!wordKvp.contains(";"))
//      {
//        LOG_VARE(wordKvp);
//      }
//      updatedCounts[wordKvp] = _wordKvpsToOccuranceCounts[wordKvp];
//      LOG_VART(updatedCounts[wordKvp]);
//      updatedValues[wordKvpKey] = _wordTagKeysToTagValues[wordKvpKey];
//      LOG_VART(updatedValues[wordKvpKey]);
//    }
//  }

//  _wordKvpsToOccuranceCounts = updatedCounts;
//  _wordTagKeysToTagValues= updatedValues;

//  LOG_DEBUG(
//    "Removed " << StringUtils::formatLargeNumber(duplicatedKeyTypeRemovalCount) <<
//    " tag values belonged to the same tag key for a given word.");

//TODO: finish
}

//void PoiImplicitTagRulesDeriver::_generateTagRulesByWord()
//{
//  LOG_DEBUG("Generating rules by word output...");

//  //_tagRulesByWord: key=<word>, value=map: key=<kvp>, value=<kvp occurance count>

//  for (QMap<QString, long>::const_iterator kvpsWithCountsItr = _wordKvpsToOccuranceCounts.begin();
//       kvpsWithCountsItr != _wordKvpsToOccuranceCounts.end(); ++kvpsWithCountsItr)
//  {
//    const QString wordKvp = kvpsWithCountsItr.key();
//    const QStringList wordKvpParts = wordKvp.split(";");
//    QString word = wordKvpParts[0];
//    if (word.contains("="))
//    {
//      LOG_VARE(word);
//    }
//    LOG_VART(word);

//    const QString kvp = wordKvpParts[1];
//    LOG_VART(kvp);
//    const long kvpCount = kvpsWithCountsItr.value();
//    LOG_VART(kvpCount);

//    if (!_tagRulesByWord.contains(word))
//    {
//      _tagRulesByWord[word] = QMap<QString, long>();
//    }
//    QMap<QString, long> kvpsWithCounts = _tagRulesByWord[word];
//    kvpsWithCounts[kvp] = kvpCount;
//    _tagRulesByWord[word] = kvpsWithCounts;
//  }
//  LOG_VARD(_wordKvpsToOccuranceCounts.size());
//  _wordKvpsToOccuranceCounts.clear();
//}

//void PoiImplicitTagRulesDeriver::_rulesByWordToRules(const ImplicitTagRulesByWord& rulesByWord)
//{
//  LOG_DEBUG("Generating rules output...");

//  //key=<concatenated kvps list>, value=<rule>
//  QMap<QString, ImplicitTagRulePtr> tagsToRules;
//  //key=<word>, value=map: key=<kvp>, value=<kvp occurance count>
//  for (ImplicitTagRulesByWord::const_iterator rulesByWordItr = rulesByWord.begin();
//       rulesByWordItr != rulesByWord.end(); ++rulesByWordItr)
//  {
//    QString word = rulesByWordItr.key();
//    if (word.contains("="))
//    {
//      LOG_VARE(word);
//    }
//    LOG_VART(word);

//    const QSet<QString> kvps = rulesByWordItr.value().keys().toSet();
//    const QString kvpsStr = _kvpsToString(kvps);
//    LOG_VART(kvpsStr);

//    ImplicitTagRulePtr rule;
//    if (tagsToRules.contains(kvpsStr))
//    {
//      LOG_TRACE("Tag set already exists for rule.");
//      rule = tagsToRules[kvpsStr];
//    }
//    else
//    {
//      LOG_TRACE("Creating new rule for tag set...");
//      rule.reset(new ImplicitTagRule());
//      tagsToRules[kvpsStr] = rule;
//      _tagRules.append(rule);
//      LOG_VART(_tagRules.size());
//      const Tags tags = _kvpsToTags(kvps);
//      LOG_VART(tags);
//      rule->setTags(tags);
//    }
//    rule->getWords().insert(word);

//    LOG_VART(rule->getWords());
//    LOG_VART(rule->getTags());
//  }

//  long totalWordInstances = 0;
//  long totalTagInstances = 0;
//  for (ImplicitTagRules::const_iterator rulesItr = _tagRules.begin(); rulesItr != _tagRules.end();
//       ++rulesItr)
//  {
//    const ImplicitTagRulePtr rule = *rulesItr;

//    const long ruleWordCount = rule->getWords().size();
//    totalWordInstances += ruleWordCount;
//    if (ruleWordCount > _highestRuleWordCount)
//    {
//      _highestRuleWordCount = ruleWordCount;
//    }

//    const long ruleTagCount = rule->getTags().size();
//    totalTagInstances += ruleTagCount;
//    if (ruleTagCount > _highestRuleTagCount)
//    {
//      _highestRuleTagCount = ruleTagCount;
//    }
//  }
//  if (_tagRules.size() > 0)
//  {
//    _avgWordsPerRule = totalWordInstances / _tagRules.size();
//    _avgTagsPerRule = totalTagInstances / _tagRules.size();
//  }
//}

//Tags PoiImplicitTagRulesDeriver::_kvpsToTags(const QSet<QString>& kvps)
//{
//  Tags tags;
//  for (QSet<QString>::const_iterator kvpsItr = kvps.begin(); kvpsItr != kvps.end(); ++kvpsItr)
//  {
//    tags.appendValue(*kvpsItr);
//  }
//  return tags;
//}

//QString PoiImplicitTagRulesDeriver::_kvpsToString(const QSet<QString>& kvps)
//{
//  QString kvpsStr;
//  for (QSet<QString>::const_iterator kvpsItr = kvps.begin(); kvpsItr != kvps.end(); ++kvpsItr)
//  {
//    kvpsStr += *kvpsItr % ";";
//  }
//  kvpsStr.chop(1);
//  return kvpsStr;
//}

//void PoiImplicitTagRulesDeriver::_unescapeRuleWords()
//{
//  LOG_DEBUG("Unescaping rule words...");

//  ImplicitTagRulesByWord rulesByWord;
//  for (ImplicitTagRulesByWord::const_iterator rulesByWordItr = _tagRulesByWord.begin();
//       rulesByWordItr != _tagRulesByWord.end(); ++rulesByWordItr)
//  {
//    QString word = rulesByWordItr.key();
//    if (word.contains("="))
//    {
//      LOG_VARE(word);
//    }
//    if (word.contains("%3D"))
//    {
//      word = word.replace("%3D", "=");
//    }
//    else if (word.contains("%3d"))
//    {
//      word = word.replace("%3d", "=");
//    }

//    rulesByWord[word] = rulesByWordItr.value();
//  }
//  _tagRulesByWord = rulesByWord;

//  for (ImplicitTagRules::iterator rulesItr = _tagRules.begin(); rulesItr != _tagRules.end();
//       ++rulesItr)
//  {
//    ImplicitTagRulePtr rule = *rulesItr;
//    const QSet<QString> ruleWords = rule->getWords();
//    QSet<QString> modifiedRuleWords;
//    for (QSet<QString>::const_iterator wordsItr = ruleWords.begin(); wordsItr != ruleWords.end();
//         ++wordsItr)
//    {
//      QString word = *wordsItr;
//      if (word.contains("%3D"))
//      {
//        word = word.replace("%3D", "=");
//      }
//      else if (word.contains("%3d"))
//      {
//        word = word.replace("%3d", "=");
//      }
//      modifiedRuleWords.insert(word);
//    }
//    rule->setWords(modifiedRuleWords);
//  }
//}

}
