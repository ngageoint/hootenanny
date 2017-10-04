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

QString PoiImplicitTagRulesDeriver::_getMostSpecificPoiKvp(const Tags& tags) const
{
  QString mostSpecificPoiKvp;
  for (Tags::const_iterator tagItr = tags.begin(); tagItr != tags.end(); ++tagItr)
  {
    const QString kvp = tagItr.key() % "=" % tagItr.value();
    if (OsmSchema::getInstance().getCategories(kvp).intersects(OsmSchemaCategory::poi()))
    {
      if (mostSpecificPoiKvp != QLatin1String("poi=yes") &&
          (mostSpecificPoiKvp.isEmpty() ||
          !OsmSchema::getInstance().isAncestor(kvp, mostSpecificPoiKvp)))
      {
        mostSpecificPoiKvp = kvp;
      }
    }
  }
  return mostSpecificPoiKvp;
}

void PoiImplicitTagRulesDeriver::_updateForNewWord(const QString word, const QString kvp)
{
  const QString wordKvp = word % ";" % kvp;
  if (!_wordKvpsToOccuranceCounts.contains(wordKvp))
  {
    _wordKvpsToOccuranceCounts[wordKvp] = 1;
  }
  else
  {
    _wordKvpsToOccuranceCounts[wordKvp]++;
  }

  const QStringList kvpParts = kvp.split("=");
  const QString kvpKey = kvpParts[0];
  const QString kvpVal = kvpParts[1];
  const QString wordKvpKey = word % ";" % kvpKey;
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
}

QMap<QString, QMap<QString, long> > PoiImplicitTagRulesDeriver::deriveRules(const QStringList inputs,
                                                                         const QStringList typeKeys,
                                                                   const int minOccurancesThreshold)
{
  _minOccurancesThreshold = minOccurancesThreshold;

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

      if (element->getElementType() == ElementType::Node &&
          (typeKeys.isEmpty() || tags.hasAnyKey(typeKeys)))
      {
        const QStringList names = tags.getNames();
        if (names.size() > 0)
        {
          const QString mostSpecificPoiKvp = _getMostSpecificPoiKvp(tags);
          if (!mostSpecificPoiKvp.isEmpty())
          {
            for (int i = 0; i < names.size(); i++)
            {
              const QString name = names.at(i);

              _updateForNewWord(name, mostSpecificPoiKvp);

              const QStringList nameTokens = tokenizer.tokenize(name);
              for (int j = 0; j < nameTokens.size(); j++)
              {
                _updateForNewWord(nameTokens.at(i), mostSpecificPoiKvp);
              }
            }
          }
        }
      }
    }
    inputReader->finalizePartial();
  }

  _removeKvpsBelowOccuranceThreshold();
  _removeDuplicatedKvpTypes();

  return _combineWordMaps();
}

void PoiImplicitTagRulesDeriver::_removeKvpsBelowOccuranceThreshold()
{
  QMap<QString, long> updatedCounts;
  QMap<QString, QStringList> updatedValues;

  for (QMap<QString, long>::const_iterator kvpCountsItr = _wordKvpsToOccuranceCounts.begin();
       kvpCountsItr != _wordKvpsToOccuranceCounts.end(); ++kvpCountsItr)
  {
    const long count = kvpCountsItr.value();
    if (count >= _minOccurancesThreshold)
    {
      const QStringList keyParts = kvpCountsItr.key().split(";");
      const QString word = keyParts[0];
      const QString kvp = keyParts[1];
      const QStringList kvpParts = kvp.split("=");
      const QString kvpKey = kvpParts[0];
      const QString kvpVal = kvpParts[1];
      const QString wordKvpKey = word % ";" % kvpKey;

      updatedCounts[kvpCountsItr.key()] = count;
      if (!updatedValues.contains(wordKvpKey))
      {
        updatedValues[wordKvpKey] = QStringList();
      }
      updatedValues[wordKvpKey].append(kvpVal);
    }
  }

  _wordKvpsToOccuranceCounts = updatedCounts;
  _wordTagKeysToTagValues= updatedValues;
}

void PoiImplicitTagRulesDeriver::_removeDuplicatedKvpTypes()
{
  QMap<QString, long> updatedCounts;
  QMap<QString, QStringList> updatedValues;

  for (QMap<QString, QStringList>::const_iterator valsItr = _wordTagKeysToTagValues.begin();
       valsItr != _wordTagKeysToTagValues.end(); ++valsItr)
  {
    const QString wordKvpKey = valsItr.key();
    const QStringList vals = valsItr.value();

    assert(vals.size() != 0);
    if (vals.size() > 1)
    {
      QString highestOccurranceKvp;
      long highestOccurranceCount = 0;

      for (int i = 0; i < vals.size(); i++)
      {
        const QString wordKvp = wordKvpKey % "=" % vals.at(i);
        const long occurranceCount = _wordKvpsToOccuranceCounts[wordKvp];
        if (occurranceCount > highestOccurranceCount)
        {
          highestOccurranceCount = occurranceCount;
          highestOccurranceKvp = wordKvp;
        }
      }

      if (highestOccurranceCount > 0)
      {
        updatedCounts[highestOccurranceKvp] = highestOccurranceCount;
        const QString highestOccurranceVal = highestOccurranceKvp.split("=")[1];
        if (!updatedValues.contains(wordKvpKey))
        {
          updatedValues[wordKvpKey] = QStringList();
        }
        updatedValues[wordKvpKey].append(highestOccurranceVal);
      }
    }
    else //size == 1
    {
      const QString wordKvp = wordKvpKey % "=" % vals.at(0);
      updatedCounts[wordKvp] = _wordKvpsToOccuranceCounts[wordKvp];
      updatedValues[wordKvpKey] = _wordTagKeysToTagValues[wordKvpKey];
    }
  }

  _wordKvpsToOccuranceCounts = updatedCounts;
  _wordTagKeysToTagValues= updatedValues;
}

QMap<QString, QMap<QString, long> > PoiImplicitTagRulesDeriver::_combineWordMaps()
{
  QMap<QString, QMap<QString, long> > wordsToKvpsWithCounts;

  for (QMap<QString, long>::const_iterator kvpsWithCountsItr = _wordKvpsToOccuranceCounts.begin();
       kvpsWithCountsItr != _wordKvpsToOccuranceCounts.end(); ++kvpsWithCountsItr)
  {
    const QString wordKvp = kvpsWithCountsItr.key();
    const QStringList wordKvpParts = wordKvp.split(";");
    const QString word = wordKvpParts[0];
    const QString kvp = wordKvpParts[1];
    const long kvpCount = kvpsWithCountsItr.value();

    if (!wordsToKvpsWithCounts.contains(word))
    {
      wordsToKvpsWithCounts[kvp] = QMap<QString, long>();
    }
    QMap<QString, long> kvpsWithCounts = wordsToKvpsWithCounts[word];
    kvpsWithCounts[kvp] = kvpCount;
    wordsToKvpsWithCounts[word] = kvpsWithCounts;
  }

  return wordsToKvpsWithCounts;
}

}
