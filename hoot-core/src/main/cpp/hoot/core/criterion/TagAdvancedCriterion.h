
#ifndef TAG_ADVANCED_CRITERION_H
#define TAG_ADVANCED_CRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/schema/TagFilter.h>

// Qt
#include <QString>
#include <QMap>
#include <QRegExp>

namespace hoot
{

/**
 * Criterion allowing for advanced tag filtering concepts, as defined in the Hootenanny schema,
 * including:
 *
 * - tag key/value string wildcard
   - type tag similarity
   - aliases
   - children/ancestors
   - categories
   - associations
 */
class TagAdvancedCriterion : public ElementCriterion, public Configurable
{
public:

  static std::string className() { return "hoot::TagAdvancedCriterion"; }

  TagAdvancedCriterion();
  TagAdvancedCriterion(const QString filterJson);

  virtual bool isSatisfied(const ConstElementPtr& e) const;

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new TagAdvancedCriterion()); }

  void setConfiguration(const Settings& s);

  virtual QString getDescription() const
  { return "Identifies elements by tag using a set of advanced schema functionality"; }

private:

  // one filter for each type: must, must_not, and should
  QMap<QString, QList<TagFilter>> _tagFilters;
  boost::shared_ptr<QRegExp> _keyMatcher;
  boost::shared_ptr<QRegExp> _valueMatcher;

  void _parseFilterString(const QString filterJson);
  void _loadTagFilters(const QString tagFilterType, boost::shared_ptr<pt::ptree> propTree);

  bool _elementPassesTagFilter(const ConstElementPtr& e, const TagFilter& filter) const;
  bool _elementPassesMustTagFilters(const ConstElementPtr& e) const;
  bool _elementPassesMustNotTagFilters(const ConstElementPtr& e) const;
  bool _elementPassesShouldTagFilters(const ConstElementPtr& e) const;

  /*
   * Checks for a match between the kvp portion of the tag filter and the input tags
   */
  bool _filterMatchesAnyTag(const TagFilter& filter, const Tags& tags) const;
  /*
   * Looks for match other than a tag string match...basically all other type of matches;
   * associations, etc.
   */
  bool _hasAuxMatch(const ConstElementPtr& e, const TagFilter& filter, const QString type) const;
};

}

#endif // TAG_ADVANCED_CRITERION_H
