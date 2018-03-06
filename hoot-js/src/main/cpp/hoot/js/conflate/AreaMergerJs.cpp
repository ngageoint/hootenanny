#include "AreaMergerJs.h"

#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/polygon/AreaMerger.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/OsmMapJs.h>
#include <hoot/js/util/HootExceptionJs.h>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(AreaMergerJs)


AreaMergerJs::AreaMergerJs()
{
}

AreaMergerJs::~AreaMergerJs()
{
}

void AreaMergerJs::Init(Handle<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  exports->Set(String::NewFromUtf8(current, "areaMerge"),
               FunctionTemplate::New(current, jsAreaMerge)->GetFunction());
}

void AreaMergerJs::jsAreaMerge(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  try
  {
    if (args.Length() != 1)
    {
      args.GetReturnValue().Set(
        current->ThrowException(
            HootExceptionJs::create(
              IllegalArgumentException("Expected one argument for 'areaMerge'."))));
      return;
    }

    OsmMapJs* mapJs = node::ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject());
    OsmMapPtr map(mapJs->getMap());
    LOG_VARD(map->getElementCount());

    const ElementId refId = AreaMerger::merge(map);
    LOG_VARD(refId);
    LOG_VARD(map->getElementCount());

    //Set the ref area to have a conflated status, since its geometry, and tags were all that was
    //kept by the merger.
    ElementPtr refElement = map->getElement(refId);
    refElement->setStatus(Status(Status::Conflated));
    refElement->getTags()[MetadataTags::HootStatus()] = "3";

    args.GetReturnValue().Set(OsmMapJs::create(map));
  }
  catch (const HootException& e)
  {
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(e)));
  }
}

}
