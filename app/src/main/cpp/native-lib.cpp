#include <jni.h>
#include <string>

static int count;

extern "C"
JNIEXPORT jstring

JNICALL
Java_com_szj_testjni_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_szj_testjni_HelloJavaActivity_callback(JNIEnv *env, jobject instance) {

    jclass activityClass = env->GetObjectClass(instance);
    jmethodID method = env->GetMethodID(activityClass,"setText","(I)V");
    env->CallVoidMethod(instance,method,count += 1);

}