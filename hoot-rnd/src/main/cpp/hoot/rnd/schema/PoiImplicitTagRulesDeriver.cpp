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
#include <hoot/core/util/DbUtils.h>

// Qt
#include <QStringBuilder>

namespace hoot
{

PoiImplicitTagRulesDeriver::PoiImplicitTagRulesDeriver() :
_statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
_runInMemory(ConfigOptions().getPoiImplicitTagRulesRunInMemory()),
_wordCaseMappingsCache(ConfigOptions().getPoiImplicitTagRulesCacheSize())
{
  _readIgnoreLists();
}

void PoiImplicitTagRulesDeriver::_readIgnoreLists()
{
  QFile tagIgnoreFile(ConfigOptions().getPoiImplicitTagRulesTagIgnoreList());
  if (!tagIgnoreFile.open(QIODevice::ReadOnly))
  {
    throw HootException(
      QObject::tr("Error opening %1 for writing.").arg(tagIgnoreFile.fileName()));
  }
  _tagIgnoreList.clear();
  while (!tagIgnoreFile.atEnd())
  {
    _tagIgnoreList.append(QString::fromUtf8(tagIgnoreFile.readLine().constData()));
  }
  tagIgnoreFile.close();

  QFile wordIgnoreFile(ConfigOptions().getPoiImplicitTagRulesWordIgnoreList());
  if (!wordIgnoreFile.open(QIODevice::ReadOnly))
  {
    throw HootException(
      QObject::tr("Error opening %1 for writing.").arg(wordIgnoreFile.fileName()));
  }
  _wordIgnoreList.clear();
  while (!wordIgnoreFile.atEnd())
  {
    _wordIgnoreList.append(QString::fromUtf8(wordIgnoreFile.readLine().constData()));
  }
  wordIgnoreFile.close();
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
        OsmSchema::getInstance().getCategories(kvp).intersects(OsmSchemaCategory::poi()) &&
        !_tagIgnoreList.contains(kvp))
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
  if (_runInMemory)
  {
    //FixedLengthString fixedLengthWord = _qStrToFixedLengthStr(word);
    //FixedLengthString fixedLengthLowerCaseWord = _qStrToFixedLengthStr(lowerCaseWord);
    //if (_wordCaseMappings.find(fixedLengthLowerCaseWord) != _wordCaseMappings.end())
    //if (_wordCaseMappings.contains(fixedLengthLowerCaseWord))
    if (_wordCaseMappings.contains(lowerCaseWord))
    {
      //word = _fixedLengthStrToQStr(_wordCaseMappings[fixedLengthLowerCaseWord]);
      word = _wordCaseMappings[lowerCaseWord];
    }
    else
    {
      //_wordCaseMappings[fixedLengthLowerCaseWord] = fixedLengthWord;
      _wordCaseMappings[lowerCaseWord] = word;
    }
  }
  else
  {
    //If the bloom filter can be made to work with FixedLengthString, then we can replace this block
    //with something like the above but using BigMap instead of QMap.  FixedLengthString with a
    //raw stxxl map is ~3 orders of magnitude slower than using QMap.  BigMap would be expected to
    //to perform much faster than that but still more slowly than using QMap, obviously.
    QString queriedWord;
    QString* queriedWordPtr = _wordCaseMappingsCache[lowerCaseWord];
    if (queriedWordPtr != 0)
    {
      queriedWord = *queriedWordPtr;
    }
    else
    {
      queriedWord = _tempDbWriter.getWord(word);
    }
    LOG_VART(queriedWord);
    if (!queriedWord.isEmpty())
    {
      word = queriedWord;
    }
    else
    {
      QString* wordPtr = new QString();
      *wordPtr = word;
      _wordCaseMappingsCache.insert(lowerCaseWord, wordPtr);
      _tempDbWriter.insertWord(word);
    }
  }

  const QString line = word % QString("\t") % kvp % QString("\n");
  _countFile->write(line.toUtf8());
}

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

  _wordKeysToCounts.clear();
  _wordCaseMappings.clear();
  _wordCaseMappingsCache.clear();
  const bool tokenize = ConfigOptions().getPoiImplicitTagRulesTokenizeNames();
  LOG_VART(tokenize);

  _countFile.reset(
    new QTemporaryFile(
      ConfigOptions().getApidbBulkInserterTempFileDir() +
      "/poi-implicit-tag-rules-deriver-temp-XXXXXX"));
  //_countFile->setAutoRemove(false); //for debugging only
  if (!_countFile->open())
  {
    throw HootException(QObject::tr("Error opening %1 for writing.").arg(_countFile->fileName()));
  }
  LOG_DEBUG("Opened temp file: " << _countFile->fileName());

  QStringList typeKeysAllowed;
  for (int i = 0; i < typeKeys.size(); i++)
  {
    typeKeysAllowed.append(typeKeys.at(i).toLower());
  }
  LOG_VART(typeKeysAllowed.isEmpty());

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
              if (!_wordIgnoreList.contains(name))
              {
                //'=' is used in the map key for kvps, so it needs to be escaped in the word
                if (name.contains("="))
                {
                  name = name.replace("=", "%3D");
                }
                for (int j = 0; j < kvps.size(); j++)
                {
                  _updateForNewWord(name, kvps.at(j));
                }

                if (tokenize)
                {
                  const QStringList nameTokens = tokenizer.tokenize(name);
                  LOG_VART(nameTokens.size());
                   for (int j = 0; j < nameTokens.size(); j++)
                  {
                    QString nameToken = nameTokens.at(j);
                    //may need to replace more puncutation chars here
                    nameToken = nameToken.replace(",", "");
                    if (!_wordIgnoreList.contains(name))
                    {
                      for (int k = 0; k < kvps.size(); k++)
                      {
                        _updateForNewWord(nameToken, kvps.at(k));
                      }
                    }
                  }
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
    inputReader->finalizePartial();
  }
  _countFile->close();
  LOG_VARD(_wordCaseMappings.size());
  _wordCaseMappings.clear();
  _wordCaseMappingsCache.clear();

  LOG_INFO(
    "Parsed " << StringUtils::formatLargeNumber(poiCount) << " POIs from " <<
    StringUtils::formatLargeNumber(nodeCount) << " nodes.");

  _removeKvpsBelowOccuranceThresholdAndSortByOccurrance(minOccurancesThreshold);
  _removeDuplicatedKeyTypes();
  _tempDbWriter.close();
  _sortByWord();

//  LOG_INFO(
//    "Extracted "  << StringUtils::formatLargeNumber(_wordKeysToCounts.size()) <<
//    " word/tag associations.");
//  _wordKeysToCounts.clear();

  _writeRules(outputs, _sortedByWordDedupedCountFile->fileName());
}

void PoiImplicitTagRulesDeriver::_writeRules(const QStringList outputs,
                                             const QString inputFile)
{

  for (int i = 0; i < outputs.size(); i++)
  {
    const QString output = outputs.at(i);
    LOG_VART(output);
    boost::shared_ptr<ImplicitTagRuleWordPartWriter> ruleWordPartWriter =
      ImplicitTagRuleWordPartWriterFactory::getInstance().createWriter(output);
    ruleWordPartWriter->open(output);
    ruleWordPartWriter->write(inputFile);
    ruleWordPartWriter->close();
  }
}

void PoiImplicitTagRulesDeriver::_removeKvpsBelowOccuranceThresholdAndSortByOccurrance(
  const int minOccurancesThreshold)
{
  LOG_INFO("Removing tags below minimum occurance threshold of: " << minOccurancesThreshold << "...");

  _sortedCountFile.reset(
    new QTemporaryFile(
      ConfigOptions().getApidbBulkInserterTempFileDir() +
      "/poi-implicit-tag-rules-deriver-temp-XXXXXX"));
  //_sortedCountFile->setAutoRemove(false); //for debugging only
  if (!_sortedCountFile->open())
  {
    throw HootException(
      QObject::tr("Error opening %1 for writing.").arg(_sortedCountFile->fileName()));
  }
  LOG_DEBUG("Opened sorted temp file: " << _sortedCountFile->fileName());

  //This counts each unique line occurrance, sorts by decreasing occurrence count (necessary for
  //next step which removes duplicate tag keys associated with the same word), removes lines with
  //occurrance counts below the specified threshold, and replaces the space between the prepended
  //count and the word with a tab. - not sure why 1 needs to be subtracted from
  //minOccurancesThreshold here, though...
  const QString cmd =
    "sort " + _countFile->fileName() + " | uniq -c | sort -n -r | awk -v limit=" +
    QString::number(minOccurancesThreshold - 1) +
    " '$1 > limit{print}' | sed -e 's/^ *//;s/ /\t/' > " + _sortedCountFile->fileName();
  if (std::system(cmd.toStdString().c_str()) != 0)
  {
    throw HootException("Unable to sort input file.");
  }
  _sortedCountFile->close();
}

void PoiImplicitTagRulesDeriver::_removeDuplicatedKeyTypes()
{
  LOG_INFO("Removing duplicated key types...");

  //i.e. don't allow amenity=school AND amenity=shop to be associated with the same word...pick one
  //of them

  _sortedDedupedCountFile.reset(
    new QTemporaryFile(
      ConfigOptions().getApidbBulkInserterTempFileDir() +
      "/poi-implicit-tag-rules-deriver-temp-XXXXXX"));
  //_sortedDedupedCountFile->setAutoRemove(false); //for debugging only
  if (!_sortedDedupedCountFile->open())
  {
    throw HootException(
      QObject::tr("Error opening %1 for writing.").arg(_sortedDedupedCountFile->fileName()));
  }
  LOG_DEBUG("Opened sorted, deduped temp file: " << _sortedDedupedCountFile->fileName());
  if (!_sortedCountFile->open())
  {
    throw HootException(
      QObject::tr("Error opening %1 for reading.").arg(_sortedCountFile->fileName()));
  }
  LOG_DEBUG("Opened sorted input temp file: " << _sortedCountFile->fileName());

  QCache<QString, long> wordTagKeysToCountsCache(ConfigOptions().getPoiImplicitTagRulesCacheSize());
  QCache<QString, long> wordIdCache(ConfigOptions().getPoiImplicitTagRulesCacheSize());
  QCache<QString, long> tagKeyIdCache(ConfigOptions().getPoiImplicitTagRulesCacheSize());
  while (!_sortedCountFile->atEnd())
  {
    const QString line = QString::fromUtf8(_sortedCountFile->readLine().constData()).trimmed();
    LOG_VART(line);
    const QStringList lineParts = line.split("\t");
    LOG_VART(lineParts);
    QString word = lineParts[1].trimmed();
    LOG_VART(word);
    const QString kvp = lineParts[2].trimmed();
    LOG_VART(kvp);
    const long count = lineParts[0].trimmed().toLong();
    LOG_VART(count);
    const QString tagKey = kvp.split("=")[0];
    LOG_VART(tagKey);
    const QString wordTagKey = word.trimmed() % ";" % tagKey.trimmed();
    LOG_VART(wordTagKey);

    //The lines are sorted by occurrence count.  So the first time we see one word-key combo, we
    //know it had the highest occurrence count, and we can ignore all subsequent instances since
    //any one feature can't have more than one tag applied to it with the same key.

    if (_runInMemory)
    {
      if (!_wordKeysToCounts.contains(wordTagKey))
      {
        _wordKeysToCounts[wordTagKey] = count;
        //this unescaping must occur during the final temp file write
        if (word.contains("%3D"))
        {
          word = word.replace("%3D", "=");
        }
        else if (word.contains("%3d"))
        {
          word = word.replace("%3d", "=");
        }
        const QString updatedLine = QString::number(count) % "\t" % word % "\t" % kvp % "\n";
        LOG_VART(updatedLine);
        _sortedDedupedCountFile->write(updatedLine.toUtf8());
      }
      //TODO: if this becomes a common occurrance, should probably pick the best of the tags that
      //have occurrance count ties
      else if (_wordKeysToCounts[wordTagKey] == count)
      {
        LOG_DEBUG(
          "Found word with multiple tag occurrance counts of the same size.  Arbitrarily " <<
          "chose first tag.  Not creating implicit tag entry for word: " << word << ", tag: " <<
          kvp << ", count: " << count);
      }
    }
    else
    {
      //see comment in _updateForNewWord
      long queriedCount = -1;
      long* queriedCountPtr = wordTagKeysToCountsCache[wordTagKey];
      if (queriedCountPtr != 0)
      {
        queriedCount = *queriedCountPtr;
        assert(queriedCount != -1);
        if (queriedCount == count)
        {
          LOG_DEBUG(
            "Found word with multiple tag occurrance counts of the same size.  Arbitrarily " <<
            "chose first tag.  Not creating implicit tag entry for word: " << word << ", tag: " <<
            kvp << ", count: " << count);
        }
        //do nothing
      }
      else
      {
        long wordId = -1;
        long* wordIdPtr = wordIdCache[word.toLower()];
        if (wordIdPtr != 0)
        {
          wordId = *wordIdPtr;
          assert(wordId != -1);
        }
        else
        {
          wordId = _tempDbWriter.getWordIdForWord(word);
          wordIdPtr = new long();
          *wordIdPtr = wordId;
          wordIdCache.insert(word.toLower(), wordIdPtr);
        }
        LOG_VART(wordId);
        assert(wordId != -1);

        long tagKeyId = -1;
        long* tagKeyIdPtr = tagKeyIdCache[tagKey];
        if (tagKeyIdPtr != 0)
        {
          tagKeyId = *tagKeyIdPtr;
          assert(tagKeyId != -1);
        }
        else
        {
          tagKeyId = _tempDbWriter.getTagKeyIdForTagKey(tagKey);
        }
        LOG_VART(tagKeyId);

        if (tagKeyId != -1)
        {
          LOG_TRACE("Found tag key id: " << tagKeyId << " for tag key: " << tagKey << ".");
          const long queriedCount = _tempDbWriter.getWordTagKeyCount(wordId, tagKeyId);
          LOG_VART(queriedCount);
          if (queriedCount == -1)
          {
            LOG_TRACE(
              "Found no existing tag count for word: " << word << " and tag key: " << tagKey <<
              ".  Writing combination to database and updating temp file...");
            long* countPtr = new long();
            *countPtr = count;
            wordTagKeysToCountsCache.insert(wordTagKey, countPtr);
            _tempDbWriter.insertWordTagKey(wordId, tagKeyId, count);
            _updateSortedDedupedFile(word, kvp, count);
          }
          else if (queriedCount == count)
          {
            LOG_DEBUG(
              "Found word with multiple tag occurrance counts of the same size.  Arbitrarily " <<
              "chose first tag.  Not creating implicit tag entry for word: " << word << ", tag: " <<
              kvp << ", count: " << count);
          }
        }
        else
        {
          LOG_TRACE(
            "Found no tag key id: " << tagKeyId << " for tag key: " << tagKey <<
             ".  Adding tag key...");
          tagKeyId = _tempDbWriter.insertTagKey(tagKey);
          tagKeyIdPtr = new long();
          *tagKeyIdPtr = tagKeyId;
          tagKeyIdCache.insert(tagKey, tagKeyIdPtr);
          LOG_VART(tagKeyId);
          LOG_TRACE("Writing combination to database and updating temp file...");
          long* countPtr = new long();
          *countPtr = count;
          wordTagKeysToCountsCache.insert(wordTagKey, countPtr);
          _tempDbWriter.insertWordTagKey(wordId, tagKeyId, count);
          _updateSortedDedupedFile(word, kvp, count);
        }
      }
    }
  }
  wordTagKeysToCountsCache.clear();
  _sortedCountFile->close();
  _sortedDedupedCountFile->close();
}

void PoiImplicitTagRulesDeriver::_updateSortedDedupedFile(QString word, const QString kvp,
                                                          const long count)
{
  //this unescaping must occur during the final temp file write
  if (word.contains("%3D"))
  {
    word = word.replace("%3D", "=");
  }
  else if (word.contains("%3d"))
  {
    word = word.replace("%3d", "=");
  }
  const QString updatedLine = QString::number(count) % "\t" % word % "\t" % kvp % "\n";
  LOG_VART(updatedLine);
  _sortedDedupedCountFile->write(updatedLine.toUtf8());
}

void PoiImplicitTagRulesDeriver::_sortByWord()
{
  LOG_INFO("Sorting output file...");

  _sortedByWordDedupedCountFile.reset(
    new QTemporaryFile(
      ConfigOptions().getApidbBulkInserterTempFileDir() +
      "/poi-implicit-tag-rules-deriver-temp-XXXXXX"));
  //_sortedByWordDedupedCountFile->setAutoRemove(false); //for debugging only
  if (!_sortedByWordDedupedCountFile->open())
  {
    throw HootException(
      QObject::tr("Error opening %1 for writing.").arg(_sortedByWordDedupedCountFile->fileName()));
  }
  LOG_DEBUG("Opened sorted by word temp file: " << _sortedByWordDedupedCountFile->fileName());

  //sort by word, then by tag
  //-d -k2,3 -t$'\t'
  const QString cmd =
    "sort -t'\t' -k2,2 -k3,3 " + _sortedDedupedCountFile->fileName() + " -o " +
    _sortedByWordDedupedCountFile->fileName();
  if (std::system(cmd.toStdString().c_str()) != 0)
  {
    throw HootException("Unable to sort input file.");
  }
  _sortedByWordDedupedCountFile->close();
}

//FixedLengthString PoiImplicitTagRulesDeriver::qStrToFixedLengthStr(const QString str)
//{
//  FixedLengthString fixedLengthStr;
//  memset(fixedLengthStr.data, 0, sizeof fixedLengthStr.data);
//  std::wcstombs(fixedLengthStr.data, str.toStdWString().c_str(), MAX_KEY_LEN);
//  return fixedLengthStr;
//}

//QString PoiImplicitTagRulesDeriver::fixedLengthStrToQStr(const FixedLengthString& fixedLengthStr)
//{
//  wchar_t wKey[MAX_KEY_LEN];
//  std::mbstowcs(wKey, fixedLengthStr.data, MAX_KEY_LEN);
//  return QString::fromWCharArray(wKey);
//}

//QMap<QString, long> PoiImplicitTagRulesDeriver::_stxxlMapToQtMap(
//  const FixedLengthStringToLongMap& stxxlMap)
//{
//  LOG_DEBUG("Converting stxxl map to qt map...");
//  QMap<QString, long> qtMap;
//  for (FixedLengthStringToLongMap::const_iterator mapItr = stxxlMap.begin();
//       mapItr != stxxlMap.end(); ++mapItr)
//  {
//    const QString key = fixedLengthStrToQStr(mapItr->first);
//    LOG_VART(key);
//    const long value = mapItr->second;
//    LOG_VART(value);
//    qtMap[key] = value;
//  }
//  return qtMap;
//}

}
