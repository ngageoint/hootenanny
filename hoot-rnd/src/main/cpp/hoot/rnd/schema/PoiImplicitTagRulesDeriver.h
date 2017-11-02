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
//#include <hoot/rnd/util/FixedLengthString.h>
#include <hoot/rnd/schema/PoiImplicitTagRulesDeriverTempDbWriter.h>

// Qt
#include <QString>
#include <QMap>
#include <QTemporaryFile>
#include <QCache>

namespace hoot
{

class Tags;

/**
 * Derives implicit tag rules for POIs and writes the rules to various output formats
 */
class PoiImplicitTagRulesDeriver
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
   * @param typeKeys an optional list of OSM tag keys for which to derive rules; if empty, rules
   * will be derived for all OSM types
   * @param minOccurancesThreshold an optional minimum tag occurrance threshold to use when
   * deriving rules; rules will only be derived when a word is associated with a particular tag
   * at least as many times as this value
   */
  void deriveRules(const QStringList inputs, const QStringList translationScripts,
                   const QStringList outputs, const QStringList typeKeys = QStringList(),
                   const int minOccurancesThreshold = 1);

  //static FixedLengthString qStrToFixedLengthStr(const QString wordKvp);
  //static QString fixedLengthStrToQStr(const FixedLengthString& fixedLengthStr);

private:

  //for testing
  friend class PoiImplicitTagRulesDeriverTest;

  //TODO: replace with BigMap
  /*FixedLengthStringToFixedLengthStringMap*//*Tgs::BigMap<FixedLengthString, FixedLengthString>*/
  QMap<QString, QString> _wordCaseMappings;
  //TODO: replace with BigMap
  //Technically, this could be done with a vector, but I want to piggy back off BigMap.
  /*FixedLengthStringToLongMap*/QMap<QString, long> _wordKeysToCounts;

  boost::shared_ptr<QTemporaryFile> _countFile;
  boost::shared_ptr<QTemporaryFile> _sortedCountFile;
  boost::shared_ptr<QTemporaryFile> _sortedDedupedCountFile;
  boost::shared_ptr<QTemporaryFile> _sortedByWordDedupedCountFile;

  long _statusUpdateInterval;

  //not expecting these to be too large, so keeping in memory
  QStringList _tagIgnoreList;
  QStringList _wordIgnoreList;

  PoiImplicitTagRulesDeriverTempDbWriter _tempDbWriter;

  bool _runInMemory;
  QCache<QString, QString> _wordCaseMappingsCache;
  long _maxCacheSize;
  bool _wordCaseCacheLimitMessageShown;

  void _updateForNewWord(QString word, const QString kvp);
  QStringList _getPoiKvps(const Tags& tags) const;

  //temp
  //static QMap<QString, long> _stxxlMapToQtMap(const FixedLengthStringToLongMap& stxxlMap);

  void _removeKvpsBelowOccuranceThresholdAndSortByOccurrance(const int minOccurancesThreshold);
  void _removeDuplicatedKeyTypes();
  void _sortByWord();
  void _updateSortedDedupedFile(QString word, const QString kvp, const long count);

  QString _getSqliteOutput(const QStringList outputs);
  void _writeRules(const QStringList outputs, const QString sqliteOutputFile);

  void _readIgnoreLists();
};

}

#endif // POIIMPLICITTAGRULESDERIVER_H
