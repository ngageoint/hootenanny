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

#include "JsonSchemaLoader.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// json-spirit
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <json_spirit_reader.h>
#include <json_spirit_reader_template.h>
#pragma GCC diagnostic pop
using namespace json_spirit;

// Qt
#include <QDir>

// Standard
#include <fstream>
#include <iostream>
using namespace std;

namespace hoot
{

class JsonSchemaLoaderData
{
public:
  JsonSchemaLoaderData(OsmSchema& schema) : _schema(schema)
  {
  }

  set<QString> getDependencies()
  {
    return _dependencies;
  }

  void load(QString path)
  {
    ifstream is(path.toAscii().data());
    Value value;
    _baseDir.push_back(QDir(path + "/..").absolutePath() + "/");
    _dependencies.insert(QDir().absoluteFilePath(path));

    if (is.is_open() == false)
    {
      throw HootException("Error opening " + path + " for reading.");
    }

    try
    {
      read_stream_or_throw(is, value);
      _loadTags(value);

      size_t current = is.tellg();
      is.seekg (0, ios::end);
      size_t end = is.tellg();

      // this is a hack to get around poor parsing. It'd be better to use v8 parser.
      if (current != end)
      {
        throw HootException(QString("Error reading JSON file (%1). Did not read to the end of the "
          "file. Do you have an extra brace? Extra white space? Parsing stopped at position: %2.").
          arg(path).arg(current));
      }

      // update implied values via inheritance.
      _schema.update();
      _baseDir.pop_back();
    }
    catch (Error_position& ep)
    {
      _baseDir.pop_back();
      QString reason = QString("Reason: %1 %2 line: %3 col: %4").
          arg(QString::fromStdString(ep.reason_)).arg(path).arg(ep.line_).arg(ep.column_);
      throw HootException("Error parsing JSON. " + reason);
    }
  }

  static QString toString(const Value& value)
  {
    if (value.type() == str_type)
    {
      return QString::fromStdString(value.get_str());
    }
    if (value.type() == int_type)
    {
      return QString("%1").arg(value.get_int());
    }
    if (value.type() == real_type)
    {
      return QString("%1").arg(value.get_real());
    }
    else
    {
      throw HootException("While parsing JSON config file. Unsupported type when converting to "
                          "string.");
    }
  }

private:
  OsmSchema& _schema;
  QList<QString> _baseDir;
  set<QString> _dependencies;

  void _loadATag(const Value& value)
  {
    if (value.type() != obj_type)
    {
      throw HootException("Expected tag to be an object.");
    }
    else
    {
      const Object& obj = value.get_obj();

      TagVertex tv;

      for (size_t i = 0; i < obj.size(); i++)
      {
        if (i == 0)
        {
          if (obj[i].name_ == "name")
          {
            tv.name = toString(obj[i].value_);
            int equalsPos = tv.name.indexOf('=');
            if (equalsPos == 0)
            {
              throw HootException("The name cannot start with an equals sign. " + tv.name);
            }
            else if (equalsPos > 0)
            {
              tv.key = tv.name.left(equalsPos);
              tv.value = tv.name.mid(equalsPos + 1);
            }
            else
            {
              tv.key = tv.name;
            }
          }
          else
          {
            HootException("Name must be specified as the first tag entry in JSON.");
          }
        }
        else if (toString(obj[i].name_).startsWith("#"))
        {
          // pass
        }
        else if (obj[i].name_ == "associatedWith")
        {
          _loadAssociatedWith(tv, obj[i].value_);
        }
        else if (obj[i].name_ == "isA")
        {
          _schema.addIsA(tv.name, toString(obj[i].value_));
        }
        else if (obj[i].name_ == "similarTo")
        {
          _loadSimilarTo(tv.name, obj[i].value_);
        }
        else if (obj[i].name_ == "description")
        {
          tv.description = toString(obj[i].value_);
        }
        else if (obj[i].name_ == "influence")
        {
          tv.influence = obj[i].value_.get_value<double>();
        }
        else if (obj[i].name_ == "childWeight")
        {
          tv.childWeight = obj[i].value_.get_value<double>();
        }
        else if (obj[i].name_ == "mismatchScore")
        {
          tv.mismatchScore = obj[i].value_.get_value<double>();
        }
        else if (obj[i].name_ == "aliases")
        {
          tv.aliases = _toStringList(obj[i].value_);
        }
        else if (obj[i].name_ == "categories")
        {
          tv.categories = _toStringList(obj[i].value_);
        }
        else if (obj[i].name_ == "type")
        {
          QString v = toString(obj[i].value_).toLower();
          if (v == "enumeration")
          {
            tv.valueType = Enumeration;
          }
          else if (v == "text")
          {
            tv.valueType = Text;
          }
          else if (v == "int")
          {
            tv.valueType = Int;
          }
          else
          {
            throw HootException("Unexpected type tag: " + v);
          }
        }
        else if (obj[i].name_ == "geometries")
        {
          _loadGeometries(tv, obj[i].value_);
        }
        else
        {
          throw HootException(QString("An unexpected key was found in a tag (%1): %2").
                              arg(tv.name).arg(QString::fromStdString(obj[i].name_)));
        }
      }

      _schema.updateOrCreateVertex(tv);
    }
  }

  void _loadAssociatedWith(TagVertex& tv, const Value& value)
  {
    if (value.type() != array_type)
    {
      throw HootException("Expected an array for associatedWith.");
    }
    else
    {
      const Array& arr = value.get_array();

      for (size_t i = 0; i < arr.size(); i++)
      {
        _schema.addAssociatedWith(tv.name, toString(arr[i]));
      }
    }
  }

  void _loadGeometries(TagVertex& tv, const Value& value)
  {
    if (value.type() != array_type)
    {
      throw HootException("Expected an array for geometries.");
    }
    else
    {
      const Array& arr = value.get_array();

      uint16_t g = 0;
      for (size_t i = 0; i < arr.size(); i++)
      {
        QString v = toString(arr[i]).toLower();
        uint16_t e = OsmGeometries::fromString(v);
        g |= e;
      }

      tv.geometries = g;
    }
  }

  void _loadSimilarTo(QString fromName, const Value& value)
  {
    if (value.type() != obj_type)
    {
      throw HootException("Expected an object for similar to.");
    }
    else
    {
      const Object& obj = value.get_obj();

      QString toName;
      double weight = -1;

      for (size_t i = 0; i < obj.size(); i++)
      {
        if (obj[i].name_ == "name")
        {
          toName = toString(obj[i].value_);
          if (toName.isEmpty())
          {
            throw HootException("Name must be non-null.");
          }
        }
        else if (obj[i].name_ == "weight")
        {
          weight = obj[i].value_.get_value<double>();
        }
        else
        {
          throw HootException("Only 'tag' is supported at the top level at this time. " +
                              obj[i].name_);
        }
      }
      _schema.addSimilarTo(fromName, toName, weight);
    }
  }

  void _loadTags(const Value& value)
  {
    if (value.type() != obj_type)
    {
      throw HootException("Expected a list of tags at the top level.");
    }
    else
    {
      const Object& obj = value.get_obj();

      for (size_t i = 0; i < obj.size(); i++)
      {
        if (QString::fromStdString(obj[i].name_).startsWith("#"))
        {
          // comment
          continue;
        }
        else if (obj[i].name_ == "tag")
        {
          _loadATag(obj[i].value_);
        }
        else if (obj[i].name_ == "import")
        {
          load(_baseDir.back() + toString(obj[i].value_));
        }
        else
        {
          throw HootException("The specified top level tag isn't supported: " +
            QString::fromStdString(obj[i].name_));
        }
      }
    }
  }

  bool _toBool(const Value& value)
  {
    if (value.type() != bool_type)
    {
      throw HootException("Expected a bool type.");
    }
    else
    {
      return value.get_bool();
    }
  }

  QStringList _toStringList(const Value& value)
  {
    QStringList result;
    if (value.type() != array_type)
    {
      throw HootException("Expected an array for string list.");
    }
    else
    {
      const Array& arr = value.get_array();

      for (size_t i = 0; i < arr.size(); i++)
      {
        result.append(toString(arr[i]));
      }
    }

    return result;
  }

};

JsonSchemaLoader::JsonSchemaLoader(OsmSchema& schema)
{
  _d = new JsonSchemaLoaderData(schema);
}

JsonSchemaLoader::~JsonSchemaLoader()
{
  delete _d;
}

set<QString> JsonSchemaLoader::getDependencies()
{
  return _d->getDependencies();
}

void JsonSchemaLoader::load(QString fn)
{
  _d->load(fn);
}

void JsonSchemaLoader::load(OsmSchema& schema, QString path)
{
  JsonSchemaLoader s(schema);
  s.load(path);
}

}
