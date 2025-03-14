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
 * @copyright Copyright (C) 2015-2023 Maxar (http://www.maxar.com/)
 */

// Boost
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/properties.hpp>
#if HOOT_HAVE_BOOST_PROPERTY_MAP_PROPERTY_MAP_HPP
# include <boost/property_map/property_map.hpp>
#else
# error "Boost properties include not found during configure."
#endif

// Hoot
#include <hoot/core/HootConfig.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/OsmSchemaLoader.h>
#include <hoot/core/schema/OsmSchemaLoaderFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QDomDocument>
#include <QStringBuilder>

// Standard
#include <fstream>

// TGS
#include <tgs/HashMap.h>

#ifdef BOOST_GRAPH_NO_BUNDLED_PROPERTIES
#error Bundle support is required.
#endif

/**
 *  Using the boost::graph_traits<>::edge_iterator from boost::edges() and boost::tie can cause compilers
 *  to assume that the iterator isn't initialized and is maybe being used before initialization.  boost::tie()
 *  actually does the initialization and therefore the code is correct.  Ignore the warning for this file only.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

using namespace std;

namespace hoot
{

using TagGraph = boost::adjacency_list<
  // Use listS for storing VertexList -- faster, but not as space efficient (no biggie)
  boost::listS,
  // use vecS for storing OutEdgeList -- faster for traversal, but slower to build (no biggie)
  boost::vecS,
  // Our graph is directed and we don't need to go backwards
  boost::directedS,
  SchemaVertex,
  TagEdge
>;

using VertexId = boost::graph_traits<TagGraph>::vertex_descriptor;
using EdgeId = boost::graph_traits<TagGraph>::edge_descriptor;

const SchemaVertex empty;

struct AverageKey
{
  AverageKey(VertexId vid1_, double w1_, VertexId vid2_, double w2_)
    : vid1(vid1_),
      w1(w1_),
      vid2(vid2_),
      w2(w2_)
  {
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
      size_t h1 = hash<pair<hoot::VertexId, hoot::VertexId>>()(pair<hoot::VertexId, hoot::VertexId>(k.vid1, k.vid2));
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
  AverageResult() = default;

  AverageResult(VertexId vid_, double score_)
    : vid(vid_),
      score(score_)
  {
  }

  VertexId vid;
  double score;
};

class SimilarToOnly
{
public:

  SimilarToOnly() : _graph(nullptr) { }

  SimilarToOnly(const TagGraph& graph) : _graph(&graph) { }

  bool operator()(const EdgeId& edge_id) const
  {
    EdgeType type = (*_graph)[edge_id].type;
    return (type == SimilarTo);
  }

private:

  const TagGraph* _graph;
};

class VertexNameComparator
{
public:

  VertexNameComparator(const TagGraph& graph) : _graph(&graph) { }

  bool operator() (VertexId v1, VertexId v2) const
  {
    return (*_graph)[v1].getName() < (*_graph)[v2].getName();
  }

private:

  const TagGraph* _graph;
};

OsmSchemaCategory OsmSchemaCategory::fromStringList(const QStringList& s)
{
  OsmSchemaCategory result;
  for (const auto& str : s)
    result = fromString(str) | result;

  return result;
}

OsmGeometries::Type OsmGeometries::fromString(const QString& s)
{
  Type result = Empty;

  if (s == "node")            result = Node;
  else if (s == "area")       result = Area;
  else if (s == "linestring") result = LineString;
  else if (s == "closedway")  result = ClosedWay;
  else if (s == "way")        result = Way;
  else if (s == "relation")   result = Relation;
  else
    throw HootException("Unexpected enumerated type when parsing OsmGeometries: " + s);

  LOG_VART(result);
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
    : _isACost(1.0),
      _logWarnCount(0)
  {
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

  pair<EdgeId, EdgeId> addSimilarTo(QString name1, QString name2, double weight, bool oneway = false)
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
      e2 = add_edge(vid2, vid1, similarTo, _graph).first;

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
      result = kvp1;
    else if (kvpNormalized1.endsWith("*"))
      result = kvp2;
    else
    {
      VertexId vid1 = _name2Vertex[kvpNormalized1];
      VertexId vid2 = _name2Vertex[kvpNormalized2];

      AverageKey key(vid1, w1, vid2, w2);
      auto it = _cachedAverages.find(key);
      if (it != _cachedAverages.end())
      {
        score = it->second.score;
        result = _graph[it->second.vid].getName();
      }
      else
      {
        VertexId avgVid = _calculateAverage(vid1, w1, vid2, w2, score);
        _cachedAverages[key] = AverageResult(avgVid, score);
        result = _graph[avgVid].getName();
      }
    }

    return result;
  }

  // TODO: move this to a test class
  void createTestingGraph()
  {
    SchemaVertex v;
    v.setType(SchemaVertex::Tag);
    v.setInfluence(1);
    v.setKey("highway");
    v.setName("highway=road");
    v.setValueType(Enumeration);
    v.setValue("road");
    v.addCategory("transportation");
    _addVertex(v);

    TagEdge isA;
    isA.similarToWeight = 1;
    isA.averageWeight = epsilon;
    isA.type = IsA;

    v.setName("highway=primary");
    v.setValue("primary");
    VertexId highwayPrimary = _addVertex(v);
    addIsA("highway=primary", "highway=road");

    v.setName("highway=secondary");
    v.setValue("secondary");
    VertexId highwaySecondary = _addVertex(v);
    addIsA("highway=secondary", "highway=road");

    v.setName("highway=residential");
    v.setValue("residential");
    VertexId highwayResidential = _addVertex(v);
    addIsA("highway=residential", "highway=road");

    v.setName("highway=service");
    v.setValue("service");
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
    v.setInfluence(2.0);
    v.setKey("abstract_name");
    v.setName("abstract_name");
    v.setValue("");
    v.setValueType(Text);
    v.setCategories(QStringList("name"));
    _addVertex(v);

    v.setKey("name");
    v.setName("name");
    _addVertex(v);
    addIsA("name", "abstract_name");

    ////
    /// create a match all type
    ////
    v.setKey("poi");
    v.setName("poi");
    v.setValueType(Enumeration);
    v.setGeometries(OsmGeometries::Node | OsmGeometries::Area);
    v.setCategories(QStringList());
    _addVertex(v);

    v.setKey("poi");
    v.setValue("yes");
    v.setName("poi=yes");
    v.setCategories(QStringList("poi"));
    VertexId poiYes = _addVertex(v);
    addIsA("poi=yes", "poi");

    v.setKey("leisure");
    v.setValue("");
    v.setName("leisure");
    VertexId leisure = _addVertex(v);
    add_edge(leisure, poiYes, isA, _graph);
    addIsA("leisure", "poi=yes");

    v.setKey("leisure");
    v.setValue("*");
    v.setName("leisure=*");
    v.setGeometries(OsmGeometries::Node | OsmGeometries::Area);
    _addVertex(v);
    addIsA("leisure=*", "poi=yes");

    v.setKey("leisure");
    v.setValue("track");
    v.setName("leisure=track");
    v.setGeometries(OsmGeometries::Node | OsmGeometries::Way);
    _addVertex(v);
    addIsA("leisure=track", "leisure=*");
  }

  VertexId createOrGetVertex(const QString& str)
  {
    VertexId vid;
    if (_name2Vertex.contains(str))
      vid = _name2Vertex[str];
    else
    {
      SchemaVertex v;
      v.setInfluence(-1);
      v.setName(str);
      v.setValueType(Unknown);

      vid = _addVertex(v);
    }
    return vid;
  }

  vector<SchemaVertex> getAllTags()
  {
    vector<SchemaVertex> result;

    result.reserve(_name2Vertex.size());
    for (auto it = _name2Vertex.begin(); it != _name2Vertex.end(); ++it)
      result.push_back(_graph[it.value()]);
    return result;
  }

  QSet<QString> getAllTagKeys()
  {
    QSet<QString> result;
    result.reserve(_name2Vertex.size());
    for (auto it = _name2Vertex.begin(); it != _name2Vertex.end(); ++it)
      result.insert(it.key());
    return result;
  }

  vector<SchemaVertex> getAssociatedTags(QString name)
  {
    set<VertexId> vids;
    _getAssociatedTags(_name2Vertex[name], vids);

    vector<SchemaVertex> result;

    for (const auto& id : vids)
      result.push_back(_graph[id]);

    return result;
  }

  vector<SchemaVertex> getChildTags(QString name)
  {
    set<VertexId> vids;
    _getChildTags(_name2Vertex[name], vids);

    vector<SchemaVertex> result;

    for (const auto& id : vids)
      result.push_back(_graph[id]);
    return result;
  }

  const SchemaVertex& getFirstCommonAncestor(const QString& kvp1, const QString& kvp2)
  {
    if (!_name2Vertex.contains(kvp1) || !_name2Vertex.contains(kvp2))
      return empty;

    VertexId v1 = _name2Vertex[kvp1];
    VertexId v2 = _name2Vertex[kvp2];
    VertexId v1Ancestor = v1;

    while (_isValid(v1Ancestor))
    {
      if (v1Ancestor == v2 || _isAncestor(v2, v1Ancestor))
        return _graph[v1Ancestor];
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
      result = kvp;
    else
      result = kvp.left(index);
    return result;
  }

  vector<SchemaVertex> getSchemaVertices(const Tags& tags) const
  {
    vector<SchemaVertex> result;

    vector<VertexId> vids = _getVertexIds(tags);

    // go through each compound vertex found and evaluate it for match against tags.
    for (const auto& id : vids)
      result.emplace_back(_graph[id]);
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
        const vector<pair<VertexId, double>>& similars = _vertexToScoresCache[id1];

        for (const auto& similar : similars)
        {
          if (similar.second >= minimumScore)
            result.push_back(_graph[similar.first]);
        }
      }
    }
    return result;
  }

  vector<SchemaVertex> getTagByCategory(OsmSchemaCategory c)
  {
    vector<SchemaVertex> result;

    boost::graph_traits<TagGraph>::vertex_iterator vi, vend;
    for (boost::tie(vi, vend) = vertices(_graph); vi != vend; ++vi)
    {
      const SchemaVertex& tv = _graph[*vi];
      if (OsmSchemaCategory::fromStringList(tv.getCategories()).contains(c))
        result.push_back(tv);
    }
    return result;
  }

  const SchemaVertex& getTagVertex(const QString& kvp) const
  {
    LOG_VART(kvp);

    QString n = normalizeKvp(kvp);
    LOG_VART(n);
    if (_name2Vertex.contains(n))
    {
      const SchemaVertex& v = _graph[_name2Vertex[n]];
      LOG_VART(v);
      LOG_VART(v.getType());
      if (v.getType() == SchemaVertex::Tag)
        return v;
    }
    return empty;
  }

  vector<SchemaVertex> getUniqueSchemaVertices(const Tags& tags) const
  {
    vector<SchemaVertex> result;

    vector<VertexId> vids = _getVertexIds(tags);
    vids = _removeAncestorVertices(vids);

    // go through each compound vertex found and evaluate it for match against tags.
    for (const auto& id : vids)
      result.push_back(_graph[id]);
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
      auto it = _isAncestorCache.find(key);
      if (it == _isAncestorCache.end())
      {
        result = _isAncestor(_name2Vertex[nChildKvp], _name2Vertex[nParentKvp]);
        _isAncestorCache[key] = result;
      }
      else
        result = it->second;
    }
    return result;
  }

  /**
   * Normalizes enumerated names. If the name exists as stated. (e.g. highway=road), then that
   * kvp is returned. However, if the kvp doesn't exist, but there is an enumerated kvp that =*
   * then that is substituted. E.g. addr:housenumber=123 would be converted to addr:housenumber=*.
   * If neither of those situations matches then an empty string is returned.
   */
  QString normalizeEnumeratedKvp(const QString& kvp) const
  {
    QString result = _normalizeEnumeratedKvp(kvp);
    if (result.isEmpty())
      result = _normalizeEnumeratedKvp(kvp.toLower());
    return result;
  }

  QString normalizeKvp(const QString& kvp) const
  {
    QString result = _normalizeKvp(kvp);
    if (result.isEmpty())
      result = _normalizeKvp(kvp.toLower());
    return result;
  }

  double score(const QString& kvp1, const QString& kvp2)
  {
    double result = 0.0;

    QString kvpn1 = normalizeEnumeratedKvp(kvp1);
    LOG_VART(kvpn1);
    QString kvpn2 = normalizeEnumeratedKvp(kvp2);
    LOG_VART(kvpn2);

    if (kvpn1.isEmpty() == false && kvpn2.isEmpty() == false)
    {
      VertexId id1 = _name2Vertex[kvpn1];
      LOG_VART(id1);
      VertexId id2 = _name2Vertex[kvpn2];
      LOG_VART(id2);

      if (_processed.find(id1) == _processed.end())
      {
        _calculateScores(id1);
        _processed.insert(id1);
      }

      pair<VertexId, VertexId> key(id1, id2);

      if (_cachedScores.find(key) != _cachedScores.end())
        result = _cachedScores[key];
      else
        result = 0.0;
      LOG_VART(result);

      if (id1 == id2 && kvp1 != kvp2)
      {
        // if this is a enumerated wild card match, but the values are different then use the
        // mismatch score. E.g. addr:housenumber=12 vs. addr:housenumber=56
        if (kvpn1.endsWith("=*"))
          result = getTagVertex(kvpn1).getMismatchScore();
        else  // if this is an alias match
          result = 1.0;
      }
      LOG_VART(result);
    }
    return result;
  }

  void setIsACost(double cost)
  {
    _isACost = cost;
    boost::graph_traits<TagGraph>::edge_iterator ei, eend;
    for (boost::tie(ei, eend) = edges(_graph); ei != eend; ++ei)
    {
      if (_graph[*ei].type == IsA)
        _graph[*ei].similarToWeight = cost;
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

    vector<VertexId> orderedVertices;

    boost::graph_traits<TagGraph>::vertex_iterator vi, vend;
    for (boost::tie(vi, vend) = vertices(_graph); vi != vend; ++vi)
      orderedVertices.push_back(*vi);
    VertexNameComparator vnc(_graph);
    sort(orderedVertices.begin(), orderedVertices.end(), vnc);
    for (const auto& vid : orderedVertices)
    {
      QString label = _graph[vid].getName();
      QString vStr = QString("\"%1\"").arg(label);

      label = label.replace("{", "\\}").replace("}", "\\}");
      if (_graph[vid].getChildWeight() > 0)
        label = QString("%1\\nchildWeight = %2").arg(label).arg(_graph[vid].getChildWeight());
      result += QString("  %1 [shape=record,label=\"%2\"];\n").arg(vStr, label);
    }

    result += "\n";

    set<pair<VertexId, VertexId>> used;
    QStringList orderedEdges;

    boost::graph_traits<TagGraph>::edge_iterator ei, eend;
    for (boost::tie(ei, eend) = edges(_graph); ei != eend; ++ei)
    {
      VertexId src = source(*ei, _graph);
      VertexId trg = target(*ei, _graph);

      if (_graph[trg].getName() == _graph[src].getName())
        throw IllegalArgumentException("Unexpected vertices with the same name. " + _graph[src].getName());

      if (_graph[trg].getName() < _graph[src].getName())
        swap(src, trg);

      pair<VertexId, VertexId> p(src, trg);

      if (used.find(p) == used.end())
      {
        QString srcStr = _graph[src].getName();
        QString trgStr = _graph[trg].getName();

        // only show is a relationships for legit tags. No need w/ things like "amenity" or "poi",
        // it just clutters the graph.
        if (_graph[*ei].type == IsA &&
           (_graph[src].getName().contains("=") && _graph[trg].getName().contains("=")))
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
    VertexId vid = createOrGetVertex(tv.getName());

    const SchemaVertex& v = _graph[vid];
    if (v.isValid())
    {
      if (_logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN(tv.getName() << " was specified multiple times in the schema file.");
      }
      else if (_logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(typeid(this).name() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      _logWarnCount++;
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
  HashMap<pair<VertexId, VertexId>, double> _cachedScores;
  HashMap<AverageKey, AverageResult> _cachedAverages;
  HashMap<VertexId, VertexId> _parents;
  QList<pair<QRegExp, VertexId>> _regexKeys;
  HashMap<pair<VertexId, VertexId>, bool> _isAncestorCache;
  using VertexToScoreCache = HashMap<VertexId, vector<pair<VertexId, double>>>;
  VertexToScoreCache _vertexToScoresCache;

  TagGraph _graph;

  //this should be static, but there's no header file
  int _logWarnCount;

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
      score = 1;
      return vid1;
    }
    else if (_isAncestor(vid2, vid1))
    {
      score = 1;
      return vid2;
    }

    boost::graph_traits<TagGraph>::edge_iterator ei, eend;
    for (boost::tie(ei, eend) = edges(_graph); ei != eend; ++ei)
    {
      _graph[*ei].w1 = pow(_graph[*ei].averageWeight, w1);
      _graph[*ei].w2 = pow(_graph[*ei].averageWeight, w2);
    }

    multiplies<double> combiner;
    greater<double> comparer;
    dijkstra_shortest_paths(_graph, vid1,
                            boost::distance_map(&d1[0]).
                            weight_map(get(&TagEdge::w1, _graph)).
                            distance_zero(1.0).
                            distance_combine(combiner).
                            distance_inf(0.0).
                            distance_compare(comparer));

    std::vector<double> d2(num_vertices(_graph));

    dijkstra_shortest_paths(_graph, vid2,
                            boost::distance_map(&d2[0]).
                            weight_map(get(&TagEdge::w2, _graph)).
                            distance_zero(1.0).
                            distance_combine(combiner).
                            distance_inf(0.0).
                            distance_compare(comparer));

    double bestScore = -1.0;
    VertexId bestVid = vid1;
    boost::graph_traits<TagGraph>::vertex_iterator vi, vend;
    LOG_TRACE("from " << _graph[vid1].getName() << " to " << _graph[vid2].getName());
    for (boost::tie(vi, vend) = vertices(_graph); vi != vend; ++vi)
    {
      // The best minimum score is generally the average.
      // give a very slight advantage to the tags with a higher max score.
      // give a very slight advantage to the first input.
      double s = std::min(d1[*vi], d2[*vi] + 1e-6) +
        std::max(d1[*vi], d2[*vi]) / 1e6;
      if (s > 0)
       LOG_TRACE("  " << _graph[*vi].getName() << " : " << d1[*vi] << " " << d2[*vi] <<
       " (" << s << ")");

      if (s > bestScore)
      {
        bestScore = s;
        bestVid = *vi;
      }
    }

    LOG_TRACE("  best vid: " << bestVid << " " << _graph[bestVid].getName().toStdString());

    score = bestScore;
    return bestVid;
  }

  void _calculateScores(VertexId vd)
  {
    std::vector<double> d(num_vertices(_graph));

    multiplies<double> combiner;
    greater<double> comparer;
    dijkstra_shortest_paths(_graph, vd,
                            boost::distance_map(&d[0]).
                            weight_map(get(&TagEdge::similarToWeight, _graph)).
                            distance_zero(1.0).
                            distance_combine(combiner).
                            distance_inf(0.0).
                            distance_compare(comparer));

    boost::graph_traits<TagGraph>::vertex_iterator vi, vend;
    //LOG_TRACE("Scores for: " << _graph[vd].name.toStdString());
    for (boost::tie(vi, vend) = vertices(_graph); vi != vend; ++vi)
    {
      pair<VertexId, VertexId> key = pair<VertexId, VertexId>(vd, *vi);
      _cachedScores[key] = d[*vi];
      //LOG_TRACE("  " << _graph[*vi].name.toStdString() << " : " << d[*vi]);

      // cache the score between vd and vi in another structure that is more efficient for other
      // query types.
      if (d[*vi] > 0.0)
        _vertexToScoresCache[vd].push_back(pair<VertexId, double>(*vi, d[*vi]));
    }
  }

  void _findPotentialCompoundTags(VertexId vid, set<VertexId>& compoundTags) const
  {
    const SchemaVertex& sv = _graph[vid];

    auto it = _name2CompoundVertex.find(sv.getName());
    while (it != _name2CompoundVertex.end() && it.key() == sv.getName())
    {
      compoundTags.insert(it.value());
      ++it;
    }
  }

  void _getAssociatedTags(VertexId from, set<VertexId>& result)
  {
    boost::graph_traits<TagGraph>::edge_iterator ei, eend;
    for (boost::tie(ei, eend) = edges(_graph); ei != eend; ++ei)
    {
      VertexId thisFrom = source(*ei, _graph);
      VertexId thisTo = target(*ei, _graph);
      if (_graph[*ei].type == AssociatedWith && thisFrom == from && result.find(thisTo) == result.end())
        result.insert(thisTo);
    }
  }

  void _getChildTags(VertexId parent, set<VertexId>& result)
  {
    boost::graph_traits<TagGraph>::edge_iterator ei, eend;
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

    auto it = _parents.find(child);
    if (it != _parents.end())
      result = it->second;
    return result;
  }

  vector<VertexId> _getVertexIds(const Tags& tags) const
  {
    vector<VertexId> result;
    set<VertexId> compoundTags;

    // go through each of the tags and look for the tag vertex.
    for (auto it = tags.begin(); it != tags.end(); ++it)
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
    for (const auto& compoundTagId : compoundTags)
    {
      const SchemaVertex& sv = _graph[compoundTagId];
      if (sv.isCompoundMatch(tags))
        result.push_back(compoundTagId);
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
      return false;
    else if (parent == ancestor)
      return true;
    else
      return _isAncestor(parent, ancestor);
  }

  bool _isValid(VertexId vid) const
  {
    return vid != numeric_limits<VertexId>::max();
  }

  QString _normalizeEnumeratedKvp(const QString& kvp) const
  {
    static QString equalStar = "=*";
    if (_name2Vertex.contains(kvp))
      return kvp;
    else
    {
      QString newKvp = getKey(kvp) + equalStar;
      if (_name2Vertex.contains(newKvp))
        return newKvp;
      else
        return QString();
    }
  }

  QString _normalizeKvp(const QString& kvp) const
  {
    QString key = getKey(kvp);
    if (_name2Vertex.contains(kvp))
      return kvp;
    else if (_name2Vertex.contains(key + "=*"))
      return getKey(kvp) + "=*";
    else if (_name2Vertex.contains(key))
    {
      const SchemaVertex& v = _graph[_name2Vertex[key]];
      if (v.getValueType() == Text || v.getValueType() == Int)
        return key;
      else
        return QString();
    }
    else
    {
      // check to see if any of the regular expressions match
      for (const auto& re : qAsConst(_regexKeys))
      {
        if (re.first.exactMatch(key))
        {
          VertexId vid = re.second;
          return _graph[vid].getKey();
        }
      }
      return QString();
    }
  }

  void _percolateInheritance()
  {
    boost::graph_traits<TagGraph>::vertex_iterator vi, vend;
    for (boost::tie(vi, vend) = vertices(_graph); vi != vend; ++vi)
    {
      if (_graph[*vi].getChildWeight() > 0.0)
      {
        set<VertexId> children;
        _getChildTags(*vi, children);
        for (const auto& id : children)
        {
          pair<EdgeId, EdgeId> p = addSimilarTo(_graph[id].getName(), _graph[*vi].getName(),
                                                _graph[*vi].getChildWeight());
          _graph[p.first].show = false;
          _graph[p.second].show = false;
        }
      }
    }

    boost::graph_traits<TagGraph>::edge_iterator ei, eend;
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
      for (size_t j = 0; j < vid.size(); ++j)
      {
        if (i != j && _isAncestor(vid[j], vid[i]))
        {
          aAncestor = true;
          break;
        }
      }
      if (!aAncestor)
        result.push_back(vid[i]);
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
      const SchemaVertex& parentTv = _graph[parent];
      if (childTv.getGeometries() == 0)
        childTv.setGeometries(parentTv.getGeometries());
      if (childTv.getInfluence() == -1.0)
        childTv.setInfluence(parentTv.getInfluence());
      if (childTv.getValueType() == Unknown)
        childTv.setValueType(parentTv.getValueType());
      if (childTv.getMismatchScore() == -1.0)
        childTv.setMismatchScore(parentTv.getMismatchScore());
      // add any categories that are assigned to the parent to the child.
      for (const auto& c : parentTv.getCategories())
      {
        if (childTv.getCategories().contains(c) == false)
          childTv.addCategory(c);
      }
    }
    else
    {
      if (childTv.getInfluence() == -1.0)
      {
        if (_logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN("Influence for " << childTv.getName() << " has not been specified.");
        }
        else if (_logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(typeid(this).name() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        _logWarnCount++;
        childTv.setInfluence(1.0);
      }
      if (childTv.getValueType() == Unknown)
      {
        if (_logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN("Value type for " << childTv.getName() << " has not been specified.");
        }
        else if (_logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(typeid(this).name() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        _logWarnCount++;
      }
    }
  }

  VertexId _updateVertex(VertexId vid, const SchemaVertex& v)
  {
    _name2Vertex[v.getName()] = vid;
    _graph[vid] = v;

    if (v.getName().startsWith("regex?"))
    {
      if (v.getType() == SchemaVertex::Compound)
        throw HootException("Compound tags can not have regex names.");
      QRegExp re(v.getName().mid(6));
      _regexKeys.append(pair<QRegExp, VertexId>(re, vid));
    }

    // if this is a compound rule then update the link from tags to compound rules.
    if (v.getType() == SchemaVertex::Compound)
    {
      SchemaVertex::CompoundRuleList rules = v.getCompoundRules();
      for (const auto& r : rules)
      {
        for (const auto& p : r)
        {
          if (_name2CompoundVertex.contains(p->getName(), vid) == false)
            _name2CompoundVertex.insert(p->getName(), vid);
        }
      }
    }

    // add in the list of aliases.
    for (const auto& alias : v.getAliases())
    {
      if (_name2Vertex.contains(alias))
      {
        throw HootException(QString("Alias is being used multiple times. Please only reference an "
          "alias once or use the base tag. (offending tag: %1, offending alias: %2)").
          arg(v.getName(), alias));
      }
      _name2Vertex[alias] = vid;
    }

    return vid;
  }
};

OsmSchema::OsmSchema()
  : _d(std::make_shared<OsmSchemaData>())
{
  loadDefault();
  // Write this out to a temp file instead of to the log due to its size.
  if (Log::getInstance().getLevel() == Log::Trace)
  {
    const QString graphvizPath = "tmp/schema-graphviz";
    const QString errorMsg = "Unable to write schema graphviz file to " + graphvizPath;
    try
    {
      if (FileUtils::makeDir("tmp"))
      {
        FileUtils::writeFully(graphvizPath, toGraphvizString());
        LOG_TRACE("Wrote schema graph viz file to: " << graphvizPath);
      }
      else
      {
        LOG_TRACE(errorMsg);
      }
    }
    catch (const HootException&)
    {
      LOG_TRACE(errorMsg);
    }
  }
}

void OsmSchema::addAssociatedWith(const QString& name1, const QString& name2) const
{
  _d->addAssociatedWith(name1, name2);
}

void OsmSchema::addIsA(const QString& name1, const QString& name2) const
{
  _d->addIsA(name1, name2);
}

void OsmSchema::addSimilarTo(const QString& name1, const QString& name2, double weight, bool oneway) const
{
  _d->addSimilarTo(name1, name2, weight, oneway);
}

QString OsmSchema::average(const QString& kvp1, const QString& kvp2, double& score) const
{
  return _d->average(kvp1, 1.0, kvp2, 1.0, score);
}

QString OsmSchema::average(const QString& kvp1, double w1, const QString& kvp2, double w2,
                           double& score) const
{
  return _d->average(kvp1, w1, kvp2, w2, score);
}

void OsmSchema::createTestingGraph()
{
  _d = std::make_shared<OsmSchemaData>();
  _d->createTestingGraph();
}

vector<SchemaVertex> OsmSchema::getAllTags() const
{
  return _d->getAllTags();
}

QSet<QString> OsmSchema::getAllTagKeys()
{
  if (_allTagKeysCache.isEmpty())
    _allTagKeysCache = _d->getAllTagKeys();
  return _allTagKeysCache;
}

QSet<QString> OsmSchema::getAllTypeKeys()
{
  if (_allTypeKeysCache.isEmpty())
  {  
    QSet<QString> allTypeKeysCacheTemp = _d->getAllTagKeys();
    for (const auto& typeKey : qAsConst(allTypeKeysCacheTemp))
    {
      // All we care about for type comparison are tags of schema type "tag". We definitely don't
      // care about metadata tags, but its possible we may care about some text or numeric tags
      // at some point.
      if (!isMetaData(typeKey, "") && !isTextTag(typeKey) && !isNumericTag(typeKey))
        _allTypeKeysCache.insert(typeKey);
    }
    //LOG_VART(_allTypeKeysCache);
  }
  return _allTypeKeysCache;
}

bool OsmSchema::hasTagKey(const QString& key)
{
  return getAllTagKeys().contains(key);
}

Tags OsmSchema::getAssociatedTags(const Tags& tags)
{
  Tags tagsToReturn;
  for (auto inputTagItr = tags.constBegin(); inputTagItr != tags.constEnd(); ++inputTagItr)
  {
    const Tags childTags =
      Tags::schemaVerticesToTags(
        getAssociatedTagsAsVertices(inputTagItr.key() + "=" + inputTagItr.value()));
    for (auto childTagItr = childTags.constBegin(); childTagItr != childTags.constEnd(); ++childTagItr)
    {
      QString val = childTagItr.value().trimmed();
      // Associated tags are just returned as keys, so this is being added to the value to alleviate
      // callers that expect non-empty values.
      if (val.isEmpty())
        val = "*";
      tagsToReturn.appendValue(childTagItr.key(), val);
    }
  }
  return tagsToReturn;
}

vector<SchemaVertex> OsmSchema::getAssociatedTagsAsVertices(const QString& name) const
{
  return _d->getAssociatedTags(name);
}

OsmSchemaCategory OsmSchema::getCategories(const Tags& t) const
{
  OsmSchemaCategory result;

  // if at least one tag has the specified category then return true
  for (auto it = t.constBegin(); it != t.constEnd(); ++it)
  {
    const SchemaVertex& tv = getTagVertex(toKvp(it.key(), it.value()));
    for (const auto& category : tv.getCategories())
      result = result | OsmSchemaCategory::fromString(category);
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
  for (const auto& category : tv.getCategories())
    result = result | OsmSchemaCategory::fromString(category);
  return result;
}

vector<SchemaVertex> OsmSchema::getChildTagsAsVertices(const QString& name) const
{
  return _d->getChildTags(name);
}

Tags OsmSchema::getChildTags(const Tags& tags)
{
  Tags tagsToReturn;
  for (auto inputTagItr = tags.constBegin(); inputTagItr != tags.constEnd(); ++inputTagItr)
  {
    const Tags childTags =
      Tags::schemaVerticesToTags(
        getChildTagsAsVertices(inputTagItr.key() + "=" + inputTagItr.value()));
    for (auto childTagItr = childTags.constBegin(); childTagItr != childTags.constEnd(); ++childTagItr)
      tagsToReturn.appendValue(childTagItr.key(), childTagItr.value());
  }
  return tagsToReturn;
}

Tags OsmSchema::getAliasTags(const Tags& tags)
{
  Tags tagsToReturn;
  const std::vector<SchemaVertex> schemaVertices = _d->getSchemaVertices(tags);
  for (const auto& vertex : schemaVertices)
    tagsToReturn.add(Tags::kvpListToTags(vertex.getAliases()));
  return tagsToReturn;
}

const SchemaVertex& OsmSchema::getFirstCommonAncestor(const QString& kvp1, const QString& kvp2) const
{
  return _d->getFirstCommonAncestor(kvp1, kvp2);
}

OsmSchema& OsmSchema::getInstance()
{
  static OsmSchema instance;
  return instance;
}

double OsmSchema::getIsACost() const
{
  return _d->getIsACost();
}

vector<SchemaVertex> OsmSchema::getSchemaVertices(const Tags& tags) const
{
  return _d->getSchemaVertices(tags);
}

vector<SchemaVertex> OsmSchema::getSimilarTagsAsVertices(const QString& name, double minimumScore) const
{
  if (minimumScore <= 0)
    throw IllegalArgumentException("minimumScore must be > 0");
  return _d->getSimilarTags(name, minimumScore);
}

Tags OsmSchema::getSimilarTags(const QString& name, double minimumScore)
{
  Tags tags;
  const vector<SchemaVertex> vertices = _d->getSimilarTags(name, minimumScore);
  for (const auto& vertex : vertices)
    tags.appendValue(vertex.getKey(), vertex.getValue());
  return tags;
}

vector<SchemaVertex> OsmSchema::getTagByCategory(const OsmSchemaCategory& c) const
{
  return _d->getTagByCategory(c);
}

const SchemaVertex& OsmSchema::getTagVertex(const QString& kvp) const
{
  return _d->getTagVertex(kvp);
}

vector<SchemaVertex> OsmSchema::getUniqueSchemaVertices(const Tags& tags) const
{
  return _d->getUniqueSchemaVertices(tags);
}

bool OsmSchema::hasAnyCategory(const QString& key, const QString& val) const
{
  return getCategories(key, val) != OsmSchemaCategory::Empty;
}

bool OsmSchema::hasCategory(const Tags& t, const QString& category) const
{
  bool result = false;
  // if at least one tag has the specified category then return true
  for (auto it = t.constBegin(); it != t.constEnd(); ++it)
  {
    // ignore empty values.
    if (it.value().isEmpty() == false)
    {
      const SchemaVertex& tv = getTagVertex(it.key() + "=" + it.value());
      LOG_VART(tv);
      LOG_VART(tv.getCategories());
      if (tv.getCategories().contains(category))
      {
        result = true;
        LOG_VART(result);
        break;
      }
    }
  }

  return result;
}

bool OsmSchema::hasCategory(const QString& kvp, const QString& category) const
{
  return getTagVertex(kvp).getCategories().contains(category);
}

bool OsmSchema::hasCategory(const Tags& t, const OsmSchemaCategory& category) const
{
  return hasCategory(t, category.toString());
}

bool OsmSchema::hasCategory(const QString& kvp, const OsmSchemaCategory& category) const
{
  return hasCategory(kvp, category.toString());
}

bool OsmSchema::isAncestor(const QString& childKvp, const QString& parentKvp) const
{
  return _d->isAncestor(childKvp, parentKvp);
}

bool OsmSchema::containsTagFromList(const Tags& tags, const QStringList& tagList) const
{
  //LOG_VART(tagList.size());
  for (const auto& tag : tagList)
  {
    QStringList tagParts = tag.split("=");
    const QString key = tagParts[0];
    //LOG_VART(key);
    const QString value = tagParts[1];
    //LOG_VART(value);
    if ((value == "*" && tags.contains(key)) || (tags.get(key).toLower() == value))
      return true;
  }
  return false;
}

bool OsmSchema::allowsFor(const Tags& t, const ElementType& /*type*/,
                          OsmGeometries::Type geometries) const
{
  //  Empty tags shouldn't allow for anything
  if (t.empty())
    return false;
  int usableTags = 0;
  OsmGeometries::Type value = OsmGeometries::All;
  for (auto it = t.constBegin(); it != t.constEnd(); ++it)
  {
    const SchemaVertex& tv = getTagVertex(it.key() + "=" + it.value());
    //  Unknown vertex types aren't usable tags
    if (tv.getType() != SchemaVertex::UnknownVertexType &&
        tv.getGeometries() != OsmGeometries::Empty)
    {
      value = static_cast<OsmGeometries::Type>(value & tv.getGeometries());
      usableTags++;
    }
  }
  //  Unusable tags shouldn't allow for anything
  if (usableTags == 0)
    return false;
  //  Check geometries against usable tags
  return (value & geometries) != OsmGeometries::Empty;
}

bool OsmSchema::allowsFor(const ConstElementPtr& e, OsmGeometries::Type geometries) const
{
  return allowsFor(e->getTags(), e->getElementType(), geometries);
}

bool OsmSchema::isList(const QString& /*key*/, const QString& value) const
{
  return value.contains(';');
}

bool OsmSchema::isMetaData(const QString& key, const QString& /*value*/)
{
  if (key.startsWith(MetadataTags::HootTagPrefix()))
    return true;
  if (_metadataKey.contains(key))
    return _metadataKey[key];
  else
  {
    // for now all metadata tags are text so they're referenced by the key only. If that changes
    // then we'll need some logic here to check if a vertex is a text vertex.
    bool metadata = isAncestor(key, "metadata");
    _metadataKey[key] = metadata;
    return metadata;
  }
}

bool OsmSchema::isTextTag(const QString& key) const
{
  return getTagVertex(key).getValueType() == Text;
}

bool OsmSchema::isNumericTag(const QString& key) const
{
  TagValueType valueType = getTagVertex(key).getValueType();
  return valueType == Real || valueType == Int;
}

void OsmSchema::loadDefault()
{
  QString path = ConfPath::search("schema.json");

  _d = std::make_shared<OsmSchemaData>();

  LOG_DEBUG("Loading translation files...");
  OsmSchemaLoaderFactory::getInstance().createLoader(path)->load(path, *this);
  LOG_DEBUG("Translation files loaded.");
}

double OsmSchema::score(const QString& kvp1, const QString& kvp2) const
{
  // Tried using a LruCache here to speed up scoring, but it had a negative impact. :(
  return std::max(_d->score(kvp1, kvp2), _d->score(kvp2, kvp1));
}

double OsmSchema::score(const SchemaVertex& v1, const SchemaVertex& v2) const
{
  return score(v1.getName(), v2.getName());
}

double OsmSchema::score(const QString& kvp, const Tags& tags) const
{
  double maxScore = 0.0;
  for (auto tagItr = tags.begin(); tagItr != tags.end(); ++tagItr)
  {
    const QString key = tagItr.key().trimmed();
    const QString value = tagItr.value().trimmed();
    if (!key.isEmpty() && !value.isEmpty())
    {
      QString kvp2 = "";
      kvp2.append(key);
      kvp2.append("=");
      kvp2.append(value);
      const double scoreVal = score(kvp, kvp2);
      if (scoreVal > maxScore)
        maxScore = scoreVal;
    }
  }
  return maxScore;
}

bool OsmSchema::isGeneric(const Tags& tags)
{
  return !hasMoreThanOneType(tags) &&
          StringUtils::containsAny(tags.toKvps(), getGenericKvps().toList());
}

bool OsmSchema::isGenericKvp(const QString& kvp) const
{
  return getGenericKvps().contains(kvp);
}

QSet<QString> OsmSchema::getGenericKvps() const
{
  // There may be a better way to manage these in the schema itself.
  if (_genericKvps.isEmpty())
  {
    _genericKvps.insert("poi=yes");
    _genericKvps.insert("building=yes");
    _genericKvps.insert("area=yes");
  }
  return _genericKvps;
}

QString OsmSchema::getFirstType(const Tags& tags, const bool allowGeneric)
{
  QStringList keys = tags.keys();
  keys.sort();
  for (const auto& key : qAsConst(keys))
  {
    const QString val = tags[key];
    if (!val.trimmed().isEmpty())
    {
      const QString kvp = toKvp(key, val);
      if (isTypeKey(key) && (allowGeneric || !isGenericKvp(kvp)))
        return kvp;
    }
  }
  return "";
}

bool OsmSchema::explicitTypeMismatch(const Tags& tags1, const Tags& tags2, const double minTypeScore)
{
  LOG_VART(tags1);
  LOG_VART(tags2);

  bool featuresHaveExplicitTypeMismatch = false;

  const bool feature1HasType = hasType(tags1);
  LOG_VART(feature1HasType);
  if (feature1HasType)
  {
    const bool feature2HasType = hasType(tags2);
    LOG_VART(feature2HasType);
    if (feature2HasType)
    {
      const bool feature1Generic = isGeneric(tags1);
      LOG_VART(feature1Generic);
      if (!feature1Generic)
      {
        const bool feature2Generic = isGeneric(tags2);
        LOG_VART(feature2Generic);
        if (!feature2Generic)
        {
          const double typeScore = scoreTypes(tags1, tags2, true);
          if (typeScore < minTypeScore)
          {
            featuresHaveExplicitTypeMismatch = true;
            LOG_TRACE(
              "explicit type mismatch: " << getFirstType(tags1, false) << " and " <<
              getFirstType(tags2, false));
          }
          else
          {
            LOG_TRACE(
              "explicit type match: " << getFirstType(tags1, false) << " and " <<
              getFirstType(tags2, false));
          }
        }
      }
    }
  }
  LOG_VART(featuresHaveExplicitTypeMismatch);
  return featuresHaveExplicitTypeMismatch;
}

bool OsmSchema::typeMismatch(const Tags& tags1, const Tags& tags2, const double minTypeScore)
{
  LOG_VART(tags1);
  LOG_VART(tags2);

  bool featuresHaveTypeMismatch = false;

  const double typeScore = scoreTypes(tags1, tags2, true);
  if (typeScore < minTypeScore)
  {
    featuresHaveTypeMismatch = true;
    LOG_TRACE(
      "type mismatch: " << getFirstType(tags1, false) << " and " << getFirstType(tags2, false));
  }
  else
  {
    LOG_TRACE(
      "type match: " << getFirstType(tags1, false) << " and " << getFirstType(tags2, false));
  }

  LOG_VART(featuresHaveTypeMismatch);
  return featuresHaveTypeMismatch;
}

bool OsmSchema::hasType(const Tags& tags)
{
  for (auto tagsItr = tags.begin(); tagsItr != tags.end(); ++tagsItr)
  {
    LOG_VART(tagsItr.key());
    LOG_VART(isTypeKey(tagsItr.key()));
    if (isTypeKey(tagsItr.key()) && !tags[tagsItr.key()].trimmed().isEmpty())
    {
      LOG_TRACE("has type");
      return true;
    }
  }
  LOG_TRACE("does not have type");
  return false;
}

QString OsmSchema::mostSpecificType(const Tags& tags)
{
  QString mostSpecificType;
  bool currentMostSpecificTypeIsCombo = false;
  for (auto tagsItr = tags.begin(); tagsItr != tags.end(); ++tagsItr)
  {
    const QString key = tagsItr.key();
    const QString val = tagsItr.value();
    const QString kvp = toKvp(key, val);
    LOG_VART(kvp);
    LOG_VART(isTypeKey(tagsItr.key()));

    if (isTypeKey(key)) // Is this type in the schema at all?
    {
      const bool kvpIsCombo = hasCategory(kvp, OsmSchemaCategory::combination());
      // If this tag is considered a "combo" tag, or one that must be used in combination with some
      // other type tag to uniquely identify a type, we consider it a less specific type than a
      // non-combo tag. So if we already have a non-combo tag for our most specific type, then skip
      // this if its a combo tag.
      if ((!kvpIsCombo || currentMostSpecificTypeIsCombo) &&
          // Ensure that this tag isn't more generic (ancestor) than the current specific type tag.
          (mostSpecificType.isEmpty() || !isAncestor(kvp, mostSpecificType)))
      {
        mostSpecificType = kvp;
        currentMostSpecificTypeIsCombo = kvpIsCombo;
      }
    }
  }
  return mostSpecificType;
}

bool OsmSchema::hasMoreThanOneType(const Tags& tags)
{
  int count = 0;
  for (auto tagsItr = tags.begin(); tagsItr != tags.end(); ++tagsItr)
  {
    LOG_VART(tagsItr.key());
    LOG_VART(isTypeKey(tagsItr.key()));
    if (isTypeKey(tagsItr.key()))
    {
      count++;
      if (count > 1)
        return true;
    }
  }
  return false;
}

double OsmSchema::scoreTypes(const Tags& tags1, const Tags& tags2, const bool ignoreGenericTypes)
{  
  double maxScore = 0.0;

  for (auto tags1Itr = tags1.begin(); tags1Itr != tags1.end(); ++tags1Itr)
  {
    const QString key1 = tags1Itr.key().trimmed();
    const QString val1 = tags1Itr.value().trimmed();
    if (!key1.isEmpty() && !val1.isEmpty())
    {
      const QString kvp1 = toKvp(key1, val1);

      LOG_VART(key1);
      LOG_VART(val1);
      LOG_VART(kvp1);
      LOG_VART(isMetaData(key1, val1));
      LOG_VART(isTypeKey(key1));

      if (ignoreGenericTypes && getGenericKvps().contains(kvp1))
      {
        continue;
      }

      if (!isMetaData(key1, val1) && (isTypeKey(key1) || isTypeKey(kvp1)))
      {
        for (auto tags2Itr = tags2.begin(); tags2Itr != tags2.end(); ++tags2Itr)
        {
          const QString key2 = tags2Itr.key().trimmed();
          const QString val2 = tags2Itr.value().trimmed();
          if (!key2.isEmpty() && !val2.isEmpty())
          {
            const QString kvp2 = toKvp(key2, val2);

            LOG_VART(key2);
            LOG_VART(val2);
            LOG_VART(kvp2);
            LOG_VART(isMetaData(key2, val2));
            LOG_VART(isTypeKey(key2));

            if (ignoreGenericTypes && getGenericKvps().contains(kvp2))
              continue;

            if (!isMetaData(key2, val2) && (isTypeKey(key2) || isTypeKey(kvp2)))
            {
              const double calculatedScore = score(kvp1, kvp2);
              LOG_VART(calculatedScore);
              if (calculatedScore > maxScore)
                maxScore = calculatedScore;
            }
          }
        }
      }
    }
  }

  LOG_VART(maxScore);
  return maxScore;
}

bool OsmSchema::isTypeKey(const QString& key)
{
  return getAllTypeKeys().contains(key);
}

double OsmSchema::scoreOneWay(const QString& kvp1, const QString& kvp2) const
{
  return _d->score(kvp1, kvp2);
}

void OsmSchema::setIsACost(double cost) const
{
  _d->setIsACost(cost);
}

QString OsmSchema::toGraphvizString() const
{
  return _d->toGraphvizString();
}

QString OsmSchema::toKvp(const QString& key, const QString& value)
{
  if (value.isEmpty())
    return key;
  else
    return key % "=" % value;
}

void OsmSchema::update() const
{
  _d->update();
}

void OsmSchema::updateOrCreateVertex(const SchemaVertex& tv) const
{
  _d->updateOrCreateVertex(tv);
}

QString OsmSchema::getParentKvp(const QString& kvp1, const QString& kvp2) const
{
  if (isAncestor(kvp1, kvp2))
    return kvp2;
  else if (isAncestor(kvp2, kvp1))
    return kvp1;
  else
    return kvp1;
}

}

/** End ignoring maybe-uninitialized warnings */
#pragma GCC diagnostic pop
