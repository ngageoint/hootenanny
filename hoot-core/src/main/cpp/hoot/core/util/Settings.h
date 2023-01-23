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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Maxar (http://www.maxar.com/)
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// Qt
#include <QRegularExpression>
#include <QVariant>

// Std
// Strangely getting compile errors in hoot-test w/o this, even though its in HootCoreStable.h.
#include <set>
#include <stack>

namespace hoot
{

/**
 * Stores Hootenanny configuration options (Singleton)
 *
 * This class favors convenience over performance so use it appropriately outside performance
 * critical code sections.
 *
 * Settings support both static and dynamic variables. Static variables are denoted with
 * $(VAR_NAME) and dynamic variables are denoted with ${VAR_NAME}. Static variables are evaluated
 * at the time when the value is set. Dynamic variables are evaluated when get is called.
 */
class Settings
{
public:

  // Technically, this is a Singleton and this constructor should not be publicly accessible. There
  // does seem to be a use case for passing around temporary settings, though, which then makes
  // sense for it to remain public. Possibly, we need a separate class for HootSettings that would
  // then be a true Singleton?
  Settings();
  Settings(const Settings& s) = default;
  Settings(Settings && s) noexcept;

  Settings& operator=(const Settings& s) = default;

  using SettingsMap = QHash<QString, QVariant>;

  void append(const QString& key, const QStringList& values);
  void prepend(const QString& key, const QStringList& values);

  /**
   * Removes all user defined settings. This is most useful for unit testing.
   */
  void clear();

  /**
   * @brief push - Push a copy of the current settings to the stack.
   *               Current settings remain unmodified.
   */
  void push();

  /**
   * @brief pop - Pops settings off the stack, and overwrites the current
   *              settings with them. If there is nothing to pop, just
   *              do nothing.
   */
  void pop();

  /**
   * Retrieves the default configuration. This is global.
   */
  static Settings& getInstance();

  void loadDefaults();
  /**
   * Loads the system environment variables
   */
  void loadEnvironment();
  void loadFromString(QString json);
  /**
   * Load the JSON file at the path specified.
   */
  void loadJson(QString path);

  QVariant get(const QString& key) const;

  QString getString(const QString& key) const;
  QString getString(const QString& key, const QString& defaultValue) const;

  const SettingsMap& getAll() const { return _settings; }

  bool getBool(const QString& key) const;
  bool getBool(const QString& key, bool defaultValue) const;

  double getDouble(const QString& key) const;
  double getDouble(const QString& key, double defaultValue) const;
  double getDouble(const QString& key, double defaultValue, double minValue,
                   double maxValue) const;
  /**
   * Returns the double represented in the supplied string. Where necessary variables will be
   * resolved.
   */
  double getDoubleValue(const QString& value) const;

  int getInt(const QString& key) const;
  int getInt(const QString& key, int defaultValue) const;
  int getInt(const QString& key, int defaultValue, int min, int max) const;

  long getLong(const QString& key) const;
  long getLong(const QString& key, long defaultValue) const;
  long getLong(const QString& key, long defaultValue, long min, long max) const;
 
  /**
   * Returns a QStringList from the value. The QStringList is created by
   * QString::split(";", QString::SkipEmptyParts)
   */
  QStringList getList(const QString& key) const;
  QStringList getList(const QString& key, const QString& defaultValue) const;
  QStringList getList(const QString& key, const QStringList& defaultValue) const;

  /**
   * Returns the QVariant represented in the supplied string. Where necessary variables will be
   * resolved.
   */
  QVariant getValue(const QString& value) const;

  bool hasKey(const QString& key) const { return _settings.contains(key); }

  /**
   * @brief parseCommonArguments parses common arguments (e.g. "-D foo=bar --debug")
   *
   * Any arguments parsed are removed from args.
   * @param args the arguments to parse
   * @param toIgnore option skip list; arguments in this list are not parsed
   */
  static void parseCommonArguments(QStringList& args, const QStringList toIgnore = QStringList());

  void set(const QString& key, const char* value) { set(key, QString(value)); }
  void set(const QString& key, const std::string& value)
  { set(key, QString::fromStdString(value)); }
  void set(const QString& key, const QString& value);
  void set(const QString& key, double value) { _settings[key] = QVariant(value); }
  void set(const QString& key, int value) { _settings[key] = QVariant(value); }
  void set(const QString& key, bool value) { set(key, value ? "true" : "false"); }
  void set(const QString& key, const QStringList& value);

  void storeJson(const QString& path) const;

  QString toString() const;

  int size() const { return _settings.size(); }

  /**
   * Determines if this instance contains no settings
   *
   * @return true if no settings are present; false otherwise
   */
  bool isEmpty() const { return _settings.size() == 0; }

  /**
   * Replace individual list entries in the current value of a configuration option with new list
   * entries
   *
   * @param settings settings configuration owning the list option to modify
   * @param optionName name of the list option to modify
   * @param listReplacementEntryValues list of value replacement entries of the form:
   * "<old optionValueEntry 1>-><new optionValueEntry 1>;<old optionValueEntry 2>-><new optionValueEntry 2>..."
   */
  static void replaceListOptionEntryValues(Settings& settings, const QString& optionName, const QStringList& listReplacementEntryValues);

private:

  static std::shared_ptr<Settings> _theInstance;
  SettingsMap _settings;

  // Used to push and pop configurations
  std::stack<SettingsMap> _settingsStack;

  // matches variables in the form ${My_Var_1}
  QRegularExpression _dynamicRegex;
  // matches variables in the form $(My_Var_1)
  QRegularExpression _staticRegex;

  QString _markup(QString s) const;

  void _checkConvert(const QString& key, const QVariant& value, QVariant::Type type) const;

  QString _replaceStaticVariables(QString value) const;
  QString _replaceVariables(const QString& key, std::set<QString> used) const;
  QString _replaceVariablesValue(QString value) const;
  QString _replaceVariablesValue(QString value, const std::set<QString>& used) const;

  /*
   * This validates input for options that take a list of hoot operators (factory configurable
   * ElementVisitor or OsmMapOperation)
   */
  static void _validateOperatorRefs(const QStringList& operators);

  // This ensure option values matching factory class names always have the global namespace
  // prepended to them. This allows users to avoid having to type in the namespace repeatedly when
  // using commands and shorten command text. If hoot ever updates namespace usage to anything other
  // than a single global namespece, then this logic would become obsolete.
  static void _updateClassNamesInList(QStringList& list);
};

inline Settings& conf() { return Settings::getInstance(); }

}

#endif // CONFIGURATION_H
