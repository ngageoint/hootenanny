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
#include <hoot/rnd/io/ImplicitTagRulesJsonWriter.h>
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

//void PoiImplicitTagRulesDeriver::_updateForNewToken(const QString token, const QString kvp)
//{
//  //  QMap<QString, QStringList> _tokenToKvps;
////  QMap<QString, QStringList> _kvpToTokens;
////QMap<QString, int> _tokenKvpToCount

//  if (!_tokenToKvps.contains(token))
//  {
//    QStringList kvps;
//    kvps.append(kvp);
//    _tokenToKvps[token] = kvps;

//  }
//  else
//  {
//    _tokenToKvps[token].append(kvp);
//  }

//  if (!_kvpToTokens.contains(kvp))
//  {
//    QStringList tokens;
//    tokens.append(token);
//    _kvpToTokens[kvp] = tokens;

//  }
//  else
//  {
//    _kvpToTokens[kvp].append(token);
//  }

//  if (!_tokenKvpToCount.contains(token % ";" % kvp))
//  {
//    _tokenKvpToCount[token % ";" % kvp] = 1;
//  }
//  else
//  {
//     _tokenKvpToCount[token % ";" % kvp]++;
//  }
//}

void PoiImplicitTagRulesDeriver::_updateForNewToken2(const QString token, const QString kvp)
{
  /*
   * QList<ImplicitTagRulePtr> _rules;
     QMap<QString, ImplicitTagRulePtr> _tokensToRules;
     QMap<QString, ImplicitTagRulePtr> _kvpsToRules; //??
     QMap<QString, long> _wordKvpToCount;
     QMap<QString, QMap<QString, long> > _tokensToKvpsWithCounts;
   */

//  ImplicitTagRulePtr rule;
//  if (!_tokensToRules.contains(token))
//  {
//    //if (_minOccurancesAllowed == 1)
//    //{
//      QStringList words;
//      words.append(token);
//      Tags tags;
//      tags.appendValue(kvp);
//      rule.reset(new ImplicitTagRule(words, tags));
//      _tokensToRules[token] = rule;
//      _kvpsToRules[kvp] = rule;
//      _rules.append(rule);
//    //}
//    _wordKvpToCount[token % ";" % kvp] = 1;
//    QMap<QString, long> kvps;
//    kvps[kvp] = 1;
//    _tokensToKvpsWithCounts[token] = kvps;
//  }
//  else
//  {
//    rule = _tokensToRules[token];
//    rule->getWords().append(token);
//    rule->getTags().appendValue(kvp);
//    _wordKvpToCount[token % ";" % kvp]++;
//    QMap<QString, long> kvps = _tokensToKvpsWithCounts[token];
//    kvps[kvp]++;
//    _tokensToKvpsWithCounts[token] = kvps;
//  }

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
      if (mostSpecificPoiKvp.isEmpty() ||
          !OsmSchema::getInstance().isAncestor(kvp, mostSpecificPoiKvp))
      {
        mostSpecificPoiKvp = kvp;
      }
    }
  }
  return mostSpecificPoiKvp;
}

void PoiImplicitTagRulesDeriver::writeRules(const QString input, const QString output,
                                            const int minOccurancesAllowed,
                                            const QStringList typeKeys)
{
  _minOccurancesAllowed = minOccurancesAllowed;

  boost::shared_ptr<PartialOsmMapReader> inputReader =
    boost::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::getInstance().createReader(input));
  inputReader->open(input);
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
      //#3

      const QStringList names = tags.getNames();
      if (names.size() > 0)
      {
        const QString mostSpecificPoiKvp = _getMostSpecificPoiKvp(tags);
        if (!mostSpecificPoiKvp.isEmpty())
        {
          for (int i = 0; i < names.size(); i++)
          {
            const QString name = names.at(i);

            _updateForNewToken2(name, mostSpecificPoiKvp);

            const QStringList nameTokens = tokenizer.tokenize(name);
            for (int j = 0; j < nameTokens.size(); j++)
            {
              _updateForNewToken2(nameTokens.at(i), mostSpecificPoiKvp);
            }
          }
        }
      }

      //#2

//      const QStringList names = tags.getNames();
//      if (names.size() > 0)
//      {
//        const QString mostSpecificPoiKvp = _getMostSpecificPoiKvp(tags);
//
//        if (!mostSpecificPoiKvp.isEmpty())
//        {
//          for (int i = 0; i < names.size(); i++)
//          {
//            const QString name = names.at(i);

//            _updateForNewToken(name, mostSpecificPoiKvp);

//            const QStringList nameTokens = tokenizer.tokenize(name);
//            for (int j = 0; j < nameTokens.size(); j++)
//            {
//              _updateForNewToken(nameTokens.at(i), mostSpecificPoiKvp);
//            }
//          }
//        }

//        //#1

//        if (!mostSpecificPoiKvp.isEmpty())
//        {
//          for (int i = 0; i < names.size(); i++)
//          {
//            const QString name = names.at(i);

//            if (!_kvpCountsByToken.contains(name))
//            {
//              QStringList kvpCounts;
//              kvpCounts.append(mostSpecificPoiKvp % "=1");
//              _kvpCountsByToken.insert(name, kvpCounts);
//            }
//            else
//            {
//              const QStringList kvpCountsForToken = _kvpCountsByToken.values(name);
//              QStringList kvpCountsForTokenOut;
//              for (int j = 0; j < kvpCountsForToken.size(); j++)
//              {
//                QString kvpCountForTokenStr = kvpCountsForToken.at(j);
//                if (kvpCountForTokenStr.startsWith(mostSpecificPoiKvp))
//                {
//                  const QStringList kvpCountParts = kvpCountForTokenStr.split("=");
//                  const QString kvp = kvpCountParts[0];
//                  int kvpCountForToken = kvpCountParts[1].toInt();
//                  kvpCountForToken++;
//                  kvpCountsForTokenOut.append(kvp % "=" % QString::number(kvpCountForToken));
//                }
//                else
//                {
//                  kvpCountsForTokenOut.append(kvpCountForTokenStr);
//                }
//              }
//              _kvpCountsByToken.replace(name, kvpCountsForTokenOut);
//            }

//            const QStringList nameTokens = tokenizer.tokenize(name);
//            for (int j = 0; j < nameTokens.size(); j++)
//            {
//              const QString nameToken = nameTokens.at(j);
//              const QStringList kvpCountsForToken = _kvpCountsByToken.values(nameToken);
//              QStringList kvpCountsForTokenOut;
//              for (int j = 0; j < kvpCountsForToken.size(); j++)
//              {
//                QString kvpCountForTokenStr = kvpCountsForToken.at(j);
//                if (kvpCountForTokenStr.startsWith(mostSpecificPoiKvp))
//                {
//                  const QStringList kvpCountParts = kvpCountForTokenStr.split("=");
//                  const QString kvp = kvpCountParts[0].trimmed();
//                  int kvpCountForToken = kvpCountParts[1].trimmed().toInt();
//                  kvpCountForToken++;
//                  kvpCountsForTokenOut.append(kvp % "=" % QString::number(kvpCountForToken));
//                }
//                else
//                {
//                  kvpCountsForTokenOut.append(kvpCountForTokenStr);
//                }
//              }
//              _kvpCountsByToken.replace(nameToken, kvpCountsForTokenOut);
//            }
//          }
//        }
//      }
    }
  }
  inputReader->finalizePartial();

  assert(output.endsWith(".json"));  //TODO: temp
  ImplicitTagRulesJsonWriter rulesWriter;
  rulesWriter.open(output);
  //rulesWriter.write(_countsToRules());
  rulesWriter.write(_tokensToKvpsWithCounts);
  rulesWriter.close();
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
      if (kvpCountForToken >= _minOccurancesAllowed)
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

//QList<ImplicitTagRulePtr> PoiImplicitTagRulesDeriver::_tokenKvpsToRules()
//{
//  for (QMap<QString, QMap<QString, long> >::const_iterator tokenItr = _tokensToKvpsWithCounts.begin();
//       tokenItr != _tokensToKvpsWithCounts.end(); ++tokenItr)
//  {
//    const QString token = tokenItr.key();
//    const QMap<QString, long> kvpsWithCounts = tokenItr.value();
//    for (QMap<QString, long>::const_iterator kvpWithCountItr = kvpsWithCounts.begin();
//         kvpWithCountItr != kvpsWithCounts.end(); ++kvpWithCountItr)
//    {
//      const QString kvp = kvpWithCountItr.key();

//    }
//  }
//}

}
