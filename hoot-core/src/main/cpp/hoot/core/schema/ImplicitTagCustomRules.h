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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef IMPLICITTAGCUSTOMRULES_H
#define IMPLICITTAGCUSTOMRULES_H

namespace hoot
{

/**
 * Custom rules to invoke when creating an implicit tag rules database
 */
class ImplicitTagCustomRules
{

public:

  ImplicitTagCustomRules();
  ~ImplicitTagCustomRules() = default;

  QStringList getTagIgnoreList() const { return _tagIgnoreList; }
  QStringList getWordIgnoreList() const { return _wordIgnoreList; }
  QMap<QString, QString> getCustomRulesList() const { return _customRulesList; }

  void setTagIgnoreList(const QStringList& list) { _tagIgnoreList = list; }
  void setWordIgnoreList(const QStringList& list) { _wordIgnoreList = list; }
  void setCustomRuleFile(const QString& file);

private:

  // tags that are to be ignored when building the rules database
  QStringList _tagIgnoreList;
  // words (name tokens) that are to be ignored when building the rules database
  QStringList _wordIgnoreList;
  // tag rules to be applied manually to the rules database
  QMap<QString, QString> _customRulesList;
  // file which contains tag rules to be applied manually to the rules database
  QString _customRuleFile;

  void _clear();
  void _readCustomRuleFile();
};

}

#endif // IMPLICITTAGCUSTOMRULES_H
