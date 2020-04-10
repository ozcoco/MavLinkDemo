package me.oz.demo.ui.ui.gallery;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import me.oz.demo.ui.databinding.FragmentVectorBinding;

/**
 * @ProjectName: MavLink
 * @Package: me.oz.demo.ui.ui.gallery
 * @ClassName: VectorFragment
 * @Description:
 * @Author: oz
 * @CreateDate: 2020/2/24 10:00
 * @UpdateUser:
 * @UpdateDate: 2020/2/24 10:00
 * @UpdateRemark:
 * @Version: 1.0
 */
public class VectorFragment extends Fragment {

    FragmentVectorBinding mBinding;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {

        mBinding = FragmentVectorBinding.inflate(getLayoutInflater(), container, false);

        mBinding.setLifecycleOwner(this);

        return mBinding.getRoot();
    }

}
