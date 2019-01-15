
var _global = (0,eval)('this');
if (!_global.dnc)
{
  _global.dnc = {};
}

dnc.schema = {
getDbSchema: function()
{
    var full_FUN = [
              { name:"Unknown",value:"0" },
              { name:"Under Construction",value:"1" },
              { name:"Abandoned",value:"2" },
              { name:"Destroyed",value:"3" },
              { name:"Dismantled",value:"4" },
              { name:"Fully Functional",value:"6" },
              { name:"Damaged",value:"13" } 
             ];

    var full_SRT = [
              { name:"Unknown",value:"0" },
              { name:"ADRG",value:"1" },
              { name:"AAFIF",value:"2" },
              { name:"CHUM",value:"3" },
              { name:"City Graphic",value:"4" },
              { name:"Combat Chart",value:"5" },
              { name:"CADRG",value:"6" },
              { name:"CIB1",value:"7" },
              { name:"CIB5",value:"8" },
              { name:"DNC",value:"10" },
              { name:"DPF",value:"11" },
              { name:"DTOP 1",value:"16" },
              { name:"DTOP 2",value:"17" },
              { name:"DTOP 3",value:"18" },
              { name:"DTOP 4",value:"19" },
              { name:"DTOP 5",value:"20" },
              { name:"DVOF",value:"21" },
              { name:"FFD",value:"22" },
              { name:"Land Cover",value:"24" },
              { name:"GeoNames",value:"25" },
              { name:"GPS",value:"26" },
              { name:"ICM",value:"27" },
              { name:"NTM Imagery",value:"29" },
              { name:"Imagery",value:"30" },
              { name:"ITD",value:"31" },
              { name:"IVD",value:"32" },
              { name:"International Boundaries",value:"33" },
              { name:"JOG",value:"34" },
              { name:"LWD",value:"36" },
              { name:"MC&G",value:"37" },
              { name:"MSD 1",value:"38" },
              { name:"MSD 2",value:"39" },
              { name:"MSD 3",value:"40" },
              { name:"MSD 4",value:"41" },
              { name:"MSD 5",value:"42" },
              { name:"MIDB",value:"43" },
              { name:"Native Data",value:"44" },
              { name:"Native Map",value:"45" },
              { name:"Medical Facilities",value:"46" },
              { name:"NATE",value:"47" },
              { name:"Planning Graphic",value:"48" },
              { name:"SRTM",value:"50" },
              { name:"Site Map",value:"51" },
              { name:"TOD 0",value:"52" },
              { name:"TOD 1",value:"53" },
              { name:"TOD 2",value:"54" },
              { name:"TOD 3",value:"55" },
              { name:"TOD 4",value:"56" },
              { name:"TOD 5",value:"57" },
              { name:"UN Data",value:"59" },
              { name:"UVMap",value:"60" },
              { name:"VITD",value:"61" },
              { name:"VMap 0",value:"62" },
              { name:"VMap 1",value:"63" },
              { name:"VMap 2",value:"64" },
              { name:"WVS Plus",value:"65" },
              { name:"SPOT HRG Digital Terrain Model",value:"85" },
              { name:"Vector Data",value:"92" },
              { name:"1:25k Vector Data",value:"93" },
              { name:"1:50k Vector Data",value:"94" },
              { name:"1:100k Vector Data",value:"95" },
              { name:"Very High Resolution Commercial Monoscopic Imagery",value:"110" },
              { name:"Very High Resolution Commercial Stereoscopic Imagery",value:"111" },
              { name:"High Resolution Commercial Monoscopic Imagery",value:"112" },
              { name:"High Resolution Commercial Stereoscopic Imagery",value:"113" },
              { name:"Medium Resolution Commercial Monoscopic Imagery",value:"114" },
              { name:"Medium Resolution Commercial Stereoscopic Imagery",value:"115" },
              { name:"Low Resolution Commercial Monoscopic Imagery",value:"116" },
              { name:"Low Resolution Commercial Stereoscopic Imagery",value:"117" },
              { name:"Map 1:25k",value:"118" },
              { name:"Map 1:50k",value:"119" },
              { name:"Map 1:100k",value:"120" },
              { name:"Routing Data",value:"121" },
              { name:"Multiple",value:"996" },
              { name:"Unpopulated",value:"997" },
              { name:"Not Applicable",value:"998" },
              { name:"Other",value:"999" } 
             ];

    var schema = [
        { name:"BUILDNGA",
          fcode:"AAL015",
          desc:"Building Area",
          definition:"Building Area",
          geom:"Area",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },// End of FCODE
                     { name:"BFC",
                       desc:"Building Function Category" ,
                       optional:"R" ,
                       definition:"Building Function",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"", value:"0" },
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
                           { name:"Aircraft Maintenance", value:"29" },
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
                     },// End of BFC
                     { name:"COL",
                       desc:"Character of Light" ,
                       optional:"R" ,
                       definition:"Character of Light",
                       length:"254",
                       type:"String",
                       defValue:"UNK" 
                     },// End of COL
                     { name:"HWT",
                       desc:"House of Worship Type" ,
                       optional:"R" ,
                       definition:"The type of a house of worship based on its use,structure and/or religious denomination served.",
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
                     },// End of HWT
                    { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     } 
                    ] 
          },// End of feature BUILDNGA


        { name:"INDUSTA",
          fcode:"",
          desc:"Industry Area",
          definition:"Industry Area",
          geom:"Area",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     },
                     { name:"PRO",
                       desc:"Product" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
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
                     },// End of PRO
                     { name:"ZV2",
                       desc:"Highest Z-Value" ,
                       optional:"R" ,
                       definition:"XXX The vertical distance above Mean Sea Level (MSL) of the highest point of the landing area.",
                       units:"metre",
                       type:"Integer",
                       defValue:"99999" 
                     } // End of ZV2
                    ] 
          },// End of feature INDUSTA
        { name:"LANDMRKA",
          fcode:"",
          desc:"Landmark Area",
          definition:"Landmark Area",
          geom:"Area",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     } 
                    ] 
          },// End of feature LANDMRKA
        { name:"PARKA",
          fcode:"",
          desc:"Landmark Area",
          definition:"Landmark Area",
          geom:"Area",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     }
                    ] 
          },// End of feature PARKA
        { name:"TRANSA",
          fcode:"",
          desc:"Transpoprtation Area",
          definition:"Transportation Area",
          geom:"Area",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"APT",
                       desc:"Airfield Type" ,
                       optional:"R" ,
                       definition:"The type of an airfield based on the size and/or paving of its runways.",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Major",value:"1" },
                           { name:"Minor",value:"2" },
                           { name:"Seaplane Base",value:"4" },
                           { name:"Heliport",value:"9" },
                        ] 
                     },// End of APT
                     { name:"EXS",
                       desc:"Existence Category" ,
                       optional:"R" ,
                       definition:"Existence Category",
                       type:"enumeration",
                       defValue:"28",
                       enumerations:[
                           { name:"Under Construction",value:"5" },
                           { name:"Abandoned/Disused",value:"6" },
                           { name:"Operational",value:"28" }
                        ] 
                     },// End of EXS
                     { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     }
                    ] 
          },// End of feature TRANSA
        { name:"FENCEL",
          fcode:"",
          desc:"Fence  Line",
          definition:"Fence Line",
          geom:"Line",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     } 
                    ] 
          },// End of feature FENCEL
        { name:"PARKL",
          fcode:"",
          desc:"Park Line",
          definition:"Park Line",
          geom:"Line",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"SSC",
                       desc:"Structure Shape Category" ,
                       optional:"R" ,
                       definition:"Structure Shape Category",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Roller Coaster",value:"25" }
                        ] 
                     },// End of SSC
                     { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     }
                    ] 
          },// End of feature PARKL
        { name:"POWERL",
          fcode:"",
          desc:"Power Line",
          definition:"Power Line",
          geom:"Line",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"OHC",
                       desc:"Overhead Clearance Category" ,
                       optional:"R" ,
                       definition:"Overhead ClearanceCategory",
                       type:"Real",
                       defValue:"0.0",
                     },// End of OHC
                     { name:"OWO",
                       desc:"Over Water Category" ,
                       optional:"R" ,
                       definition:"Over Water Category",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Feature crosses navigable water",value:"1" },
                        ] 
                     }// End of OWO
                    ] 
          },// End of feature POWERL
        { name:"RAILRDL",
          fcode:"",
          desc:"Railroad Line",
          definition:"Railroad Line",
          geom:"Line",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     }, 
                     { name:"EXS",
                       desc:"Existence Category" ,
                       optional:"R" ,
                       definition:"Existence Category",
                       type:"enumeration",
                       defValue:"28",
                       enumerations:[
                           { name:"Under Construction",value:"5" },
                           { name:"Abandoned/Disused",value:"6" },
                           { name:"Destroyed",value:"7" },
                           { name:"Dismantled",value:"8" },
                           { name:"Operational",value:"28" }
                        ] 
                     },// End of EXS
                     { name:"LOC",
                       desc:"Location Category" ,
                       optional:"R" ,
                       definition:"Over Water Category",
                       type:"enumeration",
                       defValue:"8",
                       enumerations:[
                           { name:"Below Surface / Submerged / Underground",value:"4" },
                           { name:"On ground surface",value:"8" },
                           { name:"Suspended / Elevated Above Ground or Water",value:"25" },
                        ] 
                     }, // End of LOC
                     { name:"RRC",
                       desc:"Railroad Categories" ,
                       optional:"R" ,
                       definition:"Railroad Categories",
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
                     }// End of RRC
                     { name:"VRR",
                       desc:"Vertical Reference Category" ,
                       optional:"R" ,
                       definition:"Vertical Reference Category",
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
          },// End of feature RAILRDL
        { name:"TRANSL",
          fcode:"",
          desc:"Transportation Line",
          definition:"Transportation Line",
          geom:"Line",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"EXS",
                       desc:"Existence Category" ,
                       optional:"R" ,
                       definition:"Existence Category",
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
          definition:"Aeronautical Point",
          geom:"Point",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"ACC",
                       desc:"Accuracy Category" ,
                       optional:"R" ,
                       definition:"A general evaluation of the horizontal accuracy of the geographic position of a feature,as a category.",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ] 
                     },// End of ACC
                     { name:"APT",
                       desc:"Airfield Type" ,
                       optional:"R" ,
                       type:"enumeration",
                       defValue:"1", 
                       enumerations:[
                           { name:"Major Airfield", value:"1" },
                           { name:"Minor Airfield", value:"2" }
                        ] 
                     }, // End of APT
                     { name:"BRF",
                       desc:"Broadcast Frequency",
                       optional:"R",
                       units:"Hertz", 
                       type:"Integer",
                       defValue:"0" 
                     }, // End of BRF
                     { name:"COL",
                       desc:"XXX Character of Light" ,
                       optional:"R" ,
                       length:"254", 
                       type:"String",
                       defValue:"UNK" 
                     }, // End of COL
                     { name:"EXS",
                       desc:"Existence Category" ,
                       optional:"R" ,
                       definition:"Existence Category",
                       type:"enumeration",
                       defValue:"28",
                       enumerations:[
                           { name:"Operational",value:"28" },
                           { name:"Continuous Operation",value:"42" }
                        ] 
                     },
                     { name:"MCA",
                       desc:"XXX Morse Code" ,
                       optional:"R" ,
                       length:"254", 
                       type:"String",
                       defValue:"UNK" 
                     }, // End of MCA
                     { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     }
                     { name:"NST",
                       desc:"Navigation System Type" ,
                       optional:"R" ,
                       definition:"Navigation System Type",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"NOB (Non-directional Beacon)",value:"17" },
                           { name:"RNG (Radio Range)",value:"19" }
                        ] 
                     },
                     { name:"OLQ",
                       desc:"Obstruction Light Quality" ,
                       optional:"R" ,
                       definition:"Obstruction Light Quality",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
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
                     }, // End of BRF
                    ] 
          },
        { name:"BUILDNGP",
          fcode:"",
          desc:"Building Point",
          definition:"",
          geom:"Point",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"ACC",
                       desc:"Accuracy Category" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ] 
                     },
                     { name:"BFC",
                       desc:"Building Function Category" ,
                       optional:"R" ,
                       definition:"Building Function",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"", value:"0" },
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
                           { name:"Ligithouse", value:"82" },
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
                     },// End of BFC
                     { name:"COL",
                       desc:"XXX Character of Light" ,
                       optional:"R" ,
                       length:"254", 
                       type:"String",
                       defValue:"UNK" 
                     }, // End of COL
                     { name:"EXS",
                       desc:"Existence Category" ,
                       optional:"R" ,
                       definition:"Existence Category",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"28" },
                           { name:"Under Construction",value:"5" },
                           { name:"Abandoned / Disused",value:"6" },
                           { name:"Destroyed",value:"7" },
                        ] 
                     },
                     { name:"HWT",
                       desc:"House of Worship Type" ,
                       optional:"R" ,
                       definition:"The type of a house of worship based on its use,structure and/or religious denomination served.",
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
                     },// End of HWT
                     { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     },
                     { name:"SSR",
                       desc:"Structure Shape of Roof" ,
                       optional:"R" ,
                       type:"enumeration",
                       defValue:"0", 
                       enumerations:[
                           { name:"No Information", value:"0" }, 
                           { name:"Dome", value:"40" }, 
                           { name:"With Steeple", value:"51" },
                           { name:"With Cupola", value:"77" }, 
                           { name:"With Tower", value:"79" }, 
                           { name:"With Minaret", value:"80" }  
                        ] 
                     }, // End of SSR
                     { name:"SST",
                       desc:"Sound Signal Type" ,
                       optional:"R" ,
                       type:"enumeration",
                       defValue:"-32768", 
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
                     }, // End of SST
                     { name:"STA",
                       desc:"Station Type" ,
                       optional:"R" ,
                       type:"enumeration",
                       defValue:"-32768", 
                       enumerations:[
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
                     }, // End of STA
                    ] 
          },
        { name:"BUILTUPP",
          fcode:"",
          desc:"Built Up Area Point",
          definition:"Built Up Area Point",
          geom:"Point",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"ACC",
                       desc:"Accuracy Category" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ] 
                     },
                     { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     }
                    ] 
          },
        { name:"COMPP",
          fcode:"",
          desc:"Communications Point",
          definition:"Communications Point",
          geom:"Point",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"ACC",
                       desc:"Accuracy Category" ,
                       optional:"R" ,
                       definition:"",
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
                     }, // End of BRF
                     { name:"COL",
                       desc:"XXX Character of Light" ,
                       optional:"R" ,
                       length:"254", 
                       type:"String",
                       defValue:"UNK" 
                     }, // End of COL
                     { name:"NST",
                       desc:"Navigation System Type" ,
                       optional:"R" ,
                       definition:"The type(s) of equipment or system used in electronic navigation.", 
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
          },// End of feature COMPP
        { name:"INUDSTP",
          fcode:"",
          desc:"Industry Point",
          definition:"Industry Point",
          geom:"Point",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"ACC",
                       desc:"Accuracy Category" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ] 
                     },
                     { name:"COL",
                       desc:"XXX Character of Light" ,
                       optional:"R" ,
                       length:"254", 
                       type:"String",
                       defValue:"UNK" 
                     }, // End of COL
                     { name:"HGT",
                       desc:"Height Above Surface Level" ,
                       optional:"R" ,
                       definition:"", 
                       units:"Metre", 
                       type:"Integer",
                       defValue:"0" 
                     }, // End of HGT
                     { name:"LOC",
                       desc:"Location Category" ,
                       optional:"R" ,
                       definition:"Over Water Category",
                       type:"enumeration",
                       defValue:"8",
                       enumerations:[
                           { name:"On ground surface",value:"8" },
                           { name:"Off Shore",value:"22" },
                        ] 
                     }, // End of LOC
                     { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     },
                     { name:"PRO",
                       desc:"Product" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Electric",value:"31" },
                           { name:"Other",value:"999" },
                        ] 
                     },// End of PRO
                     { name:"SSC",
                       desc:"Structure Shape Category" ,
                       optional:"R" ,
                       definition:"Structure Shape Category",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
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
                     },// End of SSC
                     { name:"TUC",
                       desc:"Transportation Use Category" ,
                       optional:"R" ,
                       definition:"The primary use(s) of a transportation system.", 
                       type:"enumeration",
                       defValue:"0", 
                       enumerations:[
                           { name:"Unknown", value:"0" }, 
                           { name:"Both Road and Railroad", value:"1" }, 
                           { name:"Railroad", value:"3" }, 
                           { name:"Road", value:"4" }, 
                           { name:"Marine", value:"12" }, 
                           { name:"Air", value:"13" }, 
                           { name:"Pedestrian", value:"17" } 
                        ] 
                     }, // End of TUC
                     { name:"USE",
                       desc:"Useage" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"132",
                       enumerations:[
                           { name:"Container",value:"132" },
                           { name:"Other",value:"999" }
                        ] 
                     },
                     { name:"ZV2",
                       desc:"Highest Z-Value" ,
                       optional:"R" ,
                       definition:"XXX The vertical distance above Mean Sea Level (MSL) of the highest point of the landing area.",
                       units:"metre",
                       type:"Integer",
                       defValue:"99999" 
                     } // End of ZV2
                    ] 
          },
        { name:"LANDMRKP",
          fcode:"",
          desc:"Landmark Point",
          definition:"Landmark Point",
          geom:"Point",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"ACC",
                       desc:"Accuracy Category" ,
                       optional:"R" ,
                       definition:"",
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
          definition:"Power Point",
          geom:"Point",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"ACC",
                       desc:"Accuracy Category" ,
                       optional:"R" ,
                       definition:"",
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
          definition:"",
          geom:"Area",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     }
                    ] 
          },
        { name:"FORESHOA",
          fcode:"",
          desc:"Foreshore Area",
          definition:"",
          geom:"Area",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"BA020" 
                     },
                     { name:"MCC",
                       desc:"Material Composition Category" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"1",
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
                       desc:"Material Compsition Secondary" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"1",
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
          definition:"",
          geom:"Line",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"ACC",
                       desc:"Accuracy Category" ,
                       optional:"R" ,
                       definition:"",
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
                       desc:"Name 3" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     },
                     { name:"NM4",
                       desc:"Name 4" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     },
                     { name:"USE",
                       desc:"Useage" ,
                       optional:"R" ,
                       definition:"",
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
          definition:"",
          geom:"Line",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"NM3",
                       desc:"Name 3" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     },
                     { name:"NM4",
                       desc:"Name 4" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     },
                     { name:"USE",
                       desc:"Useage" ,
                       optional:"R" ,
                       definition:"",
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
          definition:"",
          geom:"Line",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"ACC",
                       desc:"Accuracy Category" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ] 
                     },
                     { name:"SLT",
                       desc:"Shoreline Type" ,
                       optional:"R" ,
                       definition:"The physical characteristics of a shoreline.", 
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
                           { name:"Other", value:"999" }, 
                        ] 
                     }, // End of SLT
                    ] 
          },
        { name:"FORESHOL",
          fcode:"",
          desc:"Foreshore Line",
          definition:"",
          geom:"Line",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     }
                    ] 
          },
        { name:"FORESHOP",
          fcode:"",
          desc:"Foreshore Point",
          definition:"",
          geom:"Point",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"BA020" 
                     },
                     { name:"MCC",
                       desc:"Material Composition Category" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"1",
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
                       desc:"Material Compsition Secondary" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"1",
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
          definition:"",
          geom:"Point",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"BA030" 
                     }
                    ] 
          },
        { name:"ECRTEXT",
          fcode:"",
          desc:"Earthcover Text",
          definition:"",
          geom:"Table",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"ZD040" 
                     },
                     { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     },
                    ] 
          },
        { name:"ENVAREA",
          fcode:"",
          desc:"Environment Area",
          definition:"",
          geom:"Area",
          columns:[
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"ZC040" 
                     },
                     { name:"VAV",
                       desc:"Variation Anomaly Value" ,
                       optional:"R" ,
                       definition:"",
                       units:"degrees", 
                       type:"Integer",
                       defValue:"0" 
                     },
                    ] 
          },
        { name:"CURRDIAP",
          fcode:"",
          desc:"Current Diagram Point",
          definition:"",
          geom:"Point",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
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
                       units:"knot", 
                       type:"Real",
                       defValue:"0.0" 
                     },
                     { name:"D81",
                       desc:"Direction of Current 1",
                       optional:"R",
                       units:"knot", 
                       type:"Real",
                       defValue:"0.0" 
                     },
                     { name:"D82",
                       desc:"Direction of Current 2",
                       optional:"R",
                       units:"knot", 
                       type:"Real",
                       defValue:"0.0" 
                     },
                     { name:"D83",
                       desc:"Direction of Current 3",
                       optional:"R",
                       units:"knot", 
                       type:"Real",
                       defValue:"0.0" 
                     },
                     { name:"D84",
                       desc:"Direction of Current 4",
                       optional:"R",
                       units:"knot", 
                       type:"Real",
                       defValue:"0.0" 
                     },
                     { name:"D85",
                       desc:"Direction of Current 5",
                       optional:"R",
                       units:"knot", 
                       type:"Real",
                       defValue:"0.0" 
                     },
                     { name:"D86",
                       desc:"Direction of Current 6",
                       optional:"R",
                       units:"knot", 
                       type:"Real",
                       defValue:"0.0" 
                     },
                     { name:"D87",
                       desc:"Direction of Current 7",
                       optional:"R",
                       units:"knot", 
                       type:"Real",
                       defValue:"0.0" 
                     },
                     { name:"D88",
                       desc:"Direction of Current 8",
                       optional:"R",
                       units:"knot", 
                       type:"Real",
                       defValue:"0.0" 
                     },
                     { name:"D89",
                       desc:"Direction of Current 9",
                       optional:"R",
                       units:"knot", 
                       type:"Real",
                       defValue:"0.0" 
                     },
                     { name:"D90",
                       desc:"Direction of Current 10",
                       optional:"R",
                       units:"knot", 
                       type:"Real",
                       defValue:"0.0" 
                     },
                     { name:"D91",
                       desc:"Direction of Current 11",
                       optional:"R",
                       units:"knot", 
                       type:"Real",
                       defValue:"0.0" 
                     }
                    ] 
          },
        { name:"CURRFLP",
          fcode:"",
          desc:"Current Flow Point",
          definition:"",
          geom:"Point",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
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
                       desc:"Current Type Category" ,
                       optional:"R" ,
                       definition:"",
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
                       desc:"Current Information 1" ,
                       optional:"R" ,
                       definition:"Current Information 1",
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
                       desc:"Current Information 1" ,
                       optional:"R" ,
                       definition:"Current Information 1",
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
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     },
                    ] 
          },
        { name:"TIDEP",
          fcode:"",
          desc:"Tide Point",
          definition:"",
          geom:"Point",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     },
                    ] 
          },
        { name:"HYDAREA",
          fcode:"",
          desc:"Hydrographic Area",
          definition:"",
          geom:"Area",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"ACC",
                       desc:"Accuracy Category" ,
                       optional:"R" ,
                       definition:"",
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
          definition:"",
          geom:"Line",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"ACC",
                       desc:"Accuracy Category" ,
                       optional:"R" ,
                       definition:"",
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
          definition:"",
          geom:"Point",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"CSM",
                       desc:"Secondary Material Characteristics" ,
                       optional:"R" ,
                       definition:"",
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
                       desc:"Material Composition Category" ,
                       optional:"R" ,
                       definition:"",
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
                       desc:"Material Composition Secondary" ,
                       optional:"R" ,
                       definition:"",
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
                       desc:"Material Composition Underlying" ,
                       optional:"R" ,
                       definition:"",
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
                       desc:"Physical Surface Characteristics" ,
                       optional:"R" ,
                       definition:"",
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
                       desc:"Underlying Material Characteristics" ,
                       optional:"R" ,
                       definition:"",
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
          definition:"",
          geom:"Point",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"ACC",
                       desc:"Accuracy Category" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" },
                           { name:"Doubtful",value:"3" }
                        ] 
                     },
                     { name:"DAT",
                       desc:"XXX Date" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"26" 
                     },
                     { name:"EXS",
                       desc:"Existence Category" ,
                       optional:"R" ,
                       definition:"Existence Category",
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
                       desc:"Sounding Category" ,
                       optional:"R" ,
                       definition:"Sounding Category",
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
                       desc:"Sounding Velocity" ,
                       optional:"R" ,
                       definition:"Sounding Velocity",
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
          definition:"",
          geom:"Area",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"EXS",
                       desc:"Existence Category" ,
                       optional:"R" ,
                       definition:"Existence Category",
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
                       desc:"Hydrological Category" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"8",
                       enumerations:[
                           { name:"Dry",value:"3" },
                           { name:"Perennial/Permanent",value:"8" }
                        ] 
                     },
                     { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     },
                     { name:"RPA",
                       desc:"Required Port Access" ,
                       optional:"R" ,
                       definition:"",
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
          definition:"",
          geom:"Area",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"HYC",
                       desc:"Hydrological Category" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"8",
                       enumerations:[
                           { name:"Perennial/Permanent",value:"8" }
                        ] 
                     },
                     { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     },
                     { name:"RPA",
                       desc:"Required Port Access" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
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
          definition:"",
          geom:"Area",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"EXS",
                       desc:"Existence Category" ,
                       optional:"R" ,
                       definition:"Existence Category",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Natural",value:"45" },
                           { name:"Man-made",value:"46" }
                        ] 
                     },
                     { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
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
          definition:"",
          geom:"Area",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"ACC",
                       desc:"Accuracy Category" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ] 
                     },
                     { name:"HYC",
                       desc:"Hydrological Category" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"8",
                       enumerations:[
                           { name:"Perennial/Permanent",value:"8" }
                        ] 
                     },
                     { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     },
                     { name:"RPA",
                       desc:"Required Port Access" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Access Required",value:"1" },
                           { name:"Access Not Required",value:"2" }
                        ] 
                     },
                     { name:"SLT",
                       desc:"Shoreline Type" ,
                       optional:"R" ,
                       definition:"",
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
          definition:"",
          geom:"Line",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"LOC",
                       desc:"Location Category" ,
                       optional:"R" ,
                       definition:"Over Water Category",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Below Surface / Submerged / Underground",value:"4" },
                           { name:"On ground surface",value:"8" },
                           { name:"Suspended / Elevated Above Ground or Water",value:"25" },
                        ] 
                     }, // End of LOC
                     { name:"OHC",
                       desc:"Overhead Clearance Category" ,
                       optional:"R" ,
                       definition:"Overhead ClearanceCategory",
                       type:"Real",
                       defValue:"0.0",
                     },// End of OHC
                     { name:"OWO",
                       desc:"Over Water Category" ,
                       optional:"R" ,
                       definition:"Over Water Category",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Feature crosses navigable water",value:"1" },
                        ] 
                     }// End of OWO
                    ] 
          },
        { name:"BARRIERL",
          fcode:"",
          desc:"Barrier Line",
          definition:"",
          geom:"Line",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"EXS",
                       desc:"Existence Category" ,
                       optional:"R" ,
                       definition:"Existence Category",
                       type:"enumeration",
                       defValue:"50",
                       enumerations:[
                           { name:"Non-Tidal",value:"50" }
                        ] 
                     },
                     { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     },
                    ] 
          },
        { name:"CANALL",
          fcode:"",
          desc:"Canal Line",
          definition:"",
          geom:"Line",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"EXS",
                       desc:"Existence Category" ,
                       optional:"R" ,
                       definition:"Existence Category",
                       type:"enumeration",
                       defValue:"32",
                       enumerations:[
                           { name:"Navigable",value:"32" }
                        ] 
                     },
                     { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
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
          definition:"",
          geom:"Line",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     },
                     { name:"USE",
                       desc:"Usage" ,
                       optional:"R" ,
                       definition:"Usage",
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
          definition:"",
          geom:"Line",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"ACC",
                       desc:"Accuracy Category" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ] 
                     },
                     { name:"AHC",
                       desc:"Associated Hydrological Category" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Perennial",value:"1" },
                           { name:"Intermittent",value:"2" }
                        ] 
                     },
                     { name:"HYC",
                       desc:"Hydrological Category" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"8",
                       enumerations:[
                           { name:"Non-Perennial/Intermittent/Fluctuating",value:"6" },
                           { name:"Perennial/Permanent",value:"8" }
                        ] 
                     },
                     { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     },
                     { name:"SLT",
                       desc:"Shoreline Type" ,
                       optional:"R" ,
                       definition:"",
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
        { name:"EMBANKA",
          fcode:"",
          desc:"Embankment Area",
          definition:"",
          geom:"Area",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"EXS",
                       desc:"Existence Category" ,
                       optional:"R" ,
                       definition:"Existence Category",
                       type:"enumeration",
                       defValue:"51",
                       enumerations:[
                           { name:"Tidal/Tidal Fluctuation",value:"51" }
                        ] 
                     },
                     { name:"USE",
                       desc:"Usage" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"as a Causeway",value:"127" }
                        ] 
                     },
                     { name:"VRR",
                       desc:"Vertical Reference Category" ,
                       optional:"R" ,
                       definition:"Vertical Reference Category",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
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
          definition:"",
          geom:"Area",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     }
                    ]
          },
        { name:"SNOWICEA",
          fcode:"",
          desc:"Snow/Ice Area",
          definition:"",
          geom:"Area",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"SIC",
                       desc:"Snow/Ice Category" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
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
          definition:"",
          geom:"Area",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"EXS",
                       desc:"Existence Category" ,
                       optional:"R" ,
                       definition:"Existence Category",
                       type:"enumeration",
                       defValue:"30",
                       enumerations:[
                           { name:"Not Isolated",value:"30" },
                        ] 
                     },
                     { name:"TRE",
                       desc:"Tree Category" ,
                       optional:"R" ,
                       definition:"",
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
                       desc:"Vegetation Category" ,
                       optional:"R" ,
                       definition:"",
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
          definition:"",
          geom:"Area",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"DAT",
                       desc:"XXX Date" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"26" 
                     },
                     { name:"EXS",
                       desc:"Existence Category" ,
                       optional:"R" ,
                       definition:"Existence Category",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Definite",value:"1" },
                           { name:"Doubtful",value:"2" },
                           { name:"Reported",value:"3" },
                        ] 
                     },
                     { name:"LOC",
                       desc:"Location Category" ,
                       optional:"R" ,
                       definition:"Over Water Category",
                       type:"enumeration",
                       defValue:"4",
                       enumerations:[
                           { name:"Below Surface / Submerged / Underground",value:"4" }
                        ] 
                     }, // End of LOC
                     { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
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
          definition:"",
          geom:"Line",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"USE",
                       desc:"Usage" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
                           { name:"Unknown",value:"0" },
                           { name:"Levee/Dyke",value:"69" },
                           { name:"as a Causeway",value:"127" }
                        ] 
                     },
                     { name:"VRR",
                       desc:"Vertical Reference Category" ,
                       optional:"R" ,
                       definition:"Vertical Reference Category",
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
          definition:"",
          geom:"Point",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"HGT",
                       desc:"Height Above Surface Level" ,
                       optional:"R" ,
                       definition:"", 
                       units:"Metre", 
                       type:"Integer",
                       defValue:"0" 
                     }, // End of HGT
                    ] 
          },
        { name:"TREEP",
          fcode:"",
          desc:"Tree Point",
          definition:"",
          geom:"Point",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"EXS",
                       desc:"Existence Category" ,
                       optional:"R" ,
                       definition:"Existence Category",
                       type:"enumeration",
                       defValue:"31",
                       enumerations:[
                           { name:"Isolated",value:"31" },
                        ] 
                     },
                     { name:"TRE",
                       desc:"Tree Category" ,
                       optional:"R" ,
                       definition:"",
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
                       desc:"Vegetation Category" ,
                       optional:"R" ,
                       definition:"",
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
          definition:"",
          geom:"Area",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"COD",
                       desc:"Certainty of Delineation" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Limits and Info Known",value:"1" },
                           { name:"Limits and Info Unknown",value:"2" }
                        ] 
                     },
                     { name:"EXS",
                       desc:"Existence Category" ,
                       optional:"R" ,
                       definition:"Existence Category",
                       type:"enumeration",
                       defValue:"46",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Natural",value:"45" },
                           { name:"Man-made",value:"46" }
                        ] 
                     },
                     { name:"MAC",
                       desc:"Maritime Area Category" ,
                       optional:"R" ,
                       definition:"",
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
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"70",
                       type:"String",
                       defValue:"UNK" 
                     },
                     { name:"OPS",
                       desc:"Operational State" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Null",value:"-32768" },
                           { name:"Operational",value:"1" },
                           { name:"non-Operational",value:"2" }
                        ] 
                     },
                     { name:"PBV",
                       desc:"Pilot Boarding Vehicle" ,
                       optional:"R" ,
                       definition:"",
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
                       desc:"Product" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"0",
                       enumerations:[
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
                     },// End of PRO
                     { name:"TIM",
                       desc:"Time Attribute" ,
                       optional:"R" ,
                       definition:"",
                       type:"Integer",
                       defValue:"-32l68" 
                     },
                     { name:"TXT",
                       desc:"Text Attribute" ,
                       optional:"R" ,
                       definition:"",
                       length:"255",
                       type:"String",
                       defValue:"None" 
                     },
                     { name:"USE",
                       desc:"Usage" ,
                       optional:"R" ,
                       definition:"",
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
          definition:"",
          geom:"Area",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"ATN",
                       desc:"Aids to Navigation" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"2",
                       enumerations:[
                           { name:"Marked",value:"1" },
                           { name:"Unmarked",value:"2" }
                        ] 
                     },
                     { name:"COD",
                       desc:"Certainty of Delineation" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Limits and Info Known",value:"1" },
                           { name:"Limits and Info Unknown",value:"2" }
                        ] 
                     },
                     { name:"DAN",
                       desc:"Description of Aids to Navigation" ,
                       optional:"R" ,
                       definition:"",
                       length:"255",
                       type:"String",
                       defValue:"N/A" 
                     },
                     { name:"DAT",
                       desc:"XXX Date" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"26" 
                     },
                     { name:"EXS",
                       desc:"Existence Category" ,
                       optional:"R" ,
                       definition:"Existence Category",
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
                       desc:"IMO Approval Status" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Approved",value:"1" },
                           { name:"Not Approved",value:"2" }
                        ] 
                     },
                     { name:"MAC",
                       desc:"Maritime Area Category" ,
                       optional:"R" ,
                       definition:"",
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
                       desc:"Maintenance Status" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Maintained",value:"1" },
                           { name:"Not Maintained",value:"2" }
                        ] 
                     },
                     { name:"TSP",
                       desc:"Traffic Scheme Part" ,
                       optional:"R" ,
                       definition:"",
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
                       desc:"Work in Progress Category" ,
                       optional:"R" ,
                       definition:"",
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
          definition:"",
          geom:"Area",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"HDI",
                       desc:"Hydrographic Depth/Height Information" ,
                       optional:"R" ,
                       definition:"",
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
                       desc:"Route Intended Use" ,
                       optional:"R" ,
                       definition:"",
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





        { name:"",
          fcode:"",
          desc:"",
          definition:"",
          geom:"Point",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code" ,
                       optional:"R" ,
                       length:"5",
                       type:"String",
                       defValue:"" 
                     },
                     { name:"ACC",
                       desc:"Accuracy Category" ,
                       optional:"R" ,
                       definition:"",
                       type:"enumeration",
                       defValue:"1",
                       enumerations:[
                           { name:"Accurate",value:"1" },
                           { name:"Approximate",value:"2" }
                        ] 
                     },
                     { name:"",
                       desc:"",
                       optional:"R",
                       units:"metre", 
                       type:"Integer",
                       defValue:"0" 
                     },
                     { name:"EXS",
                       desc:"Existence Category" ,
                       optional:"R" ,
                       definition:"Existence Category",
                       type:"enumeration",
                       defValue:"28",
                       enumerations:[
                           { name:"Operational",value:"28" },
                           { name:"Continuous Operation",value:"42" }
                        ] 
                     },
                     { name:"NAM",
                       desc:"Name" ,
                       optional:"R" ,
                       definition:"",
                       length:"30",
                       type:"String",
                       defValue:"UNK" 
                     },
                    ] 
          },


    ]; // End of schema

    return schema; 

} // End of getDbSchema

} // End of dnc.schema


exports.getDbSchema = dnc.schema.getDbSchema;
