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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "Tags.h"

// Hoot
#include <hoot/core/Exception.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/UuidHelper.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/Units.h>

namespace hoot
{

QStringList Tags::_nameKeys;
QStringList Tags::_pseudoNameKeys;

void Tags::addTags(const Tags& t)
{
  for (Tags::const_iterator it = t.constBegin(); it != t.constEnd(); it++)
  {
    operator[](it.key()) = it.value();
  }
}

void Tags::addNote(QString note)
{
  appendValue("note", note);
}

void Tags::appendValue(QString k, QString v)
{
  QString oldV = value(k);

  if (oldV != "")
  {
    QStringList l = split(oldV);
    l.append(v);
    setList(k, l);
  }
  else
  {
    insert(k, v);
  }
}

void Tags::appendValue(QString k, QStringList v)
{
  QString oldV = value(k);

  QStringList l = split(oldV);
  l.append(v);
  setList(k, l);
}

void Tags::appendValueIfUnique(QString k, QStringList v)
{
  QString oldV = value(k);

  if (oldV != "")
  {
    QStringList l = split(oldV);

    for (int i = 0; i < v.size(); i++)
    {
      if (!l.contains(v[i]))
      {
        l.append(v[i]);
      }
    }

    setList(k, l);
  }
  else
  {
    setList(k, v);
  }
}

void Tags::appendValueIfUnique(QString k, QString v)
{
  QString oldV = value(k);

  if (oldV != "")
  {
    QStringList l = split(oldV);
    if (!l.contains(v))
    {
      l.append(v);
      setList(k, l);
    }
  }
  else
  {
    insert(k, v);
  }
}

QStringList Tags::getCreateUuid()
{
  QStringList result;

  if (contains(uuidKey()))
  {
    result = getList(uuidKey());
  }
  else
  {
    QString uuid = UuidHelper::createUuid().toString();
    set(uuidKey(), uuid);

    result << uuid;
  }

  return result;
}

double Tags::getDouble(const QString& k) const
{
  QString v = get(k);

  bool ok;
  double result = v.toDouble(&ok);
  if (!ok)
  {
    throw HootException("Expected a double for key: " + k + " but got: " + v);
  }
  return result;
}

double Tags::getDouble(const QString& k, double defaultValue) const
{
  if (contains(k))
  {
    return getDouble(k);
  }
  else
  {
    return defaultValue;
  }
}

double Tags::getVelocity(const QString& k) const
{
  QString v = get(k);

  bool isKm = false;    //kilometer per hour
  bool isMph = false;   //miles per hour
  bool isKnots = false; //knots
  if (v.contains("km/h", Qt::CaseInsensitive) || v.contains("kph", Qt::CaseInsensitive)
      || v.contains("kmph", Qt::CaseInsensitive))
  {
     isKm = true;
  }
  else if (v.contains("mph", Qt::CaseInsensitive))
  {
    isMph = true;
  }
  else if (v.contains("knots", Qt::CaseInsensitive))
  {
    isKnots = true;
  }
  else //default kilometre per hour is assumed
  {
    isKm = true;
  }

  bool ok;
  double result = v.split(" ")[0].toDouble(&ok);
  if (!ok)
  {
    throw HootException("Expected a double for key: " + k + " but got: " + v);
  }

  if (isKm)
  {
    result = kiloToVelocity(result).value();
  }
  else if (isMph)
  {
    result = mileToVelocity(result).value();
  }
  else if (isKnots)
  {
    result = knotToVelocity(result).value();
  }
  return result;
}

double Tags::getLength(const QString& k) const
{
  QString v = get(k);

  bool isKm = false;  //kilometer
  bool isMi = false;  //mile
  bool isNmi = false; //nautical mile
  bool isFi = false;  //feet and inch

  if (v.contains("km", Qt::CaseInsensitive) || v.contains("kilometre", Qt::CaseInsensitive)
      || v.contains("kilometres", Qt::CaseInsensitive) || v.contains("kilometer", Qt::CaseInsensitive)
      || v.contains("kilometers", Qt::CaseInsensitive))
  {
     isKm = true;
  }
  else if ((v.contains("mi", Qt::CaseInsensitive) && !v.contains("n", Qt::CaseInsensitive))
            || v.contains("mile", Qt::CaseInsensitive) || v.contains("miles", Qt::CaseInsensitive))
  {
    isMi = true;
  }
  else if (v.contains("nmi", Qt::CaseInsensitive) || v.contains("international nautical mile", Qt::CaseInsensitive))
  {
    isNmi = true;
  }
  else if (v.contains("'", Qt::CaseInsensitive) || v.contains("\"", Qt::CaseInsensitive))
  {
    isFi = true;
  }

  bool ok;
  double result = v.split(" ")[0].toDouble(&ok);
  if (!ok && !isFi)
  {
    throw HootException("Expected a double for key: " + k + " but got: " + v);
  }

  if (isKm)
  {
    result = kiloToLength(result).value();
  }
  else if (isMi)
  {
    result = mileToLength(result).value();
  }
  else if (isNmi)
  {
    result = nmiToLength(result).value();
  }
  else if (isFi)
  {
    v = v.split(" ")[0];

    QString vf = "";
    QString vi = "";
    double vfd = 0.0;
    double vid = 0.0;
    if (v.contains("'", Qt::CaseInsensitive))
    {
      QStringList vfi = v.split("'");
      if (vfi.length() == 2)
      {
        vf = vfi[0];
        vi = vfi[1];
        if (vi.contains("\""))
        {
          vi = vi.replace("\"","");
        }
        vfd = vf.toDouble(&ok);
        if (!ok)
        {
          throw HootException("Expected a double for key: " + k + " but got: " + v);
        }
        if (!vi.isEmpty())
        {
          vid = vi.toDouble(&ok);
          if (!ok)
          {
            throw HootException("Expected a double for key: " + k + " but got: " + v);
          }
        }
      }
      else
      {
        vf = vfi[0];
        vfd = vf.toDouble(&ok);
        if (!ok)
        {
          throw HootException("Expected a double for key: " + k + " but got: " + v);
        }
      }
    }
    result = feetToLength(vfd + vid * 0.0833333).value();
  }
  return result;
}

int Tags::getInformationCount() const
{
  int count = 0;
  for (Tags::const_iterator it = constBegin(); it != constEnd(); it++)
  {
    QString key = it.key();
    if (OsmSchema::getInstance().isMetaData(key, it.value()) == false &&
        it.value() != "")
    {
      //LOG_DEBUG("info key: " + key);
      count++;
    }
  }
  return count;
}

QStringList Tags::getList(const QString& k) const
{
  QStringList result;
  readValues(k, result);
  return result;
}

QStringList Tags::getMatchingKeys(const QStringList& k)
{
  QStringList result;
  for (int i = 0; i < k.size(); i++)
  {
    if (k[i].startsWith("regex?"))
    {
      QString regexStr = k[i].mid(6);
      QRegExp regex(regexStr);

      for (const_iterator it = begin(); it != end(); it++)
      {
        if (regex.exactMatch(it.key()))
        {
          result.append(it.key());
        }
      }
    }
    else if (contains(k[i]))
    {
      result.append(k[i]);
    }
  }

  return result;
}

QStringList Tags::getNames() const
{
  QStringList result;

  if (size() == 0)
  {
    return result;
  }

  // make sure the _nameKeys field is populated.
  getNameKeys();

  for (int i = 0; i < _nameKeys.size(); i++)
  {
    readValues(_nameKeys[i], result);
  }

  return result;
}

const QStringList& Tags::getNameKeys()
{
  // getting the name tags can be a bit expensive so we'll just do it once.
  if (_nameKeys.size() == 0)
  {
    const vector<SchemaVertex>& tags =
        OsmSchema::getInstance().getTagByCategory(OsmSchemaCategory::name());

    for (size_t i = 0; i < tags.size(); i++)
    {
      //LOG_INFO("key : " << (tags[i].key.toStdString()));
      _nameKeys.append(tags[i].key);
    }
  }

  return _nameKeys;
}

int Tags::getNonDebugCount() const
{
  int count = 0;
  for (Tags::const_iterator it = constBegin(); it != constEnd(); it++)
  {
    QString key = it.key();
    if (!key.startsWith("hoot:") && key != "created_by" && it.value() != "")
    {
      //LOG_DEBUG("non-debug key: " + key);
      count++;
    }
  }
  return count;
}

const QStringList& Tags::getPseudoNameKeys() const
{
  // getting the name tags can be a bit expensive so we'll just do it once.
  if (_pseudoNameKeys.size() == 0)
  {
    const vector<SchemaVertex>& tags =
        OsmSchema::getInstance().getTagByCategory(OsmSchemaCategory::pseudoName());

    for (size_t i = 0; i < tags.size(); i++)
    {
      //LOG_INFO("key : " << (tags[i].key.toStdString()));
      _pseudoNameKeys.append(tags[i].key);
    }
  }

  return _pseudoNameKeys;
}

QStringList Tags::getPseudoNames() const
{
  QStringList result;

  if (size() == 0)
  {
    return result;
  }

  // make sure the _nameKeys field is populated.
  getPseudoNameKeys();

  for (int i = 0; i < _pseudoNameKeys.size(); i++)
  {
    readValues(_pseudoNameKeys[i], result);
  }

  return result;
}

bool Tags::isFalse(const QString& key) const
{
  QString v = value(key).toLower();
  return (v == "no" || v == "false" || v == "0" || v == "off");
}

bool Tags::isTrue(const QString& key) const
{
  QString v = value(key).toLower();
  return (v == "yes" || v == "true" || v == "1" || v == "on");
}

bool Tags::operator==(const Tags& other) const
{
  if (other.size() != size())
  {
    return false;
  }

  for (const_iterator it = begin(); it != end(); it++)
  {
    QStringList l1 = split(it.value());
    l1.sort();
    QStringList l2 = split(other.get(it.key()));
    l2.sort();
    if (l1 != l2)
    {
      return false;
    }
  }

  return true;
}

Meters Tags::readMeters(QString key) const
{
  bool ok;
  // todo add support for ft suffix too.
  Meters result = value(key).toDouble(&ok);
  if (ok == false)
  {
    throw Exception("Invalid value in meters, key: " + key + " value: " + value(key));
  }
  return result;
}

void Tags::readValues(const QString &k, QStringList& list) const
{
  static QString regexStart("regex?");
  if (k.startsWith(regexStart))
  {
    QString regexStr = k.mid(6);
    QRegExp regex(regexStr);

    for (const_iterator it = begin(); it != end(); it++)
    {
      if (regex.exactMatch(it.key()))
      {
        list.append(split(it.value()));
      }
    }
  }
  else if (contains(k))
  {
    list.append(split(value(k)));
  }
}

void Tags::removeEmptyTags()
{
  // remove all the empty tags.
  for (Tags::const_iterator it = begin(); it != end(); ++it)
  {
    if (get(it.key()) == "")
    {
      remove(it.key());
    }
  }
}

void Tags::set(const QString& key, const QString& value)
{
  operator[](key) = value;
}

void Tags::set(const QString& key, bool v)
{
  if (v)
  {
    set(key, "yes");
  }
  else
  {
    set(key, "no");
  }
}

void Tags::set(const Tags& other)
{
  for (Tags::const_iterator it = other.constBegin(); it != other.constEnd(); ++it)
  {
    set(it.key(), it.value());
  }
}

QStringList Tags::split(const QString& values)
{
  QChar semi = ';';
  QString semi2 = ";;";
  QStringList result;

  int strStart = 0;
  int last = values.length() - 1;
  for (int i = 0; i < values.length(); i++)
  {
    // if there is an empty string at the beginning
    if (i == 0)
    {
      if (values[0] == semi && (values.size() == 1 || values[1] != semi))
      {
        strStart = 1;
      }
    }
    // if there is an empty string at the end
    else if (i == last)
    {
      if (values[last] == semi && values[last - 1] != semi)
      {
        QString s = values.mid(strStart, i - strStart);
        result << s.replace(semi2, semi);
        strStart = i + 1;
      }
    }
    else if (values[i] == semi && values[i - 1] != semi && values[i + 1] != semi)
    {
      QString s = values.mid(strStart, i - strStart);
      result << s.replace(semi2, semi);
      strStart = i + 1;
    }
  }

  if (strStart <= last)
  {
    QString s = values.mid(strStart);
    result << s.replace(semi2, semi);
  }

  return result;
}

QString Tags::toString() const
{
  QString result;

  for (Tags::const_iterator it = constBegin(); it != constEnd(); it++)
  {
    result += it.key() + " = " + it.value() + "\n";
  }
  return result;
}

}
