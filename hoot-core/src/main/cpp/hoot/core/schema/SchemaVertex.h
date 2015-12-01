#ifndef __SCHEMA_VERTEX_H__
#define __SCHEMA_VERTEX_H__

// hoot
#include <hoot/core/elements/Tags.h>
#include <hoot/core/schema/KeyValuePair.h>

// Qt
#include <QString>
#include <QStringList>

// Standard
#include <stdint.h>

// Tgs
#include <tgs/StreamUtils.h>

namespace hoot
{

enum TagValueType
{
  Boolean,
  Text,
  Enumeration,
  Int,
  Real,
  Unknown
};

/**
 * A tag concept represents a single concept that is represented in one or more tags. By grouping
 * multiple tags into a single concept we can show that two or more tags that are required may
 * represent a single concept. For example:
 *
 * - public_transport=platform (a concept of a public platform)
 * - public_transport=platform;bus=yes (another more refined concept of above)
 * - public_transport=platform;railway=yes (another concept that isn't compatible with bus above)
 * - highway=road;bus=yes (another concept that isn't compatible with bus above)
 *
 * In the cases above just because one tag matches in the set doesn't mean that there is a
 * similarity. You must have both tags matching to adequately represent the concept.
 *
 * Alternatively some simple cases like highway=primary are sufficient in and of themselves.
 */
class SchemaVertex
{
public:
  typedef enum VertexType
  {
    UnknownVertexType,
    Tag,
    Compound
  } VertexType;

  typedef QList<KeyValuePairPtr> CompoundRule;
  typedef QList<CompoundRule> CompoundRuleList;

  SchemaVertex();

  /**
   * Add a compound rule. This must be a "Compound" type to be valid.
   */
  void addCompoundRule(const CompoundRule& rule);

  /**
   * Yes, technically you could modify the contents of the rules returned. Please don't. Taking
   * this short cut we don't need to copy the rules into a proper const object each time.
   */
  const CompoundRuleList& getCompoundRules() const { return _compoundRules; }

  VertexType getType() const { return _type; }

  /**
   * Returns true if one or more of the compound rules match this tag.
   */
  bool isCompoundMatch(const Tags& t) const;

  bool isEmpty() const { return name.isEmpty(); }

  /**
   * Returns ture if each of the elements in this rule matches one of the kvps in t.
   */
  static bool isMatch(const CompoundRule& rule, const Tags& t);

  bool isValid() const { return _type != UnknownVertexType; }

  /**
   * Sets the name and does not parse out the key/value. The key and value will not be changed.
   */
  void setName(QString n);

  /**
   * Sets the name and parses out and stores the key/value.
   */
  void setNameKvp(QString n);

  void setType(VertexType t);

  void setValueTypeString(QString t);

  QString toString() const;

  QString name;
  QString description;
  QString key;
  QString value;
  double influence;
  double childWeight;

  /**
   * The mismatchScore is used only with wildcard enumerated types. (e.g. addr:housenumber=*).
   * This score is the score returned when two wildcard enumerated types are compared that have
   * different values.
   */
  double mismatchScore;
  enum TagValueType valueType;
  QStringList aliases;

  /**
   * Each tag can have categories associated with it. This can help when grouping a number of
   * disparate tags into a category. E.g. building=school, amenity=restaurant and tourism=attraction
   * are all POIs, but don't fit neatly into a single hierarchy.
   *
   * If a category is set on an ancestor then it is also set in the category tag.
   */
  QStringList categories;
  uint16_t geometries;

private:
  VertexType _type;
  CompoundRuleList _compoundRules;
};

}

#endif // __SCHEMA_VERTEX_H__
