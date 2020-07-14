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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "Settings.h"

// hoot
#include <hoot/core/Hoot.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/ConfigDefaults.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/criterion/ElementCriterion.h>

// Boost
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
namespace pt = boost::property_tree;

// Qt
#include <QStringList>

// Standard
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>
using namespace std;

namespace hoot
{

const char* BASE_CONFIG_OPTION_KEY = "base.config";

class JsonLoader
{
public:

  JsonLoader(Settings* s) : _s(s)
  {
  }

  void load(QString path)
  {
    fstream is(path.toUtf8().constData(), ios::in);

    if (is.good() == false)
    {
      throw HootException(QString("Error reading %1").arg(path));
    }

    try
    {
      load(is);
    }
    catch (const HootException& e)
    {
      throw HootException("Error parsing file: " + path + ". " + e.what());
    }
  }

  void load(istream& is)
  {
    try
    {
      pt::ptree pt;
      pt::read_json(is, pt);
      _loadTags(pt);
    }
    catch (const std::exception& e)
    {
      QString reason = e.what();
      throw HootException("Error parsing JSON " + reason);
    }
  }

  void loadFromString(QString json)
  {
    stringstream ss(json.toUtf8().constData(), ios::in);

    if (ss.good() == false)
    {
      throw HootException(QString("Error reading from JSON string:\n%1").arg(json));
    }

    load(ss);
  }

private:

  Settings* _s;

  void _loadTags(pt::ptree& tree)
  {
    pt::ptree::assoc_iterator it = tree.find(BASE_CONFIG_OPTION_KEY);

    if (it != tree.not_found())
    {
      //  Split the base configs and process them in order
      QString base = it->second.data().c_str();
      const QStringList baseConfigs =
        base.trimmed().split(",", QString::SplitBehavior::SkipEmptyParts);
      for (const QString value : baseConfigs)
      {
        LOG_DEBUG("Loading base config: " << value << "...");
        load(ConfPath::search(value));
      }
    }

    //  Iterate all of the children key/value pairs
    for (pt::ptree::value_type& element : tree.get_child(""))
    {
      const QString optionName = QString::fromUtf8(element.first.c_str());
      LOG_VART(optionName);

      //  Skip comments
      if (optionName.startsWith("#"))
        continue;

      //  Skip base config
      if (optionName == BASE_CONFIG_OPTION_KEY)
        continue;

      //  Throw an exception for unrecognized keys
      if (!_s->hasKey(optionName))
        throw HootException("Unknown JSON setting: (" + optionName + ")");

      //  Set key/value pair as name and data, data() turns everything to a string
      const QString optionVal = QString::fromUtf8(element.second.data().c_str());
      LOG_VART(optionVal);
      if (optionVal.contains("->"))
      {
        // '->' replaces one option val with another and there can be multiple list replacement
        // entries within a single opt val string
        const QStringList values = optionVal.split(";", QString::SkipEmptyParts);
        Settings::replaceListOptionEntryValues(*_s, optionName, values);
      }
      else
      {
        _s->set(optionName, optionVal);
      }
    }
  }
};

std::shared_ptr<Settings> Settings::_theInstance = NULL;

Settings::Settings() :
_dynamicRegex("\\$\\{([\\w\\.]+)\\}"),
_staticRegex("\\$\\(([\\w\\.]+)\\)")
{
  _dynamicRegex.optimize();
  _staticRegex.optimize();
}

void Settings::prepend(const QString& key, const QStringList& values)
{
  QStringList l = getList(key, QStringList());
  for (int i = values.size() - 1; i == 0; i--)
  {
    l.prepend(values.at(i));
  }
  set(key, l);
}

void Settings::append(const QString& key, const QStringList& values)
{
  QStringList l = getList(key, QStringList());
  l.append(values);
  set(key, l);
}

void Settings::_checkConvert(const QString& key, const QVariant& value, QVariant::Type type) const
{
  if (value.isNull() || value.canConvert(QVariant::Bool) == false)
  {
    throw HootException(QString("Unable to convert key: '%1', value: '%2' to %3.")
      .arg(key).arg(value.toString()).arg(QVariant::typeToName(type)));
  }
}

void Settings::clear()
{
  // this can be very handy when determining why/when settings got cleared.
  if (this == _theInstance.get())
  {
    LOG_DEBUG("Clearing global settings.");
  }
  _settings.clear();
}

QVariant Settings::get(const QString& key) const
{
  if (hasKey(key) == false)
  {
    throw HootException("Error finding key: " + key);
  }
  QVariant result = _settings.value(key);

  if (result.type() == QVariant::String)
  {
    std::set<QString> used;
    QString r = _replaceVariables(key, used);
    result = r;
  }
  return result;
}

bool Settings::getBool(const QString& key) const
{
  if (hasKey(key) == false)
  {
    throw HootException("Error finding key: " + key);
  }
  const QVariant v = get(key);
  _checkConvert(key, v, QVariant::Bool);
  return v.toBool();
}

bool Settings::getBool(const QString& key, bool defaultValue) const
{
  if (hasKey(key) == false)
  {
    return defaultValue;
  }
  const QVariant v = get(key);
  _checkConvert(key, v, QVariant::Bool);
  return v.toBool();
}

double Settings::getDouble(const QString& key) const
{
  if (hasKey(key) == false)
  {
    throw HootException("Error finding key: " + key);
  }
  const QVariant v = get(key);
  _checkConvert(key, v, QVariant::Double);
  return v.toDouble();
}

double Settings::getDouble(const QString& key, double defaultValue) const
{
  if (hasKey(key) == false)
  {
    return defaultValue;
  }
  const QVariant v = get(key);
  _checkConvert(key, v, QVariant::Double);
  return v.toDouble();
}

double Settings::getDouble(const QString& key, double defaultValue, double min, double max) const
{
  double retVal = getDouble(key, defaultValue);
  if (retVal < min)
  {
    retVal = min;
  }
  else if (retVal > max)
  {
    retVal = max;
  }
  return retVal;
}

double Settings::getDoubleValue(const QString& value) const
{
  QVariant v = getValue(value);

  if (v.isNull() || v.canConvert(QVariant::Double) == false)
  {
    throw HootException("Unable to convert " + v.toString() + " to a double.");
  }
  return v.toDouble();
}

Settings& Settings::getInstance()
{
  if (_theInstance == NULL)
  {
    _theInstance.reset(new Settings());
    _theInstance->loadDefaults();
  }
  return *_theInstance;
}

int Settings::getInt(const QString& key) const
{
  if (hasKey(key) == false)
  {
    throw HootException("Error finding key: " + key);
  }
  const QVariant v = get(key);
  _checkConvert(key, v, QVariant::Int);
  return v.toInt();
}

int Settings::getInt(const QString& key, int defaultValue) const
{
  if (hasKey(key) == false)
  {
    return defaultValue;
  }
  const QVariant v = get(key);
  _checkConvert(key, v, QVariant::Int);
  return v.toInt();
}

int Settings::getInt(const QString& key, int defaultValue, int min, int max) const
{
  int retVal = getInt(key, defaultValue);

  if ( retVal < min )
  {
    retVal = min;
  }
  else if ( retVal > max )
  {
    retVal = max;
  }

  return retVal;
}

long Settings::getLong(const QString& key) const
{
  if (hasKey(key) == false)
  {
    throw HootException("Error finding key: " + key);
  }
  const QVariant v = get(key);
  _checkConvert(key, v, QVariant::LongLong);
  return v.toLongLong();
}

long Settings::getLong(const QString& key, long defaultValue) const
{
  if (hasKey(key) == false)
  {
    return defaultValue;
  }
  const QVariant v = get(key);
  _checkConvert(key, v, QVariant::LongLong);
  return v.toLongLong();
}

long Settings::getLong(const QString& key, long defaultValue, long min, long max) const
{
  long retVal = getLong(key, defaultValue);

  if ( retVal < min )
  {
    retVal = min;
  }
  else if ( retVal > max )
  {
    retVal = max;
  }

  return retVal;
}

QStringList Settings::getList(const QString& key) const
{
  QString str = getString(key);

  return str.split(";");
}

QStringList Settings::getList(const QString& key, const QString& defaultValue) const
{
  QString str = getString(key, defaultValue);

  return str.split(";", QString::SkipEmptyParts);
}

QStringList Settings::getList(const QString& key, const QStringList& defaultValue) const
{
  QStringList result;

  if (hasKey(key))
  {
    QString str = getString(key);
    result = str.split(";", QString::SkipEmptyParts);
  }
  else
  {
    for (int i = 0; i < defaultValue.size(); i++)
    {
      result.append(_replaceVariablesValue(defaultValue[i]));
    }
  }
  return result;
}

QString Settings::getString(const QString& key) const
{
  if (hasKey(key) == false)
  {
    throw HootException("Error finding key: " + key);
  }
  return get(key).toString();
}

QString Settings::getString(const QString& key, const QString& defaultValue) const
{
  if (hasKey(key) == false)
  {
    std::set<QString> used;
    used.insert(key);
    return _replaceVariablesValue(defaultValue, used);
  }
  return getString(key);
}

QVariant Settings::getValue(const QString& value) const
{
  return QVariant(_replaceVariablesValue(value));
}

void Settings::loadEnvironment()
{
  LOG_DEBUG("Loading environment...");
  for (int n = 0; environ[n]; n++)
  {
    QString e = environ[n];
    int i = e.indexOf("=");
    QString k = e.mid(0, i);
    QString v = e.mid(i + 1);
    set(k, v);
  }
}

void Settings::loadDefaults()
{
  ConfigDefaults::populateDefaults(*this);
  QString path = ConfPath::search("hoot.json");
  loadEnvironment();
  loadJson(path);
  try
  {
    QString localPath = ConfPath::search("LocalHoot.json");
    loadJson(localPath);
  }
  catch (const FileNotFoundException&)
  {
    // pass
  }
}

void Settings::loadFromString(QString json)
{
  JsonLoader l(this);
  l.loadFromString(json);
}

void Settings::loadJson(QString path)
{
  JsonLoader l(this);
  l.load(ConfPath::search(path));
}

void Settings::_validateOperatorRefs(const QStringList& operators)
{
  for (int i = 0; i < operators.size(); i++)
  {
    const QString operatorName = operators[i];
    LOG_VART(operatorName);
    const QString errorMsg = "Invalid option operator class name: " + operatorName;

    if (!operatorName.startsWith("hoot::"))
    {
      throw IllegalArgumentException(errorMsg);
    }

    // Should either be a visitor, op, or criterion, but we don't know which one, so check for all
    // of them.
    std::shared_ptr<OsmMapOperation> op;
    try
    {
      op.reset(Factory::getInstance().constructObject<OsmMapOperation>(operatorName));
    }
    catch (const boost::bad_any_cast&)
    {
    }
    catch (const HootException&)
    {
    }

    if (!op)
    {
      std::shared_ptr<ElementVisitor> vis;
      try
      {
        vis.reset(Factory::getInstance().constructObject<ElementVisitor>(operatorName));
      }
      catch (const boost::bad_any_cast&)
      {
      }
      catch (const HootException&)
      {
      }
      if (!vis)
      {
        std::shared_ptr<ElementCriterion> crit;
        try
        {
          crit.reset(Factory::getInstance().constructObject<ElementCriterion>(operatorName));
        }
        catch (const boost::bad_any_cast&)
        {
        }
        catch (const HootException&)
        {
        }
        if (!crit)
        {
          throw IllegalArgumentException(errorMsg);
        }
      }
    }
  }
}

void Settings::parseCommonArguments(QStringList& args)
{
  LOG_DEBUG("Parsing command arguments...");
  LOG_VART(args);

  bool foundOne = true;

  // TODO: This list really should be fed from the options defined in main.cpp in hoot-core-test.
  QStringList hootTestCmdsIgnore;
  hootTestCmdsIgnore.append("--quick");
  hootTestCmdsIgnore.append("--slow");
  hootTestCmdsIgnore.append("--glacial");
  hootTestCmdsIgnore.append("--all");
  hootTestCmdsIgnore.append("--quick-only");
  hootTestCmdsIgnore.append("--slow-only");
  hootTestCmdsIgnore.append("--glacial-only");
  hootTestCmdsIgnore.append("--single");
  hootTestCmdsIgnore.append("--names");
  hootTestCmdsIgnore.append("--all-names");
  hootTestCmdsIgnore.append("--diff");
  hootTestCmdsIgnore.append("--include");
  hootTestCmdsIgnore.append("--exclude");

  const QString optionInputFormatErrorMsg =
    "define must take the form key=value (or key+=value, key++=value, or key-=value).";

  while (args.size() > 0 && foundOne)
  {
    if (args[0] == "--conf" || args[0] == "-C")
    {
      if (args.size() < 2)
      {
        throw HootException("--conf must be followed by a file name.");
      }
      conf().loadJson(args[1]);
      // move on to the next argument.
      args = args.mid(2);
    }
    else if (args[0] == "--trace")
    {
      Log::getInstance().setLevel(Log::Trace);
      args = args.mid(1);
    }
    else if (args[0] == "--debug")
    {
      Log::getInstance().setLevel(Log::Debug);
      args = args.mid(1);
    }
    else if (args[0] == "--info")
    {
      Log::getInstance().setLevel(Log::Info);
      args = args.mid(1);
    }
    else if (args[0] == "--status")
    {
      Log::getInstance().setLevel(Log::Status);
      args = args.mid(1);
    }
    else if (args[0] == "--warn")
    {
      Log::getInstance().setLevel(Log::Warn);
      args = args.mid(1);
    }
    else if (args[0] == "--error")
    {
      Log::getInstance().setLevel(Log::Error);
      args = args.mid(1);
    }
    else if (args[0] == "--fatal")
    {
      Log::getInstance().setLevel(Log::Fatal);
      args = args.mid(1);
    }
    // HootTest settings have already been parsed by this point
    else if (hootTestCmdsIgnore.contains(args[0]))
    {
      args = args.mid(1);
    }
    else if (args[0] == "--define" || args[0] == "-D")
    {
      if (args.size() < 2)
      {
        throw HootException(optionInputFormatErrorMsg);
      }

      QString kv = args[1];
      bool append = false;
      bool remove = false;
      bool prepend = true;
      bool replace = false;

      // '++=' prepends an option to an option list
      QStringList kvl = kv.split("++=");
      if (kvl.size() != 2)
      {
        // '+=' appends an option to an option list
        int sep = kv.indexOf("+=");
        kvl.clear();
        if (sep != -1)
        {
          kvl << kv.mid(0, sep);
          kvl << kv.mid(sep + 2);
        }
        append = true;
        remove = false;
        prepend = false;
        replace = false;
      }

      if (kvl.size() != 2)
      {
        // '-=' removes an option from an option list
        int sep = kv.indexOf("-=");
        kvl.clear();
        if (sep != -1)
        {
          kvl << kv.mid(0, sep);
          kvl << kv.mid(sep + 2);
        }
        append = false;
        remove = true;
        prepend = false;
        replace = false;
      }

      if (kvl.size() != 2)
      {
        // split on the first '='
        int sep = kv.indexOf('=');
        kvl.clear();
        if (sep != -1)
        {
          kvl << kv.mid(0, sep);
          kvl << kv.mid(sep + 1);
        }
        append = false;
        remove = false;
        prepend = false;
        if (kv.contains("->"))
        {
          // '->' replaces one option val with another and there can be multiple list replacement
          // entries within a single opt val string
          replace = true;
        }
        else
        {
          replace = false;
        }
      }

      LOG_VART(append);
      LOG_VART(remove);
      LOG_VART(prepend);
      LOG_VART(replace);

      if (kvl.size() != 2)
      {
        throw IllegalArgumentException(optionInputFormatErrorMsg);
      }

      const QString optionName = kvl[0];
      LOG_VART(optionName);
      const QString optionVal = kvl[1];
      LOG_VART(optionVal);

      if (!conf().hasKey(optionName))
      {
        throw IllegalArgumentException("Unknown settings option: (" + optionName + ")");
      }

      const QStringList values = optionVal.split(";", QString::SkipEmptyParts);
      LOG_VART(values);

      // There are many more options that take class names as input than this. It would be
      // difficult to validate all of them since they inherit from various base classes, so just
      // starting with these more commonly used options whose values are classes that all inherit
      // from ElementVisitor, OsmMapOperation, or ElementCriterion.
      if (optionName == ConfigOptions::getConvertOpsKey() ||
          optionName == ConfigOptions::getConflatePreOpsKey() ||
          optionName == ConfigOptions::getConflatePostOpsKey())
      {
        _validateOperatorRefs(values);
      }

      if (append)
      {
        conf().append(optionName, values);
      }
      else if (remove)
      {
        foreach (QString v, values)
        {
          QStringList newList = conf().getList(optionName);
          if (!newList.contains(v))
          {
            throw IllegalArgumentException("Option list does not contain value: (" + v + ")");
          }
          newList.removeAll(v);
          conf().set(optionName, newList);
        }
      }
      else if (prepend)
      {
        conf().prepend(optionName, values);
      }
      else if (replace)
      {
        replaceListOptionEntryValues(conf(), optionName, values);
      }
      else
      {
        conf().set(optionName, optionVal);
      }

      // move on to the next argument
      args = args.mid(2);
    }
    else
    {
      foundOne = false;
    }
  }

  // re-initialize the logger and other resources after the settings have been parsed.
  Hoot::getInstance().reinit();
}

void Settings::replaceListOptionEntryValues(Settings& settings, const QString& optionName,
                                            const QStringList& listReplacementEntryValues)
{
  LOG_DEBUG(optionName << " before replacement: " << conf().getList(optionName));
  foreach (QString v, listReplacementEntryValues)
  {
    const QStringList valEntryParts = v.split("->");
    if (valEntryParts.size() != 2)
    {
      throw IllegalArgumentException(
        "When replacing one configuration option value with another, the replacement string "
        "must have the form: "
        "\"<old optionValueEntry 1>-><new optionValueEntry 1>;<old optionValueEntry 2>-><new optionValueEntry 2>...\"");
    }
    const QString oldValEntry = valEntryParts[0];
    LOG_VART(oldValEntry);
    const QString newValEntry = valEntryParts[1];
    LOG_VART(newValEntry);
    QStringList newListVal = conf().getList(optionName);
    LOG_VART(newListVal);
    if (!newListVal.contains(oldValEntry))
    {
      throw IllegalArgumentException(
        "Option list: " + optionName + " does not contain value entry: (" + oldValEntry + ")");
    }
    // replace existing option val entry with new one
    newListVal = newListVal.replaceInStrings(oldValEntry, newValEntry);
    LOG_VART(newListVal);
    settings.set(optionName, newListVal);
  }
  LOG_DEBUG(optionName << " after replacement: " << conf().getList(optionName));
}

QString Settings::_replaceVariables(const QString& key, std::set<QString> used) const
{
  if (used.find(key) != used.end())
  {
    throw HootException("Recursive key in configuration file. (" + key + ")");
  }
  // if the variable doesn't exist then it defaults to an empty string.
  if (_settings.contains(key) == false)
  {
    return "";
  }
  QString value = _settings.value(key).toString();

  used.insert(key);
  return _replaceVariablesValue(value, used);
}

QString Settings::_replaceStaticVariables(QString value) const
{
  bool done = false;

  while (!done)
  {
    done = true;
    int offset = 0;
    QRegularExpressionMatch match = _staticRegex.match(value, offset);
    if (match.hasMatch())
    {
      QString varStr = match.captured(0);
      QString subKey = match.captured(1);
      QString expanded = "";
      if (hasKey(subKey))
        expanded = getString(subKey);
      value.replace(varStr, expanded);
      offset += expanded.length();
      done = false;
    }
  }

  return value;
}

QString Settings::_replaceVariablesValue(QString value) const
{
  std::set<QString> used;
  return _replaceVariablesValue(value, used);
}

QString Settings::_replaceVariablesValue(QString value, std::set<QString> used) const
{
  bool done = false;

  while (!done)
  {
    done = true;
    int offset = 0;
    QRegularExpressionMatch match = _dynamicRegex.match(value, offset);
    if (match.hasMatch())
    {
      QString varStr = match.captured(0);
      QString subKey = match.captured(1);
      QString expanded = _replaceVariables(subKey, used);
      value.replace(varStr, expanded);
      offset += expanded.length();
      done = false;
    }
  }

  return value;
}

void Settings::set(const QString& key, const QStringList &value)
{
  set(key, value.join(";"));
}

void Settings::set(const QString& key, const QString& value)
{
  _settings[key] = _replaceStaticVariables(value);
}

void Settings::storeJson(const QString& path) const
{
  fstream os(path.toUtf8().constData(), ios::out);

  if (os.good() == false)
  {
    throw HootException(QString("Error opening %1 for writing.").arg(path));
  }

  os << toString().toUtf8().constData();
}

QString Settings::toString() const
{
  QString result = "{\n";

  for (SettingsMap::const_iterator it = _settings.constBegin(); it != _settings.constEnd(); ++it)
  {
    result += QString("  \"%1\":\"%2\",\n").arg(_markup(it.key()))
        .arg(_markup(it.value().toString()));
  }
  result += "  \"#end\": \"\"\n";

  result += "}\n";

  return result;
}

}
