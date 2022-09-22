/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "TextFileWordWeightDictionary.h"

namespace hoot
{

TextFileWordWeightDictionary::TextFileWordWeightDictionary(const QString& filePath)
  : _count(0)
{
  _loadFile(filePath);
  _nonWord.setPattern("[^\\w]");
}

double TextFileWordWeightDictionary::getWeight(const QString& word) const
{
  QString normalized = word.toLower().normalized(QString::NormalizationForm_C);
  normalized.replace(_nonWord, "");
  auto it = _weights.find(normalized);

  if (it == _weights.end())
    return 0;
  else
    return (double)it->second / (double)_count;
}

void TextFileWordWeightDictionary::_loadFile(const QString& path)
{
  QFile fp(path);

  if (fp.open(QIODevice::ReadOnly) == false)
    throw HootException("Error opening file: " + path);

  QString first = fp.readLine();
  QString totalWordCount = "Total word count: ";
  if (first.startsWith(totalWordCount) == false)
    throw HootException("Expected the '" + totalWordCount + "' to come first.");

  first.replace(totalWordCount, "");
  bool ok;
  _count += first.toDouble(&ok);
  if (!ok)
    throw HootException("Bad double value in total word count: " + first);

  while (fp.atEnd() == false)
  {
    QString line = QString::fromUtf8(fp.readLine());

    QStringList kvp = line.split("\t");
    if (kvp.size() != 2)
      throw HootException("Expected the line to be <word><tab><count>. Got: " + line);

    int count = kvp[1].toInt(&ok);
    if (!ok)
      throw HootException("Expected count to be an integer.");

    _weights[kvp[0].toLower()] = count;
  }
}

}
