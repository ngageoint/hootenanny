require 'selenium-cucumber'
require 'capybara'
require 'capybara/cucumber'
require 'capybara/webkit'

# Do Not Remove This File
# Add your custom steps here
# $driver is instance of webdriver use this instance to write your custom code

def combine(e1, e2)
  result = []
  e1.each { |x| result.push(x) }
  e2.each { |x| result.push(x) }
  return result
end

After do
  # Do something after each scenario.
  Capybara.current_session.driver.quit
end

Given /^I am on Hootenanny$/ do
  if Capybara.javascript_driver == :webkit
    #page.driver.allow_url '*'
    page.driver.block_unknown_urls
  end
  hootUrl = ENV["HOOT_ID_URL"]
  if not hootUrl
    # get hoot home
    hootHome = ENV["HOOT_HOME"]
    conf = hootHome + "/hoot-services/src/main/resources/conf/hoot-services.conf"
    localConf = hootHome + "/hoot-services/src/main/resources/conf/local.conf"
    # read services conf
    confHash = Hash[File.read(conf).split("\n").map{|i|i.split('=')}]
    if File.exists?(localConf)
      localHash = Hash[File.read(localConf).split("\n").map{|i|i.split('=')}]
      confHash = confHash.merge(localHash)
    end
    # derive hoot URL
    hootUrl = confHash["coreJobServerUrl"] + "/hootenanny-id/"
  end
  # delete cookies
  browser = Capybara.current_session.driver.browser
  browser.manage.delete_all_cookies
  visit hootUrl
end
 
When /^I fill in "([^"]*)" with "([^"]*)"$/ do |field, value|
  element = find(:xpath, '//input[@placeholder="' + field + '"]')
  element.set(value)
end

When /^I accept an alert$/ do
  accept_alert
end
 
When /^I press "([^"]*)"$/ do |button|
  click_link_or_button(button)
end

# some strangeness going on with these select dataset methods...need to make more robust; Really
# needs to be done with xpath to be more precise, but the problem run into is that the svg datasets
# element is using a namespace without a qualifier (foo:http:/some-namespace).  So, that's breaking
# the xpath queries.  Need to modify the ui code to either remove the namespace from the svg
# element (not great), or add the namespace qualifier to it.

When /^I select reference dataset "([^"]*)"$/ do |text|
  elements = all('g', :text => text)
  if elements[0].text == text
    elements[0].click
  else
    elements[1].click
  end
end

When /^I select secondary dataset "([^"]*)"$/ do |text|
  elements = all('g', :text => text)
  # this logic is the opposite of what I would expect it to be
  if elements[1].text == text
    elements[0].click
  else
    elements[1].click
  end
end
 
When /^I press first "([^"]*)"$/ do |text|
  Capybara.ignore_hidden_elements = false
  elements = all('a', :text => text)
  if elements.empty?
    elements = all(:xpath, '//input[@value="' + text + '"]')
  end
  elements[0].click
  Capybara.ignore_hidden_elements = true
end
 
When /^I press second "([^"]*)"$/ do |text|
  Capybara.ignore_hidden_elements = false
  e1 = all('a', :text => text)
  e2 = all(:xpath, '//input[@value="' + text + '"]')
  elements = combine(e1, e2)
  elements[1].click
  Capybara.ignore_hidden_elements = true
end

When /^I scroll element into view and press "([^"]*)"$/ do |id|
  Capybara.ignore_hidden_elements = false
  element = page.driver.browser.find_element(:id, id)
  page.driver.browser.execute_script("arguments[0].scrollIntoView(true)", element)
  element.click
  Capybara.ignore_hidden_elements = true
end
 
When /^I wait ([0-9]*) ([a-z]+) to see "([^"]*)"$/ do |timeout, unit, text|
  if unit == "seconds"
    multiplier = 1
  elsif unit == "minutes"
    multiplier = 60
  else
    throw :badunits
  end
  oldTimeout = Capybara.default_max_wait_time
  Capybara.default_max_wait_time = Float(timeout) * multiplier
  page.should have_content(text)
  Capybara.default_max_wait_time = oldTimeout
end

When /^I wait ([0-9]*) ([a-z]+) to not see "([^"]*)"$/ do |timeout, unit, text|
  if unit == "seconds"
    multiplier = 1
  elsif unit == "minutes"
    multiplier = 60
  else
    throw :badunits
  end
  oldTimeout = Capybara.default_max_wait_time
  Capybara.default_max_wait_time = Float(timeout) * multiplier
  page.should have_no_content(text)
  Capybara.default_max_wait_time = oldTimeout
end

Then /^I resize the window$/ do
  if Capybara.javascript_driver != :webkit
    window = Capybara.current_session.driver.browser.manage.window
    window.resize_to(1500, 700) # width, height
  end
end

# Some of the scenarios that use this need to use something more robust now, since datasets are now seen multiple times.
Then /^I should see "([^"]*)"$/ do |text|
  page.should have_content(text)
end

Then /^I should not see "([^"]*)"$/ do |text|
  page.should have_no_content(text)
end

# Added for manage tab testing
When(/^I select the "([^"]*)" div$/) do |txt|
  all('div',:text=>txt).last.click
end

Then(/^I select the "([^"]*)" tab$/) do |label|
  find('#settingsSidebar').find('label', :text => label).click
end

When(/^I select the "([^"]*)" option in the "([^"]*)" combobox$/) do |opt, cb|
  combobox = page.find(:css, 'input[placeholder="' + cb + '"]')
  combobox.find(:xpath, '..').find('.combobox-caret').click
  find('a', :text=> opt).click
end

Then(/^I select the "([^"]*)" id$/) do |arg|
  find('#'+arg).click
  sleep 10
end

Then(/^I should see options in this order:$/) do |table|
  expected_order = table.raw.flatten
  actual_order = page.all('#settingsSidebar label').collect(&:text)
  actual_order.should == expected_order
end
