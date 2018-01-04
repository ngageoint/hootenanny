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
#include <hoot/core/elements/Tags.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/algorithms/string/StringTokenizer.h>
#include <hoot/rnd/io/ImplicitTagRulesSqliteRecordWriter.h>

// Qt
#include <QStringBuilder>
#include <QRegExp>

namespace hoot
{

PoiImplicitTagRulesDeriver::PoiImplicitTagRulesDeriver() :
_statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
_minTagOccurrencesPerWord(1),
_minWordLength(1),
_useSchemaTagValuesForWordsOnly(true),
_translateAllNamesToEnglish(true)
{
}

void PoiImplicitTagRulesDeriver::setConfiguration(const Settings& conf)
{
  const ConfigOptions confOptions(conf);
  _customRules.setCustomRuleFile(confOptions.getPoiImplicitTagRulesCustomRuleFile());
  setMinTagOccurrencesPerWord(confOptions.getPoiImplicitTagRulesMinimumTagOccurrencesPerWord());
  setMinWordLength(confOptions.getPoiImplicitTagRulesMinimumWordLength());
  _customRules.setTagIgnoreFile(confOptions.getPoiImplicitTagRulesTagIgnoreFile());
  _customRules.setWordIgnoreFile(confOptions.getPoiImplicitTagRulesWordIgnoreFile());
  setUseSchemaTagValuesForWordsOnly(
    confOptions.getPoiImplicitTagRulesUseSchemaTagValuesForWordsOnly());
  setTranslateAllNamesToEnglish(confOptions.getPoiImplicitTagRulesTranslateAllNamesToEnglish());
}

void PoiImplicitTagRulesDeriver::deriveRules(const QString input, const QString output)
{
  if (input.isEmpty())
  {
    throw HootException("No input was specified.");
  }
  if (!input.endsWith(".implicitTagRules"))
  {
    throw IllegalArgumentException(
      QString("A *.implicitTagRules file must be the input to implicit tag rules derivation.  ") +
      QString("Input specified: ") + input);
  }

  if (!output.endsWith(".sqlite"))
  {
    throw HootException(
      "Incorrect output specified: " + output + ".  Must be a .sqlite database file.");
  }

  LOG_INFO(
    "Deriving POI implicit tag rules for input: " << input << ".  Writing to output: " <<
    output << "...");

  LOG_VARD(_minTagOccurrencesPerWord);
  LOG_VARD(_minWordLength);
  LOG_VARD(_customRules.getWordIgnoreFile());
  LOG_VARD(_customRules.getTagIgnoreFile());
  LOG_VARD(_customRules.getCustomRuleFile());
  LOG_VARD(_useSchemaTagValuesForWordsOnly);

  _customRules.init();

  LOG_VARD(_customRules.getWordIgnoreList().size());
  LOG_VARD(_customRules.getWordIgnoreList());
  LOG_VARD(_customRules.getTagIgnoreList().size());
  LOG_VARD(_customRules.getTagIgnoreList());
  LOG_VARD(_customRules.getCustomRulesList().size());
  LOG_VARD(_customRules.getCustomRulesList());

  if (_useSchemaTagValuesForWordsOnly)
  {
    _schemaTagValues.clear();
    _wordsNotInSchema.clear();
    const std::vector<SchemaVertex> tags =
      OsmSchema::getInstance().getTagByCategory(OsmSchemaCategory::poi());
    StringTokenizer tokenizer;
    for (std::vector<SchemaVertex>::const_iterator tagItr = tags.begin();
         tagItr != tags.end(); ++tagItr)
    {
      SchemaVertex tag = *tagItr;
      const QString tagVal = tag.value.toLower().replace("_", " ");
      if (!tagVal.contains("*"))
      {
        if (!_customRules.getWordIgnoreList().contains(tagVal, Qt::CaseInsensitive))
        {
          _schemaTagValues.insert(tagVal);
          if (tagVal == "theatre")
          {
            _schemaTagValues.insert("theater");
          }
          if (tagVal == "centre")
          {
            _schemaTagValues.insert("center");
          }
          //TODO: add aliases?
          LOG_TRACE("Appended " << tagVal << " to schema tag values.");
        }
        QStringList vals = tokenizer.tokenize(tagVal);
        for (int i = 0; i < vals.size(); i++)
        {
          const QString val = vals.at(i);
          if (!_customRules.getWordIgnoreList().contains(val, Qt::CaseInsensitive))
          {
            _schemaTagValues.insert(val);
            if (val == "theatre")
            {
              _schemaTagValues.insert("theater");
            }
            if (val == "centre")
            {
              _schemaTagValues.insert("center");
            }
            LOG_TRACE("Appended " << val << " to schema tag values.");
          }
        }
      }
    }
    LOG_VARD(_schemaTagValues.size());
    QStringList schemaTagValuesList = _schemaTagValues.toList();
    qSort(schemaTagValuesList.begin(), schemaTagValuesList.end());
    LOG_VART(schemaTagValuesList);
  }

  if (_minTagOccurrencesPerWord == 1 && _minWordLength == 1 &&
      _customRules.getWordIgnoreList().size() == 0 && _customRules.getTagIgnoreList().size() == 0 &&
      _customRules.getCustomRulesList().size() == 0 && !_useSchemaTagValuesForWordsOnly)
  {
    LOG_INFO("Skipping filtering as no filtering criteria were specified...");
    if (_minTagOccurrencesPerWord >= 2)
    {
      _removeKvpsBelowOccurrenceThreshold(input, _minTagOccurrencesPerWord);
      _writeRules(_thresholdedCountFile->fileName(), output);
    }
    else
    {
      LOG_INFO("Skipping count thresholding since threshold = 1...");
      _writeRules(input, output);
    }
  }
  else
  {
    if (_minTagOccurrencesPerWord >= 2)
    {
      _removeKvpsBelowOccurrenceThreshold(input, _minTagOccurrencesPerWord);
      _applyFiltering(_thresholdedCountFile->fileName());
    }
    else
    {
      LOG_INFO("Skipping count thresholding since threshold = 1...");
      _applyFiltering(input);
    }

    _writeRules(_filteredCountFile->fileName(), output);
  }
}

void PoiImplicitTagRulesDeriver::_writeRules(const QString input, const QString output)
{
  LOG_VART(output);
  ImplicitTagRulesSqliteRecordWriter ruleWordPartWriter;
  ruleWordPartWriter.open(output);
  ruleWordPartWriter.write(input);
  ruleWordPartWriter.close();
}

void PoiImplicitTagRulesDeriver::_removeKvpsBelowOccurrenceThreshold(const QString input,
  const int minOccurrencesThreshold)
{
  LOG_INFO("Removing tags below minimum occurrence threshold of: " +
           QString::number(minOccurrencesThreshold) << "...");

  _thresholdedCountFile.reset(
    new QTemporaryFile(
      ConfigOptions().getApidbBulkInserterTempFileDir() +
      "/poi-implicit-tag-rules-deriver-temp-XXXXXX"));
  _thresholdedCountFile->setAutoRemove(!ConfigOptions().getPoiImplicitTagRulesKeepTempFiles());
  if (!_thresholdedCountFile->open())
  {
    throw HootException(
      QObject::tr("Error opening %1 for writing.").arg(_thresholdedCountFile->fileName()));
  }
  LOG_DEBUG("Opened thresholded temp file: " << _thresholdedCountFile->fileName());
  if (ConfigOptions().getPoiImplicitTagRulesKeepTempFiles())
  {
    LOG_WARN("Keeping temp file: " << _thresholdedCountFile->fileName());
  }

  //This removes lines with occurrence counts below the specified threshold - not sure why 1 needs
  //to be subtracted from the min occurrences here, though...
  const QString cmd =
    "cat " + input + " | awk -v limit=" + QString::number(minOccurrencesThreshold - 1) +
    " '$1 > limit{print}' > " + _thresholdedCountFile->fileName();
  LOG_DEBUG(cmd);
  if (std::system(cmd.toStdString().c_str()) != 0)
  {
    throw HootException("Unable to sort input file.");
  }
  _thresholdedCountFile->close();
}

void PoiImplicitTagRulesDeriver::_applyFiltering(const QString input)
{
  LOG_INFO("Applying word/tag/rule filtering to output...");

  _filteredCountFile.reset(
    new QTemporaryFile(
      ConfigOptions().getApidbBulkInserterTempFileDir() +
      "/poi-implicit-tag-rules-deriver-temp-XXXXXX"));
  _filteredCountFile->setAutoRemove(!ConfigOptions().getPoiImplicitTagRulesKeepTempFiles());
  if (!_filteredCountFile->open())
  {
    throw HootException(
      QObject::tr("Error opening %1 for writing.").arg(_filteredCountFile->fileName()));
  }
  LOG_DEBUG("Opened filtered temp file: " << _filteredCountFile->fileName());
  if (ConfigOptions().getPoiImplicitTagRulesKeepTempFiles())
  {
    LOG_WARN("Keeping temp file: " << _filteredCountFile->fileName());
  }
  QFile inputFile(input);
  if (!inputFile.open(QIODevice::ReadOnly))
  {
    throw HootException(QObject::tr("Error opening %1 for reading.").arg(input));
  }
  LOG_DEBUG("Opened input file: " << input);

  long linesParsedCount = 0;
  long linesWrittenCount = 0;
  long wordsTooSmallCount = 0;
  long ignoredWordsCount = 0;
  long ignoredTagsCount = 0;
  long ignoredRuleCountDueToCustomRules = 0;
  long wordNotASchemaValueCount = 0;
  long wordNotInCommonEnglishList = 0;

  StringTokenizer tokenizer;
  while (!inputFile.atEnd())
  {
    const QString line = QString::fromUtf8(inputFile.readLine().constData()).trimmed();
    LOG_VART(line);
    const QStringList lineParts = line.split("\t");
    LOG_VART(lineParts);
    QString word = lineParts[1].trimmed();
    LOG_VART(word);

    //TODO: move to generator - FIX
    //word = StringUtils::replaceNonAlphaNumericCharsWithSpace(word).trimmed();

    //TODO: move to common class and call from generator
//    word =
//      word.replace("(", "").replace(")", "").replace(".", "").replace("/", " ").replace("<", "")
//          .replace(">", "").replace("[", "").replace("]", "").replace("@", "").replace("&", "and");
//    if (word.startsWith("-"))
//    {
//      word = word.replace(0, 1, "");
//    }
//    if (word.startsWith("_"))
//    {
//      word = word.replace(0, 1, "");
//    }
    //TOOD: expand this
//    if (word.at(0).isDigit() &&
//        (word.endsWith("th") || word.endsWith("nd") || word.endsWith("rd") || word.endsWith("st") ||
//         word.endsWith("ave") || word.endsWith("avenue") || word.endsWith("st") ||
//         word.endsWith("street") || word.endsWith("pl") || word.endsWith("plaza")))
//    {
//      word = "";
//    }

    //TODO: fix and add to base visitor
//    const QStringList wordTokens = word.split(" ");
//    if (wordTokens.size() == 1)
//    {
//       QRegExp addressStart1Regex("\d{1,5}[a-z]{1}");
//       if (addressStart1Regex.exactMatch(word))
//       {
//         word = "";
//       }
//    }

//    if (_translateAllNamesToEnglish && _useSchemaTagValuesForWordsOnly &&
//        !_englishWords.contains(word))
//    {
//      word = "";
//      wordNotInCommonEnglishList++;
//    }

    bool wordNotASchemaTagValue = false;
    if (_useSchemaTagValuesForWordsOnly && !word.trimmed().isEmpty() &&
        !_customRules.getWordIgnoreList().contains(word, Qt::CaseInsensitive))
    {
      wordNotASchemaTagValue = true;
      const QStringList tokenizedWords = tokenizer.tokenize(word.toLower());
      for (int i = 0; i < tokenizedWords.size(); i++)
      {
        const QString tokenizedWord = tokenizedWords.at(i);
        if (_schemaTagValues.contains(tokenizedWord) &&
            !_customRules.getWordIgnoreList().contains(tokenizedWord, Qt::CaseInsensitive))
        {
          wordNotASchemaTagValue = false;
          break;
        }
      }
    }

    const bool wordTooSmall = word.length() < _minWordLength;
    if (!wordTooSmall && !_customRules.getWordIgnoreList().contains(word, Qt::CaseInsensitive) &&
        !wordNotASchemaTagValue)
    {
      const QString kvp = lineParts[2].trimmed();
      LOG_VART(kvp);
      const QString tagKey = kvp.split("=")[0];
      LOG_VART(tagKey);

      const QString keyWildCard = tagKey % "=*";
      LOG_VART(keyWildCard);

      const QStringList tagIgnoreList = _customRules.getTagIgnoreList();
      const bool ignoreTag =
        !tagIgnoreList.isEmpty() &&
        (tagIgnoreList.contains(kvp) || tagIgnoreList.contains(keyWildCard));
      LOG_VART(ignoreTag);

      if (!ignoreTag)
      {
        const QString customRuleTag = _customRules.getCustomRulesList().value(word.toLower(), "");
        if (customRuleTag == kvp)
        {
          LOG_TRACE(
            "Skipping word/tag combo on custom rule list.  Word: " << word << ", tag: " <<
            kvp << ".");
          ignoredRuleCountDueToCustomRules++;
        }
        else
        {
          const long count = lineParts[0].trimmed().toLong();
          LOG_VART(count);
          const QString line = QString::number(count) % "\t" % word % "\t" % kvp % "\n";
          LOG_VART(line);
          _filteredCountFile->write(line.toUtf8());
          linesWrittenCount++;
        }
      }
      else
      {
        if (ignoreTag)
        {
          LOG_TRACE("Skipping tag on the ignore list: " << kvp << ".");
        }
        else
        {
          LOG_TRACE("Skipping tag not on the include list: " << kvp << ".");
        }
        ignoredTagsCount++;
      }
    }
    else
    {
      if (wordTooSmall)
      {
        LOG_TRACE(
          "Skipping word: " << word <<
          ", the length of which is less than the minimum allowed word length of: " <<
          _minWordLength);
        wordsTooSmallCount++;
      }
      else if (wordNotASchemaTagValue)
      {
        LOG_TRACE(
          "Schema tag value requirement for word is being enforced and word is not a schema " <<
          "tag value: " << word.toLower() << ".");
        _wordsNotInSchema.insert(word.toLower());
        wordNotASchemaValueCount++;
      }
      else
      {
        LOG_TRACE("Skipping word on the ignore list: " << word << ".");
        ignoredWordsCount++;
      }
    }

    linesParsedCount++;
    if (linesParsedCount % (_statusUpdateInterval * 100) == 0)
    {
      PROGRESS_INFO(
        "Filtered " << StringUtils::formatLargeNumber(linesParsedCount) <<
        " count file lines from input.");
    }
  }
  inputFile.close();

  LOG_INFO("Parsed " << StringUtils::formatLargeNumber(linesParsedCount) << " words.");
  LOG_INFO(
    "Skipped " << StringUtils::formatLargeNumber(wordsTooSmallCount) <<
    " words that were too small.");
  LOG_INFO("Ignored " << StringUtils::formatLargeNumber(ignoredWordsCount) << " words.");
  LOG_INFO("Ignored " << StringUtils::formatLargeNumber(ignoredTagsCount) << " tags.");
  LOG_INFO(
    "Ignored " << StringUtils::formatLargeNumber(ignoredRuleCountDueToCustomRules) <<
    " rules due to them overlapping with the custom rules list.");
  LOG_INFO(
    "Skipped " << StringUtils::formatLargeNumber(wordNotASchemaValueCount) <<
    " words that were not a schema value.");
  if (wordNotASchemaValueCount > 0)
  {
    QStringList wordsNotInSchemaList = _wordsNotInSchema.toList();
    qSort(wordsNotInSchemaList.begin(), wordsNotInSchemaList.end());
    LOG_VART(wordsNotInSchemaList);
  }
  if (wordNotInCommonEnglishList > 0)
  {
    LOG_INFO(
      "Skipped " << StringUtils::formatLargeNumber(wordNotInCommonEnglishList) <<
      " words that were not in the common English words list.");
  }

  LOG_DEBUG("Writing custom rules...");
  long ruleCount = 0;
  LOG_VARD(_customRules.getCustomRulesList().size());
  if (_customRules.getCustomRulesList().size() > 0)
  {
    const QMap<QString, QString> customRulesList = _customRules.getCustomRulesList();
    for (QMap<QString, QString>::const_iterator customRulesItr = customRulesList.begin();
         customRulesItr != customRulesList.end(); ++customRulesItr)
    {
      const QString line =
        QString::number(999999) % "\t" % customRulesItr.key().trimmed() % "\t" %
        customRulesItr.value().trimmed() % "\n";
      LOG_VART(line);
      _filteredCountFile->write(line.toUtf8());
      ruleCount++;
      linesWrittenCount++;
    }
    LOG_INFO("Wrote " << ruleCount << " custom rules.");
  }

  LOG_INFO(
    "Wrote " << StringUtils::formatLargeNumber(linesWrittenCount) << " / " <<
     StringUtils::formatLargeNumber(linesParsedCount) << " lines to filtered file.");

  _filteredCountFile->close();
}

}
