//
// Created by msj on 2017/8/1.
//

#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <jni.h>
#include <string.h>

namespace kikasignature {

#ifdef __ANDROID__

  // generate from APP
  const char *APP_SIGNATURE_DEBUG = "3082035d30820245a003020102020424fc2929300d06092a864886f70d01010b0500305f310b300906035504061302434e310b300906035504081302424a3110300e060355040713074265696a696e67310f300d060355040a130658696e6d6569310f300d060355040b130658696e6d6569310f300d0603550403130658696e6d6569301e170d3135313232333033323530355a170d3433303531303033323530355a305f310b300906035504061302434e310b300906035504081302424a3110300e060355040713074265696a696e67310f300d060355040a130658696e6d6569310f300d060355040b130658696e6d6569310f300d0603550403130658696e6d656930820122300d06092a864886f70d01010105000382010f003082010a028201010085b01e865f0a9cb7a411aa41ecd634cd57dec35f73a36bf14d6aa13a6287898f7a1ebf94fb8fa8f98fc31856f5eef8a645d55f0117fac41613a1ff164307c66d583ee3d35ab86d2e44409b82b83e110d828ee421f580cdd59c5ba93f4ebbb25b7a1328650f14ccaa283411c11e8109628e03b6a8ec44feea2c01db6fb5310793ecc1e2d6712eed070b11a792827270e4b219c38ec1b491c991edd0a2bd2d903a911b06430e1faab24a74fe2076a831e36e630aeff244f3655ea9d5699dcefee840381893b86e9639b11109c54b81786fa4d0d9796e26b0cc81f612fc2919e0cd19419a8bf922b57034019b556ded54dd5cc12a5bafd4b34c16e014d51717d0870203010001a321301f301d0603551d0e04160414a798c50109c95f7a53516877edc3214fa9b86976300d06092a864886f70d01010b0500038201010033881b3bcb7ad0f40ae9d36166c180d0bc5a0fbe71aabd448dd8e81ab977c6adf9528a589389c6d92cd60ca3948d8c4afb5f9153a6ef86e5dbe4e56977a4cff4f5aa3ff78cec3fd70d0dc990f1dba078613b7b7531d2c031da00d66eeca391920803ba7c4d049b0fbb1b2d34add60ba1ea99e05c931eb72df102b5104bf2fb93b1eafccde1ecd12a626a40f3708908cd0b18f030f64f789b5186345ed2bab4113c95ce84a818a2f7a2767a4052a8942cf082f129f312def1df9e272e25822f25dbc8a14589ac8aa92d210e67f986209e0161a0df5938f41287d1a4a6fbb938e4f7ae8de33d86cedcc293a04c4b6001aeb359edb0d0ace2efc820825edd6ac0f4";
  const char *APP_SIGNATURE_RELEASE = "308201943081fea00302010202045396a6d4300d06092a864886f70d0101050500300f310d300b0603550403130470696d70301e170d3134303631303036333335365a170d3339303630343036333335365a300f310d300b0603550403130470696d7030819f300d06092a864886f70d010101050003818d0030818902818100a4dc96797980a39ff60866daa99bdf33d54b77c381abdd582ff6c0e39fdb6d43c43804737152bbacbdbb90d2fb4b017bc7a5f2059606ddc5f9b1d6718da1c8bec594292267235364f576e8a2eaf0382e688b57743278871f00f1fed122ad21397fb28a0fd7613d314aabf8187f5e4432aea6868480c03fc907f22042d0eebca30203010001300d06092a864886f70d010105050003818100993882b0234bd3a370e01023108207138f11fe880ffe988b88681845111a970332383e6838fcebbc2dc7d9a413b14fb41a0a4b07a6a44a4ee41a251419484929e92b3e8bf1f7e44e9e6f17337e0ba61609f50fc85b20f57ddd3d196c97312c8c8a2ee84decbbbb0f7087b9af317d11da665aed59c303c74f257219e8e4f666fe";


  void ByteToHexStr(const char *source, char *dest, int sourceLen) {
      short i;
      char highByte, lowByte;

      for (i = 0; i < sourceLen; i++) {
          highByte = source[i] >> 4;
          lowByte = source[i] & 0x0f;
          highByte += 0x30;

          if (highByte > 0x39) {
              dest[i * 2] = highByte + 0x07;
          } else {
              dest[i * 2] = highByte;
          }

          lowByte += 0x30;
          if (lowByte > 0x39) {
              dest[i * 2 + 1] = lowByte + 0x07;
          } else {
              dest[i * 2 + 1] = lowByte;
          }
      }
  }

  // byte数组转MD5字符串
  jstring ToMd5(JNIEnv *env, jbyteArray source) {
      // MessageDigest类
      jclass classMessageDigest = env->FindClass("java/security/MessageDigest");
      // MessageDigest.getInstance()静态方法
      jmethodID midGetInstance = env->GetStaticMethodID(classMessageDigest, "getInstance",
                                                        "(Ljava/lang/String;)Ljava/security/MessageDigest;");
      // MessageDigest object
      jobject objMessageDigest = env->CallStaticObjectMethod(classMessageDigest, midGetInstance,
                                                             env->NewStringUTF("md5"));

      // update方法，这个函数的返回值是void，写V
      jmethodID midUpdate = env->GetMethodID(classMessageDigest, "update", "([B)V");
      env->CallVoidMethod(objMessageDigest, midUpdate, source);

      // digest方法
      jmethodID midDigest = env->GetMethodID(classMessageDigest, "digest", "()[B");
      jbyteArray objArraySign = (jbyteArray) env->CallObjectMethod(objMessageDigest, midDigest);

      jsize intArrayLength = env->GetArrayLength(objArraySign);
      jbyte *byte_array_elements = env->GetByteArrayElements(objArraySign, NULL);
      size_t length = (size_t) intArrayLength * 2 + 1;
      char *char_result = (char *) malloc(length);
      memset(char_result, 0, length);

      // 将byte数组转换成16进制字符串，发现这里不用强转，jbyte和unsigned char应该字节数是一样的
      ByteToHexStr((const char *) byte_array_elements, char_result, intArrayLength);
      // 在末尾补\0
      *(char_result + intArrayLength * 2) = '\0';

      jstring stringResult = env->NewStringUTF(char_result);
      // release
      env->ReleaseByteArrayElements(objArraySign, byte_array_elements, JNI_ABORT);
      // 释放指针使用free
      free(char_result);
      env->DeleteLocalRef(classMessageDigest);
      env->DeleteLocalRef(objMessageDigest);

      return stringResult;
  }

  //获取应用签名
  jstring loadSignature(JNIEnv *env, jobject context) {
      // 获得Context类
      jclass cls = env->GetObjectClass(context);
      // 得到getPackageManager方法的ID
      jmethodID mid = env->GetMethodID(cls, "getPackageManager",
                                       "()Landroid/content/pm/PackageManager;");

      // 获得应用包的管理器
      jobject pm = env->CallObjectMethod(context, mid);

      // 得到getPackageName方法的ID
      mid = env->GetMethodID(cls, "getPackageName", "()Ljava/lang/String;");
      // 获得当前应用包名
      jstring packageName = (jstring) env->CallObjectMethod(context, mid);

      // 获得PackageManager类
      cls = env->GetObjectClass(pm);
      // 得到getPackageInfo方法的ID
      mid = env->GetMethodID(cls, "getPackageInfo",
                             "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
      // 获得应用包的信息
      jobject packageInfo = env->CallObjectMethod(pm, mid, packageName, 0x40); //GET_SIGNATURES = 64;
      // 获得PackageInfo 类
      cls = env->GetObjectClass(packageInfo);
      // 获得签名数组属性的ID
      jfieldID fid = env->GetFieldID(cls, "signatures", "[Landroid/content/pm/Signature;");
      // 得到签名数组
      jobjectArray signatures = (jobjectArray) env->GetObjectField(packageInfo, fid);
      // 得到签名
      jobject signature = env->GetObjectArrayElement(signatures, 0);

      // 获得Signature类
      cls = env->GetObjectClass(signature);

      jmethodID string_id = env->GetMethodID(cls, "toCharsString", "()Ljava/lang/String;");
      jstring str = static_cast<jstring>(env->CallObjectMethod(signature, string_id));
      return str;
  }

  //检测签名是否匹配
  jboolean checkSignature(JNIEnv *env, jobject context, const char *appSig) {

      jstring appSignature = loadSignature(env, context); // 当前 App 的签名
      jstring releaseSignature = env->NewStringUTF(appSig); // 发布时候的签名
      const char *charAppSignature = env->GetStringUTFChars(appSignature, NULL);
      const char *charReleaseSignature = env->GetStringUTFChars(releaseSignature, NULL);

      jboolean result = JNI_FALSE;
      // 比较是否相等
      if (charAppSignature != NULL && charReleaseSignature != NULL) {
          if (strcmp(charAppSignature, charReleaseSignature) == 0) {
              result = JNI_TRUE;
          }
      }

      env->ReleaseStringUTFChars(appSignature, charAppSignature);
      env->ReleaseStringUTFChars(releaseSignature, charReleaseSignature);

      return result;
  }

  static jobject getApplication(JNIEnv *env) {
      jobject application = NULL;
      jclass activity_thread_clz = env->FindClass("android/app/ActivityThread");
      if (activity_thread_clz != NULL) {
          jmethodID currentApplication = env->GetStaticMethodID(
                  activity_thread_clz, "currentApplication", "()Landroid/app/Application;");
          if (currentApplication != NULL) {
              application = env->CallStaticObjectMethod(activity_thread_clz, currentApplication);
          }
          env->DeleteLocalRef(activity_thread_clz);
      }
      return application;
  }

  static jboolean checkSignatureImpl(JNIEnv *env, const char *appSig) {
      jobject appContext = getApplication(env);

      if (appContext != NULL) {
          jboolean signatureValid = checkSignature(env, appContext, appSig);
          return signatureValid;
      }
      return JNI_FALSE;
  }

  static bool checkSignature(JNIEnv *env) {
      return (checkSignatureImpl(env, APP_SIGNATURE_DEBUG) == JNI_TRUE) ||
             (checkSignatureImpl(env, APP_SIGNATURE_RELEASE) == JNI_TRUE);
  }

#else

  static bool checkSignature(JNIEnv *env) {
    return true;
  }

#endif // __ANDROID__

} // namespace kikasignature

#endif //SIGNATURE_H
