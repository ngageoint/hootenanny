
#ifndef OPEN_NLP_LANGUAGE_DETECTOR_H
#define OPEN_NLP_LANGUAGE_DETECTOR_H

// hoot
#include <hoot/rnd/language/LanguageDetector.h>

namespace hoot
{

/**
 * Detects source language for a given piece of text using Apache OpenNLP
 *
 * http://opennlp.apache.org/
 *
 * TODO: may have to run as a service to avoid incurring model loading overhead
 */
class OpenNlpLanguageDetector : public LanguageDetector
{

public:

  static std::string className() { return "hoot::OpenNlpLanguageDetector"; }

  OpenNlpLanguageDetector();
  ~OpenNlpLanguageDetector();

  virtual QString detect(const QString text);

  virtual void setConfiguration(const Settings& conf);

  virtual long getDetectionsMade() const { return _numDetections; }

private:

  QString _modelFile;
  QString _classPath;

  long _numDetections;
};

}

#endif // OPEN_NLP_LANGUAGE_DETECTOR_H
