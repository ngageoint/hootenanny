#ifndef AREAMERGERJS_H
#define AREAMERGERJS_H

#include <hoot/js/HootJsStable.h>
#include <hoot/js/SystemNodeJs.h>

namespace hoot
{

/**
 * Hoot JS wrapper around AreaMerger
 */
class AreaMergerJs : public node::ObjectWrap
{

public:

 static void Init(v8::Handle<v8::Object> target);

private:

  AreaMergerJs();
  ~AreaMergerJs();

  /**
   * Merges two areas
   *
   * @param args an OSM map containing two areas, which can be either a ways or relations
   * @return a map with the areas merged
   */
  static void jsAreaMerge(const v8::FunctionCallbackInfo<v8::Value>& args);
};

}

#endif // AREAMERGERJS_H
