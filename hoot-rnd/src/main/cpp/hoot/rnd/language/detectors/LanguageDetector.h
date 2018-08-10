

#ifndef LANGUAGE_DETECTOR_H
#define LANGUAGE_DETECTOR_H

// Qt
#include <QString>

namespace hoot
{

/**
 * Interface for classes who can detect a language based on sample text from that language
 */
class LanguageDetector
{
public:

  static std::string className() { return "hoot::LanguageDetector"; }

  /**
   * Detects a language given text
   *
   * @param text text to determine the source language from
   * @return an ISO-639-1 language code (?)
   */
  virtual QString detect(const QString text) = 0;

  /**
   * Returns the number of language detections made by this detector
   *
   * @return number of language detections made
   */
  virtual long getDetectionsMade() const = 0;
};

}

#endif // LANGUAGE_DETECTOR_H
