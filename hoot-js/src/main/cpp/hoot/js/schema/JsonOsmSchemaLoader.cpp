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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "JsonOsmSchemaLoader.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/js/util/DataConvertJs.h>
#include <hoot/js/util/StreamUtilsJs.h>
#include <hoot/core/schema/SchemaChecker.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QByteArray>
#include <QFile>
#include <QDir>

namespace hoot
{

unsigned int JsonOsmSchemaLoader::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmSchemaLoader, JsonOsmSchemaLoader)

JsonOsmSchemaLoader::JsonOsmSchemaLoader()
{
  _context = Context::New();
}

void JsonOsmSchemaLoader::load(QString path, OsmSchema& s)
{
  QFile fp(path);
  if (fp.open(QFile::ReadOnly) == false)
  {
    throw HootException("Error opening JSON file: " + path);
  }

  // maintain a stack of file locations so imports happen properly
  _baseDir.append(QDir(path + "/..").absolutePath() + "/");
  _deps.insert(path);
  QByteArray ba = fp.readAll();

  HandleScope handleScope;
  Context::Scope context_scope(_context);

  // If needed, this will throw an exception with user readable(ish) error message.
  v8::Handle<v8::Value> result = fromJson(QString::fromUtf8(ba.data(), ba.size()), path);

  QVariantList l = toCpp<QVariantList>(result);

  for (int i = 0; i < l.size(); i++)
  {
    try
    {
      if (l[i].type() != QVariant::Map)
      {
        throw HootException(QString("Expected to get a dict as the 'object', got: %2").
          arg(l[i].typeName()));
      }
      _processObject(l[i].toMap(), s);
    }
    catch(HootException& e)
    {
      throw HootException(QString("Error processing '%1' in object '%2'. %3").
        arg(path).arg(toJson(toV8(l[i]))).arg(e.getWhat()));
    }
  }

  _baseDir.pop_back();

  // update implied values via inheritance after all imports are processed
  if (_baseDir.size() == 0)
  {
    s.update();

    //check if schemavertex is unknown type or has empty geometries
    SchemaChecker(s).check();
  }
}

double JsonOsmSchemaLoader::_asDouble(const QVariant& v) const
{
  bool ok;
  double result = v.toDouble(&ok);
  if (!ok)
  {
    throw IllegalArgumentException(QString("Expected to receive a number, but got: %1 (%2)").
      arg(toString(v)).arg(v.typeName()));
  }
  return result;
}

QString JsonOsmSchemaLoader::_asString(const QVariant& v) const
{
  if (v.type() == QVariant::String)
  {
    return v.toString();
  }
  else
  {
    throw IllegalArgumentException(QString("Expected to receive a string, but got: %1 (%2)").
      arg(toString(v)).arg(v.typeName()));
  }
}

QStringList JsonOsmSchemaLoader::_asStringList(const QVariant& v) const
{
  QStringList result;

  if (v.type() == QVariant::List)
  {
    QVariantList l = v.toList();
    result.reserve(l.size());

    for (int i = 0; i < l.size(); i++)
    {
      result.push_back(_asString(l[i]));
    }
  }
  else
  {
    throw IllegalArgumentException(QString("Expected to receive a list, but got: %1 (%2)").
      arg(toString(v)).arg(v.typeName()));
  }

  return result;
}

void _loadAssociatedWith(SchemaVertex& tv, const QVariant& v, OsmSchema& s)
{
  if (v.type() != QVariant::List)
  {
    throw HootException("Expected an array for associatedWith.");
  }
  else
  {
    const QVariantList l = v.toList();

    for (int i = 0; i < l.size(); ++i)
    {
      s.addAssociatedWith(tv.name, toString(l[i]));
    }
  }
}

void JsonOsmSchemaLoader::_loadBase(QVariantMap& copy, OsmSchema& s, SchemaVertex& tv)
{
  // we don't need this anymore
  copy.remove("objectType");
  // we don't use this.
  copy.remove("tagInfoCount");

  if (copy.contains("associatedWith"))
  {
    _loadAssociatedWith(tv, copy.take("associatedWith"), s);
  }
  if (copy.contains("isA"))
  {
    s.addIsA(tv.name, _asString(copy.take("isA")));
  }
  if (copy.contains("similarTo"))
  {
    _loadSimilarTo(tv.name, copy.take("similarTo"), s);
  }
  if (copy.contains("description"))
  {
    tv.description = _asString(copy.take("description"));
  }
  if (copy.contains("tagInfoDescription"))
  {
    if (tv.description.isEmpty())
    {
      tv.description = _asString(copy["tagInfoDescription"]);
    }
    copy.remove("tagInfoDescription");
  }
  if (copy.contains("influence"))
  {
    tv.influence = _asDouble(copy.take("influence"));
  }
  if (copy.contains("childWeight"))
  {
    tv.childWeight = _asDouble(copy.take("childWeight"));
  }
  if (copy.contains("mismatchScore"))
  {
    tv.mismatchScore = _asDouble(copy.take("mismatchScore"));
  }
  if (copy.contains("aliases"))
  {
    tv.aliases = _asStringList(copy.take("aliases"));
  }
  if (copy.contains("categories"))
  {
    tv.categories = _asStringList(copy.take("categories"));
  }
  if (copy.contains("geometries"))
  {
    _loadGeometries(tv, copy.take("geometries"));
  }

  if (copy.size() != 0)
  {
    if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN(QString("Unrecognized tags found in %1: (%2)").arg(tv.name).
        arg(toJson(toV8(copy.keys()))));
    }
    else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }
}

void JsonOsmSchemaLoader::_loadCompound(const QVariantMap& v, OsmSchema& s)
{
  QVariantMap copy;
  // copy all the non-comments
  for (QVariantMap::const_iterator it = v.begin(); it != v.end(); ++it)
  {
    if (it.key().startsWith('#') == false)
    {
      copy[it.key()] = it.value();
    }
  }

  SchemaVertex tv;
  tv.setType(SchemaVertex::Compound);

  if (copy.contains("name"))
  {
    tv.setName(_asString(copy.take("name")));
  }
  else
  {
    throw HootException("Expected the object to contain a name.");
  }
  if (copy.contains("tags"))
  {
    _loadCompoundTags(tv, copy.take("tags"));
  }
  else
  {
    throw HootException(QString("Expected compound to contain a 'tags' entry. (%1)").
      arg(copy["name"].toString()));
  }

  // load all the values common to tag and compound.
  _loadBase(copy, s, tv);

  s.updateOrCreateVertex(tv);
}

void JsonOsmSchemaLoader::_loadCompoundTags(SchemaVertex& tv, const QVariant& value)
{
  if (value.type() != QVariant::List)
  {
    throw HootException("Expected an array for compound tags.");
  }
  else
  {
    QVariantList arr = value.toList();

    if (arr.size() == 0)
    {
      throw HootException("A compound tag must have 1 or more tag entries.");
    }

    for (int i = 0; i < arr.size(); i++)
    {
      const QVariant& v2 = arr[i];

      if (v2.type() != QVariant::List)
      {
        throw HootException("A compound tag must have 1 or more sub-arrays.");
      }
      QVariantList a2 = v2.toList();

      if (a2.size() == 0)
      {
        throw HootException("A compound tag entry must have 1 or more KVP entries.");
      }

      QList<KeyValuePairPtr> rule;
      for (int i = 0; i < a2.size(); i++)
      {
        QVariant v3 = a2[i];
        if (v3.type() != QVariant::String)
        {
          throw HootException("A compound tag rule must be an array of strings.");
        }

        rule.append(KeyValuePairPtr(new KeyValuePair(_asString(v3))));
      }

      tv.addCompoundRule(rule);
    }
  }
}

void JsonOsmSchemaLoader::_loadGeometries(SchemaVertex& tv, const QVariant& v) const
{
  if (v.type() != QVariant::List)
  {
    throw HootException("Expected an array for associatedWith.");
  }
  else
  {
    const QVariantList arr = v.toList();

    uint16_t g = 0;
    for (int i = 0; i < arr.size(); i++)
    {
      QString v = toString(_asString(arr[i])).toLower();
      uint16_t e = OsmGeometries::fromString(v);
      g |= e;
    }

    tv.geometries = g;
  }
}

void JsonOsmSchemaLoader::_loadSimilarTo(QString fromName, const QVariant& value, OsmSchema& s)
  const
{
  if (value.type() == QVariant::List)
  {
    QVariantList l = value.toList();
    for (int i = 0; i < l.size(); ++i)
    {
      _loadSimilarTo(fromName, l[i], s);
    }
  }
  else if (value.type() != QVariant::Map)
  {
    throw HootException("Expected an dict for similar to.");
  }
  else
  {
    QVariantMap obj = value.toMap();

    QString toName;
    double weight1 = -1;
    double weight2 = -1;
    bool oneway = false;

    for (QVariantMap::const_iterator it = obj.begin(); it != obj.end(); ++it)
    {
      if (it.key() == "name")
      {
        toName = _asString(it.value());
        if (toName.isEmpty())
        {
          throw HootException("Name must be non-null.");
        }
      }
      else if (it.key() == "weight")
      {
        if (it.value().type() == QVariant::List)
        {
          QVariantList arr = it.value().toList();

          if (arr.size() != 2)
          {
            throw HootException("Expected either an array of size 2, or a single double as the "
              "weight.");
          }
          weight1 = _asDouble(arr[0]);
          weight2 = _asDouble(arr[1]);
        }
        else
        {
          weight1 = _asDouble(it.value());
        }
      }
      else if (it.key() == "oneway")
      {
        oneway = it.value().toBool();
      }
      else
      {
        throw HootException("Only 'tag' is supported at the top level at this time. " +
                            it.key());
      }
    }

    if (weight1 >= 0 && weight2 >= 0)
    {
      if (oneway)
      {
        throw HootException("Expected only oneway and a single weight, or two weights. "
          "Got both.");
      }

      s.addSimilarTo(fromName, toName, weight1, true);
      s.addSimilarTo(toName, fromName, weight2, true);
    }
    else
    {
      s.addSimilarTo(fromName, toName, weight1, oneway);
    }
  }
}

void JsonOsmSchemaLoader::_loadTag(const QVariantMap& v, OsmSchema& s)
{
  QVariantMap copy;
  // copy all the non-comments
  for (QVariantMap::const_iterator it = v.begin(); it != v.end(); ++it)
  {
    if (it.key().startsWith('#') == false)
    {
      copy[it.key()] = it.value();
    }
  }

  SchemaVertex tv;

  tv.setType(SchemaVertex::Tag);
  if (copy.contains("name"))
  {
    tv.setNameKvp(_asString(copy.take("name")));
  }
  else
  {
    throw HootException("Expected the object to contain a name.");
  }
  if (copy.contains("dataType"))
  {
    tv.setValueTypeString(_asString(copy.take("dataType")));
  }

  // load all the values common to tag and compound.
  _loadBase(copy, s, tv);

  s.updateOrCreateVertex(tv);
}

void JsonOsmSchemaLoader::_processObject(const QVariantMap& v, OsmSchema& s)
{
  if (v.contains("import"))
  {
    LOG_TRACE("Loading: " + v["import"].toString());
    load(_baseDir.back() + v["import"].toString(), s);
  }
  else if (v.contains("objectType"))
  {
    QString ot = _asString(v["objectType"]);
    if (ot == "tag")
    {
      _loadTag(v, s);
    }
    else if (ot == "compound")
    {
      _loadCompound(v, s);
    }
    else
    {
      throw HootException("Unexpected object type: " + ot);
    }
  }
  else
  {
    throw HootException("Unexpected record type in: " + toString(v));
  }
}

}
