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

#include "GmlWriter.h"

// GDAL
#include <ogr_geometry.h>

// GEOS
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
using namespace geos::geom;

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/OsmMap.h>

// Qt
#include <QFileInfo>

namespace hoot
{

void GmlWriter::write(boost::shared_ptr<const OsmMap> map, QString path)
{
  if (path.toLower().endsWith(".gml"))
  {
    path.remove(path.size() - 4, path.size());
  }
  writePoints(map, path + "Points.shp");
}

void GmlWriter::writePoints(boost::shared_ptr<const OsmMap> map, const QString& path)
{
  GDALAllRegister();
  OGRSetNonLinearGeometriesEnabledFlag(FALSE);

  const char *pszDriverName = "GML";
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

  QString layerName;
  layerName = QFileInfo(path).baseName();
  poLayer = poDS->CreateLayer(layerName.toAscii(),
                              map->getProjection().get(), wkbPoint, NULL );
  if( poLayer == NULL )
  {
    throw HootException(QString("Layer creation failed. %1").arg(path));
  }

  QStringList shpColumns;

  for (int i = 0; i < _columns.size(); i++)
  {
    OGRFieldDefn oField(_columns[i].toAscii(), OFTString);

    oField.SetWidth(64);

    if( poLayer->CreateField( &oField ) != OGRERR_NONE )
    {
      throw HootException(QString("Error creating field (%1).").arg(_columns[i]));
    }

    shpColumns.append(poLayer->GetLayerDefn()->GetFieldDefn(i)->GetNameRef());
  }

  if (_includeInfo)
  {
    OGRFieldDefn oField(MetadataTags::ErrorCircular().toStdString().c_str(), OFTReal);
    if( poLayer->CreateField( &oField ) != OGRERR_NONE )
    {
      throw HootException(QString("Error creating field (" + MetadataTags::ErrorCircular() + ")."));
    }
  }

  const NodeMap& nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    const boost::shared_ptr<Node>& node = it->second;

    if (node->getTags().getNonDebugCount() > 0)
    {
      OGRFeature* poFeature = OGRFeature::CreateFeature( poLayer->GetLayerDefn() );
      // set all the column values.
      for (int i = 0; i < _columns.size(); i++)
      {
        if (node->getTags().contains(_columns[i]))
        {
          QByteArray c = shpColumns[i].toAscii();
          QByteArray v = node->getTags()[_columns[i]].toAscii();
          poFeature->SetField(c.constData(), v.constData());
        }
      }

      if (_includeInfo)
      {
        poFeature->SetField(MetadataTags::ErrorCircular().toStdString().c_str(), node->getCircularError());
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

}
