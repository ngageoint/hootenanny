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

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/util/Log.h>

// Std
//#include </usr/lib/jvm/java-1.8.0-openjdk/include/linux/jni_md.h>
#include <jni.h>
#include <stdio.h>

namespace hoot
{

class JavaInteropTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(JavaInteropTest);
  CPPUNIT_TEST(runJniTest1);
  CPPUNIT_TEST(runJniTest2);
  CPPUNIT_TEST(runJniTest3);
  CPPUNIT_TEST(runJnaTest);
  CPPUNIT_TEST_SUITE_END();

public:

  JavaInteropTest()
  {
  }

  void runJniTest1()
  {
    JavaVM *vm;
    JNIEnv *env;
    JavaVMInitArgs vm_args;
    vm_args.version = JNI_VERSION_1_2;
    vm_args.nOptions = 0;
    vm_args.ignoreUnrecognized = 1;

    // Construct a VM
    /*jint res =*/ JNI_CreateJavaVM(&vm, (void **)&env, &vm_args);

    // Construct a String
    jstring jstr = env->NewStringUTF("Hello World");

    // First get the class that contains the method you need to call
    jclass clazz = env->FindClass("java/lang/String");

    // Get the method that you want to call
    jmethodID to_lower = env->GetMethodID(clazz, "toLowerCase", "()Ljava/lang/String;");
    // Call the method on the object
    jobject result = env->CallObjectMethod(jstr, to_lower);

    // Get a C-style string
    const char* str = env->GetStringUTFChars((jstring) result, NULL);

    printf("%s\n", str);

    // Clean up
    env->ReleaseStringUTFChars(jstr, str);

    // Shutdown the VM.
    vm->DestroyJavaVM();
  }

  void runJniTest2()
  {

  }

  void runJniTest3()
  {

  }

  void runJnaTest()
  {

  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(JavaInteropTest, "quick");

}
