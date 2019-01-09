
#include "TagFilter.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/HootException.h>

namespace hoot
{

TagFilter::TagFilter(const QString key, const QString value) :
_key(key),
_value(value),
_allowAliases(false),
_similarityThreshold(false),
_allowChildren(false),
_allowAncestors(false),
_allowAssociations(false),
_category(OsmSchemaCategory::Empty)
{
  LOG_VART(_key);
  LOG_VART(_value);

  if (_key.isEmpty())
  {
    throw IllegalArgumentException("Invalid tag filter tag key: " + _key);
  }
  if (_value.isEmpty())
  {
    throw IllegalArgumentException("Invalid tag filter tag value: " + _value);
  }
}

void TagFilter::setSimilarityThreshold(double threshold)
{
  if (threshold != -1.0 && (threshold < 0.001 || threshold > 1.0))
  {
    throw IllegalArgumentException(
      "Invalid tag filter similarity threshold: " + QString::number(threshold));
  }

  _similarityThreshold = threshold;
}

TagFilter TagFilter::fromJson(const pt::ptree::value_type& tagFilterPart)
{
  /*
   * a non-sensical and contradictory example, but it illustrates all the possible types of
   * filter inputs
   *
   * {
       "must":
       [
         {
           "tag": "tourism=hotel",
           "allowAliases": "true"
         },
         {
           "tag": "surface=gravel",
           "allowChildren": "true"
         },
         {
           "tag": "*",
           "category": "transportation"
         },
         {
           "tag": "building:part=yes",
           "allowAssociations": "true"
         }
       ]
       "should":
       [
         {
           "tag": "amenity=restaurant",
           "similarityThreshold": "0.8"
         },
         {
           "tag": "amenity=place_of_worship"
         },
         {
           "tag": "*address*=*"
         },
         {
           "tag": "poi*=*"
         },
         {
           "tag": "*building=*"
         },
         {
           "tag": "*=*address*"
         },
         {
           "tag": "*=poi*"
         },
         {
           "tag": "*=*building"
         }
       ],
       "must_not":
       [
         {
           "tag": "amenity=chapel"
         },
         {
           "tag": "highway=secondary",
           "allowAncestors": "true"
         }
       ]
     }
   */

  bool categorySpecified = false;
  QString categoryStr = "";
  boost::optional<std::string> categoryProp =
    tagFilterPart.second.get_optional<std::string>("category");
  if (categoryProp)
  {
    categoryStr = QString::fromStdString(categoryProp.get());
  }
  OsmSchemaCategory category(OsmSchemaCategory::fromString(categoryStr));
  if (category != OsmSchemaCategory::Empty)
  {
    categorySpecified = true;
  }
  LOG_VART(category);

  boost::optional<std::string> filterProp =
    tagFilterPart.second.get_optional<std::string>("tag");
  // We allow missing wildcard tag filters when a category is specified only.
  if (!filterProp && category == OsmSchemaCategory::Empty)
  {
    throw IllegalArgumentException("Invalid tag filter.");
  }
  QString key = "*";
  QString value = "*";
  if (filterProp)
  {
    const QString filter = QString::fromStdString(filterProp.get());
    LOG_VART(filter);
    if (category == OsmSchemaCategory::Empty &&
        (filter.trimmed().isEmpty() || !filter.contains("=")))
    {
      throw IllegalArgumentException("Invalid tag filter: " + filter);
    }
    const QStringList filterParts = filter.split("=");
    key = filterParts[0].trimmed().toLower();
    LOG_VART(key);
    value = filterParts[1].trimmed().toLower();
    LOG_VART(value);
  }
  else if (!categorySpecified)
  {
    throw IllegalArgumentException(
      "If no tag filter is specified a category filter must be specified.");
  }

  bool allowAliases = false;
  boost::optional<bool> allowAliasesProp = tagFilterPart.second.get_optional<bool>("allowAliases");
  if (allowAliasesProp)
  {
    allowAliases = allowAliasesProp.get();
  }
  LOG_VART(allowAliases);

  double similarityThreshold = -1.0;
  boost::optional<double> similarityThresholdProp =
    tagFilterPart.second.get_optional<double>("similarityThreshold");
  if (similarityThresholdProp)
  {
    similarityThreshold = similarityThresholdProp.get();
  }
  LOG_VART(similarityThreshold);

  bool allowChildren = false;
  boost::optional<bool> allowChildrenProp =
    tagFilterPart.second.get_optional<bool>("allowChildren");
  if (allowChildrenProp)
  {
    allowChildren = allowChildrenProp.get();
  }
  LOG_VART(allowChildren);

  bool allowAncestors = false;
  boost::optional<bool> allowAncestorsProp =
    tagFilterPart.second.get_optional<bool>("allowAncestors");
  if (allowAncestorsProp)
  {
    allowAncestors = allowAncestorsProp.get();
  }
  LOG_VART(allowAncestors);

  bool allowAssociations = false;
  boost::optional<bool> allowAssociationsProp =
    tagFilterPart.second.get_optional<bool>("allowAssociations");
  if (allowAssociationsProp)
  {
    allowAssociations = allowAssociationsProp.get();
  }
  LOG_VART(allowAssociations);

  TagFilter filter(key, value);
  filter.setAllowAliases(allowAliases);
  filter.setSimilarityThreshold(similarityThreshold);
  filter.setAllowChildren(allowChildren);
  filter.setAllowAncestors(allowAncestors);
  filter.setAllowAssociations(allowAssociations);
  filter.setCategory(category);
  return filter;
}

QString TagFilter::toString() const
{
  QString str = "filter: " + _key + "=" + _value + ", allowAliases: ";
  if (_allowAliases)
  {
    str += "true";
  }
  else
  {
    str += "false";
  }
  if (_similarityThreshold != -1.0)
  {
    str += ", similarity threshold: " + QString::number(_similarityThreshold);
  }
  str += ", allowChildren: ";
  if (_allowChildren)
  {
    str += "true";
  }
  else
  {
    str += "false";
  }
  str += ", allowAncestors: ";
  if (_allowAncestors)
  {
    str += "true";
  }
  else
  {
    str += "false";
  }
  str += ", allowAssociations: ";
  if (_allowAssociations)
  {
    str += "true";
  }
  else
  {
    str += "false";
  }
  if (_category != OsmSchemaCategory::Empty)
  {
    str += ", category: " + _category.toString();
  }
  return str;
}

}
