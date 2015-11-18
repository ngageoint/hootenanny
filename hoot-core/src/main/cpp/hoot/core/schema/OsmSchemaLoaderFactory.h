#ifndef OSMSCHEMALOADERFACTORY_H
#define OSMSCHEMALOADERFACTORY_H

#include "OsmSchemaLoader.h"

namespace hoot
{

class OsmSchemaLoaderFactory
{
public:
  OsmSchemaLoaderFactory() {};

  static OsmSchemaLoaderFactory& getInstance();

  shared_ptr<OsmSchemaLoader> createLoader(QString url);

private:
  static auto_ptr<OsmSchemaLoaderFactory> _theInstance;
};

}

#endif // OSMSCHEMALOADERFACTORY_H
