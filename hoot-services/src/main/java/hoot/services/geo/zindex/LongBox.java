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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.geo.zindex;

import java.util.Arrays;


/**
 * Used to represent a bounding box in Z-Value space.
 */
final public class LongBox {
    static public String toBinary(long v, int bits) {
        String format = String.format("%%%ds", bits);
        return String.format(format, Long.toBinaryString(v)).replace(" ", "0");
    }

    long[] _min, _max;

    public LongBox(long[] min, long[] max) {
        assert (min.length == max.length);
        _min = min;
        _max = max;
    }

    public LongBox(LongBox box) {
        _min = Arrays.copyOf(box._min, box._min.length);
        _max = Arrays.copyOf(box._max, box._max.length);
    }

    public long calculateVolume() {
        long result = 1;
        for (int i = 0; i < _min.length; i++) {
            result *= getWidth(i);
        }
        return result;
    }

    /**
     * Returns true if at least one edge overlaps with one of the other boxes
     * edges.
     */
    public boolean edgeOverlaps(LongBox b) {
        boolean result = false;

        for (int i = 0; i < _min.length; i++) {
            result = result || b.getMin()[i] == getMax()[i] || b.getMin()[i] == b.getMin()[i]
                    || b.getMax()[i] == b.getMax()[i];
        }

        return result;
    }

    public LongBox expand(int size) {
        for (int i = 0; i < _min.length; i++) {
            _min[i] -= size;
            _max[i] += size;
        }

        return this;
    }

    public int getDimensions() {
        return _min.length;
    }

    public long[] getMax() {
        return _max;
    }

    public long[] getMin() {
        return _min;
    }

    public long getWidth(int d) {
        return _max[d] - _min[d] + 1;
    }

    public boolean in(long[] p) {
        boolean result = true;
        for (int i = 0; i < _min.length; i++) {
            result = result && p[i] >= _min[i] && p[i] <= _max[i];
        }
        return result;
    }

    public boolean intersects(LongBox b) {
        assert (b.getDimensions() == getDimensions());

        boolean result = true;
        for (int i = 0; i < _min.length; i++) {
            result = result && b.getMin()[i] <= getMax()[i];
            result = result && b.getMax()[i] >= getMin()[i];
        }

        return result;
    }

    public String toBinaryString(int bits) {
        String result = "{ ";
        for (int i = 0; i < _min.length; i++) {
            result += String.format("(%s : %s) ", toBinary(_min[i], bits), toBinary(_max[i], bits));
        }
        result += "}";
        return result;
    }

    @Override
    public String toString() {
        String result = "{ ";
        for (int i = 0; i < _min.length; i++) {
            result += String.format("(%d : %d) ", _min[i], _max[i]);
        }
        result += "}";
        return result;
    }
}
