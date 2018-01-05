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
#ifndef IMPLICITTAGRAWRULESDERIVER_H
#define IMPLICITTAGRAWRULESDERIVER_H

// Hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/filters/ImplicitTagEligibleCriterion.h>

// Qt
#include <QString>
#include <QHash>
#include <QTemporaryFile>

namespace hoot
{

class Tags;

/**
 * Derives implicit tag rules for POIs and writes the rules to various output formats
 */
class ImplicitTagRawRulesDeriver : public Configurable
{

public:

  ImplicitTagRawRulesDeriver();

  /**
   * Derives implicit tag rules for POIs given input data and writes the rules to output
   *
   * @param inputs a list of hoot supported feature input formats to derive rules from
   * @param translationScripts list of OSM translation scripts corresponding to the datasets
   * specified by the inputs parameter
   * @param outputs a list of hoot supported implicit tag rule output formats
   */
  void deriveRawRules(const QStringList inputs, const QStringList translationScripts,
                      const QString output);

  virtual void setConfiguration(const Settings& conf);

  void setSortParallelCount(int count) { _sortParallelCount = count; }
  void setSkipFiltering(bool skip) { _skipFiltering = skip; }
  void setKeepTempFiles(bool keep) { _keepTempFiles = keep; }
  void setTempFileDir(QString dir) { _tempFileDir = dir; }
  void setTranslateAllNamesToEnglish(bool translate) { _translateAllNamesToEnglish = translate; }
  void setElementFilter(const QString type);

private:

  //for testing
  friend class ImplicitTagRawRulesDeriverTest;

  long _statusUpdateInterval;
  boost::shared_ptr<QFile> _output;

  boost::shared_ptr<QTemporaryFile> _countFile;
  boost::shared_ptr<QTemporaryFile> _sortedCountFile;
  boost::shared_ptr<QTemporaryFile> _dedupedCountFile;
  boost::shared_ptr<QTemporaryFile> _tieResolvedCountFile;

  QHash<QString, QString> _wordKeysToCountsValues;
  QHash<QString, QStringList> _duplicatedWordTagKeyCountsToValues;

  long _countFileLineCtr;
  int _sortParallelCount;
  bool _skipFiltering;
  bool _keepTempFiles;
  QString _tempFileDir;
  bool _translateAllNamesToEnglish;

  boost::shared_ptr<ImplicitTagEligibleCriterion> _elementFilter;

  void _updateForNewWord(QString word, const QString kvp);
  QStringList _getPoiKvps(const Tags& tags) const;

  void _sortByTagOccurrence();
  void _removeDuplicatedKeyTypes();
  void _resolveCountTies();
  void _sortByWord(boost::shared_ptr<QTemporaryFile> input);
};

}

#endif // IMPLICITTAGRAWRULESDERIVER_H
