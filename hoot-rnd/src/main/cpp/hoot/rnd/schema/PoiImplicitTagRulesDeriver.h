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
#include <hoot/rnd/io/ImplicitTagRuleWordPartWriterFactory.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QString>
#include <QMap>
#include <QHash>
#include <QTemporaryFile>

namespace hoot
{

class Tags;

/**
 * Derives implicit tag rules for POIs and writes the rules to various output formats
 */
class PoiImplicitTagRulesDeriver : public Configurable
{

public:

  PoiImplicitTagRulesDeriver();

  /**
   * Derives implicit tag rules for POIs given input data and writes the rules to output
   *
   * @param inputs a list of hoot supported feature input formats to derive rules from
   * @param translationScripts list of OSM translation scripts corresponding to the datasets
   * specified by the inputs parameter
   * @param outputs a list of hoot supported implicit tag rule output formats
   */
  void deriveRules(const QString input, const QStringList outputs);

  virtual void setConfiguration(const Settings& conf);

  void setCustomRuleFile(const QString file) { _customRuleFile = file; }
  void setMinTagOccurrencesPerWord(const int minOccurrences)
  { _minTagOccurrencesPerWord = minOccurrences; }
  void setMinWordLength(const int length) { _minWordLength = length; }
  void setRuleIgnoreFile(const QString file) { _ruleIgnoreFile = file; }
  void setTagIgnoreRuleFile(const QString file) { _tagIgnoreFile = file; }
  void setTagFile(const QString file) { _tagFile = file; }
  void setWordIgnoreFile(const QString file) { _wordIgnoreFile = file; }

private:

  //for testing
  friend class PoiImplicitTagRulesDeriverTest;

  long _statusUpdateInterval;
  QString _customRuleFile;
  int _minTagOccurrencesPerWord;
  int _minWordLength;
  QString _ruleIgnoreFile;
  QString _tagIgnoreFile;
  QString _tagFile;
  QString _wordIgnoreFile;

  QStringList _tagIgnoreList;
  QStringList _wordIgnoreList;
  QMap<QString, QString> _customRulesList;
  QMap<QString, QString> _rulesIgnoreList;
  QStringList _tagsAllowList;

  boost::shared_ptr<QTemporaryFile> _thresholdedCountFile;
  boost::shared_ptr<QTemporaryFile> _filteredCountFile;
  boost::shared_ptr<QTemporaryFile> _finalSortedByWordCountFile;

  void _removeKvpsBelowOccurrenceThreshold(const QString input, const int minOccurrencesThreshold);
  void _applyFiltering();
  void _sortByWord();

  QString _getSqliteOutput(const QStringList outputs);
  void _writeRules(const QStringList outputs, const QString sqliteOutputFile);

  void _readIgnoreLists();
  void _readAllowLists();
};

}

#endif // POIIMPLICITTAGRULESDERIVER_H
