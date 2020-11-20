/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef HOOT_SERVICES_LANGUAGE_DETECTOR_CLIENT_H
#define HOOT_SERVICES_LANGUAGE_DETECTOR_CLIENT_H

// hoot
#include <hoot/rnd/language/LanguageDetector.h>
#include "LanguageDetectionConfidenceLevel.h"

// Qt
#include <QCache>
#include <QMap>
#include <QSet>

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

namespace hoot
{

class HootNetworkCookieJar;

struct DetectionResult
{
  QString sourceText;
  QString detectedLangCode;
};

/**
 * Detects non-English languages given using language detection technologies integrated into the
 * Hootenanny web services.
 */
class HootServicesLanguageDetectorClient : public LanguageDetector
{

public:

  static std::string className() { return "hoot::HootServicesLanguageDetectorClient"; }

  HootServicesLanguageDetectorClient();
  virtual ~HootServicesLanguageDetectorClient();

  virtual void setConfiguration(const Settings& conf);

  /**
   * @see LanguageDetector
   */
  virtual QString detect(const QString& text) override;

protected:

  //determines whether cookies are used in web requests; should be set to true to support auth,
  //except when running tests as they don't make web requests that require auth
  bool _useCookies;

  long _numDetectionsAttempted;
  long _numDetectionsMade;
  long _numEnglishTextsSkipped;

  bool _skipWordsInEnglishDict;

private:

  friend class HootServicesLanguageDetectorClientTest;

  //detector implementations to use server side; if left empty, the server will try to use as many
  //of them as necessary to detect the language
  QStringList _detectors;

  std::shared_ptr<QCache<QString, DetectionResult>> _cache;

  int _statusUpdateInterval;

  //number of words that were deemed untranslatable (contained numbers, etc.)
  long _undetectableWords;
  //have some issues where the server returns lang codes w/ no lang names due to the detector
  //being able to detect more langs than advertise; can use this to track down those instances and
  //properly update the server side lang configs
  QMap<QString, QSet<QString>> _langCodesWithNoLangNamesAvailable;
  //number of each time of detection confidence returned
  QMap<QString, int> _confidenceCounts;
  //the minimum allowed confidence level to declare a valid detected language (not all server side
  //detectors support confidence)
  LanguageDetectionConfidenceLevel _minConfidence;
  //the number of times different server side detectors were used
  QMap<QString, int> _detectorUsedCounts;

  long _cacheHits;
  long _cacheSize;
  long _cacheMaxSize;

  //if true, we've hit our cache limit and it will start flushing older entries
  static bool _loggedCacheMaxReached;

  //persistent session is required by hoot services
  std::shared_ptr<HootNetworkCookieJar> _cookies;

  static QString _getDetectUrl();

  QString _getRequestData(const QString& text) const;
  QString _parseResponse(const std::shared_ptr<boost::property_tree::ptree>& replyObj,
                         QString& detectorUsed);

  QString _getLangFromCache(const QString& text);
  void _insertLangIntoCache(const QString& text, const QString& detectedLangCode);

  bool _textIsDetectable(const QString& text) const;

  QString _getCountsStr(const QString& title, const QMap<QString, int>& data) const;
  QString _getUnvailableLangNamesStr() const;
  /** Timeout for HTTP requests in seconds */
  int _timeout;
};

}

#endif // HOOT_SERVICES_LANGUAGE_DETECTOR_CLIENT_H
