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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef JSONOSMSCHEMALOADER_H
#define JSONOSMSCHEMALOADER_H

// hoot
#include <hoot/core/schema/OsmSchemaLoader.h>

// v8
#include <v8.h>

namespace hoot
{

class JsonOsmSchemaLoader : public OsmSchemaLoader
{
public:

  static string className() { return "hoot::JsonOsmSchemaLoader"; }

  static unsigned int logWarnCount;

  JsonOsmSchemaLoader();

  virtual ~JsonOsmSchemaLoader() {}

  virtual bool isSupported(QString url) const { return url.endsWith(".json"); }

  virtual void load(QString path, OsmSchema& s);

  virtual set<QString> getDependencies() { return _deps; }

protected:

  set<QString> _deps;
  QList<QString> _baseDir;
  v8::Persistent<v8::Context> _context;

  double _asDouble(const QVariant& v) const;

  /**
   * Will return the string representation iff v is a string.
   */
  QString _asString(const QVariant& v) const;

  QStringList _asStringList(const QVariant& v) const;

  void _processObject(const QVariantMap& v, OsmSchema& s);

  void _loadBase(QVariantMap& v, OsmSchema &s, SchemaVertex& tv);

  void _loadCompound(const QVariantMap& v, OsmSchema& s);

  void _loadCompoundTags(SchemaVertex& tv, const QVariant& value);

  void _loadGeometries(SchemaVertex& tv, const QVariant& v) const;

  void _loadSimilarTo(QString fromName, const QVariant& value, OsmSchema& s) const;

  void _loadTag(const QVariantMap& v, OsmSchema& s);

};

}

#endif // JSONOSMSCHEMAFILELOADER_H
