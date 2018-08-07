

#ifndef TO_ENGLISH_TRANSLATOR_H
#define TO_ENGLISH_TRANSLATOR_H

// Qt
#include <QString>

namespace hoot
{

/**
 * Simple interface for a to English language translator
 */
class ToEnglishTranslator
{
public:

  /**
   * Set the source language for the translator
   *
   * @param langCode an ISO-639-1 language code
   */
  virtual void setSourceLanguage(const QString langCode) = 0;

  /**
   * Translates text to English
   *
   * @param textToTranslate the text to translate
   */
  virtual void translate(const QString textToTranslate) = 0;

  /**
   * Returns text that has been translated to English by translate
   *
   * @return translated text
   */
  virtual QString getTranslatedText() const = 0;
};

}

#endif // TO_ENGLISH_TRANSLATOR_H
