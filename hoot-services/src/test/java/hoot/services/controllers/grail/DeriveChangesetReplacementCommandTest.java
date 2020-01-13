package hoot.services.controllers.grail;

import org.junit.Test;

import com.vividsolutions.jts.util.Assert;

public class DeriveChangesetReplacementCommandTest {

    @Test
    public void testExponent() {
        Double delta;
        int exp;

        delta = 134.5;
        exp = DeriveChangesetReplacementCommand.exponent(delta);
        Assert.equals(2, exp, "Scientific notation exponent doesn't match");

        delta = 13.45;
        exp = DeriveChangesetReplacementCommand.exponent(delta);
        Assert.equals(1, exp, "Scientific notation exponent doesn't match");

        delta = 1.345;
        exp = DeriveChangesetReplacementCommand.exponent(delta);
        Assert.equals(0, exp, "Scientific notation exponent doesn't match");

        delta = 0.1345;
        exp = DeriveChangesetReplacementCommand.exponent(delta);
        Assert.equals(-1, exp, "Scientific notation exponent doesn't match");

        delta = 0.01345;
        exp = DeriveChangesetReplacementCommand.exponent(delta);
        Assert.equals(-2, exp, "Scientific notation exponent doesn't match");

        delta = 0.001345;
        exp = DeriveChangesetReplacementCommand.exponent(delta);
        Assert.equals(-3, exp, "Scientific notation exponent doesn't match");

        delta = 0.0001345;
        exp = DeriveChangesetReplacementCommand.exponent(delta);
        Assert.equals(-4, exp, "Scientific notation exponent doesn't match");

        delta = 0.00001345;
        exp = DeriveChangesetReplacementCommand.exponent(delta);
        Assert.equals(-5, exp, "Scientific notation exponent doesn't match");

        delta = 0.000001345;
        exp = DeriveChangesetReplacementCommand.exponent(delta);
        Assert.equals(-6, exp, "Scientific notation exponent doesn't match");

    }

    @Test
    public void testAdaptiveAlpha() {
        Double delta;
        int alpha;

        delta = 134.5;
        alpha = DeriveChangesetReplacementCommand.adaptiveAlpha(delta);
        Assert.equals(1000000, alpha, "Adaptive alpha distance doesn't match");

        delta = 13.45;
        alpha = DeriveChangesetReplacementCommand.adaptiveAlpha(delta);
        Assert.equals(100000, alpha, "Adaptive alpha distance doesn't match");

        delta = 1.345;
        alpha = DeriveChangesetReplacementCommand.adaptiveAlpha(delta);
        Assert.equals(10000, alpha, "Adaptive alpha distance doesn't match");

        delta = 0.1345;
        alpha = DeriveChangesetReplacementCommand.adaptiveAlpha(delta);
        Assert.equals(1000, alpha, "Adaptive alpha distance doesn't match");

        delta = 0.01345;
        alpha = DeriveChangesetReplacementCommand.adaptiveAlpha(delta);
        Assert.equals(100, alpha, "Adaptive alpha distance doesn't match");

        delta = 0.001345;
        alpha = DeriveChangesetReplacementCommand.adaptiveAlpha(delta);
        Assert.equals(10, alpha, "Adaptive alpha distance doesn't match");

        delta = 0.0001345;
        alpha = DeriveChangesetReplacementCommand.adaptiveAlpha(delta);
        Assert.equals(1, alpha, "Adaptive alpha distance doesn't match");

        delta = 0.00001345;
        alpha = DeriveChangesetReplacementCommand.adaptiveAlpha(delta);
        Assert.equals(0, alpha, "Adaptive alpha distance doesn't match");

        delta = 0.000001345;
        alpha = DeriveChangesetReplacementCommand.adaptiveAlpha(delta);
        Assert.equals(0, alpha, "Adaptive alpha distance doesn't match");

    }

}
