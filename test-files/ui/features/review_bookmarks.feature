Feature: Review Bookmarks

    Scenario: Conflate AllDataTypes
        Given I am on Hootenanny
        And I click Get Started
        And I press "Add Reference Dataset"
        And I click the "AllDataTypesACucumber" Dataset
        And I press "Add Layer"
        Then I wait 30 "seconds" to see "AllDataTypesACucumber"
        And I press "Add Secondary Dataset"
        And I click the "AllDataTypesBCucumber" Dataset
        And I press "Add Layer"
        Then I wait 30 "seconds" to see "AllDataTypesBCucumber"
        Then I should see "Conflate"
        And I press "Conflate"
        Then I fill "saveAs" input with "mergedBookmarkTest"
        And I scroll element into view and press "conflate2"
        Then I wait 5 "seconds" to see "Conflating …"
        Then I wait 2 "minutes" to see "mergedBookmarkTest"

    Scenario: Create Bookmark with Note
        Then I click the "Bookmark Review" link
        And I should see "Bookmark Review"
        And I should see "Title"
        And I should see "Description"
        And I should see "Note (Optional)"
        And I should see "Creator Email"
        Then I press "Save" big loud span
        And I accept the alert
        Then I type "Cucumber Bookmark 1" in input "reviewBookmarkTitle"
        Then I type "Bookmark for cucumber testing" in input "reviewBookmarkDescription"
        Then I type "This is a review bookmark" in input "reviewBookmarkNote"
        Then I type "cucumber@hootenanny.digitalglobe.com" in input "reviewBookmarkCreatorEmail"
        Then I press "Save" big loud span

    Scenario: Create Bookmark without Note
        Then I click the "Bookmark Review" link
        And I should see "Bookmark Review"
        And I should see "Title"
        And I should see "Description"
        And I should see "Note (Optional)"
        And I should see "Creator Email"
        Then I type "Cucumber Bookmark 2" in input "reviewBookmarkTitle"
        Then I type "Bookmark for cucumber testing" in input "reviewBookmarkDescription"
        Then I type "cucumber@hootenanny.digitalglobe.com" in input "reviewBookmarkCreatorEmail"
        Then I press "Save" big loud span

    Scenario: Add new comment to bookmark
        Then I select the "sprocket" div
        And I click on the "Review Bookmarks" option in the "settingsSidebar"
        And I click the "Cucumber Bookmark 1:" link
        Then I should see "Cucumber Bookmark 1"
        And I should see "User cucumber@hootenanny.digitalglobe.com commented"
        Then I type "I will review this bookmark" in input "bmkNoteTextNew"
        And I press "comment" big loud span

    Scenario: Reload review
        Then I click the "reload" icon
        And I should see "Cucumber Bookmark 1"
        And I should see "User cucumber@hootenanny.digitalglobe.com commented"

    Scenario: Go to review
        Then I select the "loadReview" div
        And I accept the alert
        And I wait 10 seconds
        And I accept the alert
        Then I wait 30 "seconds" to see "mergedBookmarkTest"
        And I click on ".activeReviewFeature"
        Then I click the "preset-close" button
        # Remove layer
        Then I click the "trash" button
        And I accept the alert

    Scenario: Filter Bookmarks
        Then I select the "sprocket" div
        And I click on the "Review Bookmarks" option in the "settingsSidebar"
        Then I select the "reviewBookmarksFilterByMapIdDiv" div
        Then I click the "mergedBookmarkTest" link under "reviewBookmarksFilterByMapIdDiv"
        And I should see "Cucumber Bookmark"
        Then I select the "reviewBookmarksFilterByCreatorDiv" div
        Then I click the "cucumber@hootenanny.digitalglobe.com" link under "reviewBookmarksFilterByCreatorDiv"
        And I should see "Cucumber Bookmark"
        Then I select the "btnClearFilters" div

    Scenario: Delete Bookmark
        Then I click on the "trash" button in the "reviewBookmarksContent"
        And I accept the alert

    Scenario: Delete Layer with active bookmark
        Then I click on the "Datasets" option in the "settingsSidebar"
        And I click on "#btnDatasetRefresh"
        And I wait
        When I click the "mergedBookmarkTest" Dataset
        When I context click the "mergedBookmarkTest" Dataset
        And I click the "Delete (1)" context menu item
        And I accept the alert
        And I accept the alert
        Then I wait 30 "seconds" to not see "mergedBookmarkTest"
        Then I click on the "Review Bookmarks" option in the "settingsSidebar"
        And I should not see "Cucumber Bookmark"
