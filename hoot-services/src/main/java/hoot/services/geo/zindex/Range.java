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
 * @copyright Copyright (C) 2015, 2016, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.geo.zindex;

/**
 * Range of values. Min is inclusive. Max is inclusive.
 */
public class Range implements Comparable<Range> {
    private long max;
    private long min;

    public Range() {
    }

    public Range(long min, long max) {
        set(min, max);
    }

    public long calculateSize() {
        return (max - min) + 1;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if ((obj == null) || (getClass() != obj.getClass())) {
            return false;
        }

        Range range = (Range) obj;

        if (max != range.max) {
            return false;
        }
        return min == range.min;

    }

    @Override
    public int hashCode() {
        int result = (int) (max ^ (max >>> 32));
        result = (31 * result) + (int) (min ^ (min >>> 32));
        return result;
    }

    @Override
    public int compareTo(Range o) {
        return Long.valueOf(this.min).compareTo(o.getMin());
    }

    public long getMax() {
        return max;
    }

    public long getMin() {
        return min;
    }

    public boolean in(long l) {
        return (l >= min) && (l <= max);
    }

    public boolean isValid() {
        return (min >= 0) && (max >= 0);
    }

    public void setInvalid() {
        min = -1;
        max = -1;
    }

    public void set(long min, long max) {
        this.min = min;
        this.max = max;
        if (this.min > this.max) {
            throw new IllegalArgumentException("min is greater than max: " + min + " " + max);
        }
    }

    @Override
    public String toString() {
        return String.format("%d : %d", min, max);
    }
}
