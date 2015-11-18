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
