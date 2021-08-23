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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef OSMSCHEMA_H
#define OSMSCHEMA_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/schema/SchemaVertex.h>

// Qt
#include <QSet>


// Standard
#include <stdint.h>
#include <vector>

namespace hoot
{

class Tags;

// There are some unused and now disabled types here that the original design intention of which is
// unclear.
enum EdgeType
{
  //CanHave,  // not used
  IsA,
  SimilarTo,
  //ParentOf, // not used
  AssociatedWith//,
  //CompoundComponent // not used
};

struct OsmSchemaCategory
{
  enum Type
  {
    Empty =           0x00,
    Poi =             0x01,
    Building =        0x02,
    Transportation =  0x04,
    Use =             0x08,
    Name =            0x10,
    PseudoName =      0x20,
    // use for the feature may have multiple purposes
    Multiuse =        0x40,
    // This tag must be combined with another tag to get the type of the feature. e.g.
    // surface=asphalt must be combined with highway=*, amenity=parking, etc. to know the type of a
    // feature with it as a tag. TODO: This probably makes more sense as an EdgeType rather than a
    // category (maybe CompoundComponent?), but have been unable to come up with a simple design
    // where it could be used in that way yet.
    Combination =     0x80//,
    //All = Poi | Building | Transportation | Use | Name | PseudoName | Multiuse
  };

  OsmSchemaCategory() : _type(Empty) {}
  OsmSchemaCategory(Type t) : _type(t) {}

  static OsmSchemaCategory building() { return OsmSchemaCategory(Building); }
  static OsmSchemaCategory poi() { return OsmSchemaCategory(Poi); }
  static OsmSchemaCategory transportation() { return OsmSchemaCategory(Transportation); }
  static OsmSchemaCategory use() { return OsmSchemaCategory(Use); }
  static OsmSchemaCategory name() { return OsmSchemaCategory(Name); }
  static OsmSchemaCategory pseudoName() { return OsmSchemaCategory(PseudoName); }
  static OsmSchemaCategory multiUse() { return OsmSchemaCategory(Multiuse); }
  static OsmSchemaCategory combination() { return OsmSchemaCategory(Combination); }

  bool operator==(const OsmSchemaCategory& t) const { return t._type == _type; }
  bool operator!=(const OsmSchemaCategory& t) const { return t._type != _type; }

  Type getEnum() const { return _type; }

  static OsmSchemaCategory fromString(const QString& s)
  {
    if (s == "poi")
    {
      return Poi;
    }
    else if (s == "building")
    {
      return Building;
    }
    else if (s == "transportation")
    {
      return Transportation;
    }
    else if (s == "use")
    {
      return Use;
    }
    else if (s == "name")
    {
      return Name;
    }
    else if (s == "pseudoname")
    {
      return PseudoName;
    }
    else if (s == "multiuse")
    {
      return Multiuse;
    }
    else if (s == "combination")
    {
      return Combination;
    }
    else if (s == "")
    {
      return Empty;
    }
    else
    {
      throw IllegalArgumentException("Unknown category. " + s);
    }
  }

  static OsmSchemaCategory fromStringList(const QStringList& s);

  /**
   * Returns true if all of the categories in c are in this. If this is empty then it always returns
   * true.
   */
  bool contains(const OsmSchemaCategory& c) const
  {
    return (getEnum() & c.getEnum()) == c.getEnum();
  }

  /**
   * Returns true if any of the categories in c are the same as the categories in this. If one or
   * both are empty then it returns false.
   */
  bool intersects(const OsmSchemaCategory& c) const
  {
    return (getEnum() & c.getEnum()) != Empty;
  }

  QString toString() const { return toStringList().join(","); }

  QStringList toStringList() const
  {
    QStringList result;

    if (_type & Poi)
    {
      result << "poi";
    }
    if (_type & Building)
    {
      result << "building";
    }
    if (_type & Transportation)
    {
      result << "transportation";
    }
    if (_type & Use)
    {
      result << "use";
    }
    if (_type & Name)
    {
      result << "name";
    }
    if (_type & PseudoName)
    {
      result << "pseudoname";
    }
    if (_type & Multiuse)
    {
      result << "multiuse";
    }
    if (_type & Combination)
    {
      result << "combination";
    }

    return result;
  }

private:

  Type _type;
};

inline OsmSchemaCategory operator&(const OsmSchemaCategory& t1, const OsmSchemaCategory& t2)
{
  return OsmSchemaCategory((OsmSchemaCategory::Type)(t1.getEnum() & t2.getEnum()));
}

inline OsmSchemaCategory operator|(const OsmSchemaCategory& t1, const OsmSchemaCategory& t2)
{
  return OsmSchemaCategory((OsmSchemaCategory::Type)(t1.getEnum() | t2.getEnum()));
}

// explicitly put these types in their own name scope. Use with OsmGeometries::Node, etc.
struct OsmGeometries
{
  enum Type : uint16_t
  {
    Empty =       0x00,
    Node =        0x01,
    Area =        0x02,
    LineString =  0x04,
    ClosedWay =   0x08,
    Way =         Area | LineString | ClosedWay,
    Relation =    0x10,
    All =         Relation | Way | Node
  };

  static Type fromString(const QString& s);
};

struct TagEdge
{
public:

  TagEdge()
  {
    show = true;
  }

  enum EdgeType type;
  double similarToWeight;
  double averageWeight;
  bool show;
  double w1;
  double w2;
};

class OsmSchemaData;
class Relation;
class Way;

/**
 * This class is re-entrant, but not thread safe (Singleton).
 */
class OsmSchema
{
public:

  static OsmSchema& getInstance();
  ~OsmSchema() = default;

  /**
   * Loads the default configuration. This should only be used by unit tests.
   */
  void loadDefault();
  /**
   * ONLY FOR UNIT TESTING. Be a good neighbor and call loadDefault() when you're done.
   */
  void createTestingGraph() const;

  void addAssociatedWith(const QString& name1, const QString& name2) const;
  void addIsA(const QString& name1, const QString& name2) const;
  void addSimilarTo(
    const QString& name1, const QString& name2, double weight, bool oneway = false) const;

  void update() const;
  void updateOrCreateVertex(const SchemaVertex& tv) const;

  QString average(
    const QString& kvp1, double w1, const QString& kvp2, double w2, double& best) const;
  QString average(const QString& kvp1, const QString& kvp2, double& best) const;

  /**
   * Searches for the first common ancestor between two key value pairs. If there is no common
   * ancestor then an empty TagVertex is returned.
   */
  const SchemaVertex& getFirstCommonAncestor(const QString& kvp1, const QString& kvp2) const;
  bool isAncestor(const QString& childKvp, const QString& parentKvp) const;

  OsmSchemaCategory getCategories(const Tags& t) const;
  OsmSchemaCategory getCategories(const QString& k, const QString& v) const;
  OsmSchemaCategory getCategories(const QString& kvp) const;

  /**
   * Retrieves a set of tags that are associated with the input tags, as defined by the hoot schema
   *
   * @param tags tags to search associations for
   * @return a set of tags
   */
  Tags getAssociatedTags(const Tags& tags);
  /**
   * Retrieves all child tags for the given input tags
   *
   * @param tags tags for which to retrieve child tags
   * @return a set of tags
   */
  Tags getChildTags(const Tags& tags);
  /**
   * Retrieves tags similar to the input tag
   *
   * @param name a kvp
   * @param minimumScore tag similarity threshold
   * @return a set of tags
   */
  Tags getSimilarTags(const QString& name, double minimumScore);

  /**
   * Retrieves all possible tag keys from the schema
   *
   * @return a collection of key strings
   */
  QSet<QString> getAllTagKeys();
  /**
   * Retrieves all possible tag keys from the schema that are associated with a type
   *
   * @return a collection of key strings
   */
  QSet<QString> getAllTypeKeys();
  bool hasTagKey(const QString& key);
  /**
   * Returns a collection of tag key/value pairs that represent generic feature types
   *
   * @return a collection of tag key/value pairs
   */
  QSet<QString> getGenericKvps() const;
  /**
   * Determines tags are contained in a list of tags
   *
   * @param tags tags to examine
   * @param tagList string tag list to check against
   * @return true if any tag in a set of input tags is contained in a string list of tags; false
   * otherwise
   * @note If we came up with a way to support key=* wildcards in the Tags class, then we could
   * probably get rid of this method altogether and just check against the tags whereever this
   * method is being called.
   */
  bool containsTagFromList(const Tags& tags, const QStringList& tagList) const;
  /**
   * Retrieves tags that are aliases of the input tags
   *
   * @param tags tags to search aliases for
   * @return a set of tags
   */
  Tags getAliasTags(const Tags& tags);
  /**
   * Determine the ancestor between two tags
   *
   * @param kvp1 first tag to examine
   * @param kvp2 second tag to examine
   * @return tag that is the ancestor of the other or the first tag if neither is an ancestor of
   * each other
   */
  QString getParentKvp(const QString& kvp1, const QString& kvp2) const;

  std::vector<SchemaVertex> getAllTags() const;
  /**
   * Returns all tags that have a similar score >= minimumScore.
   *
   * minimumScore must be > 0.
   */
  std::vector<SchemaVertex> getSimilarTagsAsVertices(const QString& name, double minimumScore) const;
  std::vector<SchemaVertex> getTagByCategory(const OsmSchemaCategory& c) const;
  std::vector<SchemaVertex> getChildTagsAsVertices(const QString& name) const;
  std::vector<SchemaVertex> getAssociatedTagsAsVertices(const QString& name) const;
  /**
   * Returns the tag vertex for a given kvp. If the vertex is compound then an empty vertex will
   * be returned.
   */
  const SchemaVertex& getTagVertex(const QString& kvp) const;

  /**
   * Returns all schema vertices that are represented in the set of tags. This will return both
   * compound vertices and tag vertices.
   */
  std::vector<SchemaVertex> getSchemaVertices(const Tags& tags) const;
  /**
   * Returns all the schema vertices in the set of tags that do not also have parent vertices in the
   * set of tags. E.g. returns railway_platform, but not public_transit=platform.
   *
   * "Unique" may not be the best modifier in the method name, but "WithParentTagsRemoved" seemed
   * a bit verbose.
   */
  std::vector<SchemaVertex> getUniqueSchemaVertices(const Tags& tags) const;

  /**
   * Returns true if at least one tag in the set of specified tags is part of the specified
   * category.
   */
  bool hasCategory(const Tags& t, const QString& category) const;
  /**
   * Returns true if the specified kvp is part of the specified category.
   */
  bool hasCategory(const QString& kvp, const QString& category) const;
  /**
   * Returns true if at least one tag in the set of specified tags is part of the specified
   * category.
   */
  bool hasCategory(const Tags& t, const OsmSchemaCategory& category) const;
  /**
   * Returns true if the specified kvp is part of the specified category.
   */
  bool hasCategory(const QString& kvp, const OsmSchemaCategory& category) const;
  /**
   * Determines if the key is part of any category in the schema
   *
   * @param key tag key
   * @param val tag value
   * @return true if the tag is part of at least one schema category; false otherwise
   */
  bool hasAnyCategory(const QString& key, const QString& val) const;

  bool allowsFor(const Tags& t, const ElementType& type, OsmGeometries::Type geometries) const;
  bool allowsFor(const ConstElementPtr& e, OsmGeometries::Type geometries) const;

  /**
   * Returns true if this is a list of values. Right now this just looks for a semicolon in value,
   * but in the future the list of valid list keys may be stored in the schema file.
   */
  bool isList(const QString& key, const QString& value) const;
  /**
   * Returns true if the kvp contains metadata about the feature as opposed to real information
   * about the features.
   *
   * Tags such as "hoot:status", "source", "uuid" are all metadata.
   *
   * Tags such as "name", "highway" and "height" are not metadata.
   */
  bool isMetaData(const QString& key, const QString& value);
  /**
   * Returns true if this is a reversed unidirectional way. (E.g. oneway=reverse)
   */
  bool isReversed(const Element& e) const;
  /**
   * Return true if this tag can contain free-form text.
   */
  bool isTextTag(const QString& key) const;
  /**
   * Return true if this tag can contain numeric text.
   */
  bool isNumericTag(const QString& key) const;
  double getIsACost() const;

  double score(const QString& kvp1, const QString& kvp2) const;
  double score(const SchemaVertex& v1, const SchemaVertex& v2) const;
  /**
   * Scores a particular kvp against an element's tags
   *
   * @param kvp the key/value pair to compare against
   * @param tags the tags to compare against
   * @return the highest similarity score found in tags when compared to kvp
   */
  double score(const QString& kvp, const Tags& tags) const;
  /**
   * Scores the type similarity between two sets of tags
   *
   * @param tags1 the first set of tags to score
   * @param tags2 the second set of tags to score
   * @param ignoreGenericTypes if true, generic types such as 'poi=yes' are ignored during
   * comparison
   * @return a similarity score from 0.0 to 1.0
   */
  double scoreTypes(const Tags& tags1, const Tags& tags2, const bool ignoreGenericTypes = false);
  /**
   * @brief scoreOneWay Returns a oneway score. E.g. highway=primary is similar to highway=road,
   *  but a highway=road isn't necessarily similar to a highway=primary (so it gets a low score).
   */
  double scoreOneWay(const QString& kvp1, const QString& kvp2) const;

  /**
   * Returns the first type key/value pair found in a set of tags
   *
   * @param tags the tags to search
   * @param allowGeneric if true, kvps associated with generic types are returned
   * @return a single key/value pair string
   */
  QString getFirstType(const Tags& tags, const bool allowGeneric);

  /**
   * Determines if two sets of tags have an explicit type mismatch. Empty tags and generic types
   * are ignored during the comparison
   *
   * @param tags1 the first set of tags to compare
   * @param tags2 the second set of tags to compare
   * @param minTypeScore the minimum similarity score at or above which the two sets of tags must
   * score in to be considered a match
   * @return true if the tags explicitly mismatch; false otherwise
   */
  bool explicitTypeMismatch(const Tags& tags1, const Tags& tags2, const double minTypeScore);
  /**
   * Determines if two sets of tags have an explicit type mismatch. Empty tags and generic types
   * are not ignored during the comparison
   *
   * @param tags1 the first set of tags to compare
   * @param tags2 the second set of tags to compare
   * @param minTypeScore the minimum similarity score at or above which the two sets of tags must
   * score in to be considered a match
   * @return true if the tags mismatch; false otherwise
   */
  bool typeMismatch(const Tags& tags1, const Tags& tags2, const double minTypeScore);
  /**
   * Determines if a key/value pair represents a generic feature type
   *
   * @param kvp the key/value pair to examine
   * @return true if the input key/value pair represents a generic type; false otherwise
   */
  bool isGenericKvp(const QString& kvp) const;
  /**
   * Determines whether a set of tags represents a generic feature type
   *
   * @param tags the tags to examine
   * @return true if the tags contain only a generic feature type; false otherwise
   */
  bool isGeneric(const Tags& tags);
  /**
   * Determines if a set of tags has a feature type
   *
   * @param tags the tags to examine
   * @return true if the tags have at least one type tag
   */
  bool hasType(const Tags& tags);
  /**
   * Determines if a set of tags have more than one feature type
   *
   * @param tags the tags to examine
   * @return true if the tags have at least two type tags
   */
  bool hasMoreThanOneType(const Tags& tags);
  /**
   * Returns the most specific type in a set of tags according to the schema
   *
   * @param tags the tags to search
   * @return a single key/value pair string
   */
  QString mostSpecificType(const Tags& tags);
  /**
   * Determines if a tag key corresponds to a type in the schema
   *
   * @param key tag key to examine
   * @return true if the key corresponds to a type; false otherwise
   */
  bool isTypeKey(const QString& key);

  QString toGraphvizString() const;

  static QString toKvp(const QString& key, const QString& value);

  /**
   * Sets the cost when traversing up the tree to a parent node. This is useful for strict score
   * checking rather than equivalent tags.
   */
  void setIsACost(double cost) const;

private:

  friend class OsmSchemaTest;
  friend class SchemaCheckerTest;

  OsmSchema();

  // the templates we're including take a crazy long time to include, so I'm isolating the
  // implementation.
  std::shared_ptr<OsmSchemaData> _d;
  static std::shared_ptr<OsmSchema> _theInstance;
  SchemaVertex _empty;

  /// Provide caching for isMetaData
  QHash<QString, bool> _metadataKey;

  QSet<QString> _allTagKeysCache;
  QSet<QString> _allTypeKeysCache;
  mutable QSet<QString> _genericKvps;
};

}

#endif // OSMSCHEMA_H
