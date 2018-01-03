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
#include <hoot/rnd/schema/PoiImplicitTagCustomRules.h>

// Qt
#include <QString>
#include <QMap>
#include <QHash>
#include <QTemporaryFile>
#include <QSet>

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

  void setMinTagOccurrencesPerWord(const int minOccurrences)
  { _minTagOccurrencesPerWord = minOccurrences; }
  void setMinWordLength(const int length) { _minWordLength = length; }

  void setCustomRuleFile(const QString file) { _customRules.setCustomRuleFile(file); }
  void setTagIgnoreFile(const QString file) { _customRules.setTagIgnoreFile(file); }
  void setWordIgnoreFile(const QString file) { _customRules.setWordIgnoreFile(file); }
  void setUseSchemaTagValuesForWordsOnly(bool use) { _useSchemaTagValuesForWordsOnly = use; }
  void setTranslateAllNamesToEnglish(bool translate) { _translateAllNamesToEnglish = translate; }

private:

  //for testing
  friend class PoiImplicitTagRulesDeriverTest;

  long _statusUpdateInterval;
  int _minTagOccurrencesPerWord;
  int _minWordLength;
  bool _useSchemaTagValuesForWordsOnly;
  QSet<QString> _schemaTagValues;
  QSet<QString> _wordsNotInSchema;
  QSet<QString> _englishWords;
  bool _translateAllNamesToEnglish;

  PoiImplicitTagCustomRules _customRules;

  boost::shared_ptr<QTemporaryFile> _thresholdedCountFile;
  boost::shared_ptr<QTemporaryFile> _filteredCountFile;

  void _removeKvpsBelowOccurrenceThreshold(const QString input, const int minOccurrencesThreshold);
  void _applyFiltering(const QString input);

  QString _getSqliteOutput(const QStringList outputs);
  void _writeRules(const QStringList outputs, const QString sqliteOutputFile);
};

}

#endif // POIIMPLICITTAGRULESDERIVER_H
