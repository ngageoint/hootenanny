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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef HOOT_SERVICES_LANGUAGE_DETECTOR_CLIENT_H
#define HOOT_SERVICES_LANGUAGE_DETECTOR_CLIENT_H

// hoot
#include <hoot/core/language/LanguageDetector.h>

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
 *
 */
class HootServicesLanguageDetectorClient : public LanguageDetector
{

public:

  static std::string className() { return "hoot::HootServicesLanguageDetectorClient"; }

  HootServicesLanguageDetectorClient();
  virtual ~HootServicesLanguageDetectorClient();

  virtual void setConfiguration(const Settings& conf);

  virtual QString detect(const QString text);

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
  QString _url;

  boost::shared_ptr<QCache<QString, DetectionResult>> _cache;

  int _statusUpdateInterval;

  long _undetectableWords;
  QMap<QString, QSet<QString>> _langCodesWithNoLangNamesAvailable;
  QMap<QString, int> _confidenceCounts;

  long _cacheHits;
  long _cacheSize;
  long _cacheMaxSize;

  static bool _loggedCacheMaxReached;

  boost::shared_ptr<HootNetworkCookieJar> _cookies;

  QString _getRequestData(const QString text) const;
  QString _parseResponse(boost::shared_ptr<boost::property_tree::ptree> replyObj,
                         QString& detectorUsed) /*const*/;

  QString _getLangFromCache(const QString text);
  void _insertLangIntoCache(const QString text, const QString detectedLangCode);

  bool _textIsDetectable(const QString text) const;

  QString _getUnvailableLangNamesStr() const;
  QString _getConfidenceCountsStr() const;
};

}

#endif // HOOT_SERVICES_LANGUAGE_DETECTOR_CLIENT_H
