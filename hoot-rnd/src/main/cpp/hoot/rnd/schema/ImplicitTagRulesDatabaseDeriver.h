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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef IMPLICITTAGRULESDATABASEDERIVER_H
#define IMPLICITTAGRULESDATABASEDERIVER_H

// Hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/rnd/schema/ImplicitTagCustomRules.h>

// Qt
#include <QString>
#include <QTemporaryFile>
#include <QSet>

namespace hoot
{

class Tags;

/**
 * Derives an implicit tag rules database for elements, given a set of raw implicit tag rules
 *
 * See note in ImplicitTagRawRulesDeriver as to why this logic is separated from that in
 * ImplicitTagRawRulesDeriver.
 */
class ImplicitTagRulesDatabaseDeriver : public Configurable
{

public:

  ImplicitTagRulesDatabaseDeriver();
  ~ImplicitTagRulesDatabaseDeriver() = default;

  /**
   * Creates an implicit tag rules database given input element data
   *
   * @param input a hoot supported feature input formats to derive implicit tag rules from
   * @param output a Sqlite database output file to store the implicit tag rules in
   */
  void deriveRulesDatabase(const QString& input, const QString& output);

  void setConfiguration(const Settings& conf) override;

  void setMinTagOccurrencesPerWord(const int minOccurrences)
  { _minTagOccurrencesPerWord = minOccurrences; }
  void setMinWordLength(const int length) { _minWordLength = length; }
  void setUseSchemaTagValuesForWordsOnly(bool use) { _useSchemaTagValuesForWordsOnly = use; }
  void setCustomRuleFile(const QString& file) { _customRules.setCustomRuleFile(file); }
  void setTagIgnoreFile(const QString& file) { _customRules.setTagIgnoreFile(file); }
  void setWordIgnoreFile(const QString& file) { _customRules.setWordIgnoreFile(file); }

private:

  //for testing
  friend class ImplicitTagRulesDatabaseDeriverTest;

  long _statusUpdateInterval;
  //the minimum number of times a word/tag combo is seen together for a rule to be generated for it
  int _minTagOccurrencesPerWord;
  //all words under this length will be ignored
  int _minWordLength;
  //If true, only words which match an OSM schema tag value will be considered for rule generation.
  //This greatly reduces the number of rules generated but can be useful in reducing rules which
  //lead to false positive type tagging.  Since OSM tags are in English, all names must be
  //translated to English if this is set to true.  Also, all taggers using databases generated with
  //this setting equal to true must also translate any element names to English that they use to
  //query against the rules database.
  bool _useSchemaTagValuesForWordsOnly;

  //Set of OSM schema tag values extracted from all hoot schema files
  QSet<QString> _schemaTagValues;
  //any words found which don't correspond to an OSM schema tag value
  QSet<QString> _wordsNotInSchema;

  //custom tag rules which are written to the rules database after it has been generated from the
  //raw input rules; These custom rules can be used to override any bad rules derived by the
  //auto-generation.
  ImplicitTagCustomRules _customRules;

  //file which has been thresholded at a word/tag occurrence count equal to or greater than
  //_minTagOccurrencesPerWord
  std::shared_ptr<QTemporaryFile> _thresholdedCountFile;
  //file which has had filtering applied to it based on _customRules
  std::shared_ptr<QTemporaryFile> _filteredCountFile;

  void _validateInputs(const QString& input, const QString& output) const;

  void _populateSchemaTagValues();
  bool _wordIsNotASchemaTagValue(const QString& word) const;

  void _writeRules(const QString& input, const QString& output) const;
  void _writeCustomRules(long& linesWrittenCount) const;

  void _removeKvpsBelowOccurrenceThreshold(const QString& input, const int minOccurrencesThreshold);
  /*
   * Applies all rules filtering based on the parsed contents of _customRules
   */
  void _applyFiltering(const QString& input);
};

}

#endif // IMPLICITTAGRULESDATABASEDERIVER_H
