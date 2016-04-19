#include "SqlExecutor.h"

// hoot
#include <hoot/core/util/HootException.h>

// Qt
#include <QSqlDatabase>

namespace hoot
{

SqlExecutor::SqlExecutor()
{

}

void SqlExecutor::exec(const QString sql, const QUrl targetDbUrl)
{
  QSqlDatabase database;

}

}
