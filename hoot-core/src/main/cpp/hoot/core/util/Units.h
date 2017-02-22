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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef UNITS_H
#define UNITS_H

// Standard
#define _USE_MATH_DEFINES
#include <math.h>

//Boost units
#include <boost/units/systems/si.hpp>
#include <boost/units/base_units/us/foot.hpp>
#include <boost/units/base_units/us/mile.hpp>
#include <boost/units/base_units/metric/knot.hpp>
#include <boost/units/base_units/metric/nautical_mile.hpp>
#include <boost/units/static_constant.hpp>
#include <boost/units/systems/si/time.hpp>

namespace hoot
{
  typedef double Degrees;
  typedef double Meters;
  typedef double Radians;

  //Boost types
  typedef boost::units::quantity<boost::units::si::time, double> Times;
  typedef boost::units::quantity<boost::units::si::length, double> Length;
  typedef boost::units::quantity<boost::units::si::velocity, double> Velocity;

  typedef boost::units::us::foot_base_unit::unit_type foot_unit;
  BOOST_UNITS_STATIC_CONSTANT(feet, foot_unit);
  typedef boost::units::us::mile_base_unit::unit_type mile_unit;
  BOOST_UNITS_STATIC_CONSTANT(mile, mile_unit);
  typedef boost::units::metric::nautical_mile_base_unit::unit_type nmi_unit;
  BOOST_UNITS_STATIC_CONSTANT(nmi, nmi_unit);
  typedef boost::units::metric::knot_base_unit::unit_type knot_unit;
  BOOST_UNITS_STATIC_CONSTANT(knot, knot_unit);

  inline Radians toRadians(Degrees d) { return d / 180.0 * M_PI; }

  inline Degrees toDegrees(Radians r) { return r / M_PI * 180.0; }

  //Boost units functions
  inline Length getKiloLength()
  {
    return 1000.0 * boost::units::si::meters;
  }

  inline Length getDecimiLength()
  {
    return 0.1 * boost::units::si::meters;
  }

  inline Length getMileLength()
  {
    return static_cast<Length>(1.0 * mile);
  }

  inline Length getNmiLength()
  {
    return static_cast<Length>(1.0 * nmi);
  }

  inline Length getFeetLength()
  {
   return static_cast<Length>(1.0 * feet);
  }

  inline Velocity getKph()
  {
    Times ts = 3600.0 * boost::units::si::seconds;
    return getKiloLength()/ts;
  }

  inline Velocity getMph()
  {
    Times ts = 3600.0 * boost::units::si::seconds;
    return getMileLength()/ts;
  }

  inline Velocity getKnotph()
  {
    return static_cast<Velocity>(1.0 * knot);
  }
}

#endif // UNITS_H
