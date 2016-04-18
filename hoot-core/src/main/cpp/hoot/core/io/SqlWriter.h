#ifndef SQLWRITER_H
#define SQLWRITER_H

// Hoot


// Qt
#include <QString>
#include <QUrl>
#include <QFile>

namespace hoot
{

class SqlWriter
{

public:

  SqlWriter();

  void write(const QString sqlFile, const QUrl targetDbUrl);

private:



};

}

#endif // SQLWRITER_H
