Feature: Conflation using Frechet distance and network conflation

  Scenario: I can conflate using the Frechet distance and network conflation advanced options
    Given I am on Hootenanny
    And I resize the window
    And I click Get Started
    And I press "Add Reference Dataset"
    And I click the "ToyTestACucumber" Dataset
    And I press "Add Layer"
    Then I wait 30 "seconds" to see "span.strong" element with text "ToyTestACucumber"
    And I press "Add Secondary Dataset"
    And I click the "ToyTestBCucumber" Dataset
    And I press "Add Layer"
    Then I wait 30 "seconds" to see "span.strong" element with text "ToyTestBCucumber"
    Then I should see "Conflate"
    And I press "Conflate"
    And I fill "saveAs" input with "TTFrechetNetworkCucumber"
    And I press "confAdvOptsLnk"
    And I click the "label" with text "Road Options"
    And I select the "Network" option in the "Network" combobox
    And I click the "label" with text "General Conflation Options"
    And I check the "Frechet Distance Subline Matcher" checkbox
    And I scroll "Apply" element into view and press it
    And I scroll element into view and press "conflate2"
    Then I wait 30 "seconds" to see "Conflating …"
    Then I wait 3 "minutes" to see "TTFrechetNetworkCucumber"
    Then I click the "trash" button
    And I accept the alert
    And I wait 5 "seconds" to not see "TTFrechetNetworkCucumber"
