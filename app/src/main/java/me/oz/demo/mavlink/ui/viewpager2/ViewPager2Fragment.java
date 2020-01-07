package me.oz.demo.mavlink.ui.viewpager2;

import androidx.lifecycle.ViewModelProviders;

import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import me.oz.demo.mavlink.R;

public class ViewPager2Fragment extends Fragment {

    private ViewPager2ViewModel mViewModel;

    public static ViewPager2Fragment newInstance() {
        return new ViewPager2Fragment();
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container,
                             @Nullable Bundle savedInstanceState) {
        return inflater.inflate(R.layout.view_pager2_fragment, container, false);
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        mViewModel = ViewModelProviders.of(this).get(ViewPager2ViewModel.class);
        // TODO: Use the ViewModel
    }

}
