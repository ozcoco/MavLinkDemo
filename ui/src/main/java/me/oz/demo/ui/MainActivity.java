package me.oz.demo.ui;

import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

import java.util.Random;

public class MainActivity extends AppCompatActivity {

    private DegreeView v_degree;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        v_degree = findViewById(R.id.v_degree);

        v_degree.postDelayed(this::updateDegree, 1000);

    }

    private void updateDegree() {

        Random random = new Random();

        final int degree = random.nextInt(360);

        v_degree.setDegree(degree);

        String text = degree + "°";

        v_degree.setText(text);

        v_degree.invalidate();

        v_degree.postDelayed(this::updateDegree, 500
        );

    }


}
