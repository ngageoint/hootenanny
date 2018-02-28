
/*
 * This file provides conversion from Java objects to their C++ counterpart. Where possible,
 * templates are used to enable conversion of complex data structures w/o any special handling
 * code.
 */

%{

#include <QString>

/****
 * Convert from Java String <=> C++ QString
 ****/
void toCpp(JNIEnv* jenv, jstring str, QString& result) {
  jboolean isCopy;
  const char* data = jenv->GetStringUTFChars(str, &isCopy);
  result = QString::fromUtf8(data);
  jenv->ReleaseStringUTFChars(str, data);
}

jstring toJava(JNIEnv* jenv, const QString& from)
{
  return jenv->NewStringUTF(from.toUtf8().data());
}

%}

%typemap(jni) QString "jstring"
%typemap(jtype) QString "String"
%typemap(jstype) QString "String"
%typemap(in) QString { toCpp(jenv, $input, $1); }
%typemap(javain) QString "$javainput"
%typemap(out) QString { $result = toJava(jenv, $1); }
%typemap(directorout) QString { $result = toJava(jenv, $1); }
%typemap(javaout) QString { return $jnicall; }
%feature("novaluewrapper") QString;
class QString;

%{

/****
 * Convert from byte[] to QByteArray
 ****/
void toCpp(JNIEnv* jenv, jobject obj, QByteArray& result) {
  jbyteArray javaByteArray = static_cast<jbyteArray>(obj);

  jboolean copy;
  jbyte* javaRawBytes = jenv->GetByteArrayElements(javaByteArray, &copy);

  jlong len = jenv->GetArrayLength(javaByteArray);
  result.insert(0, (const char*)javaRawBytes, len);

  /* JNI_ABORT - free the buffer without copying back the possible changes */
  jenv->ReleaseByteArrayElements(javaByteArray, javaRawBytes, JNI_ABORT);
}

jbyteArray toJava(JNIEnv* jenv, const QByteArray& from)
{
  jbyteArray result = jenv->NewByteArray(from.size());

  jsize length = from.size();
  jenv->SetByteArrayRegion(result, 0, length, (const jbyte*)from.data());

  return result;
}

/****
 * Convert form ArrayList<> to QList<>
 ****/
static bool javaUtilArrayListInitialized = false;
static jclass java_util_ArrayList;
static jmethodID java_util_ArrayList_;
jmethodID java_util_ArrayList_size;
jmethodID java_util_ArrayList_get;
jmethodID java_util_ArrayList_add;

void initJniArrayList(JNIEnv* jenv) {
  if (javaUtilArrayListInitialized == false)
  {
    java_util_ArrayList      = static_cast<jclass>(jenv->NewGlobalRef(jenv->FindClass("java/util/ArrayList")));
    java_util_ArrayList_     = jenv->GetMethodID(java_util_ArrayList, "<init>", "(I)V");
    java_util_ArrayList_size = jenv->GetMethodID(java_util_ArrayList, "size", "()I");
    java_util_ArrayList_get  = jenv->GetMethodID(java_util_ArrayList, "get", "(I)Ljava/lang/Object;");
    java_util_ArrayList_add  = jenv->GetMethodID(java_util_ArrayList, "add", "(Ljava/lang/Object;)Z");
    javaUtilArrayListInitialized = true;
  }
}

template<typename T>
void toCpp(JNIEnv* jenv, jobject arrayList, QList<T>& result)
{
  initJniArrayList(jenv);

  jint len = jenv->CallIntMethod(arrayList, java_util_ArrayList_size);

  result.clear();
  result.reserve(len);

  for (jint i = 0; i < len; i++) {
    T v;
    jobject element = jenv->CallObjectMethod(arrayList, java_util_ArrayList_get, i);
    toCpp(jenv, element, v);
    jenv->DeleteLocalRef(element);

    result.append(v);
  }
}

template<typename T>
jobject toJava(JNIEnv* jenv, QList<T>& from)
{
  initJniArrayList(jenv);

  jobject result = jenv->NewObject(java_util_ArrayList, java_util_ArrayList_, 0);

  foreach (const T& v, from)
  {
    jobject element = toJava(jenv, v);
    jenv->CallObjectMethod(result, java_util_ArrayList_add, element);
    jenv->DeleteLocalRef(element);
  }

  return result;
}

%}

/****
 * Java byte[] <=> QByteArray
 ****/
%typemap(jni) QByteArray "jbyteArray"
%typemap(jtype) QByteArray "byte[]"
%typemap(jstype) QByteArray "byte[]"
%typemap(in) QByteArray { toCpp(jenv, $input, $1); }
%typemap(javain) QByteArray "$javainput"
%typemap(out) QByteArray { $result = toJava(jenv, $1); }
%typemap(directorout) QByteArray { $result = toJava(jenv, $1); }
%typemap(javaout) QByteArray { return $jnicall; }
%feature("novaluewrapper") QByteArray;
class QByteArray;

/****
 * ArrayList<byte[]> to QList<QByteArray>
 ****/
%typemap(jni) QList<QByteArray> "jobject"
%typemap(jtype) QList<QByteArray> "java.util.ArrayList<byte[]>"
%typemap(jstype) QList<QByteArray> "java.util.ArrayList<byte[]>"
%typemap(in) QList<QByteArray> { toCpp(jenv, $input, $1); }
%typemap(javain) QList<QByteArray> "$javainput"
%typemap(out) QList<QByteArray> { toJava(jenv, $1); }
%feature("novaluewrapper") QList<QByteArray>;
class QList<QByteArray>;

