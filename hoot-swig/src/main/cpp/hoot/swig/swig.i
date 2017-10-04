/* File: example.i */
%module hoot

%{
#include <vector>

%}

%include "exception.i"
%include "std_string.i"
%include "std_vector.i"

//INPUT_TYPEMAP(long, jlong, long, "J");

/*
 * The chunk of indicipherable swig code below allows InputSplit to take a byte[] as input and
 * return a byte[] as output. How does this work? Dunno. The magic of Google and cut/paste.
 */
%typemap(in)     (char * BYTE, int LENGTH) {
/* Functions from jni.h */
$1 = (char *) JCALL2(GetByteArrayElements, jenv, $input, 0);
$2 = (int)    JCALL1(GetArrayLength,       jenv, $input);
}
%typemap(jni)    (char * BYTE, int LENGTH) "jbyteArray"
%typemap(jtype)  (char * BYTE, int LENGTH) "byte[]"
%typemap(jstype) (char * BYTE, int LENGTH) "byte[]"
%typemap(javain) (char * BYTE, int LENGTH) "$javainput"

/* Specify signature of method to handle */
%apply (char * BYTE, int LENGTH)   { (char* byteArray, long len) };

%typemap(jni) signed char *writeFields "jbyteArray"
%typemap(jtype) signed char *writeFields "byte[]"
%typemap(jstype) signed char *writeFields "byte[]"
%typemap(javaout) signed char *writeFields {
  return $jnicall;
}

%typemap(in,numinputs=0,noblock=1) size_t *len {
  size_t length=0;
  $1 = &length;
}

%typemap(out) signed char *writeFields {
  $result = JCALL1(NewByteArray, jenv, length);
  JCALL4(SetByteArrayRegion, jenv, $result, 0, length, $1);
}

%{
void throwJavaError(JNIEnv* jenv, const char* message)
{
  LOG_FATAL(message);
  jclass clazz = jenv->FindClass("java/lang/AssertionError");
  jenv->ThrowNew(clazz, message);
}
%}

%include "TestUtils.i"
%include "util/JavaDataConvert.i"
%include "util/GeosDataConvert.i"
%include "Version.i"
%include "conflate/multiary/MultiaryUtilities.i"
