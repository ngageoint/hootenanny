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
#include "PoiImplicitTagRawRulesGenerator.h"

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
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/algorithms/Translator.h>

// Qt
#include <QStringBuilder>
#include <QThread>

namespace hoot
{

PoiImplicitTagRawRulesGenerator::PoiImplicitTagRawRulesGenerator() :
_statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
_tokenizeNames(true),
_countFileLineCtr(0),
_sortParallelCount(QThread::idealThreadCount()),
_skipFiltering(false),
_skipTranslation(false),
_keepTempFiles(false),
_tempFileDir(ConfigOptions().getApidbBulkInserterTempFileDir()),
_translateAllNamesToEnglish(false),
_maxWordTokenizationGroupSize(1)
{
}

void PoiImplicitTagRawRulesGenerator::setConfiguration(const Settings& conf)
{
  ConfigOptions options = ConfigOptions(conf);
  setTokenizeNames(options.getPoiImplicitTagRulesTokenizeNames());
  setSortParallelCount(options.getPoiImplicitTagRulesSortParallelCount());
  const int idealThreads = QThread::idealThreadCount();
  LOG_VART(idealThreads);
  if (_sortParallelCount < 1 || _sortParallelCount > idealThreads)
  {
    setSortParallelCount(idealThreads);
  }
  setSkipFiltering(options.getPoiImplicitTagRulesSkipFiltering());
  setSkipTranslation(options.getPoiImplicitTagRulesSkipTranslation());
  setKeepTempFiles(options.getPoiImplicitTagRulesKeepTempFiles());
  setTempFileDir(options.getApidbBulkInserterTempFileDir());
  setTranslateAllNamesToEnglish(options.getPoiImplicitTagRulesTranslateAllNamesToEnglish());
  setMaxWordTokenizationGroupSize(options.getPoiImplicitTagRulesMaximumWordTokenizationGroupSize());
}

void PoiImplicitTagRawRulesGenerator::_updateForNewWord(QString word, const QString kvp)
{
  word = word.simplified();
  LOG_TRACE("Updating word: " << word << " with kvp: " << kvp << "...");

  bool wordIsNumber = false;
  word.toLong(&wordIsNumber);
  if (wordIsNumber)
  {
    LOG_TRACE("Skipping word: " << word << ", which is a number.");
    return;
  }

  const bool hasAlphaChar = StringUtils::hasAlphabeticCharacter(word);
  if (!hasAlphaChar)
  {
    LOG_TRACE("Skipping word: " << word << ", which has no alphabetic characters.");
    return;
  }

  const QString line = word.toLower() % QString("\t") % kvp % QString("\n");
  _countFile->write(line.toUtf8());
  _countFileLineCtr++;
}

void PoiImplicitTagRawRulesGenerator::generateRules(const QStringList inputs,
                                                    const QStringList translationScripts,
                                                    const QString output)
{
  if (inputs.isEmpty())
  {
    throw HootException("No inputs were specified.");
  }
  if (inputs.size() != translationScripts.size())
  {
    LOG_VARD(inputs.size());
    LOG_VARD(translationScripts.size());
    throw HootException(
      "The size of the input datasets list must equal the size of the list of translation scripts.");
  }
  if (output.isEmpty())
  {
    throw HootException("No output was specified.");
  }
  _output.reset(new QFile());
  _output->setFileName(output);
  if (_output->exists() && !_output->remove())
  {
    throw HootException(QObject::tr("Error removing existing %1 for writing.").arg(output));
  }
  _output->close();

  LOG_INFO(
    "Generating POI implicit tag rules raw file for inputs: " << inputs <<
    ", translation scripts: " << translationScripts << ".  Writing to output: " << output << "...");
  LOG_VARD(_tokenizeNames);
  LOG_VARD(_sortParallelCount);
  LOG_VARD(_skipFiltering);
  LOG_VARD(_skipTranslation);
  LOG_VARD(_sortParallelCount);
  LOG_VARD(_translateAllNamesToEnglish);
  LOG_VARD(_maxWordTokenizationGroupSize);

  _wordKeysToCountsValues.clear();
  _duplicatedWordTagKeyCountsToValues.clear();
  _countFileLineCtr = 0;

  _countFile.reset(
    new QTemporaryFile(_tempFileDir + "/poi-implicit-tag-raw-rules-generator-temp-XXXXXX"));
  _countFile->setAutoRemove(!_keepTempFiles);
  if (!_countFile->open())
  {
    throw HootException(QObject::tr("Error opening %1 for writing.").arg(_countFile->fileName()));
  }
  LOG_DEBUG("Opened temp file: " << _countFile->fileName());
  if (_keepTempFiles)
  {
    LOG_WARN("Keeping temp file: " << _countFile->fileName());
  }

  long poiCount = 0;
  long nodeCount = 0;
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
    const QString translationScript = translationScripts.at(i);
    if (!_skipTranslation && translationScript.toLower() != "none")
    {
      boost::shared_ptr<TranslationVisitor> translationVisitor(new TranslationVisitor());
      translationVisitor->setPath(translationScript);
      inputStream.reset(new ElementVisitorInputStream(inputReader, translationVisitor));
    }

    StringTokenizer tokenizer;
    while (inputStream->hasMoreElements())
    {
      ElementPtr element = inputStream->readNextElement();

      if (element->getElementType() != ElementType::Node && inputIsPbf)
      {
        LOG_INFO("Reached end of PBF nodes for input: " << input << ".");
        break;
      }

      nodeCount++;

      //TODO: _skipFiltering may go away
      if (_skipFiltering || _poiFilter.isSatisfied(element))
      {
        QStringList names = element->getTags().getNames();
        assert(!names.isEmpty());
        if (names.isEmpty())
        {
          throw HootException("Names empty.");
        }

        if (_translateAllNamesToEnglish)
        {
          QStringList filteredNames;
          if (element->getTags().contains("name:en"))
          {
            filteredNames.append(element->getTags().get("name:en"));
          }
          else
          {
            for (int i = 0; i < names.size(); i++)
            {
              const QString name = names.at(i);
              LOG_VART(name);
              if (name != element->getTags().get("alt_name"))
              {
                const QString englishName = Translator::getInstance().toEnglish(name);
                LOG_VART(englishName);
//                if (englishName.toLower() != name.toLower().replace("-", " ").replace("'", " "))
//                {
//                  LOG_TRACE(
//                    "Successfully translated " << name << " to " << englishName << ".");
//                }
                filteredNames.append(englishName);
                break;
              }
            }
            if (filteredNames.isEmpty() && element->getTags().contains("alt_name"))
            {
              QString altName = element->getTags().get("alt_name");
              if (altName.contains(";"))
              {
                altName = altName.split(";")[0];
              }
              LOG_VART(altName);
              const QString englishName = Translator::getInstance().toEnglish(altName);
//              if (englishName.toLower() != altName.toLower().replace("-", " ").replace("'", " "))
//              {
//                LOG_TRACE(
//                  "Successfully translated " << altName << " to " << englishName << ".");
//              }
              LOG_VART(englishName);
              filteredNames.append(englishName);
            }
          }
          LOG_VART(filteredNames);
          assert(!filteredNames.isEmpty());
          names = filteredNames;
        }
        LOG_VART(names);

        const QStringList kvps = ImplicitTagEligiblePoiCriterion::getPoiKvps(element->getTags());
        assert(!kvps.isEmpty());
        if (kvps.isEmpty())
        {
          throw HootException("POI kvps empty.");
        }

        for (int i = 0; i < names.size(); i++)
        {
          QString name = names.at(i);
          LOG_VART(name);
          //TODO: replace name multiple spaces with one?

          //'=' is used in the map key for kvps, so it needs to be escaped in the word
          if (name.contains("="))
          {
            name = name.replace("=", "%3D");
          }
          for (int j = 0; j < kvps.size(); j++)
          {
            _updateForNewWord(name, kvps.at(j));
          }

          if (_tokenizeNames)
          {
            QStringList nameTokens = tokenizer.tokenize(name);
            LOG_VART(nameTokens.size());

            //tokenization - we'll just do 1, 2, 3 for now

            for (int j = 0; j < nameTokens.size(); j++)
            {
              QString nameToken = nameTokens.at(j);
              //TODO: may need to replace more punctuation chars here - replace all non-alphanumeric?
              //esp ()/
              nameToken = nameToken.replace(",", "");
              LOG_VART(nameToken);

              if (_translateAllNamesToEnglish)
              {
                const QString englishNameToken = Translator::getInstance().toEnglish(nameToken);
//                if (englishNameToken.toLower() != nameToken.toLower().replace("-", " ").replace("'", " "))
//                {
//                  LOG_TRACE(
//                    "Successfully translated " << nameToken << " to " << englishNameToken << ".");
//                }
                nameToken = englishNameToken;
                LOG_VART(englishNameToken);
              }

              for (int k = 0; k < kvps.size(); k++)
              {
                _updateForNewWord(nameToken, kvps.at(k));
              }
            }

            if (nameTokens.size() > 2 && _maxWordTokenizationGroupSize >= 2)
            {
              for (int j = 0; j < nameTokens.size() - 1; j++)
              {
                QString nameToken = nameTokens.at(j) + " " + nameTokens.at(j + 1);
                //TODO: may need to replace more punctuation chars here
                nameToken = nameToken.replace(",", "");
                LOG_VART(nameToken);

                if (_translateAllNamesToEnglish)
                {
                  const QString englishNameToken = Translator::getInstance().toEnglish(nameToken);
//                  if (englishNameToken.toLower() != nameToken.toLower().replace("-", " ").replace("'", " "))
//                  {
//                    LOG_TRACE(
//                      "Successfully translated " << nameToken << " to " << englishNameToken << ".");
//                  }
                  nameToken = englishNameToken;
                  LOG_VART(englishNameToken);
                }

                for (int k = 0; k < kvps.size(); k++)
                {
                  _updateForNewWord(nameToken, kvps.at(k));
                }
              }
            }

            if (nameTokens.size() > 3 && _maxWordTokenizationGroupSize >= 3)
            {
              for (int j = 0; j < nameTokens.size() - 2; j++)
              {
                QString nameToken =
                  nameTokens.at(j) + " " + nameTokens.at(j + 1) + " " + nameTokens.at(j + 2);
                //TODO: may need to replace more punctuation chars here
                nameToken = nameToken.replace(",", "");
                LOG_VART(nameToken);

                if (_translateAllNamesToEnglish)
                {
                  const QString englishNameToken = Translator::getInstance().toEnglish(nameToken);
//                  if (englishNameToken.toLower() != nameToken.toLower().replace("-", " ").replace("'", " "))
//                  {
//                    LOG_TRACE(
//                      "Successfully translated " << nameToken << " to " << englishNameToken << ".");
//                  }
                  nameToken = englishNameToken;
                  LOG_VART(englishNameToken);
                }

                for (int k = 0; k < kvps.size(); k++)
                {
                  _updateForNewWord(nameToken, kvps.at(k));
                }
              }
            }
          }
        }

        poiCount++;

        if (poiCount % _statusUpdateInterval == 0)
        {
          PROGRESS_INFO(
            "Parsed " << StringUtils::formatLargeNumber(poiCount) << " eligible POIs / " <<
            StringUtils::formatLargeNumber(nodeCount) << " nodes.");
        }
      }
    }
    inputReader->finalizePartial();
  }
  _countFile->close();

  LOG_INFO(
    "Parsed " << StringUtils::formatLargeNumber(poiCount) << " POIs from " <<
    StringUtils::formatLargeNumber(nodeCount) << " nodes.");
  LOG_INFO(
    "Wrote " << StringUtils::formatLargeNumber(_countFileLineCtr) << " lines to count file.");

  _sortByTagOccurrence();
  _removeDuplicatedKeyTypes();
  bool tieCountsNeededResolved = false;
  if (_duplicatedWordTagKeyCountsToValues.size() > 0)
  {
    _resolveCountTies();
    tieCountsNeededResolved = true;
  }
  LOG_INFO(
    "Extracted "  << StringUtils::formatLargeNumber(_wordKeysToCountsValues.size()) <<
    " word/tag associations.");
  _wordKeysToCountsValues.clear();
  if (tieCountsNeededResolved)
  {
    _sortByWord(_tieResolvedCountFile);
  }
  else
  {
    _sortByWord(_dedupedCountFile);
  }
}

void PoiImplicitTagRawRulesGenerator::_sortByTagOccurrence()
{
  LOG_INFO("Sorting output by tag occurrence count...");
  LOG_VART(_sortParallelCount);

  _sortedCountFile.reset(
    new QTemporaryFile(_tempFileDir + "/poi-implicit-tag-raw-rules-generator-temp-XXXXXX"));
  _sortedCountFile->setAutoRemove(!_keepTempFiles);
  if (!_sortedCountFile->open())
  {
    throw HootException(
      QObject::tr("Error opening %1 for writing.").arg(_sortedCountFile->fileName()));
  }
  LOG_DEBUG("Opened sorted temp file: " << _sortedCountFile->fileName());
  if (_keepTempFiles)
  {
    LOG_WARN("Keeping temp file: " << _sortedCountFile->fileName());
  }

  //This counts each unique line occurrence, sorts by decreasing occurrence count (necessary for
  //next step which removes duplicate tag keys associated with the same word), and replaces the
  //space between the prepended count and the word with a tab.

  //sort by highest count, then by word, then by tag
  const QString cmd =
    "sort --parallel=" + QString::number(_sortParallelCount) + " " + _countFile->fileName() +
    " | uniq -c | sort -n -r --parallel=" + QString::number(_sortParallelCount) + " | " +
    "sed -e 's/^ *//;s/ /\t/' > " + _sortedCountFile->fileName();
  if (std::system(cmd.toStdString().c_str()) != 0)
  {
    throw HootException("Unable to sort file.");
  }
  LOG_INFO(
    "Wrote " <<
    StringUtils::formatLargeNumber(FileUtils::getNumberOfLinesInFile(_sortedCountFile->fileName())) <<
    " lines to sorted file.");
}

void PoiImplicitTagRawRulesGenerator::_removeDuplicatedKeyTypes()
{
  LOG_INFO("Removing duplicated tag key types from output...");

  //i.e. don't allow amenity=school AND amenity=shop to be associated with the same word...pick one
  //of them

  _dedupedCountFile.reset(
    new QTemporaryFile(
      _tempFileDir + "/poi-implicit-tag-raw-rules-generator-temp-XXXXXX"));
  _dedupedCountFile->setAutoRemove(!_keepTempFiles);
  if (!_dedupedCountFile->open())
  {
    throw HootException(
      QObject::tr("Error opening %1 for writing.").arg(_dedupedCountFile->fileName()));
  }
  LOG_DEBUG("Opened dedupe temp file: " << _dedupedCountFile->fileName());
  if (_keepTempFiles)
  {
    LOG_WARN("Keeping temp file: " << _dedupedCountFile->fileName());
  }

  long lineCount = 0;
  long writtenLineCount = 0;
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
    const QString countStr = lineParts[0].trimmed();
    const long count = countStr.toLong();
    LOG_VART(count);
    const QStringList kvpParts = kvp.split("=");
    const QString tagKey = kvpParts[0];
    LOG_VART(tagKey);
    const QString wordTagKey = word.trimmed() % ";" % tagKey.trimmed();
    LOG_VART(wordTagKey);
    const QString wordTagKeyCount = word.trimmed() % ";" % tagKey.trimmed() % ";" % countStr.trimmed();
    LOG_VART(wordTagKeyCount);
    const QString tagValue = kvpParts[1];
    LOG_VART(tagValue);

    //The lines are sorted by occurrence count.  So the first time we see one word-key combo, we
    //know it had the highest occurrence count, and we can ignore all subsequent instances since
    //any one feature can't have more than one tag applied to it with the same key.

    const QString queriedCountAndValue = _wordKeysToCountsValues.value(wordTagKey, "");
    if (queriedCountAndValue.isEmpty())
    {
      _wordKeysToCountsValues[wordTagKey] = countStr % ";" % tagValue;
      //this unescaping must occur during the final temp file write
      if (word.contains("%3D"))
      {
        word = word.replace("%3D", "=");
      }
      else if (word.contains("%3d"))
      {
        word = word.replace("%3d", "=");
      }
      const QString updatedLine = countStr % "\t" % word % "\t" % kvp % "\n";
      LOG_VART(updatedLine);
      _dedupedCountFile->write(updatedLine.toUtf8());
      writtenLineCount++;
    }
    else
    {
      const long queriedCount = queriedCountAndValue.split(";")[0].toLong();
      if (queriedCount == count)
      {
        LOG_TRACE(
          "Recording duplicated word/tag key/count for: " << wordTagKeyCount << " with value: " <<
          tagValue);
        _duplicatedWordTagKeyCountsToValues[wordTagKeyCount].append(tagValue);
      }
    }

    lineCount++;
    if (lineCount % (_statusUpdateInterval * 10) == 0)
    {
      PROGRESS_INFO(
        "Parsed " << StringUtils::formatLargeNumber(lineCount) <<
        " lines from input for duplicated tag key removal.");
    }
  }
  _sortedCountFile->close();
  LOG_INFO(
    "Wrote " << StringUtils::formatLargeNumber(writtenLineCount) << " lines to deduped file.");
  _dedupedCountFile->close();
}

void PoiImplicitTagRawRulesGenerator::_resolveCountTies()
{
  LOG_INFO(
    "Resolving word/tag key/count ties for " <<
    StringUtils::formatLargeNumber(_duplicatedWordTagKeyCountsToValues.size()) <<
    " duplicated word/tag key/counts...");

  _tieResolvedCountFile.reset(
    new QTemporaryFile(
      _tempFileDir + "/poi-implicit-tag-raw-rules-generator-temp-XXXXXX"));
  _tieResolvedCountFile->setAutoRemove(!_keepTempFiles);
  if (!_tieResolvedCountFile->open())
  {
    throw HootException(
      QObject::tr("Error opening %1 for writing.").arg(_tieResolvedCountFile->fileName()));
  }
  LOG_DEBUG("Opened tie resolve temp file: " << _tieResolvedCountFile->fileName());
  if (_keepTempFiles)
  {
    LOG_WARN("Keeping temp file: " << _tieResolvedCountFile->fileName());
  }
  if (!_dedupedCountFile->open())
  {
    throw HootException(
      QObject::tr("Error opening %1 for reading.").arg(_dedupedCountFile->fileName()));
  }

  long lineCount = 0;
  long duplicateResolutions = 0;
  while (!_dedupedCountFile->atEnd())
  {
    const QString line = QString::fromUtf8(_dedupedCountFile->readLine().constData()).trimmed();
    LOG_VART(line);
    const QStringList lineParts = line.split("\t");
    LOG_VART(lineParts);
    QString word = lineParts[1].trimmed();
    LOG_VART(word);
    const QString kvp = lineParts[2].trimmed();
    LOG_VART(kvp);
    const QString countStr = lineParts[0].trimmed();
    const long count = countStr.toLong();
    LOG_VART(count);
    const QStringList kvpParts = kvp.split("=");
    const QString tagKey = kvpParts[0];
    LOG_VART(tagKey);
    const QString wordTagKey = word.trimmed() % ";" % tagKey.trimmed();
    LOG_VART(wordTagKey);
    const QString wordTagKeyCount = word.trimmed() % ";" % tagKey.trimmed() % ";" % countStr.trimmed();
    LOG_VART(wordTagKeyCount);
    const QString tagValue = kvpParts[1];
    LOG_VART(tagValue);

    if (_duplicatedWordTagKeyCountsToValues.contains(wordTagKeyCount))
    {
      LOG_TRACE("Resolving duplicated word/tag key/count for " << wordTagKeyCount << "...");

      QString lineWithMostSpecificKvp = line % "\n";
      const QStringList tagValues = _duplicatedWordTagKeyCountsToValues[wordTagKeyCount];
      for (int i = 0; i < tagValues.size(); i++)
      {
        const QString childKvp = tagKey % "=" % tagValues[i];
        if (OsmSchema::getInstance().isAncestor(childKvp, tagKey % "=" % tagValue))
        {
          lineWithMostSpecificKvp = countStr % "\t" % word % "\t" % childKvp % "\n";
        }
      }
      LOG_VART(lineWithMostSpecificKvp);
      _tieResolvedCountFile->write(lineWithMostSpecificKvp.toUtf8());
      duplicateResolutions++;
    }
    else
    {
      const QString updatedLine = countStr % "\t" % word % "\t" % kvp % "\n";
      LOG_VART(updatedLine);
      _tieResolvedCountFile->write(updatedLine.toUtf8());
    }

    lineCount++;
    if (lineCount % (_statusUpdateInterval * 10) == 0)
    {
      PROGRESS_INFO(
        "Parsed " << StringUtils::formatLargeNumber(lineCount) <<
        " lines from input for duplicated tag key count ties.");
    }
  }
  LOG_INFO(
    "Resolved " << StringUtils::formatLargeNumber(duplicateResolutions) <<
    " word/tag key/count ties.");
  _duplicatedWordTagKeyCountsToValues.clear();
  _tieResolvedCountFile->close();
}

void PoiImplicitTagRawRulesGenerator::_sortByWord(boost::shared_ptr<QTemporaryFile> input)
{
  LOG_INFO("Sorting output by word...");

  //sort by word, then by tag
  const QString cmd =
    "sort -t'\t' -k2,2 -k3,3 --parallel=" + QString::number(_sortParallelCount) + " " +
     input->fileName() + " -o " + _output->fileName();
  if (std::system(cmd.toStdString().c_str()) != 0)
  {
    throw HootException("Unable to sort input file.");
  }

  LOG_INFO(
    "Wrote " <<
    StringUtils::formatLargeNumber(
      FileUtils::getNumberOfLinesInFile(_output->fileName())) << " lines to final sorted file.");
}

}
