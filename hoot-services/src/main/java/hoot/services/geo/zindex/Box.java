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

import java.util.Arrays;

import com.vividsolutions.jts.geom.Envelope;


/**
 * Used to represent a bounding box in Cartesian space.
 */
public final class Box {
    private final double[] min;
    private final double[] max;

    public Box(double[] min, double[] max) {
        this.min = Arrays.copyOf(min, min.length);
        this.max = Arrays.copyOf(max, max.length);
        check();
    }

    public Box(int d) {
        min = new double[d];
        max = new double[d];
    }

    public Box(Envelope envelope) {
        min = new double[2];
        max = new double[2];

        min[0] = envelope.getMinX();
        max[0] = envelope.getMaxX();
        min[1] = envelope.getMinY();
        max[1] = envelope.getMaxY();
    }

    private void check() {
        for (int i = 0; i < min.length; i++) {
            if (min[i] > max[i]) {
                throw new IllegalArgumentException("Min is greater than max. " + toString());
            }
        }
    }

    public int getDimensions() {
        return max.length;
    }

    public double[] getMax() {
        return max;
    }

    public double[] getMin() {
        return min;
    }

    public boolean in(double[] p) {
        boolean result = true;
        for (int i = 0; i < min.length; i++) {
            result = result && (p[i] >= min[i]) && (p[i] <= max[i]);
        }
        return result;
    }

    public boolean intersects(Box b) {
        boolean result = true;
        for (int i = 0; i < min.length; i++) {
            result = result && (b.getMin()[i] <= getMax()[i]);
            result = result && (b.getMax()[i] >= getMin()[i]);
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

        for (int i = 0; i < min.length; i++) {
            d = Math.min(d, Math.abs(min[i] - b.min[i]));
            d = Math.min(d, Math.abs(min[i] - b.max[i]));
            d = Math.min(d, Math.abs(max[i] - b.min[i]));
            d = Math.min(d, Math.abs(max[i] - b.max[i]));
        }

        return d;
    }

    @Override
    public String toString() {
        String result = "{ ";
        for (int i = 0; i < min.length; i++) {
            result += String.format("(%f : %f) ", min[i], max[i]);
        }
        result += "}";
        return result;
    }

    public double getWidth(int d) {
        return max[d] - min[d];
    }

    /**
     * returns true if this box is inside "container"
     */
    public boolean in(Box container) {
        boolean result = true;
        for (int i = 0; i < min.length; i++) {
            result = result && (min[i] >= container.min[i]);
            result = result && (max[i] <= container.max[i]);
        }

        return result;
    }
}
