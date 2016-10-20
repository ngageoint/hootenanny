Feature: Check Layer Load Actions

    Scenario: Open Hootenanny and Add Dataset Without Selecting Layer
        Given I am on Hootenanny
        When I click Get Started
        And I press "Add Reference Dataset"
        And I press "Add Layer"
        Then I should see "Please select a dataset to add to the map!"

    Scenario: Attempt to Load Same Dataset Twice
        When I click the "DcGisRoadsCucumber" Dataset
        And I press "Add Layer"
        Then I wait 15 "seconds" to see "span.strong" element with text "DcGisRoadsCucumber"
        When I press "Add Secondary Dataset"
        And I click the "DcGisRoadsCucumber" Dataset
        And I press "Add Layer"
        Then I should see "A layer with this name has already been added to the map!"
        
    Scenario: Attempt to Load Same Dataset from Recently Used Layers
        When I select the "DcGisRoadsCucumber" option in the "Recently Used Layers" combobox
        And I press "Add Layer"
        Then I should see "A layer with this name has already been added to the map!"

    Scenario: Remove Layer and reload from Recently Used Layers
        When I click the "DcOsmRoadsCucumber" Dataset
        And I press "Add Layer"
        And I wait 15 "seconds" to see "span.strong" element with text "DcOsmRoadsCucumber"
        And I remove the first layer
        And I press "Add Reference Dataset"        
        When I select the "DcGisRoadsCucumber" option in the "Recently Used Layers" combobox
        And I press "Add Layer"        
        Then I wait 15 "seconds" to see "span.strong" element with text "DcGisRoadsCucumber"
