/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.geo.zindex;

import java.util.List;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;


public class ZCurveRangerTest {
    @Test
    @Category(UnitTest.class)
    public void testMaxBitColumn() {
        ZValue zv = new ZValue(2, 4, new double[] { 0, 1 }, new double[] { 0, 1 });
        ZCurveRanger ranger = new ZCurveRanger(zv);
        Assert.assertEquals(7, ZCurveRanger.getMaxBitColumn(0xFF));
        Assert.assertEquals(0, ZCurveRanger.getMaxBitColumn(0x1));
        Assert.assertEquals(1, ZCurveRanger.getMaxBitColumn(0x2));
    }

    @Test
    @Category(UnitTest.class)
    public void testGetSplitValue() {
        ZValue zv = new ZValue(2, 4, new double[] { 0, 1 }, new double[] { 0, 1 });
        ZCurveRanger ranger = new ZCurveRanger(zv);

        Assert.assertEquals(0x10, ZCurveRanger.getSplitValue(0x0, 0x10));
        Assert.assertEquals(0x110, ZCurveRanger.getSplitValue(0x100, 0x113));
        Assert.assertEquals(0x4, ZCurveRanger.getSplitValue(0x3, 0x5));
    }

    @Test
    @Category(UnitTest.class)
    public void testBreakBox() {
        ZValue zv = new ZValue(2, 4, new double[] { 0, 1 }, new double[] { 0, 1 });
        ZCurveRanger ranger = new ZCurveRanger(zv);

        LongBox in;
        LongBox[] lb;

        in = new LongBox(new long[] { 2, 0 }, new long[] { 5, 3 });
        lb = ranger.breakBox(in);
        Assert.assertEquals("{ (2 : 3) (0 : 3) }", lb[0].toString());
        Assert.assertEquals("{ (4 : 5) (0 : 3) }", lb[1].toString());

        in = new LongBox(new long[] { 2, 3 }, new long[] { 5, 4 });
        lb = ranger.breakBox(in);
        Assert.assertEquals("{ (2 : 5) (3 : 3) }", lb[0].toString());
        Assert.assertEquals("{ (2 : 5) (4 : 4) }", lb[1].toString());

        in = new LongBox(new long[] { 2, 2 }, new long[] { 4, 7 });
        lb = ranger.breakBox(in);
        Assert.assertEquals("{ (2 : 4) (2 : 3) }", lb[0].toString());
        Assert.assertEquals("{ (2 : 4) (4 : 7) }", lb[1].toString());

        in = new LongBox(new long[] { 4, 4 }, new long[] { 7, 5 });
        lb = ranger.breakBox(in);
        Assert.assertEquals("{ (4 : 5) (4 : 5) }", lb[0].toString());
        Assert.assertEquals("{ (6 : 7) (4 : 5) }", lb[1].toString());
    }

    @Test
    @Category(UnitTest.class)
    public void testDecomposeBox() {
        ZValue zv = new ZValue(2, 4, new double[] { 0, 1 }, new double[] { 0, 1 });
        ZCurveRanger ranger = new ZCurveRanger(zv);

        LongBox in;
        List<LongBox> lb;

        in = new LongBox(new long[] { 2, 0 }, new long[] { 5, 3 });
        lb = ranger.decomposeBox(in, 1);
        Assert.assertEquals("{ (2 : 3) (0 : 1) }", lb.get(0).toString());
        Assert.assertEquals("{ (2 : 3) (2 : 3) }", lb.get(1).toString());
        Assert.assertEquals("{ (4 : 5) (0 : 1) }", lb.get(2).toString());
        Assert.assertEquals("{ (4 : 5) (2 : 3) }", lb.get(3).toString());

        in = new LongBox(new long[] { 0, 0 }, new long[] { 3, 7 });
        lb = ranger.decomposeBox(in, 1);
        Assert.assertEquals("{ (0 : 3) (0 : 1) }", lb.get(0).toString());
        Assert.assertEquals("{ (0 : 3) (2 : 3) }", lb.get(1).toString());
        Assert.assertEquals("{ (0 : 3) (4 : 5) }", lb.get(2).toString());
        Assert.assertEquals("{ (0 : 3) (6 : 7) }", lb.get(3).toString());

        in = new LongBox(new long[] { 1, 2 }, new long[] { 2, 7 });
        lb = ranger.decomposeBox(in, 1);
        Assert.assertEquals("{ (1 : 1) (2 : 3) }", lb.get(0).toString());
        Assert.assertEquals("{ (2 : 2) (2 : 3) }", lb.get(1).toString());
        Assert.assertEquals("{ (1 : 2) (4 : 5) }", lb.get(2).toString());
        Assert.assertEquals("{ (1 : 2) (6 : 7) }", lb.get(3).toString());
    }

    @Test
    @Category(UnitTest.class)
    public void testDecomposeRange() {
        ZValue zv = new ZValue(2, 4, new double[] { 0, 1 }, new double[] { 0, 1 });
        ZCurveRanger ranger = new ZCurveRanger(zv);

        List<Range> r;
        LongBox in;

        in = new LongBox(new long[] { 2, 0 }, new long[] { 5, 3 });
        r = ranger.decomposeRange(in, 0);
        Assert.assertEquals("[4 : 27]", r.toString());

        in = new LongBox(new long[] { 1, 0 }, new long[] { 2, 7 });
        r = ranger.decomposeRange(in, 1);
        Assert.assertEquals("[1 : 14, 33 : 46]", r.toString());

        in = new LongBox(new long[] { 1, 0 }, new long[] { 2, 7 });
        ranger.setSlop(1);
        r = ranger.decomposeRange(in, 1);
        Assert.assertEquals("[1 : 6, 9 : 14, 33 : 38, 41 : 46]", r.toString());
    }

    @Test
    @Category(UnitTest.class)
    public void test2D() {
        ZValue zv = new ZValue(2, 10, new double[] { -180, -90 }, new double[] { 180, 90 });
        ZCurveRanger ranger = new ZCurveRanger(zv);

        List<Range> r;
        Box b;

        b = new Box(new double[] { -105, 38 }, new double[] { -104.9, 38.1 });
        r = ranger.decomposeRange(b, 1);
        // this has _not_ been manually verified. It just looks right and
        // provides feedback on changes.
        Assert.assertEquals("[586555 : 586555, 586641 : 586641]", r.toString());
    }
}
