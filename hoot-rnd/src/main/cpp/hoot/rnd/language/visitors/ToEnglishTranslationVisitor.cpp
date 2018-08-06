
#include "ToEnglishTranslationVisitor.h"

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

HOOT_FACTORY_REGISTER(ConstElementVisitor, ToEnglishTranslationVisitor)

ToEnglishTranslationVisitor::ToEnglishTranslationVisitor()
{
}

ToEnglishTranslationVisitor::~ToEnglishTranslationVisitor()
{
}

void ToEnglishTranslationVisitor::setConfiguration(const Settings& /*conf*/)
{

}

void ToEnglishTranslationVisitor::visit(const boost::shared_ptr<Element>& /*e*/)
{

}

}
