/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class at_o2xfs_xfs_service_std_win32_Win32MessageLoop */

#ifndef _Included_at_o2xfs_xfs_service_std_win32_Win32MessageLoop
#define _Included_at_o2xfs_xfs_service_std_win32_Win32MessageLoop
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     at_o2xfs_xfs_service_std_win32_Win32MessageLoop
 * Method:    createWindow
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_at_o2xfs_xfs_service_std_win32_Win32MessageLoop_createWindow
  (JNIEnv *, jobject);

/*
 * Class:     at_o2xfs_xfs_service_std_win32_Win32MessageLoop
 * Method:    doRun
 * Signature: ([BLat/o2xfs/xfs/service/std/WindowProcCallback;)V
 */
JNIEXPORT void JNICALL Java_at_o2xfs_xfs_service_std_win32_Win32MessageLoop_runLoop(JNIEnv *, jobject, jobject);

/*
 * Class:     at_o2xfs_xfs_service_std_win32_Win32MessageLoop
 * Method:    close
 * Signature: ([B)V
 */
JNIEXPORT void JNICALL Java_at_o2xfs_xfs_service_std_win32_Win32MessageLoop_close
  (JNIEnv *, jobject, jbyteArray);

#ifdef __cplusplus
}
#endif
#endif