
#include "JoshuaTranslator.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QTextCodec>
#include <QLocale>

namespace hoot
{

JoshuaTranslator::JoshuaTranslator() :
_sourceLang("DE")
{
}

JoshuaTranslator::~JoshuaTranslator()
{
}

QString JoshuaTranslator::translate(const QString /*toTranslate*/)
{
  return "";
}

void JoshuaTranslator::setSourceLanguage(const QString /*langCode*/)
{
}

}
