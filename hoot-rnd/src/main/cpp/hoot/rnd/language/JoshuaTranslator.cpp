
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
  _client.reset(new QTcpSocket(/*this*/parent));
  _client->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

  //connect(_client.get(), SIGNAL(connected()), this, SLOT(connected()));
  //connect(_client.get(), SIGNAL(disconnected()), this, SLOT(disconnected()));
  //connect(_client.get(), SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
  connect(_client.get(), SIGNAL(readyRead()), this, SLOT(readyRead()));
  connect(_client.get(), SIGNAL(error(QAbstractSocket::SocketError)), this,
          SLOT(error(QAbstractSocket::SocketError)));

  _client->connectToHost(host, port);
  if (!_client->waitForConnected(5000))
  {
    throw HootException("Unable to connect to the language translation service.");
  }
}

JoshuaTranslator::~JoshuaTranslator()
{
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

  LOG_VARD(textToTranslate);
  _returnedData.clear();
  //joshua expects a newline at the end of what's passed in
  _client->write(QString(textToTranslate + "\n").toUtf8());
//  const bool bytesWereWritten = _client->waitForBytesWritten(5000);
//  LOG_VARD(bytesWereWritten);
  const bool readyRead = _client->waitForReadyRead(5000);
  LOG_VARD(readyRead);
}

//void JoshuaTranslator::connected()
//{
//  LOG_DEBUG("connected");
//}

//void JoshuaTranslator::disconnected()
//{
//  LOG_DEBUG("disconnected");
//}

//void JoshuaTranslator::bytesWritten(qint64 bytes)
//{
//  LOG_DEBUG("bytes written: " << bytes);

//  const bool readyRead = _client->waitForReadyRead(5000);
//  LOG_VARD(readyRead);
//}

void JoshuaTranslator::readyRead()
{
  if (_client->bytesAvailable())
  {
    _returnedData.append(_client->readAll());
  }
  LOG_VARD(_returnedData.size());

  if (_returnedData.endsWith('\n'))
  {
    //106 = UTF-8
    _translatedText = QTextCodec::codecForMib(106)->toUnicode(_returnedData).trimmed();
    LOG_VARD(_translatedText);
    emit translationComplete();
  }
}

void JoshuaTranslator::error(QAbstractSocket::SocketError error)
{
  LOG_ERROR("error: " << error);
}

}
