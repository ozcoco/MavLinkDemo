package me.oz.demo.mavlink.ui.playvideowithbitmap;

import android.graphics.SurfaceTexture;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.view.LayoutInflater;
import android.view.TextureView;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.core.os.HandlerCompat;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProviders;

import me.oz.demo.mavlink.databinding.PlayVideoWithBitmapFragmentBinding;

public class PlayVideoWithBitmapFragment extends Fragment implements TextureView.SurfaceTextureListener {

    private PlayVideoWithBitmapViewModel mVm;

    private PlayVideoWithBitmapFragmentBinding mBinding;

    public static PlayVideoWithBitmapFragment newInstance() {
        return new PlayVideoWithBitmapFragment();
    }


    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container,
                             @Nullable Bundle savedInstanceState) {

        mBinding = PlayVideoWithBitmapFragmentBinding.inflate(inflater, container, false);

        mBinding.setLifecycleOwner(this);

        mVm = ViewModelProviders.of(this).get(PlayVideoWithBitmapViewModel.class);

        mBinding.setVm(mVm);

        return mBinding.getRoot();
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        init();
    }

    private Handler mUiHandler;

    private HandlerThread mUiHandlerThread;

    private void init() {

        mUiHandlerThread = new HandlerThread("mUiHandlerThread");

        mUiHandlerThread.start();

        mUiHandler = HandlerCompat.createAsync(mUiHandlerThread.getLooper());

        mBinding.texture.setSurfaceTextureListener(this);

        updateUI();
    }

    private void updateUI() {

        mUiHandler.postDelayed(() -> {

            mBinding.texture.getSurfaceTexture().updateTexImage();

            updateUI();

        }, 16);
    }


    @Override
    public void onDestroyView() {
        super.onDestroyView();

        if (mUiHandlerThread != null) {

            mUiHandlerThread.quitSafely();

        }

        if (mUiHandler != null) {

            mUiHandler.removeCallbacksAndMessages(null);

            mUiHandler = null;
        }

    }

    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {

    }

    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {

    }

    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        return false;
    }

    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {

    }
}
