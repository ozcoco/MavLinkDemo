package me.oz.demo.mavlink.ui.nativeintent;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProviders;

import me.oz.demo.mavlink.databinding.NativeIntentFragmentBinding;

public class NativeIntentFragment extends Fragment {

    private NativeIntentViewModel mViewModel;

    private NativeIntentFragmentBinding mBinding;

    public static NativeIntentFragment newInstance() {
        return new NativeIntentFragment();
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container,
                             @Nullable Bundle savedInstanceState) {

        mViewModel = ViewModelProviders.of(this).get(NativeIntentViewModel.class);

        mBinding = NativeIntentFragmentBinding.inflate(inflater, container, false);

        mBinding.setLifecycleOwner(this);

        mBinding.setVm(mViewModel);

        return mBinding.getRoot();
    }


    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
    }

}
