package me.oz.demo.mavlink.utils;

import android.graphics.Bitmap;

import java.nio.ByteBuffer;

/**
 * @ProjectName: MavLink
 * @Package: me.oz.demo.mavlink.utils
 * @ClassName: CvUtils
 * @Description:
 * @Author: oz
 * @CreateDate: 2020/1/7 16:58
 * @UpdateUser:
 * @UpdateDate: 2020/1/7 16:58
 * @UpdateRemark:
 * @Version: 1.0
 */
public class CvUtils {

    static {
        System.loadLibrary("cv-lib");
    }

    public native static byte[] nativeNV21Gray(byte[] yuv, int width, int height);

    public native static void nativeBitmapGray(Bitmap bmp);

    public native static void nativeYUV420888Gray(ByteBuffer y, ByteBuffer u, ByteBuffer v, int width, int height);

    public native static void nativeYUV420888Bitmap565(ByteBuffer y, ByteBuffer u, ByteBuffer v, Bitmap bmp, int width, int height);

}
