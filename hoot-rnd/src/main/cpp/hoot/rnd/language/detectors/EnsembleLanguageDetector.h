
#ifndef ENSEMBLE_LANGUAGE_DETECTOR_H
#define ENSEMBLE_LANGUAGE_DETECTOR_H

// hoot
#include <hoot/rnd/language/detectors/LanguageDetector.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Detects source language for a given piece of text using multiple language detectors
 */
class EnsembleLanguageDetector : public LanguageDetector, public Configurable
{

public:

  static std::string className() { return "hoot::ConglomerateLanguageDetector"; }

  EnsembleLanguageDetector();

  virtual QString detect(const QString text);

  virtual void setConfiguration(const Settings& conf);

  virtual long getDetectionsMade() const { return _numDetections; }

private:

  QStringList _detectors;
  QString _url;
  long _numDetections;
};

}

#endif // ENSEMBLE_LANGUAGE_DETECTOR_H
