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
 * Range of values. Min is inclusive. Max is inclusive.
 */
public class Range implements Comparable<Range> {
    private long _max;
    private long _min;

    public Range() {

    }

    public Range(long min, long max) {
        set(min, max);
    }

    public long calculateSize() {
        return _max - _min + 1;
    }

    @Override
    public int compareTo(Range other) {
        return new Long(_min).compareTo(other._min);
    }

    public long getMax() {
        return _max;
    }

    public long getMin() {
        return _min;
    }

    public boolean in(long l) {
        return l >= _min && l <= _max;
    }

    public boolean isValid() {
        return _min >= 0 && _max >= 0;
    }

    public void setInvalid() {
        _min = -1;
        _max = -1;
    }

    public void set(long min, long max) {
        _min = min;
        _max = max;
        if (_min > _max) {
            throw new IllegalArgumentException("min is greater than max: " + min + " " + max);
        }
    }

    @Override
    public String toString() {
        return String.format("%d : %d", _min, _max);
    }
}
