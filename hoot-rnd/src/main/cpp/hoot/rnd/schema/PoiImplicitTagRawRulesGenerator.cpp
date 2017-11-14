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
_skipTranslation(false)
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
  LOG_VAR(_tokenizeNames);
  LOG_VAR(_sortParallelCount);
  LOG_VAR(_skipFiltering);
  LOG_VAR(_skipTranslation);

  _wordKeysToCounts.clear();
  _wordCaseMappings.clear();
  _countFileLineCtr = 0;

  _countFile.reset(
    new QTemporaryFile(
      ConfigOptions().getApidbBulkInserterTempFileDir() +
      "/poi-implicit-tag-rules-deriver-temp-XXXXXX"));
  _countFile->setAutoRemove(!ConfigOptions().getPoiImplicitTagRulesKeepTempFiles());
  if (!_countFile->open())
  {
    throw HootException(QObject::tr("Error opening %1 for writing.").arg(_countFile->fileName()));
  }
  LOG_DEBUG("Opened temp file: " << _countFile->fileName());
  if (ConfigOptions().getPoiImplicitTagRulesKeepTempFiles())
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
    if (!_skipTranslation)
    {
      boost::shared_ptr<TranslationVisitor> translationVisitor(new TranslationVisitor());
      translationVisitor->setPath(translationScripts.at(i));
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

      //TODO: skip filtering may go away
      if (_skipFiltering || _poiFilter.isSatisfied(element))
      {
        const QStringList names = element->getTags().getNames();
        assert(!names.isEmpty());
        if (names.isEmpty())
        {
          throw HootException("Names empty.");
        }
        const QStringList kvps = ImplicitTagEligiblePoiCriterion::getPoiKvps(element->getTags());
        assert(!kvps.isEmpty());
        if (kvps.isEmpty())
        {
          throw HootException("POI kvps empty.");
        }

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

          if (_tokenizeNames)
          {
            const QStringList nameTokens = tokenizer.tokenize(name);
            LOG_VART(nameTokens.size());
            for (int j = 0; j < nameTokens.size(); j++)
            {
              QString nameToken = nameTokens.at(j);
              //may need to replace more punctuation chars here
              nameToken = nameToken.replace(",", "");

              for (int k = 0; k < kvps.size(); k++)
              {
                _updateForNewWord(nameToken, kvps.at(k));
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

      nodeCount++;
    }
    inputReader->finalizePartial();
  }
  _countFile->close();

  LOG_INFO(
    "Parsed " << StringUtils::formatLargeNumber(_wordCaseMappings.size()) << " words from " <<
    StringUtils::formatLargeNumber(poiCount) << " POIs and " <<
    StringUtils::formatLargeNumber(nodeCount) << " nodes.");
  LOG_INFO(
    "Wrote " << StringUtils::formatLargeNumber(_countFileLineCtr) << " lines to count file.");
  _wordCaseMappings.clear();

  _sortByTagOccurrence();
  _removeDuplicatedKeyTypes();
  LOG_INFO(
    "Extracted "  << StringUtils::formatLargeNumber(_wordKeysToCounts.size()) <<
    " word/tag associations.");
  _wordKeysToCounts.clear();
  _sortByWord();
}

void PoiImplicitTagRawRulesGenerator::_sortByTagOccurrence()
{
  LOG_INFO("Sorting output by tag occurrence count...");
  LOG_VART(_sortParallelCount);

  _sortedCountFile.reset(
    new QTemporaryFile(
      ConfigOptions().getApidbBulkInserterTempFileDir() +
      "/poi-implicit-tag-rules-deriver-temp-XXXXXX"));
  _sortedCountFile->setAutoRemove(!ConfigOptions().getPoiImplicitTagRulesKeepTempFiles());
  if (!_sortedCountFile->open())
  {
    throw HootException(
      QObject::tr("Error opening %1 for writing.").arg(_sortedCountFile->fileName()));
  }
  LOG_DEBUG("Opened sorted temp file: " << _sortedCountFile->fileName());
  if (ConfigOptions().getPoiImplicitTagRulesKeepTempFiles())
  {
    LOG_WARN("Keeping temp file: " << _sortedCountFile->fileName());
  }

  //TODO: update this comment
  //This counts each unique line occurrence, sorts by decreasing occurrence count (necessary for
  //next step which removes duplicate tag keys associated with the same word), removes lines with
  //occurrence counts below the specified threshold, and replaces the space between the prepended
  //count and the word with a tab. - not sure why 1 needs to be subtracted from
  //the min occurrences here, though...
  //--parallel=4
  const QString cmd =
    "sort --parallel=" + QString::number(_sortParallelCount) + " " + _countFile->fileName() +
    " | uniq -c | sort -n -r --parallel=" + QString::number(_sortParallelCount) + " | " +
    "sed -e 's/^ *//;s/ /\t/' > " + _sortedCountFile->fileName();
  if (std::system(cmd.toStdString().c_str()) != 0)
  {
    throw HootException("Unable to sort file.");
  }
}

void PoiImplicitTagRawRulesGenerator::_removeDuplicatedKeyTypes()
{
  LOG_INFO("Removing duplicated tag key types from output...");

  //i.e. don't allow amenity=school AND amenity=shop to be associated with the same word...pick one
  //of them

  _sortedDedupedCountFile.reset(
    new QTemporaryFile(
      ConfigOptions().getApidbBulkInserterTempFileDir() +
      "/poi-implicit-tag-rules-deriver-temp-XXXXXX"));
  _sortedDedupedCountFile->setAutoRemove(
    !ConfigOptions().getPoiImplicitTagRulesKeepTempFiles());
  if (!_sortedDedupedCountFile->open())
  {
    throw HootException(
      QObject::tr("Error opening %1 for writing.").arg(_sortedDedupedCountFile->fileName()));
  }
  LOG_DEBUG("Opened dedupe temp file: " << _sortedDedupedCountFile->fileName());
  if (ConfigOptions().getPoiImplicitTagRulesKeepTempFiles())
  {
    LOG_WARN("Keeping temp file: " << _sortedDedupedCountFile->fileName());
  }

  long lineCount = 0;
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
        "Found word with multiple tag occurrence counts of the same size.  Arbitrarily " <<
        "chose first tag.  Not creating implicit tag entry for word: " << word << ", tag: " <<
        kvp << ", count: " << count);
    }

    lineCount++;
    if (lineCount % _statusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "Parsed " << StringUtils::formatLargeNumber(lineCount) <<
        " elements from input for duplicated tag key removal.");
    }
  }
  _sortedCountFile->close();
  _sortedDedupedCountFile->close();
}

void PoiImplicitTagRawRulesGenerator::_sortByWord()
{
  LOG_INFO("Sorting output by word...");
  LOG_VART(_sortParallelCount);

  //sort by word, then by tag
  //-d -k2,3 -t$'\t'
  const QString cmd =
    "sort -t'\t' -k2,2 -k3,3 --parallel=" + QString::number(_sortParallelCount) + " " +
     _sortedDedupedCountFile->fileName() + " -o " + _output->fileName();
  if (std::system(cmd.toStdString().c_str()) != 0)
  {
    throw HootException("Unable to sort input file.");
  }
}

}
