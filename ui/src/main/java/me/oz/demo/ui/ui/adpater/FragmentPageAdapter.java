package me.oz.demo.ui.ui.adpater;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.lifecycle.Lifecycle;
import androidx.viewpager2.adapter.FragmentStateAdapter;

import java.util.List;

/**
 * @ProjectName: MavLink
 * @Package: me.oz.demo.ui.ui.adpater
 * @ClassName: FragmentPageAdapter
 * @Description:
 * @Author: oz
 * @CreateDate: 2020/2/24 9:53
 * @UpdateUser:
 * @UpdateDate: 2020/2/24 9:53
 * @UpdateRemark:
 * @Version: 1.0
 */
public class FragmentPageAdapter extends FragmentStateAdapter {

    private List<Fragment> mFragments;

    public FragmentPageAdapter(@NonNull FragmentManager fragmentManager, @NonNull Lifecycle lifecycle, List<Fragment> mFragments) {
        super(fragmentManager, lifecycle);

        this.mFragments = mFragments;
    }

    @NonNull
    @Override
    public Fragment createFragment(int position) {
        return mFragments.get(position);
    }

    @Override
    public int getItemCount() {
        return mFragments.size();
    }

}
