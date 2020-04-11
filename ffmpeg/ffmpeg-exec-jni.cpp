//
// Created by Administrator on 2020/4/11.
//

extern "C"
{
#include "ffmpeg-exec.h"
}

#include "utils/StringUtils.hpp"
#include <jni.h>

int ffmpeg_exec_jni (int argc, char **argv);

int ffmpeg_exec_jni (int argc, char **argv)
{
  return ffmpeg_exec (argc, argv);
}

using String = char *;

extern "C"
JNIEXPORT jint
JNICALL
Java_com_xdynamics_av_FFmepgUtils_native_1exec (JNIEnv *env, jclass type, jstring __cmd)
{

  const char *cmd = env->GetStringUTFChars (__cmd, 0);

  std::vector<std::string> vecCmd = ::split (cmd, " ");

  const int argc = static_cast<int>(vecCmd.size ());

  String *argv = new String[vecCmd.size ()];

  for (int i = 0; i < vecCmd.size (); ++i)
    {

      argv[i] = vecCmd[i].data ();

    }

  int ret = ffmpeg_exec_jni (argc, argv);

  delete[] argv;

  env->ReleaseStringUTFChars (__cmd, cmd);

  return static_cast<jint>(ret);
}

