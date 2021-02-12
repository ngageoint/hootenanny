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
 * @copyright Copyright (C) 2015, 2017, 2020 Maxar (http://www.maxar.com/)
 */

// Standard
#include <cstring>
#include <iostream>
#include <stdint.h>

#define ones(T,k) ((((T)2) << (k-1)) - 1)

#define rdbit(w,k) (((w) >> (k)) & 1)

#define rotateRight(arg, nRots, nDims)                                  \
((((arg) >> (nRots)) | ((arg) << ((nDims)-(nRots)))) & ones(bitmask_t,nDims))

#define adjust_rotation(rotation,nDims,bits)                            \
do {                                                                    \
      /* rotation = (rotation + 1 + ffs(bits)) % nDims; */              \
      bits &= -bits & nd1Ones;                                          \
      while (bits)                                                      \
        bits >>= 1, ++rotation;                                         \
      if ( ++rotation >= nDims )                                        \
        rotation -= nDims;                                              \
} while (0)


namespace Tgs
{

  class HilbertCurve
  {
  public:

    typedef int64_t bitmask_t;
    typedef int64_t halfmask_t;

    HilbertCurve(int dimensions, int order)
    {
      _dimensions = dimensions;
      _order = order;

      if (_dimensions * _order > (int)(sizeof(bitmask_t) * 8 - 1))
      {
        std::cerr << "dimension * order must be <= " << (sizeof(bitmask_t) * 8 - 1) << "." <<
          std::endl;
        std::cerr << "got dimension: " << dimensions << " order: " << order << std::endl;
      }
      if (_order < 1 || _dimensions < 1)
      {
        std::cerr << "dimension and order must be >= 1" << std::endl;
        std::cerr << "got dimension: " << dimensions << " order: " << order << std::endl;
      }

      _point = new bitmask_t[_dimensions];
    }

    HilbertCurve(const HilbertCurve& curve)
    {
      _dimensions = curve._dimensions;
      _order = curve._order;
      _point = new bitmask_t[_dimensions];
      std::memcpy(_point, curve._point, sizeof(bitmask_t) * _dimensions);
    }

    ~HilbertCurve()
    {
      delete [] _point;
    }

    long encode(int point[])
    {
      long int result;

      // swap the ordering for historical reasons.
      for (int i = 0; i < _dimensions; i++)
      {
        _point[i] = point[_dimensions - 1 - i];
      }

      result = hilbert_c2inew(_dimensions, _order, _point);
      //hilbert_c2i(_dimensions, _order, point, &result);
      return result;
    }

  private:

    int _dimensions;
    int _order;
    bitmask_t* _point;

    bitmask_t bitTranspose(unsigned nDims, unsigned nBits, bitmask_t inCoords)
    {
      unsigned const nDims1 = nDims-1;
      unsigned inB = nBits;
      unsigned utB;
      bitmask_t inFieldEnds = 1;
      bitmask_t inMask = ones(bitmask_t,inB);
      bitmask_t coords = 0;

      while ((utB = inB / 2))
        {
          unsigned const shiftAmt = nDims1 * utB;
          bitmask_t const utFieldEnds =
      inFieldEnds | (inFieldEnds << (shiftAmt+utB));
          bitmask_t const utMask =
      (utFieldEnds << utB) - utFieldEnds;
          bitmask_t utCoords = 0;
          unsigned d;
          if (inB & 1)
      {
        bitmask_t const inFieldStarts = inFieldEnds << (inB-1);
        unsigned oddShift = 2*shiftAmt;
        for (d = 0; d < nDims; ++d)
          {
            bitmask_t in = inCoords & inMask;
            inCoords >>= inB;
            coords |= (in & inFieldStarts) <<	oddShift++;
            in &= ~inFieldStarts;
            in = (in | (in << shiftAmt)) & utMask;
            utCoords |= in << (d*utB);
          }
      }
          else
      {
        for (d = 0; d < nDims; ++d)
          {
            bitmask_t in = inCoords & inMask;
            inCoords >>= inB;
            in = (in | (in << shiftAmt)) & utMask;
            utCoords |= in << (d*utB);
          }
      }
          inCoords = utCoords;
          inB = utB;
          inFieldEnds = utFieldEnds;
          inMask = utMask;
        }
      coords |= inCoords;
      return coords;
    }

    /*****************************************************************
     * hilbert_c2i
     *
     * Convert coordinates of a point on a Hilbert curve to its index.
     * Inputs:
     *  nDims:      Number of coordinates.
     *  nBits:      Number of bits/coordinate.
     *  coord:      Array of n nBits-bit coordinates.
     * Outputs:
     *  index:      Output index value.  nDims*nBits bits.
     * Assumptions:
     *      nDims*nBits <= (sizeof bitmask_t) * (bits_per_byte)
     */
    bitmask_t
    hilbert_c2inew(unsigned nDims, unsigned nBits, bitmask_t const coord[])
    {
      if (nDims > 1)
        {
          unsigned const nDimsBits = nDims*nBits;
          bitmask_t index;
          unsigned d;
          bitmask_t coords = 0;
          for (d = nDims; d--; )
      {
        coords <<= nBits;
        coords |= coord[d];
      }

          if (nBits > 1)
      {
        halfmask_t const ndOnes = ones(halfmask_t,nDims);
        halfmask_t const nd1Ones= ndOnes >> 1; /* for adjust_rotation */
        unsigned b = nDimsBits;
        unsigned rotation = 0;
        halfmask_t flipBit = 0;
        bitmask_t const nthbits = ones(bitmask_t,nDimsBits) / ndOnes;
        coords = bitTranspose(nDims, nBits, coords);
        coords ^= coords >> nDims;
        index = 0;
        do
          {
            halfmask_t bits = (coords >> (b-=nDims)) & ndOnes;
            bits = rotateRight(flipBit ^ bits, rotation, nDims);
            index <<= nDims;
            index |= bits;
            flipBit = (halfmask_t)1 << rotation;
            adjust_rotation(rotation,nDims,bits);
          } while (b);
        index ^= nthbits >> 1;
      }
          else
      index = coords;
          for (d = 1; d < nDimsBits; d *= 2)
      index ^= index >> d;
          return index;
        }
      else
        return coord[0];
    }
  };
}

