
#ifndef JOSHUA_TRANSLATOR_H
#define JOSHUA_TRANSLATOR_H

// hoot
#include <hoot/core/language/ToEnglishTranslator.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/rnd/language/SupportedTranslationLanguages.h>

// Qt
#include <QTcpSocket>
#include <QByteArray>
#include <QFile>

namespace hoot
{

/**
 * Translates text from a single source language to English using Joshua.
 *
 * @article{post2015joshua,
    Author = {Post, Matt and Cao, Yuan and Kumar, Gaurav},
    Journal = {The Prague Bulletin of Mathematical Linguistics},
    Title = {Joshua 6: A phrase-based and hierarchical statistical machine translation system},
    Year = {2015}
  }
 */
class JoshuaTranslator : public QObject, public ToEnglishTranslator, public Configurable
{
  Q_OBJECT

public:

  typedef struct ServiceMapping
  {
    public:

      QString langCode;
      QString langPackDir;
      QString host;
      int port;

      QString toString() const
      { return langCode + ";" + langPackDir + ";" + host + ";" + QString::number(port); }
  } ServiceMapping;

  JoshuaTranslator(QObject* parent = 0);
  ~JoshuaTranslator();

  virtual QStringList getSourceLanguages() const { return _sourceLangs; }
  virtual void setSourceLanguages(const QStringList langCodes);
  virtual void translate(const QString sourceLangCode, const QString textToTranslate);
  virtual void translate(const QString textToTranslate);
  virtual QString getTranslatedText() const { return _translatedText; }

  virtual void setConfiguration(const Settings& conf);

  virtual void setSupportedLanguages(
    boost::shared_ptr<SupportedTranslationLanguages> supportedLangs)
  { _supportedLangs = supportedLangs; }

signals:

  void translationComplete();

private slots:

  void _readyRead();
  void _error(QAbstractSocket::SocketError error);

private:

  QByteArray _returnedData;
  QString _translatedText;
  QMap<QString, ServiceMapping> _serviceMappings;
  QStringList _sourceLangs;
  QMap<QString, boost::shared_ptr<QTcpSocket>> _clients;
  boost::shared_ptr<QTcpSocket> _activeClient;
  QFile _serviceMappingsFile;
  boost::shared_ptr<SupportedTranslationLanguages> _supportedLangs;

  void _readServiceMappings(const QString serviceMappingsFile);
  bool _inDetectMode() const;
};

}

#endif // JOSHUA_TRANSLATOR_H
