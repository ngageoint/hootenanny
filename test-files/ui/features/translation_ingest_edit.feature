Feature: Translation Ingest, Edit, and Save

    Scenario: Opening Management Tab
        Given I am on Hootenanny
        And I click Get Started
        And I resize the window
        Then I select the "sprocket" div

    Scenario: Ingest Translation
        When I click on the "Translations" option in the "settingsSidebar"
        And I click the "Add New Translation" link under "utiltranslation"
        Then I fill "translationName" input with "Cucumber Translation"
        And I fill "translationDescription" input with "Cucumber Test Translation"
        And I add to "translationText" textarea with: hoot.require('translation_assistant')
        And I fill "translationText" textarea with: var attributeMapping = {"jakarta_raya_coastline": {"NATURAL": {"water": "coastline"}}};var fcode;var schema;
        And I add to "translationText" textarea with: //translateToOsm - takes 'attrs' and returns OSM 'tags'
        And I add to "translationText" textarea with: var translateToOsm = function(attrs, layerName) {return translation_assistant.translateAttributes(attrs, layerName, attributeMapping, fcode, schema);};
        Then I click first "Save Edits"
        And I wait
        And I should see a link "Cucumber Translation"
        And I hover over the "a" with text "Cucumber Translation"
        And I should see "Cucumber Test Translation"

    Scenario: Edit Translation
        And I click the "Cucumber Translation" link
        And I fill "translationDescription" input with "Jakarta Coastline"
        And I fill "translationText" textarea with: var attributeMapping = {"jakarta_raya_coastline": {"NATURAL": {"water": "other"}}};var fcode;var schema;
        And I add to "translationText" textarea with: //translateToOsm - takes 'attrs' and returns OSM 'tags'
        And I add to "translationText" textarea with: var translateToOsm = function(attrs, layerName) {return translation_assistant.translateAttributes(attrs, layerName, attributeMapping, fcode, schema);};
        Then I click first "Save Edits"
        Then I scroll "Cucumber Translation" into view
        And I hover over the "label" with text "Translations"
        And I hover over the "a" with text "Cucumber Translation"
        And I should see "Jakarta Coastline"

    # Scenario: Download Translation
    #     When I click on the "Translations" option in the "settingsSidebar"
    #     When I click the "export" icon under the "Cucumber Translation" link
    #     Then the download file pattern "Cucumber Translation*.js" should exist

    Scenario: Delete Translation
        Then I scroll "Cucumber Translation" into view
        Then I delete the "Cucumber Translation" translation
        And I accept the alert
        And I wait 5 "seconds" to not see "Cucumber Translation"
