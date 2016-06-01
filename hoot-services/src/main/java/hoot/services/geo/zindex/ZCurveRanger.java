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

import java.util.Collections;
import java.util.PriorityQueue;
import java.util.Vector;


/**
 * Decomposes a bounding box into a range of zcurve values.
 * 
 * Yes, ZCurve Ranger. Picture a guy wearing a space helmet in blue spandex
 * covered in Zs.
 * 
 * //TODO: Prototype -- Requires things like basic error checking.
 */
public class ZCurveRanger {
    /**
     * Used when experimenting with various range decomposition approaches.
     */
    private final static class LongBoxContainer implements Comparable<LongBoxContainer> {
        private LongBox _box;
        private long _excess;

        LongBoxContainer(LongBox box, long excess) {
            _box = box;
            _excess = excess;
        }

        @Override
        public int compareTo(LongBoxContainer other) {
            if (getExcess() > other.getExcess()) {
                return -1;
            }
            else if (getExcess() < other.getExcess()) {
                return 1;
            }
            else {
                return 0;
            }
        }

        LongBox getBox() {
            return _box;
        }

        long getExcess() {
            return _excess;
        }

        boolean isPerfectFit() {
            return getExcess() == 0;
        }

        @Override
        public String toString() {
            return String.format("%d: %s", _excess, _box.toString());
        }
    }

    // the amount of slop to accept when merging ranges.
    private long _slop;

    private ZValue _zv;

    public ZCurveRanger(ZValue zv) {
        _zv = zv;
        // completely arbitrary.
        _slop = 5;
    }

    /**
     * Find a good break point for the given box based on major z-value breaks
     * and break the box into two children.
     */
    LongBox[] _breakBox(LongBox box) {
        LongBox[] result;

        int bestMaxBit = 0;
        int bestD = 0;

        // go through all the dimensions and find the one that has the most
        // significant binary
        // breaks from min to max.
        for (int d = 0; d < _zv.getDimensions(); d++) {
            long v1 = box.getMin()[d];
            long v2 = box.getMax()[d];

            assert (v1 >= 0) : "bad lower bound";
            assert (v1 <= v2) : "lower and upper bound are not consistent";

            int maxBit = _getMaxBitColumn(v1 ^ v2);
            // the later dimensions have a more significant bits so we'll split
            // on >=
            if (maxBit >= bestMaxBit && v1 < v2) {
                bestMaxBit = maxBit;
                bestD = d;
            }
        }

        long splitPoint = _getSplitValue(box.getMin()[bestD], box.getMax()[bestD]);

        // if there aren't any good splits
        if (splitPoint == -1) {
            result = new LongBox[] { box };
        }
        else {
            result = new LongBox[2];
            result[0] = new LongBox(box);
            result[0].getMax()[bestD] = splitPoint - 1;
            result[1] = new LongBox(box);
            result[1].getMin()[bestD] = splitPoint;
        }

        return result;
    }

    private long _calculateExcess(LongBox box) {
        long startSize = toRange(box).calculateSize();

        return startSize - box.calculateVolume();
    }

    /**
     * Recursively decompose a box over level iterations.
     */
    Vector<LongBox> _decomposeBox(LongBox box, int level) {
        Vector<LongBox> result = new Vector<LongBox>();

        // break the input box on major z-curve boundaries
        LongBox[] children = _breakBox(box);

        // if there is only one child we're done.
        if (children.length == 1) {
            result.add(children[0]);
        }
        // if we've decomposed enough (eh' stinky!)
        else if (level <= 0) {
            for (int i = 0; i < children.length; i++) {
                result.add(children[i]);
            }
        }
        // if we've still got some decomposition to do
        else {
            for (int i = 0; i < children.length; i++) {
                result.addAll(_decomposeBox(children[i], level - 1));
            }
        }

        return result;
    }

    int _getMaxBitColumn(long v) {
        int column = -1;
        assert (v >= 0);

        while (v != 0 && column < 100) {
            column++;
            v = v >> 1;
        }

        if (column >= 100) {
            throw new Error();
        }

        return column;
    }

    long _getSplitValue(long v1, long v2) {
        if (v2 - v1 < 1) {
            return -1;
        }

        long d = v1 ^ v2;

        int maxBit = _getMaxBitColumn(d);

        long mask = 0;
        for (int i = 0; i < maxBit; i++) {
            mask = (mask << 1) | 1;
        }
        mask = ~mask;

        return v2 & mask;
    }

    /**
     * Condense ranges that overlap or are within "_slop" cells of each other.
     */
    public Vector<Range> condenseRanges(Vector<Range> r) {
        Vector<Range> result = new Vector<Range>();
        Collections.sort(r);

        result.add(r.get(0));
        for (int i = 1; i < r.size(); i++) {
            if (r.get(i).getMin() - result.lastElement().getMax() <= _slop) {
                result.lastElement().set(result.lastElement().getMin(), r.get(i).getMax());
            }
            else {
                result.add(r.get(i));
            }
        }

        return result;
    }

    /**
     * Focuses the decomposition to the ares that intersect the focus box. The
     * shows some promise with the dual space index.
     */
    public Vector<Range> decomposeRange(Box box, Box focusBox, int levels) {
        return decomposeRange(toLongBox(box), toLongBox(focusBox), levels);
    }

    /**
     * Decomposes the z value ranges that cover box into an array of ranges.
     * 
     * @param box
     *            - Must be a valid box within the ZValue.
     */
    public Vector<Range> decomposeRange(Box box, int levels) {
        return decomposeRange(toLongBox(box), levels);
    }

    public LongBox clipBox(LongBox box) {
        LongBox result = new LongBox(box);

        for (int i = 0; i < result.getDimensions(); i++) {
            result._min[i] = Math.max(0, result._min[i]);
            result._max[i] = Math.min(_zv.getMaxDimensionRange(), result._max[i]);
        }

        return result;
    }

    public Vector<Range> decomposeRange(LongBox box, int levels) {
        Vector<Range> result = new Vector<Range>();
        Vector<LongBox> boxes = _decomposeBox(clipBox(box), levels);

        for (int i = 0; i < boxes.size(); i++) {
            result.add(toRange(boxes.elementAt(i)));
        }

        return condenseRanges(result);
    }

    public Vector<Range> decomposeRange(LongBox box, LongBox focusBox, int levels) {
        Vector<Range> result = new Vector<Range>();

        Vector<LongBox> boxes = _decomposeBox(box, levels / 2);

        for (int j = 0; j < levels * 2; j++) {
            Vector<LongBox> newBoxes = new Vector<LongBox>();

            for (int i = 0; i < boxes.size(); i++) {
                LongBox b = boxes.get(i);
                if (b.intersects(focusBox) && _calculateExcess(b) > 0)
                // if (box.edgeOverlaps(b))
                // if (rangeCoversIdentity(_toRange(b)))
                {
                    newBoxes.addAll(_decomposeBox(b, 0));
                    // System.out.printf("j: %d\n", j);
                }
                else {
                    // System.out.println(b);
                    newBoxes.add(b);
                }
            }

            boxes = newBoxes;
        }

        for (int i = 0; i < boxes.size(); i++) {
            result.add(toRange(boxes.elementAt(i)));
        }

        return condenseRanges(result);
    }

    public Vector<Range> decomposeRangeIterative(Box box, int count) {
        return decomposeRangeIterative(toLongBox(box), count);
    }

    public Vector<Range> decomposeRangeIterative(LongBox box, int count) {
        Vector<Range> result = new Vector<Range>();
        Vector<LongBox> completed = new Vector<LongBox>();

        PriorityQueue<LongBoxContainer> pq = new PriorityQueue<LongBoxContainer>();
        pq.add(new LongBoxContainer(box, _calculateExcess(box)));

        while (pq.isEmpty() == false && (pq.size() + completed.size()) < count) {
            LongBoxContainer lbc = pq.remove();
            // System.out.println(lbc.toString());

            if (lbc.isPerfectFit() == true /*
                                            * ||
                                            * lbc.getBox().intersects(focusBox)
                                            * == false
                                            */) {
                completed.add(lbc.getBox());
            }
            else {
                Vector<LongBox> boxes = _decomposeBox(lbc.getBox(), 0);

                if (boxes.size() == 1) {
                    completed.add(boxes.get(0));
                }
                else if (boxes.size() == 2) {
                    pq.add(new LongBoxContainer(boxes.get(0), _calculateExcess(boxes.get(0))));
                    pq.add(new LongBoxContainer(boxes.get(1), _calculateExcess(boxes.get(1))));
                }
                else {
                    throw new Error();
                }
            }
        }

        while (pq.isEmpty() == false) {
            completed.add(pq.remove().getBox());
        }

        // System.out.printf("completed: %d\n", completed.size());

        for (int i = 0; i < completed.size(); i++) {
            result.add(toRange(completed.get(i)));
        }

        return condenseRanges(result);
    }

    boolean rangeCoversIdentity(Range r) {
        long[] min = new long[_zv.getDimensions()];
        long[] max = new long[_zv.getDimensions()];
        _zv.decompose(r.getMin(), min);
        _zv.decompose(r.getMax(), max);

        long int1, int2;
        int1 = min[0];
        int2 = max[0];

        for (int i = 1; i < min.length; i++) {
            int1 = Math.max(int1, min[i]);
            int2 = Math.min(int2, max[i]);
        }

        return int2 >= int1;
    }

    /**
     * The number of cells that can be acceptably merged into one range. This
     * should never be less than one. Other than that it is a tunable parameter.
     * Denser data sets probably want a smaller value. Less dense data sets
     * should have a higher value.
     * 
     * @param slop
     */
    public void setSlop(int slop) {
        _slop = slop;
    }

    public LongBox toLongBox(Box box) {
        long[] min = new long[box.getDimensions()];
        long[] max = new long[box.getDimensions()];

        for (int i = 0; i < box.getDimensions(); i++) {
            min[i] = _zv.calculateComponent(box.getMin()[i], i);
            max[i] = _zv.calculateComponent(box.getMax()[i], i);
        }

        return new LongBox(min, max);
    }

    public Range toRange(LongBox box) {
        long[] scratch = new long[_zv.getDimensions()];
        Range result = new Range();
        for (int i = 0; i < _zv.getDimensions(); i++) {
            scratch[i] = box.getMin()[i];
        }
        long min = _zv.calculate(scratch);

        for (int i = 0; i < _zv.getDimensions(); i++) {
            scratch[i] = box.getMax()[i];
        }
        try {
            result.set(min, _zv.calculate(scratch));
        }
        catch (IllegalArgumentException e) {
            // System.out.println(box.toString());
            e.printStackTrace();
            throw e;
        }

        return result;
    }
}
