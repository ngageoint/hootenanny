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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "Settings.h"

// hoot
#include <hoot/core/Hoot.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/ConfigDefaults.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

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
    catch (HootException& e)
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
    catch (std::exception e)
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
    BOOST_FOREACH(pt::ptree::value_type& element, tree.get_child(""))
    {
      QString name = QString::fromUtf8(element.first.c_str());
      //  Skip comments
      if (name.startsWith("#"))
        continue;
      if (!_s->hasKey(name))
      {
        LOG_WARN("Unknown JSON setting: (" << name << ")");
        //  Don't allow unknown settings to be loaded
        continue;
      }
      //  Set key/value pair as name and data, data() turns everything to a string
      _s->set(name, QString::fromUtf8(element.second.data().c_str()));
    }
  }
};

Settings* Settings::_theInstance = NULL;

Settings::Settings() :
  _dynamicRegex("\\$\\{([\\w\\.]+)\\}"),
  _staticRegex("\\$\\(([\\w\\.]+)\\)")
{

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
  if (this == _theInstance)
  {
    LOG_INFO("Clearing global settings.");
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

double Settings::getDouble(const QString& key, double defaultValue, double min, double max ) const
{
  double retVal = getDouble(key, defaultValue);
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
    _theInstance = new Settings();
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
  for (int n = 0; environ[n]; n++)
  {
    QString e = environ[n];
    int i = e.indexOf("=");
    QString k = e.mid(0, i);
    QString v = e.mid(i + 1);
    set(k, v);
  }

  QString env = QString::fromUtf8(getenv("HOOT_OPTIONS"));

  if (!env.isEmpty())
  {
    QStringList args = env.split(" ", QString::SkipEmptyParts);
    parseCommonArguments(args);
    if (args.size() != 0)
    {
      LOG_WARN("Error parsing all arguments in HOOT_OPTIONS: " << args);
    }
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
  catch(FileNotFoundException& e)
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

void Settings::parseCommonArguments(QStringList& args)
{
  bool foundOne = true;

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
    else if (args[0] == "--verbose")
    {
      Log::getInstance().setLevel(Log::Verbose);
      args = args.mid(1);
    }
    else if (args[0] == "--info")
    {
      Log::getInstance().setLevel(Log::Info);
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
    else if (args[0] == "--define" || args[0] == "-D")
    {
      if (args.size() < 2)
      {
        throw HootException("--define or -D must be followed by key=value.");
      }
      QString kv = args[1];
      QStringList kvl = kv.split("+=");
      bool append = true;
      bool remove = false;
      if (kvl.size() != 2)
      {
        // split on the first '-='
        int sep = kv.indexOf("-=");
        kvl.clear();
        if (sep != -1)
        {
          kvl << kv.mid(0, sep);
          kvl << kv.mid(sep + 2);
        }
        append = false;
        remove = true;
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
      }
      if (kvl.size() != 2)
      {
        throw HootException("define must takes the form key=value.");
      }
      if (!conf().hasKey(kvl[0]))
      {
        LOG_WARN("Unknown settings option: (" << kvl[0] << ")");
        // move on to the next argument, don't keep an invalid option
        args = args.mid(2);
        continue;
      }
      if (append)
      {
        QStringList values = kvl[1].split(";", QString::SkipEmptyParts);
        conf().append(kvl[0], values);
      }
      else if (remove)
      {
        QStringList values = kvl[1].split(";", QString::SkipEmptyParts);
        foreach (QString v, values)
        {
          QStringList newList = conf().getList(kvl[0]);
          newList.removeAll(v);
          conf().set(kvl[0], newList);
        }
      }
      else
      {
        conf().set(kvl[0], kvl[1]);
      }
      // move on to the next argument.
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
    if (_staticRegex.indexIn(value, offset) >= 0)
    {
      offset += _staticRegex.matchedLength();
      QString varStr = _staticRegex.capturedTexts()[0];
      QString subKey = _staticRegex.capturedTexts()[1];
      QString expanded;
      if (hasKey(subKey))
      {
        expanded = getString(subKey);
      }
      value.replace(varStr, expanded);
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
    if (_dynamicRegex.indexIn(value, offset) >= 0)
    {
      offset += _dynamicRegex.matchedLength();
      QString varStr = _dynamicRegex.capturedTexts()[0];
      QString subKey = _dynamicRegex.capturedTexts()[1];
      QString expanded = _replaceVariables(subKey, used);
      value.replace(varStr, expanded);
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
