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

void PoiImplicitTagRulesDeriver::_updateForNewWord(QString word, const QString kvp)
{
  LOG_TRACE("Updating word: " << word << " with kvp: " << kvp << "...");

  //FixedLengthString fixedLengthWord = _qStrToFixedLengthStr(word);
  const QString lowerCaseWord = word.toLower();
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

  const QString line = word % QString("\t") % kvp % QString("\n");
  _countFile->write(line.toUtf8());
}

bool PoiImplicitTagRulesDeriver::_outputsContainsSqlite(const QStringList outputs)
{
  bool containsSqlite = false;
  for (int i = 0; i < outputs.size(); i++)
  {
    if (outputs.at(i).endsWith(".sqlite"))
    {
      containsSqlite = true;
    }
  }
  return containsSqlite;
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

  _countFile.reset(
    new QTemporaryFile(
      ConfigOptions().getApidbBulkInserterTempFileDir() +
      "/poi-implicit-tag-rules-deriver-temp-XXXXXX"));
  _countFile->setAutoRemove(false); //for debugging only
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
              //'=' is used in the map key for kvps, so it needs to be escaped in the word
              if (name.contains("="))
              {
                name = name.replace("=", "%3D");
              }
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

        if (nodeCount % (_statusUpdateInterval * 100) == 0)
        {
          PROGRESS_INFO(
            "Parsed " << StringUtils::formatLargeNumber(nodeCount) << " nodes from input.");
        }
      }
    }
    inputReader->finalizePartial();
  }
  LOG_VARD(_wordCaseMappings.size());
  _wordCaseMappings.clear();

  _removeKvpsBelowOccuranceThresholdAndSortByWord(minOccurancesThreshold);
  _removeDuplicatedKeyTypes();

//  LOG_INFO(
//    "Generated " << StringUtils::formatLargeNumber(_tagRules.size()) <<
//    " implicit tag rules for " << StringUtils::formatLargeNumber(_tagRulesByWord.size()) <<
//    " unique words and " << StringUtils::formatLargeNumber(poiCount) << " POIs (" <<
//    StringUtils::formatLargeNumber(nodeCount) << " nodes parsed).");
//  LOG_INFO("Average words per rule: " << _avgWordsPerRule);
//  LOG_INFO("Average tags per rule: " << _avgTagsPerRule);
//  LOG_INFO("Highest rule word count: " << _highestRuleWordCount);
//  LOG_INFO("Highest rule tag count: " << _highestRuleTagCount);

  _writeRules(outputs, _sortedDedupedCountFile->fileName());
}

//QList<boost::shared_ptr<ImplicitTagRuleWordPartWriter> > PoiImplicitTagRulesDeriver::_getOutputWriters(
//  const QStringList outputs)
//{
//  QList<boost::shared_ptr<ImplicitTagRuleWordPartWriter> > ruleWordPartWriters;
//  for (int i = 0; i < outputs.size(); i++)
//  {
//    const QString output = outputs.at(i);
//    LOG_VART(output);
//    boost::shared_ptr<ImplicitTagRuleWordPartWriter> ruleWordPartWriter =
//      ImplicitTagRuleWordPartWriterFactory::getInstance().createWriter(output);
//    ruleWordPartWriter->open(output);
//    ruleWordPartWriters.append(ruleWordPartWriter);
//  }
//  return ruleWordPartWriters;
//}

void PoiImplicitTagRulesDeriver::_writeRules(const QStringList outputs,
                                             const QString inputFile)
{
//  QList<boost::shared_ptr<ImplicitTagRuleWordPartWriter> > ruleWordPartWriters =
//    _getOutputWriters(outputs);
//  LOG_VART(ruleWordPartWriters.size());
//  for (QList<boost::shared_ptr<ImplicitTagRuleWordPartWriter> >::iterator writersItr = ruleWordPartWriters.begin();
//       writersItr != ruleWordPartWriters.end(); ++writersItr)
//  {
//    boost::shared_ptr<ImplicitTagRuleWordPartWriter> rulesWriter = *writersItr;
//    rulesWriter->write(inputFile);
//    rulesWriter->close();
//  }

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

void PoiImplicitTagRulesDeriver::_removeKvpsBelowOccuranceThresholdAndSortByWord(
  const int minOccurancesThreshold)
{
  _sortedCountFile.reset(
    new QTemporaryFile(
      ConfigOptions().getApidbBulkInserterTempFileDir() +
      "/poi-implicit-tag-rules-deriver-temp-XXXXXX"));
  _sortedCountFile->setAutoRemove(false); //for debugging only
  if (!_sortedCountFile->open())
  {
    throw HootException(
      QObject::tr("Error opening %1 for writing.").arg(_sortedCountFile->fileName()));
  }
  LOG_DEBUG("Opened sorted temp file: " << _sortedCountFile->fileName());

  //This counts each unique line occurrance, sorts alphabetically on the second column (word),
  //removes lines with occurrance counts below the specified threshold, and replaces the space
  //between the prepended count and the word with a tab. - not sure why 1 needs to be subtracted
  //from minOccurancesThreshold here, though...

  const QString cmd =
    "sort " + _countFile->fileName() + " | uniq -c | sort -k2,2 | awk -v limit=" +
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
  //i.e. don't allow amenity=school AND amenity=building in the same rule

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
    const QString line = QString::fromUtf8(_sortedCountFile->readLine().constData());
    LOG_VART(line);
    const QStringList lineParts = line.split("\t");
    LOG_VART(lineParts);
    QString word = lineParts[1];
    LOG_VART(word);
    const QString kvp = lineParts[2];
    LOG_VART(kvp);
    const long count = lineParts[0].toLong();
    LOG_VART(count);
    const QString key = kvp.split("=")[0];
    LOG_VART(key);
    const QString wordKey = word % ";" % key;
    LOG_VART(wordKey);

    //TODO: in case of ties, pick the more specific tag (?)

    //The lines are sorted by occurrence count.  So the first time we see one word-key combo, we
    //know it had the highest occurrence count, and we can ignore all subsequent instances since
    //any one feature can't have more than one tag applied to it with the same key.
    if (!_wordKeysToCounts.contains(wordKey))
    {
      _wordKeysToCounts[wordKey] = count;
      //this unescaping must occur during the final temp file write
      if (word.contains("%3D"))
      {
        word = word.replace("%3D", "=");
      }
      else if (word.contains("%3d"))
      {
        word = word.replace("%3d", "=");
      }
      const QString updatedLine = QString::number(count) % "\t" % word % "\t" % kvp;
      LOG_VART(updatedLine);
      _sortedDedupedCountFile->write(updatedLine.toUtf8());
    }
  }
  _sortedCountFile->close();
  _sortedDedupedCountFile->close();
}

}
