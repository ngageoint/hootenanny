
#ifndef TIKA_LANGUAGE_DETECTOR_H
#define TIKA_LANGUAGE_DETECTOR_H

// hoot
#include <hoot/rnd/language/LanguageDetector.h>

namespace hoot
{

/**
 * Detects source language for a given piece of text using Apache Tika
 *
 * http://tika.apache.org/
 *
 * TODO: may have to run as a service to avoid incurring model loading overhead
 */
class TikaLanguageDetector : public LanguageDetector
{

public:

  static std::string className() { return "hoot::TikaLanguageDetector"; }

  TikaLanguageDetector();
  ~TikaLanguageDetector();

  virtual QString detect(const QString text);

  virtual void setConfiguration(const Settings& conf);

  virtual long getDetectionsMade() const { return _numDetections; }

private:

  QString _classPath;

  long _numDetections;
};

}

#endif // TIKA_LANGUAGE_DETECTOR_H
