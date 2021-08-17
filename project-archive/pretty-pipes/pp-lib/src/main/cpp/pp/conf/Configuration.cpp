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
 * @copyright Copyright (C) 2015, 2017 Maxar (http://www.maxar.com/)
 */

#include "Configuration.h"

// Pretty Pipes
#include <pp/Exception.h>

// Qt
#include <QString>

using namespace std;

namespace pp
{

Configuration::Configuration()
{
}

const string& Configuration::get(const string& key, const string& defaultValue) const
{
  if (hasKey(key))
  {
    return get(key);
  }
  else
  {
    return defaultValue;
  }
}

bool Configuration::getBoolean(const std::string& key) const
{
  bool ok;
  string value = _conf.at(key);
  QString str = QString::fromStdString(value);
  bool result = false;
  int v = str.toInt(&ok);
  if (ok == true)
  {
    if (v)
    {
      result = true;
    }
    else
    {
      result = false;
    }
  }
  else if (str == "true" || str == "on" || str == "yes")
  {
    result = true;
  }
  else if (str == "false" || str == "off" || str == "no")
  {
    result = false;
  }
  else
  {
    throw Exception("Error parsing bool.");
  }

  return result;
}

string Configuration::getBytes(const std::string& key) const
{
  std::string value = get(key);
  QByteArray arr(value.data(), value.size());
  QByteArray bytes = QByteArray::fromBase64(arr);
  std::string bytesStr(bytes.data(), bytes.size());
  return bytesStr;
}

double Configuration::getDouble(const std::string& key) const
{
  bool ok;
  string value = _conf.at(key);
  double result = QString::fromStdString(value).toDouble(&ok);
  if (ok == false)
  {
    throw Exception("Error parsing double.");
  }
  return result;
}

float Configuration::getFloat(const std::string& key) const
{
  bool ok;
  string value = _conf.at(key);
  float result = QString::fromStdString(value).toFloat(&ok);
  if (ok == false)
  {
    throw Exception("Error parsing float.");
  }
  return result;
}

int Configuration::getInt(const std::string& key) const
{
  bool ok;
  string value = _conf.at(key);
  int result = QString::fromStdString(value).toInt(&ok);
  if (ok == false)
  {
    throw Exception("Error parsing int.");
  }
  return result;
}

int Configuration::getInt(const std::string& key, int defaultValue) const
{
  if (hasKey(key))
  {
    return getInt(key);
  }
  else
  {
    return defaultValue;
  }
}

long Configuration::getLong(const std::string& key) const
{
  bool ok;
  string value = _conf.at(key);
  long result = QString::fromStdString(value).toLong(&ok);
  if (ok == false)
  {
    throw Exception(QString("Error parsing long. %1 = %2").arg(QString::fromStdString(key)).
      arg(QString::fromStdString(value)));
  }
  return result;
}

void Configuration::setBytes(const std::string& key, const std::string& value)
{
  QByteArray arr(value.data(), value.size());
  QByteArray base64 = arr.toBase64();
  std::string base64Str(base64.data(), base64.size());
  set(key, base64Str);
}

}
