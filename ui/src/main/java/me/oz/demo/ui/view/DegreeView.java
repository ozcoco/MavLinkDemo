package me.oz.demo.ui.view;

import android.content.Context;
import android.content.res.Resources;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.text.TextPaint;
import android.util.AttributeSet;
import android.view.View;

import me.oz.demo.ui.R;

/**
 * TODO: document your custom view class.
 */
public class DegreeView extends View {

    private final static int BEGIN_LEFT = 0;

    private final static int BEGIN_RIGHT = 1;

    private int gap = 10;

    private int degree = 90;

    private int indicatorColor = Color.WHITE;

    private int indicatorWidth = 0;

    private int indicatorHeight = 0;

    private int indicatorRadius = 0;

    private int tickMarksColor = Color.WHITE;

    private int tickMarksWidth = 0;

    private int tickMarksHeight = 0;

    private int tickMarksRadius = 0;

    private CharSequence text = "";

    private int textColor = Color.WHITE;

    private int textSize = 16;

    private int begin = BEGIN_RIGHT;

    private TextPaint textPaint;

    public DegreeView(Context context) {
        super(context);
        init(null, 0);
    }

    public DegreeView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(attrs, 0);
    }

    public DegreeView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        init(attrs, defStyle);
    }

    private void init(AttributeSet attrs, int defStyle) {
        // Load attributes
        final TypedArray a = getContext().obtainStyledAttributes(
                attrs, R.styleable.DegreeView, defStyle, 0);

        textColor = a.getColor(
                R.styleable.DegreeView_textColor,
                textColor);

        indicatorColor = a.getColor(
                R.styleable.DegreeView_indicatorColor,
                indicatorColor);

        tickMarksColor = a.getColor(
                R.styleable.DegreeView_tickMarksColor,
                tickMarksColor);

        gap = a.getInt(R.styleable.DegreeView_gap, gap);

        degree = a.getInt(R.styleable.DegreeView_degree, degree);

        begin = a.getInt(R.styleable.DegreeView_begin, begin);

        text = a.getText(R.styleable.DegreeView_text);

        textSize = a.getDimensionPixelSize(R.styleable.DegreeView_textSize, textSize);

        indicatorWidth = a.getDimensionPixelSize(R.styleable.DegreeView_indicatorWidth, indicatorWidth);

        indicatorHeight = a.getDimensionPixelSize(R.styleable.DegreeView_indicatorHeight, indicatorHeight);

        indicatorRadius = a.getDimensionPixelSize(R.styleable.DegreeView_indicatorRadius, indicatorRadius);

        tickMarksWidth = a.getDimensionPixelSize(R.styleable.DegreeView_tickMarksWidth, tickMarksWidth);

        tickMarksHeight = a.getDimensionPixelSize(R.styleable.DegreeView_tickMarksHeight, tickMarksHeight);

        tickMarksRadius = a.getDimensionPixelSize(R.styleable.DegreeView_tickMarksRadius, tickMarksRadius);

        a.recycle();

        final Resources res = getResources();

        textPaint = new TextPaint(Paint.ANTI_ALIAS_FLAG);

        textPaint.setTextAlign(Paint.Align.LEFT);

        textPaint.density = res.getDisplayMetrics().density;

        textPaint.setAntiAlias(true);

        textPaint.setDither(true);

    }


    private RectF createTickWithBegin() {

        RectF rt = null;

        final int inner = (indicatorWidth - tickMarksWidth) / 2;

        final int py = getHeight() / 2;

        switch (begin) {

            case BEGIN_LEFT:

                rt = new RectF(getPaddingStart() + inner,
                        py - Math.round(tickMarksHeight / 2),
                        getPaddingStart() + inner + tickMarksWidth,
                        py + Math.round(tickMarksHeight / 2));

                break;

            case BEGIN_RIGHT:

                rt = new RectF(getWidth() - getPaddingEnd() - tickMarksWidth - inner,
                        py - Math.round(tickMarksHeight / 2),
                        getWidth() - getPaddingEnd() - inner,
                        py + Math.round(tickMarksHeight / 2));

                break;
        }

        return rt;
    }

    private void drawTickMarks(Canvas canvas) {

        textPaint.setColor(tickMarksColor);

        final int px = getWidth() / 2;

        final int py = getHeight() / 2;

        final RectF rt = createTickWithBegin();

        final int N = Math.abs(Math.round(degree / gap));

        canvas.save();

        for (int i = 0; i <= N; i++) {

            canvas.drawRoundRect(rt, tickMarksRadius, tickMarksRadius, textPaint);

            canvas.rotate(degree < 0 ? -gap : gap, px, py);

        }

        canvas.restore();
    }


    private RectF createIndicatorWithBegin() {

        RectF rt = null;

        final int py = getHeight() / 2;

        switch (begin) {

            case BEGIN_LEFT:

                rt = new RectF(getPaddingStart(),
                        py - Math.round(indicatorHeight / 2),
                        getPaddingStart() + indicatorWidth,
                        py + Math.round(indicatorHeight / 2));
                break;

            case BEGIN_RIGHT:

                rt = new RectF(getWidth() - getPaddingEnd() - indicatorWidth,
                        py - Math.round(indicatorHeight / 2),
                        getWidth() - getPaddingEnd(),
                        py + Math.round(indicatorHeight / 2));
                break;
        }

        return rt;
    }

    private void drawIndicator(Canvas canvas) {

        textPaint.setColor(indicatorColor);

        final int px = getWidth() / 2;

        final int py = getHeight() / 2;

        final RectF rt = createIndicatorWithBegin();

        final int N = Math.abs(Math.round(degree / gap));

        canvas.save();

        canvas.rotate(N * (degree < 0 ? -gap : gap), px, py);

        canvas.drawRoundRect(rt, indicatorRadius, indicatorRadius, textPaint);

        canvas.restore();

    }


    private void drawText(Canvas canvas) {

        textPaint.setColor(textColor);

        textPaint.setStyle(Paint.Style.FILL);

        textPaint.setTextSize(textSize);

        textPaint.setTextAlign(Paint.Align.CENTER);

        final int px = getWidth() / 2;

        final int py = getHeight() / 2;

        float baseLineY = Math.abs(textPaint.ascent() + textPaint.descent()) / 2;

        canvas.drawText(text, 0, text.length(), px, py + baseLineY, textPaint);

    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        drawTickMarks(canvas);

        drawIndicator(canvas);

        drawText(canvas);

    }


    public int getDegree() {
        return degree;
    }

    public void setDegree(int degree) {
        this.degree = degree;
        invalidate();
    }

    public CharSequence getText() {
        return text;
    }

    public void setText(CharSequence text) {
        this.text = text;
        invalidate();
    }
}
