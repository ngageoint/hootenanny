

#ifndef TO_ENGLISH_TRANSLATOR_H
#define TO_ENGLISH_TRANSLATOR_H

// Hoot
#include <hoot/core/util/Configurable.h>

// Qt
#include <QString>

namespace hoot
{

/**
 * Interface for a to English language translator
 */
class ToEnglishTranslator : public Configurable
{
public:

  static std::string className() { return "hoot::ToEnglishTranslator"; }

  /**
   * Returns the translators source languages
   *
   * @return list of ISO-639-1 language codes
   */
  virtual QStringList getSourceLanguages() const = 0;

  /**
   * Set the source language for the translator
   *
   * @param langCode an ISO-639-1 language code
   */
  virtual void setSourceLanguages(const QStringList langCodes) = 0;

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

  virtual bool detectionMade() const = 0;
};

}

#endif // TO_ENGLISH_TRANSLATOR_H
