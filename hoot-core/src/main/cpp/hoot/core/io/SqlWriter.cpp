#include "SqlWriter.h"

// hoot
#include <hoot/core/util/HootException.h>

// Qt
#include <QSqlDatabase>

namespace hoot
{

SqlWriter::SqlWriter()
{

}

void SqlWriter::write(const QString sqlFile, const QUrl targetDbUrl)
{
  QFile inputFile(sqlFile);
  if (!inputFile.open(QIODevice::ReadOnly))
  {
    throw HootException("Unable to open SQL file.");
  }

  QSqlDatabase database;

}

}
