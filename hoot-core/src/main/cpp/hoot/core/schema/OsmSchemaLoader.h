#ifndef OSM_SCHEMAFILELOADER_H
#define OSM_SCHEMAFILELOADER_H

#include "OsmSchema.h"

namespace hoot
{

class OsmSchemaLoader
{
public:
  static string className() { return "hoot::OsmSchemaLoader"; }

  OsmSchemaLoader();

  virtual ~OsmSchemaLoader();

  /**
   * Returns true if the specified path is likely supported. This should be a quick operation like
   * verifying a URL, or looking at a file extension.
   */
  virtual bool isSupported(QString path) const = 0;

  /**
   * Loads the specified path (and any referenced files) into the schema s. s may already contain
   * some content in which case the new content will be added to s, or overwrite values in s as
   * appropriate.
   */
  virtual void load(QString path, OsmSchema& s) = 0;

  /**
   * Returns a list of the absolute paths of files that are loaded into this schema. If "import"
   * statements are used in the json file then all imported files are included in the dependency
   * list.
   */
  virtual set<QString> getDependencies() = 0;
};

}

#endif // OSM_SCHEMAFILELOADER_H
