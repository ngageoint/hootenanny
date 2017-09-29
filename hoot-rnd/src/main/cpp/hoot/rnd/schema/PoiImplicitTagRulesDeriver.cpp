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

void PoiImplicitTagRulesDeriver::_updateForNewToken(const QString token, const QString kvp)
{
  if (!_tokensToKvpsWithCounts.contains(token))
  {
    QMap<QString, long> kvpWithCount;
    kvpWithCount[kvp] = 1;
    _tokensToKvpsWithCounts[token] = kvpWithCount;
  }
  else if (!_tokensToKvpsWithCounts[token].contains(kvp))
  {
    QMap<QString, long> kvpWithCount = _tokensToKvpsWithCounts[token];
    kvpWithCount[kvp]++;
    _tokensToKvpsWithCounts[token] = kvpWithCount;
  }
  else
  {
    _tokensToKvpsWithCounts[token][kvp]++;
  }
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

//bool maxOccuranceFirst(const QMap<QString, long>& a, const QMap<QString, long>& b)
//{
//  //TODO: make a function for this
//  long highestKvpOccuranceA = 0;
//  for (QMap<QString, long>::const_iterator kvpWithCountItr = a.begin();
//         kvpWithCountItr != a.end(); ++kvpWithCountItr)
//  {
//    const long kvpCountA = kvpWithCountItr.value();
//    if (kvpCountA > highestKvpOccuranceA)
//    {
//      highestKvpOccuranceA = kvpCountA;
//    }
//  }

//  long highestKvpOccuranceB = 0;
//  for (QMap<QString, long>::const_iterator kvpWithCountItr = b.begin();
//         kvpWithCountItr != b.end(); ++kvpWithCountItr)
//  {
//    const long kvpCountB = kvpWithCountItr.value();
//    if (kvpCountB > highestKvpOccuranceB)
//    {
//      highestKvpOccuranceB = kvpCountB;
//    }
//  }

//  return highestKvpOccuranceA > highestKvpOccuranceB;
//}

QMap<QString, QMap<QString, long> > PoiImplicitTagRulesDeriver::deriveRules(const QStringList inputs,
                                                                        const int minOccurances,
                                                                        const QStringList typeKeys)
{
  _minOccurances = minOccurances;

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

              _updateForNewToken(name, mostSpecificPoiKvp);

              const QStringList nameTokens = tokenizer.tokenize(name);
              for (int j = 0; j < nameTokens.size(); j++)
              {
                _updateForNewToken(nameTokens.at(i), mostSpecificPoiKvp);
              }
            }
          }
        }
      }
    }
    inputReader->finalizePartial();
  }

  _removeKvpsBelowOccuranceThreshold();

  //TODO: sort by descending kvp occurance per word?
  //qSort(_tokensToKvpsWithCounts.begin(), _tokensToKvpsWithCounts.end(), maxOccuranceFirst);

  return _tokensToKvpsWithCounts;
}

void PoiImplicitTagRulesDeriver::_removeKvpsBelowOccuranceThreshold()
{
  QMap<QString, QMap<QString, long> > updatedData;
  for (QMap<QString, QMap<QString, long> >::const_iterator tokenItr = _tokensToKvpsWithCounts.begin();
       tokenItr != _tokensToKvpsWithCounts.end(); ++tokenItr)
  {
    const QString token = tokenItr.key();
    const QMap<QString, long> kvpsWithCounts = tokenItr.value();
    for (QMap<QString, long>::const_iterator kvpWithCountItr = kvpsWithCounts.begin();
         kvpWithCountItr != kvpsWithCounts.end(); ++kvpWithCountItr)
    {
      const QString kvp = kvpWithCountItr.key();
      const long kvpCountForToken =  kvpWithCountItr.value();
      if (kvpCountForToken >= _minOccurances)
      {
        if (!_tokensToKvpsWithCounts.contains(token))
        {
          QMap<QString, long> updatedKvpsWithCounts;
          updatedKvpsWithCounts[kvp] = kvpCountForToken;
          updatedData[token] = updatedKvpsWithCounts;
        }
        else
        {
          QMap<QString, long> updatedKvpsWithCounts = _tokensToKvpsWithCounts[token];
          updatedKvpsWithCounts[kvp] = kvpCountForToken;
          updatedData[token] = updatedKvpsWithCounts;
        }
      }
    }
  }
  _tokensToKvpsWithCounts = updatedData;
}

}
