Feature: Manually resolve reviews feature

  Scenario: I can conflate the AllDataTypes data, manually resolve some reviews, and merge some POIs during the review process
    Given I am on Hootenanny
    And I resize the window
    And I click Get Started
    And I press "Add Reference Dataset"
    And I click the "AllDataTypesACucumber" Dataset
    And I press "Add Layer"
    Then I wait 15 "seconds" to see "span.strong" element with text "AllDataTypesACucumber"
    And I press "Add Secondary Dataset"
    And I click the "AllDataTypesBCucumber" Dataset
    And I press "Add Layer"
    Then I wait 15 "seconds" to see "span.strong" element with text "AllDataTypesBCucumber"
    Then I should see "Conflate"
    And I press "Conflate"
    And I append "saveAs" input with "_Cucumber"
    And I scroll element into view and press "conflate2"
    Then I wait 30 "seconds" to see "Conflating …"
    Then I wait 3 "minutes" to see "Merged_AllDataTypes"
    Then I should see "Complete Review"
    Then I wait 30 "seconds" to see "Reviews remaining: 8 (Resolved: 0)"
    Then I should see "Hide Table"
    Then I should see "name"
    Then I should see a "a.previous" on the map
    Then I should see a "a.next" on the map
    Then I should see "Resolved"
    #Add a new point and edit name tag
    When I click the "add-point" button
    And I hover over "#map"
    And I click the "map" at "400","100"
    And I click the "div.label" with text "Park"
    And I type "Test Name" in input "preset-input-name"
    And I click the "All tags" link
    Then I should see these tags in the table:
        | leisure | park |
        | name | Test Name |
    #Edit a review feature
    When I select a node map feature with class "activeReviewFeature"
    And I type "Edit Name" in input "preset-input-name"
    Then I wait 2 "seconds" to see "div.tag-table td.f1" element with text "Edit Name"

    #Then I should see "Merge"
    #And I press "Merge"
    #Then I wait 30 "seconds" to see "Resolved"
    #And I hover over ".layer-data"
    #And I click the "map" at "100","100"
    And I press "Resolved"
    #This should work too
    #And I click the "r" key
    Then I wait 30 "seconds" to see "Reviews remaining: 7 (Resolved: 1)"
    And I press "Resolved"
    Then I wait 30 "seconds" to see "Reviews remaining: 6 (Resolved: 2)"
    #Click the review item box
    And I hover over "#map"
    When I click the review item column in the tag table
    Then I should see "Edit feature: "
    And I click on "a.next"
    And I hover over "#map"
    Then I should not see "Edit feature: "
    When I click the review item column in the tag table
    Then I should see "Edit feature: "
    And I click on "a.next"
    And I hover over "#map"
    Then I should not see "Edit feature: "
    When I click the review item column in the tag table
    Then I should see "Edit feature: "
    And I click on "a.next"
    And I hover over "#map"
    Then I should not see "Edit feature: "
    When I click the review item column in the tag table
    Then I should see "Edit feature: "
    And I click on "a.next"
    And I hover over "#map"
    Then I should not see "Edit feature: "
    When I click the review item column in the tag table
    Then I should see "Edit feature: "
    And I click on "a.next"
    And I hover over "#map"
    Then I should not see "Edit feature: "
    When I click the review item column in the tag table
    Then I should see "Edit feature: "
    And I click on "a.next"
    And I hover over "#map"
    Then I should not see "Edit feature: "
    When I click the review item column in the tag table
    Then I should see "Edit feature: "
    And I click on "a.next"
    And I hover over "#map"
    Then I should not see "Edit feature: "
    When I click the review item column in the tag table
    Then I should see "Edit feature: "
    And I click on "a.next"
    And I hover over "#map"
    Then I should not see "Edit feature: "
    When I click the review item column in the tag table
    Then I should see "Edit feature: "
    And I click on "a.next"
    And I hover over "#map"
    Then I should not see "Edit feature: "
    #Click the review against item box
    #Click random feature
    And I press "Resolved"
    Then I wait 30 "seconds" to see "Reviews remaining: 5 (Resolved: 3)"
    Then I wait 5 seconds to see "g.activeReviewFeature" on the map
    Then I should see a "g.activeReviewFeature2" on the map
    And I press "Resolved"
    Then I wait 30 "seconds" to see "Reviews remaining: 4 (Resolved: 4)"
    And I press "Resolved"
    Then I wait 30 "seconds" to see "Reviews remaining: 3 (Resolved: 5)"
    And I press "Resolved"
    Then I wait 30 "seconds" to see "Reviews remaining: 2 (Resolved: 6)"
    And I press "Resolved"
    Then I wait 30 "seconds" to see "Reviews remaining: 1 (Resolved: 7)"
    And I press "Merge"
    Then I wait 30 "seconds" to not see "Please wait while merging review items."
    And I click on "a.next"
    Then I wait 5 "seconds" to see "Please resolve or undo the current feature changes before proceeding to the next review."
    And I click on "a.previous"
    Then I wait 5 "seconds" to see "Please resolve or undo the current feature changes before proceeding to the next review."
    And I press "Resolved"
    Then I wait 30 "seconds" to see "There are no more available features to review."
    Then I should not see "Reviews remaining:"
    Then I should not see "Hide Table"
    Then I should not see "name"
    Then I should not see "a.previous"
    Then I should not see "a.next"
    Then I should not see "Resolved"
