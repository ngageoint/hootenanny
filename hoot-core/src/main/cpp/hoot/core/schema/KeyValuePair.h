#ifndef KEYVALUEPAIR_H
#define KEYVALUEPAIR_H

// Hoot
#include <hoot/core/util/HootException.h>

// Qt
#include <QString>

namespace hoot
{

class KeyValuePair
{
public:
  KeyValuePair(QString kvp)
  {
    // this is faster than using split
    int index = kvp.indexOf('=');

    if (index == -1)
    {
      throw IllegalArgumentException("The key value pair must contain an equal sign.");
    }

    _name = kvp;
    _key = kvp.left(index);
    _value = kvp.mid(index + 1);

    // not supported at this time.
    if (_key.startsWith("regex?") || _value.startsWith("regex?"))
    {
      throw IllegalArgumentException("A key value pair cannot be a regular expression.");
    }
  }

  QString getKey() const { return _key; }
  QString getName() const { return _name; }
  QString getValue() const { return _value; }

  bool isMatch(const QString& k, const QString& v) { return _key == k && _value == v; }

  QString toString() const { return _name; }

private:
  QString _name;
  QString _key;
  QString _value;
};

typedef boost::shared_ptr<KeyValuePair> KeyValuePairPtr;
typedef boost::shared_ptr<const KeyValuePair> ConstKeyValuePairPtr;

}

#endif // KEYVALUEPAIR_H
