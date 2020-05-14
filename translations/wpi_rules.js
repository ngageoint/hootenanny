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
 * ' * @copyright Copyright ...'
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

//
// Rules for NGA World Port Index translation
//

wpi = {
  // Numerical attribute translations
  numRules : {
    'INDEX_NO':'world_port_index', // From TDSv61
    // 'LATITUDE':'',
    // 'LAT_DEG':'',
    // 'LAT_MIN':'',
    // 'LONGITUDE':'',
    // 'LONG_DEG':'',
    // 'LONG_MIN':'',
    'REGION_NO':'world_port_index:region',
    'TIDE_RANGE':'tidal:range'
  }, // End numRules

  // Text attribute translations
  txtRules : {
    'CHART':'world_port_index:chart', // The chart it came from
    'COUNTRY':'is_in:country_code',
    // 'LAT_HEMI':'',
    // 'LONG_HEMI':'',
    'PORT_NAME':'name',
    'PUB':'world_port_index:publication', // Publication
  }, // End txtRules

  // one2one rules - translated JSON format
  one2one : {
    'ANCH_DEPTH':{
      'A':['anchorage:depth','23.2m-over'],
      'B':['anchorage:depth','21.6-22.9m'],
      'C':['anchorage:depth','20.1-21.3m'],
      'D':['anchorage:depth','18.6-19.8m'],
      'E':['anchorage:depth','17.1-18.2m'],
      'F':['anchorage:depth','15.5-16.8m'],
      'G':['anchorage:depth','14.0-15.2m'],
      'H':['anchorage:depth','12.5-13.7m'],
      'J':['anchorage:depth','11.0-12.2m'],
      'K':['anchorage:depth','9.4-10.7m'],
      'L':['anchorage:depth','7.9-9.1m'],
      'M':['anchorage:depth','6.4-7.6m'],
      'N':['anchorage:depth','4.9-6.1m'],
      'O':['anchorage:depth','3.4-4.6m'],
      'P':['anchorage:depth','1.8-3.0m'],
      'Q':['anchorage:depth','0-1.5m']
    },
    'CARGODEPTH':{
      'A':['cargo_wharf:depth','23.2m-over'],
      'B':['cargo_wharf:depth','21.6-22.9m'],
      'C':['cargo_wharf:depth','20.1-21.3m'],
      'D':['cargo_wharf:depth','18.6-19.8m'],
      'E':['cargo_wharf:depth','17.1-18.2m'],
      'F':['cargo_wharf:depth','15.5-16.8m'],
      'G':['cargo_wharf:depth','14.0-15.2m'],
      'H':['cargo_wharf:depth','12.5-13.7m'],
      'J':['cargo_wharf:depth','11.0-12.2m'],
      'K':['cargo_wharf:depth','9.4-10.7m'],
      'L':['cargo_wharf:depth','7.9-9.1m'],
      'M':['cargo_wharf:depth','6.4-7.6m'],
      'N':['cargo_wharf:depth','4.9-6.1m'],
      'O':['cargo_wharf:depth','3.4-4.6m'],
      'P':['cargo_wharf:depth','1.8-3.0m'],
      'Q':['cargo_wharf:depth','0-1.5m']
    },
    'CHAN_DEPTH':{
      'A':['channel:depth','23.2m-over'],
      'B':['channel:depth','21.6-22.9m'],
      'C':['channel:depth','20.1-21.3m'],
      'D':['channel:depth','18.6-19.8m'],
      'E':['channel:depth','17.1-18.2m'],
      'F':['channel:depth','15.5-16.8m'],
      'G':['channel:depth','14.0-15.2m'],
      'H':['channel:depth','12.5-13.7m'],
      'J':['channel:depth','11.0-12.2m'],
      'K':['channel:depth','9.4-10.7m'],
      'L':['channel:depth','7.9-9.1m'],
      'M':['channel:depth','6.4-7.6m'],
      'N':['channel:depth','4.9-6.1m'],
      'O':['channel:depth','3.4-4.6m'],
      'P':['channel:depth','1.8-3.0m'],
      'Q':['channel:depth','0-1.5m']
    },
    'OIL_DEPTH':{
      'A':['oil_terminal:depth','23.2m-over'],
      'B':['oil_terminal:depth','21.6-22.9m'],
      'C':['oil_terminal:depth','20.1-21.3m'],
      'D':['oil_terminal:depth','18.6-19.8m'],
      'E':['oil_terminal:depth','17.1-18.2m'],
      'F':['oil_terminal:depth','15.5-16.8m'],
      'G':['oil_terminal:depth','14.0-15.2m'],
      'H':['oil_terminal:depth','12.5-13.7m'],
      'J':['oil_terminal:depth','11.0-12.2m'],
      'K':['oil_terminal:depth','9.4-10.7m'],
      'L':['oil_terminal:depth','7.9-9.1m'],
      'M':['oil_terminal:depth','6.4-7.6m'],
      'N':['oil_terminal:depth','4.9-6.1m'],
      'O':['oil_terminal:depth','3.4-4.6m'],
      'P':['oil_terminal:depth','1.8-3.0m'],
      'Q':['oil_terminal:depth','0-1.5m']
    },

    // Entrance Restrictions
    'ENTRY_ICE':{
      'Y':['restriction:entry_ice','yes'],
      'N':['restriction:entry_ice','no']
    },
    'ENTRYOTHER':{
      'Y':['restriction:entry_other','yes'],
      'N':['restriction:entry_other','no']
    },
    'ENTRYSWELL':{
      'Y':['restriction:entry_swell','yes'],
      'N':['restriction:entry_swell','no']
    },
    'ENTRY_TIDE':{
      'Y':['restriction:entry_tide','yes'],
      'N':['restriction:entry_tide','no']
    },

    // Pilotage
    'PILOTADVSD':{
      'Y':['pilot:advised','yes'],
      'N':['pilot:advised','no']
    },
    'PILOTAVAIL':{
      'Y':['pilot:available','yes'],
      'N':['pilot:available','no']
    },
    'LOC_ASSIST':{
      'Y':['pilot:local_assistance','yes'],
      'N':['pilot:local_assistance','no']
    },
    'PILOT_REQD':{
      'Y':['pilot:required','yes'],
      'N':['pilot:required','no']
    },

    // Quarantine
    'PRATIQUE':{
      'Y':['quarantine:pratique','yes'],
      'N':['quarantine:pratique','no']
    },
    'SSCC_CERT':{
      'Y':['quarantine:sscc_cert','yes'],
      'N':['quarantine:sscc_cert','no']
    },
    'QUAR_OTHER':{
      'Y':['quarantine:other','yes'],
      'N':['quarantine:other','no']
    },

    // Communications
    'COMM_AIR':{
      'Y':['communication:air','yes'],
      'N':['communication:air','no']
    },
    'COMM_FAX':{
      'Y':['communication:fax','yes'],
      'N':['communication:fax','no']
    },
    'COMM_PHONE':{
      'Y':['communication:phone','yes'],
      'N':['communication:phone','no']
    },
    'COMM_RADIO':{
      'Y':['communication:radio','yes'],
      'N':['communication:radio','no']
    },
    'COMM_RAIL':{
      'Y':['communication:rail','yes'],
      'N':['communication:rail','no']
    },
    'COMM_VHF':{
      'Y':['communication:vhf','yes'],
      'N':['communication:vhf','no']
    },

    // Load/Offload
    'CARGO_ANCH':{
      'Y':['anchorage:cargo','yes'],
      'N':['anchorage:cargo','no']
    },
    'CARGOWHARF':{
      'Y':['cargo_wharf','yes'],
      'N':['cargo_wharf','no']
    },
    'CARBCHMOOR':{
      'Y':['beach_mooring','yes'],
      'N':['beach_mooring','no']
    },
    'CARGMDMOOR':{
      'Y':['medium_mooring','yes'],
      'N':['medium_mooring','no']
    },
    'CARICEMOOR':{
      'Y':['ice_mooring','yes'],
      'N':['ice_mooring','no']
    },

    // Cranes
    'CRANEFIXED':{
      'Y':['crane:fixed','yes'],
      'N':['crane:fixed','no']
    },
    'CRANEFLOAT':{
      'Y':['crane:floating','yes'],
      'N':['crane:floating','no']
    },
    'CRANEMOBIL':{
      'Y':['crane:mobile','yes'],
      'N':['crane:mobile','no']
    },

    // Lifts
    'LIFT50_100':{
      'Y':['lift:50_100','yes'],
      'N':['lift:50_100','no']
    },
    'LIFT_0_24':{
      'Y':['lift:0_24','yes'],
      'N':['lift:0_24','no']
    },
    'LIFT_100_':{
      'Y':['lift:over_100','yes'],
      'N':['lift:over_100','no']
    },
    'LIFT_25_49':{
      'Y':['lift:25_49','yes'],
      'N':['lift:25_49','no']
    },

    // Services
    'ELECREPAIR':{
      'Y':['service:electrical_repair','yes'],
      'N':['service:electrical_repair','no']
    },
    'ELECTRICAL':{
      'Y':['service:electricity','yes'],
      'N':['service:electricity','no']
    },
    'LONGSHORE':{
      'Y':['service:longshore','yes'],
      'N':['service:longshore','no']
    },
    'NAV_EQUIP':{
      'Y':['service:navigation_equipment','yes'],
      'N':['service:navigation_equipment','no']
    },
    'SERV_STEAM':{
      'Y':['service:steam','yes'],
      'N':['service:steam','no']
    },

    // Supplies
    'DECKSUPPLY':{
      'Y':['supply:deck_supplies','yes'],
      'N':['supply:deck_supplies','no']
    },
    'DIESEL':{
      'Y':['supply:diesel','yes'],
      'N':['supply:diesel','no']
    },
    'ENG_SUPPLY':{
      'Y':['supply:engine','yes'],
      'N':['supply:engine','no']
    },
    'FUEL_OIL':{
      'Y':['supply:fuel_oil','yes'],
      'N':['supply:fuel_oil','no']
    },
    'PROVISIONS':{
      'Y':['supply:provisions','yes'],
      'N':['supply:provisions','no']
    },
    'WATER':{
      'Y':['supply:water','yes'],
      'N':['supply:water','no']
    },


    'DEGAUSS':{
      'Y':['degauss','yes'],
      'N':['degauss','no']
    },
    'DRTYBALLST':{
      'Y':['dirty_ballast','yes'],
      'N':['dirty_ballast','no']
    },
    'DRYDOCK':{
      'S':['drydock:size','small_upto_200m'],
      'M':['drydock:size','medium_201-300m'],
      'L':['drydock:size','large_over_300m'],
    },
    'ETAMESSAGE':{
      'Y':['eta_message_required','yes'],
      'N':['eta_message_required','no']
    },
    'GARBAGE':{
      'Y':['garbage_disposal','yes'],
      'N':['garbage_disposal','no']
    },
    'HOLDGROUND':{
      'Y':['good_holding_ground','yes'],
      'N':['good_holding_ground','no']
    },
    'HARBORSIZE':{
      'L':['harbour:size','large'],
      'M':['harbour:size','medium'],
      'S':['harbour:size','small'],
      'V':['harbour:size','very_small']
    },
    'HARBORTYPE':{
      'Cn':['harbour:type','coastal_natural'],
      'CB':['harbour:type','coastal_breakwater'],
      'Ct':['harbour:type','coastal_tide'],
      'Rn':['harbour:type','river_natural'],
      'Rb':['harbour:type','river_basin'],
      'N':['harbour:type','none'],
      'Rt':['harbour:type','river_tidal_gate'],
      'Lc':['harbour:type','lake or canal'],
      'Or':['harbour:type','open_roadstead'],
      'Th':['harbour:type','typhoon_harbour']
    },
    'MED_FACIL':{
      'Y':['medical_facilities','yes'],
      'N':['medical_facilities','no']
    },
    'MAX_VESSEL':{
      'L':['vessel_size:maximum','over_500ft'],
      'M':['vessel_size:maximum','upto_500ft']
    },
    'OVERHD_LIM':{
      'Y':['overhead_limits','yes'],
      'N':['overhead_limits','no']
    },
    'PORTOFENTR':{
      'Y':['port_of_entry','yes'],
      'N':['port_of_entry','no']
    },
    'REPAIRCODE':{
      'A':['repair:type','major'],
      'B':['repair:type','moderate'],
      'C':['repair:type','limited'],
      'D':['repair:type','emergency_only'],
      'N':['repair:type','None'],
    },
    'RAILWAY':{
      'L':['marine_railway','large_over_1000tons'],
      'M':['marine_railway','medium_201-1000tons'],
      'S':['marine_railway','small_upto_200tons'],
    }, // Page IX
    'SHELTER':{
      'E':['shelter_afforded','excellent'],
      'G':['shelter_afforded','good'],
      'F':['shelter_afforded','fair'],
      'P':['shelter_afforded','poor'],
      'N':['shelter_afforded','none'],
    },
    'TUGSALVAGE':{
      'Y':['salvage_tug','yes'],
      'N':['salvage_tug','no']
    },
    'TUG_ASSIST':{
      'Y':['assistance_tug','yes'],
      'N':['assistance_tug','no']
    },
    'TURN_BASIN':{
      'Y':['turning_area','yes'],
      'N':['turning_area','no']
    },
    'US_REP':{
      'Y':['us_representitive_available','yes'],
      'N':['us_representitive_available','no']
    },
  } // End one2one
}; // End wpi
