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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "OgrReader.h"

// GDAL
#include <ogr_geometry.h>
#include <ogr_spatialref.h>

// GEOS
#include <geos/geom/LineString.h>
using namespace geos::geom;

// Hoot
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/elements/ElementIterator.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/io/OgrUtilities.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/PythonSchemaTranslator.h>
#include <hoot/core/schema/ScriptSchemaTranslator.h>
#include <hoot/core/schema/ScriptSchemaTranslatorFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QDir>
#include <QFileInfo>
#include <QDateTime>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapReader, OgrReader)

/**
 * I've put this in the C++ to avoid too much header nastiness for the classes that use the
 * OgrReader.
 */
class OgrReaderInternal : public ProgressReporter
{
public:

  static std::string className() { return "hoot::OgrReaderInternal"; }

  static int logWarnCount;

  OgrReaderInternal();

  virtual ~OgrReaderInternal();

  void close();

  /**
   * @see ProgressReporter
   */
  virtual void setProgress(Progress progress) { _progress = progress; }
  /**
   * @see ProgressReporter
   */
  virtual unsigned int getNumSteps() const { return 1; }

  /**
   * See the associated configuration options text for details.
   */
  std::shared_ptr<Envelope> getBoundingBoxFromConfig(const Settings& s, OGRSpatialReference* srs);

  Meters getDefaultCircularError() const { return _defaultCircularError; }
  Status getDefaultStatus() const { return _status; }

  QString getTranslationFile() const { return _translatePath; }

  long getFeatureCount() const { return _featureCount; }

  QStringList getLayersWithGeometry(const QString& path) const;

  void open(const QString& path, const QString& layer);

  QRegExp getNameFilter();

  bool isOpen() const { return _dataSource != NULL; }

  /**
   * Reads all the features into the given map.
   */
  void read(const OsmMapPtr& map);

  /**
   * Reads the next feature into the given map.
   */
  void readNext(const OsmMapPtr& map);

  void setDefaultCircularError(Meters circularError) { _defaultCircularError = circularError; }
  void setDefaultStatus(Status s) { _status = s; }
  void setLimit(long limit) { _limit = limit; }

  void setSchemaTranslationScript(const QString& translate)
  { _finalizeTranslate(); _translatePath = translate; }

  void initializePartial();

  void setUseDataSourceIds(bool useIds);

  ElementPtr readNextElement();
  bool hasMoreElements();

  std::shared_ptr<OGRSpatialReference> getProjection() const;

protected:

  Meters _defaultCircularError;
  Status _status;
  OsmMapPtr _map;
  OGRLayer* _layer;
  long _limit;
  long _count;
  long _featureCount;
  bool _useFileId;
  std::shared_ptr<GDALDataset> _dataSource;
  QString _path;
  QString _layerName;
  OGRCoordinateTransformation* _transform;
  std::shared_ptr<OGRSpatialReference> _wgs84;
  std::shared_ptr<ScriptSchemaTranslator> _translator;
  QStringList _pendingLayers;
  bool _addSourceDateTime;
  QString _nodeIdFieldName;

  //partial read iterators
  NodeMap::const_iterator _nodesItr;
  WayMap::const_iterator _waysItr;
  RelationMap::const_iterator _relationsItr;

  // store all key/value strings in this QHash, this promotes implicit sharing of string data. The
  // QHash goes away when the reading is done, but the memory sharing remains.
  QHash<QString, QString> _strings;

  QString _translatePath;

  Progress _progress;

  void _addFeature(OGRFeature* f);
  void _addGeometry(OGRGeometry* g, Tags& t);
  void _addLineString(OGRLineString* ls, Tags& t);
  void _addMultiPolygon(OGRMultiPolygon* mp, Tags& t);
  void _addPolygon(OGRPolygon* p, Tags& t);
  void _addPoint(OGRPoint* p, Tags& t);
  void _addPolygon(OGRPolygon* p, RelationPtr r, Meters circularError);

  WayPtr _createWay(OGRLinearRing* lr, Meters circularError);

  void _finalizeTranslate();

  /**
   * Use some hard coded rules to convert from projections that PROJ4 doesn't handle to projections
   * that it will handle.
   */
  std::shared_ptr<OGRSpatialReference> _fixProjection(std::shared_ptr<OGRSpatialReference> srs);

  void _initTranslate();

  void _openLayer(const QString& path, const QString& layer);
  void _openNextLayer();

  Meters _parseCircularError(Tags& t);

  void _reproject(double& x, double& y);

  const QString& _saveMemory(const QString& s);

  virtual void _translate(Tags&);

  void populateElementMap();

  QString _toWkt(OGRSpatialReference* srs);
  QString _toWkt(OGRGeometry* geom);
};

class OgrElementIterator : public ElementIterator
{
public:

  OgrElementIterator(OgrReaderInternal* d)
  {
    _d = d;
    _map.reset(new OsmMap());
  }

  virtual ~OgrElementIterator()
  {
    _d->close();
    delete _d;
  }

protected:

  virtual void _next()
  {
    _map->clear();
    _d->readNext(_map);

    const NodeMap& nm = _map->getNodes();
    for (NodeMap::const_iterator it = nm.begin(); it != nm.end(); ++it)
    {
      _addElement(_map->getNode(it->first));
    }

    const WayMap& wm = _map->getWays();
    for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
    {
      _addElement(_map->getWay(it->first));
    }
  }

private:

  OsmMapPtr _map;
  OgrReaderInternal* _d;
};

OgrReader::OgrReader()
{
  _d = new OgrReaderInternal();
}

OgrReader::OgrReader(const QString& path)
{
  _d = new OgrReaderInternal();
  if (isSupported(path) == true)
  {
    _d->open(path, QString(""));
  }
}

OgrReader::OgrReader(const QString& path, const QString& layer)
{
  _d = new OgrReaderInternal();
  if (isSupported(path) == true)
  {
    _d->open(path, layer);
  }
}

OgrReader::~OgrReader()
{
  delete _d;
}

void OgrReader::setProgress(Progress progress)
{
  if (_d == 0)
  {
    throw IllegalArgumentException(
      "Internal reader must be initialized before setting progress on OgrReader.");
  }
  _d->setProgress(progress);
}

ElementIterator* OgrReader::createIterator(const QString& path, const QString& layer) const
{
  OgrReaderInternal* d = new OgrReaderInternal();
  d->setDefaultCircularError(_d->getDefaultCircularError());
  d->setDefaultStatus(_d->getDefaultStatus());
  d->setSchemaTranslationScript(_d->getTranslationFile());
  d->open(path, layer);

  return new OgrElementIterator(d);
}

std::shared_ptr<OGRSpatialReference> OgrReaderInternal::_fixProjection(std::shared_ptr<OGRSpatialReference> srs)
{
  std::shared_ptr<OGRSpatialReference> result;
  int epsgOverride = ConfigOptions().getOgrReaderEpsgOverride();
  if (epsgOverride >= 0)
  {
    result.reset(new OGRSpatialReference());

    if (result->importFromEPSG(epsgOverride) != OGRERR_NONE)
    {
      throw HootException(QString("Error creating EPSG:%1 projection.").arg(epsgOverride));
    }

    return result;
  }

  // proj4 requires some extra parameters to handle Google map style projections. Check for this
  // situation for known EPSGs and warn/fix the issue.
  result.reset(new OGRSpatialReference());
  result->importFromEPSG(3785);
  if (srs && result->IsSame(srs.get()) &&
    _toWkt(result.get()) != _toWkt(srs.get()))
  {
    LOG_WARN("Overriding input projection with proj4 compatible EPSG:3785. See this for details: https://trac.osgeo.org/proj/wiki/FAQ#ChangingEllipsoidWhycantIconvertfromWGS84toGoogleEarthVirtualGlobeMercator");
    return result;
  }

  result->importFromEPSG(900913);
  if (srs && result->IsSame(srs.get()) &&
    _toWkt(result.get()) != _toWkt(srs.get()))
  {
    LOG_WARN("Overriding input projection with proj4 compatible EPSG:900913. See this for details: https://trac.osgeo.org/proj/wiki/FAQ#ChangingEllipsoidWhycantIconvertfromWGS84toGoogleEarthVirtualGlobeMercator");
    return result;
  }

  result->importFromEPSG(3857);
  if (srs && result->IsSame(srs.get()) &&
    _toWkt(result.get()) != _toWkt(srs.get()))
  {
    LOG_WARN("Overriding input projection with proj4 compatible EPSG:3857. See this for details: https://trac.osgeo.org/proj/wiki/FAQ#ChangingEllipsoidWhycantIconvertfromWGS84toGoogleEarthVirtualGlobeMercator");
    return result;
  }

  // this check came from https://insightcloud.digitalglobe.com/redmine/issues/4399
  // The input isn't considered the same as EPSG:3857 according to OGR. I do this WKT level check
  // to override the projection.
  const char* wkt3857 = "PROJCS[\"WGS_1984_Web_Mercator_Auxiliary_Sphere\",GEOGCS[\"GCS_WGS_1984\",DATUM[\"WGS_1984\",SPHEROID[\"WGS_84\",6378137.0,298.257223563]],PRIMEM[\"Greenwich\",0.0],UNIT[\"Degree\",0.0174532925199433]],PROJECTION[\"Mercator_Auxiliary_Sphere\"],PARAMETER[\"False_Easting\",0.0],PARAMETER[\"False_Northing\",0.0],PARAMETER[\"Central_Meridian\",0.0],PARAMETER[\"Standard_Parallel_1\",0.0],PARAMETER[\"Auxiliary_Sphere_Type\",0.0],UNIT[\"Meter\",1.0],AUTHORITY[\"EPSG\",\"3857\"]]";
  result->importFromWkt((char**)&wkt3857);
  if (srs && result->IsSame(srs.get()))
  {
    LOG_WARN("Overriding input projection with proj4 compatible EPSG:3857. See this for details: https://trac.osgeo.org/proj/wiki/FAQ#ChangingEllipsoidWhycantIconvertfromWGS84toGoogleEarthVirtualGlobeMercator");
    result->importFromEPSG(3857);
    return result;
  }

  result = srs;
  return result;
}

std::shared_ptr<Envelope> OgrReader::getBoundingBoxFromConfig(const Settings& s,
  OGRSpatialReference* srs)
{
  return _d->getBoundingBoxFromConfig(s, srs);
}

/**
 * Returns a list of all layer names includeing those that don't have geometry.
 */
QStringList OgrReader::getLayerNames(const QString& path)
{
  QStringList result;
  std::shared_ptr<GDALDataset> ds = OgrUtilities::getInstance().openDataSource(path, true);
  int count = ds->GetLayerCount();
  for (int i = 0; i < count; i++)
  {
    OGRLayer* l = ds->GetLayer(i);
    result.append(l->GetName());
    l->Dereference();
  }

  // make the results consistent
  result.sort();

  return result;
}

/**
 * Returns a filtered list of layer names that have geometry.
 */
QStringList OgrReader::getFilteredLayerNames(const QString& path)
{
  QRegExp filterStr = _d->getNameFilter();
  LOG_VART(filterStr.pattern());

  QStringList result;

  QStringList allLayers = _d->getLayersWithGeometry(path);
  LOG_VARD(allLayers);

  for (int i = 0; i < allLayers.size(); i++)
  {
    if (allLayers[i].contains(filterStr))
    {
      result.append(allLayers[i]);
    }
  }

  // make the results consistent
  result.sort();

  return result;
}

bool OgrReader::isReasonablePath(const QString& path)
{
  return OgrUtilities::getInstance().isReasonableUrl(path);
}

long OgrReader::getFeatureCount(const QString& path, const QString& layer)
{
  _d->open(path, layer);
  _d->close();
  return _d->getFeatureCount();
}

void OgrReader::read(const QString& path, const QString& layer, const OsmMapPtr& map)
{
  _d->open(path, layer);
  _d->read(map);
  _d->close();
}

void OgrReader::setDefaultCircularError(Meters circularError)
{
  _d->setDefaultCircularError(circularError);
}

void OgrReader::setDefaultStatus(Status s)
{
  _d->setDefaultStatus(s);
}

void OgrReader::setLimit(long limit)
{
  _d->setLimit(limit);
}

void OgrReader::setSchemaTranslationScript(const QString& translate)
{
  _d->setSchemaTranslationScript(translate);
}

void OgrReader::initializePartial()
{
  _d->initializePartial();
}

bool OgrReader::hasMoreElements()
{
  return _d->hasMoreElements();
}

ElementPtr OgrReader::readNextElement()
{
  return _d->readNextElement();
}

void OgrReader::close()
{
  return _d->close();
}

void OgrReader::finalizePartial()
{
  _elementsRead = 0;
}

bool OgrReader::isSupported(const QString& url)
{
  return isReasonablePath(url);
}

void OgrReader::setUseDataSourceIds(bool useDataSourceIds)
{
  _d->setUseDataSourceIds(useDataSourceIds);
}

void OgrReader::open(const QString& url)
{
  _d->open(url, "");
}

std::shared_ptr<OGRSpatialReference> OgrReader::getProjection() const
{
  return _d->getProjection();
}

int OgrReaderInternal::logWarnCount = 0;

OgrReaderInternal::OgrReaderInternal()
{
  _map = OsmMapPtr(new OsmMap());
  _nodesItr = _map->getNodes().begin();
  _waysItr =  _map->getWays().begin();
  _relationsItr = _map->getRelations().begin();
  _layer = NULL;
  _transform = NULL;
  _status = Status::Invalid;
  _defaultCircularError = ConfigOptions().getCircularErrorDefaultValue();
  _limit = -1;
  _featureCount = 0;
  _addSourceDateTime = ConfigOptions().getReaderAddSourceDatetime();
  _nodeIdFieldName = ConfigOptions().getOgrReaderNodeIdFieldName();
}

OgrReaderInternal::~OgrReaderInternal()
{
  close();

  if (_transform != NULL)
  {
    OGRCoordinateTransformation::DestroyCT(_transform);
  }
}

QStringList OgrReaderInternal::getLayersWithGeometry(const QString& path) const
{
  QStringList result;
  LOG_DEBUG("Opening layers with geometry: " << path);
  std::shared_ptr<GDALDataset> ds = OgrUtilities::getInstance().openDataSource(path, true);
  int count = ds->GetLayerCount();
  LOG_VART(count);
  for (int i = 0; i < count; i++)
  {
    OGRLayer* l = ds->GetLayer(i);
    if (l->GetGeomType() != wkbNone)
    {
      result.append(l->GetName());
      LOG_VART(l->GetName());
    }
    l->Dereference();
  }

  // make the result consistent.
  result.sort();

  return result;
}

QRegExp OgrReaderInternal::getNameFilter()
{
  // Just to make sure we have a translation script to work with
  _initTranslate();

  // Check the commandline / environment var
  QString rawFilter = ConfigOptions().getOgrImportFilter();

  if (rawFilter == "")
  {
    // Try the translation file. This function will return "." by default
    rawFilter = _translator->getLayerNameFilter();
  }

  QRegExp result(rawFilter);

  return result;
}

void OgrReaderInternal::_addFeature(OGRFeature* f)
{
  Tags t;

  OGRFeatureDefn* fieldDefn = f->GetDefnRef();

  for (int i = 0; i < fieldDefn->GetFieldCount(); i++)
  {
    QString value;

    // This skips NULL fields.
    if (! f->IsFieldSet(i)) continue;

    if (f->GetDefnRef()->GetFieldDefn(i)->GetType() == OFTReal)
    {
      value = QString::number(f->GetFieldAsDouble(i), 'g', 17);
    }
    else
    {
      value = QString::fromUtf8(f->GetFieldAsString(i));
    }

    // Ticket 5833: make sure tag is only added if value is non-null
    if ( value.length() == 0 )
    {
      LOG_TRACE(
        "Skipping tag w/ key=" << fieldDefn->GetFieldDefn(i)->GetNameRef() <<
        " since the value field is empty");
      continue;
    }

    t[fieldDefn->GetFieldDefn(i)->GetNameRef()] = value;
  }

  _translate(t);

  if (_addSourceDateTime)
  {
    // Add an ingest datetime tag
    t.appendValue(MetadataTags::SourceIngestDateTime(),
                  QDateTime::currentDateTime().toUTC().toString("yyyy-MM-ddThh:mm:ss.zzzZ"));
  }

  if (t.size() != 0)
  {
    _addGeometry(f->GetGeometryRef(), t);
  }
}

void OgrReaderInternal::_addGeometry(OGRGeometry* g, Tags& t)
{
  if (g)
  {
    try
    {
      switch (wkbFlatten(g->getGeometryType()))
      {
        case wkbLineString:
          LOG_TRACE("Adding line string: " << _toWkt(g).left(100));
          _addLineString((OGRLineString*)g, t);
          break;
        case wkbPoint:
          LOG_TRACE("Adding point: " << _toWkt(g).left(100));
          _addPoint((OGRPoint*)g, t);
          break;
        case wkbPolygon:
          LOG_TRACE("Adding polygon: " << _toWkt(g).left(100));
          _addPolygon((OGRPolygon*)g, t);
          break;
        case wkbMultiPolygon:
          LOG_TRACE("Adding multi-polygon: " << _toWkt(g).left(100));
          _addMultiPolygon((OGRMultiPolygon*)g, t);
          break;
        case wkbMultiPoint:
        case wkbMultiLineString:
        case wkbGeometryCollection:
        {
          LOG_TRACE(
            "Adding geometry collection (multipoint, multiline, etc.): " <<
            _toWkt(g).left(100));
          OGRGeometryCollection* gc = dynamic_cast<OGRGeometryCollection*>(g);
          int nParts = gc->getNumGeometries();
          for (int i = 0; i < nParts; i++)
          {
            _addGeometry(gc->getGeometryRef(i), t);
          }
          break;
        }
        default:
          throw HootException("Unsupported geometry type.");
      }
    }
    catch (const IllegalArgumentException& e)
    {
      throw IllegalArgumentException(
        "Error projecting geometry with tags: " + t.toString() + " " + e.what());
    }
  }
}

void OgrReaderInternal::_addLineString(OGRLineString* ls, Tags& t)
{
  Meters circularError = _parseCircularError(t);

  WayPtr way(new Way(_status, _map->createNextWayId(), circularError));

  way->setTags(t);
  for (int i = 0; i < ls->getNumPoints(); i++)
  {
    double x = ls->getX(i);
    double y = ls->getY(i);
    _reproject(x, y);
    NodePtr n(Node::newSp(_status, _map->createNextNodeId(), x, y, circularError));
    _map->addNode(n);
    way->addNode(n->getId());
  }
  _map->addWay(way);
}

void OgrReaderInternal::_addMultiPolygon(OGRMultiPolygon* mp, Tags& t)
{
  Meters circularError = _parseCircularError(t);

  int nParts = mp->getNumGeometries();
  if (nParts == 1)
  {
    _addPolygon((OGRPolygon*)mp->getGeometryRef(0), t);
  }
  else
  {
    RelationPtr r(new Relation(_status, _map->createNextRelationId(), circularError,
      MetadataTags::RelationMultiPolygon()));
    r->setTags(t);

    for (int i = 0; i < nParts; i++)
    {
      _addPolygon((OGRPolygon*)mp->getGeometryRef(i), r, circularError);
    }

    _map->addRelation(r);
  }
}

void OgrReaderInternal::_addPoint(OGRPoint* p, Tags& t)
{
  Meters circularError = _parseCircularError(t);

  double x = p->getX();
  double y = p->getY();
  _reproject(x, y);
  long id;
  if (_nodeIdFieldName.isEmpty())
  {
    id = _map->createNextNodeId();
  }
  else
  {
    bool ok = false;
    id = t.get(_nodeIdFieldName).toLong(&ok);
    if (!ok)
    {
      throw HootException("Unable to parse node ID from field: " + _nodeIdFieldName);
    }
  }
  NodePtr node(Node::newSp(_status, id, x, y, circularError));

  node->setTags(t);
  _map->addNode(node);
}

void OgrReaderInternal::_addPolygon(OGRPolygon* p, Tags& t)
{
  Meters circularError = _parseCircularError(t);

  AreaCriterion areaCrit;
  if (p->getNumInteriorRings() == 0)
  {
    OGRLinearRing* exteriorRing = p->getExteriorRing();
    if (exteriorRing != 0)
    {
      WayPtr outer = _createWay(p->getExteriorRing(), circularError);
      if (areaCrit.isSatisfied(t, ElementType::Way) == false)
      {
        t.setArea(true);
      }
      outer->setTags(t);
      _map->addWay(outer);
    }
    else
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN(
          "Skipping polygon in layer: " << _layerName.toLatin1().data() <<
          " with empty exterior ring: " << _toWkt(p).left(100));
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
  }
  else
  {
    RelationPtr r(new Relation(_status, _map->createNextRelationId(), circularError,
      MetadataTags::RelationMultiPolygon()));
    if (areaCrit.isSatisfied(t, ElementType::Relation) == false)
    {
      t.setArea(true);
    }
    r->setTags(t);
    _addPolygon(p, r, circularError);
    _map->addRelation(r);
  }
}

QString OgrReaderInternal::_toWkt(OGRGeometry* geom)
{
  char* buffer;
  geom->exportToWkt(&buffer);
  QString result = QString::fromUtf8(buffer);
  delete buffer;
  return result;
}

void OgrReaderInternal::_addPolygon(OGRPolygon* p, RelationPtr r, Meters circularError)
{
  WayPtr outer = _createWay(p->getExteriorRing(), circularError);
  _map->addWay(outer);
  r->addElement(MetadataTags::RoleOuter(), outer);

  for (int i = 0; i < p->getNumInteriorRings(); i++)
  {
    WayPtr inner = _createWay(p->getInteriorRing(i), circularError);
    _map->addWay(inner);
    r->addElement(MetadataTags::RoleInner(), inner);
  }
}

void OgrReaderInternal::close()
{
  // no need to finalize the translation here. We may need it again.

  _useFileId = false;

  if (_dataSource != NULL)
  {
    if (_layer != NULL)
    {
      _layer->Dereference();
    }
    _dataSource.reset();
    _layer = NULL;
    _pendingLayers.clear();
  }
}

WayPtr OgrReaderInternal::_createWay(OGRLinearRing* lr, Meters circularError)
{
  WayPtr way(new Way(_status, _map->createNextWayId(), circularError));

  // make sure the ring is closed
  lr->closeRings();

  // go through all the points except the last one (duplicate of the first)
  for (int i = 0; i < lr->getNumPoints() - 1; i++)
  {
    double x = lr->getX(i);
    double y = lr->getY(i);
    _reproject(x, y);
    NodePtr n(Node::newSp(_status, _map->createNextNodeId(), x, y, circularError));
    _map->addNode(n);
    way->addNode(n->getId());
  }
  // add the first node back on to close the way.
  way->addNode(way->getNodeId(0));

  return way;
}

void OgrReaderInternal::_finalizeTranslate()
{
  _translator.reset();
}

std::shared_ptr<Envelope> OgrReaderInternal::getBoundingBoxFromConfig(const Settings& s,
  OGRSpatialReference* srs)
{
  ConfigOptions co(s);
  std::shared_ptr<Envelope> result;
  const QString bboxStrRaw = co.getConvertBoundingBox();
  const bool asWgs84 = co.getOgrReaderBoundingBoxLatlng();
  QString bboxStr;
  QString key;

  if (bboxStrRaw.isEmpty())
  {
    return result;
  }
  else if (bboxStrRaw.isEmpty() == false)
  {
    bboxStr = bboxStrRaw;
  }

  if (!asWgs84)
  {
    result.reset(new Envelope(GeometryUtils::envelopeFromConfigString(bboxStr)));
  }
  else
  {
    if (!srs)
    {
      throw HootException("A valid projection must be available when using a lat/lng bounding "
        "box.");
    }

    QStringList bbox = bboxStr.split(",");

    if (bbox.size() != 4)
    {
      throw HootException(
        QString("Error parsing %1 (%2)").arg(co.getConvertBoundingBoxKey()).arg(bboxStr));
    }

    bool ok;
    vector<double> bboxValues(4);
    for (size_t i = 0; i < 4; i++)
    {
      bboxValues[i] = bbox[i].toDouble(&ok);
      if (!ok)
      {
        throw HootException(
          QString("Error parsing %1 (%2)").arg(co.getConvertBoundingBoxKey()).arg(bboxStr));
      }
    }

    result.reset(new Envelope());
    std::shared_ptr<OGRSpatialReference> wgs84 = MapProjector::createWgs84Projection();
    std::shared_ptr<OGRCoordinateTransformation> transform(
      OGRCreateCoordinateTransformation(wgs84.get(), srs));
    const int steps = 8;
    for (int xi = 0; xi <= steps; xi++)
    {
      double x = bboxValues[0] + xi * (bboxValues[2] - bboxValues[0]) / (double)steps;
      for (int yi = 0; yi <= steps; yi++)
      {
        double ty = bboxValues[1] + yi * (bboxValues[3] - bboxValues[1]) / (double)steps;
        double tx = x;

        transform->TransformEx(1, &tx, &ty);

        result->expandToInclude(tx, ty);
      }
    }
  }

  return result;
}

void OgrReaderInternal::_initTranslate()
{
  if (_translatePath != "" && _translator.get() == 0)
  {
    // Nice and short. Taken from TranslatedTagDifferencer
    _translator.reset(ScriptSchemaTranslatorFactory::getInstance().createTranslator(_translatePath));

    if (_translator.get() == 0)
    {
      throw HootException("Unable to find a valid translation format for: " + _translatePath);
    }
  }
}

void OgrReaderInternal::open(const QString& path, const QString& layer)
{
  _initTranslate();

  _path = path;
  LOG_TRACE("Opening data source for layer: " << layer);
  _dataSource = OgrUtilities::getInstance().openDataSource(path, true);
  if (layer.isEmpty() == false)
  {
    _pendingLayers.append(layer);
  }
  else
  {
    _pendingLayers = getLayersWithGeometry(path);
  }
  LOG_VARD(_pendingLayers);
}

void OgrReaderInternal::_openLayer(const QString& path, const QString& layer)
{
  _path = path;
  _layerName = layer;
  if (layer == "")
  {
    throw HootException("Please specify a layer to open.");
  }
  else
  {
    _layer = _dataSource->GetLayerByName(layer.toUtf8());
  }

  if (_layer == NULL)
  {
    throw HootException("Failed to identify source layer from data source.");
  }

  std::shared_ptr<OGRSpatialReference> sourceSrs;

  if (_layer->GetSpatialRef())
  {
    sourceSrs.reset(_layer->GetSpatialRef()->Clone());
    sourceSrs = _fixProjection(sourceSrs);
  }

  if (sourceSrs.get() != 0 && sourceSrs->IsProjected())
  {
    LOG_DEBUG("Input SRS: " << _toWkt(sourceSrs.get()));
    _wgs84.reset(new OGRSpatialReference());
    if (_wgs84->SetWellKnownGeogCS("WGS84") != OGRERR_NONE)
    {
      throw HootException("Error creating EPSG:4326 projection.");
    }

    _transform = OGRCreateCoordinateTransformation(sourceSrs.get(), _wgs84.get());

    if (_transform == 0)
    {
      throw HootException(QString("Error creating transformation object: ") + CPLGetLastErrorMsg());
    }
  }

  std::shared_ptr<Envelope> filter = getBoundingBoxFromConfig(conf(), sourceSrs.get());
  if (filter.get())
  {
    _layer->SetSpatialFilterRect(filter->getMinX(), filter->getMinY(), filter->getMaxX(),
      filter->getMaxY());
    LOG_DEBUG("Setting spatial filter on " << layer << " to: " << filter->toString());
  }

  // retrieve the feature count for current layer
  _featureCount = _layer->GetFeatureCount(false);
}

void OgrReaderInternal::_openNextLayer()
{
  _layer = NULL;

  if (_pendingLayers.isEmpty() == false)
  {
    LOG_TRACE("Opening layer " + _pendingLayers.front());
    _openLayer(_path, _pendingLayers.front());
    _pendingLayers.pop_front();
  }
}

Meters OgrReaderInternal::_parseCircularError(Tags& t)
{
  Meters circularError = _defaultCircularError;

  // parse the circularError out of the tags.
  if (t.contains(MetadataTags::ErrorCircular()))
  {
    bool ok;
    double a = t[MetadataTags::ErrorCircular()].toDouble(&ok);
    if (!ok)
    {
      try
      {
        a = t.getLength(MetadataTags::ErrorCircular()).value();
        ok = true;
      }
      catch (const HootException&)
      {
        ok = false;
      }
    }
    if (ok)
    {
      circularError = a;
      t.remove(MetadataTags::ErrorCircular());
    }
  }
  else if (t.contains(MetadataTags::Accuracy()))
  {
    bool ok;
    double a = t[MetadataTags::Accuracy()].toDouble(&ok);
    if (!ok)
    {
      try
      {
        a = t.getLength(MetadataTags::Accuracy()).value();
        ok = true;
      }
      catch (const HootException&)
      {
        ok = false;
      }
    }
    if (ok)
    {
      circularError = a;
      t.remove(MetadataTags::Accuracy());
    }
  }

  return circularError;
}

void OgrReaderInternal::read(const OsmMapPtr& map)
{
  _map = map;
  _count = 0;

  _openNextLayer();

  if (_layer == NULL)
  {
    throw HootException("Error reading from input. No valid layers. Did you forget to set the "
      "layer name?");
  }

  LOG_INFO("Reading: " << _layerName.toLatin1().data() << "...");

  OGRFeature* f;
  while ((f = _layer->GetNextFeature()) != NULL && (_limit == -1 || _count < _limit))
  {
    _addFeature(f);
    OGRFeature::DestroyFeature(f);
    f = 0;

    _count++;
    if (_progress.getState() != Progress::JobState::Pending && _count % 10000 == 0)
    {
      LOG_VART(_count);
      LOG_VART(_featureCount);
      LOG_VART(_layerName.toLatin1().data());
      // TODO: This isn't clearing out the current line before each new log statement.
      _progress.setFromRelative(
        (float)_count / (float)_featureCount, Progress::JobState::Running,
        "Read " + StringUtils::formatLargeNumber(_count) + " / " +
        StringUtils::formatLargeNumber(_featureCount) + " features from layer: " +
        _layerName.toLatin1().data(), true);
    }
  }
}

void OgrReaderInternal::readNext(const OsmMapPtr& map)
{
  bool done = false;

  _map = map;

  while (!done)
  {
    // if the current layer is empty
    if (_layer == NULL)
    {
      // open the next layer
      _openNextLayer();
    }

    // if there are no more layers
    if (_layer == NULL)
    {
      // we're done
      done = true;
    }
    // if this is a valid layer
    else
    {
      // read the next feature
      OGRFeature* f = _layer->GetNextFeature();
      // if there was a "next" feature
      if (f != NULL)
      {
        // add the feature
        _addFeature(f);
        OGRFeature::DestroyFeature(f);
        // we're done
        done = true;
      }
      // if there wasn't a next feature
      else
      {
        // this layer is now empty set it to null so we'll load the next layer
        _layer = NULL;
      }
    }
  }
}

void OgrReaderInternal::_reproject(double& x, double& y)
{
  if (_transform != NULL)
  {
    double inx = x;
    double iny = y;
    if (_transform->Transform(1, &x, &y) == FALSE)
    {
      LOG_TRACE("Source x: " << inx);
      LOG_TRACE("Source y: " << iny);
      LOG_TRACE("Target x: " << x);
      LOG_TRACE("Target y: " << y);
      throw IllegalArgumentException("Unable to transform point. Is the point outside the "
                                     "projection bounds?");
    }
  }
}

const QString& OgrReaderInternal::_saveMemory(const QString& s)
{
  if (!_strings.contains(s))
  {
    _strings[s] = s;
  }
  return _strings[s];
}

void OgrReaderInternal::_translate(Tags& t)
{
  if (_translator.get() != 0)
  {
    QByteArray geomType;

    switch (wkbFlatten(_layer->GetGeomType()))
    {
    case wkbPoint:
    case wkbMultiPoint:
      geomType = "Point";
      break;

    case wkbLineString:
    case wkbMultiLineString:
      geomType = "Line";
      break;

    case wkbPolygon:
    case wkbMultiPolygon:
      geomType = "Area";
      break;

    case wkbGeometryCollection:
      geomType = "Collection";
      break;

    default:
      throw HootException("Unsupported geometry type.");
    }

    LOG_TRACE("Translating tags of size: " << t.size() << " to OSM...");
    _translator->translateToOsm(t, _layer->GetLayerDefn()->GetName(), geomType);
  }
  else
  {
    t[MetadataTags::HootLayername()] = _layer->GetLayerDefn()->GetName();
  }
}

void OgrReaderInternal::initializePartial()
{
  _map.reset(new OsmMap());
  _nodesItr = _map->getNodes().begin();
  _waysItr =  _map->getWays().begin();
  _relationsItr = _map->getRelations().begin();

  _useFileId = false;
}

void OgrReaderInternal::setUseDataSourceIds(bool useIds)
{
  _useFileId = useIds;
}

bool OgrReaderInternal::hasMoreElements()
{
  // If we're not open, definitely no more elements
  if (isOpen() == false)
  {
    return false;
  }

  // Do we have data already in map from previous reads?
  if ( (_nodesItr != _map->getNodes().end()) || (_waysItr != _map->getWays().end())
      || (_relationsItr != _map->getRelations().end()) )
  {
    return true;
  }

  // Let's try a read and see if that put anything into the map

  // Do a read if the element maps are empty
  populateElementMap();

  bool result = ( (_nodesItr != _map->getNodes().end()) || (_waysItr != _map->getWays().end())
      || (_relationsItr != _map->getRelations().end()) );

  return result;
}

ElementPtr OgrReaderInternal::readNextElement()
{
  if ((_nodesItr == _map->getNodes().end()) && (_waysItr == _map->getWays().end())
      && (_relationsItr == _map->getRelations().end()))
  {
    // Load the next OGR feature, with 1..N elemenents per feature, into the map of the various
    //    element types
    populateElementMap();
  }

  ElementPtr returnElement;
  if (_nodesItr != _map->getNodes().end())
  {
    returnElement = _nodesItr->second->cloneSp();
    ++_nodesItr;
  }
  else if (_waysItr != _map->getWays().end())
  {
    returnElement.reset(new Way(*_waysItr->second.get()));
    ++_waysItr;
  }
  else
  {
    returnElement.reset(new Relation(*_relationsItr->second.get()));
    ++_relationsItr;
  }

  return returnElement;
}

void OgrReaderInternal::populateElementMap()
{
  _map->clear();

  readNext(_map);

  _nodesItr = _map->getNodes().begin();
  _waysItr =  _map->getWays().begin();
  _relationsItr = _map->getRelations().begin();
}

std::shared_ptr<OGRSpatialReference> OgrReaderInternal::getProjection() const
{
  std::shared_ptr<OGRSpatialReference> wgs84(new OGRSpatialReference());
  if (wgs84->SetWellKnownGeogCS("WGS84") != OGRERR_NONE)
  {
    throw HootException("Error creating EPSG:4326 projection.");
  }
  return wgs84;
}

QString OgrReaderInternal::_toWkt(OGRSpatialReference* srs)
{
  char* buffer;
  srs->exportToWkt(&buffer);
  QString result = QString::fromUtf8(buffer);
  delete buffer;
  return result;
}

}
