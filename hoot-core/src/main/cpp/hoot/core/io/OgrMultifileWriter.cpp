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
 * @copyright Copyright (C) 2023 Maxar (http://www.maxar.com/)
 */

#include "OgrMultifileWriter.h"

// GDAL
#include <gdal_frmts.h>
#include <gdal_priv.h>
#include <ogr_geometry.h>

// GEOS
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>

// Hoot
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>

using namespace geos::geom;

namespace hoot
{

class ColumnVisitor : public ElementConstOsmMapVisitor
{
public:

  ColumnVisitor(ElementType type) : _type(type) { }
  ~ColumnVisitor() override = default;

  void visit(const std::shared_ptr<const Element>& e) override
  {
    if (e->getElementType() == _type || _type == ElementType::Unknown)
    {
      const Tags& tags = e->getTags();
      for (auto it = tags.begin(); it != tags.end(); ++it)
        _keys[it.key()]++;
    }
  }

  QMultiMap<int, QString> getBackwardKeys()
  {
    QMultiMap<int, QString> result;
    for (auto it = _keys.begin(); it != _keys.end(); ++it)
      result.insertMulti(it.value(), it.key());
    return result;
  }

  QString getDescription() const override { return ""; }
  QString getName() const override { return ""; }
  QString getClassName() const override { return ""; }

private:

  ElementType _type;
  QHash<QString, int> _keys;
};

OgrMultifileWriter::OgrMultifileWriter()
  : _includeCircularError(ConfigOptions().getWriterIncludeCircularErrorTags()),
    _circularErrorIndex(-1)
{
}

QStringList OgrMultifileWriter::getColumns(ConstOsmMapPtr map, ElementType type) const
{
  if (!_baseColumns.empty())
    return _baseColumns;
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

void OgrMultifileWriter::open(const QString& url)
{
  if (!QDir(url).exists() && !FileUtils::makeDir(url))
      throw HootException("Error creating directory for writing: " + url);
  _outputDir = QDir(url);
}

void OgrMultifileWriter::setConfiguration(const Settings& conf)
{
  _includeCircularError = ConfigOptions(conf).getWriterIncludeCircularErrorTags();
}

void OgrMultifileWriter::write(const ConstOsmMapPtr& map)
{
  QString ext = _getFileExtension();
  writeLines(map, _outputDir.absoluteFilePath("Lines" + ext));
  writePoints(map, _outputDir.absoluteFilePath("Points" + ext));
  writePolygons(map, _outputDir.absoluteFilePath("Polygons" + ext));
}

void OgrMultifileWriter::write(const ConstOsmMapPtr& map, const QString& path)
{
  QString ext = _getFileExtension();
   QString tempPath = path;
   if (tempPath.endsWith(ext, Qt::CaseInsensitive))
     tempPath.remove(tempPath.size() - 4, tempPath.size());
   writeLines(map, tempPath + "Lines" + ext);
   writePoints(map, tempPath + "Points" + ext);
   writePolygons(map, tempPath + "Polygons" + ext);
}

void OgrMultifileWriter::_setupDataset(const ConstOsmMapPtr& map, const QString& path, OGRwkbGeometryType geometry_type, ElementType element_type)
{
  GDALAllRegister();

  _removeMultifile(path);

  const char* pszDriverName = _getDriverName();
  _driver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
  if (_driver == nullptr)
    throw HootException(QString("%1 driver not available.").arg(pszDriverName));

  _dataset = _driver->Create(path.toLatin1(), 0, 0, 0, GDT_Unknown, nullptr);
  if (_dataset == nullptr)
    throw HootException(QString("Data source creation failed. %1").arg(path));

  OgrOptions options = _getOptions();
  QString layerName;
  layerName = QFileInfo(path).baseName();
  _layer = _dataset->CreateLayer(layerName.toLatin1(), map->getProjection().get(), geometry_type, options.getCrypticOptions());
  if (_layer == nullptr)
    throw HootException(QString("Layer creation failed. %1").arg(path));

  _usedColumns = getColumns(map, element_type);

  for (int i = 0; i < _usedColumns.size(); i++)
  {
    OGRFieldDefn oField(_usedColumns[i].toLatin1(), OFTString);
    oField.SetWidth(64);
    if (_layer->CreateField(&oField) != OGRERR_NONE)
      throw HootException(QString("Error creating field (%1).").arg(_usedColumns[i]));

    _ogrColumns.append(_layer->GetLayerDefn()->GetFieldDefn(i)->GetNameRef());
  }

  if (_includeCircularError)
  {
    OGRFieldDefn oField("error_circ", OFTReal);
    if (_layer->CreateField(&oField) != OGRERR_NONE)
      throw HootException(QString("Error creating field (%1).").arg(MetadataTags::ErrorCircular()));
    _circularErrorIndex = _layer->GetLayerDefn()->GetFieldCount() - 1;
  }
}

OGRFeature* OgrMultifileWriter::_createFeature(const Tags& tags, double circular_error) const
{
  OGRFeature* poFeature = OGRFeature::CreateFeature(_layer->GetLayerDefn());
  // set all the column values.
  for (int i = 0; i < _usedColumns.size(); i++)
  {
    if (tags.contains(_usedColumns[i]))
    {
      QByteArray c = _ogrColumns[i].toLatin1();
      QByteArray v = tags[_usedColumns[i]].toUtf8();
      poFeature->SetField(c.constData(), v.constData());
    }
  }
  //  Include circular error if requested
  if (_includeCircularError)
    poFeature->SetField(_circularErrorIndex, circular_error);
  //  Return the result
  return poFeature;
}

void OgrMultifileWriter::_cleanupDataset()
{
  GDALClose(_dataset);
  _driver = nullptr;
  _dataset = nullptr;
  _layer = nullptr;
  _usedColumns.clear();
  _ogrColumns.clear();
}

void OgrMultifileWriter::writeLines(const ConstOsmMapPtr& map, const QString& path)
{
  _setupDataset(map, path, wkbLineString, ElementType::Way);

  const WayMap& ways = map->getWays();
  for (auto it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;

    if (AreaCriterion().isSatisfied(way) == false)
    {
      OGRFeature* poFeature = _createFeature(way->getTags(), way->getCircularError());

      // convert the geometry.
      std::string wkt = ElementToGeometryConverter(map).convertToLineString(way)->toString();
      const char* t = wkt.data();
      OGRGeometry* geom;

      if (OGRGeometryFactory::createFromWkt(&t, _layer->GetSpatialRef(), &geom) != OGRERR_NONE)
      {
        LOG_ERROR("Error parsing WKT (" << wkt << ")");
        OGRFeature::DestroyFeature(poFeature);
        return;
      }

      if (poFeature->SetGeometryDirectly(geom) != OGRERR_NONE)
      {
        LOG_ERROR("Error setting geometry " << way->getElementId());
        OGRFeature::DestroyFeature(poFeature);
        return;
      }

      if (_layer->CreateFeature(poFeature) != OGRERR_NONE)
      {
        LOG_ERROR("Error creating feature " << way->getElementId());
      }

      OGRFeature::DestroyFeature(poFeature);
    }
  }
  _cleanupDataset();
}

void OgrMultifileWriter::writePoints(const ConstOsmMapPtr& map, const QString& path)
{
  _setupDataset(map, path, wkbPoint, ElementType::Node);

  const NodeMap& nodes = map->getNodes();
  for (auto it = nodes.begin(); it != nodes.end(); ++it)
  {
    const NodePtr& node = it->second;

    if (node->getTags().getNonDebugCount() > 0)
    {
      OGRFeature* poFeature = _createFeature(node->getTags(), node->getCircularError());

      // convert the geometry.
      std::shared_ptr<OGRGeometry> geom = std::make_shared<OGRPoint>(node->getX(), node->getY());

      if (poFeature->SetGeometry(geom.get()) != OGRERR_NONE)
      {
        LOG_ERROR("Error setting geometry: " << node->getElementId());
        OGRFeature::DestroyFeature(poFeature);
        return;
      }

      if (_layer->CreateFeature(poFeature) != OGRERR_NONE)
      {
        LOG_ERROR("Error creating feature: " << node->getElementId());
      }

      OGRFeature::DestroyFeature(poFeature);
    }
  }

  _cleanupDataset();
}

void OgrMultifileWriter::writePolygons(const ConstOsmMapPtr& map, const QString& path)
{
  _setupDataset(map, path, wkbMultiPolygon, ElementType::Unknown);

  const WayMap& ways = map->getWays();
  for (auto it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;

    if (AreaCriterion().isSatisfied(way))
      _writeWayPolygon(map, way);
  }

  const RelationMap& relations = map->getRelations();
  for (auto it = relations.begin(); it != relations.end(); ++it)
  {
    RelationPtr relation = it->second;

    if (relation->isMultiPolygon())
      _writeRelationPolygon(map, relation);
  }

  _cleanupDataset();
}

void OgrMultifileWriter::_writeRelationPolygon(const ConstOsmMapPtr& map, const RelationPtr& relation) const
{
  OGRFeature* poFeature = _createFeature(relation->getTags(), relation->getCircularError());

  // convert the geometry.
  const ConstRelationPtr& r = relation;
  std::shared_ptr<geos::geom::Geometry> geometry = ElementToGeometryConverter(map).convertToGeometry(r);
  if (!geometry.get() || geometry->isEmpty())
  {
    LOG_WARN("Error converting geometry: " << r->getElementId());
    OGRFeature::DestroyFeature(poFeature);
    return;
  }

  //  Convert the polygon to a multipolygon if needed (FlagGeobuf cannot combine polygons and multipolygons in the same layer)
  geometry = _polyToMultipoly(geometry);

  std::string wkt = geometry->toString();
  const char* t = wkt.data();
  OGRGeometry* geom;

  if (OGRGeometryFactory::createFromWkt(&t, _layer->GetSpatialRef(), &geom) != OGRERR_NONE)
  {
    LOG_WARN("Error parsing WKT (" << wkt << ")");
    OGRFeature::DestroyFeature(poFeature);
    return;
  }

  if (poFeature->SetGeometryDirectly(geom) != OGRERR_NONE)
  {
    LOG_ERROR("Error setting geometry " << r->getElementId());
    OGRFeature::DestroyFeature(poFeature);
    return;
  }

  if (_layer->CreateFeature(poFeature) != OGRERR_NONE)
  {
    LOG_ERROR("Error creating feature " << r->getElementId());
  }

  OGRFeature::DestroyFeature(poFeature);
}

void OgrMultifileWriter::_writeWayPolygon(const ConstOsmMapPtr& map, const WayPtr& way) const
{
  OGRFeature* poFeature = _createFeature(way->getTags(), way->getCircularError());

  // convert the geometry.
  std::shared_ptr<Geometry> p = ElementToGeometryConverter(map).convertToGeometry(way);
  if (p->getGeometryTypeId() != GEOS_POLYGON)
  {
    LOG_WARN("Geometry mismatch, expected polygon but created " << toString(p->getGeometryType()));
    OGRFeature::DestroyFeature(poFeature);
    return;
  }

  //  Convert the polygon to a multipolygon if needed (FlagGeobuf cannot combine polygons and multipolygons in the same layer)
  p = _polyToMultipoly(p);

  std::string wkt = p->toString();
  const char* t = wkt.data();
  OGRGeometry* geom;
  if (OGRGeometryFactory::createFromWkt(&t, _layer->GetSpatialRef(), &geom) != OGRERR_NONE)
  {
    LOG_WARN("Error parsing WKT (" << wkt << ")");
    OGRFeature::DestroyFeature(poFeature);
    return;
  }

  if (poFeature->SetGeometryDirectly(geom) != OGRERR_NONE)
  {
    LOG_WARN("Error setting geometry " << way->getElementId());
    OGRFeature::DestroyFeature(poFeature);
    return;
  }

  if (_layer->CreateFeature(poFeature) != OGRERR_NONE)
  {
    LOG_WARN("Error creating feature " << way->getElementId());
  }

  OGRFeature::DestroyFeature(poFeature);
}

std::shared_ptr<geos::geom::Geometry> OgrMultifileWriter::_polyToMultipoly(const std::shared_ptr<geos::geom::Geometry>& geometry) const
{
  std::shared_ptr<geos::geom::Geometry> multi = geometry;
  if (_convertPolygons() && geometry->getGeometryTypeId() == GeometryTypeId::GEOS_POLYGON)
  {
    //  Convert the single polygon to a multipolygon
    std::vector<geos::geom::Geometry*>* single = new std::vector<geos::geom::Geometry*>({ geometry->clone().release() });
    multi.reset(geos::geom::GeometryFactory::getDefaultInstance()->createMultiPolygon(single));
  }
  return multi;
}

}
