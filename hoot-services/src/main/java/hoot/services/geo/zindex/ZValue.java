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

/**
 * //TODO: Prototype -- Requires things like basic error checking.
 * 
 * This class is re-entrant but not thread safe.
 */
final public class ZValue {
    private long[] _b;
    private int _depth;
    private int _dimensions;

    private double[] _min, _max;

    private long _range;

    /**
     * Creates a new ZValue calculator
     * 
     * @param dimensions
     *            Number of dimensions in the data
     * @param depth
     *            Depth in bits to calculate. depth * dimensions must be less
     *            than 63.
     */
    public ZValue(int dimensions, int depth, double[] min, double[] max) {
        _depth = depth;
        _dimensions = dimensions;
        _min = min;
        _max = max;
        _range = (1 << (_depth)) - 1;
        _b = new long[dimensions];

    }

    public long calculate(double[] point) {
        for (int i = 0; i < _dimensions; i++) {
            _b[i] = calculateComponent(point[i], i);
        }

        return calculate(_b);
    }

    /**
     * Calculates the z value by interleaving values that are already scaled to
     * the proper space.
     */
    public long calculate(long[] point) {
        long bitRead = 1 << (_depth - 1);
        long result = 0;
        for (int depth = 0; depth < _depth; depth++) {
            // reverse the order so it looks like a "z" and makes it consistent
            // with
            // the Wikipedia
            // definition.
            for (int i = _dimensions - 1; i >= 0; i--) {
                long bit = (point[i] & bitRead) != 0 ? 1 : 0;
                result = (result << 1) | bit;
            }
            bitRead = bitRead >> 1;
        }

        return result;
    }

    /**
     * Calculates the non-interleaved component for one dimension.
     */
    public long calculateComponent(double v, int d) {
        return Math.round((v - _min[d]) / (_max[d] - _min[d]) * _range);
    }

    public void decompose(long v, long[] point) {
        assert (point.length == getDimensions());

        for (int i = 0; i < point.length; i++) {
            point[i] = 0;
        }

        long bitRead = 1 << (_depth * _dimensions - 1);
        for (int depth = 0; depth < _depth; depth++) {
            // reverse the order so it looks like a "z" and makes it consistent
            // with
            // the Wikipedia definition.
            for (int i = _dimensions - 1; i >= 0; i--) {
                long bit = (v & bitRead) != 0 ? 1 : 0;
                point[i] = (point[i] << 1) | bit;
                bitRead = bitRead >> 1;
            }
        }
    }

    public int getDepth() {
        return _depth;
    }

    public int getDimensions() {
        return _dimensions;
    }

    public double getMax(int d) {
        return _max[d];
    }

    /**
     * Returns the maximum value in any one dimension (number of bins in that
     * direction - 1).
     */
    public long getMaxDimensionRange() {
        return _range;
    }

    public double getMin(int d) {
        return _min[d];
    }
}
