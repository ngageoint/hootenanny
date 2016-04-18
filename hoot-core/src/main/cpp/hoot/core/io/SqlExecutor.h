#ifndef SQLEXECUTOR_H
#define SQLEXECUTOR_H

// Hoot


// Qt
#include <QString>
#include <QUrl>
#include <QFile>

namespace hoot
{

class SqlExecutor
{

public:

  SqlExecutor();

  void exec(const QString sql, const QUrl targetDbUrl);

private:



};

}

#endif // SQLEXECUTOR_H
