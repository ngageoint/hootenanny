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
#ifndef IMPLICITTAGCUSTOMRULES_H
#define IMPLICITTAGCUSTOMRULES_H

// Qt
#include <QStringList>
#include <QMap>

namespace hoot
{

/**
 * Custom rules to invoke when creating an implicit tag rules database
 */
class ImplicitTagCustomRules
{

public:

  ImplicitTagCustomRules() = default;

  void init();

  QString getCustomRuleFile() const { return _customRuleFile; }
  void setCustomRuleFile(const QString& file) { _customRuleFile = file; }

  QString getTagIgnoreFile() const { return _tagIgnoreFile; }
  void setTagIgnoreFile(const QString& file) { _tagIgnoreFile = file; }

  QString getWordIgnoreFile() const { return _wordIgnoreFile; }
  void setWordIgnoreFile(const QString& file) { _wordIgnoreFile = file; }

  QStringList getTagIgnoreList() const { return _tagIgnoreList; }
  QStringList getWordIgnoreList() const { return _wordIgnoreList; }

  QMap<QString, QString> getCustomRulesList() const { return _customRulesList; }

private:

  //file which contains tag rules to be applied manually to the rules database
  QString _customRuleFile;
  //file which contains tags that are to be ignored when building the rules database
  QString _tagIgnoreFile;
  //file which contains words (name tokens) that are to be ignored when building the rules database
  QString _wordIgnoreFile;

  QStringList _tagIgnoreList;
  QStringList _wordIgnoreList;
  QMap<QString, QString> _customRulesList;

  void _readIgnoreLists();
  void _readAllowLists();
  void _clear();
  void _readCustomRuleFile();
};

}

#endif // IMPLICITTAGCUSTOMRULES_H
