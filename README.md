# Introduction

[![Join the chat at https://gitter.im/ngageoint/hootenanny](https://badges.gitter.im/ngageoint/hootenanny.svg)](https://gitter.im/ngageoint/hootenanny?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Hootenanny was developed to provide an open source, standards-based approach to geospatial vector 
data conflation. Hootenanny is designed to facilitate automated and semi-automated conflation of 
critical foundation GEOINT features in the topographic domain, namely roads (polylines), buildings 
(polygons), and points-of-interest (POI's) (points). Conflation happens at the dataset level, where
 the user's workflow determines the best reference dataset and source content, geometry and 
attributes, to transfer to the output map.

If you have any support questions please contact Hootenanny.Help@DigitalGlobe.com.

[FAQ](https://github.com/ngageoint/hootenanny/wiki/Frequently-Asked-Questions)

# Installing Hootenanny 

Hootenanny has been tested on both CentOS/RHEL 6.x and Ubuntu 14.04.  Ubuntu receives more 
active development, has more streamlined installation instructions, and is the recommended operating
system to run Hootenanny on. 

To quickly run Hootenanny on Ubuntu, follow the Ubuntu Vagrant section in the 
Hootenanny - Installation Instructions document included with each release.  For additional notes 
on configuring a Hootenanny development environment on Ubuntu, see the Hootenanny - Developer Guide 
document included with each release.

To run Hootenanny on CentOS/RHEL, follow the CentOS/RHEL section in the 
Hootenanny - Installation Instructions document included with each release.  There is currently no 
Vagrant setup available for CentOS/RHEL.  Following the CentOS/RHEL installation instructions is 
currently more complicated than installing Hootenanny on Ubuntu and requires a higher level of 
UNIX expertise.  Setting up a Hootenanny development environment on CentOS/RHEL is neither advised 
nor supported.

Running Hootenanny with Hadoop is optional.  Hootenanny has been tested on Hadoop 0.20.2.  Neither 
the Ubuntu nor CentOS/RHEL installation documents currently include instructions for installing 
Hadoop, but the Developer Guide has instructions for setting up a pseudo-distributed test cluster.

# Using Hootenanny

Refer to the Hootenanny - User Guide and Hootenanny - User Interface Guide included with each
release for details on using the Hootenanny software.  For more in depth information on the 
technology behind Hootenanny, see the Hootenanny - Algorithms document.

# Redistribution

Hootenanny was developed at the National Geospatial-Intelligence Agency (NGA) in collaboration with DigitalGlobe.  The government has "unlimited rights" and is releasing this software to increase the impact of government instruments by providing developers with the opportunity to take things in new directions. The software use, modification, and distribution rights are stipulated within the GNU General Public License. The GPL license is available in LICENSE.txt

All pull requests contributions to this project will be released under the GNU General Public License 3.0. Software source code previously released under an open source license and then modified by NGA staff is considered a "joint work" (see 17 USC 101); it is partially copyrighted, partially public domain, and as a whole is protected by the copyrights of the non-government authors and must be released according to the terms of the original open source license.

Licensed under the GNU General Public License v3.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.gnu.org/copyleft/gpl.html.

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

Imagery provided by permission from DigitalGlobe. Users are responsible for complying with terms of use for data and imagery they use in conjunction with Hootenanny. Specifically, the must properly protect and comply with all legal, copyright, and licensing terms.

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

# Developers

In alphabetical order:

* Jong Choi (Jong.Choi@digitalglobe.com)
* Jason Goffeney (jason.goffeney@digitalglobe.com)
* Brian Hatchl (Brian.Hatchl@digitalglobe.com)
* Matt Jackson (Matthew.Jackson@digitalglobe.com)
* Ben Marchant (Benjamin.Marchant@digitalglobe.com)
* Terry Ott (Terry.Ott@digitalglobe.com)
* Greg Schmidt (Greg.Schmidt@digitalglobe.com)
* Josh Sisskind (Josh.Sisskind@digitalglobe.com)
* Ming Su (Ming.Su@digitalglobe.com)
* Jason R. Surratt (Jason.Surratt@digitalglobe.com)
* Brandon Witham (Brandon.Witham@digitalglobe.com)

