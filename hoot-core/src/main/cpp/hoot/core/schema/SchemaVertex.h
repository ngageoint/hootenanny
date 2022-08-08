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
 * @copyright Copyright (C) 2015, 2017, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef __SCHEMA_VERTEX_H__
#define __SCHEMA_VERTEX_H__

// Standard
#include <stdint.h>

// Tgs
#include <tgs/StreamUtils.h>

// Hoot
#include <hoot/core/schema/KeyValuePair.h>

namespace hoot
{

class Tags;

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
 * Alternatively, some simple cases like highway=primary are sufficient in and of themselves.
 */
class SchemaVertex
{
public:

  enum VertexType
  {
    UnknownVertexType,
    Tag,
    Compound
  };

  using CompoundRule = QList<KeyValuePairPtr>;
  using CompoundRuleList = QList<CompoundRule>;

  SchemaVertex();

  /**
   * Add a compound rule. This must be a "Compound" type to be valid.
   */
  void addCompoundRule(const CompoundRule& rule);

  /**
   * Returns true if one or more of the compound rules match this tag.
   */
  bool isCompoundMatch(const Tags& t) const;
  bool isEmpty() const { return _name.isEmpty(); }
  /**
   * Returns ture if each of the elements in this rule matches one of the kvps in t.
   */
  static bool isMatch(const CompoundRule& rule, const Tags& t);
  bool isValid() const { return _type != UnknownVertexType; }

  VertexType getType() const { return _type; }
  /**
   * Yes, technically you could modify the contents of the rules returned. Please don't. Taking
   * this short cut we don't need to copy the rules into a proper const object each time.
   */
  const CompoundRuleList& getCompoundRules() const { return _compoundRules; }
  QString getName() const { return _name; }
  QString getDescription() const { return _description; }
  QString getKey() const { return _key; }
  QString getValue() const { return _value; }
  double getInfluence() const { return _influence; }
  double getChildWeight() const { return _childWeight; }
  double getMismatchScore() const { return _mismatchScore; }
  TagValueType getValueType() const { return _valueType; }
  QStringList getAliases() const { return _aliases; }
  QStringList getCategories() const { return _categories; }
  uint16_t getGeometries() const { return _geometries; }

  /**
   * Sets the name and does not parse out the key/value. The key and value will not be changed.
   */
  void setName(const QString& n) { _name = n; }
  void setDescription(const QString& d) { _description = d; }
  /**
   * Sets the name and parses out and stores the key/value.
   */
  void setNameKvp(const QString& n);
  void setType(const VertexType& t);
  void setValueTypeString(const QString& t);
  void setKey(const QString& k) { _key = k; }
  void setValue(const QString& v) { _value = v; }
  void setInfluence(double i) { _influence = i; }
  void setChildWeight(double c) { _childWeight = c; }
  void setMismatchScore(double s) { _mismatchScore = s; }
  void setValueType(const TagValueType& t) { _valueType = t; }
  void setAliases(const QStringList& a) { _aliases = a; }
  void setCategories(const QStringList& c) { _categories = c; }
  void addCategory(const QString& category) { _categories.append(category); }
  void setGeometries(uint16_t g) { _geometries = g; }

  QString toString() const;

private:

  VertexType _type;
  CompoundRuleList _compoundRules;

  QString _name;
  QString _description;
  QString _key;
  QString _value;
  double _influence; // What is this used for?
  double _childWeight;

  /**
   * The mismatchScore is used only with wildcard enumerated types. (e.g. addr:housenumber=*).
   * This score is the score returned when two wildcard enumerated types are compared that have
   * different values.
   */
  double _mismatchScore;
  enum TagValueType _valueType;
  QStringList _aliases;

  /**
   * Each tag can have categories associated with it. This can help when grouping a number of
   * disparate tags into a category. E.g. building=school, amenity=restaurant and tourism=attraction
   * are all POIs, but don't fit neatly into a single hierarchy.
   *
   * If a category is set on an ancestor then it is also set in the category tag.
   */
  QStringList _categories;
  uint16_t _geometries;
};

}

#endif // __SCHEMA_VERTEX_H__
