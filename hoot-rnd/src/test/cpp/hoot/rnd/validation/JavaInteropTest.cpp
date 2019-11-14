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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

//#pragma GCC diagnostic ignored "-Wwrite-strings"

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/util/Log.h>

// JNI
#include <jni.h>

// Std
#include <stdio.h>

namespace hoot
{

class JavaInteropTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(JavaInteropTest);
  //CPPUNIT_TEST(runJniTest1);
  CPPUNIT_TEST(runJniTest2);
  CPPUNIT_TEST_SUITE_END();

public:

  JavaInteropTest()
  {
  }

  void runJniTest1()
  {
    // hello world to lower case with Java String class

    JavaVM* vm;
    JNIEnv* env;
    JavaVMInitArgs vm_args;
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 0;
    vm_args.ignoreUnrecognized = 1;

    jint res = JNI_CreateJavaVM(&vm, (void**)&env, &vm_args);
    LOG_VARW(res);

    jstring jstr = env->NewStringUTF("Hello World");
    jclass clazz = env->FindClass("java/lang/String");
    jmethodID to_lower = env->GetMethodID(clazz, "toLowerCase", "()Ljava/lang/String;");
    jobject result = env->CallObjectMethod(jstr, to_lower);
    const char* str = env->GetStringUTFChars((jstring)result, NULL);
    printf("%s\n", str);

    env->ReleaseStringUTFChars(jstr, str);
    vm->DestroyJavaVM();
  }

  void runJniTest2()
  {
    // call a method from an object that returns a String object

    JavaVM* jvm;
    JNIEnv* env;
    JavaVMInitArgs vm_args;

    JavaVMOption options[2];
    options[0].optionString = (char*)"-Djava.class.path=.";
    options[1].optionString = (char*)"-Djava.class.path=/home/vagrant/hoot/tmp/me-josm-4.4.4.jar";
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 2;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = 1;;

    jint res = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
    LOG_VARW(res);

    jclass cls = env->FindClass("org/openstreetmap/josm/data/validation/tests/Buildings");
    jobject buildings = env->AllocObject(cls);
    jclass buildingClass = env->GetObjectClass(buildings);
    jmethodID methodId = env->GetMethodID(buildingClass/*cls*/, "getName", "()Ljava/lang/String;");
    LOG_VARW(methodId == 0);
    jobject result = env->CallObjectMethod(buildings, methodId);
    LOG_VARW(result == 0);
    const char* str = env->GetStringUTFChars((jstring)result, NULL);
    LOG_VARW(QString(str));

    jvm->DestroyJavaVM();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(JavaInteropTest, "quick");

}
