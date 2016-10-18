Feature: Paste Attributes


    Scenario: Getting Started
        Given I am on Hootenanny
        And I resize the window
        And I click Get Started

    Scenario: Import Dataset
    	Then I select the "sprocket" div
        When I click on the "Datasets" option in the "settingsSidebar"
        And I press "Import Single Dataset"
        And I select the "File (osm,osm.zip,pbf)" option in the "Select Import Type" combobox
        And I select "/test-files/dcpoi_clip.osm" dataset
        Then I should see element "[id='importDatasetLayerName']" with value "dcpoi_clip"
        Then I type "dcpoi_clip_paste_attributes_Cucumber" in input "importDatasetLayerName"
        When I press "big.loud" span with text "Import"
        Then I wait 30 "seconds" to see "dcpoi_clip_paste_attributes_Cucumber"
        Then I select the "sprocket" div

    Scenario: Copy and Paste Attributes
        And I press "Add Reference Dataset"
        And I click the "dcpoi_clip_paste_attribu..." Dataset
        And I press "Add Layer"
        Then I wait 15 "seconds" to see "span.strong" element with text "dcpoi_clip_paste_attributes_Cucumber"
        When I select a node map feature with OSM id "n1"
        And I click the "Point" preset
        And I click the "Park" preset
        And I wait 2 seconds

        #I copy a feature
        Then I select a node map feature with OSM id "n1"
        And I click the control "c" key

        # I paste a feature
        Then I click the control "v" key
        And I click the "map" at "440","425"
        Then I should see a "g.n-1" on the map

        #I overwrite paste attributes with keyboard shortcut
        Then I select a node map feature with OSM id "n2"
        And I click the control shift "v" key
        Then I should see element "#preset-input-name" with value "KALORAMA PARK"
        And I expand the tag list toggle
        Then I should see the last element "input.value.combobox-input" with value "park"
        Then I click undo

        #I append paste attributes with keyboard shortcut
        Then I click the control alt "v" key
        Then I should see element "#preset-input-name" with value "KALORAMA RECREATION CENTER;KALORAMA PARK"
        And I expand the tag list toggle
        Then I should see the last element "input.value.combobox-input" with value "park"
        Then I click undo

        #I overwrite paste attributes with UI button
        Then I click paste tags, overwrite
        Then I should see element "#preset-input-name" with value "KALORAMA PARK"
        And I expand the tag list toggle
        Then I should see the last element "input.value.combobox-input" with value "park"
        Then I click undo

        #I append paste attributes with UI button
        Then I click paste tags, append
        Then I should see element "#preset-input-name" with value "KALORAMA RECREATION CENTER;KALORAMA PARK"
        And I expand the tag list toggle
        Then I should see the last element "input.value.combobox-input" with value "park"
        Then I click undo

        #Click save to clear local history
        #Otherwise next feature test will get alert about restoring unsaved features
        When I press "Save"
        Then I click the "save-button" button
        Then I click the "trash" button
        And I accept the alert
