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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "Tags.h"

// Hoot
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Units.h>
#include <hoot/core/util/UuidHelper.h>

using namespace std;

namespace hoot
{

QStringList Tags::_nameKeys;
QStringList Tags::_pseudoNameKeys;

Tags::Tags(const QString& key, const QString& value)
  : QHash<QString, QString>()
{
  set(key, value);
}

bool Tags::isValidKvp(const QString& str)
{
  if (!str.contains("="))
    return false;

  const QStringList kvpParts = str.split("=");
  if (kvpParts.size() != 2)
    return false;

  const QString key = kvpParts[0];
  const QString val = kvpParts[1];
  if (key.trimmed().isEmpty() || val.trimmed().isEmpty())
    return false;

  return true;
}

QString Tags::kvpToKey(const QString& kvp)
{
  if (!kvp.contains("="))
    return "";

  const QStringList kvpParts = kvp.split("=");
  if (kvpParts.size() != 2)
    return "";

  return kvpParts[0].trimmed();
}

QString Tags::kvpToVal(const QString& kvp)
{
  if (!kvp.contains("="))
    return "";

  const QStringList kvpParts = kvp.split("=");
  if (kvpParts.size() != 2)
    return "";

  return kvpParts[1].trimmed();
}

QStringList Tags::kvpToParts(const QString& kvp)
{
  if (!kvp.contains("="))
    return QStringList();
  return kvp.split("=");
}

void Tags::add(const Tags& t)
{
  for (auto it = t.constBegin(); it != t.constEnd(); ++it)
    operator[](it.key()) = it.value();
}

void Tags::addNote(const QString& note)
{
  appendValue("note", note);
}

void Tags::appendValue(const QString& kvp)
{
  const QString errorMsg = "KVP: " + kvp + " should be of the format: key=value";
  if (!kvp.contains("="))
    throw HootException(errorMsg);

  const QStringList kvpParts = kvp.split("=");
  if (kvpParts.size() != 2)
    throw HootException(errorMsg);

  appendValue(kvpParts[0].trimmed(), kvpParts[1].trimmed());
}

void Tags::appendValue(const QString& k, const QString& v)
{
  QString oldV = value(k);

  if (oldV != "")
  {
    QStringList l = split(oldV);
    l.append(v);
    setList(k, l);
  }
  else
    insert(k, v);
}

void Tags::appendValue(const QString& k, const QStringList& v)
{
  QString oldV = value(k);

  QStringList l = split(oldV);
  l.append(v);
  setList(k, l);
}

void Tags::appendValueIfUnique(const QString& k, const QStringList& v)
{
  QString oldV = value(k);

  if (oldV != "")
  {
    QStringList l = split(oldV);

    for (const auto& value : qAsConst(v))
    {
      if (!l.contains(value))
        l.append(value);
    }

    setList(k, l);
  }
  else
    setList(k, v);
}

void Tags::appendValueIfUnique(const QString& k, const QString& v)
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
    insert(k, v);
}

bool Tags::hasInformationTag() const
{
  for (auto it = constBegin(); it != constEnd(); ++it)
  {
    QString key = it.key();
    //LOG_VART(key);
    if (OsmSchema::getInstance().isMetaData(key, it.value()) == false && it.value() != "")
      return true;
  }
  return false;
}

QStringList Tags::getCreateUuid()
{
  QStringList result;

  if (contains(uuidKey()))
    result = getList(uuidKey());
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
    throw HootException("Expected a double for key: " + k + " but got: " + v);

  return result;
}

double Tags::getDouble(const QString& k, double defaultValue) const
{
  if (contains(k))
    return getDouble(k);
  else
    return defaultValue;
}

Velocity Tags::getVelocity(const QString& k) const
{
  QString v = get(k);

  QMap<QString, Velocity> stringUnits;
  stringUnits["km/h"] = getKph();
  stringUnits["kph"] = getKph();
  stringUnits["kmph"] = getKph();

  stringUnits["mph"] = getMph();
  stringUnits["knots"] = getKnotph();

  QString num;
  QString units;
  _valueRegexParser(v, num, units);

  bool ok;
  double r = num.toDouble(&ok);
  if (!ok)
    throw HootException("Expected a double for key: " + k + " but got: " + v);

  Velocity result;
  if (stringUnits.contains(units))
    result = r * stringUnits[units];
  else
    throw HootException("Invalid units: " + units);

  return result;
}

Length Tags::getLength(const QString& k) const
{
  QString v = get(k);

  Length result;
  bool ok;
  // feet and inch are special case, do check and calculation.
  if (v.contains("'", Qt::CaseInsensitive) || v.contains("\"", Qt::CaseInsensitive))
  {
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
          vi = vi.replace("\"","");

        vfd = vf.toDouble(&ok);
        if (!ok)
          throw HootException("Expected a double for key: " + k + " but got: " + v);

        if (!vi.isEmpty())
        {
          vid = vi.toDouble(&ok);
          if (!ok)
            throw HootException("Expected a double for key: " + k + " but got: " + v);
        }
      }
      else
      {
        vf = vfi[0];
        vfd = vf.toDouble(&ok);
        if (!ok)
          throw HootException("Expected a double for key: " + k + " but got: " + v);
      }
    }
    result = (vfd + vid * 0.0833333) * getFeetLength();
  }
  else
  {
    QString num;
    QString units;

    QMap<QString, Length> stringUnits;
    stringUnits["miles"] = getMileLength();
    stringUnits["mile"] = getMileLength();
    stringUnits["mi"] = getMileLength();

    stringUnits["km"] = getKiloLength();
    stringUnits["kilometre"] = getKiloLength();
    stringUnits["kilometres"] = getKiloLength();
    stringUnits["kilometer"] = getKiloLength();
    stringUnits["kilometers"] = getKiloLength();

    stringUnits["nmi"] = getNmiLength();
    stringUnits["international nautical mile"] = getNmiLength();
    stringUnits["ft"] = getFeetLength();
    stringUnits["feet"] = getFeetLength();
    stringUnits["foot"] = getFeetLength();
    stringUnits["decimetres"] = getDecimiLength();

    _valueRegexParser(v, num, units);

    double r = num.toDouble(&ok);
    if (!ok)
      throw HootException("Expected a double for key: " + k + " but got: " + v);

    if (stringUnits.contains(units))
      result = r * stringUnits[units];
    else
      throw HootException("Invalid units: " + units);
  }
  return result;
}

int Tags::getInformationCount() const
{
  int count = 0;
  for (auto it = constBegin(); it != constEnd(); ++it)
  {
    QString key = it.key();
    //LOG_VART(key);
    if (OsmSchema::getInstance().isMetaData(key, it.value()) == false && it.value() != "")
      count++;
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

      for (auto it = constBegin(); it != constEnd(); ++it)
      {
        if (regex.exactMatch(it.key()))
          result.append(it.key());
      }
    }
    else if (contains(k[i]))
      result.append(k[i]);
  }

  return result;
}

bool Tags::hasName() const
{
  return !getName().isEmpty();
}

bool Tags::haveMatchingName(const Tags& tags1, const Tags& tags2, const bool strictNameMatch)
{
  const QStringList tag1Names = tags1.getNames(!strictNameMatch);
  const QStringList tag2Names = tags2.getNames(!strictNameMatch);
  for (const auto& tag1Name : tag1Names)
  {
    for (const auto& tag2Name : tag2Names)
    {
      if (tag1Name.compare(tag2Name, Qt::CaseInsensitive) == 0)
        return true;
    }
  }
  return false;
}

QStringList Tags::getNames(const bool includeAltName) const
{
  QStringList result;

  if (empty())
    return result;

  // make sure the _nameKeys field is populated.
  getNameKeys();

  for (auto key : _nameKeys)
  {
    if (includeAltName || (!includeAltName && key.toLower() != "alt_name"))
      readValues(key, result);
  }

  return result;
}

QString Tags::getName() const
{
  QString name = get("name").trimmed();
  if (!name.isEmpty())
    return name;
  else
  {
    QStringList names = getNames();
    for (const auto& n : qAsConst(names))
    {
      name = n.trimmed();
      //arbitrarily returning the first name here
      if (!name.isEmpty())
        return name;
    }
  }
  return "";
}

const QStringList& Tags::getNameKeys()
{
  // Getting the name tags can be a bit expensive, so we'll just do it once.
  if (_nameKeys.empty())
  {
    const vector<SchemaVertex>& tags =
      OsmSchema::getInstance().getTagByCategory(OsmSchemaCategory::name());
    for (const auto& vertex : tags)
      _nameKeys.append(vertex.getKey());
  }

  return _nameKeys;
}

QStringList Tags::getNameKeys(const Tags& tags)
{
  QStringList nameKeysInTags;
  const QStringList globalNameKeys = getNameKeys();
  for (const auto& nameKey : globalNameKeys)
  {
    if (tags.contains(nameKey))
      nameKeysInTags.append(nameKey);
  }
  return nameKeysInTags;
}

int Tags::getNonDebugCount() const
{
  int count = 0;
  for (auto it = constBegin(); it != constEnd(); ++it)
  {
    QString key = it.key();
    if (!key.startsWith(MetadataTags::HootTagPrefix()) && key != "created_by" && it.value() != "")
      count++;
  }
  return count;
}

const QStringList& Tags::getPseudoNameKeys() const
{
  // getting the name tags can be a bit expensive so we'll just do it once.
  if (_pseudoNameKeys.empty())
  {
    const vector<SchemaVertex>& tags =
        OsmSchema::getInstance().getTagByCategory(OsmSchemaCategory::pseudoName());

    for (const auto& vertex : tags)
    {
      LOG_TRACE("key : " << (vertex.getKey().toStdString()));
      _pseudoNameKeys.append(vertex.getKey());
    }
  }

  return _pseudoNameKeys;
}

QStringList Tags::getPseudoNames() const
{
  QStringList result;

  if (empty())
    return result;

  // make sure the _nameKeys field is populated.
  getPseudoNameKeys();

  for (const auto& key : qAsConst(_pseudoNameKeys))
    readValues(key, result);

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
    return false;

  for (auto it = constBegin(); it != constEnd(); ++it)
  {
    QStringList l1 = split(it.value());
    l1.sort();
    QStringList l2 = split(other.get(it.key()));
    l2.sort();
    if (l1 != l2)
      return false;
  }

  return true;
}

bool Tags::hasSameNonMetadataTags(const Tags& other) const
{
  Tags otherCopy = other;
  otherCopy.removeMetadata();
  Tags thisCopy = *this;
  thisCopy.removeMetadata();
  return otherCopy == thisCopy;
}

int Tags::removeMetadata()
{
  int numRemoved = removeByTagKeyStartsWith(MetadataTags::HootTagPrefix());

  // there are some other metadata keys that don't start with 'hoot:'
  QStringList keysToRemove;
  OsmSchema& schema = OsmSchema::getInstance();
  for (auto it = constBegin(); it != constEnd(); ++it)
  {
    const QString key = it.key();
    if (schema.isMetaData(key, it.value()))
      keysToRemove.append(key);
  }

  for (const auto& key : qAsConst(keysToRemove))
    numRemoved += remove(key);

  return numRemoved;
}

int Tags::removeKeys(const QStringList& keys)
{
  int numRemoved = 0;
  for (const auto& key : qAsConst(keys))
  {
    LOG_TRACE("Removing " << key << "...");
    numRemoved += remove(key);
  }
  return numRemoved;
}

int Tags::removeKey(const QRegExp& regex)
{
  QStringList keysToRemove;
  for (auto it = constBegin(); it != constEnd(); ++it)
  {
    const QString key = it.key();
    if (regex.exactMatch(key))
      keysToRemove.append(key);
  }

  return removeKeys(keysToRemove);
}

int Tags::removeKeys(const QList<QRegExp>& regexes)
{
  int numRemoved = 0;
  for (const auto& regex : qAsConst(regexes))
    numRemoved += removeKey(regex);

  return numRemoved;
}

int Tags::removeByTagKeyStartsWith(const QString& tagKeySubstring)
{
  QStringList keysToRemove;
  for (auto it = constBegin(); it != constEnd(); ++it)
  {
    const QString key = it.key();
    if (key.startsWith(tagKeySubstring))
      keysToRemove.append(key);
  }

  int numRemoved = 0;
  for (const auto& key : qAsConst(keysToRemove))
    numRemoved += remove(key);
  return numRemoved;
}

QStringList Tags::getDataOnlyValues(const Tags& tags) const
{
  QStringList t;
  for (auto it = tags.constBegin(); it != tags.constEnd(); ++it)
  {
    if (!it.key().startsWith(MetadataTags::HootTagPrefix()))
      t.append(it.value());
  }
  return t;
}

bool Tags::dataOnlyEqual(const Tags& other) const
{
  QStringList l1 = getDataOnlyValues(*this);
  QStringList l2 = getDataOnlyValues(other);

  if (l1.size() != l2.size())
    return false;

  for (int index = 0; index < l1.size(); ++index)
  {
    QStringList keys1 = split(l1[index]);
    keys1.sort();
    QStringList keys2 = split(l2[index]);
    keys2.sort();
    if (l1 != l2)
      return false;
  }

  return true;
}

Meters Tags::readMeters(const QString& key) const
{
  bool ok;
  // TODO: add support for ft suffix too.
  Meters result = value(key).toDouble(&ok);
  if (ok == false)
    throw HootException("Invalid value in meters, key: " + key + " value: " + value(key));
  return result;
}

void Tags::readValues(const QString &k, QStringList& list) const
{
  static QString regexStart("regex?");
  if (k.startsWith(regexStart))
  {
    QString regexStr = k.mid(6);
    QRegExp regex(regexStr);

    for (auto it = constBegin(); it != constEnd(); ++it)
    {
      if (regex.exactMatch(it.key()))
        list.append(split(it.value()));
    }
  }
  else if (contains(k))
    list.append(split(value(k)));
}

void Tags::set(const QString& key, const QString& value)
{
  operator[](key) = value;
}

void Tags::set(const QString& key, bool v)
{
  if (v)
    set(key, "yes");
  else
    set(key, "no");
}

void Tags::set(const Tags& other)
{
  for (auto it = other.constBegin(); it != other.constEnd(); ++it)
    set(it.key(), it.value());
}

QStringList Tags::split(const QString& values)
{
  QChar semi = ';';
  QString semi2 = ";;";
  QStringList result;

  int strStart = 0;
  int last = values.length() - 1;
  for (int i = 0; i < values.length(); ++i)
  {
    // if there is an empty string at the beginning
    if (i == 0)
    {
      if (values[0] == semi && (values.size() == 1 || values[1] != semi))
        strStart = 1;
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
  for (auto it = constBegin(); it != constEnd(); ++it)
    result += it.key() + " = " + it.value() + "\n";
  return result;
}

void Tags::_valueRegexParser(const QString& str, QString& num, QString& units) const
{
  QRegExp nRegExp("(\\d+(\\.\\d+)?)");
  int pos = 0;
  while ((pos = nRegExp.indexIn(str,pos)) != -1)
  {
    num = nRegExp.cap(1).trimmed();
    pos += nRegExp.matchedLength();
  }

  QRegExp sRegExp("(\\d+(\\.\\d+)?)");
  QString copyStr = str;
  units = copyStr.replace(sRegExp, QString("")).trimmed();
}

QString Tags::getFirstMatchingKvp(const QStringList& kvps) const
{
  for (const auto& kvp : qAsConst(kvps))
  {
    if (!kvp.contains("="))
      throw IllegalArgumentException("Invalid kvp: " + kvp);

    const QStringList kvpParts = kvp.split("=");
    if (kvpParts.size() != 2)
      throw IllegalArgumentException("Invalid kvp: " + kvp);

    const QString key = kvpParts[0];
    const QString value = kvpParts[1];
    if ((value != "*" && get(key) == value) || (value == "*" && contains(key)))
      return key + "=" + value;
  }
  return "";
}

bool Tags::hasKvp(const QString& kvp) const
{
  return hasAnyKvp(QStringList(kvp));
}

bool Tags::hasAnyKvp(const QStringList& kvps) const
{
  return !getFirstMatchingKvp(kvps).isEmpty();
}

QStringList Tags::toKvps() const
{
  QStringList kvps;
  for (auto it = constBegin(); it != constEnd(); ++it)
    kvps.append(it.key() + "=" + it.value());

  return kvps;
}

QStringList Tags::toKeys() const
{
  QStringList keys;
  for (auto it = constBegin(); it != constEnd(); ++it)
    keys.append(it.key());

  return keys;
}

bool Tags::hasAnyKey(const QStringList& keys) const
{
  for (const auto& key : qAsConst(keys))
  {
    if (contains(key))
      return true;
  }
  return false;
}

QString Tags::getFirstMatchingKey(const QStringList& keys) const
{
  for (const auto& key : qAsConst(keys))
  {
    if (contains(key))
      return key;
  }
  return "";
}

Tags Tags::kvpListToTags(const QStringList& kvps)
{
  Tags tagsToReturn;
  for (const auto& tagStr : qAsConst(kvps))
  {
    if (!tagStr.contains("="))
      throw IllegalArgumentException("Invalid tag: " + tagStr);

    const QStringList tagStrParts = tagStr.split("=");
    if (tagStrParts.size() != 2)
      throw IllegalArgumentException("Invalid tag: " + tagStr);

    tagsToReturn.appendValue(tagStrParts[0], tagStrParts[1]);
  }
  return tagsToReturn;
}

Tags Tags::schemaVerticesToTags(const std::vector<SchemaVertex>& schemaVertices)
{
  Tags tags;
  for (const auto& vertex : schemaVertices)
    tags.appendValue(vertex.getKey(), vertex.getValue());
  return tags;
}

bool Tags::bothContainKvp(const Tags& tags1, const Tags& tags2, const QString& kvp)
{
  return tags1.hasKvp(kvp) && tags2.hasKvp(kvp);
}

bool Tags::onlyOneContainsKvp(const Tags& tags1, const Tags& tags2, const QString& kvp)
{
  const bool firstHasKvp = tags1.hasKvp(kvp);
  const bool secondHasKvp = tags2.hasKvp(kvp);
  return (!firstHasKvp && secondHasKvp) || (firstHasKvp && !secondHasKvp);
}

bool Tags::bothHaveInformation(const Tags& tags1, const Tags& tags2)
{
  return tags1.hasInformationTag() && tags2.hasInformationTag();
}

bool Tags::onlyOneHasInformation(const Tags& tags1, const Tags& tags2)
{
  return tags1.hasInformationTag() || tags2.hasInformationTag();
}

}
