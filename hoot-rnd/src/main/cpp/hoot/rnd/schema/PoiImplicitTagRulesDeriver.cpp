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

// Qt
#include <QStringBuilder>

namespace hoot
{

PoiImplicitTagRulesDeriver::PoiImplicitTagRulesDeriver()
{
}

//QString PoiImplicitTagRulesDeriver::_getMostSpecificPoiKvp(const Tags& tags) const
//{
//  LOG_TRACE("Retrieving most specific POI kvp...");

//  QString mostSpecificPoiKvp;
//  for (Tags::const_iterator tagItr = tags.begin(); tagItr != tags.end(); ++tagItr)
//  {
//    const QString kvp = tagItr.key() % "=" % tagItr.value();
//    LOG_VART(kvp);
//    LOG_VART(OsmSchema::getInstance().getCategories(kvp).intersects(OsmSchemaCategory::poi()));
//    if (OsmSchema::getInstance().getCategories(kvp).intersects(OsmSchemaCategory::poi()))
//    {
//      LOG_VART(OsmSchema::getInstance().isAncestor(kvp, mostSpecificPoiKvp));
//      if (kvp != QLatin1String("poi=yes") &&
//          (mostSpecificPoiKvp.isEmpty() ||
//           !OsmSchema::getInstance().isAncestor(kvp, mostSpecificPoiKvp)))
//      {
//        mostSpecificPoiKvp = kvp;
//        LOG_VART(mostSpecificPoiKvp);
//      }
//    }
//  }
//  return mostSpecificPoiKvp;
//}

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

  //';' is used as the delimiter in the map keys, so it needs to be escaped in the name, but we're
  //not going to allow it in the kvp, as it doesn't make sense for it to be there
//  if (word.contains(";"))
//  {
//    word = word.replace(";", "%3B");
//    LOG_TRACE("Escaped word to: " << word);
//  }
//  if (kvp.contains(";"))
//  {
//    kvp = kvp.replace(";", "");
//    LOG_TRACE("Modified kvp to: " << kvp);
//  }

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

QMap<QString, QMap<QString, long> > PoiImplicitTagRulesDeriver::deriveRules(const QStringList inputs,
                                                                         const QStringList typeKeys,
                                                                   const int minOccurancesThreshold)
{
  LOG_INFO(
    "Deriving POI implicit tag rules for inputs: " << inputs << " type keys: " << typeKeys <<
    " with minimum occurance threshold: " << minOccurancesThreshold << "...");

  QStringList typeKeysAllowed;
  for (int i = 0; i < typeKeys.size(); i++)
  {
    typeKeysAllowed.append(typeKeys.at(i).toLower());
  }

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
          //const QString mostSpecificPoiKvp = _getMostSpecificPoiKvp(tags);
          //LOG_VART(mostSpecificPoiKvp);
          const QStringList kvps = _getPoiKvps(tags);
          LOG_VART(kvps.size());
          //if (!mostSpecificPoiKvp.isEmpty())
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
          }
        }
      }
    }
    inputReader->finalizePartial();
  }
  _wordCaseMappings.clear();

  _removeKvpsBelowOccuranceThreshold(minOccurancesThreshold);
  _removeDuplicatedKeyTypes();
  _removeIrrelevantKeyTypes(typeKeysAllowed);

  return _generateOutput();
}

void PoiImplicitTagRulesDeriver::_removeKvpsBelowOccuranceThreshold(const int minOccurancesThreshold)
{
  LOG_DEBUG(
    "Removing duplicated kvp below mininum occurance threshold: " << minOccurancesThreshold <<
    "...");

  QMap<QString, long> updatedCounts;
  QMap<QString, QStringList> updatedValues;

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
  }

  _wordKvpsToOccuranceCounts = updatedCounts;
  _wordTagKeysToTagValues= updatedValues;
}

void PoiImplicitTagRulesDeriver::_removeIrrelevantKeyTypes(const QStringList typeKeysAllowed)
{
  LOG_DEBUG("Removing irrelevant kvps...");

  if (typeKeysAllowed.size() == 0)
  {
    return;
  }

  QMap<QString, long> updatedCounts;
  QMap<QString, QStringList> updatedValues;

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
  }

  _wordKvpsToOccuranceCounts = updatedCounts;
  _wordTagKeysToTagValues= updatedValues;
}

void PoiImplicitTagRulesDeriver::_removeDuplicatedKeyTypes()
{
  LOG_DEBUG("Removing duplicated kvp types...");

  QMap<QString, long> updatedCounts;
  QMap<QString, QStringList> updatedValues;

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
    }
    else //size == 1
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
}

QMap<QString, QMap<QString, long> > PoiImplicitTagRulesDeriver::_generateOutput()
{
  LOG_DEBUG("Generating output...");

  //key=<word>, value=map: key=<kvp>, value=<kvp occurance count>
  QMap<QString, QMap<QString, long> > wordsToKvpsWithCounts;

  for (QMap<QString, long>::const_iterator kvpsWithCountsItr = _wordKvpsToOccuranceCounts.begin();
       kvpsWithCountsItr != _wordKvpsToOccuranceCounts.end(); ++kvpsWithCountsItr)
  {
    const QString wordKvp = kvpsWithCountsItr.key();
    const QStringList wordKvpParts = wordKvp.split(";");
    QString word = wordKvpParts[0];
//    if (word.contains("%3B", Qt::CaseInsensitive))
//    {
//      LOG_VAR(word);
//      word = word.replace("%3B", ";", Qt::CaseInsensitive);
//    }
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

}
