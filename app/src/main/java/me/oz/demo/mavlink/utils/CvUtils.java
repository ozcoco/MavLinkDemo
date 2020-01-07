package me.oz.demo.mavlink.utils;

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


    public native static byte[] native_NV21_gray(byte[] yuv, int width, int height);

}
