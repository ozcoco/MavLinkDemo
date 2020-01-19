package me.oz.demo.mavlink.net;

/**
 * @ProjectName: MavLink
 * @Package: me.oz.demo.mavlink.net
 * @ClassName: NativeHttpClient
 * @Description:
 * @Author: oz
 * @CreateDate: 2020/1/19 11:16
 * @UpdateUser:
 * @UpdateDate: 2020/1/19 11:16
 * @UpdateRemark:
 * @Version: 1.0
 */
public final class NativeHttpClient {

    static {

        System.loadLibrary("net-lib");

    }

    public native static String get(String url);

}
