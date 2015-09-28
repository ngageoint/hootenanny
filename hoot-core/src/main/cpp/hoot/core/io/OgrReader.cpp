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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "OgrReader.h"

// GDAL
#include <ogr_geometry.h>
#include <ogr_spatialref.h>

// GEOS
#include <geos/geom/LineString.h>
using namespace geos::geom;

// Hoot
#include <hoot/core/io/ScriptTranslator.h>
#include <hoot/core/io/ScriptTranslatorFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/io/OgrUtilities.h>
#include <hoot/core/util/Progress.h>
#include <hoot/core/Factory.h>

#include <boost/shared_ptr.hpp>

// Qt
#include <QDir>
#include <QFileInfo>
#include <QDateTime>

// #include "JavaScriptTranslator.h"
#include "PythonTranslator.h"

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapReader, OgrReader)

/**
 * I've put this in the C++ to avoid too much header nastiness for the classes that use the
 * OgrReader.
 */
class OgrReaderInternal
{
public:

  OgrReaderInternal();

  virtual ~OgrReaderInternal();

  void close();

  Meters getDefaultCircularError() const { return _circularError; }

  Status getDefaultStatus() const { return _status; }

  QString getTranslationFile() const { return _translatePath; }

  long getFeatureCount() const { return _featureCount; }

  QStringList getLayersWithGeometry(QString path) const;

  void open(QString path, QString layer);

  QRegExp getNameFilter();

  bool isOpen() const { return _dataSource != NULL; }

  /**
   * Reads all the features into the given map.
   */
  void read(shared_ptr<OsmMap> map, Progress progress);

  /**
   * Reads the next feature into the given map.
   */
  void readNext(const shared_ptr<OsmMap>& map);

  void setDefaultCircularError(Meters circularError) { _circularError = circularError; }

  void setDefaultStatus(Status s) { _status = s; }

  void setLimit(long limit) { _limit = limit; }

  void setTranslationFile(QString translate) { _finalizeTranslate(); _translatePath = translate; }

  void initializePartial();

  void setUseDataSourceIds(bool useIds);

  ElementPtr readNextElement();

  bool hasMoreElements();

  boost::shared_ptr<OGRSpatialReference> getProjection() const;

  Progress streamGetProgress() const;

protected:
  Meters _circularError;
  Status _status;
  shared_ptr<OsmMap> _map;
  OGRLayer* _layer;
  long _limit;
  long _count;
  long _featureCount;
  bool _useFileId;
  shared_ptr<OGRDataSource> _dataSource;
  QString _path;
  QString _layerName;
  OGRCoordinateTransformation* _transform;
  auto_ptr<OGRSpatialReference> _wgs84;
  auto_ptr<ScriptTranslator> _translator;
  long _streamFeatureCount;
  QStringList _pendingLayers;

  /*
  long _partialNodesRead;
  long _partialWaysRead;
  long _partialRelationsRead;
  bool _firstPartialReadCompleted;
  */

  //partial read iterators
  OsmMap::NodeMap::const_iterator _nodesItr;
  WayMap::const_iterator _waysItr;
  RelationMap::const_iterator _relationsItr;

  // store all key/value strings in this QHash, this promotes implicit sharing of string data. The
  // QHash goes away when the reading is done, but the memory sharing remains.
  QHash<QString, QString> _strings;

  QString _translatePath;

  void _addFeature(OGRFeature* f);

  void _addGeometry(OGRGeometry* g, Tags& t);

  void _addLineString(OGRLineString* ls, Tags& t);

  void _addMultiPolygon(OGRMultiPolygon* mp, Tags& t);

  void _addPolygon(OGRPolygon* p, Tags& t);

  void _addPoint(OGRPoint* p, Tags& t);

  void _addPolygon(OGRPolygon* p, shared_ptr<Relation> r, Meters circularError);

  shared_ptr<Way> _createWay(OGRLinearRing* lr, Meters circularError);

  void _finalizeTranslate();

  void _initTranslate();

  void _openLayer(QString path, QString layer);

  void _openNextLayer();

  Meters _parseCircularError(Tags& t);

  void _reproject(double& x, double& y);

  const QString& _saveMemory(const QString& s);

  virtual void _translate(Tags&);

  void populateElementMap();

  QString _toWkt(OGRSpatialReference* srs);
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

    const OsmMap::NodeMap& nm = _map->getNodeMap();
    for (OsmMap::NodeMap::ConstIterator it = nm.constBegin(); it != nm.constEnd(); it++)
    {
      _addElement(_map->getNode(it.key()));
    }

    const WayMap& wm = _map->getWays();
    for (WayMap::const_iterator it = wm.begin(); it != wm.end(); it++)
    {
      _addElement(_map->getWay(it->first));
    }

  }

private:
  shared_ptr<OsmMap> _map;
  OgrReaderInternal* _d;
};


OgrReader::OgrReader()
{
  _d = new OgrReaderInternal();
}

OgrReader::OgrReader(QString path)
{
  _d = new OgrReaderInternal();
  if ( isSupported(path) == true )
  {
    _d->open(path, QString(""));
  }
}

OgrReader::OgrReader(QString path, QString layer)
{
  _d = new OgrReaderInternal();
  if ( isSupported(path) == true )
  {
    _d->open(path, layer);
  }
}

OgrReader::~OgrReader()
{
  delete _d;
}

ElementIterator* OgrReader::createIterator(QString path, QString layer) const
{
  OgrReaderInternal* d = new OgrReaderInternal();
  d->setDefaultCircularError(_d->getDefaultCircularError());
  d->setDefaultStatus(_d->getDefaultStatus());
  d->setTranslationFile(_d->getTranslationFile());
  d->open(path, layer);

  return new OgrElementIterator(d);
}

QStringList OgrReader::getLayerNames(QString path)
{
  QStringList result;
  shared_ptr<OGRDataSource> ds = OgrUtilities::getInstance().openDataSource(path);
  int count = ds->GetLayerCount();
  for (int i = 0; i < count; i++)
  {
    OGRLayer* l = ds->GetLayer(i);
    result.append(l->GetName());
    l->Dereference();
  }

  // make the results consitent
  result.sort();

  return result;
}

QStringList OgrReader::getFilteredLayerNames(QString path)
{
  QRegExp filterStr = _d->getNameFilter();

  QStringList result;

  QStringList allLayers = getLayerNames(path);

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

bool OgrReader::isReasonablePath(QString path)
{
  return OgrUtilities::getInstance().isReasonableUrl(path);
}

long OgrReader::getFeatureCount(QString path, QString layer)
{
  _d->open(path, layer);
  _d->close();
  return _d->getFeatureCount();
}

void OgrReader::read(QString path, QString layer, shared_ptr<OsmMap> map, Progress progress)
{
  _d->open(path, layer);
  _d->read(map, progress);
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

void OgrReader::setTranslationFile(QString translate)
{ 
  _d->setTranslationFile(translate);
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

bool OgrReader::isSupported(QString url)
{
  return isReasonablePath(url);
}

void OgrReader::setUseDataSourceIds(bool useDataSourceIds)
{
  _d->setUseDataSourceIds(useDataSourceIds);
}

void OgrReader::open(QString url)
{
  _d->open(url, "");
}

boost::shared_ptr<OGRSpatialReference> OgrReader::getProjection() const
{
  return _d->getProjection();
}

Progress OgrReader::streamGetProgress() const
{
  return _d->streamGetProgress();
}

OgrReaderInternal::OgrReaderInternal()
{
  _map = boost::shared_ptr<OsmMap>(new OsmMap());
  _nodesItr = _map->getNodeMap().begin();
  _waysItr =  _map->getWays().begin();
  _relationsItr = _map->getRelationMap().begin();
  _layer = NULL;
  _transform = NULL;
  _status = Status::Invalid;
  _circularError = 15.0;
  _limit = -1;
  _featureCount = 0;
  _streamFeatureCount = 0;
}

OgrReaderInternal::~OgrReaderInternal()
{
  close();

  if (_transform != NULL)
  {
    OGRCoordinateTransformation::DestroyCT(_transform);
  }
}

QStringList OgrReaderInternal::getLayersWithGeometry(QString path) const
{
  QStringList result;
  shared_ptr<OGRDataSource> ds = OgrUtilities::getInstance().openDataSource(path);
  int count = ds->GetLayerCount();
  for (int i = 0; i < count; i++)
  {
    OGRLayer* l = ds->GetLayer(i);
    if (l->GetGeomType() != wkbNone)
    {
      result.append(l->GetName());
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
      /*
      LOG_DEBUG("Skipping tag w/ key=" << fieldDefn->GetFieldDefn(i)->GetNameRef() <<
        " since the value field is empty");
      */
      continue;
    }

    t[fieldDefn->GetFieldDefn(i)->GetNameRef()] = value;
  }

  _translate(t);

  // Add an ingest datetime tag
  t.appendValue("source:ingest:datetime",
                QDateTime::currentDateTimeUtc().toString("yyyy-MM-ddThh:mm:ss.zzzZ"));

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
          //LOG_DEBUG("Adding line string");
          _addLineString((OGRLineString*)g, t);
          break;
        case wkbPoint:
          //LOG_DEBUG("Adding point");
          _addPoint((OGRPoint*)g, t);
          break;
        case wkbPolygon:
          //LOG_DEBUG("Adding polygon");
          _addPolygon((OGRPolygon*)g, t);
          break;
        case wkbMultiPolygon:
          //LOG_DEBUG("Adding multi-polygon");
          _addMultiPolygon((OGRMultiPolygon*)g, t);
          break;
        case wkbMultiPoint:
        case wkbMultiLineString:
        case wkbGeometryCollection:
        {
          LOG_DEBUG("Adding geometry collection (multipoint, multiline, etc.)");
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
    catch (IllegalArgumentException& e)
    {
      LOG_WARN("Error projecting geometry with tags: " << t.toString());
      throw e;
    }
  }
}

void OgrReaderInternal::_addLineString(OGRLineString* ls, Tags& t)
{
  Meters circularError = _parseCircularError(t);

  shared_ptr<Way> way(new Way(_status, _map->createNextWayId(), circularError));

  way->setTags(t);
  for (int i = 0; i < ls->getNumPoints(); i++)
  {
    double x = ls->getX(i);
    double y = ls->getY(i);
    _reproject(x, y);
    shared_ptr<Node> n(new Node(_status, _map->createNextNodeId(), x, y, circularError));
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
    shared_ptr<Relation> r(new Relation(_status, _map->createNextRelationId(), circularError,
      Relation::MULTIPOLYGON));
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
  shared_ptr<Node> node(new Node(_status, _map->createNextNodeId(), x, y,
    circularError));

  node->setTags(t);
  _map->addNode(node);
}

void OgrReaderInternal::_addPolygon(OGRPolygon* p, Tags& t)
{
  Meters circularError = _parseCircularError(t);

  if (t.isArea() == false)
  {
    t.setArea(true);
  }

  if (p->getNumInteriorRings() == 0)
  {
    shared_ptr<Way> outer = _createWay(p->getExteriorRing(), circularError);
    outer->setTags(t);
    _map->addWay(outer);
  }
  else
  {
    shared_ptr<Relation> r(new Relation(_status, _map->createNextRelationId(), circularError,
      Relation::MULTIPOLYGON));
    r->setTags(t);
    _addPolygon(p, r, circularError);
    _map->addRelation(r);
  }
}

void OgrReaderInternal::_addPolygon(OGRPolygon* p, shared_ptr<Relation> r, Meters circularError)
{
  shared_ptr<Way> outer = _createWay(p->getExteriorRing(), circularError);
  _map->addWay(outer);
  r->addElement(Relation::OUTER, outer);

  for (int i = 0; i < p->getNumInteriorRings(); i++)
  {
    shared_ptr<Way> inner = _createWay(p->getInteriorRing(i), circularError);
    _map->addWay(inner);
    r->addElement(Relation::INNER, inner);
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

shared_ptr<Way> OgrReaderInternal::_createWay(OGRLinearRing* lr, Meters circularError)
{
  shared_ptr<Way> way(new Way(_status, _map->createNextWayId(), circularError));

  // make sure the ring is closed
  lr->closeRings();

  // go through all the points except the last one (duplicate of the first)
  for (int i = 0; i < lr->getNumPoints() - 1; i++)
  {
    double x = lr->getX(i);
    double y = lr->getY(i);
    _reproject(x, y);
    shared_ptr<Node> n(new Node(_status, _map->createNextNodeId(), x, y, circularError));
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

void OgrReaderInternal::_initTranslate()
{
  if (_translatePath != "" && _translator.get() == 0)
  {
    // Nice and short. Taken from TranslatedTagDifferencer
    _translator.reset(ScriptTranslatorFactory::getInstance().createTranslator(_translatePath));

    if (_translator.get() == 0)
    {
      throw HootException("Unable to find a valid translation format for: " + _translatePath);
    }
  }
}

void OgrReaderInternal::open(QString path, QString layer)
{
  _initTranslate();

  _path = path;
  _dataSource = OgrUtilities::getInstance().openDataSource(path);
  if (layer.isEmpty() == false)
  {
    _pendingLayers.append(layer);
  }
  else
  {
    _pendingLayers = getLayersWithGeometry(path);
  }
}

void OgrReaderInternal::_openLayer(QString path, QString layer)
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

  QString bboxStr = ConfigOptions().getOgrReaderBoundingBox();
  if (bboxStr.isEmpty() == false)
  {
    QStringList bbox = bboxStr.split(",");

    if (bbox.size() != 4)
    {
      LOG_INFO(ConfigOptions().getOgrReaderBoundingBoxDescription());
      throw HootException("Error parsing " + ConfigOptions().getOgrReaderBoundingBoxKey() + " " +
        bboxStr);
    }

    bool ok;
    vector<double> bboxValues(4);
    for (size_t i = 0; i < 4; i++)
    {
      bboxValues[i] = bbox[i].toDouble(&ok);
      if (!ok)
      {
        LOG_INFO(ConfigOptions().getOgrReaderBoundingBoxDescription());
        throw HootException("Error parsing " + ConfigOptions().getOgrReaderBoundingBoxKey() + " " +
          bboxStr);
      }
    }

    _layer->SetSpatialFilterRect(bboxValues[0], bboxValues[1], bboxValues[2], bboxValues[3]);
    LOG_DEBUG("Setting spatial filter on " << layer << " to: " << bboxValues);
  }

  auto_ptr<OGRSpatialReference> tmpSourceSrs;
  OGRSpatialReference* sourceSrs;

  int epsgOverride = ConfigOptions().getOgrReaderEpsgOverride();
  if (epsgOverride >= 0)
  {
    tmpSourceSrs.reset(new OGRSpatialReference());
    sourceSrs = tmpSourceSrs.get();

    if (sourceSrs->importFromEPSG(epsgOverride) != OGRERR_NONE)
    {
      throw HootException(QString("Error creating EPSG:%1 projection.").arg(epsgOverride));
    }
  }
  else
  {
    sourceSrs = _layer->GetSpatialRef();

    // proj4 requires some extra parameters to handle Google map style projections. Check for this
    // situation for known EPSGs and warn/fix the issue.
    tmpSourceSrs.reset(new OGRSpatialReference());
    tmpSourceSrs->importFromEPSG(3785);
    if (sourceSrs && tmpSourceSrs->IsSame(sourceSrs) &&
      _toWkt(tmpSourceSrs.get()) != _toWkt(sourceSrs))
    {
      LOG_WARN("Overriding input projection with proj4 compatible EPSG:3785. See this for details: https://trac.osgeo.org/proj/wiki/FAQ#ChangingEllipsoidWhycantIconvertfromWGS84toGoogleEarthVirtualGlobeMercator");
      sourceSrs = tmpSourceSrs.get();
    }
    else
    {
      tmpSourceSrs->importFromEPSG(900913);
      if (sourceSrs && tmpSourceSrs->IsSame(sourceSrs) &&
        _toWkt(tmpSourceSrs.get()) != _toWkt(sourceSrs))
      {
        LOG_WARN("Overriding input projection with proj4 compatible EPSG:900913. See this for details: https://trac.osgeo.org/proj/wiki/FAQ#ChangingEllipsoidWhycantIconvertfromWGS84toGoogleEarthVirtualGlobeMercator");
        sourceSrs = tmpSourceSrs.get();
      }
    }
  }

  if (sourceSrs != 0 && sourceSrs->IsProjected())
  {
    LOG_DEBUG("Input SRS: " << _toWkt(sourceSrs));
    _wgs84.reset(new OGRSpatialReference());
    if (_wgs84->SetWellKnownGeogCS("WGS84") != OGRERR_NONE)
    {
      throw HootException("Error creating EPSG:4326 projection.");
    }

    _transform = OGRCreateCoordinateTransformation(sourceSrs, _wgs84.get());

    if (_transform == 0)
    {
      throw HootException(QString("Error creating transformation object: ") + CPLGetLastErrorMsg());
    }
  }

  // retrieve the feature count for current layer
  _featureCount = _layer->GetFeatureCount(false);
}

void OgrReaderInternal::_openNextLayer()
{
  _layer = NULL;

  if (_pendingLayers.isEmpty() == false)
  {
    LOG_DEBUG("Opening layer " + _pendingLayers.front());
    _openLayer(_path, _pendingLayers.front());
    _pendingLayers.pop_front();
  }
}

Meters OgrReaderInternal::_parseCircularError(Tags& t)
{
  Meters circularError = _circularError;

  // parse the circularError out of the tags.
  if (t.contains("error:circular"))
  {
    bool ok;
    double a = t["error:circular"].toDouble(&ok);
    if (ok)
    {
      circularError = a;
      t.remove("error:circular");
    }
  }
  else if (t.contains("accuracy"))
  {
    bool ok;
    double a = t["accuracy"].toDouble(&ok);
    if (ok)
    {
      circularError = a;
      t.remove("accuracy");
    }
  }

  return circularError;
}

void OgrReaderInternal::read(shared_ptr<OsmMap> map, Progress progress)
{
  _map = map;
  _count = 0;

  _openNextLayer();

  if (_layer == NULL)
  {
    throw HootException("Error reading from input. No valid layers. Did you forget to set the "
      "layer name?");
  }

  OGRFeature* f;
  while ((f = _layer->GetNextFeature()) != NULL && (_limit == -1 || _count < _limit))
  {
    _addFeature(f);
    OGRFeature::DestroyFeature(f);
    f = 0;
    if (_count % 1000 == 0 && Log::getInstance().isInfoEnabled())
    {
      cout << "Loading " << _path.toUtf8().data() << " " << _layerName.toAscii().data() << " " <<
              _count << " / " << _featureCount << "   \r";
      cout.flush();
    }
    _count++;
    if(progress.getState() != "Pending")
    {
      progress.setFromRelative((double)_count / (double)_featureCount, "Running", false, "Reading ogr features" );
    }
  }
  if (Log::getInstance().isInfoEnabled())
  {
    cout << endl;
  }
}

void OgrReaderInternal::readNext(const shared_ptr<OsmMap>& map)
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
      LOG_WARN("Source x: " << inx);
      LOG_WARN("Source y: " << iny);
      LOG_WARN("Target x: " << x);
      LOG_WARN("Target y: " << y);
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
    _translator->translateToOsm(t, _layer->GetLayerDefn()->GetName());
  }
}

void OgrReaderInternal::initializePartial()
{
  _map.reset(new OsmMap());
  _nodesItr = _map->getNodeMap().begin();
  _waysItr =  _map->getWays().begin();
  _relationsItr = _map->getRelationMap().begin();

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
  if ( (_nodesItr != _map->getNodeMap().end()) || (_waysItr != _map->getWays().end())
      || (_relationsItr != _map->getRelationMap().end()) )
  {
    return true;
  }

  // Let's try a read and see if that put anything into the map

  // Do a read if the element maps are empty
  populateElementMap();

  bool result = ( (_nodesItr != _map->getNodeMap().end()) || (_waysItr != _map->getWays().end())
      || (_relationsItr != _map->getRelationMap().end()) );

  return result;
}

ElementPtr OgrReaderInternal::readNextElement()
{
  //LOG_DEBUG("Inside OGR::readNextElement");

  if ( (_nodesItr == _map->getNodeMap().end()) && (_waysItr == _map->getWays().end())
      && (_relationsItr == _map->getRelationMap().end()) )
  {
    // Load the next OGR feature, with 1..N elemenents per feature, into the map of the various
    //    element types
    populateElementMap();
  }

  ElementPtr returnElement;
  if ( _nodesItr != _map->getNodeMap().end() )
  {
    returnElement.reset(new Node(*_nodesItr.value()));
    _nodesItr++;
  }
  else if ( _waysItr != _map->getWays().end() )
  {
    returnElement.reset(new Way(*_waysItr->second.get()));
    _waysItr++;
  }
  else
  {
    returnElement.reset(new Relation(*_relationsItr->second.get()));
    _relationsItr++;
  }

  return returnElement;
}

void OgrReaderInternal::populateElementMap()
{
  _map->clear();

  readNext(_map);

  _nodesItr = _map->getNodeMap().begin();
  _waysItr =  _map->getWays().begin();
  _relationsItr = _map->getRelationMap().begin();

  _streamFeatureCount++;
}

boost::shared_ptr<OGRSpatialReference> OgrReaderInternal::getProjection() const
{
  boost::shared_ptr<OGRSpatialReference> wgs84(new OGRSpatialReference());
  if (wgs84->SetWellKnownGeogCS("WGS84") != OGRERR_NONE)
  {
    throw HootException("Error creating EPSG:4326 projection.");
  }

  return wgs84;
}

Progress OgrReaderInternal::streamGetProgress() const
{
  Progress streamProgress("OGRReader");

  const float floatCount = _streamFeatureCount;
  const float percentComplete = floatCount / _featureCount * 100;
  streamProgress.setPercentComplete( percentComplete );
  //LOG_DEBUG("Percent complete set to " << percentComplete );

  return streamProgress;
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
