

#ifndef LANGUAGE_TRANSLATOR_H
#define LANGUAGE_TRANSLATOR_H

// Qt
#include <QString>

namespace hoot
{

/**
 *
 */
class ToEnglishTranslator
{
public:

  virtual QString translate(const QString toTranslate) const = 0;
  virtual void setSourceLanguage(const QString langCode);
};

}

#endif // LANGUAGE_TRANSLATOR_H
