package me.oz.demo.mavlink.ui.nativeintent;

import android.Manifest;
import android.app.Application;
import android.content.pm.PackageManager;

import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;
import androidx.lifecycle.AndroidViewModel;
import androidx.lifecycle.MutableLiveData;

import java.util.concurrent.Executors;

import me.oz.demo.mavlink.net.NativeHttpClient;

public class NativeIntentViewModel extends AndroidViewModel {

    public MutableLiveData<String> msg = new MutableLiveData<>();

    public NativeIntentViewModel(@NonNull Application application) {
        super(application);

        int ret = ActivityCompat.checkSelfPermission(application, Manifest.permission.INTERNET);

        if (ret == PackageManager.PERMISSION_GRANTED) {

            init();
        }

    }

    private void init() {

        Executors.newSingleThreadExecutor().execute(() -> {
            msg.postValue(NativeHttpClient.get("http://www.baidu.com"));
        });
    }

}
