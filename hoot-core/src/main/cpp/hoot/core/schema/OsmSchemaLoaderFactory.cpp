
#include "OsmSchemaLoaderFactory.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/schema/OsmSchemaLoader.h>

namespace hoot
{

auto_ptr<OsmSchemaLoaderFactory> OsmSchemaLoaderFactory::_theInstance;

OsmSchemaLoaderFactory& OsmSchemaLoaderFactory::getInstance()
{
  if (_theInstance.get() == 0)
  {
    _theInstance.reset(new OsmSchemaLoaderFactory());
  }

  return *_theInstance;
}

shared_ptr<OsmSchemaLoader> OsmSchemaLoaderFactory::createLoader(QString url)
{
  vector<std::string> names = Factory::getInstance().getObjectNamesByBase(
    OsmSchemaLoader::className());

  for (size_t i = 0; i < names.size(); ++i)
  {
    shared_ptr<OsmSchemaLoader> l(Factory::getInstance().constructObject<OsmSchemaLoader>(
      names[i]));

    if (l->isSupported(url))
    {
      return l;
    }
  }

  LOG_DEBUG("Available loaders: " << names);
  throw HootException("Unable to load specified URL (wrong extension?): " + url);
}

}
