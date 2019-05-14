
%{
#include <hoot/rnd/conflate/multiary/MultiaryUtilities.h>

/****
 * Convert from Java MultiarySimpleMatch <=> C++ MultiarySimpleMatch
 ****/
// cache the method references.
static bool javaUtilMultiarySimpleMatchInitialized = false;
static jclass javaUtilMultiarySimpleMatch;
static jmethodID javaUtilMultiarySimpleMatchNew;
static jmethodID javaUtilMultiarySimpleMatchGetCPtr;

// intialize the method references the first time.
void initJniMultiarySimpleMatch(JNIEnv* jenv) {
  if (javaUtilMultiarySimpleMatchInitialized == false)
  {
    javaUtilMultiarySimpleMatch = static_cast<jclass>(
      jenv->NewGlobalRef(jenv->FindClass("hoot/MultiarySimpleMatch")));
    if (javaUtilMultiarySimpleMatch == 0)
    {
      throwJavaError(jenv, "Error retrieving hoot.MultiarySimpleMatch class.");
    }
    javaUtilMultiarySimpleMatchNew = jenv->GetMethodID(javaUtilMultiarySimpleMatch, "<init>", "(JZ)V");
    if (javaUtilMultiarySimpleMatchNew == 0)
    {
      throwJavaError(jenv, "Error retrieving hoot.MultiarySimpleMatch class.");
    }
    javaUtilMultiarySimpleMatchGetCPtr = jenv->GetStaticMethodID(javaUtilMultiarySimpleMatch,
      "getCPtr", "(Lhoot/MultiarySimpleMatch;)J");
    if (javaUtilMultiarySimpleMatchGetCPtr == 0)
    {
      throwJavaError(jenv, "Error retrieving hoot.MultiarySimpleMatch class.");
    }

    javaUtilMultiarySimpleMatchInitialized = true;
  }
}

void toCpp(JNIEnv* jenv, jobject obj, hoot::MultiarySimpleMatch& result)
{
  initJniMultiarySimpleMatch(jenv);

  jlong cptr = jenv->CallStaticLongMethod(javaUtilMultiarySimpleMatch,
    javaUtilMultiarySimpleMatchGetCPtr, obj);

  result = *(hoot::MultiarySimpleMatch*)cptr;
}

jobject toJava(JNIEnv* jenv, const hoot::MultiarySimpleMatch& obj)
{
  initJniMultiarySimpleMatch(jenv);

  hoot::MultiarySimpleMatch* copy = new hoot::MultiarySimpleMatch(obj);

  jobject result = jenv->NewObject(javaUtilMultiarySimpleMatch, javaUtilMultiarySimpleMatchNew,
    (jlong)copy, true);
  return result;
}

%}


/****
 * ArrayList<MultiarySimpleMatch> <=> QList<MultiarySimpleMatch>
 ****/
%typemap(jni) QList<hoot::MultiarySimpleMatch> "jobject"
%typemap(jtype) QList<hoot::MultiarySimpleMatch> "java.util.ArrayList<MultiarySimpleMatch>"
%typemap(jstype) QList<hoot::MultiarySimpleMatch> "java.util.ArrayList<MultiarySimpleMatch>"
%typemap(out) QList<hoot::MultiarySimpleMatch> { $result = toJava(jenv, $1); }
%typemap(directorout) QList<hoot::MultiarySimpleMatch> { $result = toJava(jenv, $1); }
%typemap(javaout) QList<hoot::MultiarySimpleMatch> { return $jnicall; }
%feature("novaluewrapper") QList<hoot::MultiarySimpleMatch>;
class QList<hoot::MultiarySimpleMatch>;

%{

/****
 * Convert from Java MultiaryElement <=> C++ MultiaryElement
 ****/
// cache the method references.
static bool javaUtilMultiaryElementInitialized = false;
static jclass javaUtilMultiaryElement;
static jmethodID javaUtilMultiaryElementNew;
static jmethodID javaUtilMultiaryElementGetCPtr;

// intialize the method references the first time.
void initJniMultiaryElement(JNIEnv* jenv) {
  if (javaUtilMultiaryElementInitialized == false)
  {
    javaUtilMultiaryElement = static_cast<jclass>(
      jenv->NewGlobalRef(jenv->FindClass("hoot/MultiaryElement")));
    if (javaUtilMultiaryElement == 0)
    {
      throwJavaError(jenv, "Error retrieving hoot.MultiaryElement class.");
    }
    javaUtilMultiaryElementNew = jenv->GetMethodID(javaUtilMultiaryElement, "<init>", "(JZ)V");
    if (javaUtilMultiaryElementNew == 0)
    {
      throwJavaError(jenv, "Error retrieving hoot.MultiaryElement class.");
    }
    javaUtilMultiaryElementGetCPtr = jenv->GetStaticMethodID(javaUtilMultiaryElement,
      "getCPtr", "(Lhoot/MultiaryElement;)J");
    if (javaUtilMultiaryElementGetCPtr == 0)
    {
      throwJavaError(jenv, "Error retrieving hoot.MultiaryElement class.");
    }

    javaUtilMultiaryElementInitialized = true;
  }
}

void toCpp(JNIEnv* jenv, jobject obj, hoot::MultiaryElement& result)
{
  initJniMultiaryElement(jenv);

  jlong cptr = jenv->CallStaticLongMethod(javaUtilMultiaryElement,
    javaUtilMultiaryElementGetCPtr, obj);

  result = *(hoot::MultiaryElement*)cptr;
}

jobject toJava(JNIEnv* jenv, const hoot::MultiaryElement& obj)
{
  initJniMultiaryElement(jenv);

  hoot::MultiaryElement* copy = new hoot::MultiaryElement(obj);

  jobject result = jenv->NewObject(javaUtilMultiaryElement, javaUtilMultiaryElementNew,
    (jlong)copy, true);
  return result;
}

%}

/****
 * ArrayList<MultiaryElement> <=> QList<MultiaryElement>
 ****/
%typemap(jni) QList<hoot::MultiaryElement> "jobject"
%typemap(jtype) QList<hoot::MultiaryElement> "java.util.ArrayList<MultiaryElement>"
%typemap(jstype) QList<hoot::MultiaryElement> "java.util.ArrayList<MultiaryElement>"
%typemap(out) QList<hoot::MultiaryElement> { $result = toJava(jenv, $1); }
%typemap(directorout) QList<hoot::MultiaryElement> { $result = toJava(jenv, $1); }
%typemap(javaout) QList<hoot::MultiaryElement> { return $jnicall; }
%feature("novaluewrapper") QList<hoot::MultiaryElement>;
class QList<hoot::MultiaryElement>;

%include <hoot/rnd/conflate/multiary/MultiaryUtilities.h>

