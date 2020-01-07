package me.oz.demo.mavlink;

import android.Manifest;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.util.Size;
import android.view.ViewGroup;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.camera.core.CameraX;
import androidx.camera.core.ImageAnalysis;
import androidx.camera.core.ImageAnalysisConfig;
import androidx.camera.core.Preview;
import androidx.camera.core.PreviewConfig;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.lifecycle.ViewModelProviders;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import me.oz.demo.mavlink.databinding.ActivityMainBinding;
import me.oz.demo.mavlink.utils.CvUtils;
import me.oz.demo.mavlink.utils.ImageUtils;

public class MainActivity extends AppCompatActivity {

    private final static int REQUEST_CODE_PERMISSIONS = 0x1010;

    private ActivityMainBinding mBinding;

    private MainViewModel mVm;

    private ExecutorService mExecutor;

    private Size imageSize;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mBinding = ActivityMainBinding.inflate(getLayoutInflater());

        mBinding.setLifecycleOwner(this);

        mVm = ViewModelProviders.of(this).get(MainViewModel.class);

        mBinding.setVm(mVm);

        setContentView(mBinding.getRoot());

        requestPermission();

    }

    private void requestPermission() {

        String[] permissions = new String[]{
                Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.CAMERA
        };

        boolean isGrant = true;

        for (String permission : permissions) {
            if (ContextCompat.checkSelfPermission(this, permission) != PackageManager.PERMISSION_GRANTED) {
                isGrant = false;
                break;
            }
        }

        if (isGrant) {

            startCamera();

        } else {

            ActivityCompat.requestPermissions(this, permissions, REQUEST_CODE_PERMISSIONS);

        }
    }

    private void startCamera() {

        mExecutor = Executors.newSingleThreadExecutor();

        mBinding.viewFinder.addOnLayoutChangeListener((view, i, i1, i2, i3, i4, i5, i6, i7) -> updateTransform());

        DisplayMetrics displayMetrics = new DisplayMetrics();

        getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);

        imageSize = new Size(displayMetrics.widthPixels, displayMetrics.heightPixels);

        PreviewConfig previewConfig = new PreviewConfig.Builder().setTargetResolution(imageSize).build();

        Preview preview = new Preview(previewConfig);

        preview.setOnPreviewOutputUpdateListener(output -> {

            ViewGroup parent = (ViewGroup) mBinding.viewFinder.getParent();

            parent.removeView(mBinding.viewFinder);

            parent.addView(mBinding.viewFinder, 0);

            mBinding.viewFinder.setSurfaceTexture(output.getSurfaceTexture());

            updateTransform();

        });

        CameraX.bindToLifecycle(this, analysis(), preview);
    }


    private void updateTransform() {
        Matrix matrix = new Matrix();
        // Compute the center of the view finder
        float centerX = mBinding.viewFinder.getWidth() / 2f;
        float centerY = mBinding.viewFinder.getHeight() / 2f;

        float[] rotations = {0, 90, 180, 270};
        // Correct preview output to account for display rotation
        float rotationDegrees = rotations[mBinding.viewFinder.getDisplay().getRotation()];

        matrix.postRotate(-rotationDegrees, centerX, centerY);

        // Finally, apply transformations to our TextureView
        mBinding.viewFinder.setTransform(matrix);
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();

        CameraX.unbindAll();
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

        if (requestCode == REQUEST_CODE_PERMISSIONS) {

            boolean isGrant = true;

            for (int grantResult : grantResults) {
                if (grantResult != PackageManager.PERMISSION_GRANTED) {
                    isGrant = false;
                    break;
                }
            }

            if (isGrant) {

                startCamera();

            } else {

                Toast.makeText(getApplicationContext(), "No permission!!!", Toast.LENGTH_SHORT).show();

            }
        }

    }


    private ImageAnalysis analysis() {

        ImageAnalysisConfig config =
                new ImageAnalysisConfig.Builder()
                        .setTargetResolution(imageSize)
                        .setImageReaderMode(ImageAnalysis.ImageReaderMode.ACQUIRE_LATEST_IMAGE)
                        .build();

        ImageAnalysis imageAnalysis = new ImageAnalysis(config);

        imageAnalysis.setAnalyzer(mExecutor, (image, rotationDegrees) -> {

            if (image.getImage() == null) return;

            Log.d("^_^ ---->", "image format: " + image.getImage().getFormat());

//            byte[] pxs = ImageUtils.imageToByteArray(image.getImage());

            byte[] pxs = CvUtils.native_NV21_gray(ImageUtils.YUV_420_888toNV21(image.getImage()), image.getWidth(), image.getHeight());

            Bitmap bmp = BitmapFactory.decodeByteArray(pxs, 0, pxs.length);

            mBinding.imgShow.post(() -> {

                mBinding.imgShow.setRotation(rotationDegrees);

                mBinding.imgShow.setImageBitmap(bmp);
            });
        });

        return imageAnalysis;
    }
}
