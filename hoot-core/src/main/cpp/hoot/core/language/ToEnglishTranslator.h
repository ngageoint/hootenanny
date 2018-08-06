

#ifndef TO_ENGLISH_TRANSLATOR_H
#define TO_ENGLISH_TRANSLATOR_H

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

  virtual ~ToEnglishTranslator() {}

  virtual void setSourceLanguage(const QString langCode) = 0;
  virtual void translate(const QString textToTranslate) = 0;
  virtual QString getTranslatedText() const = 0;
};

}

#endif // TO_ENGLISH_TRANSLATOR_H
