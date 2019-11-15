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

// JNI
#include <jni.h>

namespace hoot
{

class JavaInteropTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(JavaInteropTest);
  //CPPUNIT_TEST(runJniTest1); // works
  //CPPUNIT_TEST(runJniTest2); // works
  //CPPUNIT_TEST(runJniTest3); // works
  //CPPUNIT_TEST(runJniTest4); // works
  //CPPUNIT_TEST(runJniTest5); // works
  //CPPUNIT_TEST(runJniTest6); // works
  CPPUNIT_TEST_SUITE_END();

public:

  JavaInteropTest()
  {
  }

  void runJniTest1()
  {
    // hello world calling Java built-in String class to lower case

    JavaVM* vm = 0;
    JNIEnv* env = 0;
    JavaVMInitArgs vm_args;
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 0;
    vm_args.ignoreUnrecognized = 1;
    // TODO: I think this has to be a Singleton.
    jint res = JNI_CreateJavaVM(&vm, (void**)&env, &vm_args);
    LOG_VARW(res);
    //_createVM(vm, env);

    jstring jstr = env->NewStringUTF("Hello World");
    jclass clazz = env->FindClass("java/lang/String");
    jmethodID to_lower = env->GetMethodID(clazz, "toLowerCase", "()Ljava/lang/String;");
    jobject result = env->CallObjectMethod(jstr, to_lower);
    const char* str = env->GetStringUTFChars((jstring)result, NULL);
    LOG_VARW(str);

    env->ReleaseStringUTFChars(jstr, str);
    vm->DestroyJavaVM();
  }

  void runJniTest2()
  {
    // call a static method in a jar that takes in no params and returns a string

    JavaVM* vm = 0;
    JNIEnv* env = 0;
    JavaVMInitArgs vm_args;
    JavaVMOption options[2];
    options[0].optionString = (char*)"-Djava.class.path=.";
    options[1].optionString = (char*)"-Djava.class.path=/home/vagrant/hoot/tmp/me-josm-4.4.4.jar";
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 2;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = 1;;
    jint res = JNI_CreateJavaVM(&vm, (void**)&env, &vm_args);
    LOG_VARW(res);

    jclass cls = env->FindClass("org/openstreetmap/josm/URLSetup");
    jmethodID methodId = env->GetStaticMethodID(cls, "get_planetsense_url", "()Ljava/lang/String;");
    jobject result = env->CallStaticObjectMethod(cls, methodId);
    const char* str = env->GetStringUTFChars((jstring)result, NULL);
    LOG_VARW(QString(str));
  }

  void runJniTest3()
  {
    // call a static method in a jar that takes in params and returns a string

    JavaVM* vm = 0;
    JNIEnv* env = 0;
    JavaVMInitArgs vm_args;
    JavaVMOption options[2];
    options[0].optionString = (char*)"-Djava.class.path=.";
    options[1].optionString = (char*)"-Djava.class.path=/home/vagrant/hoot/tmp/me-josm-4.4.4.jar";
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 2;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = 1;;
    jint res = JNI_CreateJavaVM(&vm, (void**)&env, &vm_args);
    LOG_VARW(res);

    jclass cls = env->FindClass("org/openstreetmap/josm/tools/Utils");
    jmethodID methodId =
      env->GetStaticMethodID(
        cls, "escapeReservedCharactersHTML", "(Ljava/lang/String;)Ljava/lang/String;");
    jstring input = env->NewStringUTF("<test>");
    jobject result = env->CallStaticObjectMethod(cls, methodId, input);
    const char* str = env->GetStringUTFChars((jstring)result, NULL);
    LOG_VARW(QString(str));
  }

  void runJniTest4()
  {
    // call an instance method on an object (no constructor params) in a jar that returns a String
    // object

    JavaVM* vm = 0;
    JNIEnv* env = 0;
    JavaVMInitArgs vm_args;
    JavaVMOption options[2];
    options[0].optionString = (char*)"-Djava.class.path=.";
    options[1].optionString = (char*)"-Djava.class.path=/home/vagrant/hoot/tmp/me-josm-4.4.4.jar";
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 2;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = 1;;
    jint res = JNI_CreateJavaVM(&vm, (void**)&env, &vm_args);
    LOG_VARW(res);

    jclass cls = env->FindClass("org/openstreetmap/josm/tools/WikiReader");
    jobject wikiReader = env->AllocObject(cls);
    jmethodID methodId = env->GetMethodID(cls, "getBaseUrlWiki", "()Ljava/lang/String;");
    jobject result = env->CallObjectMethod(wikiReader, methodId);
    const char* str = env->GetStringUTFChars((jstring)result, NULL);
    LOG_VARW(QString(str));

    vm->DestroyJavaVM();
  }

  void runJniTest5()
  {
    // call an instance method on an object (with constructor params) in a jar that returns a String
    // object

    JavaVM* vm = 0;
    JNIEnv* env = 0;
    JavaVMInitArgs vm_args;
    JavaVMOption options[2];
    options[0].optionString = (char*)"-Djava.class.path=.";
    options[1].optionString = (char*)"-Djava.class.path=/home/vagrant/hoot/tmp/me-josm-4.4.4.jar";
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 2;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = 1;;
    jint res = JNI_CreateJavaVM(&vm, (void**)&env, &vm_args);
    LOG_VARW(res);

    jclass cls = env->FindClass("org/openstreetmap/josm/tools/WikiReader");
    jstring baseUrl = env->NewStringUTF("http://blah");
    jmethodID constructorMethodId = env->GetMethodID(cls, "<init>", "(Ljava/lang/String;)V");
    jobject wikiReader = env->NewObject(cls, constructorMethodId, baseUrl);
    jmethodID methodId = env->GetMethodID(cls, "getBaseUrlWiki", "()Ljava/lang/String;");
    jobject result = env->CallObjectMethod(wikiReader, methodId);
    const char* str = env->GetStringUTFChars((jstring)result, NULL);
    LOG_VARW(QString(str));

    vm->DestroyJavaVM();
  }

  void runJniTest6()
  {
    // pass a custom object into an object's method and then retrieve it

    JavaVM* vm = 0;
    JNIEnv* env = 0;
    JavaVMInitArgs vm_args;
    JavaVMOption options[2];
    options[0].optionString = (char*)"-Djava.class.path=.";
    options[1].optionString = (char*)"-Djava.class.path=/home/vagrant/hoot/tmp/me-josm-4.4.4.jar";
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 2;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = 1;;
    jint res = JNI_CreateJavaVM(&vm, (void**)&env, &vm_args);
    LOG_VARW(res);

    // create two nodes
    jclass nodeCls = env->FindClass("org/openstreetmap/josm/data/osm/Node");
    jmethodID nodeConstructorMethodId = env->GetMethodID(nodeCls, "<init>", "(J)V");
    jobject node1 = env->NewObject(nodeCls, nodeConstructorMethodId, 1L);
    jobject node2 = env->NewObject(nodeCls, nodeConstructorMethodId, 2L);

    // create a node pair and add both nodes
    jclass nodePairCls = env->FindClass("org/openstreetmap/josm/data/osm/NodePair");
    jmethodID nodePairConstructorMethodId =
      env->GetMethodID(nodePairCls, "<init>",
      "(Lorg/openstreetmap/josm/data/osm/Node;Lorg/openstreetmap/josm/data/osm/Node;)V");
    jobject nodePair = env->NewObject(nodePairCls, nodePairConstructorMethodId, node1, node2);

    // get the first node
    jmethodID getNodeAMethodId =
      env->GetMethodID(nodePairCls, "getA", "()Lorg/openstreetmap/josm/data/osm/Node;");
    jobject node1Returned = env->CallObjectMethod(nodePair, getNodeAMethodId);

    // print the node's id and latitude

    jmethodID getLatMethodId = env->GetMethodID(nodeCls, "lat", "()D");
    jdouble node1Lat = env->CallDoubleMethod(node1Returned, getLatMethodId);
    double lat = (double)node1Lat;
    LOG_VARW(lat);

    jmethodID getIdMethodId = env->GetMethodID(nodeCls, "getId", "()J");
    jlong node1Id = env->CallLongMethod(node1Returned, getIdMethodId);
    long id = (long)node1Id;
    LOG_VARW(id);

    vm->DestroyJavaVM();
  }

private:

  // doesn't work yet
//  void _createVM(JavaVM* vm, JNIEnv* env, const QStringList& options = QStringList())
//  {
//    //JavaVM* vm;
//    //JNIEnv* env;
//    JavaVMInitArgs vm_args;
//    vm_args.version = JNI_VERSION_1_8;
//    vm_args.nOptions = options.size();
//    JavaVMOption vmOptions[options.size()];
//    for (int i = 0; i < options.size(); i++)
//    {
//      vmOptions[i].optionString = (char*)options.at(i).toStdString().c_str();
//    }
//    if (options.size() > 0)
//    {
//      vm_args.options = vmOptions;
//    }
//    vm_args.ignoreUnrecognized = 1;

//    // TODO: I think this has to be a Singleton.
//    jint res = JNI_CreateJavaVM(&vm, (void**)&env, &vm_args);
//    LOG_VARW(res);
//  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(JavaInteropTest, "quick");

}
