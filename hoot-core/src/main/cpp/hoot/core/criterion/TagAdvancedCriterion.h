
#ifndef TAG_ADVANCED_CRITERION_H
#define TAG_ADVANCED_CRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/schema/TagFilter.h>

// Qt
#include <QString>
#include <QMap>

namespace hoot
{

/**
 *
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
  { return "TODO"; }

private:

  QMap<QString, QList<TagFilter>> _tagFilters;

  void _parseFilterString(const QString filterJson);
  void _loadTagFilters(const QString tagFilterType, boost::shared_ptr<pt::ptree> propTree);

  bool _elementPassesTagFilter(const ConstElementPtr& e, const TagFilter& filter) const;
  bool _elementPassesMustTagFilters(const ConstElementPtr& e) const;
  bool _elementPassesMustNotTagFilters(const ConstElementPtr& e) const;
  bool _elementPassesShouldTagFilters(const ConstElementPtr& e) const;
  bool _filterMatchesAnyTag(const TagFilter& filter, const Tags& tags) const;
  bool _hasAuxMatch(const ConstElementPtr& e, const TagFilter& filter, const QString type) const;
};

}

#endif // TAG_ADVANCED_CRITERION_H
