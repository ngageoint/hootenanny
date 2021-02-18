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
 * @copyright Copyright (C) 2015, 2017 Maxar (http://www.maxar.com/)
 */

#ifndef PP_CONFIGURATION_H
#define PP_CONFIGURATION_H

#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <hadoop/Pipes.hh>
#pragma GCC diagnostic warning "-Wunused-parameter"

// Qt
#include <QString>

// Standard
#include <map>

namespace pp
{

// Avoids a warning by SWIG.
#ifndef SWIG
class Configuration : public HadoopPipes::JobConf
#else
class Configuration
#endif
{
public:
  Configuration();

#ifndef SWIG
  Configuration(const std::map<std::string, std::string>& values) : _conf(values) {}

  Configuration(const Configuration& c) : HadoopPipes::JobConf(), _conf(c._conf) {}

  Configuration& operator=(const Configuration& c) { _conf = c._conf; return *this; }
#endif

  virtual ~Configuration() {}

  virtual bool hasKey(const std::string& key) const { return _conf.find(key) != _conf.end(); }

  virtual const std::string& get(const std::string& key) const { return _conf.find(key)->second; }

  virtual const std::string& get(const std::string& key, const std::string& defaultValue) const;

  virtual bool getBoolean(const std::string& key) const;

  /**
   * Decodes the bytes from Base64 then returns the bytes as raw binary.
   */
  virtual std::string getBytes(const std::string& key) const;

  virtual double getDouble(const std::string& key) const;

  virtual float getFloat(const std::string& key) const;

  virtual int getInt(const std::string& key) const;

  virtual int getInt(const std::string& key, int defaultValue) const;

  virtual long getLong(const std::string& key) const;

#ifndef SWIG
  virtual const std::map<std::string, std::string>& getMap() const { return _conf; }
#endif

  virtual void set(const std::string& key, const std::string& value) { _conf[key] = value; }

  /**
   * Base64 encodes the bytes before they are set as a string. This may contain values
   * that wouldn't otherwise be acceptable in XML.
   */
  virtual void setBytes(const std::string& key, const std::string& value);

  /**
   * Stores a double as text in XML. This is almost guaranteed to lose precision.
   */
  virtual void setDouble(const std::string& key, double value) { _conf[key] = QString("%1").arg(value, 0, 'g', 15).toStdString(); }

  virtual void setInt(const std::string& key, int value) { _conf[key] = QString("%1").arg(value).toStdString(); }

  virtual void setLong(const std::string& key, long value) { _conf[key] = QString("%1").arg(value).toStdString(); }

private:

  std::map<std::string, std::string> _conf;
};

}

#endif // PP_CONFIGURATION_H
