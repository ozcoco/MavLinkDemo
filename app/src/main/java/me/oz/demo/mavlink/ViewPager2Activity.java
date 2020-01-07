package me.oz.demo.mavlink;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import me.oz.demo.mavlink.ui.viewpager2.ViewPager2Fragment;

public class ViewPager2Activity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.view_pager2_activity);
        if (savedInstanceState == null) {
            getSupportFragmentManager().beginTransaction()
                    .replace(R.id.container, ViewPager2Fragment.newInstance())
                    .commitNow();
        }
    }
}
