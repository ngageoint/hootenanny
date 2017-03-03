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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SCRIPTTRANSLATOR_H
#define SCRIPTTRANSLATOR_H

// Boost
#include <boost/shared_ptr.hpp>

// geos
#include <geos/geom/Geometry.h>

// hoot
#include <hoot/core/io/schema/StrictChecking.h>

// Qt
#include <QHash>
#include <QString>

namespace hoot
{
using namespace boost;
using namespace geos::geom;
class Feature;
class Schema;
class Tags;

/**
 * A script for translating feature attributes.
 * See ScriptTranslatorFactory for constructing a ScriptTranslator.
 */
class ScriptTranslator
{
public:

  static std::string className() { return "hoot::ScriptTranslator"; }

  ScriptTranslator();

  virtual ~ScriptTranslator();

  /**
   * This can be called to force the script to close out and uninitialize. This will be called
   * automatically by the destructor.
   */
  void close();

  /**
   * Returns true if the script appears to be valid. This should do a reasonable check (e.g. does
   * the file exist). But doesn't necessarily check for valid syntax, etc.
   *
   * This shouldn't throw an exception.
   */
  virtual bool isValidScript() = 0;

  /**
   * String containing the regexp to use for filtering the layer names.
   */
  virtual const QString getLayerNameFilter() { return "."; }

  /**
   * lower order values make the script engine get evaluated earlier.
   */
  virtual int order() const { return 0; }

  void setErrorTreatment(StrictChecking strict) { _strict = strict; }

  /**
   * Sets the script to be used. The script won't be initialized until the first call to translate.
   */
  void setScript(QString path) { close(); _scriptPath = path; }

  /**
   * Translates the specified tags. The result is placed back into the original tag set. The script
   * will be initialized if necessary.
   */
  void translateToOsm(Tags& tags, const char *layerName, const char* geomType);

protected:
  bool _initialized;
  QString _scriptPath;

  StrictChecking _strict;
  // store all key/value strings in this QHash, this promotes implicit sharing of string data. The
  // QHash goes away when the reading is done, but the memory sharing remains.
  QHash<QString, QString> _strings;

  /**
   * Called before translate is called. It is possible to get multiple calls. E.g:
   * - _init(); _translate(); _translate(); _finalize();
   * - _init(); _translate(); _finalize();
   * - _init(); _finalize();
   *
   * You are guaranteed that _init() and _finalize will be called in pairs with zero or more
   * _translate() calls in between.
   */
  virtual void _init() = 0;

  /**
   * Called after translations are done.
   */
  virtual void _finalize() = 0;

  const QString& _saveMemory(const QString& s);

  void strictError(const QString& s);

  /**
   * Wrapped by translateToOsm().
   */
  virtual void _translateToOsm(Tags& tags, const char *layerName, const char* geomType) = 0;
};

}

#endif // SCRIPTTRANSLATOR_H
