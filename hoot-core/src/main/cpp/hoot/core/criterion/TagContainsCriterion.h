
#ifndef TAGCONTAINSCRITERION_H
#define TAGCONTAINSCRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>

// Qt
#include <QStringList>

namespace hoot
{

/**
 * Identifies elements by tag key and tag value substring
 */
class TagContainsCriterion : public ElementCriterion
{
public:

  static std::string className() { return "hoot::TagContainsCriterion"; }

  TagContainsCriterion() {}
  TagContainsCriterion(QString key, QString valueSubstring);
  TagContainsCriterion(QStringList keys, QStringList valueSubstrings);

  virtual bool isSatisfied(const boost::shared_ptr<const Element> &e) const;

  /**
    * Adds an additional pair to the search list. If any one of the pairs matches then it is
    * considered a match.
    */
   void addPair(QString key, QString valueSubstring);

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new TagContainsCriterion()); }

  virtual QString getDescription() const
  { return "Identifies elements by tag key and tag value substring"; }

private:

  QStringList _key, _valueSubstring;
};

}

#endif // TAGCONTAINSCRITERION_H
