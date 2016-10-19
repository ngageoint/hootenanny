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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include <hoot/core/HootConfig.h>

// Boost
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#if HOOT_HAVE_BOOST_PROPERTY_MAP_PROPERTY_MAP_HPP
# include <boost/property_map/property_map.hpp>
#else
# error "Boost properties include not found during configure."
#endif
using namespace boost;

// Hoot
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/OsmSchemaLoaderFactory.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QDomDocument>
#include <QHash>
#include <QSet>

// Standard
#include <iostream>
#include <fstream>
using namespace std;

// TGS
#include <tgs/HashMap.h>

#ifdef BOOST_GRAPH_NO_BUNDLED_PROPERTIES
#error Bundle support is required.
#endif

namespace hoot
{

QString OsmSchema::_layerNameKey = "hoot:layername";

typedef boost::adjacency_list<
  // Use listS for storing VertexList -- faster, but not as space efficient (no biggie)
  boost::listS,
  // use vecS for storing OutEdgeList -- faster for traversal, but slower to build (no biggie)
  boost::vecS,
  // Our graph is directed and we don't need to go backwards
  boost::directedS,
  SchemaVertex,
  TagEdge
> TagGraph;

typedef graph_traits <TagGraph>::vertex_descriptor VertexId;
typedef graph_traits <TagGraph>::edge_descriptor EdgeId;

SchemaVertex empty;

struct AverageKey
{
  AverageKey(VertexId vid1, double w1, VertexId vid2, double w2)
  {
    this->vid1 = vid1;
    this->w1 = w1;
    this->vid2 = vid2;
    this->w2 = w2;
  }

  VertexId vid1;
  double w1;
  VertexId vid2;
  double w2;
};

bool operator==(const AverageKey& k1, const AverageKey& k2)
{
  return (k1.vid1 == k2.vid1 &&
          k1.vid2 == k2.vid2 &&
          k1.w1 == k2.w1 &&
          k1.w2 == k2.w2);
}


}

namespace __gnu_cxx
{
template<>
  struct hash<hoot::AverageKey>
  {
    size_t operator()(const hoot::AverageKey& k) const
    {
      size_t h1 = hash< pair<hoot::VertexId, hoot::VertexId> >()(
            pair<hoot::VertexId, hoot::VertexId>(k.vid1, k.vid2));
      size_t h2 = fastHashDouble()(k.w1);
      size_t h3 = fastHashDouble()(k.w2);
      return Tgs::cantorPairing(h1, Tgs::cantorPairing(h2, h3));
    }
  };
}

namespace hoot
{

struct AverageResult
{
  AverageResult() {}

  AverageResult(VertexId vid, double score)
  {
    this->vid = vid;
    this->score = score;
  }

  VertexId vid;
  double score;
};

class SimilarToOnly
{
public:

  SimilarToOnly() : _graph(0) {}

  SimilarToOnly(TagGraph& graph) : _graph(&graph) {}

  bool operator()(const EdgeId& edge_id) const
  {
    EdgeType type = (*_graph)[edge_id].type;
    return (type == SimilarTo);
  }

private:
  TagGraph* _graph;
};

class VertexNameComparator
{
public:
  VertexNameComparator(const TagGraph& graph) : _graph(graph) {}

  bool operator()(VertexId v1, VertexId v2)
  {
    return _graph[v1].name < _graph[v2].name;
  }

private:
  const TagGraph _graph;
};

OsmSchemaCategory OsmSchemaCategory::fromStringList(const QStringList &s)
{
  OsmSchemaCategory result;
  for (int i = 0; i < s.size(); i++)
  {
    result = fromString(s[i]) | result;
  }

  return result;
}

uint16_t OsmGeometries::fromString(const QString& s)
{
  uint16_t result = 0;

  // breaking coding convention for the sake of readability/brevity.
  if (s == "node") result = Node;
  else if (s == "area") result = Area;
  else if (s == "linestring") result = LineString;
  else if (s == "closedway") result = ClosedWay;
  else if (s == "way") result = Way;
  else if (s == "relation") result = Relation;
  else
  {
    throw HootException("Unexpected enumerated type when parsing OsmGeometries: " + s);
  }

  return result;
}

// a small value.
const double epsilon = 1e-6;

/**
 * This class is implemented internally to avoid including the very expensive boost graph code in
 * the numerous files that use OsmSchema.
 */
class OsmSchemaData
{
public:

  OsmSchemaData()
  {
    _isACost = 1.0;
  }

  EdgeId addIsA(QString name1, QString name2)
  {
    TagEdge isA;
    isA.similarToWeight = 1;
    isA.averageWeight = epsilon;
    isA.type = IsA;

    VertexId vid1 = createOrGetVertex(name1);
    VertexId vid2 = createOrGetVertex(name2);

    EdgeId result = add_edge(vid1, vid2, isA, _graph).first;

    _parents[vid1] = vid2;

    return result;
  }

  pair<EdgeId, EdgeId> addSimilarTo(QString name1, QString name2, double weight,
    bool oneway = false)
  {
    TagEdge similarTo;
    similarTo.similarToWeight = weight;
    similarTo.averageWeight = similarTo.similarToWeight;
    similarTo.type = SimilarTo;

    VertexId vid1 = createOrGetVertex(name1);
    VertexId vid2 = createOrGetVertex(name2);

    EdgeId e1 = add_edge(vid1, vid2, similarTo, _graph).first;
    EdgeId e2;
    if (oneway == false)
    {
      e2 = add_edge(vid2, vid1, similarTo, _graph).first;
    }
    return pair<EdgeId, EdgeId>(e1, e2);
  }

  pair<EdgeId, EdgeId> addAssociatedWith(QString name1, QString name2)
  {
    TagEdge associatedWith;
    associatedWith.similarToWeight = 0;
    associatedWith.averageWeight = 0;
    associatedWith.type = AssociatedWith;

    VertexId vid1 = createOrGetVertex(name1);
    VertexId vid2 = createOrGetVertex(name2);

    EdgeId e1 = add_edge(vid1, vid2, associatedWith, _graph).first;
    EdgeId e2 = add_edge(vid2, vid1, associatedWith, _graph).first;
    return pair<EdgeId, EdgeId>(e1, e2);
  }


  QString average(QString kvp1, double w1, QString kvp2, double w2, double& score)
  {
    QString result;
    QString kvpNormalized1 = normalizeEnumeratedKvp(kvp1);
    QString kvpNormalized2 = normalizeEnumeratedKvp(kvp2);
    if (kvpNormalized1.isEmpty() || kvpNormalized2.isEmpty())
    {
      score = 0.0;
      result = kvp1;
    }
    else if (kvpNormalized2.endsWith("*"))
    {
      result = kvp1;
    }
    else if (kvpNormalized1.endsWith("*"))
    {
      result = kvp2;
    }
    else
    {
      VertexId vid1 = _name2Vertex[kvpNormalized1];
      VertexId vid2 = _name2Vertex[kvpNormalized2];

      AverageKey key(vid1, w1, vid2, w2);
      HashMap<AverageKey, AverageResult>::iterator it = _cachedAverages.find(key);
      if (it != _cachedAverages.end())
      {
        score = it->second.score;
        result = _graph[it->second.vid].name;
      }
      else
      {
        VertexId avgVid = _calculateAverage(vid1, w1, vid2, w2, score);

        _cachedAverages[key] = AverageResult(avgVid, score);

        result = _graph[avgVid].name;
      }
    }

    return result;
  }

  void createTestingGraph()
  {
    SchemaVertex v;
    v.setType(SchemaVertex::Tag);
    v.influence = 1;
    v.key = "highway";
    v.name = "highway=road";
    v.valueType = Enumeration;
    v.value = "road";
    v.categories.append("transportation");
    _addVertex(v);

    TagEdge isA;
    isA.similarToWeight = 1;
    isA.averageWeight = epsilon;
    isA.type = IsA;

    v.name = "highway=primary";
    v.value = "primary";
    VertexId highwayPrimary = _addVertex(v);
    addIsA("highway=primary", "highway=road");

    v.name = "highway=secondary";
    v.value = "secondary";
    VertexId highwaySecondary = _addVertex(v);
    addIsA("highway=secondary", "highway=road");

    v.name = "highway=residential";
    v.value = "residential";
    VertexId highwayResidential = _addVertex(v);
    addIsA("highway=residential", "highway=road");

    v.name = "highway=service";
    v.value = "service";
    VertexId highwayService = _addVertex(v);
    addIsA("highway=service", "highway=road");

    TagEdge similarTo;
    similarTo.similarToWeight = 0.8;
    similarTo.averageWeight = similarTo.similarToWeight;
    similarTo.type = SimilarTo;

    add_edge(highwayPrimary, highwaySecondary, similarTo, _graph);
    add_edge(highwaySecondary, highwayPrimary, similarTo, _graph);

    similarTo.similarToWeight = 0.64;
    similarTo.averageWeight = similarTo.similarToWeight;
    add_edge(highwaySecondary, highwayResidential, similarTo, _graph);
    add_edge(highwayResidential, highwaySecondary, similarTo, _graph);

    similarTo.similarToWeight = 0.8;
    similarTo.averageWeight = similarTo.similarToWeight;
    add_edge(highwayResidential, highwayService, similarTo, _graph);
    add_edge(highwayService, highwayResidential, similarTo, _graph);

    // create some text types.
    v.influence = 2.0;
    v.key = "abstract_name";
    v.name = "abstract_name";
    v.value.clear();
    v.valueType = Text;
    v.categories = QStringList("name");
    _addVertex(v);

    v.key = "name";
    v.name = "name";
    _addVertex(v);
    addIsA("name", "abstract_name");

    ////
    // create a match all type
    ////
    v.key = "poi";
    v.name = "poi";
    v.valueType = Enumeration;
    v.geometries = OsmGeometries::Node | OsmGeometries::Area;
    v.categories = QStringList();
    _addVertex(v);

    v.key = "poi";
    v.value = "yes";
    v.name = "poi=yes";
    v.categories = QStringList("poi");
    VertexId poiYes = _addVertex(v);
    addIsA("poi=yes", "poi");

    v.key = "leisure";
    v.value = "";
    v.name = "leisure";
    VertexId leisure = _addVertex(v);
    add_edge(leisure, poiYes, isA, _graph);
    addIsA("leisure", "poi=yes");

    v.key = "leisure";
    v.value = "*";
    v.name = "leisure=*";
    v.geometries = OsmGeometries::Node | OsmGeometries::Area;
    _addVertex(v);
    addIsA("leisure=*", "poi=yes");

    v.key = "leisure";
    v.value = "track";
    v.name = "leisure=track";
    v.geometries = OsmGeometries::Node | OsmGeometries::Way;
    _addVertex(v);
    addIsA("leisure=track", "leisure=*");
  }

  VertexId createOrGetVertex(const QString& str)
  {
    VertexId vid;
    if (_name2Vertex.contains(str))
    {
      vid = _name2Vertex[str];
    }
    else
    {
      SchemaVertex v;
      v.influence = -1;
      v.name = str;
      v.valueType = Unknown;

      vid = _addVertex(v);
    }
    return vid;
  }

  vector<SchemaVertex> getAllTags()
  {
    vector<SchemaVertex> result;

    result.reserve(_name2Vertex.size());
    for (QHash<QString, VertexId>::const_iterator it = _name2Vertex.begin();
      it != _name2Vertex.end(); ++it)
    {
      result.push_back(_graph[it.value()]);
    }

    return result;
  }

  QSet<QString> getAllTagKeys()
  {
    QSet<QString> result;
    result.reserve(_name2Vertex.size());
    for (QHash<QString, VertexId>::const_iterator it = _name2Vertex.begin();
      it != _name2Vertex.end(); ++it)
    {
      result.insert(it.key());
    }\
    return result;
  }

  vector<SchemaVertex> getAssociatedTags(QString name)
  {
    set<VertexId> vids;
    _getAssociatedTags(_name2Vertex[name], vids);

    vector<SchemaVertex> result;

    for (set<VertexId>::iterator it = vids.begin(); it != vids.end(); it++)
    {
      result.push_back(_graph[*it]);
    }

    return result;
  }

  vector<SchemaVertex> getChildTags(QString name)
  {
    set<VertexId> vids;
    _getChildTags(_name2Vertex[name], vids);

    vector<SchemaVertex> result;

    for (set<VertexId>::iterator it = vids.begin(); it != vids.end(); it++)
    {
      result.push_back(_graph[*it]);
    }

    return result;
  }

  const SchemaVertex& getFirstCommonAncestor(const QString& kvp1, const QString& kvp2)
  {
    if (!_name2Vertex.contains(kvp1) || !_name2Vertex.contains(kvp1))
    {
      return empty;
    }
    VertexId v1 = _name2Vertex[kvp1];
    VertexId v2 = _name2Vertex[kvp2];
    VertexId v1Ancestor = v1;

    while (_isValid(v1Ancestor))
    {
      if (v1Ancestor == v2 || _isAncestor(v2, v1Ancestor))
      {
        return _graph[v1Ancestor];
      }
      v1Ancestor = _getParent(v1Ancestor);
    }

    return empty;
  }

  double getIsACost() const
  {
    return _isACost;
  }

  QString getKey(const QString& kvp) const
  {
    // this is faster than using split and it gets called a lot.
    QString result;
    int index = kvp.indexOf('=');
    if (index == -1)
    {
      result = kvp;
    }
    else
    {
      result = kvp.left(index);
    }

    return result;
  }

  vector<SchemaVertex> getSchemaVertices(const Tags& tags) const
  {
    vector<SchemaVertex> result;

    vector<VertexId> vids = _getVertexIds(tags);

    // go through each compound vertex found and evaluate it for match against tags.
    for (size_t i = 0; i < vids.size(); ++i)
    {
      result.push_back(_graph[vids[i]]);
    }

    return result;
  }

  vector<SchemaVertex> getSimilarTags(QString kvp1, double minimumScore)
  {
    vector<SchemaVertex> result;
    QString kvpn1 = normalizeEnumeratedKvp(kvp1);

    if (kvpn1.isEmpty() == false)
    {
      VertexId id1 = _name2Vertex[kvpn1];

      if (_processed.find(id1) == _processed.end())
      {
        _calculateScores(id1);
        _processed.insert(id1);
      }

      if (_vertexToScoresCache.find(id1) != _vertexToScoresCache.end())
      {
        const vector< pair< VertexId, double > >& similars = _vertexToScoresCache[id1];

        for (size_t i = 0; i < similars.size(); i++)
        {
          if (similars[i].second >= minimumScore)
          {
            result.push_back(_graph[similars[i].first]);
          }
        }
      }
    }

    return result;
  }

  vector<SchemaVertex> getTagByCategory(OsmSchemaCategory c)
  {
    vector<SchemaVertex> result;

    graph_traits < TagGraph >::vertex_iterator vi, vend;
    for (boost::tie(vi, vend) = vertices(_graph); vi != vend; ++vi)
    {
      const SchemaVertex& tv = _graph[*vi];
      if (OsmSchemaCategory::fromStringList(tv.categories).contains(c))
      {
        result.push_back(tv);
      }
    }

    return result;
  }

  const SchemaVertex& getTagVertex(const QString& kvp) const
  {
    QString n = normalizeKvp(kvp);
    if (_name2Vertex.contains(n))
    {
      const SchemaVertex& v = _graph[_name2Vertex[n]];
      if (v.getType() == SchemaVertex::Tag)
      {
        return v;
      }
    }

    return empty;
  }

  vector<SchemaVertex> getUniqueSchemaVertices(const Tags& tags) const
  {
    vector<SchemaVertex> result;

    vector<VertexId> vids = _getVertexIds(tags);
    vids = _removeAncestorVertices(vids);

    // go through each compound vertex found and evaluate it for match against tags.
    for (size_t i = 0; i < vids.size(); ++i)
    {
      result.push_back(_graph[vids[i]]);
    }

    return result;
  }

  bool isAncestor(const QString& childKvp, const QString& parentKvp)
  {
    bool result = false;
    QString nChildKvp = normalizeKvp(childKvp);
    QString nParentKvp = normalizeKvp(parentKvp);

    if (_name2Vertex.contains(nChildKvp) && _name2Vertex.contains(nParentKvp))
    {
      // cache the results to speed things up.
      VertexId childVid = _name2Vertex[nChildKvp];
      VertexId parentVid = _name2Vertex[nParentKvp];
      pair<VertexId, VertexId> key(childVid, parentVid);
      HashMap< pair<VertexId, VertexId>, bool >::const_iterator it = _isAncestorCache.find(key);
      if (it == _isAncestorCache.end())
      {
        result = _isAncestor(_name2Vertex[nChildKvp], _name2Vertex[nParentKvp]);
        _isAncestorCache[key] = result;
      }
      else
      {
        result = it->second;
      }
    }
    return result;
  }

  /**
   * Normalizes enumerated names. If the name exists as stated. (e.g. highway=road), then that
   * kvp is returned. However, if the kvp doesn't exist, but there is an enumerated kvp that =*
   * then that is substituted. E.g. addr:housenumber=123 would be converted to addr:housenumber=*.
   * If neither of those situations matches then an empty string is returned.
   */
  QString normalizeEnumeratedKvp(const QString& kvp)
  {
    QString result = _normalizeEnumeratedKvp(kvp);
    if (result.isEmpty())
    {
      result = _normalizeEnumeratedKvp(kvp.toLower());
    }

    return result;
  }

  QString normalizeKvp(const QString& kvp) const
  {
    QString result = _normalizeKvp(kvp);
    if (result.isEmpty())
    {
      result = _normalizeKvp(kvp.toLower());
    }

    return result;
  }

  double score(const QString& kvp1, const QString& kvp2)
  {
    double result = 0.0;

    QString kvpn1 = normalizeEnumeratedKvp(kvp1);
    QString kvpn2 = normalizeEnumeratedKvp(kvp2);

    if (kvpn1.isEmpty() == false && kvpn2.isEmpty() == false)
    {
      VertexId id1 = _name2Vertex[kvpn1];
      VertexId id2 = _name2Vertex[kvpn2];

      if (_processed.find(id1) == _processed.end())
      {
        _calculateScores(id1);
        _processed.insert(id1);
      }

      pair<VertexId, VertexId> key(id1, id2);

      if (_cachedScores.find(key) != _cachedScores.end())
      {
        result = _cachedScores[key];
      }
      else
      {
        result = 0.0;
      }

      if (id1 == id2 && kvp1 != kvp2)
      {
        // if this is a enumerated wild card match, but the values are different then use the
        // mismatch score. E.g. addr:housenumber=12 vs. addr:housenumber=56
        if (kvpn1.endsWith("=*"))
        {
          result = getTagVertex(kvpn1).mismatchScore;
        }
        // if this is an alias match
        else
        {
          result = 1.0;
        }
      }
    }

    return result;
  }

  void setIsACost(double cost)
  {
    _isACost = cost;
    graph_traits < TagGraph >::edge_iterator ei, eend;
    for (boost::tie(ei, eend) = edges(_graph); ei != eend; ++ei)
    {
      if (_graph[*ei].type == IsA)
      {
        _graph[*ei].similarToWeight = cost;
      }
    }

    _processed.clear();
    _cachedScores.clear();
    _cachedAverages.clear();
  }

  /**
   * Dump the graph structure out to a file compatible with dot (sudo apt-get install graphviz)
   */
  QString toGraphvizString() const
  {
    QString result;

    result += "digraph structs {\n";

    QList<VertexId> orderedVertexes;
    QMap<size_t, VertexId> vertexIndex;

    graph_traits < TagGraph >::vertex_iterator vi, vend;
    for (boost::tie(vi, vend) = vertices(_graph); vi != vend; ++vi)
    {
      orderedVertexes.push_back(*vi);
    }

    VertexNameComparator vnc(_graph);
    qSort(orderedVertexes.begin(), orderedVertexes.end(), vnc);

    for (int i = 0; i < orderedVertexes.size(); i++)
    {
      VertexId vid = orderedVertexes[i];
      QString label = _graph[vid].name;
      QString vStr = QString("\"%1\"").arg(label);
      vertexIndex[vid] = i;

      label = label.replace("{", "\\}").replace("}", "\\}");
      if (_graph[vid].childWeight > 0)
      {
        label = QString("%1\\nchildWeight = %2").arg(label).arg(_graph[vid].childWeight);
      }
      result += QString("  %1 [shape=record,label=\"%2\"];\n").arg(vStr).arg(label);
    }

    result += "\n";

    set< pair<VertexId, VertexId> > used;
    QStringList orderedEdges;

    graph_traits < TagGraph >::edge_iterator ei, eend;
    for (boost::tie(ei, eend) = edges(_graph); ei != eend; ++ei)
    {
      VertexId src = source(*ei, _graph);
      VertexId trg = target(*ei, _graph);

      if (_graph[trg].name == _graph[src].name)
      {
        throw IllegalArgumentException("Unexpected vertices with the same name. " +
          _graph[src].name);
      }

      if (_graph[trg].name < _graph[src].name)
      {
        swap(src, trg);
      }

      pair<VertexId, VertexId> p(src, trg);

      if (used.find(p) == used.end())
      {
//        QString srcStr = QString("v%1").arg(vertexIndex[src]);
//        QString trgStr = QString("v%1").arg(vertexIndex[trg]);

        QString srcStr = _graph[src].name;
        QString trgStr = _graph[trg].name;

        // only show is a relationships for legit tags. No need w/ things like "amenity" or "poi",
        // it just clutters the graph.
        if (_graph[*ei].type == IsA &&
          (_graph[src].name.contains("=") && _graph[trg].name.contains("=")))
        {
          orderedEdges.push_back(
            QString("\"%1\" -> \"%2\" [arrowhead=normal,color=blue2,weight=1,label=\"%3\"];\n").
            arg(srcStr, trgStr).arg(_graph[*ei].similarToWeight));
        }
        else if (_graph[*ei].type == SimilarTo && _graph[*ei].show)
        {
          orderedEdges.push_back(
            QString("\"%1\" -> \"%2\" [arrowhead=odot,color=chartreuse3,weight=%3,arrowtail=odot,label=\"%4\"];\n").
            arg(srcStr, trgStr).arg(_graph[*ei].similarToWeight).arg(_graph[*ei].similarToWeight));
          used.insert(p);
        }
      }
    }

    qSort(orderedEdges);

    result.append(orderedEdges.join(""));

    result += "}\n";

    return result;
  }

  void updateOrCreateVertex(const SchemaVertex& tv)
  {
    VertexId vid = createOrGetVertex(tv.name);

    const SchemaVertex& v = _graph[vid];
    if (v.isValid())
    {
      LOG_WARN(tv.name << " was specified multiple times in the schema file.");
    }

    _updateVertex(vid, tv);
  }

  void update()
  {
    _percolateInheritance();
  }

private:

  QHash<QString, VertexId> _name2Vertex;
  /**
   * Maps a single tag that makes up a rule in a compound vertex to that vertex. There will likely
   * be multiple entries for a single vertex.
   */
  QMultiHash<QString, VertexId> _name2CompoundVertex;

  double _isACost;
  HashSet<VertexId> _processed;
  HashMap< pair<VertexId, VertexId>, double> _cachedScores;
  HashMap<AverageKey, AverageResult> _cachedAverages;
  HashMap<VertexId, VertexId> _parents;
  QList< pair<QRegExp, VertexId> > _regexKeys;
  HashMap< pair<VertexId, VertexId>, bool > _isAncestorCache;
  typedef HashMap< VertexId, vector< pair< VertexId, double > > > VertexToScoreCache;
  VertexToScoreCache _vertexToScoresCache;

  TagGraph _graph;

  VertexId _addVertex(const SchemaVertex& v)
  {
    VertexId vid = add_vertex(v, _graph);
    _updateVertex(vid, v);
    return vid;
  }

  VertexId _calculateAverage(VertexId vid1, double w1, VertexId vid2, double w2, double& score)
  {
    std::vector<double> d1(num_vertices(_graph));

    if (_isAncestor(vid1, vid2))
    {
      //cout << _graph[vid1].name.toStdString() << " descendant of " << _graph[vid2].name.toStdString() << endl;
      score = 1;
      return vid1;
    }
    else if (_isAncestor(vid2, vid1))
    {
      //cout << _graph[vid2].name.toStdString() << " descendant of " << _graph[vid1].name.toStdString() << endl;
      score = 1;
      return vid2;
    }

    graph_traits < TagGraph >::edge_iterator ei, eend;
    for (boost::tie(ei, eend) = edges(_graph); ei != eend; ++ei)
    {
      _graph[*ei].w1 = pow(_graph[*ei].averageWeight, w1);
      _graph[*ei].w2 = pow(_graph[*ei].averageWeight, w2);
    }

    multiplies<double> combiner;
    greater<double> comparer;
    dijkstra_shortest_paths(_graph, vid1,
                            distance_map(&d1[0]).
                            weight_map(get(&TagEdge::w1, _graph)).
                            distance_zero(1.0).
                            distance_combine(combiner).
                            distance_inf(0.0).
                            distance_compare(comparer));

    std::vector<double> d2(num_vertices(_graph));

    dijkstra_shortest_paths(_graph, vid2,
                            distance_map(&d2[0]).
                            weight_map(get(&TagEdge::w2, _graph)).
                            distance_zero(1.0).
                            distance_combine(combiner).
                            distance_inf(0.0).
                            distance_compare(comparer));

    double bestScore = -1.0;
    VertexId bestVid = vid1;
    graph_traits < TagGraph >::vertex_iterator vi, vend;
    // LOG_DEBUG("from " << _graph[vid1].name << " to " << _graph[vid2].name);
    for (boost::tie(vi, vend) = vertices(_graph); vi != vend; ++vi)
    {
      // The best minimum score is generally the average.
      // give a very slight advantage to the tags with a higher max score.
      // give a very slight advantage to the first input.
      double s = std::min(d1[*vi], d2[*vi] + 1e-6) +
        std::max(d1[*vi], d2[*vi]) / 1e6;
      //if (s > 0)
      // LOG_DEBUG("  " << _graph[*vi].name << " : " << d1[*vi] << " " << d2[*vi] <<
      // " (" << s << ")");

      if (s > bestScore)
      {
        bestScore = s;
        bestVid = *vi;
      }
    }

//    LOG_DEBUG("  best vid: " << bestVid << " " << _graph[bestVid].name.toStdString());

    score = bestScore;
    return bestVid;
  }

  void _calculateScores(VertexId vd)
  {
    std::vector<double> d(num_vertices(_graph));

    multiplies<double> combiner;
    greater<double> comparer;
    dijkstra_shortest_paths(_graph, vd,
                            distance_map(&d[0]).
                            weight_map(get(&TagEdge::similarToWeight, _graph)).
                            distance_zero(1.0).
                            distance_combine(combiner).
                            distance_inf(0.0).
                            distance_compare(comparer));

    graph_traits < TagGraph >::vertex_iterator vi, vend;
    //LOG_DEBUG("Scores for: " << _graph[vd].name.toStdString());
    for (boost::tie(vi, vend) = vertices(_graph); vi != vend; ++vi)
    {
      pair<VertexId, VertexId> key = pair<VertexId, VertexId>(vd, *vi);
      _cachedScores[key] = d[*vi];
      //LOG_DEBUG("  " << _graph[*vi].name.toStdString() << " : " << d[*vi]);

      // cache the score between vd and vi in another structure that is more efficient for other
      // query types.
      if (d[*vi] > 0.0)
      {
        _vertexToScoresCache[vd].push_back(pair<VertexId, double>(*vi, d[*vi]));
      }
    }
  }

  void _findPotentialCompoundTags(VertexId vid, set<VertexId>& compoundTags) const
  {
    const SchemaVertex& sv = _graph[vid];

    QMultiHash<QString, VertexId>::const_iterator it = _name2CompoundVertex.find(sv.name);
    while (it != _name2CompoundVertex.end() && it.key() == sv.name)
    {
      compoundTags.insert(it.value());
      ++it;
    }
  }

  void _getAssociatedTags(VertexId from, set<VertexId>& result)
  {
    graph_traits < TagGraph >::edge_iterator ei, eend;
    for (boost::tie(ei, eend) = edges(_graph); ei != eend; ++ei)
    {
      VertexId thisFrom = source(*ei, _graph);
      VertexId thisTo = target(*ei, _graph);
      if (_graph[*ei].type == AssociatedWith && thisFrom == from &&
          result.find(thisTo) == result.end())
      {
        result.insert(thisTo);
      }
    }
  }

  void _getChildTags(VertexId parent, set<VertexId>& result)
  {
    graph_traits < TagGraph >::edge_iterator ei, eend;
    for (boost::tie(ei, eend) = edges(_graph); ei != eend; ++ei)
    {
      VertexId thisChild = source(*ei, _graph);
      VertexId thisParent = target(*ei, _graph);
      if (_graph[*ei].type == IsA && thisParent == parent && result.find(thisChild) == result.end())
      {
        result.insert(thisChild);
        _getChildTags(thisChild, result);
      }
    }
  }

  VertexId _getParent(VertexId child) const
  {
    VertexId result = numeric_limits<VertexId>::max();

    HashMap<VertexId, VertexId>::const_iterator it = _parents.find(child);
    if (it != _parents.end())
    {
      result = it->second;
    }

    return result;
  }

  vector<VertexId> _getVertexIds(const Tags& tags) const
  {
    vector<VertexId> result;

    set<VertexId> compoundTags;

    // go through each of the tags and look for the tag vertex.
    for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
    {
      QString n = normalizeKvp(OsmSchema::toKvp(it.key(), it.value()));

      // find each compound vertex linked from a tag vertex and put into a set.
      if (_name2Vertex.contains(n))
      {
        VertexId vid = _name2Vertex[n];

        _findPotentialCompoundTags(vid, compoundTags);

        result.push_back(vid);
      }
    }

    // go through each compound vertex found and evaluate it for match against tags.
    for (set<VertexId>::const_iterator it = compoundTags.begin(); it != compoundTags.end(); ++it)
    {
      VertexId compoundTagId = *it;

      const SchemaVertex& sv = _graph[compoundTagId];

      if (sv.isCompoundMatch(tags))
      {
        result.push_back(compoundTagId);
      }
    }

    return result;
  }

  /**
   * Returns true if ancestor is a director or indirect parent of child using the isA tag.
   */
  bool _isAncestor(VertexId child, VertexId ancestor) const
  {
    VertexId parent = _getParent(child);

    if (_isValid(parent) == false)
    {
      return false;
    }
    else if (parent == ancestor)
    {
      return true;
    }
    else
    {
      return _isAncestor(parent, ancestor);
    }
  }

  bool _isValid(VertexId vid) const
  {
    return vid != numeric_limits<VertexId>::max();
  }

  QString _normalizeEnumeratedKvp(const QString& kvp)
  {
    static QString equalStar = "=*";
    if (_name2Vertex.contains(kvp))
    {
      return kvp;
    }
    else
    {
      QString newKvp = getKey(kvp) + equalStar;
      if (_name2Vertex.contains(newKvp))
      {
        return newKvp;
      }
      else
      {
        return QString();
      }
    }
  }

  QString _normalizeKvp(const QString& kvp) const
  {
    QString key = getKey(kvp);
    if (_name2Vertex.contains(kvp))
    {
      return kvp;
    }
    else if (_name2Vertex.contains(key + "=*"))
    {
      return getKey(kvp) + "=*";
    }
    else if (_name2Vertex.contains(key))
    {
      const SchemaVertex& v = _graph[_name2Vertex[key]];
      if (v.valueType == Text || v.valueType == Int)
      {
        return key;
      }
      else
      {
        return QString();
      }
    }
    else
    {
      // check to see if any of the regular expressions match
      for (QList< pair<QRegExp, VertexId> >::const_iterator it = _regexKeys.begin();
           it != _regexKeys.end(); ++it)
      {
        const QRegExp& re = it->first;
        if (re.exactMatch(key))
        {
          VertexId vid = it->second;
          return _graph[vid].key;
        }
      }

      return QString();
    }
  }

  void _percolateInheritance()
  {
    graph_traits < TagGraph >::vertex_iterator vi, vend;
    for (boost::tie(vi, vend) = vertices(_graph); vi != vend; ++vi)
    {
      if (_graph[*vi].childWeight > 0.0)
      {
        set<VertexId> children;
        _getChildTags(*vi, children);
        for (set<VertexId>::const_iterator it = children.begin(); it != children.end(); it++)
        {
          pair<EdgeId, EdgeId> p = addSimilarTo(_graph[*it].name, _graph[*vi].name,
                                                _graph[*vi].childWeight);
          _graph[p.first].show = false;
          _graph[p.second].show = false;
        }
      }
    }

    graph_traits < TagGraph >::edge_iterator ei, eend;
    for (boost::tie(ei, eend) = edges(_graph); ei != eend; ++ei)
    {
      if (_graph[*ei].type == IsA)
      {
        VertexId thisChild = source(*ei, _graph);
        _updateInheritance(thisChild);
      }
    }
  }

  vector<VertexId> _removeAncestorVertices(const vector<VertexId>& vid) const
  {
    vector<VertexId> result;

    for (size_t i = 0; i < vid.size(); ++i)
    {
      bool aAncestor = false;
      for (size_t j = 0; j < vid.size() && aAncestor == false; ++j)
      {
        if (i != j && _isAncestor(vid[j], vid[i]))
        {
          aAncestor = true;
        }
      }

      if (!aAncestor)
      {
        result.push_back(vid[i]);
      }
    }

    return result;
  }

  void _updateInheritance(VertexId vid)
  {
    SchemaVertex& childTv = _graph[vid];
    VertexId parent = _getParent(vid);
    if (_isValid(parent))
    {
      _updateInheritance(parent);
      SchemaVertex& parentTv = _graph[parent];

      if (childTv.geometries == 0)
      {
        childTv.geometries = parentTv.geometries;
      }
      if (childTv.influence == -1.0)
      {
        childTv.influence = parentTv.influence;
      }
      if (childTv.valueType == Unknown)
      {
        childTv.valueType = parentTv.valueType;
      }
      if (childTv.mismatchScore == -1.0)
      {
        childTv.mismatchScore = parentTv.mismatchScore;
      }
      // add any categories that are assigned to the parent to the child.
      for (int i = 0; i < parentTv.categories.size(); i++)
      {
        QString c = parentTv.categories[i];
        if (childTv.categories.contains(c) == false)
        {
          childTv.categories.append(c);
        }
      }
    }
    else
    {
      if (childTv.influence == -1.0)
      {
        LOG_WARN("Influence for " << childTv.name << " has not been specified.");
        childTv.influence = 1.0;
      }
      if (childTv.valueType == Unknown)
      {
        LOG_WARN("Value type for " << childTv.name << " has not been specified.");
      }
    }
  }

  VertexId _updateVertex(VertexId vid, const SchemaVertex& v)
  {
    _name2Vertex[v.name] = vid;
    _graph[vid] = v;

    if (v.name.startsWith("regex?"))
    {
      if (v.getType() == SchemaVertex::Compound)
      {
        throw HootException("Compound tags can not have regex names.");
      }
      QRegExp re(v.name.mid(6));
      _regexKeys.append(pair<QRegExp, VertexId>(re, vid));
    }

    // if this is a compound rule then update the link from tags to compound rules.
    if (v.getType() == SchemaVertex::Compound)
    {
      SchemaVertex::CompoundRuleList rules = v.getCompoundRules();
      for (int i = 0; i < rules.size(); ++i)
      {
        SchemaVertex::CompoundRule r = rules[i];
        for (int j = 0; j < r.size(); ++j)
        {
          KeyValuePairPtr p = r[j];
          if (_name2CompoundVertex.contains(p->getName(), vid) == false)
          {
            _name2CompoundVertex.insert(p->getName(), vid);
          }
        }
      }
    }

    // add in the list of aliases.
    for (int i = 0; i < v.aliases.size(); i++)
    {
      if (_name2Vertex.contains(v.aliases[i]))
      {
        throw HootException(QString("Alias is being used multiple times. Please only reference an "
          "alias once or use the base tag. (offending tag: %1, offending alias: %2)").
          arg(v.name).arg(v.aliases[i]));
      }
      _name2Vertex[v.aliases[i]] = vid;
    }

    return vid;
  }

};

OsmSchema* OsmSchema::_theInstance = 0;

OsmSchema::OsmSchema()
{
  d = new OsmSchemaData();
}

OsmSchema::~OsmSchema()
{
  delete d;
}

void OsmSchema::addAssociatedWith(QString name1, QString name2)
{
  d->addAssociatedWith(name1, name2);
}

void OsmSchema::addIsA(QString name1, QString name2)
{
  d->addIsA(name1, name2);
}

void OsmSchema::addSimilarTo(QString name1, QString name2, double weight, bool oneway)
{
  d->addSimilarTo(name1, name2, weight, oneway);
}

QString OsmSchema::average(const QString& kvp1, const QString& kvp2, double& score)
{
  return d->average(kvp1, 1.0, kvp2, 1.0, score);
}

QString OsmSchema::average(const QString& kvp1, double w1, const QString& kvp2, double w2,
                           double& score)
{
  return d->average(kvp1, w1, kvp2, w2, score);
}

void OsmSchema::createTestingGraph()
{
  d->createTestingGraph();
}

vector<SchemaVertex> OsmSchema::getAllTags()
{
  return d->getAllTags();
}

QSet<QString> OsmSchema::getAllTagKeys()
{
  return d->getAllTagKeys();
}

vector<SchemaVertex> OsmSchema::getAssociatedTags(QString name)
{
  return d->getAssociatedTags(name);
}

OsmSchemaCategory OsmSchema::getCategories(const Tags& t) const
{
  OsmSchemaCategory result;

  // if at least one tag has the specified category then return true
  for (Tags::const_iterator it = t.constBegin(); it != t.constEnd(); ++it)
  {
    const SchemaVertex& tv = getTagVertex(toKvp(it.key(), it.value()));
    for (int i = 0; i < tv.categories.size(); i++)
    {
      result = result | OsmSchemaCategory::fromString(tv.categories[i]);
    }
  }

  return result;
}

OsmSchemaCategory OsmSchema::getCategories(const QString& k, const QString& v) const
{
  return getCategories(toKvp(k, v));
}

OsmSchemaCategory OsmSchema::getCategories(const QString& kvp) const
{
  OsmSchemaCategory result;

  const SchemaVertex& tv = getTagVertex(kvp);
  for (int i = 0; i < tv.categories.size(); i++)
  {
    result = result | OsmSchemaCategory::fromString(tv.categories[i]);
  }

  return result;
}

vector<SchemaVertex> OsmSchema::getChildTags(QString name)
{
  return d->getChildTags(name);
}

const SchemaVertex& OsmSchema::getFirstCommonAncestor(const QString& kvp1, const QString& kvp2)
{
  return d->getFirstCommonAncestor(kvp1, kvp2);
}

OsmSchema& OsmSchema::getInstance()
{
  if (_theInstance == 0)
  {
    _theInstance = new OsmSchema();
    _theInstance->loadDefault();
    //LOG_INFO(_theInstance->toGraphvizString());
  }
  return *_theInstance;
}

double OsmSchema::getIsACost() const
{
  return d->getIsACost();
}

vector<SchemaVertex> OsmSchema::getSchemaVertices(const Tags& tags) const
{
  return d->getSchemaVertices(tags);
}

vector<SchemaVertex> OsmSchema::getSimilarTags(QString name, double minimumScore)
{
  if (minimumScore <= 0)
  {
    throw IllegalArgumentException("minimumScore must be > 0");
  }
  return d->getSimilarTags(name, minimumScore);
}

vector<SchemaVertex> OsmSchema::getTagByCategory(OsmSchemaCategory c) const
{
  return d->getTagByCategory(c);
}

const SchemaVertex& OsmSchema::getTagVertex(const QString& kvp) const
{
  return d->getTagVertex(kvp);
}

vector<SchemaVertex> OsmSchema::getUniqueSchemaVertices(const Tags& tags) const
{
  return d->getUniqueSchemaVertices(tags);
}

bool OsmSchema::hasCategory(const Tags& t, const QString& category) const
{
  bool result = false;
  // if at least one tag has the specified category then return true
  for (Tags::const_iterator it = t.constBegin(); it != t.constEnd(); ++it)
  {
    // ignore empty values.
    if (it.value().isEmpty() == false)
    {
      const SchemaVertex& tv = getTagVertex(it.key() + "=" + it.value());
      if (tv.categories.contains(category))
      {
        result = true;
        break;
      }
    }
  }

  return result;
}

bool OsmSchema::hasCategory(const QString& kvp, const QString& category) const
{
  const SchemaVertex& tv = getTagVertex(kvp);
  return tv.categories.contains(category);
}

bool OsmSchema::isAncestor(const QString& childKvp, const QString& parentKvp)
{
  return d->isAncestor(childKvp, parentKvp);
}

bool OsmSchema::isArea(const Tags& t, ElementType type) const
{
  bool result = false;

  // don't process if a node
  if (type == ElementType::Node)
  {
    return false;
  }

  // Print out tags
  //LOG_DEBUG("Tags: " << t.toString() );

  result |= isBuilding(t, type);
  result |= t.isTrue("building:part");
  result |= t.isTrue("area");

  // if at least one of the tags is marked as an area, but not a linestring tag then we consider
  // this to be an area feature.
  for (Tags::const_iterator it = t.constBegin(); it != t.constEnd(); ++it)
  {
    const SchemaVertex& tv = getTagVertex(it.key() + "=" + it.value());
    uint16_t g = tv.geometries;
    if (g & OsmGeometries::Area && !(g & (OsmGeometries::LineString | OsmGeometries::ClosedWay)))
    {
      result = true;
      break;
    }
  }

  return result;
}

bool OsmSchema::isArea(const ConstElementPtr& e) const
{
  return isArea(e->getTags(), e->getElementType());
}

bool OsmSchema::isAreaForStats(const Tags& t, ElementType type) const
{
  bool result = false;

  // don't process if a node
  if (type == ElementType::Node)
  {
    return false;
  }

  // don't process if a building part
  if (t.isTrue("building:part"))
  {
    return false;
  }

  // don't process if a relation
  if (type == ElementType::Relation)
  {
    return false;
  }

  result |= isBuilding(t, type);
  result |= t.isTrue("area");

  // if at least one of the tags is marked as an area, but not a linestring tag then we consider
  // this to be an area feature.
  for (Tags::const_iterator it = t.constBegin(); it != t.constEnd(); ++it)
  {
    const SchemaVertex& tv = getTagVertex(it.key() + "=" + it.value());
    uint16_t g = tv.geometries;
    if (g & OsmGeometries::Area && !(g & (OsmGeometries::LineString | OsmGeometries::ClosedWay)))
    {
      result = true;
      break;
    }
  }

  return result;
}

bool OsmSchema::isAreaForStats(const ConstElementPtr& e) const
{
  return isAreaForStats(e->getTags(), e->getElementType());
}

bool OsmSchema::isBuilding(const Tags& t, ElementType type) const
{
  bool result = false;
  if ((type != ElementType::Node) && (hasCategory(t, "building") == true))
  {
    result = true;
  }

  return result;
}

bool OsmSchema::isBuilding(const ConstElementPtr& e) const
{
  return isBuilding(e->getTags(), e->getElementType());
}

bool OsmSchema::isBuildingPart(const Tags& t, ElementType type) const
{
  bool result = false;
  if (type != ElementType::Node && t.isTrue("building:part"))
  {
    result = true;
  }
  return result;
}

bool OsmSchema::isCollection(const Element& e) const
{
  bool result = false;

  if (e.getElementType() == ElementType::Relation)
  {
    const Relation& r = dynamic_cast<const Relation&>(e);

    // This list could get HUGE.
    if (r.getType() == "waterway" ||
        r.getType() == "network" ||
        r.getType() == "route_master" ||
        r.getType() == "superroute" ||
        r.getType() == "route")
    {
      result = true;
    }
  }

  return result;
}

bool OsmSchema::isHgisPoi(const Element& e)
{
  bool result = false;

  // See ticket #6853 for a definition of a "HGIS POI"
  if (e.getElementType() == ElementType::Node)
  {
    result = hasCategory(e.getTags(), OsmSchemaCategory::hgisPoi().toString());
  }

  return result;
}

bool OsmSchema::isLinearHighway(const Tags& t, ElementType type)
{
  bool result = false;
  Tags::const_iterator it = t.find("highway");

  if ((type == ElementType::Way || type == ElementType::Relation) &&
      it != t.end() && it.value() != "")
  {
    result = true;
  }

  // make sure this isn't an area highway section.
  if (result)
  {
    result = !isArea(t, type);
  }

  /*if (result)
  {
    LOG_DEBUG("isLinearHighway; key: " << it.key());
  }*/

  return result;
}

bool OsmSchema::isLinear(const Element &e)
{
  bool result = false;

  if (e.getElementType() == ElementType::Node)
  {
    return false;
  }

  const Tags& t = e.getTags();

  if (e.getElementType() == ElementType::Relation)
  {
    const Relation& r = dynamic_cast<const Relation&>(e);
    result |= r.getType() == "multilinestring";
    result |= r.getType() == "route";
  }

  for (Tags::const_iterator it = t.constBegin(); it != t.constEnd(); ++it)
  {
    const SchemaVertex& tv = getTagVertex(it.key() + "=" + it.value());
    uint16_t g = tv.geometries;
    if (g & (OsmGeometries::LineString | OsmGeometries::ClosedWay) && !(g & OsmGeometries::Area))
    {
      result = true;
      break;
    }
  }

  return result;
}

bool OsmSchema::isLinearWaterway(const Element& e)
{
  if (e.getElementType() == ElementType::Way || e.getElementType() == ElementType::Relation)
  {
    const Tags& tags = e.getTags();
    for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); ++it)
    {
      if (it.key() == "waterway" || isAncestor(it.key(), "waterway") ||
          (it.key() == "type" && isAncestor("waterway=" + it.value(), "waterway")))
      {
        return true;
      }
    }
  }
  return false;
}

bool OsmSchema::isList(const QString& /*key*/, const QString& value)
{
  return value.contains(';');
}

bool OsmSchema::isMetaData(const QString& key, const QString& /*value*/)
{
  if (key.startsWith("hoot:"))
  {
    return true;
  }

  // for now all metadata tags are text so they're referenced by the key only. If that changes then
  // we'll need some logic here to check if a vertex is a text vertex.
  return isAncestor(key, "metadata");
}

bool OsmSchema::isMultiLineString(const Relation& r) const
{
  return r.getType() == "multilinestring";
}

bool OsmSchema::isOneWay(const Element& e) const
{
  bool result = false;
  QString oneway = e.getTags()["oneway"].toLower();
  if (e.getTags().isTrue("oneway") || oneway == "-1" || oneway == "reverse")
  {
    result = true;
  }
  return result;
}

bool OsmSchema::isPoi(const Element& e)
{
  bool result = false;

  // we consider all point features with a name, or part of the explicitly defined point category
  // to be POIs.
  if (e.getElementType() == ElementType::Node)
  {
    result = hasCategory(e.getTags(), "poi") || e.getTags().getNames().size() > 0;
  }

  return result;
}

bool OsmSchema::isReversed(const Element& e) const
{
  bool result = false;
  QString oneway = e.getTags()["oneway"].toLower();
  if (oneway == "-1" || oneway == "reverse")
  {
    result = true;
  }
  return result;
}

void OsmSchema::loadDefault()
{
  QString path = ConfPath::search("schema.json");

  delete d;
  d = new OsmSchemaData();

  OsmSchemaLoaderFactory::getInstance().createLoader(path)->load(path, *this);
}

double OsmSchema::score(const QString& kvp1, const QString& kvp2)
{
  // I tried using a LruCache here to speed up scoring, but it had a negative impact. :(
  return std::max(d->score(kvp1, kvp2), d->score(kvp2, kvp1));
}

double OsmSchema::score(const SchemaVertex& v1, const SchemaVertex& v2)
{
  return score(v1.name, v2.name);
}

double OsmSchema::scoreOneWay(const QString& kvp1, const QString& kvp2)
{
  return d->score(kvp1, kvp2);
}

void OsmSchema::setIsACost(double cost)
{
  d->setIsACost(cost);
}

QString OsmSchema::toGraphvizString() const
{
  return d->toGraphvizString();
}

QString OsmSchema::toKvp(const QString& key, const QString& value)
{
  if (value.isEmpty())
  {
    return key;
  }
  else
  {
    return key + "=" + value;
  }
}

void OsmSchema::update()
{
  d->update();
}

void OsmSchema::updateOrCreateVertex(const SchemaVertex& tv)
{
  d->updateOrCreateVertex(tv);
}

}
