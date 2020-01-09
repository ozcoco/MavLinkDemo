package me.oz.demo.mavlink;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import me.oz.demo.mavlink.ui.playvideowithbitmap.PlayVideoWithBitmapFragment;

public class PlayVideoWithBitmapActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.play_video_with_bitmap_activity);
        if (savedInstanceState == null) {
            getSupportFragmentManager().beginTransaction()
                    .replace(R.id.container, PlayVideoWithBitmapFragment.newInstance())
                    .commitNow();
        }
    }
}
