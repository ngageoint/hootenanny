Feature: Folder and conflation options control
    
    Scenario: I can cancel advanced conflation options 
        Given I am on Hootenanny
        And I resize the window
        And I click Get Started
        And I press "Add Reference Dataset"
        And I click the "AllDataTypesACucumber" Dataset
        And I press "Add Layer"
        Then I wait 30 "seconds" to see "span.strong" element with text "AllDataTypesACucumber"
        And I press "Add Secondary Dataset"
        And I click the "AllDataTypesBCucumber" Dataset
        And I press "Add Layer"
        Then I wait 30 "seconds" to see "span.strong" element with text "AllDataTypesBCucumber"
        Then I should see "Conflate"
        And I press "Conflate"
        And I press "confAdvOptsLnk"
        And I click the "label" with text "General Conflation Options"
        And I check the "Frechet Distance" checkbox
        And I click the "label" with text "General Conflation Options"
        And I scroll "Cancel" element into view and press it

    Scenario: I will be warned when inputing an invalid folder name before conflation
        When I fill "newfoldername" input with "@#&%^"
        Then I click on "#map"
        Then I should see an invalid name warning for ".newfoldername" input
        When I fill "saveAs" input with "@#&%^"
        Then I click on "#map"
        Then I should see an invalid name warning for ".saveAs" input
        Then I remove the first layer
        Then I click the "trash" button
        Then I accept the alert
