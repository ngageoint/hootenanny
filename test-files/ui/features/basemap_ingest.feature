Feature: Basemap Ingest

    Scenario: Opening Management Tab
        Given I am on Hootenanny
        And I click Get Started
        Then I select the "sprocket" div

    Scenario: Basemap Ingest
        When I click on the "Basemaps" option in the "settingsSidebar"
        When I delete any existing "RomanColosseumCucumber" basemap if necessary
        And I press "New Basemap"
        And I select "/test-files/gdal2tiles/RomanColosseum_WV2naturalcolor_clip.tif" basemap
        And I type "RomanColosseumCucumber" in input "Save As"
        When I press "big.loud" span with text "Publish"
        Then I wait 30 "seconds" to see "RomanColosseumCucumber"

    Scenario: Add Basemap To Map
        Then I wait 30 "seconds" to see css "button.closedeye"
        When I click the "closedeye" classed element under "span.fill-white.small" with text "RomanColosseumCucumber"
        Then I select the "sprocket" div
        And I click the Background settings button
        And I click the "RomanColosseumCucumber" map layer
        And I wait
        And I accept the alert
        Then I select the "sprocket" div
        When I click the "openeye" classed element under "span.fill-white.small" with text "RomanColosseumCucumber"

    Scenario: Delete Map
        When I click the "trash" classed element under "span.fill-white.small" with text "RomanColosseumCucumber"
        And I accept the alert
        Then I wait 30 "seconds" to not see "RomanColosseumCucumber"
