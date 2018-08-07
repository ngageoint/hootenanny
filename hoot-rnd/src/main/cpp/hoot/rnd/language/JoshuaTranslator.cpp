
#include "JoshuaTranslator.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QTextCodec>

namespace hoot
{

JoshuaTranslator::JoshuaTranslator(const QString host, const int port, QObject* parent) :
QObject(parent)
{
  _client.reset(new QTcpSocket(this));
  _client->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

  connect(_client.get(), SIGNAL(readyRead()), this, SLOT(_readyRead()));
  connect(_client.get(), SIGNAL(error(QAbstractSocket::SocketError)), this,
          SLOT(_error(QAbstractSocket::SocketError)));

  _client->connectToHost(host, port);
  if (!_client->waitForConnected(5000))
  {
    throw HootException("Unable to connect to the language translation service.");
  }
}

void JoshuaTranslator::setSourceLanguage(const QString langCode)
{
  if (langCode.toLower() != "de")
  {
    throw HootException("Unsupported source translation language.");
  }
}

void JoshuaTranslator::translate(const QString textToTranslate)
{
  if (_client->state() != QAbstractSocket::ConnectedState)
  {
    throw HootException("Language translation client has no active connection to service.");
  }

  LOG_VART(textToTranslate);
  _returnedData.clear();
  //joshua expects a newline at the end of what's passed in
  _client->write(QString(textToTranslate + "\n").toUtf8());
  const bool readyRead = _client->waitForReadyRead(5000);
  if (!readyRead)
  {
    throw HootException("Language translation server request timed out.");
  }
}

void JoshuaTranslator::_readyRead()
{
  if (_client->bytesAvailable())
  {
    _returnedData.append(_client->readAll());
  }
  LOG_VART(_returnedData.size());

  //server returns a single line of translated text with a newline at the end
  if (_returnedData.endsWith('\n'))
  {
    //106 = UTF-8
    _translatedText = QTextCodec::codecForMib(106)->toUnicode(_returnedData).trimmed();
    LOG_VART(_translatedText);
    emit translationComplete();
  }
}

void JoshuaTranslator::_error(QAbstractSocket::SocketError error)
{
  LOG_ERROR("error: " << error);
}

}
