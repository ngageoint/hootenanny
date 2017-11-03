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

// Qt
#include <QStringBuilder>

namespace hoot
{

PoiImplicitTagRulesDeriver::PoiImplicitTagRulesDeriver() :
_statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
_minWordLength(ConfigOptions().getPoiImplicitTagRulesMinimumWordLength())
{
  _readIgnoreLists();
  _readRulesLists();
}

void PoiImplicitTagRulesDeriver::_readRulesLists()
{
  QFile customRulesFile(ConfigOptions().getPoiImplicitTagRulesCustomRuleList());
  if (!customRulesFile.open(QIODevice::ReadOnly))
  {
    throw HootException(
      QObject::tr("Error opening %1 for writing.").arg(customRulesFile.fileName()));
  }
  _customRulesList.clear();
  while (!customRulesFile.atEnd())
  {
    const QString line = QString::fromUtf8(customRulesFile.readLine().constData());
    if (!line.startsWith("#"))
    {
      const QStringList lineParts = line.trimmed().split("\t");
      _customRulesList[lineParts[0].trimmed()] = lineParts[1].trimmed();
    }
  }
  customRulesFile.close();

  QFile rulesIgnoreFile(ConfigOptions().getPoiImplicitTagRulesRuleIgnoreList());
  if (!rulesIgnoreFile.open(QIODevice::ReadOnly))
  {
    throw HootException(
      QObject::tr("Error opening %1 for writing.").arg(rulesIgnoreFile.fileName()));
  }
  _rulesIgnoreList.clear();
  while (!rulesIgnoreFile.atEnd())
  {
    const QString line = QString::fromUtf8(rulesIgnoreFile.readLine().constData());
    if (!line.startsWith("#"))
    {
      const QStringList lineParts = line.trimmed().split("\t");
      _rulesIgnoreList[lineParts[0].trimmed()] = lineParts[1].trimmed();
    }
  }
  rulesIgnoreFile.close();
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
    const QString line = QString::fromUtf8(tagIgnoreFile.readLine().constData());
    if (!line.startsWith("#"))
    {
      _tagIgnoreList.append(line);
    }
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
    const QString line = QString::fromUtf8(wordIgnoreFile.readLine().constData());
    if (!line.startsWith("#"))
    {
      _wordIgnoreList.append(line);
    }
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
  word = word.simplified();

  const bool wordTooSmall = word.length() < _minWordLength;

  bool ok = false;
  word.toLong(&ok);
  const bool wordIsNumber = ok;

  if (!wordTooSmall && !wordIsNumber)
  {
    LOG_TRACE("Updating word: " << word << " with kvp: " << kvp << "...");

    const QString lowerCaseWord = word.toLower();
    const QString queriedWord = _wordCaseMappings.value(lowerCaseWord, "");
    if (queriedWord.isEmpty())
    {
      _wordCaseMappings[lowerCaseWord] = word;
    }
    else
    {
      word = queriedWord;
    }

    const QString line = word % QString("\t") % kvp % QString("\n");
    _countFile->write(line.toUtf8());
  }
  else
  {
    if (wordTooSmall)
    {
      LOG_TRACE(
        "Skipping word: " << word << ", the length of which is less than the minimum allowed word " <<
        "length of: " << _minWordLength);
    }
    if (wordIsNumber)
    {
      LOG_TRACE("Skipping word: " << word << ", which is a number.");
    }
  }
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
  long elementCount = 0;
  for (int i = 0; i < inputs.size(); i++)
  {
    const QString input = inputs.at(i);
    LOG_INFO("Parsing: " << input << "...");
    const bool inputIsPbf = input.endsWith(".pbf");

    boost::shared_ptr<PartialOsmMapReader> inputReader =
      boost::dynamic_pointer_cast<PartialOsmMapReader>(
        OsmMapReaderFactory::getInstance().createReader(input));
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
                    //may need to replace more punctuation chars here
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
      //nodes are parsed before other elements by the pbf reader, so we can get away with this
      else if (inputIsPbf)
      {
        LOG_INFO("Reached end of PBF nodes for input: " << input << ".");
        break;
      }

      elementCount++;

      if (elementCount % (_statusUpdateInterval * 1000) == 0)
      {
        PROGRESS_INFO(
          "Parsed " << StringUtils::formatLargeNumber(elementCount) << " elements from input.");
      }
    }
    inputReader->finalizePartial();
  }
  _countFile->close();
  LOG_VARD(_wordCaseMappings.size());
  _wordCaseMappings.clear();

  LOG_INFO(
    "Parsed " << StringUtils::formatLargeNumber(poiCount) << " POIs from " <<
    StringUtils::formatLargeNumber(nodeCount) << " nodes.");

  _removeKvpsBelowOccuranceThresholdAndSortByOccurrance(minOccurancesThreshold);
  _removeDuplicatedKeyTypes();
  _sortByWord();

  LOG_INFO(
    "Extracted "  << StringUtils::formatLargeNumber(_wordKeysToCounts.size()) <<
    " word/tag associations.");
  _wordKeysToCounts.clear();

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
  QString msg;
  if (minOccurancesThreshold > 1)
  {
    msg +=
      "Removing tags below minimum occurance threshold of: " +
      QString::number(minOccurancesThreshold) + " and ";
  }
  msg += "sorting by tag occurrance count...";
  LOG_INFO(msg);

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
  QString cmd = "sort " + _countFile->fileName() + " | uniq -c | sort -n -r";
  if (minOccurancesThreshold > 1)
  {
    cmd += " | awk -v limit=" + QString::number(minOccurancesThreshold - 1) + " '$1 > limit{print}'";
  }
  cmd += " | sed -e 's/^ *//;s/ /\t/' > " + _sortedCountFile->fileName();
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
  _sortedDedupedCountFile->setAutoRemove(false); //for debugging only
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

    //TODO: remove and add rules
    //QMap<QString, QString> _customRulesList;
    //QMap<QString, QString> _rulesIgnoreList;

    //The lines are sorted by occurrence count.  So the first time we see one word-key combo, we
    //know it had the highest occurrence count, and we can ignore all subsequent instances since
    //any one feature can't have more than one tag applied to it with the same key.

    const long queriedCount = _wordKeysToCounts.value(wordTagKey, -1);
    if (queriedCount == -1)
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
    else if (queriedCount == count)
    {
      //TODO: fix
      LOG_DEBUG(
        "Found word with multiple tag occurrance counts of the same size.  Arbitrarily " <<
        "chose first tag.  Not creating implicit tag entry for word: " << word << ", tag: " <<
        kvp << ", count: " << count);
    }
  }
  _sortedCountFile->close();
  _sortedDedupedCountFile->close();
}

void PoiImplicitTagRulesDeriver::_sortByWord()
{
  LOG_INFO("Sorting output file...");

  _sortedByWordDedupedCountFile.reset(
    new QTemporaryFile(
      ConfigOptions().getApidbBulkInserterTempFileDir() +
      "/poi-implicit-tag-rules-deriver-temp-XXXXXX"));
  _sortedByWordDedupedCountFile->setAutoRemove(false); //for debugging only
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
}
