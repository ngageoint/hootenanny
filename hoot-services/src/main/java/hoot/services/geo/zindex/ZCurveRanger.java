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
import java.util.LinkedList;
import java.util.List;
import java.util.PriorityQueue;


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
    private static final class LongBoxContainer implements Comparable<LongBoxContainer> {
        private final LongBox box;
        private final long excess;

        LongBoxContainer(LongBox box, long excess) {
            this.box = box;
            this.excess = excess;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if ((obj == null) || (getClass() != obj.getClass())) {
                return false;
            }

            LongBoxContainer that = (LongBoxContainer) obj;

            if (excess != that.excess) {
                return false;
            }
            return (box != null) ? box.equals(that.box) : (that.box == null);

        }

        @Override
        public int hashCode() {
            int result = (box != null) ? box.hashCode() : 0;
            result = (31 * result) + (int) (excess ^ (excess >>> 32));
            return result;
        }

        @Override
        public int compareTo(LongBoxContainer o) {
            if (getExcess() > o.getExcess()) {
                return -1;
            }
            else if (getExcess() < o.getExcess()) {
                return 1;
            }
            else {
                return 0;
            }
        }

        LongBox getBox() {
            return box;
        }

        long getExcess() {
            return excess;
        }

        boolean isPerfectFit() {
            return getExcess() == 0;
        }

        @Override
        public String toString() {
            return String.format("%d: %s", excess, box.toString());
        }
    }

    // the amount of slop to accept when merging ranges.
    private long slop;
    private final ZValue zv;

    public ZCurveRanger(ZValue zv) {
        this.zv = zv;
        // completely arbitrary.
        slop = 5;
    }

    /**
     * Find a good break point for the given box based on major z-value breaks
     * and break the box into two children.
     */
    LongBox[] breakBox(LongBox box) {
        int bestMaxBit = 0;
        int bestD = 0;

        // go through all the dimensions and find the one that has the most
        // significant binary breaks from min to max.
        for (int d = 0; d < zv.getDimensions(); d++) {
            long v1 = box.getMin()[d];
            long v2 = box.getMax()[d];

            int maxBit = getMaxBitColumn(v1 ^ v2);
            // the later dimensions have a more significant bits so we'll split
            // on >=
            if ((maxBit >= bestMaxBit) && (v1 < v2)) {
                bestMaxBit = maxBit;
                bestD = d;
            }
        }

        long splitPoint = getSplitValue(box.getMin()[bestD], box.getMax()[bestD]);

        // if there aren't any good splits
        LongBox[] result;
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

    private long calculateExcess(LongBox box) {
        long startSize = toRange(box).calculateSize();
        return startSize - box.calculateVolume();
    }

    /**
     * Recursively decompose a box over level iterations.
     */
    List<LongBox> decomposeBox(LongBox box, int level) {
        List<LongBox> result = new LinkedList<>();

        // break the input box on major z-curve boundaries
        LongBox[] children = breakBox(box);

        // if there is only one child we're done.
        if (children.length == 1) {
            result.add(children[0]);
        }
        // if we've decomposed enough (eh' stinky!)
        else if (level <= 0) {
            Collections.addAll(result, children);
        }
        // if we've still got some decomposition to do
        else {
            for (LongBox aChildren : children) {
                result.addAll(decomposeBox(aChildren, level - 1));
            }
        }

        return result;
    }

    static int getMaxBitColumn(long v) {
        int column = -1;
        while ((v != 0) && (column < 100)) {
            column++;
            v >>= 1;
        }

        if (column >= 100) {
            throw new RuntimeException("MaxBitColumn >= 100!");
        }

        return column;
    }

    static long getSplitValue(long v1, long v2) {
        if ((v2 - v1) < 1) {
            return -1;
        }

        long d = v1 ^ v2;

        int maxBit = getMaxBitColumn(d);

        long mask = 0;
        for (int i = 0; i < maxBit; i++) {
            mask = (mask << 1) | 1;
        }
        mask = ~mask;

        return v2 & mask;
    }

    /**
     * Condense ranges that overlap or are within "slop" cells of each other.
     */
    private List<Range> condenseRanges(List<Range> r) {
        Collections.sort(r);

        List<Range> result = new LinkedList<>();
        result.add(r.get(0));
        for (int i = 1; i < r.size(); i++) {
            if ((r.get(i).getMin() - result.get(result.size() - 1).getMax()) <= slop) {
                result.get(result.size() - 1).set(result.get(result.size() - 1).getMin(), r.get(i).getMax());
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
    public List<Range> decomposeRange(Box box, Box focusBox, int levels) {
        return decomposeRange(toLongBox(box), toLongBox(focusBox), levels);
    }

    /**
     * Decomposes the z value ranges that cover box into an array of ranges.
     * 
     * @param box
     *            - Must be a valid box within the ZValue.
     */
    public List<Range> decomposeRange(Box box, int levels) {
        return decomposeRange(toLongBox(box), levels);
    }

    private LongBox clipBox(LongBox box) {
        LongBox result = new LongBox(box);

        for (int i = 0; i < result.getDimensions(); i++) {
            result.getMin()[i] = Math.max(0, result.getMin()[i]);
            result.getMax()[i] = Math.min(zv.getMaxDimensionRange(), result.getMax()[i]);
        }

        return result;
    }

    public List<Range> decomposeRange(LongBox box, int levels) {
        List<Range> result = new LinkedList<>();
        List<LongBox> boxes = decomposeBox(clipBox(box), levels);

        for (LongBox boxe : boxes) {
            result.add(toRange(boxe));
        }

        return condenseRanges(result);
    }

    private List<Range> decomposeRange(LongBox box, LongBox focusBox, int levels) {
        List<LongBox> boxes = decomposeBox(box, levels / 2);

        for (int j = 0; j < (levels * 2); j++) {
            List<LongBox> newBoxes = new LinkedList<>();

            for (LongBox b : boxes) {
                if (b.intersects(focusBox) && (calculateExcess(b) > 0)) {
                    newBoxes.addAll(decomposeBox(b, 0));
                }
                else {
                    newBoxes.add(b);
                }
            }

            boxes = newBoxes;
        }

        List<Range> result = new LinkedList<>();
        for (LongBox boxe : boxes) {
            result.add(toRange(boxe));
        }

        return condenseRanges(result);
    }

    public List<Range> decomposeRangeIterative(Box box, int count) {
        return decomposeRangeIterative(toLongBox(box), count);
    }

    private List<Range> decomposeRangeIterative(LongBox box, int count) {

        PriorityQueue<LongBoxContainer> pq = new PriorityQueue<>();
        pq.add(new LongBoxContainer(box, calculateExcess(box)));

        List<LongBox> completed = new LinkedList<>();
        while ((!pq.isEmpty()) && ((pq.size() + completed.size()) < count)) {
            LongBoxContainer lbc = pq.remove();

            if (lbc.isPerfectFit()) {
                completed.add(lbc.getBox());
            }
            else {
                List<LongBox> boxes = decomposeBox(lbc.getBox(), 0);

                if (boxes.size() == 1) {
                    completed.add(boxes.get(0));
                }
                else if (boxes.size() == 2) {
                    pq.add(new LongBoxContainer(boxes.get(0), calculateExcess(boxes.get(0))));
                    pq.add(new LongBoxContainer(boxes.get(1), calculateExcess(boxes.get(1))));
                }
                else {
                    throw new RuntimeException("Invalid boxes.size = " + boxes.size());
                }
            }
        }

        while (!pq.isEmpty()) {
            completed.add(pq.remove().getBox());
        }

        List<Range> result = new LinkedList<>();
        for (LongBox aCompleted : completed) {
            result.add(toRange(aCompleted));
        }

        return condenseRanges(result);
    }

    boolean rangeCoversIdentity(Range r) {
        long[] min = new long[zv.getDimensions()];
        long[] max = new long[zv.getDimensions()];
        zv.decompose(r.getMin(), min);
        zv.decompose(r.getMax(), max);

        long int1 = min[0];
        long int2 = max[0];

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
        this.slop = slop;
    }

    private LongBox toLongBox(Box box) {
        long[] min = new long[box.getDimensions()];
        long[] max = new long[box.getDimensions()];

        for (int i = 0; i < box.getDimensions(); i++) {
            min[i] = zv.calculateComponent(box.getMin()[i], i);
            max[i] = zv.calculateComponent(box.getMax()[i], i);
        }

        return new LongBox(min, max);
    }

    private Range toRange(LongBox box) {
        long[] scratch = new long[zv.getDimensions()];

        Range result = new Range();
        System.arraycopy(box.getMin(), 0, scratch, 0, zv.getDimensions());

        long min = zv.calculate(scratch);
        System.arraycopy(box.getMax(), 0, scratch, 0, zv.getDimensions());

        result.set(min, zv.calculate(scratch));

        return result;
    }
}
