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
#ifndef POIIMPLICITTAGRULESDERIVER_H
#define POIIMPLICITTAGRULESDERIVER_H

// Hoot
#include <hoot/rnd/schema/ImplicitTagRule.h>

// Qt
#include <QString>
#include <QMap>

namespace hoot
{

class Tags;

/**
 *
 */
class PoiImplicitTagRulesDeriver
{

public:

  PoiImplicitTagRulesDeriver();

  void deriveRules(const QStringList inputs, const QStringList outputs,
                   const QStringList typeKeys = QStringList(), const int minOccurancesThreshold = 1);

private:

  //for testing
  friend class PoiImplicitTagRulesDeriverTest;

  //TODO: replace with stxxl map
  //key=<word>;<kvp>, value=<kvp occurance count>
  QMap<QString, long> _wordKvpsToOccuranceCounts;
  //key=<word>;<tag key>, value=<tag values>
  QMap<QString, QStringList> _wordTagKeysToTagValues;
  //key=<lower case word>, value=<word>
  QMap<QString, QString> _wordCaseMappings;

  ImplicitTagRulesByWord _tagRulesByWord;
  ImplicitTagRules _tagRules;

  void _updateForNewWord(QString word, const QString kvp);
  QStringList _getPoiKvps(const Tags& tags) const;
  void _removeKvpsBelowOccuranceThreshold(const int minOccurancesThreshold);
  void _removeDuplicatedKeyTypes();
  void _removeIrrelevantKeyTypes(const QStringList typeKeysAllowed);
  ImplicitTagRulesByWord _generateTagRulesByWord();
  ImplicitTagRules _rulesByWordToRules(const ImplicitTagRulesByWord& rulesByWord);
};

}

#endif // POIIMPLICITTAGRULESDERIVER_H
