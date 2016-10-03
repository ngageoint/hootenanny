# features/support/mouse_click.rb
Capybara::Node::Element.class_eval do
  def click_at(x, y)
    driver.browser.action.move_to(native, x.to_i, y.to_i).click.perform
  end
  def double_click_at(x, y)
    driver.browser.action.move_to(native, x.to_i, y.to_i).double_click.perform
  end
  def context_click
    driver.browser.action.context_click(self.native).perform
  end
end