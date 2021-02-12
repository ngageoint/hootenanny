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
 * @copyright Copyright (C) 2015, 2016 Maxar (http://www.maxar.com/)
 */
package hoot.services.geo.zindex;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;


public class ZValueTest {
    // recreate the wikipedia example output.
    // http://en.wikipedia.org/wiki/Z-order_curve
    @Test
    @Category(UnitTest.class)
    public void testBasics() throws Exception {
        ZValue zv = new ZValue(2, 3, new double[] { 0, 0 }, new double[] { 7, 7 });

        Assert.assertEquals(0x00, zv.calculate(new double[] { 0, 0 }));
        Assert.assertEquals(0x01, zv.calculate(new double[] { 1, 0 }));
        Assert.assertEquals(0x02, zv.calculate(new double[] { 0, 1 }));
        Assert.assertEquals(0x03, zv.calculate(new double[] { 1, 1 }));
        Assert.assertEquals(0x3b, zv.calculate(new double[] { 5, 7 }));
        Assert.assertEquals(0x3e, zv.calculate(new double[] { 6, 7 }));
        Assert.assertEquals(0x3f, zv.calculate(new double[] { 7, 7 }));
    }

    @Test
    @Category(UnitTest.class)
    public void testDecompose() {
        ZValue zv = new ZValue(2, 3, new double[] { 0, 0 }, new double[] { 7, 7 });

        long[] ll = new long[2];

        zv.decompose(37, ll);
        Assert.assertEquals(3, ll[0]);
        Assert.assertEquals(4, ll[1]);

        zv.decompose(25, ll);
        Assert.assertEquals(5, ll[0]);
        Assert.assertEquals(2, ll[1]);
    }

    @Test
    @Category(UnitTest.class)
    public void testGeographic() throws Exception {
        ZValue zv = new ZValue(2, 4, new double[] { -180, -90 }, new double[] { 180, 90 });

        Assert.assertEquals(0x0, zv.calculate(new double[] { -180, -90 }));
        Assert.assertEquals(0xC0, zv.calculate(new double[] { 0, 0 }));
        Assert.assertEquals(0xFF, zv.calculate(new double[] { 180, 90 }));
        Assert.assertEquals(0xEA, zv.calculate(new double[] { 0, 90 }));
    }

    // this test simply looks right. I haven't done thorough testing of the
    // ins/outs
    public void test4D() throws Exception {
        ZValue zv = new ZValue(4, 4, new double[] { -180, -90, -180, -90 }, new double[] { 180, 90, 180, 90 });

        Assert.assertEquals(0x0, zv.calculate(new double[] { -180, -90, -180, -90 }));
        Assert.assertEquals(0xFFFF, zv.calculate(new double[] { 180, 90, 180, 90 }));
        Assert.assertEquals(0xF000, zv.calculate(new double[] { 0, 0, 0, 0 }));
    }
}
