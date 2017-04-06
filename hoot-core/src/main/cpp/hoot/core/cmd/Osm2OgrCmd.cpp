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

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/io/OgrWriter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/visitors/ProjectToGeographicVisitor.h>

namespace hoot
{

class Osm2OgrCmd : public BaseCommand
{
public:

  static string className() { return "hoot::Osm2OgrCmd"; }

  Osm2OgrCmd() { }

  virtual QString getName() const { return "osm2ogr"; }

  static QString opsKey() { return "osm2ogr.ops"; }

  virtual int runSimple(QStringList args)
  {
    QString errorMsg = QString("%1 takes either three or six parameters.").arg(getName());
    if (args.size() < 3 || args.size() > 6 )
    {
      cout << getHelp() << endl << endl;
      throw HootException(errorMsg);
    }
    else if (args.size() == 4 || args.size() == 5)
    {
      errorMsg += "  A separate cache size must be set for nodes, ways, and relations.";
      cout << getHelp() << endl << endl;
      throw HootException(errorMsg);
    }

    int a = 0;
    QString translation = args[a++];
    QString input = args[a++];
    QString output = args[a++];
    unsigned long nodeCacheSize = 0;
    unsigned long wayCacheSize = 0;
    unsigned long relationCacheSize = 0;

    if (args.size() == 6)
    {
      nodeCacheSize = args[a++].toLong();
      wayCacheSize = args[a++].toLong();
      relationCacheSize = args[a++].toLong();
    }

    boost::shared_ptr<OgrWriter> writer(new OgrWriter());
    if (nodeCacheSize > 0 && wayCacheSize > 0 && relationCacheSize > 0)
    {
      writer->setCacheCapacity(nodeCacheSize, wayCacheSize, relationCacheSize);
    }
    writer->setScriptPath(translation);
    writer->open(output);

    OsmMapReaderFactory readerFactory = OsmMapReaderFactory::getInstance();
    if (readerFactory.hasElementInputStream(input) &&
      ConfigOptions().getOsm2ogrOps().size() == 0)
    {
      boost::shared_ptr<OsmMapReader> reader = OsmMapReaderFactory::getInstance().createReader(input);
      reader->open(input);
      boost::shared_ptr<ElementInputStream> streamReader = dynamic_pointer_cast<ElementInputStream>(reader);
      boost::shared_ptr<ElementOutputStream> streamWriter = dynamic_pointer_cast<ElementOutputStream>(writer);

      boost::shared_ptr<OGRSpatialReference> projection = streamReader->getProjection();
      ProjectToGeographicVisitor visitor;
      bool notGeographic = !projection->IsGeographic();

      if (notGeographic)
        visitor.initialize(projection);

      while (streamReader->hasMoreElements())
      {
        ElementPtr element = streamReader->readNextElement();
        if (notGeographic)
          visitor.visit(element);
        streamWriter->writeElement(element);
      }
    }
    else
    {
      boost::shared_ptr<OsmMap> map(new OsmMap());

      loadMap(map, input, true);

      // Apply any user specified operations.
      NamedOp(ConfigOptions().getOsm2ogrOps()).apply(map);

      MapProjector::projectToWgs84(map);

      writer->write(map);
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, Osm2OgrCmd)

}

