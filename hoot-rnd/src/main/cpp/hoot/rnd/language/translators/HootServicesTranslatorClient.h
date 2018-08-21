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

#ifndef HOOT_SERVICES_TRANSLATOR_CLIENT_H
#define HOOT_SERVICES_TRANSLATOR_CLIENT_H

// hoot
#include <hoot/rnd/language/translators/ToEnglishTranslator.h>

// Qt
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace hoot
{

/**
 * Translates text from a single source language to English using translation/detection
 * technologies integrated into the Hootenanny web services.
 */
class HootServicesTranslatorClient : public QObject, public ToEnglishTranslator
{
  Q_OBJECT

public:

  static std::string className() { return "hoot::HootServicesTranslatorClient"; }

  HootServicesTranslatorClient();

  virtual QStringList getSourceLanguages() const { return _sourceLangs; }
  virtual void setSourceLanguages(const QStringList langCodes);
  virtual void translate(const QString textToTranslate);
  virtual QString getTranslatedText() const { return _translatedText; }
  virtual bool detectionMade() const { return _detectionMade; }

  virtual void setConfiguration(const Settings& conf);

signals:

  void translationComplete();
  void translationError(QString textSent, QString message);

private slots:

  void _translateReplyReceived(QNetworkReply* reply);

private:

  QStringList _sourceLangs;
  QString _translator;
  QStringList _detectors;
  QString _translationUrl;
  QString _detectableUrl;
  QString _translatableUrl;
  bool _detectedLangOverrides;
  bool _performExhaustiveSearch;

  QString _translatedText;
  bool _detectionMade;

  boost::shared_ptr<QNetworkAccessManager> _client;

  void _checkLangsAvailable(const QString type);
};

}

#endif // HOOT_SERVICES_TRANSLATOR_CLIENT_H
