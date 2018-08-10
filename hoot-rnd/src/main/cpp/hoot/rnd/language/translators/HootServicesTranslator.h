
#ifndef HOOT_SERVICES_TRANSLATOR_H
#define HOOT_SERVICES_TRANSLATOR_H

// hoot
#include <hoot/rnd/language/translators/ToEnglishTranslator.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QTcpSocket>
#include <QByteArray>
#include <QFile>

namespace hoot
{

/**
 * Translates text from a single source language to English using translation/detection
 * technologies integrated into the Hootenanny web services.
 */
class HootServicesTranslator : public QObject, public ToEnglishTranslator, public Configurable
{
  Q_OBJECT

public:

  HootServicesTranslator(QObject* parent = 0);
  ~HootServicesTranslator();

  virtual QStringList getSourceLanguages() const { return _sourceLangs; }
  virtual void setSourceLanguages(const QStringList langCodes);
  virtual void translate(const QString sourceLangCode, const QString textToTranslate);
  virtual void translate(const QString textToTranslate);
  virtual QString getTranslatedText() const { return _translatedText; }
  virtual boost::shared_ptr<SupportedTranslationLanguages> getSupportedLanguages() const
  { return _supportedLangs; }

  virtual void setConfiguration(const Settings& conf);

signals:

  void translationComplete();

private slots:

  void _readyRead();
  void _error(QAbstractSocket::SocketError error);

private:

  QByteArray _returnedData;
  QString _translatedText;
  QStringList _sourceLangs;
  //QMap<QString, boost::shared_ptr<QTcpSocket>> _clients;
  //boost::shared_ptr<QTcpSocket> _activeClient;
  boost::shared_ptr<SupportedTranslationLanguages> _supportedLangs;

  bool _inDetectMode() const;
};

}

#endif // HOOT_SERVICES_TRANSLATOR_H
