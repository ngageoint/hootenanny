Feature: Clip Dataset

    Scenario: Start Hootenanny
        Given I am on Hootenanny
        And I resize the window
        And I click Get Started
    Scenario: I can open the form with the shortcut
        When I click the "map" at "0","0"
        Then I click the "8" key
        And I should see "Please add at least one dataset to the map to clip"

    Scenario: Add Data
        When I press "Add Reference Dataset"
        And I click the "DcGisRoadsCucumber" Dataset
        And I press "Add Layer"
        Then I wait 15 "seconds" to see "span.strong" element with text "DcGisRoadsCucumber"
        Then I change the reference layer color to blue
    Scenario: I can change coordinates for clip
        When I press "Tools"
        And I hover over ".tools-clip"
        And I press span with text "Clip Dataset"
        Then I should see "Enter Coordinates for Clip Bounding Box"
        And I should see "#maxlat" with a value between "38" and "39"
        And I should see "#minlat" with a value between "38" and "39"
        And I should see "#maxlat" with a value greater than "#minlat"
        And I should see "#maxlon" with a value between "-78" and "-76"
        And I should see "#minlon" with a value between "-78" and "-76"
        And I should see "#maxlon" with a value greater than "#minlon"
        Then I click the "Use Visual Extent" link
        And I fill "#maxlat" with "38.901164"
        And I fill "#maxlon" with "-77.016836"
        And I fill "#minlat" with "38.882563"
        And I fill "#minlon" with "-77.071161"
        Then I click the "Clip to Bounding Box" link
        And I click the "map" at "100","100"
        And I click the "map" at "400","400"
        Then I wait 5 "seconds" to see "Enter Coordinates for Clip Bounding Box"
        Then I press "big.loud" span with text "Next"

    Scenario: I can submit clip operation
        When I fill "LayerName" input with "CucumberClip_BB"
        And I press "big.loud" span with text "Clip"
        Then I wait 15 "seconds" to see "has been submitted."
        Then I wait 15 "seconds" to see "Success:"
        Then I remove the first layer
        When I click the "Add Reference Dataset" link
        And I click the "CucumberClip_BB" Dataset
        Then I click first "Add Layer"
        Then I wait 30 "seconds" to see "CucumberClip_BB"
        Then I wait 5 seconds
        Then I click the "trash.map-button" icon
        And I accept the alert

    Scenario: Delete data
        Then I select the "sprocket" div
        When I click on the "Datasets" option in the "settingsSidebar"
        And I click the "CucumberClip_BB" Dataset
        Then I context click the "CucumberClip_BB" Dataset
        And I click the "Delete (1)" context menu item
        And I accept the alert
        Then I wait 30 "seconds" to not see "CucumberClip_BB"
