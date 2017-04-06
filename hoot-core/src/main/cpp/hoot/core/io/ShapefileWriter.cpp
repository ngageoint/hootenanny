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

#include "ShapefileWriter.h"

// hoot
#include <hoot/core/util/Factory.h>

// GDAL
#include <ogr_geometry.h>

// GEOS
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
using namespace geos::geom;

// Hoot
#include <hoot/core/io/OgrOptions.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>

// Qt
#include <QFileInfo>
#include <QHash>
#include <QMap>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapWriter, ShapefileWriter)

class ColumnVisitor : public ElementConstOsmMapVisitor
{
public:
  ColumnVisitor(ElementType type) : _type(type) {}

  virtual void visit(const boost::shared_ptr<const Element>& e)
  {
    if (e->getElementType() == _type || _type == ElementType::Unknown)
    {
      const Tags& tags = e->getTags();

      for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
      {
        _keys[it.key()]++;
      }
    }
  }

  QMultiMap<int, QString> getBackwardKeys()
  {
    QMultiMap<int, QString> result;
    for (QHash<QString, int>::const_iterator it = _keys.begin(); it != _keys.end(); ++it)
    {
      result.insertMulti(it.value(), it.key());
    }

    return result;
  }

private:
  ElementType _type;
  QHash<QString, int> _keys;
};


QStringList ShapefileWriter::getColumns(ConstOsmMapPtr map, ElementType type) const
{
  if (_columns.size() != 0)
  {
    return _columns;
  }
  else
  {
    QStringList result;
    ColumnVisitor v(type);
    map->visitRo(v);
    QMultiMap<int, QString> backwardKeys = v.getBackwardKeys();

    QMapIterator<int, QString> i(backwardKeys);
    i.toBack();
    while (i.hasPrevious() && result.size() < 20)
    {
      i.previous();
      result << i.value();
    }

    return result;
  }
}

void ShapefileWriter::open(QString url)
{
  if (QDir(url).exists() == false)
  {
    if (QDir(".").mkpath(url) == false)
    {
      throw HootException("Error creating directory for writing.");
    }
    _outputDir = QDir(url);
  }
}

void ShapefileWriter::_removeShapefile(QString path)
{
  QFileInfo fi(path);

  QString base = fi.absolutePath() + "/" + fi.baseName();

  QFile::remove(base + ".shp");
  QFile::remove(base + ".SHP");
  QFile::remove(base + ".dbf");
  QFile::remove(base + ".DBF");
  QFile::remove(base + ".shx");
  QFile::remove(base + ".SHX");
  QFile::remove(base + ".prj");
  QFile::remove(base + ".PRJ");
}

void ShapefileWriter::write(boost::shared_ptr<const OsmMap> map, QString path)
{
  if (path.toLower().endsWith(".shp"))
  {
    path.remove(path.size() - 4, path.size());
  }
  writeLines(map, path + "Lines.shp");
  writePoints(map, path + "Points.shp");
  writePolygons(map, path + "Polygons.shp");
}

void ShapefileWriter::write(boost::shared_ptr<const OsmMap> map)
{
  writeLines(map, _outputDir.absoluteFilePath("Lines.shp"));
  writePoints(map, _outputDir.absoluteFilePath("Points.shp"));
  writePolygons(map, _outputDir.absoluteFilePath("Polygons.shp"));
}

void ShapefileWriter::writeLines(boost::shared_ptr<const OsmMap> map, const QString& path)
{
  GDALAllRegister();

  _removeShapefile(path);

  const char *pszDriverName = "ESRI Shapefile";
  GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
  if( poDriver == NULL )
  {
    throw HootException(QString("%1 driver not available.").arg(pszDriverName));
  }

  GDALDataset* poDS = poDriver->Create(path.toAscii(), 0, 0, 0, GDT_Unknown, NULL);
  if( poDS == NULL )
  {
    throw HootException(QString("Data source creation failed. %1").arg(path));
  }

  OGRLayer *poLayer;

  OgrOptions options;
  options["ENCODING"] = "UTF-8";

  QString layerName;
  layerName = QFileInfo(path).baseName();
  poLayer = poDS->CreateLayer(layerName.toAscii(),
                              map->getProjection().get(), wkbLineString,
                              options.getCrypticOptions());
  if( poLayer == NULL )
  {
    throw HootException(QString("Layer creation failed. %1").arg(path));
  }

  QStringList shpColumns;
  QStringList columns = getColumns(map, ElementType::Way);

  for (int i = 0; i < columns.size(); i++)
  {
    OGRFieldDefn oField(columns[i].toAscii(), OFTString);

    oField.SetWidth(64);

    if( poLayer->CreateField( &oField ) != OGRERR_NONE )
    {
      throw HootException(QString("Error creating field (%1).").arg(columns[i]));
    }

    shpColumns.append(poLayer->GetLayerDefn()->GetFieldDefn(i)->GetNameRef());
  }

  if (_includeInfo)
  {
    OGRFieldDefn oField("error_circ", OFTReal);
    if( poLayer->CreateField( &oField ) != OGRERR_NONE )
    {
      throw HootException(QString("Error creating field (%1).").arg(MetadataTags::ErrorCircular()));
    }
    _circularErrorIndex = poLayer->GetLayerDefn()->GetFieldCount() - 1;
  }

  const WayMap& ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); it++)
  {
    boost::shared_ptr<Way> way = it->second;

    if (OsmSchema::getInstance().isArea(way) == false)
    {
      OGRFeature* poFeature = OGRFeature::CreateFeature( poLayer->GetLayerDefn() );
      // set all the column values.
      for (int i = 0; i < columns.size(); i++)
      {
        if (way->getTags().contains(columns[i]))
        {
          QByteArray c = shpColumns[i].toAscii();
          QByteArray v = way->getTags()[columns[i]].toUtf8();
          poFeature->SetField(c.constData(), v.constData());
        }
      }

      if (_includeInfo)
      {
        poFeature->SetField(_circularErrorIndex, way->getCircularError());
      }

      // convert the geometry.
      std::string wkt = ElementConverter(map).convertToLineString(way)->toString();
      char* t = (char*)wkt.data();
      OGRGeometry* geom;
      if (OGRGeometryFactory::createFromWkt(&t, poLayer->GetSpatialRef(), &geom) != OGRERR_NONE)
      {
        throw HootException(QString("Error parsing WKT (%1)").arg(QString::fromStdString(wkt)));
      }

      if (poFeature->SetGeometryDirectly(geom) != OGRERR_NONE)
      {
        throw HootException(QString("Error setting geometry"));
      }

      if (poLayer->CreateFeature(poFeature) != OGRERR_NONE)
      {
        throw HootException(QString("Error creating feature"));
      }

      OGRFeature::DestroyFeature(poFeature);
    }
  }

  GDALClose(poDS);
}

void ShapefileWriter::writePoints(boost::shared_ptr<const OsmMap> map, const QString& path)
{
  GDALAllRegister();

  _removeShapefile(path);

  const char *pszDriverName = "ESRI Shapefile";
  GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
  if( poDriver == NULL )
  {
    throw HootException(QString("%1 driver not available.").arg(pszDriverName));
  }

  GDALDataset* poDS = poDriver->Create(path.toAscii(), 0, 0, 0, GDT_Unknown, NULL);
  if( poDS == NULL )
  {
    throw HootException(QString("Data source creation failed. %1").arg(path));
  }

  OgrOptions options;
  options["ENCODING"] = "UTF-8";

  OGRLayer *poLayer;

  QString layerName;
  layerName = QFileInfo(path).baseName();
  poLayer = poDS->CreateLayer(layerName.toAscii(),
                              map->getProjection().get(), wkbPoint, options.getCrypticOptions());
  if( poLayer == NULL )
  {
    throw HootException(QString("Layer creation failed. %1").arg(path));
  }

  QStringList shpColumns;
  QStringList columns = getColumns(map, ElementType::Node);

  for (int i = 0; i < columns.size(); i++)
  {
    OGRFieldDefn oField(columns[i].toAscii(), OFTString);

    oField.SetWidth(64);

    if( poLayer->CreateField( &oField ) != OGRERR_NONE )
    {
      throw HootException(QString("Error creating field (%1).").arg(columns[i]));
    }

    shpColumns.append(poLayer->GetLayerDefn()->GetFieldDefn(i)->GetNameRef());
  }

  if (_includeInfo)
  {
    OGRFieldDefn oField("error_circ", OFTReal);
    if( poLayer->CreateField( &oField ) != OGRERR_NONE )
    {
      throw HootException(QString("Error creating field (%1).").arg(MetadataTags::ErrorCircular()));
    }
    _circularErrorIndex = poLayer->GetLayerDefn()->GetFieldCount() - 1;
  }

  const NodeMap& nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    const boost::shared_ptr<Node>& node = it->second;

    if (node->getTags().getNonDebugCount() > 0)
    {
      OGRFeature* poFeature = OGRFeature::CreateFeature( poLayer->GetLayerDefn() );
      // set all the column values.
      for (int i = 0; i < columns.size(); i++)
      {
        if (node->getTags().contains(columns[i]))
        {
          QByteArray c = shpColumns[i].toAscii();
          QByteArray v = node->getTags()[columns[i]].toUtf8();
          poFeature->SetField(c.constData(), v.constData());
        }
      }

      if (_includeInfo)
      {
        poFeature->SetField(_circularErrorIndex, node->getCircularError());
      }

      // convert the geometry.
      OGRGeometry* geom = new OGRPoint(node->getX(), node->getY());

      if (poFeature->SetGeometryDirectly(geom) != OGRERR_NONE)
      {
        throw HootException(QString("Error setting geometry"));
      }

      if (poLayer->CreateFeature(poFeature) != OGRERR_NONE)
      {
        throw HootException(QString("Error creating feature"));
      }

      OGRFeature::DestroyFeature(poFeature);
    }
  }

  GDALClose(poDS);
}

void ShapefileWriter::writePolygons(boost::shared_ptr<const OsmMap> map, const QString& path)
{
  GDALAllRegister();

  _removeShapefile(path);

  const char *pszDriverName = "ESRI Shapefile";
  GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
  if( poDriver == NULL )
  {
    throw HootException(QString("%1 driver not available.").arg(pszDriverName));
  }

  GDALDataset* poDS = poDriver->Create(path.toAscii(), 0, 0, 0, GDT_Unknown, NULL);
  if( poDS == NULL )
  {
    throw HootException(QString("Data source creation failed. %1").arg(path));
  }

  OgrOptions options;
  options["ENCODING"] = "UTF-8";

  OGRLayer *poLayer;

  QString layerName;
  layerName = QFileInfo(path).baseName();
  poLayer = poDS->CreateLayer(layerName.toAscii(),
                              map->getProjection().get(), wkbMultiPolygon,
                              options.getCrypticOptions());
  if( poLayer == NULL )
  {
    throw HootException(QString("Layer creation failed. %1").arg(path));
  }

  QStringList shpColumns;
  QStringList columns = getColumns(map, ElementType::Unknown);

  for (int i = 0; i < columns.size(); i++)
  {
    OGRFieldDefn oField(columns[i].toAscii(), OFTString);

    oField.SetWidth(64);

    if( poLayer->CreateField( &oField ) != OGRERR_NONE )
    {
      throw HootException(QString("Error creating field (%1).").arg(columns[i]));
    }

    shpColumns.append(poLayer->GetLayerDefn()->GetFieldDefn(i)->GetNameRef());
  }

  if (_includeInfo)
  {
    OGRFieldDefn oField("error_circ", OFTReal);
    if( poLayer->CreateField( &oField ) != OGRERR_NONE )
    {
      throw HootException(QString("Error creating field (%1).").arg(MetadataTags::ErrorCircular()));
    }
    _circularErrorIndex = poLayer->GetLayerDefn()->GetFieldCount() - 1;
  }

  const WayMap& ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); it++)
  {
    boost::shared_ptr<Way> way = it->second;

    if (OsmSchema::getInstance().isArea(way))
    {
      _writeWayPolygon(map, way, poLayer, columns, shpColumns);
    }
  }

  const RelationMap& relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); it++)
  {
    boost::shared_ptr<Relation> relation = it->second;

    if (relation->isMultiPolygon())
    {
      _writeRelationPolygon(map, relation, poLayer, columns, shpColumns);
    }
  }

  GDALClose(poDS);
}

void ShapefileWriter::_writeRelationPolygon(const ConstOsmMapPtr& map,
  const boost::shared_ptr<Relation> &relation, OGRLayer *poLayer, const QStringList& columns,
  const QStringList &shpColumns)
{
  OGRFeature* poFeature = OGRFeature::CreateFeature( poLayer->GetLayerDefn() );
  // set all the column values.
  for (int i = 0; i < columns.size(); i++)
  {
    if (relation->getTags().contains(columns[i]))
    {
      QByteArray c = shpColumns[i].toAscii();
      QByteArray v = relation->getTags()[columns[i]].toUtf8();
      poFeature->SetField(c.constData(), v.constData());
    }
  }

  if (_includeInfo)
  {
    poFeature->SetField(_circularErrorIndex, relation->getCircularError());
  }

  // convert the geometry.
  const boost::shared_ptr<const Relation>& r = relation;
  std::string wkt = ElementConverter(map).convertToGeometry(r)->toString();
  char* t = (char*)wkt.data();
  OGRGeometry* geom;
  if (OGRGeometryFactory::createFromWkt(&t, poLayer->GetSpatialRef(), &geom) != OGRERR_NONE)
  {
    throw HootException(QString("Error parsing WKT (%1)").arg(QString::fromStdString(wkt)));
  }

  if (poFeature->SetGeometryDirectly(geom) != OGRERR_NONE)
  {
    throw HootException(QString("Error setting geometry"));
  }

  if (poLayer->CreateFeature(poFeature) != OGRERR_NONE)
  {
    throw HootException(QString("Error creating feature"));
  }

  OGRFeature::DestroyFeature(poFeature);
}

void ShapefileWriter::_writeWayPolygon(const ConstOsmMapPtr &map, const boost::shared_ptr<Way> &way,
  OGRLayer *poLayer, const QStringList& columns, const QStringList &shpColumns)
{
  OGRFeature* poFeature = OGRFeature::CreateFeature( poLayer->GetLayerDefn() );
  // set all the column values.
  for (int i = 0; i < columns.size(); i++)
  {
    if (way->getTags().contains(columns[i]))
    {
      QByteArray c = shpColumns[i].toAscii();
      QByteArray v = way->getTags()[columns[i]].toUtf8();
      poFeature->SetField(c.constData(), v.constData());
    }
  }

  if (_includeInfo)
  {
    poFeature->SetField(_circularErrorIndex, way->getCircularError());
  }

  // convert the geometry.
  boost::shared_ptr<Geometry> p = ElementConverter(map).convertToGeometry(way);
  if (p->getGeometryTypeId() != GEOS_POLYGON)
  {
    throw InternalErrorException("Expected a polygon geometry, but got a: " +
                                 toString(p->getGeometryType()));
  }
  std::string wkt = p->toString();
  char* t = (char*)wkt.data();
  OGRGeometry* geom;
  if (OGRGeometryFactory::createFromWkt(&t, poLayer->GetSpatialRef(), &geom) != OGRERR_NONE)
  {
    throw HootException(QString("Error parsing WKT (%1)").arg(QString::fromStdString(wkt)));
  }

  if (poFeature->SetGeometryDirectly(geom) != OGRERR_NONE)
  {
    throw HootException(QString("Error setting geometry"));
  }

  if (poLayer->CreateFeature(poFeature) != OGRERR_NONE)
  {
    throw HootException(QString("Error creating feature"));
  }

  OGRFeature::DestroyFeature(poFeature);
}

}
