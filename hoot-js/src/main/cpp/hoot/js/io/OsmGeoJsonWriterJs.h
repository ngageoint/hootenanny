#ifndef OSMGEOJSONWRITER_H
#define OSMGEOJSONWRITER_H

// node.js
#include <hoot/js/SystemNodeJs.h>

namespace hoot
{

class OsmGeoJsonWriterJs : public node::ObjectWrap
{
public:
 static void Init(v8::Handle<v8::Object> target);

private:
  OsmGeoJsonWriterJs();
  ~OsmGeoJsonWriterJs();

  static void toString(const v8::FunctionCallbackInfo<v8::Value>& args);
};

}

#endif // OSMGEOJSONWRITER_H
