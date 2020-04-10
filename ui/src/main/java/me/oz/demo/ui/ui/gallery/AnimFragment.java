package me.oz.demo.ui.ui.gallery;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.google.android.material.tabs.TabLayoutMediator;

import java.util.ArrayList;
import java.util.List;

import me.oz.demo.ui.databinding.FragmentAnimBinding;
import me.oz.demo.ui.ui.adpater.FragmentPageAdapter;

public class AnimFragment extends Fragment {

    private AnimViewModel mViewModel;

    private FragmentAnimBinding mBinding;

    private final static String[] TITLES = {"Vector", "VectorAnim"};

    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {

        mViewModel = new ViewModelProvider(this).get(AnimViewModel.class);

        mBinding = FragmentAnimBinding.inflate(inflater, container, false);

        mBinding.setLifecycleOwner(this);

        return mBinding.getRoot();
    }


    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        List<Fragment> fragments = new ArrayList<>();

        fragments.add(new VectorFragment());

        fragments.add(new VectorAnimFragment());

        mBinding.pager.setOffscreenPageLimit(fragments.size());

        mBinding.pager.setAdapter(new FragmentPageAdapter(getChildFragmentManager(), getLifecycle(), fragments));

        new TabLayoutMediator(mBinding.tabTop, mBinding.pager, true, (tab, position) -> {

            tab.setText(TITLES[position]);

        }).attach();

    }
}