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
 * @copyright Copyright (C) 2015, 2018, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef COLUMN_H
#define COLUMN_H

// hoot
#include <hoot/core/io/schema/StrictChecking.h>
#include <hoot/core/util/HootException.h>

// Qt
#include <QVariant>

namespace hoot
{

class FieldDefinition
{
public:

  class InvalidValueException : public HootException
  {
  public:

    InvalidValueException(QString fieldName, QString message) :
      HootException(message),
      _fieldName(fieldName)
    {
    }
    ~InvalidValueException() throw() = default;

  private:

    QString _fieldName;
  };

  FieldDefinition();
  virtual ~FieldDefinition() = default;

  virtual bool getAllowNull() const { return _allowNull; }
  virtual bool getDefaultIsNull() const { return _defaultIsNull; }
  virtual QVariant getDefaultValue() const = 0;

  QString getName() const { return _name; }
  virtual QVariant::Type getType() const = 0;
  int getWidth() const { return _width; }

  virtual bool hasDefaultValue() const = 0;

  /**
   * If v is valid then nothing happens, if v is invalid then an exception is thrown.
   * @throws InvalidValueException
   */
  virtual void validate(const QVariant& v, StrictChecking strict) const = 0;

  void setAllowNull(bool a) { _allowNull = a; }
  void setDefaultIsNull(bool defaultIsNull) { _defaultIsNull = defaultIsNull; }
  void setName(const QString& name) { _name = name; }
  void setWidth(int w) { _width = w; }

  virtual QString toString() const = 0;

protected:

  void _reportError(const QString& field, const QString& error, StrictChecking strict) const;

private:

  int _width;
  QString _name;
  bool _allowNull;
  bool _defaultIsNull;
};

}

#endif // COLUMN_H
