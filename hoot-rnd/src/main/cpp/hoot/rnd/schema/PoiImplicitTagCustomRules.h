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
#ifndef POIIMPLICITTAGCUSTOMRULES_H
#define POIIMPLICITTAGCUSTOMRULES_H

// Qt
#include <QStringList>
#include <QMap>

namespace hoot
{

/**
 *
 */
class PoiImplicitTagCustomRules
{

public:

  PoiImplicitTagCustomRules();

  void init();

  QString getCustomRuleFile() const { return _customRuleFile; }
  void setCustomRuleFile(const QString file) { _customRuleFile = file; }
  QString getRuleIgnoreFile() const { return _ruleIgnoreFile; }
  void setRuleIgnoreFile(const QString file) { _ruleIgnoreFile = file; }
  QString getTagIgnoreFile() const { return _tagIgnoreFile; }
  void setTagIgnoreFile(const QString file) { _tagIgnoreFile = file; }
  QString getTagFile() const { return _tagFile; }
  void setTagFile(const QString file) { _tagFile = file; }
  QString getWordIgnoreFile() const { return _wordIgnoreFile; }
  void setWordIgnoreFile(const QString file) { _wordIgnoreFile = file; }

  QStringList getTagIgnoreList() const { return _tagIgnoreList; }
  QStringList getWordIgnoreList() const { return _wordIgnoreList; }
  QMap<QString, QString> getCustomRulesList() const { return _customRulesList; }
  QMap<QString, QString> getRulesIgnoreList() const { return _rulesIgnoreList; }
  QStringList getTagsAllowList() const { return _tagsAllowList; }

private:

  QString _customRuleFile;
  QString _ruleIgnoreFile;
  QString _tagIgnoreFile;
  QString _tagFile;
  QString _wordIgnoreFile;

  QStringList _tagIgnoreList;
  QStringList _wordIgnoreList;
  QMap<QString, QString> _customRulesList;
  QMap<QString, QString> _rulesIgnoreList;
  QStringList _tagsAllowList;

  void _readIgnoreLists();
  void _readAllowLists();
  void _clear();
};

}

#endif // POIIMPLICITTAGCUSTOMRULES_H
