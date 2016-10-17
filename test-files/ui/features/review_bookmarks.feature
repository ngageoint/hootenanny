Feature: Review Bookmarks

    Scenario: Conflate AllDataTypes
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
        Then I wait 30 "seconds" to see "Conflate"
        And I press "Conflate"
        Then I fill "saveAs" input with "mergedBookmarkTest"
        And I scroll element into view and press "conflate2"
        Then I wait 5 "seconds" to see "Conflating …"
        Then I wait 2 "minutes" to see "mergedBookmarkTest"

    Scenario: Create Bookmark with Note
        Then I click the "Bookmark Review" link
        And I wait 30 "seconds" to see "Bookmark Review"
        And I wait 30 "seconds" to see "Title"
        And I wait 30 "seconds" to see "Description"
        And I wait 30 "seconds" to see "Note (Optional)"
        And I wait 30 "seconds" to see "Creator Email"
        Then I press "big.loud" span with text "Save"
        And I accept the alert
        Then I type "Cucumber Bookmark 1" in input "reviewBookmarkTitle"
        Then I type "Bookmark for cucumber testing" in input "reviewBookmarkDescription"
        Then I type "This is a review bookmark" in input "reviewBookmarkNote"
        Then I type "cucumber1@hootenanny.digitalglobe.com" in input "reviewBookmarkCreatorEmail"
        Then I press "big.loud" span with text "Save"

    Scenario: Create Bookmark without Note
        Then I click the "Bookmark Review" link
        And I wait 30 "seconds" to see "Bookmark Review"
        And I wait 30 "seconds" to see "Title"
        And I wait 30 "seconds" to see "Description"
        And I wait 30 "seconds" to see "Note (Optional)"
        And I wait 30 "seconds" to see "Creator Email"
        Then I type "Cucumber Bookmark 2" in input "reviewBookmarkTitle"
        Then I type "Bookmark for cucumber testing" in input "reviewBookmarkDescription"
        Then I type "cucumber2@hootenanny.digitalglobe.com" in input "reviewBookmarkCreatorEmail"
        Then I press "big.loud" span with text "Save"

    Scenario: Add new comment to bookmark
        Then I select the "sprocket" div
        And I click on the "Review Bookmarks" option in the "settingsSidebar"
        And I click the "Cucumber Bookmark 1:" link
        Then I wait 30 "seconds" to see "Cucumber Bookmark 1"
        And I wait 30 "seconds" to see "User cucumber1@hootenanny.digitalglobe.com commented"
        Then I type "I will review this bookmark" in input "bmkNoteTextNew"
        And I press "big.loud" span with text "comment"

    Scenario: Reload review
        Then I click the "reload" icon
        And I wait 30 "seconds" to see "Cucumber Bookmark 1"
        And I wait 30 "seconds" to see "User cucumber1@hootenanny.digitalglobe.com commented"

    Scenario: Go to review
        Then I select the "loadReview" div
        And I accept the alert
        And I wait 30 seconds
        And I accept the alert
        Then I wait 30 "seconds" to see "mergedBookmarkTest"
        Then I wait 30 "seconds" to not see "Please wait while panning to review item."
        And I click on ".activeReviewFeature"
        Then I click the "preset-close" button
        # Remove layer
        Then I click the "trash" button
        And I accept the alert

    Scenario: Filter Bookmarks
        Then I select the "sprocket" div
        And I click on the "Review Bookmarks" option in the "settingsSidebar"
        And I wait 30 "seconds" to see "Cucumber Bookmark 1"
        And I wait 30 "seconds" to see "Cucumber Bookmark 2"
        Then I click on the "Sort By" label
        And I wait 30 "seconds" to see "Created At (asc)"
        Then I choose "Created At (dsc)" radio button
        And I should see "Cucumber Bookmark 2" bookmark first and "Cucumber Bookmark 1" bookmark second
        Then I click on the "Filter By Creator" label
        And I wait 30 "seconds" to see "cucumber1@hootenanny.digitalglobe.com"
        And I wait 30 "seconds" to see "cucumber2@hootenanny.digitalglobe.com"
        And I uncheck the "cucumber1@hootenanny.digitalglobe.com" checkbox
        And I wait 30 "seconds" to not see "Cucumber Bookmark 1"
        And I wait 30 "seconds" to see "Cucumber Bookmark 2"
        And I check the "cucumber1@hootenanny.digitalglobe.com" checkbox
        And I uncheck the "cucumber2@hootenanny.digitalglobe.com" checkbox
        And I wait 30 "seconds" to see "Cucumber Bookmark 1"
        And I wait 30 "seconds" to not see "Cucumber Bookmark 2"
        And I check the "cucumber2@hootenanny.digitalglobe.com" checkbox
        And I wait 30 "seconds" to see "Cucumber Bookmark 2"
        Then I click on the "Filter By Layers" label
        And I wait 30 "seconds" to see "mergedBookmarkTest"
        Then I uncheck the "mergedBookmarkTest" checkbox
        And I wait 30 "seconds" to not see "Cucumber Bookmark 1"
        And I wait 30 "seconds" to not see "Cucumber Bookmark 2"
        And I check the "mergedBookmarkTest" checkbox
        And I wait 30 "seconds" to see "Cucumber Bookmark 1"
        And I wait 30 "seconds" to see "Cucumber Bookmark 2"
        Then I uncheck the "mergedBookmarkTest" checkbox
        And I press "big.loud" span with text "Reset"
        And I wait 30 "seconds" to see "Cucumber Bookmark 1"
        And I wait 30 "seconds" to see "Cucumber Bookmark 2"
        And I should see checkbox "mergedBookmarkTest" checked

    Scenario: Delete Bookmark
        Then I click on the "trash" button in the "reviewBookmarksContent"
        And I accept the alert

    Scenario: Delete Layer with active bookmark
        Then I click on the "Datasets" option in the "settingsSidebar"
        And I press "Refresh Datasets"
        And I wait
        When I click the "mergedBookmarkTest" Dataset
        When I context click the "mergedBookmarkTest" Dataset
        And I click the "Delete (1)" context menu item
        And I accept the alert
        And I accept the alert
        Then I click on the "Review Bookmarks" option in the "settingsSidebar"
        And I wait 5 "seconds" to not see "Cucumber Bookmark"
        Then I click on the "Datasets" option in the "settingsSidebar"
        #And I wait 5 "seconds" to not see "mergedBookmarkTest"
