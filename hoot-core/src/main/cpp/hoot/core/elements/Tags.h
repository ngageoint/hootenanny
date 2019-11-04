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
#ifndef TAGS_H
#define TAGS_H

// Hoot
#include <hoot/core/util/Units.h>
#include <hoot/core/schema/SchemaVertex.h>

// Qt
#include <QHash>
#include <QString>
#include <QStringList>

// Standard
// needed by some dev environments
#include <stdint.h>
#include <string>

namespace hoot
{

/**
 * Contains a set of key/value pairs. Consistent with the OSM way of doing things [1] if there are
 * multiple values with a single key, the values are semi-colon delimited. There is currently
 * no way to differentiate between a value that happens to contain a semi-colon and a list, although
 * using ;; has been proposed it isn't in use. Given that I haven't found any instances where this
 * is an issue, I'm not going to worry about it at this time.
 *
 * 1. http://wiki.openstreetmap.org/wiki/Semi-colon_value_separator
 */
class Tags : public QHash<QString, QString>
{
public:

  static std::string className() { return "hoot::Tags"; }
  static QString uuidKey() { return "uuid"; }

  Tags();
  Tags(const QString& key, const QString& value);

  void addNote(const QString& note);

  /**
   * Adds all the tags in t into this set of tags. Tags in t overwrite tags in this.
   */
  void add(const Tags& t);

  /**
   * Appends a value to a key. If the key already has a value then the values are semi-colon
   * delimited. If the key has no value then it is equivalent to simply setting the tag.
   */
  void appendValue(const QString& k, const QString& v);
  void appendValue(const QString& k, double v) { appendValue(k, QString::number(v)); }
  void appendValue(const QString& k, const QStringList& v);
  void appendValue(const QString& kvp);

  /**
   * Appends a value to a key. If the key already has a value then the values are semi-colon
   * delimited. If the key has no value then it is equivalent to simply setting the tag.
   *
   * The value is only appended if it doesn't already exist in the list.
   */
  void appendValueIfUnique(const QString& k, const QString& v);
  void appendValueIfUnique(const QString& k, const QStringList& v);

  /**
   * Return if there is at least one non-metadata tag. @sa getInformationalCount()
   */
  bool hasInformationTag() const;

  const QString get(const QString& k) const { return operator[](k); }

  /**
   * Gets the element's UUID(s). If the UUID doesn't exist then it creates a new UUID.
   *
   * Given that elements can be conflated and all the old UUIDs are maintained, you may get a list
   * of multiple UUIDs. All of these UUIDs reference the same element.
   */
  QStringList getCreateUuid();

  /**
   * Returns a double version of the value associated with k. If it cannot be parsed into a double
   * then an exception is thrown.
   */
  double getDouble(const QString& k) const;
  double getDouble(const QString& k, double defaultValue) const;

  /**
   * Returns the list of values associated with the key k. If k starts with regex? then all values
   * associated with the regular expression after regex? are returned. E.g. regex?a.* will return
   * all values that start with a. The order of the results is arbitrary.
   */
  QStringList getList(const QString& k) const;

  /**
   * Given a list of keys (that may include regex values), find all the keys in this Tags set that
   * match one or more entries in the list and return it. Each key will only be returned at most
   * once.
   */
  QStringList getMatchingKeys(const QStringList& k);

  /**
   * Return all the keys that are used in names.
   */
  static const QStringList& getNameKeys();

  /**
   * Get name keys for a set of tags
   *
   * @param tags tags to parse name keys from
   * @return a list of name keys
   */
  static QStringList getNameKeys(const Tags& tags);

  /**
   * Return all the names for the current feature.
   *
   * @param includeAltName if true, returns names with the alt_name tag key
   */
  QStringList getNames(const bool includeAltName = true) const;

  /**
   * Returns a name given tags
   *
   * @return the first name found, if one is present
   */
  QString getName() const;

  /**
   * Returns the speed in standard units (m/s).
   */
  Velocity getVelocity(const QString& k) const;

  /**
   * Returns the length in standard units (m).
   */
  Length getLength(const QString& k) const;

  /**
   * Return the number of tags that contain actual information, not meta-data or debug tags.
   * Meta-data tags include things like source and license. Debug is anything that starts with
   * hoot:. An example of an informational tag is name or highway.
   */
  int getInformationCount() const;

  int getNonDebugCount() const;

  /**
   * Return all the names associated with the feature. These are more loosely defined and may
   * include things like operator name and brand name. See conf/NameSchema.json for a definition.
   */
  const QStringList& getPseudoNameKeys() const;
  QStringList getPseudoNames() const;

  /**
   * Checks for common false values such as "no", "false", "0", "off".
   *
   * Note: It is not guaranteed that isFalse(tag) != isTrue(tag), but you are guaranteed that if
   * one of them is true the other will be false. E.g. value=apple will return false for both
   * methods.
   */
  bool isFalse(const QString& key) const;

  /**
   * Checks for common true values such as "yes", "true", "1", "on".
   *
   * Note: It is not guaranteed that isFalse(tag) != isTrue(tag), but you are guaranteed that if
   * one of them is true the other will be false. E.g. value=apple will return false for both
   * methods.
   */
  bool isTrue(const QString& key) const;

  bool operator==(const Tags& other) const;
  inline bool operator!=(const Tags& other) const { return !(*this == other); }

  /**
   * Similar to operator== but 'hoot::*' tags are ignored
   */
  bool dataOnlyEqual(const Tags& other) const;

  /**
   * Get a list of all non-'hoot::*' tags
   */
  QStringList getDataOnlyValues(const Tags& tags) const;

  void readValues(const QString& k, QStringList& list) const;

  Meters readMeters(const QString& key) const;

  /**
   * Remove all tags with empty strings as values.
   */
  void removeEmpty();

  /**
   * Removes all metadata tags (hoot::*)
   */
  void removeMetadata();

  // QHash::remove can be used for removal by key equal to.

  /**
   * Removes all tags with keys that contain the input substring
   *
   * @param tagKeySubstring a substring to match
   */
  void removeByTagKeyContains(const QString& tagKeySubstring);

  /**
   * Removes all tags with keys that start with the input substring
   *
   * @param tagKeySubstring a substring to match
   */
  void removeByTagKeyStartsWith(const QString& tagKeySubstring);

  /**
   * Sets the area tag to either true (is an area element), or false (is not an area element)
   */
  void setArea(bool area) { insert("area", area ? "yes" : "no"); }

  void set(const QString& key, const char* value) { set(key, QString(value)); }
  void set(const QString& key, const QString& value);
  void set(const QString& key, bool v);
  void set(const QString& key, double v) { set(key, QString::number(v)); }
  void set(const QString& key, int v) { set(key, QString::number(v)); }
  void set(const QString& key, int64_t v) { set(key, QString::number(v)); }

  /**
   * If a kvp is in other, then set it in this. If this already has that key then the value will
   * be overwritten.
   */
  void set(const Tags& other);

  /**
   * Sets a value using the strings in the specified iterator. The values in the iterator will
   * be concatenated into a list using semi-colons. [1]
   */
  template<class T>
  void set(const QString& key, T it, T end)
  {
    if (it == end)
    {
      remove(key);
      return;
    }

    QString v = (*it);
    QString value = v.replace(";", ";;");
    ++it;
    while (it != end)
    {
      v = (*it);
      value += ";" + v.replace(";", ";;");;
      ++it;
    }
    insert(key, value);
  }

  void setList(const QString& key, const QStringList& l) { set(key, l.constBegin(), l.constEnd()); }

  /**
   * Splits the specified value up into individual values. For now it just splits on ";", but it
   * may get more robust later.
   */
  static QStringList split(const QString& values);

  QString toString() const;

  /**
   * Returns true if the tags have any key=value in the input list
   *
   * @param kvps kvps to search for
   * @return true if tags contain at least one kvp; false otherwise
   */
  bool hasAnyKvp(const QStringList& kvps) const;

  /**
   * Returns true if the tags have any key in the input list
   *
   * @param keys tag keys to search for
   * @return true if the tags contain at least one of the keys; false otherwise
   */
  bool hasAnyKey(const QStringList& keys);

  /**
   * Converts a list of KVPs into tags
   *
   * @param kvps kvps to convert
   * @return a set of tags
   */
  static Tags kvpListToTags(const QStringList& kvps);

  /**
   * Converts a collection of schema vertices to tags
   *
   * @param vertices schema vertices to convert
   * @return a set of tags
   */
  static Tags schemaVerticesToTags(const std::vector<SchemaVertex>& schemaVertices);

  /**
   * Determines if two set of tags have a matching name
   *
   * @param tags1 first set of tags to examine
   * @param tags2 second set of tags to examine
   * @parm strictNameMatch if true, will not consider names with the alt_name tag key
   * @return true if the tags have at least one matching name; false otherwise
   */
  static bool haveMatchingName(const Tags& tags1, const Tags& tags2,
                               const bool strictNameMatch = false);

  /**
   * Determines whether a name exists in the set of tag
   *
   * @return true if at least one name exists; false otherwise
   */
  bool hasName() const;

  /**
   * Returns a string showing the difference between this set of tags and another
   *
   * @param other set of tags to compare these tags to
   * @return a tag diff string
   */
  QString getDiffString(const Tags& other) const;

private:

  void _valueRegexParser(const QString& str, QString& num, QString& units) const;

  static QStringList _nameKeys;
  static QStringList _pseudoNameKeys;
};

}

#endif // TAGS_H
