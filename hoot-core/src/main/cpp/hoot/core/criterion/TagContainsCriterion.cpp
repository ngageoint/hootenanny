
#include "TagContainsCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, TagContainsCriterion)

TagContainsCriterion::TagContainsCriterion(QString key, QString valueSubstring)
{
  _key.append(key);
  _valueSubstring.append(valueSubstring);
}

TagContainsCriterion::TagContainsCriterion(QStringList keys, QStringList valueSubstrings)
{
  _key = keys;
  _valueSubstring = valueSubstrings;
}

bool TagContainsCriterion::isSatisfied(const boost::shared_ptr<const Element> &e) const
{
  bool matches = false;
  for (int i = 0; i < _key.size(); i++)
  {
    if (e->getTags().contains(_key[i]) && e->getTags()[_key[i]].contains(_valueSubstring[i]))
    {
      matches = true;
      break;  //  Only one match is required
    }
  }
  return matches;
}

void TagContainsCriterion::addPair(QString key, QString valueSubstring)
{
  _key.append(key);
  _valueSubstring.append(valueSubstring);
}

}
