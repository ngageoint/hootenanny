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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "StringUtils.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QLocale>

namespace hoot
{

QString StringUtils::millisecondsToDhms(const qint64 durationInMilliseconds)
{
  QString res;
  int duration = (int)(durationInMilliseconds / 1000);
  const int seconds = (int)(duration % 60);
  duration /= 60;
  const int minutes = (int)(duration % 60);
  duration /= 60;
  const int hours = (int)(duration % 24);
  const int days = (int)(duration / 24);
  if ((hours == 0) && (days == 0))
  {
    return res.sprintf("%02d:%02d", minutes, seconds);
  }
  if (days == 0)
  {
    return res.sprintf("%02d:%02d:%02d", hours, minutes, seconds);
  }
  return res.sprintf("%dd%02d:%02d:%02d", days, hours, minutes, seconds);
}

QString StringUtils::formatLargeNumber(const unsigned long number)
{
  return QLocale(QLocale::English).toString((qulonglong)number);
}

bool StringUtils::hasAlphabeticCharacter(const QString& input)
{
  for (int i = 0; i < input.length(); i++)
  {
    if (input.at(i).isLetter())
    {
      return true;
    }
  }
  return false;
}

bool StringUtils::hasDigit(const QString& input)
{
  for (int i = 0; i < input.length(); i++)
  {
    if (input.at(i).isDigit())
    {
      return true;
    }
  }
  return false;
}

bool StringUtils::isAlphaNumeric(const QString& input)
{
  for (int i = 0; i < input.length(); i++)
  {
    const QChar character = input.at(i);
    if (!character.isLetterOrNumber())
    {
      return false;
    }
  }
  return true;
}

bool StringUtils::isNumber(const QString& input)
{
  bool isNumber = false;
  input.toLong(&isNumber);
  return isNumber;
}

std::shared_ptr<boost::property_tree::ptree> StringUtils::jsonStringToPropTree(
  const QString& jsonStr)
{
  LOG_VART(jsonStr);
  std::stringstream strStrm(jsonStr.toUtf8().constData(), std::ios::in);
  if (!strStrm.good())
  {
    throw HootException(QString("Error reading from reply string:\n%1").arg(jsonStr));
  }
  std::shared_ptr<boost::property_tree::ptree> jsonObj(new boost::property_tree::ptree());
  try
  {
    boost::property_tree::read_json(strStrm, *jsonObj);
  }
  catch (boost::property_tree::json_parser::json_parser_error& e)
  {
    QString reason = QString::fromStdString(e.message());
    QString line = QString::number(e.line());
    throw HootException(QString("Error parsing JSON: %1 (line %2)").arg(reason).arg(line));
  }
  return jsonObj;
}

std::shared_ptr<boost::property_tree::ptree> StringUtils::stringListToJsonStringArray(
  const QStringList& stringList)
{
  std::shared_ptr<boost::property_tree::ptree> strArr(new boost::property_tree::ptree());
  for (int i = 0; i < stringList.size(); i++)
  {
    boost::property_tree::ptree str;
    str.put("", stringList.at(i).toStdString());
    strArr->push_back(std::make_pair("", str));
  }
  return strArr;
}

QString StringUtils::getNumberStringPaddedWithZeroes(const int number, const int padSize)
{
  return QString("%1").arg(number, padSize, 10, QChar('0'));
}

void StringUtils::removeEmptyStrings(QStringList& strings)
{
  QStringList output;
  for (int i = 0; i < strings.size(); i++)
  {
    if (!strings.at(i).trimmed().isEmpty())
    {
      output.append(strings.at(i));
    }
  }
  strings = output;
}

QSet<QString> StringUtils::getDuplicates(const QStringList& input)
{
  QSet<QString> duplicateStrings;
  QSet<QString> uniqueStrings;
  for (int i = 0; i < input.size(); i++)
  {
    const QString str = input.at(i);
    if (uniqueStrings.contains(str))
    {
      duplicateStrings.insert(str);
    }
    else
    {
      uniqueStrings.insert(str);
    }
  }
  return duplicateStrings;
}

bool StringUtils::containsSubstring(const QStringList& input, const QString& substring)
{
  return input.filter(substring, Qt::CaseInsensitive).size() > 0;
}

void StringUtils::removeAll(QStringList& input, const QStringList& toRemove)
{
  for (int i = 0; i < toRemove.size(); i++)
  {
    input.removeAll(toRemove.at(i));
  }
}

bool StringUtils::containsAny(const QStringList& input, const QStringList& toCompare)
{
  for (int i = 0; i < toCompare.size(); i++)
  {
    // may eventually want a case sensitivity option here
    if (input.contains(toCompare.at(i)))
    {
      return true;
    }
  }
  return false;
}

void StringUtils::removeAllWithKey(QMap<QString, QString>& input, const QStringList& keysToRemove)
{
  for (int i = 0; i < keysToRemove.size(); i++)
  {
    input.remove(keysToRemove.at(i));
  }
}

}
