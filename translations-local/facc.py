
def addRaw(tags, attrs, key):
    if key in attrs and attrs[key] != '':
        tags.update({'raw:' + key:attrs[key]})

def addName(attrs, key, names):
    if key in attrs and attrs[key] != '' and attrs[key] != 'UNK':
        names.append(attrs[key])

# Tags that can contain multiple values.
appendableTags = [
  # used as a note to mappers
  'note', 
  # used as a note to end users
  'description', 
  # used as a todo for mappers
  'fixme'
]

"""
This class can be used as a "default" value mapping. E.g. if nothing else matches, use this.
"""
Default = None

class Rule:
  """
  Basic enumeration matching that also supports calling sub-rules.
  """

  def __init__(self, key, mapping):
    """
    key - The FACC key string that this rule applies to. E.g. 'RST'
    mapping - A dictionary that describes the mapping from FACC values to OSM kvp.
    """
    self.key = key
    self.mapping = mapping
    self.featureCode = ''

  def addTag(self, tags, k, v):
    # if this is a field that can contain multiple values. E.g.
    # note = "Accuracy: Doubtful;Interchange"
    if k in appendableTags:
        newValue = ''
        if k in tags and tags[k] != '':
            newValue = tags[k] + ';' + v
        else:
            newValue = v             
        tags[k] = newValue
    else:
        tags[k] = v

  def applyMapping(self, attrs, tags, mapTo):
    if type(mapTo) == dict:
      for k, v in mapTo.iteritems():
        self.addTag(tags, k, v)
    # If there is a single rule to apply, evaluate that rule
    elif isinstance(mapTo, Rule):
      mapTo.featureCode = self.featureCode
      mapTo.apply(attrs, tags)
    # If there is a list of rules to apply
    elif type(mapTo) == list:
      for i in mapTo:
        self.applyMapping(attrs, tags, i)

  def apply(self, attrs, tags):
    """
    Applies the rule to a set of tags and attributes.
    attr - The input FACC attributes as a dictionary.
    tags - The output OSM tags as a dictionary. This set of tags will contain all the tags that
      have been set by previous steps.
    """
    if not self.key in attrs:    
      return
    
    value = attrs[self.key]

    if value in self.mapping:
      mapTo = self.mapping[value]
      self.applyMapping(attrs, tags, mapTo)
    elif Default in self.mapping:
      mapTo = self.mapping[Default]
      self.applyMapping(attrs, tags, mapTo)
    else:
      # no valid mapping found.
      print "Could not find a valid mapping for: %s : %s.      " % \
        (self.key, value)
      print
      raise Exception()

class RenameRule(Rule):
  def __init__(self, oldKey, newKey):
    self.oldKey = oldKey
    self.newKey = newKey

  def apply(self, attrs, tags):
    if not self.oldKey in attrs:    
      return

    tags[self.newKey] = attrs[self.oldKey].strip()

class FeatureCodeRule(Rule):
    def __init__(self, fcodeDict):
        self.fcodeDict = fcodeDict
        self.first = True

    def apply(self, attrs, tags):
        if self.featureCode in self.fcodeDict:
            mapTo = self.fcodeDict[self.featureCode]
            self.applyMapping(attrs, tags, mapTo)
        else:
            print "Could not find valid fcode for: " + self.featureCode
        if self.featureCode == '' and self.first:
            print "Got at least one empty feature code, taking a best guess."
  
class HardCodedRule(Rule):
  def __init__(self):
    pass

  def apply(self, attrs, tags):
    featureCode = self.featureCode
    note = []
    if 'note' in tags:
        note = tags['note'].split(';')
    
    highway = ''
    if 'highway' in tags:
        highway = tags['highway']

    addRaw(tags, attrs, "PR1")
    addRaw(tags, attrs, "PR2")
    addRaw(tags, attrs, "PRC")
    if 'USG' in attrs:
        usg = attrs['USG']
        if (usg != '' and usg != '0' and usg != '999'):
          addRaw(tags, attrs, "USG")
    addRaw(tags, attrs, 'GFID')

    # 1/30 inch * SCALE for standard
    # NGA standard is +/-25m 90% circular error (CE) for vector data
    # Error is only for well defined points, not the line connecting points
    accuracy = -1
    if 'ACE_EVAL' in attrs and attrs['ACE_EVAL'] != '':
        # Convert to 95% circular error (AKA ~2 standard deviations)
        accuracy = float(attrs['ACE_EVAL']) * 1.191
    elif 'ACE' in attrs and attrs['ACE'] != '':
        # Convert to 95% circular error (AKA ~2 standard deviations)
        accuracy = float(attrs['ACE']) * 1.191
    elif 'SCALE' in attrs and attrs['SCALE'] != '':
        scale = float(attrs['SCALE'])
        # Convert to accuracy as 90% circular error
        accuracy = scale * 1.0 / 30.0 / 12 * 0.3048
        # Convert to 95% circular error (AKA ~2 standard deviations)
        accuracy = accuracy * 1.191

    if 'ACC' in attrs:
        if attrs['ACC'] == '1':
            pass
        elif attrs['ACC'] == '2':
            accuracy = accuracy * 2
            note.append('Accuracy: Approximate')
        elif attrs['ACC'] == '3':
            accuracy = accuracy * 4
            note.append('Accuracy: Doubtful')
        elif attrs['ACC'] == '5':
            accuracy = -1
            note.append('Accuracy: Disputed')
        elif attrs['ACC'] == '6':
            note.append('Accuracy: Undisputed')
        elif attrs['ACC'] == '7':
            note.append('Accuracy: Precise')
        elif attrs['ACC'] == '8':
            accuracy = -1
            note.append('Accuracy: Abrogated')

    if accuracy > -1:
        tags['accuracy'] = '%.2f' % accuracy
    
    names = []
    addName(attrs, 'NAM', names)
    addName(attrs, 'NAM_1', names)
    addName(attrs, 'NAM_2', names)
    addName(attrs, 'NAM_3', names)
    addName(attrs, 'NAM_4', names)
    addName(attrs, 'NAM_5', names)
    addName(attrs, 'NAM_6', names)

    if len(names) > 0:
        tags.update({'name':names[0]})
    if len(names) > 1:
        tags.update({'alt_name':';'.join(names[1:])})


    if 'SEC_CLASS' in attrs:
        if attrs['SEC_CLASS'] == 'U':
            tags.update({'classification':'UNCLASSIFIED'})
        else:
            tags.update({'classification':attrs['SEC_CLASS']})

    if 'WTC' in attrs:
        wtc = attrs['WTC']
        if wtc == '1':
            tags.update({'all_weather':'yes'})
        elif wtc == '2':
            tags.update({'all_weather':'fair'})
        elif wtc == '3':
            tags.update({'all_weather':'winter'})

    if 'VH3' in attrs:
        vh3 = attrs['VH3']
        if vh3 == '1':
            tags.update({'maxheight':'5'})
        if vh3 == '2':
            tags.update({'maxheight':'10'})
        if vh3 == '3':
            tags.update({'maxheight':'20'})
        if vh3 == '4':
            tags.update({'maxheight':'40'})
        if vh3 == '5':
            tags.update({'maxheight':'50'})
            note.append('maxheight is >40')

        # attrubute for Cemeterys
        # Note how do we detect if there are cemeteries by attribute?
    if 'REL' in attrs:
        rel = attrs['REL']
        tags.update({'landuse':'cemetery'})
        tags.update({'amenity':'grave_yard'})
        if rel == '1':
            tags.update({'religion':'buddhist'})
        if rel == '2':
            tags.update({'religion':'muslim'})
        if rel == '3':
            tags.update({'religion':'christian'})
            tags.update({'denomination':'roman_catholic'})
        if rel == '4':
            tags.update({'religion':'christian'})
        if rel == '5':
            tags.update({'religion':'jewish'})
        if rel == '6':
            tags.update({'religion':'christian'})
            tags.update({'denomination':'greek_orthodox'})
        if rel == '7':
            tags.update({'religion':'christian'})
            tags.update({'denomination':'protestant'})
        if rel == '8':
            tags.update({'religion':'shinto'})


    # feature type = wells AA050
    if featureCode == 'AA050':
        tags.update({'well':'unknown'})
        if 'PRO' in attrs:
            pro = attrs['PRO']
            if pro == '38':
                tags.update({'well':'gas'})
                tags.update({'man_made':'petroleum_well'})
            elif pro == '67':
                tags.update({'well':'oil'})         
                tags.update({'man_made':'petroleum_well'})
            elif pro == '116':
                tags.update({'well':'water'})         
                tags.update({'man_made':'water_well'})

    if 'highway' in attrs:
        highway = attrs['highway']

    if 'TXT' in attrs and attrs['TXT'] != '' and attrs['TXT'] != 'N_A':
        note.append(attrs['TXT'])

    if 'FCO' in attrs:
        if attrs['FCO'] == '5' or attrs['FCO'] == '6':
            highway = 'motorway'

    if 'LOC' in attrs:
        if attrs['LOC'] == '4':
            tags.update({'layer':'-1'})
            if highway != '':
                tags.update({'tunnel':'yes'})
        if attrs['LOC'] == '25':
            tags.update({'layer':'1'})
            if highway != '':
                tags.update({'bridge':'yes'})
        if attrs['LOC'] == '40':
            tags.update({'layer':'-2'})
            if highway != '':
                tags.update({'tunnel':'yes'})

    types = {
        '5':'asphault',
        '9':'paving_stones',
        '11':'concrete',
        '16':'clay',
        '18':'paving_stones',
        '21':'concrete',
        '22':'conglomerate',
        '30':'earth',
        '46':'gravel',
        '48':'ground',
        '62':'paving_stones',
        '73':'pebblestone',
        '77':'concrete',
        '83':'concrete',
        '84':'rock',
        '104':'dirt',
        '110':'paving_stone'
        }
    if 'MCC' in attrs and attrs['MCC'] in types:
        tags.update({'surface':types[attrs['MCC']]})
    elif 'SMC' in attrs and attrs['SMC'] in types:
        tags.update({'surface':types[attrs['SMC']]})
    elif 'RST' in attrs:
        if attrs['RST'] == '1':
            tags.update({'surface':'paved'})
        if attrs['RST'] == '2':
            tags.update({'surface':'unpaved'})

    if 'LTN' in attrs and attrs['LTN'] != '':
        tags.update({'lanes':attrs['LTN']})
        

                
    if 'TUC' in attrs:
        if attrs['TUC'] == '1':
            highway = 'road'
            railway = 'rail'
        elif attrs['TUC'] == '2':
            highway = 'motorway'
        elif attrs['TUC'] == '3':
            railway = 'rail'
        elif attrs['TUC'] == '4':
            highway = 'road'
        elif attrs['TUC'] == '6':
            highway = 'unclassified'
        elif attrs['TUC'] == '17':
            highway = 'path'
            tags.update({'foot':'designated'})
        elif attrs['TUC'] == '37':
            highway = 'path'
            horse = 'designated'
        elif attrs['TUC'] == '38':
            waterway = 'canal'
        elif attrs['TUC'] == '41':
            waterway = 'aqueduct'
        else:
            addRaw(tags, attrs, 'TUC')


    if 'AVB' in attrs:
        avb = attrs['AVB']
        # This category doesn't really make sense to me. FACC specifies them as various access 
        # road to other more major routes. Seems to me that they're all simply unclassified
        if avb == '5':
            highway = 'unclassified'
        if avb == '6':
            highway = 'unclassified'
        if avb == '7':
            highway = 'unclassified'
        if avb == '8':
            highway = 'unclassified'

    if 'RTT' in attrs:
        htype = ''
        if attrs['RTT'] == '14':
            # In the jakarta data they specify motorways as primary through routes
            if 'TUC' in attrs and attrs['TUC'] == '7':
                htype = 'motorway'
                if 'raw:TUC' in tags:
                    del tags['raw:TUC']
            else:
                htype = 'primary'
        elif attrs['RTT'] == '15':
            htype = 'secondary'
        elif attrs['RTT'] == '16':
            htype = 'motorway'

        # if this is an interchange
        if featureCode == 'AP020':
            if htype != '':
                htype = htype + '_link'
            else:
                note.append('Interchange')

        if htype != '':
            highway = htype

    if 'TRF' in attrs:
        if attrs['TRF'] == '3':
            tags.update({'oneway':'yes'})
            note.append('oneway direction may be wrong')
        
    if 'MED' in attrs and attrs['MED'] != '':
        # if the median status is unknown
        if attrs['MED'] == '0':
            tags.update({'divider':'unknown'})
        # if there is a median
        elif attrs['MED'] == '1':
            tags.update({'divider':'yes'})
        # if no median
        elif attrs['MED'] == '2':
            pass
        elif attrs['MED'] == '6':
            tags.update({'divider':'yes'})
            tags.update({'divider:width':'2.5'})
            note.append('Divider width is < 5m.')
        elif attrs['MED'] == '7':
            tags.update({'divider':'yes'})
            tags.update({'divider:width':'12.5'})
            note.append('Divider width is >= 5m and < 20m.')
        elif attrs['MED'] == '8':
            tags.update({'divider':'yes'})
            tags.update({'divider:width':'27.5'})
            note.append('Divider width is >= 20m and < 35m.')
        elif attrs['MED'] == '9':
            tags.update({'divider':'yes'})
            tags.update({'divider:width':'57.5'})
            note.append('Divider width is >= 35m and < 80m.')
        elif attrs['MED'] == '10':
            tags.update({'divider':'yes'})
            tags.update({'divider:width':'80'})
            note.append('Divider width is >= 80m.')

    if 'WD1' in attrs:
        wd1 = attrs['WD1']
        if (wd1 != '0' and wd1 != ''):
            width = float(wd1) / 10.0
            tags.update({'width':"%g" % width})
            
    if 'BFC' in attrs:
        bfc = attrs['BFC']
        if bfc == '1':
            tags.update({'man_made':'works'})
        elif bfc == '2':
            tags.update({'office':'government'})
        elif bfc == '3':
            tags.update({'office':'government'})
            tags.update({'building:use':'capital_building'})
        elif bfc == '4':
            tags.update({'historic':'castle'})
        elif bfc == '5':
            tags.update({'office':'government'})
            tags.update({'building:use':'administration_building'})
        elif bfc == '6':
            tags.update({'amenity':'hospital'})
        elif bfc == '7':
            tags.update({'amenity':'place_of_worship'})
        elif bfc == '8':
            tags.update({'military':'administration'})
        elif bfc == '9':
            tags.update({'tourism':'museum'})
        elif bfc == '10':
            tags.update({'amenity':'observatory'})
        elif bfc == '11':
            tags.update({'building':'palace'})
        elif bfc == '12':
            tags.update({'amenity':'police'})
        elif bfc == '13':
            tags.update({'amenity':'prison'})
        elif bfc == '14':
            tags.update({'office':'ranger'})
        elif bfc == '15':
            tags.update({'amenity':'school'})
        elif bfc == '16':
            tags.update({'building':'house'})
        elif bfc == '17':
            tags.update({'building:use':'multi_unit_dwelling'})
        elif bfc == '18':
            tags.update({'building:use':'cemetery'})
        elif bfc == '19':
            tags.update({'building:use':'farm'})
        elif bfc == '20':
            tags.update({'landuse':'greenhouse_horticulture'})
        elif bfc == '21':
            tags.update({'building':'garage'})
        elif bfc == '22':
            tags.update({'man_made':'watermill'})
        elif bfc == '23':
            tags.update({'building:use':'wind_tunnel'})
        elif bfc == '24':
            tags.update({'landuse':'industrial'})
        elif bfc == '25':
            tags.update({'building':'roundhouse'})
        elif bfc == '26':
            tags.update({'building:use':'railroad_storage_repair_facility'})
        elif bfc == '27':
            tags.update({'railway':'halt'})
        elif bfc == '28':
            tags.update({'office':'administrative'})
        elif bfc == '29':
            tags.update({'aeroway':'repair'})
        elif bfc == '30':
            tags.update({'aeroway':'apron'})
        elif bfc == '31':
            tags.update({'building:use':'customs_house'})
        elif bfc == '33':
            tags.update({'amenity':'doctors'})
        elif bfc == '34':
            tags.update({'building:use':'firing_range'})
        elif bfc == '35':
            tags.update({'amenity':'post_office'})
        elif bfc == '36':
            tags.update({'military':'barracks'})
        elif bfc == '37':
            tags.update({'amenity':'fire_station'})
        elif bfc == '38':
            tags.update({'amenity':'prison'})
        elif bfc == '51':
            tags.update({'amenity':'marketplace'})
        elif bfc == '52':
            tags.update({'amenity':'townhall'})
        elif bfc == '53':
            tags.update({'amenity':'bank'})
        elif bfc == '54':
            tags.update({'amenity':'fuel'})
        elif bfc == '55':
            tags.update({'leisure':'marina'})
        elif bfc == '56':
            tags.update({'tourism':'hotel'})
        elif bfc == '57':
            tags.update({'amenity':'restaurant'})
        elif bfc == '58':
            tags.update({'building:use':'observation'})
        elif bfc == '59':
            tags.update({'office':'research'})
        elif bfc == '60':
            tags.update({'amenity':'college'})
        elif bfc == '61':
            tags.update({'amenity':'courthouse'})
        elif bfc == '62':
            tags.update({'building:use':'legation'})
        elif bfc == '63':
            tags.update({'building:use':'mission'})
        elif bfc == '64':
            tags.update({'building:use':'chancery'})
        elif bfc == '65':
            tags.update({'building:use':'ambassadorial_residence'})
        elif bfc == '66':
            tags.update({'amenity':'embassy'})
        elif bfc == '67':
            tags.update({'building:use':'consulate'})
        elif bfc == '68':
            tags.update({'military':'guard_house'})
        elif bfc == '69':
            tags.update({'military':'guard_shack'})
        elif bfc == '70':
            tags.update({'amenity':'kennel'})
        elif bfc == '71':
            tags.update({'man_made':'works'})
            tags.update({'building:use':'vegetable_oil_mill'})
        elif bfc == '72':
            tags.update({'building:use':'aerator'})
        elif bfc == '73':
            tags.update({'man_made':'works'})
            tags.update({'building:use':'carpentry'})
        elif bfc == '74':
            tags.update({'man_made':'works'})
            tags.update({'building:use':'sawmill'})
        elif bfc == '75':
            tags.update({'man_made':'works'})
            tags.update({'building:use':'kiln_oven'})
        elif bfc == '76':
            tags.update({'building:use':'signal_box'})
        elif bfc == '77':
            tags.update({'building:use':'harbor_masters_office'})
        elif bfc == '78':
            tags.update({'building:use':'marine_police'})
            tags.update({'amenity':'police'})
        elif bfc == '79':
            tags.update({'building:use':'rescue'})
        elif bfc == '80':
            tags.update({'building:use':'port_control'})
        elif bfc == '81':
            tags.update({'building:use':'maritime_station'})
        elif bfc == '82':
            tags.update({'man_made':'lighthouse'})
        elif bfc == '83':
            tags.update({'power':'generator'})
        elif bfc == '84':
            tags.update({'man_made':'works'})
            tags.update({'building:use':'filtration_plant'})
        elif bfc == '85':
            tags.update({'man_made':'works'})
            tags.update({'building:use':'newspaper_plant'})
        elif bfc == '86':
            tags.update({'building:use':'main_telephone_exchange'})
        elif bfc == '87':
            tags.update({'building:use':'auditorium'})
        elif bfc == '88':
            tags.update({'amenity':'theatre'})
            tags.update({'building:use':'opera_house'})
        elif bfc == '89':
            tags.update({'man_made':'works'})
            tags.update({'building:use':'processing_treatment'})
        elif bfc == '90':
            tags.update({'man_made':'works'})
            tags.update({'building:use':'pumphouse'})
        elif bfc == '91':
            tags.update({'building:use':'mobile_home'})
        elif bfc == '92':
            tags.update({'building:use':'weather_station'})
        elif bfc == '93':
            tags.update({'building:use':'dependents_housing'})
        elif bfc == '94':
            if featureCode == 'AL015':
                tags.update({'building':'train_station'})
            tags.update({'railway':'station'})
        elif bfc == '95':
            tags.update({'tourism':'hotel'})
        elif bfc == '96':
            tags.update({'building:use':'diplomatic_building'})
        elif bfc == '97':
            tags.update({'shop':'general'})
        elif bfc == '98':
            tags.update({'building:use':'shed'})
        elif bfc == '99':
            tags.update({'building:use':'battery'})
        elif bfc == '100':
            tags.update({'amenity':'doctors'})
        elif bfc == '101':
            tags.update({'amenity':'townhall'})
        elif bfc == '102':
            tags.update({'building:use':'oil_gas_fascilities'})
        elif bfc == '103':
            tags.update({'building:use':'outbuliding'})
        elif bfc == '104':
            tags.update({'man_made':'works'})
            tags.update({'building:use':'paper_pulp_mill'})
        elif bfc == '105':
            tags.update({'building:use':'reformatory'})
        elif bfc == '106':
            tags.update({'building:use':'sanitarium'})
        elif bfc == '107':
            tags.update({'building:use':'satellite_tracking_station'})
        elif bfc == '108':
            tags.update({'building:use':'seminary'})
        elif bfc == '109':
            tags.update({'building:use':'senior_citizens_home'})
        elif bfc == '110':
            tags.update({'waterway':'boatyard'})
        elif bfc == '111':
            tags.update({'leisure':'sports_centre'})
        elif bfc == '112':
            tags.update({'man_made':'works'})
            tags.update({'building:use':'steel_mill'})
        elif bfc == '113':
            tags.update({'building:use':'highway_weigh_scale'})
        elif bfc == '114':
            tags.update({'amenity':'place_of_worship'})
            note.append('Non-Christian Place of Worship')
        elif bfc == '115':
            tags.update({'tourism':'hostel'})
        elif bfc == '116':
            tags.update({'man_made':'works'})
        elif bfc == '117':
            tags.update({'tourism':'motel'})
        elif bfc == '118':
            tags.update({'amenity':'community_centre'})
        elif bfc == '119':
            tags.update({'amenity':'townhall'})
        elif bfc == '120':
            tags.update({'man_made':'works'})
            tags.update({'building:use':'automobile_plant'})
        elif bfc == '121':
            tags.update({'military':'armory'})
        elif bfc == '122':
            tags.update({'shop':'mall'})
        elif bfc == '123':
            tags.update({'amenity':'prison'})
        elif bfc == '124':
            tags.update({'building:use':'repair_facility'})
        elif bfc == '125':
            tags.update({'building:use':'barn_machinery_shed'})
        elif bfc == '126':
            tags.update({'building:use':'astronimical_station'})
        elif bfc == '127':
            tags.update({'amenity':'theatre'})
        elif bfc == '128':
            tags.update({'amenity':'library'})
        elif bfc == '129':
            tags.update({'aeroway':'terminal'})
        elif bfc == '130':
            tags.update({'amenity':'bus_station'})
        elif bfc == '131':
            tags.update({'building:use':'pilot_office'})
        elif bfc == '132':
            tags.update({'building:use':'pilot_look-out'})
        elif bfc == '133':
            tags.update({'building:use':'commercial'})
        elif bfc == '134':
            tags.update({'military':'fort'})
        elif bfc == '135':
            tags.update({'building:use':'blockhouse'})
        elif bfc == '136':
            tags.update({'building:use':'martello_tower'})
        elif bfc == '137':
            tags.update({'military':'guard_tower'})
        elif bfc == '138':
            tags.update({'building:use':'redoubt'})
        elif bfc == '139':
            tags.update({'building:use':'cooling'})
        elif bfc == '150':
            tags.update({'military':'barracks'})
        elif bfc == '151':
            tags.update({'building:use':'dormitory'})
        elif bfc == '723':
            tags.update({'amenity':'police'})
            note.append('Combined Fire and Police Station')
            
    
    if 'EXS' in attrs:
        exs = attrs['EXS']
        if exs == '5':
            if highway != '':
                tags.update({'construction':highway})
                highway = 'construction'
            else:
                tags.update({'landuse':'construction'})
        elif exs == '6':
            tags.update({'abandoned':'yes'})
        elif exs == '7':
            tags.update({'access':'no'})
        elif exs == '27':
            tags.update({'access':'no'})
        # exs==28 operational

    if 'SSR' in attrs:
        ssr = attrs['SSR']
        if ssr == '6':
            tags.update({'building:roof:shape':'conical'})
        if ssr == '38':
            tags.update({'building:roof:shape':'curved'})
        if ssr == '40':
            tags.update({'building:roof:shape':'dome'})
        if ssr == '41':
            tags.update({'building:roof:shape':'flat'})
        if ssr == '42':
            tags.update({'building:roof:shape':'gable'})
        if ssr == '47':
            tags.update({'building:roof:shape':'sawtooth'})
        if ssr == '50':
            tags.update({'building:roof:shape':'with_monitor'})
        if ssr == '51':
            tags.update({'building:roof:shape':'with_steeple'})
        if ssr == '55':
            tags.update({'building:roof:shape':'flat_with_monitor'})
        if ssr == '64':
            tags.update({'building:roof:shape':'gable_with_monitor'})
        if ssr == '77':
            tags.update({'building:roof:shape':'with_cupola'})
        if ssr == '78':
            tags.update({'building:roof:shape':'with_turret'})
        if ssr == '79':
            tags.update({'building:roof:shape':'with_tower'})
        if ssr == '80':
            tags.update({'building:roof:shape':'with_minaret'})
    
    if highway != '':
        tags.update({'highway':highway})

    if len(note) > 0:
        tags.update({'note':";".join(note)})
        
    if 'COMM_CPYRT' in attrs:
        tags.update({'license':attrs['COMM_CPYRT']})
    elif 'CPYRT_NOTE' in attrs:
        tags.update({'license':attrs['CPYRT_NOTE']})



# Rules are applied in order. Rules at the end override rules at the beginning.
rules = []

# This is handy when we have a place holder for a column that isn't properly
# defined. This will produce an error if something of value is found.
undefinedLookup = {
  # Unknown
  '':{},
  'UNK':{},
  '0':{},
  # Unpopulated
  '997':{},
  # Not Applicable
  '998':{},
  # Other
  '999':{}
}

# Start by doing a rough feature code classification. Sometimes the feature 
# codes are also referenced inside the rules.
rules.append(FeatureCodeRule({
    # Buildings
    'AL015':{'building':'yes'},
    'AL020':{},
    # Cart Track
    'AP010':{'highway':'track'},
    # Interchange
    'AP020':{'highway':'road'},
    # Road
    'AP030':{'highway':'road'},
    # Trail
    'AP050':{'highway':'path','foot':'designated'},
    # Bridge
    'AQ040':{'highway':'road','bridge':'yes'},
    # Tunnel
    'AQ130':{'highway':'road','tunnel':'yes'},
    # if we get an empty fcode then we'll take a best guess with the 
    # following rules
    '':{}
    }))

# Throw errors if there is valuable data in these fields
# @todo This list is incomplete
rules.append(Rule('RTA', undefinedLookup))
rules.append(Rule('RTB', undefinedLookup))
rules.append(Rule('RTC', undefinedLookup))
rules.append(Rule('RTP', undefinedLookup))
rules.append(Rule('RTS', undefinedLookup))
rules.append(Rule('RWT', undefinedLookup))
rules.append(Rule('SAW', undefinedLookup))
rules.append(Rule('SBC', undefinedLookup))
rules.append(Rule('SCB', undefinedLookup))

# Route number
rules.append(Rule('RTN', {
  '':{},
  'UNK':{},
  Default:RenameRule('RTN', 'ref')
}))
  

# Run the legacy code.
rules.append(HardCodedRule())

# Rules for Certainty of Existance
rules.append(Rule('COE', {
  '0':{},
  # Definite - No need to explicitly define its existance.
  '1':{},
  '2':{"description":"Existance is doubtful"},
  '3':{"description":"Existance is reported"},
  '4':{"description":"Existance is possible"},
  '5':{"description":"Existance is approximate/about"},
  # Unpopulated
  '997':{},
  # Not Applicable
  '998':{},
  # Other
  '999':{},
  # Strange values in some data
  '':{}
}))
    
# Rules for Educational Building Type
rules.append(Rule('EBT', {
  '0':{},
  # There isn't a clear definition for this in OSM schema
  '1':{"amenity":"school", "school":"academy"},
  '2':{"amenity":"college"},
  # There isn't a clear definition for this in OSM schema
  '3':{"amenity":"school", "school":"education_center"},
  # There isn't a clear definition for this in OSM schema
  '4':{"amenity":"school", "school":"lyceum"},
  '5':{"amenity":"university"},
  # This tagging isn't documented, but it is used 4 times as of 
  # 4/9/2013 in taginfo
  '6':{"amenity":"school", "seminary":"yes"},
  '7':{"amenity":"college", "note":"College or university, undifferentiated"},
  # N/A
  '8':{},
  '9':{"amenity":"school"},
  '10':{"amenity":"school", "note":"A school outbuilding"},
  # Unpopulated
  '997':{},
  # Other
  '999':{"note":"'other' educational building"}
}))
    
# Rules for House of Worship Type
rules.append(Rule('HWT', {
  '0':{},
  '2':{"amenity":"place_of_worship","building":"cathedral"},
  '3':{"amenity":"place_of_worship","building":"chapel"},
  '4':{"amenity":"place_of_worship","building":"church"},
  # According to Wikipedia a marabout is a person. Odd.
  '5':{"amenity":"place_of_worship","religion":"muslim","note":"marabout"},
  # There are two other instances of building=minaret in taginfo 4/9/2013
  '6':{"amenity":"place_of_worship","religion":"muslim","building":"minaret"},
  # There are 542 other instances of building=monastery in taginfo 4/9/2013
  '7':{"amenity":"place_of_worship","building":"monastery","religion":"christian","note":"covenant or monastery"},
  '9':{"amenity":"place_of_worship","building":"mosque","religion":"muslim"},
  # There are 14 other instances of building=pagoda in taginfo 4/9/2013
  '11':{"amenity":"place_of_worship","building":"pagoda"},
  # There are 18 other instances of building=shrine in taginfo 4/9/2013
  '14':{"amenity":"place_of_worship","building":"shrine"},
  '15':{"amenity":"place_of_worship","religion":"jewish","building":"tabernacle"},
  '16':{"amenity":"place_of_worship","building":"temple"},
  '20':{"amenity":"place_of_worship","religion":"jewish","building":"synagogue"},
  '21':{"amenity":"place_of_worship","building":"stupa"},
  '23':{"amenity":"place_of_worship"},
  # Unpopulated
  '997':{},
  # Not Applicable
  '998':{},
  # Other
  '999':{"amenity":"place_of_worship","note":"'other' house of worship"}
}))

# Rule for Landmark Category
rules.append(Rule('LMC', {
  '0':{},
  '1':{"landmark":"yes"},
  '2':{"landmark":"no"},
  # Unpopulated
  '997':{},
  # Not Applicable
  '998':{},
  # Other
  '999':{}
}))
    
# Rules for Surface Roughness Description
# Many of these rules don't have a direct conversion so I've omitted them for
# now and they can be filled in as needed. An error should be generated if
# an unanticipated value is found.
rules.append(Rule('SRD', {
  '0':{},
  '12':{'natural':'scree'},
  '38':{'natural':'sand'},
  '39':{'natural':'sand','note':'low dunes'},
  '40':{'natural':'sand','note':'high dunes'},
  # Unpopulated
  '997':{},
  # Not Applicable
  '998':{},
  # Other
  '999':{}
}))

rules.append(RenameRule('HGT', 'height'))

def toUpperCaseColumns(attrs):
    result = {}
    for (k, v) in attrs.items():
        result.update({k.upper():v})
    return result

def translateFacc(attrs, featureCode = ''):
    if not attrs: return

    attrs = toUpperCaseColumns(attrs)

    tags = {}
        
    for r in rules:
      r.featureCode = featureCode
      r.apply(attrs, tags)
        
    return tags



