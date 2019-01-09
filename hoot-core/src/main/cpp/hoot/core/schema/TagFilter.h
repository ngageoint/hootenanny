
#ifndef TAG_FILTER_H
#define TAG_FILTER_H

// Hoot
#include <hoot/core/schema/OsmSchema.h>

// Qt
#include <QString>

// Boost
#include <boost/property_tree/json_parser.hpp>
namespace pt = boost::property_tree;

namespace hoot
{

/**
 *
 */
class TagFilter
{

public:

  TagFilter(const QString key, const QString value);

  QString getKey() const { return _key; }
  void setKey(QString key) { _key = key; }

  QString getValue() const { return _value; }
  void setValue(QString val) { _value = val; }

  bool getAllowAliases() const { return _allowAliases; }
  void setAllowAliases(bool allow) { _allowAliases = allow; }

  double getSimilarityThreshold() const { return _similarityThreshold; }
  void setSimilarityThreshold(double threshold);

  bool getAllowChildren() const { return _allowChildren; }
  void setAllowChildren(bool allow) { _allowChildren = allow; }

  bool getAllowAncestors() const { return _allowAncestors; }
  void setAllowAncestors(bool allow) { _allowAncestors = allow; }

  bool getAllowAssociations() const { return _allowAssociations; }
  void setAllowAssociations(bool allow) { _allowAssociations = allow; }

  OsmSchemaCategory getCategory() const { return _category; }
  void setCategory(OsmSchemaCategory category) { _category = category; }

  static TagFilter fromJson(const pt::ptree::value_type& tagFilterPart);

  QString toString() const;

private:

  friend class TagFilterTest;

  QString _key;
  QString _value;
  bool _allowAliases;
  double _similarityThreshold;
  bool _allowChildren;
  bool _allowAncestors;
  bool _allowAssociations;
  OsmSchemaCategory _category;
};

}

#endif // TAG_FILTER_H
