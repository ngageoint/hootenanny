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

import com.vividsolutions.jts.geom.Envelope;


/**
 * Used to represent a bounding box in Cartesian space.
 */
final public class Box {
    private double[] _min, _max;

    public Box(double[] min, double[] max) {
        assert (min.length == max.length);
        _min = Arrays.copyOf(min, min.length);
        _max = Arrays.copyOf(max, max.length);
        _check();
    }

    public Box(int d) {
        _min = new double[d];
        _max = new double[d];
    }

    public Box(Envelope envelope) {
        _min = new double[2];
        _max = new double[2];

        _min[0] = envelope.getMinX();
        _max[0] = envelope.getMaxX();
        _min[1] = envelope.getMinY();
        _max[1] = envelope.getMaxY();
    }

    public void _check() {
        for (int i = 0; i < _min.length; i++) {
            if (_min[i] > _max[i]) {
                throw new IllegalArgumentException("Min is greater than max. " + toString());
            }
        }
    }

    public int getDimensions() {
        return _max.length;
    }

    public double[] getMax() {
        return _max;
    }

    public double[] getMin() {
        return _min;
    }

    public boolean in(double[] p) {
        boolean result = true;
        for (int i = 0; i < _min.length; i++) {
            result = result && p[i] >= _min[i] && p[i] <= _max[i];
        }
        return result;
    }

    public boolean intersects(Box b) {
        assert (b.getDimensions() == getDimensions());

        boolean result = true;
        for (int i = 0; i < _min.length; i++) {
            result = result && b.getMin()[i] <= getMax()[i];
            result = result && b.getMax()[i] >= getMin()[i];
        }

        return result;
    }

    /**
     * Returns the minimum distance in any one dimension. This is not
     * necessarily the Euclidean distance.
     */
    public double manhattanDistance(Box b) {
        if (intersects(b)) {
            return 0.0;
        }

        double d = Double.MAX_VALUE;

        for (int i = 0; i < _min.length; i++) {
            d = Math.min(d, Math.abs(_min[i] - b._min[i]));
            d = Math.min(d, Math.abs(_min[i] - b._max[i]));
            d = Math.min(d, Math.abs(_max[i] - b._min[i]));
            d = Math.min(d, Math.abs(_max[i] - b._max[i]));
        }

        return d;
    }

    @Override
    public String toString() {
        String result = "{ ";
        for (int i = 0; i < _min.length; i++) {
            result += String.format("(%f : %f) ", _min[i], _max[i]);
        }
        result += "}";
        return result;
    }

    public double getWidth(int d) {
        return _max[d] - _min[d];
    }

    /**
     * returns true if this box is inside "container"
     */
    public boolean in(Box container) {
        assert (container.getDimensions() == getDimensions());

        boolean result = true;
        for (int i = 0; i < _min.length; i++) {
            result = result && _min[i] >= container._min[i];
            result = result && _max[i] <= container._max[i];
        }

        return result;
    }
}
