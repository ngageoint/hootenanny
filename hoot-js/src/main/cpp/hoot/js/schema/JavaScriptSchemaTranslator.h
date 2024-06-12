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
 * @copyright Copyright (C) 2015-2024 Maxar (http://www.maxar.com/)
 */

#ifndef JAVASCRIPT_SCHEMA_TRANSLATOR_H
#define JAVASCRIPT_SCHEMA_TRANSLATOR_H

// hoot
#include <hoot/core/schema/ScriptSchemaTranslator.h>
#include <hoot/core/schema/ScriptToOgrSchemaTranslator.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Settings.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/PluginContext.h>
#include <hoot/js/SystemNodeJs.h>

// Qt
#include <QVariant>

namespace hoot
{

class FieldDefinition;
class Layer;
class StringFieldDefinition;
class DoubleFieldDefinition;
class IntegerFieldDefinition;
class LongIntegerFieldDefinition;
class Schema;
class ElementType;

/**
 * Script functions available:
 * http://qt-project.org/doc/qt-4.7/ecmascript.html
 */
class JavaScriptSchemaTranslator : public ScriptSchemaTranslator,
  public ScriptToOgrSchemaTranslator, public Configurable
{
public:

  static QString className() { return "JavaScriptSchemaTranslator"; }

  static int logWarnCount;

  JavaScriptSchemaTranslator();
  ~JavaScriptSchemaTranslator() override;

  bool isValidScript() override;

  /**
   * Should not be called publicly.
   */
  void _checkError();

  std::shared_ptr<const Schema> getOgrOutputSchema() override;
  // Filter for file names
  QString getLayerNameFilter() override;
  std::vector<TranslatedFeature> translateToOgr(Tags& tags, ElementType elementType,
                                                geos::geom::GeometryTypeId geometryType) override;
  std::vector<Tags> translateToOgrTags(Tags& tags, ElementType elementType,
                                       geos::geom::GeometryTypeId geometryType) override;
  void setConfiguration(const Settings& conf) override;

  QString getScriptPath() const { return _scriptPath; }
  const Settings& getConfiguration() const { return _conf; }

  /**
   * Uses the specified script text instead of loading the script from a file.
   */
  void setScriptText(const QString& text) { close(); _scriptText = text; _scriptPath = QString(); }

protected:

  void _init() override;
  void _finalize() override;

  void _translateToOsm(Tags& tags, const char *layerName, const char* geomType) override;

private:

  std::shared_ptr<PluginContext> _gContext;
  QString _toOsmFunctionName;

  bool _error;
  std::shared_ptr<Schema> _schema;
  QString _scriptText;
  std::vector<double> _timing;
  QHash<QString, int> _logs;
  v8::Local<v8::Value> _empty[0]; // For function calls

  Settings _conf;

  std::vector<TranslatedFeature> _createAllFeatures(const QVariantList& vm) const;
  std::shared_ptr<Feature> _createFeature(const QVariantMap& vm, QString& tableName) const;

  void _parseEnumerations(std::shared_ptr<StringFieldDefinition> fd, const QVariant& enumerations) const;
  void _parseEnumerations(std::shared_ptr<DoubleFieldDefinition> fd, const QVariant& enumerations) const;
  void _parseEnumerations(std::shared_ptr<IntegerFieldDefinition> fd, const QVariant& enumerations) const;
  void _parseEnumerations(std::shared_ptr<LongIntegerFieldDefinition> fd, const QVariant& enumerations) const;

  std::shared_ptr<FieldDefinition> _parseFieldDefinition(const QVariant& fieldV) const;

  std::shared_ptr<Layer> _parseLayer(const QVariant& layer) const;

  double _toDouble(const QVariant& v) const;
  qint32 _toInt32(const QVariant& v) const;
  qint64 _toInt64(const QVariant& v) const;

  QVariantList _translateToOgrVariants(const Tags& tags, ElementType elementType, geos::geom::GeometryTypeId geometryType);
};

}

#endif // JAVASCRIPT_SCHEMA_TRANSLATOR_H
