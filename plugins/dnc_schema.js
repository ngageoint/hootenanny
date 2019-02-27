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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
var _global = (0,eval)('this');
if (!_global.dnc)
{
  _global.dnc = {};
}

dnc.schema = {
getDbSchema: function()
{
    var schema = [
        { name:"BUILDNGA",
          fcode:"AAL015",
          desc:"Building Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"BFC",
                       desc:"Building Function Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown", value:"0" },
                           { name:"Fabrication Structures", value:"1" },
                           { name:"Government Building", value:"2" },
                           { name:"Capitol Building", value:"3" },
                           { name:"Castle", value:"4" },
                           { name:"Government Administration Building", value:"5" },
                           { name:"Hospital", value:"6" },
                           { name:"House of Worship", value:"7" },
                           { name:"Military Administration/Operations Building", value:"8" },
                           { name:"Museum", value:"9" },
                           { name:"Observatory", value:"10" },
                           { name:"Palace", value:"11" },
                           { name:"Police Station", value:"12" },
                           { name:"Prison", value:"13" },
                           { name:"Ranger Station", value:"14" },
                           { name:"School", value:"15" },
                           { name:"House", value:"16" },
                           { name:"Multi-Unit Dwelling", value:"17" },
                           { name:"Cemetery Building", value:"18" },
                           { name:"Farm Building", value:"19" },
                           { name:"Greenhouse", value:"20" },
                           { name:"Garage", value:"21" },
                           { name:"Watermill/Gristmill", value:"22" },
                           { name:"Wind Tunnel", value:"23" },
                           { name:"Warehouse", value:"24" },
                           { name:"Roundhouse", value:"25" },
                           { name:"R/R Storage/Repair Facility", value:"26" },
                           { name:"Depot Terminal", value:"27" },
                           { name:"Administration Building", value:"28" },
                           { name:"Aircraft Maintenance Shop", value:"29" },
                           { name:"Hangar", value:"30" },
                           { name:"Custom House", value:"31" },
                           { name:"Health Office", value:"33" },
                           { name:"Post Office", value:"35" },
                           { name:"Barracks/Dormitory", value:"36" },
                           { name:"Fire Station", value:"37" },
                           { name:"Bank", value:"53" },
                           { name:"R&D Lab/Research Facility", value:"59" },
                           { name:"Courthouse", value:"61" },
                           { name:"Harbor Master’s Office", value:"77" },
                           { name:"Power Generation", value:"83" },
                           { name:"Newspaper Plant", value:"85" },
                           { name:"Telephone Exchange (Main)", value:"86" },
                           { name:"Auditorium", value:"87" },
                           { name:"Opera House", value:"88" },
                           { name:"Processing/Treatment", value:"89" },
                           { name:"Pumphouse", value:"90" },
                           { name:"Hotel", value:"95" },
                           { name:"Diplomatic Building", value:"96" },
                           { name:"Other", value:"999" }
                        ]
                     },
                     { name:"COL",
                       desc:"Character of Light",
                       optional:"R",
                       length:"10",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"HWT",
                       desc:"House of Worship Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"22",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Cathedral",value:"2" },
                           { name:"Chapel",value:"3" },
                           { name:"Church",value:"4" },
                           { name:"Minaret",value:"6" },
                           { name:"Monastry,Convent",value:"7" },
                           { name:"Mosque",value:"9" },
                           { name:"Pagoda",value:"11" },
                           { name:"Shrine",value:"14" },
                           { name:"Tabernacle",value:"15" },
                           { name:"Temple",value:"16" },
                           { name:"Synagogue",value:"20" },
                           { name:"Stupa",value:"21" },
                           { name:"Not Applicable",value:"22" }
                        ]
                     },
                    { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     }
                    ]
          },
        { name:"INDUSTA",
          fcode:"",
          desc:"Industry Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"PRO",
                       desc:"Product",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Chemical",value:"13" },
                           { name:"Coal",value:"17" },
                           { name:"Coke",value:"19" },
                           { name:"Gas",value:"38" },
                           { name:"Gasoline",value:"39" },
                           { name:"Gravel",value:"46" },
                           { name:"Oll",value:"67" },
                           { name:"Salt",value:"87" },
                           { name:"Sand",value:"88" },
                           { name:"Stone",value:"108" },
                           { name:"Water",value:"116" },
                           { name:"Other",value:"999" },
                        ]
                     },
                     { name:"ZV2",
                       desc:"Highest Z-Value",
                       optional:"R",
                       units:"metre",
                       type:"Integer",
                       defValue:"99999"
                     }
                    ]
          },
        { name:"LANDMRKA",
          fcode:"",
          desc:"Landmark Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     }
                    ]
          },
        { name:"PARKA",
          fcode:"",
          desc:"Landmark Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     }
                    ]
          },
        { name:"TRANSA",
          fcode:"",
          desc:"Transportation Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"APT",
                       desc:"Airfield Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Major",value:"1" },
                           { name:"Minor",value:"2" },
                           { name:"Seaplane Base",value:"4" },
                           { name:"Heliport",value:"9" },
                        ]
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"28",
                       enumerations:[
                           { name:"Under Construction",value:"5" },
                           { name:"Abandoned/Disused",value:"6" },
                           { name:"Operational",value:"28" }
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     }
                    ]
          },
        { name:"FENCEL",
          fcode:"",
          desc:"Fence  Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     }
                    ]
          },
        { name:"PARKL",
          fcode:"",
          desc:"Park Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"SSC",
                       desc:"Structure Shape Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Roller Coaster",value:"25" }
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     }
                    ]
          },
        { name:"POWERL",
          fcode:"",
          desc:"Power Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"OHC",
                       desc:"Overhead Clearance Category",
                       optional:"R",
                       type:"Real",
                       defValue:"0.0",
                     },
                     { name:"OWO",
                       desc:"Over Water Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Feature crosses navigable water",value:"1" },
                        ]
                     }
                    ]
          },
        { name:"RAILRDL",
          fcode:"",
          desc:"Railroad Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"28",
                       enumerations:[
                           { name:"Under Construction",value:"5" },
                           { name:"Abandoned/Disused",value:"6" },
                           { name:"Destroyed",value:"7" },
                           { name:"Dismantled",value:"8" },
                           { name:"Operational",value:"28" }
                        ]
                     },
                     { name:"LOC",
                       desc:"Location Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"8",
                       enumerations:[
                           { name:"Below Surface / Submerged / Underground",value:"4" },
                           { name:"On ground surface",value:"8" },
                           { name:"Suspended / Elevated Above Ground or Water",value:"25" },
                        ]
                     },
                     { name:"RRC",
                       desc:"Railroad Categories",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Carline", value:"2" },
                           { name:"Monorail", value:"3" },
                           { name:"Logging", value:"8" },
                           { name:"Marine Railroad", value:"13" },
                           { name:"Tramway", value:"14" },
                           { name:"Inclined Railway", value:"15" },
                           { name:"Main Line", value:"16" },
                           { name:"Branch Line", value:"17" },
                           { name:"Railway in Road", value:"21" }
                        ]
                     },
                     { name:"VRR",
                       desc:"Vertical Reference Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Above surface / Does Not Cover (At High Water)",value:"1" },
                           { name:"Below Surface / Submerged",value:"4" },
                           { name:"Covers and Uncovers",value:"8" },
                           { name:"Not Applicable",value:"9" }
                        ]
                     }
                    ]
          },
        { name:"TRANSL",
          fcode:"",
          desc:"Transportation Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"28",
                       enumerations:[
                           { name:"Operational",value:"28" }
                        ]
                     }
                    ]
        },
        { name:"AEROP",
          fcode:"",
          desc:"Aeronautical Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ]
                     },
                     { name:"APT",
                       desc:"Airfield Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Major Airfield", value:"1" },
                           { name:"Minor Airfield", value:"2" }
                        ]
                     },
                     { name:"BRF",
                       desc:"Broadcast Frequency",
                       optional:"R",
                       units:"Hertz",
                       type:"Integer",
                       defValue:"0"
                     },
                     { name:"COL",
                       desc:"Character of Light",
                       optional:"R",
                       length:"10",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"28",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Operational",value:"28" },
                           { name:"Continuous Operation",value:"42" }
                        ]
                     },
                     { name:"MCA",
                       desc:"Morse Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"NST",
                       desc:"Navigation System Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"NOB (Non-directional Beacon)",value:"17" },
                           { name:"RNG (Radio Range)",value:"19" }
                        ]
                     },
                     { name:"OLQ",
                       desc:"Obstruction Light Quality",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"One light present",value:"1" },
                           { name:"Multiple lights present",value:"2" }
                        ]
                     },
                     { name:"ORC",
                       desc:"Operating Range Category",
                       optional:"R",
                       units:"nautical miles",
                       type:"Integer",
                       defValue:"0"
                     },
                    ]
          },
        { name:"BUILDNGP",
          fcode:"",
          desc:"Building Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ]
                     },
                     { name:"BFC",
                       desc:"Building Function Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown", value:"0" },
                           { name:"Fabrication Structures", value:"1" },
                           { name:"Government Building", value:"2" },
                           { name:"Capitol Building", value:"3" },
                           { name:"Castle", value:"4" },
                           { name:"Government Administration Building", value:"5" },
                           { name:"Hospital", value:"6" },
                           { name:"House of Worship", value:"7" },
                           { name:"Military Administration/Operations Building", value:"8" },
                           { name:"Museum", value:"9" },
                           { name:"Observatory", value:"10" },
                           { name:"Palace", value:"11" },
                           { name:"Police Station", value:"12" },
                           { name:"Prison", value:"13" },
                           { name:"Ranger Station", value:"14" },
                           { name:"School", value:"15" },
                           { name:"House", value:"16" },
                           { name:"Multi-Unit Dwelling", value:"17" },
                           { name:"Cemetery Building", value:"18" },
                           { name:"Farm Building", value:"19" },
                           { name:"Greenhouse", value:"20" },
                           { name:"Garage", value:"21" },
                           { name:"Watermill/Gristmill", value:"22" },
                           { name:"Wind Tunnel", value:"23" },
                           { name:"Warehouse", value:"24" },
                           { name:"Roundhouse", value:"25" },
                           { name:"R/R Storage/Repair Facility", value:"26" },
                           { name:"Depot Terminal", value:"27" },
                           { name:"Administration Building", value:"28" },
                           { name:"Aircraft Maintenance Shop", value:"29" },
                           { name:"Hangar", value:"30" },
                           { name:"Custom House", value:"31" },
                           { name:"Health Office", value:"33" },
                           { name:"Post Office", value:"35" },
                           { name:"Barracks/Dormitory", value:"36" },
                           { name:"Fire Station", value:"37" },
                           { name:"Bank", value:"53" },
                           { name:"R&D Lab/Research Facility", value:"59" },
                           { name:"Courthouse", value:"61" },
                           { name:"Harbor Master’s Office", value:"77" },
                           { name:"Maritime Station", value:"81" },
                           { name:"Lighthouse", value:"82" },
                           { name:"Power Generation", value:"83" },
                           { name:"Newspaper Plant", value:"85" },
                           { name:"Telephone Exchange (Main)", value:"86" },
                           { name:"Auditorium", value:"87" },
                           { name:"Opera House", value:"88" },
                           { name:"Processing/Treatment", value:"89" },
                           { name:"Pumphouse", value:"90" },
                           { name:"Hotel", value:"95" },
                           { name:"Diplomatic Building", value:"96" },
                           { name:"Other", value:"999" }
                        ]
                     },
                     { name:"COL",
                       desc:"Character of Light",
                       optional:"R",
                       length:"10",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Under Construction",value:"5" },
                           { name:"Abandoned / Disused",value:"6" },
                           { name:"Destroyed",value:"7" },
                        ]
                     },
                     { name:"HWT",
                       desc:"House of Worship Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"22",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Cathedral",value:"2" },
                           { name:"Chapel",value:"3" },
                           { name:"Church",value:"4" },
                           { name:"Marabout",value:"5" },
                           { name:"Minaret",value:"6" },
                           { name:"Monastry,Convent",value:"7" },
                           { name:"Mosque",value:"9" },
                           { name:"Pagoda",value:"11" },
                           { name:"Shrine",value:"14" },
                           { name:"Tabernacle",value:"15" },
                           { name:"Temple",value:"16" },
                           { name:"Synagogue",value:"20" },
                           { name:"Stupa",value:"21" },
                           { name:"Not Applicable",value:"22" }
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"SSR",
                       desc:"Structure Shape of Roof",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"No Information", value:"0" },
                           { name:"Dome", value:"40" },
                           { name:"With Steeple", value:"51" },
                           { name:"With Cupola", value:"77" },
                           { name:"With Tower", value:"79" },
                           { name:"With Minaret", value:"80" } 
                        ]
                     },
                     { name:"SST",
                       desc:"Sound Signal Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"-32768",
                       enumerations:[
                           { name:"Null", value:"-32768" },
                           { name:"Unknown", value:"0" },
                           { name:"Bell", value:"1" },
                           { name:"Diaphone", value:"2" },
                           { name:"Explosive fog signal", value:"3" },
                           { name:"Gong", value:"4" },
                           { name:"Horn", value:"6" },
                           { name:"Siren", value:"9" },
                           { name:"Whistle", value:"14" },
                           { name:"Reed", value:"15" },
                           { name:"None", value:"16" }
                        ]
                     },
                     { name:"STA",
                       desc:"Station Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown", value:"0" },
                           { name:"Coast Guard", value:"1" },
                           { name:"Fireboat", value:"2" },
                           { name:"Marine Police", value:"3" },
                           { name:"Ice Signal", value:"4" },
                           { name:"Lifeboat / Rescue", value:"5" },
                           { name:"Port Control", value:"6" },
                           { name:"Pilot", value:"11" },
                           { name:"Signal", value:"13" },
                           { name:"Storm Signal", value:"15" },
                           { name:"Tide Signal", value:"17" },
                           { name:"Time Signal", value:"19" },
                           { name:"Weather Signal", value:"21" },
                           { name:"Fog Signal", value:"22" },
                           { name:"Tidal Current Signal", value:"27" },
                           { name:"Marine Traffic Signal", value:"28" },
                           { name:"Bridge Signal", value:"29" },
                           { name:"Lock Signal", value:"30" },
                           { name:"International Port Signals", value:"32" },
                           { name:"Firing Practice Signal", value:"33" },
                        ]
                     },
                    ]
          },
        { name:"BUILTUPP",
          fcode:"",
          desc:"Built Up Area Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     }
                    ]
          },
        { name:"COMMP",
          fcode:"",
          desc:"Communications Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ]
                     },
                     { name:"BRF",
                       desc:"Broadcast Frequency",
                       optional:"R",
                       units:"Hertz",
                       type:"Integer",
                       defValue:"0"
                     },
                     { name:"COL",
                       desc:"Character of Light",
                       optional:"R",
                       length:"10",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"NST",
                       desc:"Navigation System Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown", value:"0" },
                           { name:"CONSOL", value:"2" },
                           { name:"DECCA", value:"3" },
                           { name:"LORAN", value:"7" },
                           { name:"OMEGA", value:"8" },
                           { name:"Radio", value:"12" },
                           { name:"Radio Telephone", value:"13" },
                           { name:"TV", value:"15" },
                           { name:"Microwave", value:"16" },
                           { name:"Radio Telegraph", value:"33" }
                        ]
                     }
                    ]
          },
        { name:"INDUSTP",
          fcode:"",
          desc:"Industry Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ]
                     },
                     { name:"COL",
                       desc:"Character of Light",
                       optional:"R",
                       length:"10",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"HGT",
                       desc:"Height Above Surface Level",
                       optional:"R",
                       units:"Metre",
                       type:"Integer",
                       defValue:"0"
                     },
                     { name:"LOC",
                       desc:"Location Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"8",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"On ground surface",value:"8" },
                           { name:"Off Shore",value:"22" },
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"PRO",
                       desc:"Product",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Electric",value:"31" },
                           { name:"Other",value:"999" },
                        ]
                     },
                     { name:"SSC",
                       desc:"Structure Shape Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Pyramid",value:"12" },
                           { name:"Spherical (Hemispherical)",value:"17" },
                           { name:"Artificial Mountain",value:"21" },
                           { name:"Ferris Wheel",value:"23" },
                           { name:"Mast",value:"60" },
                           { name:"Arch",value:"77" },
                           { name:"Dome",value:"87" },
                           { name:"Tower",value:"107" },
                           { name:"Scanner",value:"108" },
                           { name:"Obelisk",value:"109" },
                           { name:"Other",value:"999" }
                        ]
                     },
                     { name:"TUC",
                       desc:"Transportation Use Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown", value:"0" },
                           { name:"Both Road and Railroad", value:"1" },
                           { name:"Railroad", value:"3" },
                           { name:"Road", value:"4" },
                           { name:"Marine", value:"12" },
                           { name:"Air", value:"13" },
                           { name:"Pedestrian", value:"17" }
                        ]
                     },
                     { name:"USE",
                       desc:"Useage",
                       optional:"R",
                       type:"enumeration",
                       defValue:"132",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Container",value:"132" },
                           { name:"Other",value:"999" }
                        ]
                     },
                     { name:"ZV2",
                       desc:"Highest Z-Value",
                       optional:"R",
                       units:"metre",
                       type:"Integer",
                       defValue:"99999"
                     }
                    ]
          },
        { name:"LANDMRKP",
          fcode:"",
          desc:"Landmark Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ]
                     }
                    ]
          },
        { name:"POWERP",
          fcode:"",
          desc:"Power Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ]
                     }
                    ]
          },
        { name:"ECRAREA",
          fcode:"",
          desc:"Earthcover Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     }
                    ]
          },
        { name:"FORESHOA",
          fcode:"",
          desc:"Foreshore Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:"BA020"
                     },
                     { name:"MCC",
                       desc:"Material Composition Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown", value:"0" },
                           { name:"Boulders", value:"8" },
                           { name:"Clay", value:"16" },
                           { name:"Gravel", value:"46" },
                           { name:"Mud", value:"65" },
                           { name:"Rock / Rocky", value:"84" },
                           { name:"Sand", value:"88" },
                           { name:"Shingle", value:"98" },
                           { name:"Stone", value:"108" }
                        ]
                     },
                     { name:"MCS",
                       desc:"Material Compsition Secondary",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown", value:"0" },
                           { name:"Gravel", value:"46" },
                           { name:"Mud", value:"65" },
                           { name:"Sand", value:"88" },
                           { name:"Shingle", value:"98" },
                           { name:"Stone", value:"108" }
                        ]
                     }
                    ]
          },
        { name:"ADMINL",
          fcode:"",
          desc:"Administrative Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" },
                           { name:"Disputed",value:"5" },
                           { name:"Undisputed",value:"6" }
                        ]
                     },
                     { name:"NM3",
                       desc:"Name 3",
                       optional:"R",
                       length:"30",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"NM4",
                       desc:"Name 4",
                       optional:"R",
                       length:"30",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"USE",
                       desc:"Useage",
                       optional:"R",
                       type:"enumeration",
                       defValue:"23",
                       enumerations:[
                           { name:"International",value:"23" },
                           { name:"Insular",value:"32" }
                        ]
                     }
                    ]
          },
        { name:"BOUNDRYL",
          fcode:"",
          desc:"Boundary Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"NM3",
                       desc:"Name 3",
                       optional:"R",
                       length:"30",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"NM4",
                       desc:"Name 4",
                       optional:"R",
                       length:"30",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"USE",
                       desc:"Useage",
                       optional:"R",
                       type:"enumeration",
                       defValue:"23",
                       enumerations:[
                           { name:"International",value:"23" }
                        ]
                     }
                    ]
          },
        { name:"COASTL",
          fcode:"",
          desc:"Coastline Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ]
                     },
                     { name:"SLT",
                       desc:"Shoreline Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown", value:"0" },
                           { name:"Mangrove / Nipa", value:"6" },
                           { name:"Marsh, Swamp", value:"8" },
                           { name:"Rocky", value:"10" },
                           { name:"Rubble", value:"11" },
                           { name:"Sandy", value:"13" },
                           { name:"Stony, Shingly", value:"14" },
                           { name:"Other", value:"15" },
                        ]
                     },
                    ]
          },
        { name:"FORESHOL",
          fcode:"",
          desc:"Foreshore Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     }
                    ]
          },
        { name:"FORESHOP",
          fcode:"",
          desc:"Foreshore Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:"BA020"
                     },
                     { name:"MCC",
                       desc:"Material Composition Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown", value:"0" },
                           { name:"Boulders", value:"8" },
                           { name:"Clay", value:"16" },
                           { name:"Gravel", value:"46" },
                           { name:"Mud", value:"65" },
                           { name:"Rock / Rocky", value:"84" },
                           { name:"Sand", value:"88" },
                           { name:"Shingle", value:"98" },
                           { name:"Stone", value:"108" }
                        ]
                     },
                     { name:"MCS",
                       desc:"Material Compsition Secondary",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown", value:"0" },
                           { name:"Gravel", value:"46" },
                           { name:"Mud", value:"65" },
                           { name:"Sand", value:"88" },
                           { name:"Shingle", value:"98" },
                           { name:"Stone", value:"108" }
                        ]
                     }
                    ]
          },
        { name:"ISLANDP",
          fcode:"",
          desc:"Island Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:"BA030"
                     }
                    ]
          },
        { name:"ECRTEXT",
          fcode:"",
          desc:"Earthcover Text",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:"ZD040"
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                    ]
          },
        { name:"ENVAREA",
          fcode:"",
          desc:"Environment Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:"ZC040"
                     },
                     { name:"VAV",
                       desc:"Variation Anomaly Value",
                       optional:"R",
                       units:"degrees",
                       type:"Integer",
                       defValue:"0"
                     },
                    ]
          },
        { name:"CURRDIAP",
          fcode:"",
          desc:"Current Diagram Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"C80",
                       desc:"Rate of Current",
                       optional:"R",
                       units:"knot",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"C81",
                       desc:"Rate of Current 1",
                       optional:"R",
                       units:"knot",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"C82",
                       desc:"Rate of Current 2",
                       optional:"R",
                       units:"knot",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"C83",
                       desc:"Rate of Current 3",
                       optional:"R",
                       units:"knot",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"C84",
                       desc:"Rate of Current 4",
                       optional:"R",
                       units:"knot",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"C85",
                       desc:"Rate of Current 5",
                       optional:"R",
                       units:"knot",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"C86",
                       desc:"Rate of Current 6",
                       optional:"R",
                       units:"knot",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"C87",
                       desc:"Rate of Current 7",
                       optional:"R",
                       units:"knot",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"C88",
                       desc:"Rate of Current 8",
                       optional:"R",
                       units:"knot",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"C89",
                       desc:"Rate of Current 9",
                       optional:"R",
                       units:"knot",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"C90",
                       desc:"Rate of Current 10",
                       optional:"R",
                       units:"knot",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"C91",
                       desc:"Rate of Current 11",
                       optional:"R",
                       units:"knot",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"D80",
                       desc:"Direction of Current",
                       optional:"R",
                       units:"degree",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"D81",
                       desc:"Direction of Current 1",
                       optional:"R",
                       units:"degree",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"D82",
                       desc:"Direction of Current 2",
                       optional:"R",
                       units:"degree",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"D83",
                       desc:"Direction of Current 3",
                       optional:"R",
                       units:"degree",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"D84",
                       desc:"Direction of Current 4",
                       optional:"R",
                       units:"degree",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"D85",
                       desc:"Direction of Current 5",
                       optional:"R",
                       units:"degree",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"D86",
                       desc:"Direction of Current 6",
                       optional:"R",
                       units:"degree",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"D87",
                       desc:"Direction of Current 7",
                       optional:"R",
                       units:"degree",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"D88",
                       desc:"Direction of Current 8",
                       optional:"R",
                       units:"degree",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"D89",
                       desc:"Direction of Current 9",
                       optional:"R",
                       units:"degree",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"D90",
                       desc:"Direction of Current 10",
                       optional:"R",
                       units:"degree",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"D91",
                       desc:"Direction of Current 11",
                       optional:"R",
                       units:"degree",
                       type:"Real",
                       defValue:"0.0"
                     }
                    ]
          },
        { name:"CURRFLP",
          fcode:"",
          desc:"Current Flow Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"CRN",
                       desc:"Current Rate Minimum",
                       optional:"R",
                       units:"knot",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"CRX",
                       desc:"Current Rate Maximum",
                       optional:"R",
                       units:"knot",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"CUR",
                       desc:"Current Type Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Ebb",value:"1" },
                           { name:"Flood",value:"3" },
                           { name:"General",value:"4" },
                           { name:"Ocean",value:"5" }
                        ]
                     },
                     { name:"DOF",
                       desc:"Direction of Flow",
                       optional:"R",
                       units:"degree",
                       type:"Integer",
                       defValue:"0"
                     },
                     { name:"HS1",
                       desc:"Current Information 1",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Jan",value:"1" },
                           { name:"Feb",value:"2" },
                           { name:"Mar",value:"3" },
                           { name:"Apr",value:"4" },
                           { name:"May",value:"5" },
                           { name:"Jun",value:"6" },
                           { name:"Jul",value:"7" },
                           { name:"Aug",value:"8" },
                           { name:"Sep",value:"9" },
                           { name:"Oct",value:"10" },
                           { name:"Nov",value:"11" },
                           { name:"Dec",value:"12" }
                        ]
                     },
                     { name:"HS2",
                       desc:"Current Information 1",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Jan",value:"1" },
                           { name:"Feb",value:"2" },
                           { name:"Mar",value:"3" },
                           { name:"Apr",value:"4" },
                           { name:"May",value:"5" },
                           { name:"Jun",value:"6" },
                           { name:"Jul",value:"7" },
                           { name:"Aug",value:"8" },
                           { name:"Sep",value:"9" },
                           { name:"Oct",value:"10" },
                           { name:"Nov",value:"11" }
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                    ]
          },
        { name:"TIDEP",
          fcode:"",
          desc:"Tide Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                    ]
          },
        { name:"HYDAREA",
          fcode:"",
          desc:"Hydrographic Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ]
                     },
                     { name:"CVH",
                       desc:"Depth Curve or Contour Value High",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:"99999.0"
                     },
                     { name:"CVL",
                       desc:"Depth Curve or Contour Value Low",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:"99999.0"
                     },
                    ]
          },
        { name:"HYDLINE",
          fcode:"",
          desc:"Hydrographic Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ]
                     },
                     { name:"CRV",
                       desc:"Depth Curve or Contour Value",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:"99999.0"
                     }
                    ]
          },
        { name:"BOTCHARP",
          fcode:"BF010",
          desc:"Bottom Characteristics Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"CSM",
                       desc:"Secondary Material Characteristics",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Broken",value:"1" },
                           { name:"Coarse",value:"2" },
                           { name:"Decayed",value:"3" },
                           { name:"Fine",value:"4" },
                           { name:"Gritty",value:"5" },
                           { name:"Hard",value:"6" },
                           { name:"Rotten",value:"7" },
                           { name:"Soft",value:"8" },
                           { name:"Sticky",value:"9" },
                           { name:"Stiff",value:"10" },
                           { name:"Streaky",value:"11" },
                           { name:"Tenacious",value:"12" },
                           { name:"Uneven",value:"13" },
                           { name:"Calcareous",value:"17" },
                           { name:"Flinty",value:"18" },
                           { name:"Glacial",value:"19" },
                           { name:"Ground",value:"20" },
                           { name:"Large",value:"21" },
                           { name:"Rocky",value:"22" },
                           { name:"Small",value:"23" },
                           { name:"Speckled",value:"24" },
                           { name:"Varied",value:"25" },
                           { name:"Volcanic",value:"26" },
                           { name:"Medium",value:"27" },
                       ]
                     },
                     { name:"MCC",
                       desc:"Material Composition Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Ash",value:"4" },
                           { name:"Boulders",value:"8" },
                           { name:"Chalk",value:"12" },
                           { name:"Cinders",value:"14" },
                           { name:"Cirripedia",value:"15" },
                           { name:"Clay",value:"16" },
                           { name:"Cobble",value:"18" },
                           { name:"Coral",value:"24" },
                           { name:"CoralHead",value:"25" },
                           { name:"Diatoms",value:"28" },
                           { name:"Foraminifera",value:"36" },
                           { name:"Fucus",value:"37" },
                           { name:"Globigerina",value:"41" },
                           { name:"Grass/Thatch",value:"45" },
                           { name:"Gravel",value:"46" },
                           { name:"Ground",value:"48" },
                           { name:"Lava",value:"52" },
                           { name:"Madrepores",value:"58" },
                           { name:"Manganese",value:"59" },
                           { name:"Marl",value:"61" },
                           { name:"Mattes",value:"63" },
                           { name:"Mud",value:"65" },
                           { name:"Mussels",value:"66" },
                           { name:"Ooze",value:"69" },
                           { name:"Oysters",value:"70" },
                           { name:"Pebbles",value:"73" },
                           { name:"Polyzoa",value:"75" },
                           { name:"Pteropods",value:"78" },
                           { name:"Pumice",value:"79" },
                           { name:"Quartz",value:"80" },
                           { name:"Radiolaria",value:"81" },
                           { name:"Rock/Rocky",value:"84" },
                           { name:"Sand",value:"88" },
                           { name:"Schist",value:"90" },
                           { name:"Scoria",value:"92" },
                           { name:"Sea-Tangle",value:"93" },
                           { name:"Seaweed",value:"94" },
                           { name:"Shells",value:"96" },
                           { name:"Shingle",value:"98" },
                           { name:"Silt",value:"99" },
                           { name:"Spicules",value:"105" },
                           { name:"Sponge",value:"106" },
                           { name:"Stone",value:"108" },
                           { name:"Tufa",value:"111" },
                       ]
                     },
                     { name:"MCS",
                       desc:"Material Composition Secondary",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Ash",value:"4" },
                           { name:"Boulders",value:"8" },
                           { name:"Chalk",value:"12" },
                           { name:"Cinders",value:"14" },
                           { name:"Cirripedia",value:"15" },
                           { name:"Clay",value:"16" },
                           { name:"Cobble",value:"18" },
                           { name:"Coral",value:"24" },
                           { name:"Coral Head",value:"25" },
                           { name:"Diatoms",value:"28" },
                           { name:"Foraminifera",value:"36" },
                           { name:"Fucus",value:"37" },
                           { name:"Globigerina",value:"41" },
                           { name:"Grass/Thatch",value:"45" },
                           { name:"Gravel",value:"46" },
                           { name:"Ground",value:"48" },
                           { name:"Lava",value:"52" },
                           { name:"Madrepores",value:"58" },
                           { name:"Manganese",value:"59" },
                           { name:"Marl",value:"61" },
                           { name:"Mattes",value:"63" },
                           { name:"Mud",value:"65" },
                           { name:"Mussels",value:"66" },
                           { name:"Ooze",value:"69" },
                           { name:"Oysters",value:"70" },
                           { name:"Pebbles",value:"73" },
                           { name:"Polyzoa",value:"75" },
                           { name:"Pteropods",value:"78" },
                           { name:"Pumice",value:"79" },
                           { name:"Quartz",value:"80" },
                           { name:"Radiolaria",value:"81" },
                           { name:"Rock/Rocky",value:"84" },
                           { name:"Sand",value:"88" },
                           { name:"Schist",value:"90" },
                           { name:"Scoria",value:"92" },
                           { name:"Sea-Tangle",value:"93" },
                           { name:"Seaweed",value:"94" },
                           { name:"Shells",value:"96" },
                           { name:"Shingle",value:"98" },
                           { name:"Silt",value:"99" },
                           { name:"Spicules",value:"105" },
                           { name:"Sponge",value:"106" },
                           { name:"Stone",value:"108" },
                           { name:"Tufa",value:"111" },
                       ]
                     },
                     { name:"MCU",
                       desc:"Material Composition Underlying",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Ash",value:"4" },
                           { name:"Boulders",value:"8" },
                           { name:"Chalk",value:"12" },
                           { name:"Cinders",value:"14" },
                           { name:"Cirripedia",value:"15" },
                           { name:"Clay",value:"16" },
                           { name:"Cobble",value:"18" },
                           { name:"Coral",value:"24" },
                           { name:"Coral Head",value:"25" },
                           { name:"Diatoms",value:"28" },
                           { name:"Foraminifera",value:"36" },
                           { name:"Fucus",value:"37" },
                           { name:"Globigerina",value:"41" },
                           { name:"Grass/Thatch",value:"45" },
                           { name:"Gravel",value:"46" },
                           { name:"Ground",value:"48" },
                           { name:"Lava",value:"52" },
                           { name:"Madrepores",value:"58" },
                           { name:"Manganese",value:"59" },
                           { name:"Marl",value:"61" },
                           { name:"Mattes",value:"63" },
                           { name:"Mud",value:"65" },
                           { name:"Mussels",value:"66" },
                           { name:"Ooze",value:"69" },
                           { name:"Oysters",value:"70" },
                           { name:"Pebbles",value:"73" },
                           { name:"Polyzoa",value:"75" },
                           { name:"Pteropods",value:"78" },
                           { name:"Pumice",value:"79" },
                           { name:"Quartz",value:"80" },
                           { name:"Radiolaria",value:"81" },
                           { name:"Rock/Rocky",value:"84" },
                           { name:"Sand",value:"88" },
                           { name:"Schist",value:"90" },
                           { name:"Scoria",value:"92" },
                           { name:"Sea-Tangle",value:"93" },
                           { name:"Seaweed",value:"94" },
                           { name:"Shells",value:"96" },
                           { name:"Shingle",value:"98" },
                           { name:"Silt",value:"99" },
                           { name:"Spicules",value:"105" },
                           { name:"Sponge",value:"106" },
                           { name:"Stone",value:"108" },
                           { name:"Tufa",value:"111" },
                       ]
                     },
                     { name:"PSC",
                       desc:"Physical Surface Characteristics",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Broken",value:"1" },
                           { name:"Coarse",value:"2" },
                           { name:"Decayed",value:"3" },
                           { name:"Fine",value:"4" },
                           { name:"Gritty",value:"5" },
                           { name:"Hard",value:"6" },
                           { name:"Rotten",value:"7" },
                           { name:"Soft",value:"8" },
                           { name:"Sticky",value:"9" },
                           { name:"Stiff",value:"10" },
                           { name:"Streaky",value:"11" },
                           { name:"Tenacious",value:"12" },
                           { name:"Uneven",value:"13" },
                           { name:"Calcareous",value:"17" },
                           { name:"Flinty",value:"18" },
                           { name:"Glacial",value:"19" },
                           { name:"Ground",value:"20" },
                           { name:"Large",value:"21" },
                           { name:"Rocky",value:"22" },
                           { name:"Small",value:"23" },
                           { name:"Speckled",value:"24" },
                           { name:"Varied",value:"25" },
                           { name:"Volcanic",value:"26" },
                           { name:"Medium",value:"27" },
                           { name:"Springs in Seabed",value:"28" },
                           { name:"Mobile Bottom",value:"29" },
                       ]
                     },
                     { name:"UMC",
                       desc:"Underlying Material Characteristics",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Broken",value:"1" },
                           { name:"Coarse",value:"2" },
                           { name:"Decayed",value:"3" },
                           { name:"Fine",value:"4" },
                           { name:"Gritty",value:"5" },
                           { name:"Hard",value:"6" },
                           { name:"Rotten",value:"7" },
                           { name:"Soft",value:"8" },
                           { name:"Sticky",value:"9" },
                           { name:"Stiff",value:"10" },
                           { name:"Streaky",value:"11" },
                           { name:"Tenacious",value:"12" },
                           { name:"Uneven",value:"13" },
                           { name:"Calcareous",value:"17" },
                           { name:"Flinty",value:"18" },
                           { name:"Glacial",value:"19" },
                           { name:"Ground",value:"20" },
                           { name:"Large",value:"21" },
                           { name:"Rocky",value:"22" },
                           { name:"Small",value:"23" },
                           { name:"Speckled",value:"24" },
                           { name:"Varied",value:"25" },
                           { name:"Volcanic",value:"26" },
                           { name:"Medium",value:"27" },
                       ]
                     },
                    ]
          },
        { name:"SOUNDP",
          fcode:"",
          desc:"Soundings Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" },
                           { name:"Doubtful",value:"3" }
                        ]
                     },
                     { name:"DAT",
                       desc:"Date",
                       optional:"R",
                       type:"Integer",
                       defValue:"26"
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Definite",value:"1" },
                           { name:"Doubtful",value:"2" },
                           { name:"Reported",value:"3" },
                        ]
                     },
                     { name:"HDH",
                       desc:"Hydrographic Drying Height",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:""
                     },
                     { name:"HDP",
                       desc:"Hydrographic Depth",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:""
                     },
                     { name:"SND",
                       desc:"Sounding Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"10",
                       enumerations:[
                           { name:"Drying Heights",value:"1" },
                           { name:"No Bottom Found",value:"2" },
                           { name:"Slant",value:"9" },
                           { name:"Ordinary",value:"10" },
                           { name:"Other",value:"999" }
                        ]
                     },
                     { name:"SVC",
                       desc:"Sounding Velocity",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Echo Sounder Calibrated at 4800 ft/sec Uncorrected",value:"1" },
                           { name:"Echo Sounder Calibrated at 1500 m/sec Uncorrected",value:"2" },
                           { name:"Mathews Tables (NP 139) Corrected",value:"3" },
                           { name:"SVM Corrected",value:"4" },
                           { name:"Other Calibration",value:"5" },
                        ]
                     },
                     { name:"VAL",
                       desc:"Value",
                       optional:"R",
                       units:"year",
                       type:"Integer",
                       defValue:"0"
                     },
                    ]
          },
        { name:"CANALA",
          fcode:"",
          desc:"Canal Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"32",
                       enumerations:[
                           { name:"Abandoned/Disused",value:"6" },
                           { name:"Navigable",value:"32" }
                        ]
                     },
                     { name:"HDP",
                       desc:"Hydrographic Depth",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:""
                     },
                     { name:"HYC",
                       desc:"Hydrological Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"8",
                       enumerations:[
                           { name:"Dry",value:"3" },
                           { name:"Perennial/Permanent",value:"8" }
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"RPA",
                       desc:"Required Port Access",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Access Required",value:"1" },
                           { name:"Access Not Required",value:"2" }
                        ]
                     },
                     { name:"WID",
                       desc:"Width",
                       optional:"R",
                       units:"metre",
                       type:"Integer",
                       defValue:"0"
                     },
                    ]
          },
        { name:"LAKEA",
          fcode:"",
          desc:"Lake Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"HYC",
                       desc:"Hydrological Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"8",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Perennial/Permanent",value:"8" }
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"RPA",
                       desc:"Required Port Access",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Access Required",value:"1" },
                           { name:"Access Not Required",value:"2" }
                        ]
                     },
                    ]
          },
        { name:"MISCIWYA",
          fcode:"",
          desc:"Miscellaneous Inland Waterways Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Natural",value:"45" },
                           { name:"Man-made",value:"46" }
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"WID",
                       desc:"Width",
                       optional:"R",
                       units:"metre",
                       type:"Integer",
                       defValue:"0"
                     },
                    ]
          },
        { name:"RIVERA",
          fcode:"",
          desc:"River Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ]
                     },
                     { name:"HYC",
                       desc:"Hydrological Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"8",
                       enumerations:[
                           { name:"Perennial/Permanent",value:"8" }
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"RPA",
                       desc:"Required Port Access",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Access Required",value:"1" },
                           { name:"Access Not Required",value:"2" }
                        ]
                     },
                     { name:"SLT",
                       desc:"Shoreline Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Mangrove/Nipa",value:"6" },
                           { name:"Marsh,Swamp",value:"8" },
                           { name:"Rocky",value:"10" },
                           { name:"Rubble",value:"11" },
                           { name:"Sandy",value:"13" },
                           { name:"Stony,Shingly",value:"14" },
                           { name:"Other",value:"15" }
                        ]
                     },
                    ]
          },
        { name:"AQUEDCTL",
          fcode:"",
          desc:"Aqueduct Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"LOC",
                       desc:"Location Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Below Surface / Submerged / Underground",value:"4" },
                           { name:"On ground surface",value:"8" },
                           { name:"Suspended / Elevated Above Ground or Water",value:"25" },
                        ]
                     },
                     { name:"OHC",
                       desc:"Overhead Clearance Category",
                       optional:"R",
                       type:"Real",
                       defValue:"0.0",
                     },
                     { name:"OWO",
                       desc:"Over Water Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Feature crosses navigable water",value:"1" },
                        ]
                     }
                    ]
          },
        { name:"BARRIERL",
          fcode:"",
          desc:"Barrier Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"50",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Non-Tidal",value:"50" }
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                    ]
          },
        { name:"CANALL",
          fcode:"",
          desc:"Canal Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"32",
                       enumerations:[
                           { name:"Navigable",value:"32" }
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"WID",
                       desc:"Width",
                       optional:"R",
                       units:"metre",
                       type:"Integer",
                       defValue:"0"
                     },
                    ]
          },
        { name:"DAML",
          fcode:"",
          desc:"Dam Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"USE",
                       desc:"Usage",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Flood Barrage",value:"131" },
                           { name:"Other",value:"999" }
                        ]
                     },
                    ]
          },
        { name:"RIVERL",
          fcode:"",
          desc:"River Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ]
                     },
                     { name:"AHC",
                       desc:"Associated Hydrological Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Perennial",value:"1" },
                           { name:"Intermittent",value:"2" }
                        ]
                     },
                     { name:"HYC",
                       desc:"Hydrological Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"8",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Non-Perennial/Intermittent/Fluctuating",value:"6" },
                           { name:"Perennial/Permanent",value:"8" }
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"SLT",
                       desc:"Shoreline Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Mangrove/Nipa",value:"6" },
                           { name:"Marsh,Swamp",value:"8" },
                           { name:"Rocky",value:"10" },
                           { name:"Rubble",value:"11" },
                           { name:"Sandy",value:"13" },
                           { name:"Stony,Shingly",value:"14" },
                           { name:"Other",value:"15" }
                        ]
                     },
                    ]
          },
        { name:"EMBANKA",
          fcode:"",
          desc:"Embankment Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"51",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Tidal/Tidal Fluctuation",value:"51" }
                        ]
                     },
                     { name:"USE",
                       desc:"Usage",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"as a Causeway",value:"127" }
                        ]
                     },
                     { name:"VRR",
                       desc:"Vertical Reference Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Above surface / Does Not Cover (At High Water)",value:"1" },
                           { name:"Covers and Uncovers",value:"8" },
                           { name:"Not Applicable",value:"9" }
                        ]
                     }
                    ]
          },
        { name:"ORCHARDA",
          fcode:"",
          desc:"Orchard Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     }
                    ]
          },
        { name:"SNOWICEA",
          fcode:"",
          desc:"Snow/Ice Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"SIC",
                       desc:"Snow/Ice Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Snow",value:"1" },
                           { name:"Ice",value:"2" }
                        ]
                     }
                    ]
          },
        { name:"TREEA",
          fcode:"",
          desc:"Tree Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"30",
                       enumerations:[
                           { name:"Not Isolated",value:"30" },
                        ]
                     },
                     { name:"TRE",
                       desc:"Tree Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Deciduous",value:"1" },
                           { name:"Evergreen",value:"2" },
                           { name:"Mixed",value:"3" }
                        ]
                     },
                     { name:"VEG",
                       desc:"Vegetation Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Casuarina",value:"11" },
                           { name:"Coniferous",value:"12" },
                           { name:"Nipa Palm",value:"16" },
                           { name:"Palm",value:"17" },
                           { name:"Filao",value:"18" },
                           { name:"Mangrove",value:"19" },
                           { name:"Eucalyptus",value:"29" },
                           { name:"Cypress",value:"38" },
                           { name:"Other",value:"999" }
                        ]
                     }
                    ]
          },
        { name:"VOLCANOA",
          fcode:"",
          desc:"Volcano Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"DAT",
                       desc:"Date",
                       optional:"R",
                       type:"Integer",
                       defValue:"26"
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Definite",value:"1" },
                           { name:"Doubtful",value:"2" },
                           { name:"Reported",value:"3" },
                        ]
                     },
                     { name:"LOC",
                       desc:"Location Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"4",
                       enumerations:[
                           { name:"Below Surface / Submerged / Underground",value:"4" }
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"VAL",
                       desc:"Value",
                       optional:"R",
                       units:"year",
                       type:"Integer",
                       defValue:"0"
                     }
                    ]
          },
        { name:"LCRLINE",
          fcode:"",
          desc:"Land Cover Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"USE",
                       desc:"Usage",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Levee/Dyke",value:"69" },
                           { name:"as a Causeway",value:"127" }
                        ]
                     },
                     { name:"VRR",
                       desc:"Vertical Reference Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Above surface / Does Not Cover (At High Water)",value:"1" },
                           { name:"Covers and Uncovers",value:"8" },
                           { name:"Not Applicable",value:"9" }
                        ]
                     },
                    ]
          },
        { name:"SNOWICEP",
          fcode:"",
          desc:"Snow/Ice Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"HGT",
                       desc:"Height Above Surface Level",
                       optional:"R",
                       units:"Metre",
                       type:"Integer",
                       defValue:"0"
                     },
                    ]
          },
        { name:"TREEP",
          fcode:"",
          desc:"Tree Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"31",
                       enumerations:[
                           { name:"Isolated",value:"31" },
                        ]
                     },
                     { name:"TRE",
                       desc:"Tree Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Deciduous",value:"1" },
                           { name:"Evergreen",value:"2" },
                           { name:"Mixed",value:"3" }
                        ]
                     },
                     { name:"VEG",
                       desc:"Vegetation Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Casuarina",value:"11" },
                           { name:"Coniferous",value:"12" },
                           { name:"Nipa Palm",value:"16" },
                           { name:"Palm",value:"17" },
                           { name:"Filao",value:"18" },
                           { name:"Eucalyptus",value:"29" },
                        ]
                     }
                    ]
          },
        { name:"LIMBNDYA",
          fcode:"",
          desc:"Limit Boundaries Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"COD",
                       desc:"Certainty of Delineation",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Limits and Info Known",value:"1" },
                           { name:"Limits and Info Unknown",value:"2" }
                        ]
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"46",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Natural",value:"45" },
                           { name:"Man-made",value:"46" }
                        ]
                     },
                     { name:"MAC",
                       desc:"Maritime Area Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Anchorage (general)",value:"11" },
                           { name:"Anchoring Berths",value:"12" },
                           { name:"Explosive Anchorage",value:"13" },
                           { name:"Large Vessel/Deepwater/Deep Draft",value:"14" },
                           { name:"Anchoring Prohibited",value:"15" },
                           { name:"Quarantine Anchorage",value:"16" },
                           { name:"Reserved Anchorage",value:"17" },
                           { name:"Tanker Anchorage",value:"19" },
                           { name:"Submarine Cable Area",value:"20" },
                           { name:"Pipeline Area",value:"21" },
                           { name:"Fishing Prohibited",value:"22" },
                           { name:"Cable and Pipeline Area",value:"23" },
                           { name:"Turning Area/Swinging Circle",value:"24" },
                           { name:"Spoil Area/Spoil Ground",value:"25" },
                           { name:"Unsurveyed Area",value:"26" },
                           { name:"Submarine Exercise Area",value:"27" },
                           { name:"Mine Laying Practice Area",value:"28" },
                           { name:"Firing Danger Area",value:"29" },
                           { name:"Dumping Ground for Hazardous Materials",value:"30" },
                           { name:"Incineration Area",value:"31" },
                           { name:"Oil Field",value:"32" },
                           { name:"Gas Field",value:"33" },
                           { name:"Explosive Dumping Ground",value:"35" },
                           { name:"Safety Zone",value:"37" },
                           { name:"Precautionary Area",value:"42" },
                           { name:"Areas to be Avoided",value:"43" },
                           { name:"Degaussing Range",value:"44" },
                           { name:"Outfall Area",value:"45" },
                           { name:"Intake Area",value:"46" },
                           { name:"Pilot Boarding Area",value:"48" },
                           { name:"Cargo Transshipment Area",value:"49" },
                           { name:"Seaplane Anchorage",value:"53" },
                           { name:"Time Limited Anchorage",value:"54" },
                           { name:"Fairway",value:"55" },
                           { name:"Fish Trap Area",value:"56" },
                           { name:"Marine Farm",value:"57" },
                           { name:"Dredging Area",value:"58" },
                           { name:"Sewer Area",value:"61" },
                           { name:"Other",value:"999" },
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"OPS",
                       desc:"Operational Status",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Operational",value:"1" },
                           { name:"non-Operational",value:"2" }
                        ]
                     },
                     { name:"PBV",
                       desc:"Pilot Boarding Vehicle",
                       optional:"R",
                       type:"enumeration",
                       defValue:"-32768",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"By Boat",value:"1" },
                           { name:"By Helecopter",value:"2" }
                        ]
                     },
                     { name:"PRO",
                       desc:"Product",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Ammunition",value:"3" },
                           { name:"Chemical",value:"13" },
                           { name:"Explosives",value:"33" },
                           { name:"Gas",value:"38" },
                           { name:"Gasoline",value:"39" },
                           { name:"Oll",value:"67" },
                           { name:"Radioactive Material",value:"82" },
                           { name:"Water",value:"116" },
                           { name:"None",value:"130" }
                        ]
                     },
                     { name:"TIM",
                       desc:"Time Attribute",
                       optional:"R",
                       type:"Integer",
                       defValue:"-32l68"
                     },
                     { name:"TXT",
                       desc:"Text Attribute",
                       optional:"R",
                       length:"255",
                       type:"String",
                       defValue:"None"
                     },
                     { name:"USE",
                       desc:"Usage",
                       optional:"R",
                       type:"enumeration",
                       defValue:"-32768",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Telegraph",value:"51" },
                           { name:"Telephone",value:"52" },
                           { name:"Power",value:"53" },
                           { name:"Other",value:"999" }
                        ]
                     }
                  ]
          },
        { name:"MARITIMA",
          fcode:"",
          desc:"Maritime Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ATN",
                       desc:"Aids to Navigation",
                       optional:"R",
                       type:"enumeration",
                       defValue:"2",
                       enumerations:[
                           { name:"Marked",value:"1" },
                           { name:"Unmarked",value:"2" }
                        ]
                     },
                     { name:"COD",
                       desc:"Certainty of Delineation",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Limits and Info Known",value:"1" },
                           { name:"Limits and Info Unknown",value:"2" }
                        ]
                     },
                     { name:"DAN",
                       desc:"Description of Aids to Navigation",
                       optional:"R",
                       length:"255",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"DAT",
                       desc:"Date",
                       optional:"R",
                       type:"Integer",
                       defValue:"26"
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Definite",value:"1" },
                           { name:"Reported",value:"3" }
                        ]
                     },
                     { name:"HDP",
                       desc:"Hydrographic Depth",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:""
                     },
                     { name:"IAS",
                       desc:"IMO Approval Status",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Approved",value:"1" },
                           { name:"Not Approved",value:"2" }
                        ]
                     },
                     { name:"MAC",
                       desc:"Maritime Area Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Dredged Channel/Dregged Area",value:"2" },
                           { name:"Mine Danger Area",value:"4" },
                           { name:"Prohibited Shipping Area/Entry",value:"5" },
                           { name:"Works in Progress Area",value:"9" },
                           { name:"Roundabout Zone (TSS)",value:"40" },
                           { name:"Inshore traffic Zone (TSS)",value:"41" }
                        ]
                     },
                     { name:"MAS",
                       desc:"Maintenance Status",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Maintained",value:"1" },
                           { name:"Not Maintained",value:"2" }
                        ]
                     },
                     { name:"TSP",
                       desc:"Traffic Scheme Part",
                       optional:"R",
                       type:"enumeration",
                       defValue:"-32768",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Separation Zone Area",value:"3" }
                        ]
                     },
                     { name:"VAL",
                       desc:"Value",
                       optional:"R",
                       units:"year",
                       type:"Integer",
                       defValue:"0"
                     },
                     { name:"WID",
                       desc:"Width",
                       optional:"R",
                       units:"metre",
                       type:"Integer",
                       defValue:"-2147483648"
                     },
                     { name:"WPC",
                       desc:"Work in Progress Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Land Reclamation",value:"1" },
                           { name:"Construction of Structures",value:"2" }
                        ]
                     }
                  ]
          },
        { name:"ROUTEA",
          fcode:"",
          desc:"Route Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"HDI",
                       desc:"Hydrographic Depth/Height Information",
                       optional:"R",
                       type:"enumeration",
                       defValue:"12",
                       enumerations:[
                           { name:"Depth known by Other than Wire",value:"9" },
                           { name:"Depth Known",value:"12" }
                        ]
                     },
                     { name:"HDP",
                       desc:"Hydrographic Depth",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:""
                     },
                     { name:"RTT",
                       desc:"Route Intended Use",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Deep Water Route",value:"4" },
                           { name:"Two Way Route",value:"11" }
                        ]
                     }
                    ]
          },
        { name:"SEPARTNA",
          fcode:"",
          desc:"Separation Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Definite",value:"1" }
                        ]
                     },
                     { name:"IAS",
                       desc:"IMO Approval Status",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Approved",value:"1" },
                           { name:"Not Approved",value:"2" }
                        ]
                     },
                     { name:"OPS",
                       desc:"Operational Status",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Operational",value:"1" },
                           { name:"non-Operational",value:"2" }
                        ]
                     },
                     { name:"TSP",
                       desc:"Traffic Scheme Part",
                       optional:"R",
                       type:"enumeration",
                       defValue:"3",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Separation Zone Area",value:"3" },
                           { name:"Inbound Area",value:"6" },
                           { name:"Outbound Area",value:"7" }
                        ]
                     },
                     { name:"WID",
                       desc:"Width",
                       optional:"R",
                       units:"metre",
                       type:"Integer",
                       defValue:"0"
                     }
                    ]
          },
        { name:"SWEPTA",
          fcode:"",
          desc:"Swept Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"DAT",
                       desc:"Date",
                       optional:"R",
                       type:"Integer",
                       defValue:"26"
                     },
                     { name:"HDP",
                       desc:"Hydrographic Depth",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"VAL",
                       desc:"Value",
                       optional:"R",
                       units:"year",
                       type:"Integer",
                       defValue:"0"
                     }
                    ]
          },
        { name:"DISTL",
          fcode:"",
          desc:"Distance Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"BRR",
                       desc:"Bearing and Reciprocal Category",
                       optional:"R",
                       length:"11",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"LOR",
                       desc:"Length of Range",
                       optional:"R",
                       units:"metre",
                       type:"Integer",
                       defValue:"0"
                     },
                     { name:"UNI",
                       desc:"Units Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Metres",value:"1" },
                           { name:"Nautical Miles",value:"11" },
                           { name:"Feet",value:"22" },
                           { name:"Kilometres",value:"23" },
                           { name:"Yards",value:"24" }
                        ]
                     }
                    ]
          },
        { name:"FERRYL",
          fcode:"",
          desc:"Ferry Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"FER",
                       desc:"Ferry Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"2",
                       enumerations:[
                           { name:"With Cables/Chains",value:"1" },
                           { name:"Without Cables/Chains",value:"2" }
                        ]
                     }
                    ]
          },
        { name:"LIMBNDYL",
          fcode:"",
          desc:"Limit Boundaries Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"BRG",
                       desc:"Bearing of Object",
                       optional:"R",
                       units:"degree",
                       type:"Real",
                       defValue:""
                     },
                     { name:"COD",
                       desc:"Certainty of Delineation",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Limits and Info Known",value:"1" },
                           { name:"Limits and Info Unknown",value:"2" }
                        ]
                     },
                     { name:"DRP",
                       desc:"Description of Reference Point",
                       optional:"R",
                       length:"30",
                       type:"String",
                       defValue:"N/A"
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"46",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Natural",value:"45" },
                           { name:"Man-made",value:"46" }
                        ]
                     },
                     { name:"LAF",
                       desc:"Line Associated Features",
                       optional:"R",
                       type:"enumeration",
                       defValue:"-32768",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"One Object (Other Than a Directional Light)",value:"1" },
                           { name:"Directional Light",value:"2" },
                           { name:"Two or More Lights",value:"3" },
                           { name:"Two or More Beacons",value:"4" },
                           { name:"2 or More Objects (Other Than 2 Lights or 2 Beacons)",value:"5" },
                           { name:"Measured Distance Markers",value:"6" }
                        ]
                     },
                     { name:"MAC",
                       desc:"Maritime Area Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Anchorage (general)",value:"11" },
                           { name:"Anchoring Berths",value:"12" },
                           { name:"Explosive Anchorage",value:"13" },
                           { name:"Large Vessel/Deepwater/Deep Draft",value:"14" },
                           { name:"Anchoring Prohibited",value:"15" },
                           { name:"Quarantine Anchorage",value:"16" },
                           { name:"Reserved Anchorage",value:"17" },
                           { name:"Tanker Anchorage",value:"19" },
                           { name:"Submarine Cable Area",value:"20" },
                           { name:"Pipeline Area",value:"21" },
                           { name:"Fishing Prohibited",value:"22" },
                           { name:"Cable and Pipeline Area",value:"23" },
                           { name:"Turning Area/Swinging Circle",value:"24" },
                           { name:"Spoil Area/Spoil Ground",value:"25" },
                           { name:"Unsurveyed Area",value:"26" },
                           { name:"Submarine Exercise Area",value:"27" },
                           { name:"Mine Laying Practice Area",value:"28" },
                           { name:"Firing Danger Area",value:"29" },
                           { name:"Dumping Ground for Hazardous Materials",value:"30" },
                           { name:"Incineration Area",value:"31" },
                           { name:"Oil Field",value:"32" },
                           { name:"Gas Field",value:"33" },
                           { name:"Explosive Dumping Ground",value:"35" },
                           { name:"Safety Zone",value:"37" },
                           { name:"Precautionary Area",value:"42" },
                           { name:"Areas to be Avoided",value:"43" },
                           { name:"Degaussing Range",value:"44" },
                           { name:"Outfall Area",value:"45" },
                           { name:"Intake Area",value:"46" },
                           { name:"Pilot Boarding Area",value:"48" },
                           { name:"Cargo Transshipment Area",value:"49" },
                           { name:"Seaplane Anchorage",value:"53" },
                           { name:"Time Limited Anchorage",value:"54" },
                           { name:"Fairway",value:"55" },
                           { name:"Fish Trap Area",value:"56" },
                           { name:"Marine Farm",value:"57" },
                           { name:"Dredging Area",value:"58" },
                           { name:"Sewer Area",value:"61" },
                           { name:"Other",value:"999" },
                        ]
                     },
                     { name:"MBL",
                       desc:"Maritime Boundary Limit",
                       optional:"R",
                       type:"enumeration",
                       defValue:"-32768",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Colregs Demarcation Line",value:"1" },
                           { name:"Customs Boundary",value:"2" },
                           { name:"Harbor Limit",value:"4" },
                           { name:"Territorial Waters - Limit of Sovereignty",value:"6" },
                           { name:"Territorial Waters Baseline",value:"7" },
                           { name:"Clearing Line",value:"10" }
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"OPS",
                       desc:"Operational Status",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Operational",value:"1" },
                           { name:"non-Operational",value:"2" }
                        ]
                     },
                     { name:"PBV",
                       desc:"Pilot Boarding Vehicle",
                       optional:"R",
                       type:"enumeration",
                       defValue:"-32768",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"By Boat",value:"1" }
                        ]
                     },
                     { name:"PRO",
                       desc:"Product Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Ammunition",value:"3" },
                           { name:"Chemical",value:"13" },
                           { name:"Explosives",value:"33" },
                           { name:"Gas",value:"38" },
                           { name:"Gasoline",value:"39" },
                           { name:"Oll",value:"67" },
                           { name:"Radioactive Material",value:"82" },
                           { name:"Water",value:"116" },
                           { name:"None",value:"130" }
                        ]
                     },
                     { name:"TIM",
                       desc:"Time Attribute",
                       optional:"R",
                       type:"Integer",
                       defValue:"-32l68"
                     },
                     { name:"TXT",
                       desc:"Text Attribute",
                       optional:"R",
                       length:"255",
                       type:"String",
                       defValue:"None"
                     },
                     { name:"USE",
                       desc:"Usage",
                       optional:"R",
                       type:"enumeration",
                       defValue:"-32768",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Telegraph",value:"51" },
                           { name:"Telephone",value:"52" },
                           { name:"Power",value:"53" },
                           { name:"Other",value:"999" }
                        ]
                     }
                    ]
          },
        { name:"MARITIML",
          fcode:"",
          desc:"Maritime Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ATN",
                       desc:"Aids to Navigation",
                       optional:"R",
                       type:"enumeration",
                       defValue:"2",
                       enumerations:[
                           { name:"Marked",value:"1" },
                           { name:"Unmarked",value:"2" }
                        ]
                     },
                     { name:"COD",
                       desc:"Certainty of Delineation",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Limits and Info Known",value:"1" },
                           { name:"Limits and Info Unknown",value:"2" }
                        ]
                     },
                     { name:"DAN",
                       desc:"Description of Aids to Navigation",
                       optional:"R",
                       length:"255",
                       type:"String",
                       defValue:"N/A"
                     },
                     { name:"DAT",
                       desc:"Date",
                       optional:"R",
                       type:"Integer",
                       defValue:"26"
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Definite",value:"1" },
                           { name:"Reported",value:"3" }
                        ]
                     },
                     { name:"HDP",
                       desc:"Hydrographic Depth",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:""
                     },
                     { name:"IAS",
                       desc:"IMO Approval Status",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Approved",value:"1" },
                           { name:"Not Approved",value:"2" }
                        ]
                     },
                     { name:"MAC",
                       desc:"Maritime Area Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Dredged Channel/Dregged Area",value:"2" },
                           { name:"Mine Danger Area",value:"4" },
                           { name:"Prohibited Shipping Area/Entry",value:"5" },
                           { name:"Works in Progress Area",value:"9" },
                           { name:"Roundabout Zone (TSS)",value:"40" },
                           { name:"Inshore traffic Zone (TSS)",value:"41" }
                        ]
                     },
                     { name:"MAS",
                       desc:"Maintenance Status",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Maintained",value:"1" },
                           { name:"Not Maintained",value:"2" }
                        ]
                     },
                     { name:"TSP",
                       desc:"Traffic Scheme Part",
                       optional:"R",
                       type:"enumeration",
                       defValue:"-32768",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Outer Boundary",value:"2" }
                        ]
                     },
                     { name:"VAL",
                       desc:"Value",
                       optional:"R",
                       units:"year",
                       type:"Integer",
                       defValue:"0"
                     },
                     { name:"WID",
                       desc:"Width",
                       optional:"R",
                       units:"metre",
                       type:"Integer",
                       defValue:"-2147483648"
                     },
                     { name:"WPC",
                       desc:"Work in Progress Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Land Reclamation",value:"1" },
                           { name:"Construction of Structures",value:"2" }
                        ]
                     }
                  ]
          },
        { name:"ROUTEL",
          fcode:"",
          desc:"Route Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ATN",
                       desc:"Aids to Navigation",
                       optional:"R",
                       type:"enumeration",
                       defValue:"2",
                       enumerations:[
                           { name:"Marked",value:"1" },
                           { name:"Unmarked",value:"2" }
                        ]
                     },
                     { name:"BRR",
                       desc:"Bearing and Reciprocal Category",
                       optional:"R",
                       length:"11",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"BRS",
                       desc:"Bearing from Seaward",
                       optional:"R",
                       units:"degree",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"DAN",
                       desc:"Description of Aids to Navigation",
                       optional:"R",
                       length:"255",
                       type:"String",
                       defValue:"N/A"
                     },
                     { name:"DRP",
                       desc:"Description of Reference Point",
                       optional:"R",
                       length:"30",
                       type:"String",
                       defValue:"N/A"
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"One Way",value:"22" },
                           { name:"Two Way",value:"23" }
                        ]
                     },
                     { name:"HDI",
                       desc:"Hydrographic Depth/Height Information",
                       optional:"R",
                       type:"enumeration",
                       defValue:"12",
                       enumerations:[
                           { name:"Depth known by Other than Wire",value:"9" },
                           { name:"Depth Known",value:"12" }
                        ]
                     },
                     { name:"HDP",
                       desc:"Hydrographic Depth",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:""
                     },
                     { name:"RTT",
                       desc:"Route Intended Use",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Recommended track for other than deep draft vessels",value:"2" },
                           { name:"Recommended track for deep draft vessels",value:"3" },
                           { name:"Transit Route",value:"5" }
                        ]
                     }
                    ]
          },
        { name:"SEPARTNL",
          fcode:"",
          desc:"Separation Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Definite",value:"1" }
                        ]
                     },
                     { name:"IAS",
                       desc:"IMO Approval Status",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Approved",value:"1" },
                           { name:"Not Approved",value:"2" }
                        ]
                     },
                     { name:"OPS",
                       desc:"Operational Status",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Operational",value:"1" },
                           { name:"non-Operational",value:"2" }
                        ]
                     },
                     { name:"TSP",
                       desc:"Traffic Scheme Part",
                       optional:"R",
                       type:"enumeration",
                       defValue:"4",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Outer Boundary",value:"2" },
                           { name:"Separation Zone Line",value:"4" }
                        ]
                     },
                     { name:"WID",
                       desc:"Width",
                       optional:"R",
                       units:"metre",
                       type:"Integer",
                       defValue:"-21474883648"
                     }
                    ]
          },
        { name:"SWEPTL",
          fcode:"",
          desc:"Swept Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"DAT",
                       desc:"Date",
                       optional:"R",
                       type:"Integer",
                       defValue:"26"
                     },
                     { name:"HDP",
                       desc:"Hydrographic Depth",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"VAL",
                       desc:"Value",
                       optional:"R",
                       units:"year",
                       type:"Integer",
                       defValue:"0"
                     }
                    ]
          },
        { name:"LIMBNDYP",
          fcode:"",
          desc:"Limit Boundaries Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"COD",
                       desc:"Certainty of Delineation",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Limits and Info Known",value:"1" },
                           { name:"Limits and Info Unknown",value:"2" }
                        ]
                     },
                     { name:"MAC",
                       desc:"Maritime Area Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Anchorage (general)",value:"11" },
                           { name:"Anchoring Berths",value:"12" },
                           { name:"Explosive Anchorage",value:"13" },
                           { name:"Large Vessel/Deepwater/Deep Draft",value:"14" },
                           { name:"Anchoring Prohibited",value:"15" },
                           { name:"Quarantine Anchorage",value:"16" },
                           { name:"Reserved Anchorage",value:"17" },
                           { name:"Tanker Anchorage",value:"19" },
                           { name:"Fishing Prohibited",value:"22" },
                           { name:"Safety Zone",value:"37" },
                           { name:"Areas to be Avoided",value:"43" },
                           { name:"Pilot Boarding Area",value:"48" },
                           { name:"Seaplane Anchorage",value:"53" },
                           { name:"Time Limited Anchorage",value:"54" },
                           { name:"Other",value:"999" },
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"PBV",
                       desc:"Pilot Boarding Vehicle",
                       optional:"R",
                       type:"enumeration",
                       defValue:"-32768",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"By Boat",value:"1" },
                           { name:"By Helecopter",value:"2" }
                        ]
                     },
                     { name:"TIM",
                       desc:"Time Attribute",
                       optional:"R",
                       type:"Integer",
                       defValue:"-32l68"
                     },
                     { name:"TXT",
                       desc:"Text Attribute",
                       optional:"R",
                       length:"255",
                       type:"String",
                       defValue:"None"
                     }
                    ]
          },
        { name:"MARITIMP",
          fcode:"",
          desc:"Maritime Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Definite",value:"1" },
                           { name:"Reported",value:"3" }
                        ]
                     },
                     { name:"IAS",
                       desc:"IMO Approval Status",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Approved",value:"1" },
                           { name:"Not Approved",value:"2" }
                        ]
                     },
                     { name:"MAC",
                       desc:"Maritime Area Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Dredged Channel/Dregged Area",value:"2" },
                           { name:"Mine Danger Area",value:"4" },
                           { name:"Prohibited Shipping Area/Entry",value:"5" },
                           { name:"Roundabout Zone (TSS)",value:"40" },
                        ]
                     },
                     { name:"MAS",
                       desc:"Maintenance Status",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Maintained",value:"1" },
                        ]
                     },
                     { name:"TSP",
                       desc:"Traffic Scheme Part",
                       optional:"R",
                       type:"enumeration",
                       defValue:"-32768",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Arrow",value:"1" },
                           { name:"Separation Zone Point",value:"5" }
                        ]
                     }
                  ]
          },
        { name:"ROUTEP",
          fcode:"",
          desc:"Route Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"DOF",
                       desc:"Direction of Flow",
                       optional:"R",
                       units:"degree",
                       type:"Integer",
                       defValue:"0"
                     },
                     { name:"RTT",
                       desc:"Route Intended Use",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Recommended Direction of Traffic Flow",value:"13" }
                        ]
                     }
                    ]
          },
        { name:"SEPARTNP",
          fcode:"",
          desc:"Separation Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"DOF",
                       desc:"Direction of Flow",
                       optional:"R",
                       units:"degree",
                       type:"Integer",
                       defValue:"0"
                     },
                     { name:"IAS",
                       desc:"IMO Approval Status",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Approved",value:"1" },
                           { name:"Not Approved",value:"2" }
                        ]
                     },
                     { name:"TSP",
                       desc:"Traffic Scheme Part",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Arrow",value:"1" }
                        ]
                     }
                    ]
          },
        { name:"SECTORA",
          fcode:"",
          desc:"Sector Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"COL",
                       desc:"Character of Light",
                       optional:"R",
                       length:"10",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"LSA",
                       desc:"Light Sector Angle",
                       optional:"R",
                       length:"11",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                    ]
          },
        { name:"LEADINGL",
          fcode:"",
          desc:"Leading Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"BRG",
                       desc:"Bearing of object",
                       optional:"R",
                       units:"degree",
                       type:"Real",
                       defValue:""
                     },
                     { name:"LAF",
                       desc:"Line Associated Features",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"One Object (Other Than a Directional Light)",value:"1" },
                           { name:"Directional Light",value:"2" },
                           { name:"Two or More Lights",value:"3" },
                           { name:"Two or More Beacons",value:"4" },
                           { name:"2 or More Objects (Other Than 2 Lights or 2 Beacons)",value:"5" },
                           { name:"Directional Radiobeacon",value:"7" },
                           { name:"Moire Effect Light",value:"8" }
                        ]
                     }
                    ]
          },
        { name:"LIGHTSL",
          fcode:"",
          desc:"Lights Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"HLT",
                       desc:"Hydrographic Light Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"4",
                       enumerations:[
                           { name:"Strip Light",value:"4" }
                        ]
                     }
                    ]
          },
        { name:"BUOYBCNP",
          fcode:"",
          desc:"Buoy/Beacon Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" },
                           { name:"Doubtful",value:"3" },
                        ]
                     },
                     { name:"BRF",
                       desc:"Broadcast Frequency",
                       optional:"R",
                       units:"Hertz",
                       type:"Integer",
                       defValue:"-2147483648"
                     },
                     { name:"BR2",
                       desc:"Broadcast Frequency 2",
                       optional:"R",
                       units:"Hertz",
                       type:"Integer",
                       defValue:"-2147483648"
                     },
                     { name:"BTC",
                       desc:"Beacon/Buoy Type Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Large Navigational Buoy (LANBY)",value:"4" },
                           { name:"Light Float",value:"6" },
                           { name:"Mooring",value:"7" },
                           { name:"Ocean Data Acquisition System (ODAS)",value:"10" },
                           { name:"Articulated Lights",value:"35" }
                        ]
                     },
                     { name:"CCC",
                       desc:"Color Code Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Black (B)",value:"1" },
                           { name:"Blue (Bu)",value:"2" },
                           { name:"Brown (Br)",value:"3" },
                           { name:"Gray (Gy)",value:"4" },
                           { name:"Green (G)",value:"5" },
                           { name:"Orange (Or)",value:"9" },
                           { name:"Red (R)",value:"12" },
                           { name:"Violet (Vi)",value:"14" },
                           { name:"White (W)",value:"15" },
                           { name:"Yellow (Y)",value:"19" },
                           { name:"Red & White (RW)",value:"20" },
                           { name:"Red & Green (RG)",value:"21" },
                           { name:"Red & Black (RB)",value:"22" },
                           { name:"Red-Green-Red (RGR)",value:"23" },
                           { name:"Green & White (GW)",value:"24" },
                           { name:"Green & Red (GR)",value:"25" },
                           { name:"Green & Black (GB)",value:"26" },
                           { name:"Green-Red-Green (GRG)",value:"27" },
                           { name:"Green-Yellow-Black (GYB)",value:"28" },
                           { name:"Yellow & Black (YB)",value:"29" },
                           { name:"Yellow-Black-Yellow (YBY)",value:"30" },
                           { name:"Yellow & Red (YR)",value:"31" },
                           { name:"Yellow & Green (YG)",value:"32" },
                           { name:"Yellow-Red-White (YRW)",value:"33" },
                           { name:"Black & Yellow (BY)",value:"34" },
                           { name:"Black-Yellow-Black (BYB)",value:"35" },
                           { name:"Black-Red-Black (BRB)",value:"36" },
                           { name:"Black & White (BW)",value:"37" },
                           { name:"Black & Red (BR)",value:"38" },
                           { name:"Black & Green (BG)",value:"39" },
                           { name:"White & Red (WR)",value:"40" },
                           { name:"White & Orange (W Or)",value:"41" },
                           { name:"White & Green (WG)",value:"42" },
                           { name:"White & Black (WB)",value:"43" },
                           { name:"White & Yellow (WY)",value:"44" },
                           { name:"White-Red-Green (WRG)",value:"45" },
                           { name:"White-Green-White (WGW)",value:"46" },                        ]
                     },
                     { name:"COL",
                       desc:"Character of Light",
                       optional:"R",
                       length:"10",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"EOL",
                       desc:"Elevation of Light",
                       optional:"R",
                       units:"metre",
                       type:"Integer",
                       defValue:"-32768"
                     },
                     { name:"LVN",
                       desc:"Light Range Nominal",
                       optional:"R",
                       units:"nautical mile",
                       type:"Integer",
                       defValue:"-32768"
                     },
                     { name:"MLR",
                       desc:"Multiple Light Ranges",
                       optional:"R",
                       length:"10",
                       type:"String",
                       defValue:"N/A"
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"NST",
                       desc:"Navigation System Types",
                       optional:"R",
                       type:"enumeration",
                       defValue:"52",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Circular Radiobeacon (RC)",value:"1" },
                           { name:"Radio Direction Finding",value:"4" },
                           { name:"Directional Radiobeacon (RD)",value:"5" },
                           { name:"Racon (Radar Responder Beacon)",value:"10" },
                           { name:"NDB (Non-Directional Beacon)",value:"17" },
                           { name:"Rotating Loop Radiobeacon",value:"41" },
                           { name:"Radar Station (Ra)",value:"45" },
                           { name:"Radiobeacon Type Unknown (R Bn)",value:"51" },
                           { name:"None",value:"52" },
                           { name:"QTG Station (R)",value:"53" },
                           { name:"Ramark (Ramark)",value:"54" },
                        ]
                     },
                     { name:"NS2",
                       desc:"Navigation System Types 2",
                       optional:"R",
                       type:"enumeration",
                       defValue:"52",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Circular Radiobeacon (RC)",value:"1" },
                           { name:"Radio Direction Finding",value:"4" },
                           { name:"Directional Radiobeacon (RD)",value:"5" },
                           { name:"Racon (Radar Responder Beacon)",value:"10" },
                           { name:"NDB (Non-Directional Beacon)",value:"17" },
                           { name:"Rotating Loop Radiobeacon",value:"41" },
                           { name:"Radar Station (Ra)",value:"45" },
                           { name:"Radiobeacon Type Unknown (R Bn)",value:"51" },
                           { name:"None",value:"52" },
                           { name:"QTG Station (R)",value:"53" },
                           { name:"Ramark (Ramark)",value:"54" },
                        ]
                     },
                     { name:"ORC",
                       desc:"Operating Range Category",
                       optional:"R",
                       units:"nautical miles",
                       type:"Integer",
                       defValue:"-32768"
                     },
                     { name:"OR2",
                       desc:"Operating Range Category 2",
                       optional:"R",
                       units:"nautical miles",
                       type:"Integer",
                       defValue:"-32768"
                     },
                     { name:"PER",
                       desc:"Period of Light",
                       optional:"R",
                       units:"second",
                       type:"Real",
                       defValue:""
                     },
                     { name:"REF",
                       desc:"Radar Reflector Present",
                       optional:"R",
                       type:"enumeration",
                       defValue:"2",
                       enumerations:[
                           { name:"Radar Reflector Present",value:"1" },
                           { name:"Radar Reflector Absent",value:"2" }
                        ]
                     },
                     { name:"SSC",
                       desc:"Structure Shape Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Barrel,Ton",value:"1" },
                           { name:"Conical/Peaked/NUN",value:"6" },
                           { name:"Cylindrical (Upright)/CAN",value:"7" },
                           { name:"Pillar, Spindle",value:"10" },
                           { name:"Spar",value:"16" },
                           { name:"Spherical (Hemispherical)",value:"17" },
                           { name:"Superbouy",value:"73" },
                           { name:"Diamond-shaped Buoy",value:"85" },
                           { name:"Other",value:"999" }
                        ]
                     },
                     { name:"SST",
                       desc:"Sound Signal Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"16",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown", value:"0" },
                           { name:"Bell", value:"1" },
                           { name:"Diaphone", value:"2" },
                           { name:"Explosive fog signal", value:"3" },
                           { name:"Gong", value:"4" },
                           { name:"Horn", value:"6" },
                           { name:"Siren", value:"9" },
                           { name:"Whistle", value:"14" },
                           { name:"Reed", value:"15" },
                           { name:"None", value:"16" }
                        ]
                     },
                     { name:"TMC",
                       desc:"Topmark characteristic",
                       optional:"R",
                       type:"enumeration",
                       defValue:"33",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"East Mark (2 cones base together)",value:"1" },
                           { name:"Isolated Danger (2 balls)",value:"2" },
                           { name:"North Mark (2 cones pointing up)",value:"3" },
                           { name:"Special (X)",value:"6" },
                           { name:"South Mark (2 cones pointing down)",value:"8" },
                           { name:"West Mark (2 cones points together)",value:"9" },
                           { name:"Ball Over Cone",value:"15" },
                           { name:"Cone Over Ball",value:"16" },
                           { name:"Broom Point Up",value:"17" },
                           { name:"Perch",value:"18" },
                           { name:"Diamond",value:"19" },
                           { name:"Cone Point Upwards",value:"21" },
                           { name:"Cone (Point Downwards)",value:"22" },
                           { name:"Upright Cross",value:"23" },
                           { name:"Can (Open)",value:"25" },
                           { name:"Can (Filled)",value:"26" },
                           { name:"Ball (Open)",value:"27" },
                           { name:"Ball (Filled)",value:"28" },
                           { name:"Can Over Ball (Open)",value:"29" },
                           { name:"Cross Over Ball (Open)",value:"30" },
                           { name:"Diamond Over Ball (Filled)",value:"31" },
                           { name:"Double Cone Points Apart (Open)",value:"32" },
                           { name:"None",value:"33" },
                        ]
                     },
                     { name:"TXT",
                       desc:"Text Attribute",
                       optional:"R",
                       length:"255",
                       type:"String",
                       defValue:"None"
                     },
                    ]
          },
        { name:"LIGHTSP",
          fcode:"",
          desc:"Lights Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" },
                           { name:"Doubtful",value:"3" },
                        ]
                     },
                     { name:"BRF",
                       desc:"Broadcast Frequency",
                       optional:"R",
                       units:"Hertz",
                       type:"Integer",
                       defValue:"-2147483648"
                     },
                     { name:"BR2",
                       desc:"Broadcast Frequency 2",
                       optional:"R",
                       units:"Hertz",
                       type:"Integer",
                       defValue:"-2147483648"
                     },
                     { name:"CCC",
                       desc:"Color Code Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Black (B)",value:"1" },
                           { name:"Blue (Bu)",value:"2" },
                           { name:"Brown (Br)",value:"3" },
                           { name:"Gray (Gy)",value:"4" },
                           { name:"Green (G)",value:"5" },
                           { name:"Orange (Or)",value:"9" },
                           { name:"Red (R)",value:"12" },
                           { name:"Violet (Vi)",value:"14" },
                           { name:"White (W)",value:"15" },
                           { name:"Yellow (Y)",value:"19" },
                           { name:"Red & White (RW)",value:"20" },
                           { name:"Red & Green (RG)",value:"21" },
                           { name:"Red & Black (RB)",value:"22" },
                           { name:"Red-Green-Red (RGR)",value:"23" },
                           { name:"Green & White (GW)",value:"24" },
                           { name:"Green & Red (GR)",value:"25" },
                           { name:"Green & Black (GB)",value:"26" },
                           { name:"Green-Red-Green (GRG)",value:"27" },
                           { name:"Green-Yellow-Black (GYB)",value:"28" },
                           { name:"Yellow & Black (YB)",value:"29" },
                           { name:"Yellow-Black-Yellow (YBY)",value:"30" },
                           { name:"Yellow & Red (YR)",value:"31" },
                           { name:"Yellow & Green (YG)",value:"32" },
                           { name:"Yellow-Red-White (YRW)",value:"33" },
                           { name:"Black & Yellow (BY)",value:"34" },
                           { name:"Black-Yellow-Black (BYB)",value:"35" },
                           { name:"Black-Red-Black (BRB)",value:"36" },
                           { name:"Black & White (BW)",value:"37" },
                           { name:"Black & Red (BR)",value:"38" },
                           { name:"Black & Green (BG)",value:"39" },
                           { name:"White & Red (WR)",value:"40" },
                           { name:"White & Orange (W Or)",value:"41" },
                           { name:"White & Green (WG)",value:"42" },
                           { name:"White & Black (WB)",value:"43" },
                           { name:"White & Yellow (WY)",value:"44" },
                           { name:"White-Red-Green (WRG)",value:"45" },
                           { name:"White-Green-White (WGW)",value:"46" },                        ]
                     },
                     { name:"COL",
                       desc:"Character of Light",
                       optional:"R",
                       length:"10",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"EOL",
                       desc:"Elevation of Light",
                       optional:"R",
                       units:"metre",
                       type:"Integer",
                       defValue:"-32768"
                     },
                     { name:"HLT",
                       desc:"Hydrographic Light Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Sectored Light",value:"1" },
                           { name:"Other",value:"2" },
                           { name:"Moire Effect Light",value:"3" },
                           { name:"Occasional",value:"5" }
                        ]
                     },
                     { name:"IAC",
                       desc:"IALA Aid Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Non-IALA Aid",value:"1" },
                           { name:"IALA Aid",value:"2" }
                        ]
                     },
                     { name:"LVN",
                       desc:"Light Range Nominal",
                       optional:"R",
                       units:"nautical mile",
                       type:"Integer",
                       defValue:"0"
                     },
                     { name:"MLR",
                       desc:"Multiple Light Ranges",
                       optional:"R",
                       length:"10",
                       type:"String",
                       defValue:"N/A"
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"NST",
                       desc:"Navigation System Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Circular Radiobeacon (RC)",value:"1" },
                           { name:"Radio Direction Finding",value:"4" },
                           { name:"Directional Radiobeacon (RD)",value:"5" },
                           { name:"Racon (Radar Responder Beacon)",value:"10" },
                           { name:"NDB (Non-Directional Beacon)",value:"17" },
                           { name:"Rotating Loop Radiobeacon",value:"41" },
                           { name:"Radar Station (Ra)",value:"45" },
                           { name:"Radiobeacon Type Unknown (R Bn)",value:"51" },
                           { name:"None",value:"52" },
                           { name:"QTG Station (R)",value:"53" },
                           { name:"Ramark (Ramark)",value:"54" },

                        ]
                     },
                     { name:"NS2",
                       desc:"Navigation System Type 2",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Circular Radiobeacon (RC)",value:"1" },
                           { name:"Radio Direction Finding",value:"4" },
                           { name:"Directional Radiobeacon (RD)",value:"5" },
                           { name:"Racon (Radar Responder Beacon)",value:"10" },
                           { name:"NDB (Non-Directional Beacon)",value:"17" },
                           { name:"Rotating Loop Radiobeacon",value:"41" },
                           { name:"Radar Station (Ra)",value:"45" },
                           { name:"Radiobeacon Type Unknown (R Bn)",value:"51" },
                           { name:"None",value:"52" },
                           { name:"QTG Station (R)",value:"53" },
                           { name:"Ramark (Ramark)",value:"54" }
                        ]
                     },
                     { name:"PER",
                       desc:"Period of Light",
                       optional:"R",
                       units:"second",
                       type:"Real",
                       defValue:""
                     },
                     { name:"REF",
                       desc:"Radar Reflector Present",
                       optional:"R",
                       type:"enumeration",
                       defValue:"2",
                       enumerations:[
                           { name:"Radar Reflector Present",value:"1" },
                           { name:"Radar Reflector Absent",value:"2" }
                        ]
                     },
                     { name:"SST",
                       desc:"Sound Signal Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"16",
                       enumerations:[
                           { name:"Unknown", value:"0" },
                           { name:"Bell", value:"1" },
                           { name:"Diaphone", value:"2" },
                           { name:"Explosive fog signal", value:"3" },
                           { name:"Gong", value:"4" },
                           { name:"Horn", value:"6" },
                           { name:"Siren", value:"9" },
                           { name:"Whistle", value:"14" },
                           { name:"Reed", value:"15" },
                           { name:"None", value:"16" }
                        ]
                     },
                     { name:"TMC",
                       desc:"Topmark characteristic",
                       optional:"R",
                       type:"enumeration",
                       defValue:"33",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"East Mark (2 cones base together)",value:"1" },
                           { name:"Isolated Danger (2 balls)",value:"2" },
                           { name:"North Mark (2 cones pointing up)",value:"3" },
                           { name:"Special (X)",value:"6" },
                           { name:"South Mark (2 cones pointing down)",value:"8" },
                           { name:"West Mark (2 cones points together)",value:"9" },
                           { name:"Ball Over Cone",value:"15" },
                           { name:"Cone Over Ball",value:"16" },
                           { name:"Broom Point Up",value:"17" },
                           { name:"Perch",value:"18" },
                           { name:"Diamond",value:"19" },
                           { name:"Cone Point Upwards",value:"21" },
                           { name:"Cone (Point Downwards)",value:"22" },
                           { name:"Upright Cross",value:"23" },
                           { name:"Can (Open)",value:"25" },
                           { name:"Can (Filled)",value:"26" },
                           { name:"Ball (Open)",value:"27" },
                           { name:"Ball (Filled)",value:"28" },
                           { name:"Can Over Ball (Open)",value:"29" },
                           { name:"Cross Over Ball (Open)",value:"30" },
                           { name:"Diamond Over Ball (Filled)",value:"31" },
                           { name:"Double Cone Points Apart (Open)",value:"32" },
                           { name:"None",value:"33" },
                        ]
                     },
                     { name:"TXT",
                       desc:"Text Attribute",
                       optional:"R",
                       length:"255",
                       type:"String",
                       defValue:"None"
                     },
                    ]
          },
        { name:"MARKERP",
          fcode:"",
          desc:"Marker Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"USE",
                       desc:"Useage",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Marine",value:"57" }
                        ]
                     },
                    ]
          },
        { name:"BRIDGEA",
          fcode:"",
          desc:"Bridge Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"BOT",
                       desc:"Bridge Opening Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown", value:"0" },
                           { name:"Draw/Bascule", value:"4" },
                           { name:"Swing", value:"10" },
                           { name:"Lift", value:"11" },
                           { name:"Retractable", value:"12" },
                           { name:"Not Applicable/Fixed", value:"13" }
                        ]
                     },
                     { name:"BSC",
                       desc:"Bridge Structure Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown", value:"0" },
                           { name:"Arch", value:"1" },
                           { name:"Cantilever", value:"2" },
                           { name:"Deck", value:"3" },
                           { name:"Floating Bridge / Pontoon", value:"5" },
                           { name:"Girder", value:"6" },
                           { name:"Truss", value:"8" },
                           { name:"Suspension", value:"9" },
                           { name:"Transporter", value:"12" },
                           { name:"Slab", value:"15" },
                           { name:"Stringer", value:"16" },
                           { name:"Other", value:"999" }
                        ]
                     },
                     { name:"DAT",
                       desc:"Date",
                       optional:"R",
                       type:"Integer",
                       defValue:"26"
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"28",
                       enumerations:[
                           { name:"Under Construction",value:"5" },
                           { name:"Destroyed",value:"7" },
                           { name:"Operational",value:"28" }
                        ]
                     },
                     { name:"MVC",
                       desc:"Maximum Vertical Clearance",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"OHC",
                       desc:"Overhead Clearance Category",
                       optional:"R",
                       type:"Real",
                       defValue:"0.0",
                     },
                     { name:"OWO",
                       desc:"Over Water Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Feature crosses navigable water",value:"1" },
                           { name:"Feature does not cross navigable water",value:"2" }
                        ]
                     },
                     { name:"SHC",
                       desc:"Safe Horizontal Clearance",
                       optional:"R",
                       type:"Real",
                       defValue:"0.0",
                     },
                     { name:"VAL",
                       desc:"Value",
                       optional:"R",
                       units:"year",
                       type:"Integer",
                       defValue:"0"
                     },
                    ]
          },
        { name:"BRIDGSPA",
          fcode:"",
          desc:"Bridge Span Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:"AQ045"
                     },
                     { name:"BSM",
                       desc:"Bridge Span Mobility",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Mobile Span",value:"1" }
                        ]
                     }
                    ]
          },
        { name:"DANGERA",
          fcode:"",
          desc:"Danger Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" },
                           { name:"Doubtful",value:"3" }
                        ]
                     },
                     { name:"DAT",
                       desc:"Date",
                       optional:"R",
                       type:"Integer",
                       defValue:"26"
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Definite",value:"1" },
                           { name:"Doubtful",value:"2" },
                           { name:"Reported",value:"3" },
                        ]
                     },
                     { name:"HDI",
                       desc:"Hydrographic Depth/Height Information",
                       optional:"R",
                       type:"enumeration",
                       defValue:"12",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Depth known by Other than Wire",value:"9" },
                           { name:"Depth Unknown",value:"12" }
                        ]
                     },
                     { name:"HDP",
                       desc:"Hydrographic Depth",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"VAL",
                       desc:"Value",
                       optional:"R",
                       units:"year",
                       type:"Integer",
                       defValue:"0"
                     },
                     { name:"VRR",
                       desc:"Vertical Reference Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Above surface / Does Not Cover (At High Water)",value:"1" },
                           { name:"Below Surface / Submerged",value:"4" },
                           { name:"Covers and Uncovers",value:"8" }
                        ]
                     }
                    ]
          },
        { name:"FISHHATA",
          fcode:"",
          desc:"Fish Hatchery Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:"BH050"
                     }
                    ]
          },
        { name:"HAZARDA",
          fcode:"",
          desc:"Hazard Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" },
                           { name:"Doubtful",value:"3" }
                        ]
                     },
                     { name:"COD",
                       desc:"Certainty of Delineation",
                       optional:"R",
                       type:"enumeration",
                       defValue:"2",
                       enumerations:[
                           { name:"Limits and Info Unknown",value:"2" }
                        ]
                     },
                     { name:"DAT",
                       desc:"Date",
                       optional:"R",
                       type:"Integer",
                       defValue:"26"
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Definite",value:"1" },
                           { name:"Doubtful",value:"2" },
                           { name:"Reported",value:"3" },
                        ]
                     },
                     { name:"HDI",
                       desc:"Hydrographic Depth/Height Information",
                       optional:"R",
                       type:"enumeration",
                       defValue:"12",
                       enumerations:[
                           { name:"Depth known by Other than Wire",value:"9" },
                           { name:"Depth known by Wire Drag",value:"10" },
                           { name:"Depth unknown but safe to depth shown",value:"11" },
                           { name:"Depth Unknown",value:"12" },
                           { name:"Not Applicable",value:"15" }
                        ]
                     },
                     { name:"HDP",
                       desc:"Hydrographic Depth",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"LOC",
                       desc:"Location Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"8",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Below Surface / Submerged / Underground",value:"4" },
                           { name:"Hull Showing",value:"13" },
                           { name:"Masts Showing",value:"14" },
                           { name:"Funnel Showing",value:"20" },
                           { name:"Superstructure Showing",value:"21" },
                           { name:"Masts and Funnel Showing",value:"28" }
                        ]
                     },
                     { name:"SFC",
                       desc:"Sea Floor Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown Obstruction",value:"1" },
                           { name:"Other",value:"2" },
                           { name:"Fish Haven",value:"3" }
                        ]
                     },
                     { name:"SOH",
                       desc:"Severity of Hazard",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Dangerous",value:"1" },
                           { name:"Non-Dangerous",value:"2" }
                        ]
                     },
                     { name:"TXT",
                       desc:"Text Attribute",
                       optional:"R",
                       length:"255",
                       type:"String",
                       defValue:"None"
                     },
                     { name:"VAL",
                       desc:"Value",
                       optional:"R",
                       units:"year",
                       type:"Integer",
                       defValue:"0"
                     },
                     { name:"VRR",
                       desc:"Vertical Reference Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"4",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Above surface / Does Not Cover (At High Water)",value:"1" },
                           { name:"Awash at Sounding Datum",value:"2" },
                           { name:"Below Surface / Submerged",value:"4" },
                           { name:"Covers and Uncovers",value:"8" }
                        ]
                     }
                    ]
          },
        { name:"LOADINGA",
          fcode:"",
          desc:"Loading Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                    ]
          },
        { name:"OBSTRUCA",
          fcode:"",
          desc:"Obstruction Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                    ]
          },
        { name:"REEFA",
          fcode:"",
          desc:"Reef Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" },
                           { name:"Doubtful",value:"3" },
                        ]
                     },
                     { name:"COD",
                       desc:"Certainty of Delineation",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Limits and Info Known",value:"1" },
                           { name:"Limits and Info Unknown",value:"2" }
                        ]
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Definite",value:"1" },
                           { name:"Doubtful",value:"2" },
                           { name:"Reported",value:"3" }
                        ]
                     },
                     { name:"HDH",
                       desc:"Hydrographic Drying Height",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:""
                     },
                     { name:"HDI",
                       desc:"Hydrographic Depth/Height Information",
                       optional:"R",
                       type:"enumeration",
                       defValue:"12",
                       enumerations:[
                           { name:"Depth known by Other than Wire",value:"9" },
                           { name:"Depth Unknown",value:"12" },
                           { name:"Uncovering Height Known",value:"13" },
                           { name:"Uncovering height unknown",value:"14" }
                        ]
                     },
                     { name:"HDP",
                       desc:"Hydrographic Depth",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"MCC",
                       desc:"Material Composition Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown", value:"0" },
                           { name:"Coral", value:"24" },
                           { name:"Rock/Rocky", value:"84" }
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"SOH",
                       desc:"Severity of Hazard",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Dangerous",value:"1" },
                           { name:"Non-Dangerous",value:"2" }
                        ]
                     },
                     { name:"VAL",
                       desc:"Value",
                       optional:"R",
                       units:"year",
                       type:"Integer",
                       defValue:"0"
                     },
                     { name:"VRR",
                       desc:"Vertical Reference Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"4",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Awash at Sounding Datum",value:"2" },
                           { name:"Below Surface / Submerged",value:"4" },
                           { name:"Covers and Uncovers",value:"8" }
                        ]
                     }
                    ]
          },
        { name:"RUINSA",
          fcode:"",
          desc:"Ruins Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"HDH",
                       desc:"Hydrographic Drying Height",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:""
                     },
                     { name:"HDI",
                       desc:"Hydrographic Depth/Height Information",
                       optional:"R",
                       type:"enumeration",
                       defValue:"12",
                       enumerations:[
                           { name:"Depth known by Other than Wire",value:"9" },
                           { name:"Depth known by Wire Drag",value:"10" },
                           { name:"Depth Unknown",value:"12" },
                           { name:"Not Applicable",value:"15" }
                        ]
                     },
                     { name:"HDP",
                       desc:"Hydrographic Depth",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ]
                     },
                     { name:"HGT",
                       desc:"Height Above Surface Level",
                       optional:"R",
                       units:"Metre",
                       type:"Integer",
                       defValue:"0"
                     },
                     { name:"LOC",
                       desc:"Location Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"22",
                       enumerations:[
                           { name:"On ground surface",value:"8" },
                           { name:"Off Shore",value:"22" },
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"VRR",
                       desc:"Vertical Reference Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"4",
                       enumerations:[
                           { name:"Above surface / Does Not Cover (At High Water)",value:"1" },
                           { name:"Below Surface / Submerged",value:"4" },
                           { name:"Covers and Uncovers",value:"8" },
                           { name:"Not Applicable",value:"9" }
                        ]
                     }
                    ]
          },
        { name:"BRIDGEL",
          fcode:"",
          desc:"Bridge Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"BOT",
                       desc:"Bridge Opening Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown", value:"0" },
                           { name:"Draw/Bascule", value:"4" },
                           { name:"Swing", value:"10" },
                           { name:"Lift", value:"11" },
                           { name:"Retractable", value:"12" },
                           { name:"Not Applicable/Fixed", value:"13" }
                        ]
                     },
                     { name:"BSC",
                       desc:"Bridge Structure Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown", value:"0" },
                           { name:"Arch", value:"1" },
                           { name:"Cantilever", value:"2" },
                           { name:"Deck", value:"3" },
                           { name:"Floating Bridge / Pontoon", value:"5" },
                           { name:"Girder", value:"6" },
                           { name:"Truss", value:"8" },
                           { name:"Suspension", value:"9" },
                           { name:"Transporter", value:"12" },
                           { name:"Slab", value:"15" },
                           { name:"Stringer", value:"16" },
                           { name:"Other", value:"999" }
                        ]
                     },
                     { name:"DAT",
                       desc:"Date",
                       optional:"R",
                       type:"Integer",
                       defValue:"26"
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"28",
                       enumerations:[
                           { name:"Under Construction",value:"5" },
                           { name:"Operational",value:"28" }
                        ]
                     },
                     { name:"MVC",
                       desc:"Maximum Vertical Clearance",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"OHC",
                       desc:"Overhead Clearance Category",
                       optional:"R",
                       type:"Real",
                       defValue:"0.0",
                     },
                     { name:"OWO",
                       desc:"Over Water Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Feature crosses navigable water",value:"1" }
                        ]
                     },
                     { name:"SHC",
                       desc:"Safe Horizontal Clearance",
                       optional:"R",
                       type:"Real",
                       defValue:"0.0",
                     },
                     { name:"TUC",
                       desc:"Transportation Use Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown", value:"0" },
                           { name:"Both Road and Railroad", value:"1" },
                           { name:"Railroad", value:"3" },
                           { name:"Road", value:"4" },
                           { name:"Pedestrian", value:"17" }
                        ]
                     },
                     { name:"VAL",
                       desc:"Value",
                       optional:"R",
                       units:"year",
                       type:"Integer",
                       defValue:"0"
                     },
                    ]
          },
        { name:"BRIDGSPL",
          fcode:"",
          desc:"Bridge Span Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:"AQ045"
                     },
                     { name:"BSM",
                       desc:"Bridge Span Mobility",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Mobile Span",value:"1" }
                        ]
                     }
                    ]
          },
        { name:"HAZARDL",
          fcode:"",
          desc:"Hazard Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"28",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Abandoned/Disused",value:"6" },
                           { name:"Operational",value:"28" }
                        ]
                     },
                     { name:"LOC",
                       desc:"Location Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"4",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Below Surface / Submerged / Underground",value:"4" }
                        ]
                     },
                     { name:"USE",
                       desc:"Usage",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Telegraph",value:"51" },
                           { name:"Telephone",value:"52" },
                           { name:"Power",value:"53" }
                        ]
                     }
                    ]
          },
        { name:"PIPELINL",
          fcode:"",
          desc:"Pipeline Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"DEP",
                       desc:"Depth Below Surface Level",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:""
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"28",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Under Construction",value:"5" },
                           { name:"Abandoned/Disused",value:"6" },
                           { name:"Operational",value:"28" },
                           { name:"Other",value:"999" }
                        ]
                     },
                     { name:"HSB",
                       desc:"Height above sea bottom",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:""
                     },
                     { name:"LOC",
                       desc:"Location Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"On ground surface",value:"8" },
                           { name:"Sunken/On sea bottom",value:"17" },
                           { name:"Below sea bottom",value:"23" },
                           { name:"Suspended or elevated above sea bottom",value:"24" },
                           { name:"Suspended / Elevated Above Ground or Water Surface",value:"25" },
                        ]
                     },
                     { name:"OHC",
                       desc:"Overhead Clearance Category",
                       optional:"R",
                       type:"Real",
                       defValue:"0.0",
                     },
                     { name:"OWO",
                       desc:"Over Water Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Feature crosses navigable water",value:"1" },
                           { name:"Feature does not cross navigable water",value:"2" }
                        ]
                     },
                     { name:"PLT",
                       desc:"Pipeline Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Transport",value:"1" },
                           { name:"Outfall",value:"2" },
                           { name:"Intake",value:"3" },
                        ]
                     },
                     { name:"PRO",
                       desc:"Product",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Chemical",value:"13" },
                           { name:"Gas",value:"38" },
                           { name:"Gasoline",value:"39" },
                           { name:"Oll",value:"67" },
                           { name:"Sewage",value:"95" },
                           { name:"Water",value:"116" },
                           { name:"Other",value:"999" },
                        ]
                     },
                    ]
          },
        { name:"TUNNELL",
          fcode:"",
          desc:"Tunnel Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"HDP",
                       desc:"Hydrographic Depth",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"TUC",
                       desc:"Transportation Use Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown", value:"0" },
                           { name:"Both Road and Railroad", value:"1" },
                           { name:"Railroad", value:"3" },
                           { name:"Road", value:"4" },
                           { name:"Aqueduct", value:"37" },
                           { name:"Canal", value:"38" }
                        ]
                     },
                    ]
          },
        { name:"DANGERP",
          fcode:"",
          desc:"Danger Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" },
                           { name:"Doubtful",value:"3" }
                        ]
                     },
                     { name:"DAT",
                       desc:"Date",
                       optional:"R",
                       type:"Integer",
                       defValue:"26"
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Definite",value:"1" },
                           { name:"Doubtful",value:"2" },
                           { name:"Reported",value:"3" },
                        ]
                     },
                     { name:"HDI",
                       desc:"Hydrographic Depth/Height Information",
                       optional:"R",
                       type:"enumeration",
                       defValue:"-32768",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Depth known by Other than Wire",value:"9" },
                           { name:"Depth known by Wire Drag",value:"10" },
                           { name:"Depth Unknown",value:"12" },
                           { name:"Not Applicable",value:"15" }
                        ]
                     },
                     { name:"HDP",
                       desc:"Hydrographic Depth",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:""
                     },
                     { name:"VAL",
                       desc:"Value",
                       optional:"R",
                       units:"year",
                       type:"Integer",
                       defValue:"0"
                     },
                     { name:"VRR",
                       desc:"Vertical Reference Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Above surface / Does Not Cover (At High Water)",value:"1" },
                           { name:"Below Surface / Submerged",value:"4" },
                           { name:"Covers and Uncovers",value:"8" }
                        ]
                     }
                    ]
          },
        { name:"HAZARDP",
          fcode:"",
          desc:"Hazard Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" },
                           { name:"Doubtful",value:"3" }
                        ]
                     },
                     { name:"COD",
                       desc:"Certainty of Delineation",
                       optional:"R",
                       type:"enumeration",
                       defValue:"2",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Limits and Info Known",value:"1" },
                           { name:"Limits and Info Unknown",value:"2" }
                        ]
                     },
                     { name:"DAT",
                       desc:"Date",
                       optional:"R",
                       type:"Integer",
                       defValue:"26"
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Definite",value:"1" },
                           { name:"Doubtful",value:"2" },
                           { name:"Reported",value:"3" },
                        ]
                     },
                     { name:"HDH",
                       desc:"Hydrographic Drying Height",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:""
                     },
                     { name:"HDI",
                       desc:"Hydrographic Depth/Height Information",
                       optional:"R",
                       type:"enumeration",
                       defValue:"12",
                       enumerations:[
                           { name:"Depth known by Other than Wire",value:"9" },
                           { name:"Depth known by Wire Drag",value:"10" },
                           { name:"Depth unknown but safe to depth shown",value:"11" },
                           { name:"Depth Unknown",value:"12" },
                           { name:"Uncovering Height Known",value:"13" },
                           { name:"Uncovering Height Unknown",value:"14" },
                           { name:"Not Applicable",value:"15" }
                        ]
                     },
                     { name:"HDP",
                       desc:"Hydrographic Depth",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"LOC",
                       desc:"Location Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"4",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Below Surface / Submerged / Underground",value:"4" },
                           { name:"Hull Showing",value:"13" },
                           { name:"Masts Showing",value:"14" },
                           { name:"Funnel Showing",value:"20" },
                           { name:"Superstructure Showing",value:"21" },
                           { name:"Masts and Funnel Showing",value:"28" }
                        ]
                     },
                     { name:"MCC",
                       desc:"Material Composition Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown", value:"0" },
                           { name:"Coral", value:"24" },
                           { name:"Rock/Rocky", value:"84" }
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"SFC",
                       desc:"Sea Floor Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown Obstruction",value:"1" },
                           { name:"Other",value:"2" },
                           { name:"Fish Haven",value:"3" },
                           { name:"Well",value:"4" },
                           { name:"Submerged Production Platform",value:"5" },
                        ]
                     },
                     { name:"SOH",
                       desc:"Severity of Hazard",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Dangerous",value:"1" },
                           { name:"Non-Dangerous",value:"2" }
                        ]
                     },
                     { name:"TXT",
                       desc:"Text Attribute",
                       optional:"R",
                       length:"255",
                       type:"String",
                       defValue:"None"
                     },
                     { name:"VAL",
                       desc:"Value",
                       optional:"R",
                       units:"year",
                       type:"Integer",
                       defValue:"0"
                     },
                     { name:"VRR",
                       desc:"Vertical Reference Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"4",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Above surface / Does Not Cover (At High Water)",value:"1" },
                           { name:"Awash at Sounding Datum",value:"2" },
                           { name:"Below Surface / Submerged",value:"4" },
                           { name:"Covers and Uncovers",value:"8" }
                        ]
                     }
                    ]
          },
        { name:"LOADINGP",
          fcode:"",
          desc:"Loading Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"CHA",
                       desc:"Light Characteristic Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"23",
                       enumerations:[
                           { name:"Lighted",value:"21" },
                           { name:"Unlighted",value:"23" }
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"NST",
                       desc:"Navigation System Types",
                       optional:"R",
                       type:"enumeration",
                       defValue:"52",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Racon (Radar Responder Beacon)",value:"10" },
                           { name:"None",value:"52" },
                           { name:"QTG Station (R)",value:"53" },
                           { name:"Ramark (Ramark)",value:"54" },
                           { name:"Other",value:"999" }
                        ]
                     },
                     { name:"SST",
                       desc:"Sound Signal Type",
                       optional:"R",
                       type:"enumeration",
                       defValue:"16",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown", value:"0" },
                           { name:"Bell", value:"1" },
                           { name:"Diaphone", value:"2" },
                           { name:"Explosive fog signal", value:"3" },
                           { name:"Gong", value:"4" },
                           { name:"Horn", value:"6" },
                           { name:"Siren", value:"9" },
                           { name:"Whistle", value:"14" },
                           { name:"Reed", value:"15" },
                           { name:"None", value:"16" }
                        ]
                     },
                     { name:"USE",
                       desc:"Useage",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Single Point Mooring",value:"133" },
                           { name:"Other",value:"999" }
                        ]
                     },
                    ]
          },
        { name:"OBSTRUCP",
          fcode:"",
          desc:"Obstruction Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     }
                    ]
          },
        { name:"RUINSP",
          fcode:"",
          desc:"Ruins Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"HDH",
                       desc:"Hydrographic Drying Height",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:""
                     },
                     { name:"HDI",
                       desc:"Hydrographic Depth/Height Information",
                       optional:"R",
                       type:"enumeration",
                       defValue:"12",
                       enumerations:[
                           { name:"Depth known by Other than Wire",value:"9" },
                           { name:"Depth known by Wire Drag",value:"10" },
                           { name:"Depth Unknown",value:"12" },
                           { name:"Not Applicable",value:"15" }
                        ]
                     },
                     { name:"HDP",
                       desc:"Hydrographic Depth",
                       optional:"R",
                       units:"metre",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"LOC",
                       desc:"Location Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"22",
                       enumerations:[
                           { name:"On ground surface",value:"8" },
                           { name:"Off Shore",value:"22" },
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"VRR",
                       desc:"Vertical Reference Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"4",
                       enumerations:[
                           { name:"Above surface / Does Not Cover (At High Water)",value:"1" },
                           { name:"Below Surface / Submerged",value:"4" },
                           { name:"Covers and Uncovers",value:"8" },
                           { name:"Not Applicable",value:"9" }
                        ]
                     }
                    ]
          },
        { name:"LANDINGA",
          fcode:"",
          desc:"Landing Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:"BB150"
                     },
                     { name:"COD",
                       desc:"Certainty of Delineation",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Limits and Info Known",value:"1" }
                        ]
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"28",
                       enumerations:[
                           { name:"Natural",value:"45" }
                        ]
                     }
                    ]
          },
        { name:"PIERA",
          fcode:"",
          desc:"Pier Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"LOC",
                       desc:"Location Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"30",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"On Water Surface / Floating",value:"15" },
                           { name:"Non-Floating",value:"30" }
                        ]
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"USE",
                       desc:"Usage",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Berthing of Vessels",value:"119" }
                        ]
                     },
                    ]
          },
        { name:"STRUCTRA",
          fcode:"",
          desc:"Structure Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"VRR",
                       desc:"Vertical Reference Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Above surface / Does Not Cover (At High Water)",value:"1" },
                           { name:"Below Surface / Submerged",value:"4" },
                           { name:"Covers and Uncovers",value:"8" }
                        ]
                     }
                    ]
          },
        { name:"PIERL",
          fcode:"",
          desc:"Pier Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                     { name:"USE",
                       desc:"Usage",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Unknown",value:"0" },
                           { name:"Berthing of Vessels",value:"119" }
                        ]
                     },
                     { name:"VRR",
                       desc:"Vertical Reference Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Above surface / Does Not Cover (At High Water)",value:"1" },
                           { name:"Below Surface / Submerged",value:"4" },
                           { name:"Covers and Uncovers",value:"8" }
                        ]
                     }
                    ]
          },
        { name:"SEAWALLL",
          fcode:"",
          desc:"Seawall Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     }
                    ]
          },
        { name:"STRUCTRL",
          fcode:"",
          desc:"Structure Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"VRR",
                       desc:"Vertical Reference Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Above surface / Does Not Cover (At High Water)",value:"1" },
                           { name:"Below Surface / Submerged",value:"4" },
                           { name:"Covers and Uncovers",value:"8" }
                        ]
                     }
                    ]
          },
        { name:"BERTHP",
          fcode:"",
          desc:"Berthing Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"BER",
                       desc:"Berthing Identifier",
                       optional:"R",
                       length:"25",
                       type:"String",
                       defValue:"UNK"
                     },
                    ]
          },
        { name:"CALLINP",
          fcode:"",
          desc:"Calling in",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"DF1",
                       desc:"Direction of Traffic 1",
                       optional:"R",
                       units:"degree",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"DF2",
                       desc:"Direction of Traffic 2",
                       optional:"R",
                       units:"degree",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"DF3",
                       desc:"Direction of Traffic 3",
                       optional:"R",
                       units:"degree",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"DF4",
                       desc:"Direction of Traffic 4",
                       optional:"R",
                       units:"degree",
                       type:"Real",
                       defValue:"0.0"
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                    ]
          },
        { name:"LANDINGP",
          fcode:"",
          desc:"Landing Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:"BB150"
                     },
                     { name:"COD",
                       desc:"Certainty of Delineation",
                       optional:"R",
                       type:"enumeration",
                       defValue:"2",
                       enumerations:[
                           { name:"Limits and Info Known",value:"1" },
                           { name:"Limits and Info Unknown",value:"2" }
                        ]
                     },
                     { name:"EXS",
                       desc:"Existence Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Natural",value:"45" },
                           { name:"Man-made",value:"46" }
                        ]
                     }
                    ]
          },
        { name:"MOORINGP",
          fcode:"",
          desc:"Mooring Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     }
                    ]
          },
        { name:"RELLINE",
          fcode:"",
          desc:"Relief Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"HQC",
                       desc:"Hypsographic Portrayal Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"2",
                       enumerations:[
                           { name:"Index",value:"1" },
                           { name:"Intermediate",value:"2" },
                           { name:"Supplementary (1/2)",value:"3" },
                           { name:"Form Lines",value:"4" },
                           { name:"Index Approximate",value:"7" },
                           { name:"Intermediate Approximate",value:"12" }
                        ]
                     },
                     { name:"ZV2",
                       desc:"Highest Z-Value",
                       optional:"R",
                       units:"metre",
                       type:"Integer",
                       defValue:"99999"
                     }
                    ]
          },
        { name:"RELPOINT",
          fcode:"",
          desc:"Relief Point",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"ACC",
                       desc:"Accuracy Category",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ]
                     },
                     { name:"ELA",
                       desc:"Elevation Accuracy",
                       optional:"R",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ]
                     },
                     { name:"ZV2",
                       desc:"Highest Z-Value",
                       optional:"R",
                       units:"metre",
                       type:"Integer",
                       defValue:"99999"
                     }
                    ]
          },
        { name:"COAAREA",
          fcode:"",
          desc:"Coastlines/Countries Area",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"N/A"
                     }
                    ]
          },
        { name:"COALINE",
          fcode:"",
          desc:"Coastlines/Countries Line",
          geom:"Line",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     }
                    ]
          },
        { name:"COAPOINT",
          fcode:"",
          desc:"Coastlines/Countries POINT",
          geom:"Point",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code",
                       optional:"R",
                       length:"5",
                       type:"String",
                       defValue:""
                     },
                     { name:"NAM",
                       desc:"Name",
                       optional:"R",
                       type:"String",
                       defValue:"UNK"
                     },
                    ]
          },
        { name:"DQYAREA",
          fcode:"",
          desc:"Data Quality Area",
          geom:"Area",
          columns:[
                     { name:"SOURCE_ID",
                       desc:"Chart identifier",
                       optional:"R",
                       type:"Integer",
                       defValue:""
                     },
                     { name:"NAME",
                       desc:"Name",
                       optional:"R",
                       length:"40",
                       type:"String",
                       defValue:""
                     },
                     { name:"COMP_DATE",
                       desc:"Name",
                       optional:"R",
                       length:"20",
                       type:"String",
                       defValue:"N/A"
                     },
                     { name:"EDITION",
                       desc:"Edition",
                       optional:"R",
                       type:"Integer",
                       defValue:""
                     },
                     { name:"PRINT_DATE",
                       desc:"Chart Edition Date",
                       optional:"R",
                       length:"40",
                       type:"String",
                       defValue:""
                     },
                     { name:"SCALE",
                       desc:"Scale",
                       optional:"R",
                       type:"Integer",
                       defValue:""
                     },
                     { name:"REF_LAT",
                       desc:"Reference Latitude",
                       optional:"R",
                       type:"Real",
                       defValue:""
                     },
                     { name:"ABS_HORIZ_ACC",
                       desc:"Absolute Horizontal Accuracy",
                       optional:"R",
                       type:"Integer",
                       defValue:""
                     },
                     { name:"ABS_VERT_ACC",
                       desc:"Absolute Vertical Accuracy",
                       optional:"R",
                       type:"Integer",
                       defValue:""
                     },
                     { name:"DATUM",
                       desc:"Datum",
                       optional:"R",
                       length:"40",
                       type:"String",
                       defValue:""
                     },
                     { name:"LINEAGE",
                       desc:"Linage",
                       optional:"R",
                       length:"100",
                       type:"String",
                       defValue:""
                     },
                     { name:"SOURCE_INFO",
                       desc:"Source Information",
                       optional:"R",
                       length:"100",
                       type:"String",
                       defValue:""
                     },
                    ]
          }
    ]; // End of schema

    return schema;

} // End of getDbSchema

} // End of dnc.schema


exports.getDbSchema = dnc.schema.getDbSchema;
