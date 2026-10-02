// ============================================================================
// Includes
// ----------------------------------------------------------------------------
#include "ui/include/text_area.hpp"

#include "resources/include/asset_registry.hpp"
#include "events/include/sfml_event_manager.hpp"

#include "utils/include/logger.hpp"
#include "utils/include/colors.hpp"
#include "utils/include/null_check.hpp"

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

using namespace battleships::utils;
namespace battleships::ui {

// ============================================================================
// Class Text_area
// ----------------------------------------------------------------------------

    //--------------------------
    // Constructor / Destructor
    //--------------------------
    Text_area::Text_area(
        sf::RenderWindow& window,
        const std::string_view widget_name
        )
        : Widget(window, Widget_data{.name = widget_name, .type = Widget_type::TEXT_AREA})
        , _state(State::DEFAULT)
        , _rect(std::make_unique<sf::RectangleShape>())
        , _caret(std::make_unique<sf::RectangleShape>())
        , _bar_background(std::make_unique<sf::RectangleShape>())
        , _scroll_bar(std::make_unique<sf::RectangleShape>())
        , _up_arrow(std::make_unique<Button>(_window, "up arrow"))
        , _down_arrow(std::make_unique<Button>(_window, "down arrow"))
        , _old_mouse_pos{0.0f, 0.0f}
        , _font_name("pixel_bold")
    {
        if (auto temp_font = resources::Asset_registry::load_font(_font_name)) {
            _font = std::move(temp_font);
            _text = std::make_unique<sf::Text> (*_font);
            register_events();

            LOG(utils::Log_lvl::TRACE) << "Created text_area " << _data.name;
        }
        else {
            LOG(utils::Log_lvl::WARN) << "Failed to load " << _font_name << " font for text_area " << widget_name;
        }
    }

    //--------------------------
    // Class specific functions
    //--------------------------

    // WIP !!!
    void Text_area::draw() {

        NULL_CHECK_VOID(_rect)
        NULL_CHECK_VOID(_bar_background)
        NULL_CHECK_VOID(_scroll_bar)

        if (!has_state(State::HIDDEN)) {
            _window.draw(*_rect);
            if (has_state(State::SCROLLABLE)) { _window.draw(*_bar_background); }
            if (has_state(State::SCROLLABLE)) { _window.draw(*_scroll_bar); }
        }
    }

    void Text_area::update(float const dt) { //Not implemented -- Do nothing
        static_cast<void> (dt);
    }

    //--------------------------
    // States
    //--------------------------
    void Text_area::set_state(const State state) { _state = state; }
    void Text_area::add_state(const State state) { _state |= state; }
    void Text_area::remove_state(const State state) { _state &= (~state); }
    bool Text_area::has_state(const State state) const {
        if ( (_state & state) == state) {
            return true;
        }
        else { return false; }
    }

    //--------------------------
    // Getters
    //--------------------------
    sf::Vector2f Text_area::get_pos() const {

        NULL_CHECK(_rect)
        return _rect->getPosition();
    }

    sf::Vector2f Text_area::get_size() const {

        NULL_CHECK(_rect)
        sf::Vector2f total_size { _rect->getSize() };
        total_size.x += get_scoll_bar_width();
        return total_size;
    }

    //Not implemented
    sf::Vector2f Text_area::get_scale() const {
        LOG(utils::Log_lvl::WARN) << "Attempting to use unimplemented function.";
        return {};
    }

    sf::Color Text_area::get_rect_color() const {

        NULL_CHECK(_rect)
        return _rect->getFillColor();
    }

    float Text_area::get_box_width() const {

        NULL_CHECK(_rect)
        return _rect->getSize().x;
    }

    float Text_area::get_box_height() const {

        NULL_CHECK(_rect)
        return _rect->getSize().y;
    }

    float Text_area::get_scoll_bar_width() const {

        NULL_CHECK(_bar_background)
        NULL_CHECK(_scroll_bar)

        return _bar_background->getSize().x;
    }

    sf::Color Text_area::get_scroll_bar_bg_color() const {

        NULL_CHECK(_bar_background)
        return _bar_background->getFillColor();
    }

    sf::Color Text_area::get_scroll_bar_color() const {

        NULL_CHECK(_scroll_bar)
        return _scroll_bar->getFillColor();
    }

    bool Text_area::is_scrollable() const {

        if (has_state(State::SCROLLABLE)) { return true; }
        else { return false; }
    }

    bool Text_area::is_visible() const {

        if (has_state(State::HIDDEN)) { return false; }
        else { return true; }
    }

    //--------------------------
    // Setters
    //--------------------------

    //Arrows not added yet !!!
    void Text_area::set_pos(sf::Vector2f const pos) {

        NULL_CHECK_VOID(_rect)

        //Set the typing text area position
        _rect->setPosition(pos);
        reposition_scroll_bar();

        resize_scroll_bar(); //This function will be deleted from here later
        //In future text will need to wrap up again
    }

    void Text_area::set_size(sf::Vector2f const size) {

        NULL_CHECK_VOID(_rect)
        NULL_CHECK_VOID(_bar_background)

        _rect->setSize(size);

        sf::Vector2f bg_bar_size { _bar_background->getSize() };
        bg_bar_size.y = size.y;
        _bar_background->setSize(bg_bar_size);

        //The text_area size has changed, scrollbar will reset to the top
        //along with all contents
        reposition_scroll_bar();

        //In future text will need to wrap up again
    }

    void Text_area::set_scale(sf::Vector2f const scale) { static_cast<void> (scale); } //Not implemented

    void Text_area::set_rect_color(const sf::Color color) {

        NULL_CHECK_VOID(_rect)
        _rect->setFillColor(color);
    }

    void Text_area::set_box_width(const float width) {

        NULL_CHECK_VOID(_rect)

        sf::Vector2f new_size { _rect->getSize() };
        new_size.x = width;
        _rect->setSize(new_size);
        reposition_scroll_bar();
    }

    void Text_area::set_box_height(const float height) {

        NULL_CHECK_VOID(_rect)

        sf::Vector2f new_size { _rect->getSize() };
        new_size.y = height;
        _rect->setSize(new_size);
        reposition_scroll_bar();
    }

    void Text_area::set_scroll_bar_width(const float width) {

        NULL_CHECK_VOID(_bar_background)
        NULL_CHECK_VOID(_scroll_bar)

        sf::Vector2f bar_size {_bar_background->getSize() };
        bar_size.x = width;
        _bar_background->setSize(bar_size);

        bar_size = _scroll_bar->getSize();
        bar_size.x = width;
        _scroll_bar->setSize(bar_size);

        //The scrollbar width has changed, scrollbar will reset to the top
        //along with all contents
        reposition_scroll_bar();

        //In future text will need to wrap up again
    }

    void Text_area::set_scroll_bar_bg_color(const sf::Color color) {

        NULL_CHECK_VOID(_bar_background)
        _bar_background->setFillColor(color);
    }

    void Text_area::set_scroll_bar_color(const sf::Color color) {

        NULL_CHECK_VOID(_scroll_bar)
        _scroll_bar->setFillColor(color);
    }

    void Text_area::set_scrollable(const bool flag) {
        if (flag) { add_state(State::SCROLLABLE); }
        else { remove_state(State::SCROLLABLE); }
    }

    void Text_area::set_visible(const bool flag) {
        if (flag) { remove_state(State::HIDDEN); }
        else { add_state(State::HIDDEN); }
    }

    //--------------------------
    // Private functions
    //--------------------------
    void Text_area::reposition_scroll_bar() {

        NULL_CHECK_VOID(_rect)
        NULL_CHECK_VOID(_bar_background)
        NULL_CHECK_VOID(_scroll_bar)

        const sf::Vector2f rect_pos { _rect->getPosition() };
        const sf::Vector2f rect_size { _rect->getSize() };
        const sf::Vector2f bar_pos { rect_pos.x + rect_size.x, rect_pos.y };

        //Clipping the bg_bar to the right of the text_area
        _bar_background->setPosition(bar_pos);

        //Clipping the scroll_bar to the right of the text_area
        _scroll_bar->setPosition(bar_pos);
    }

    void Text_area::resize_scroll_bar() {

        //For now dont worry about this function
        //In future the text and wrapping will determine the height of the scroll bar to mimic real life scroll bars
        //The more text, the further it must scroll and smaller it will be.
        //For now an imaginary height just for test drawing
        auto temp { _scroll_bar->getSize() };
        temp.y = 50.0f;
        _scroll_bar->setSize(temp);
    }

    void Text_area::scrolling(const sf::Vector2f mouse_pos) {

        NULL_CHECK_VOID(_scroll_bar)
        NULL_CHECK_VOID(_bar_background)

        //Change in mouse pos after 2 or more frames
        const float displacement { mouse_pos.y - _old_mouse_pos.y };

        //Not const, we need to clamp so our scroll bar doesn't exceed the height of the widget!
        sf::Vector2f final_pos { _scroll_bar->getPosition() };
        final_pos.y += displacement;

        const float min_height_y { _bar_background->getPosition().y };
        const float max_height_y { min_height_y + get_box_height() };
        const sf::Vector2f min_height_vect { _bar_background->getPosition() };
        sf::Vector2f max_height_vect { _bar_background->getPosition() };
        max_height_vect.y = max_height_y;
        max_height_vect.y -= _scroll_bar->getSize().y;

        //Clamping on a vector2f
        if (final_pos.y <= min_height_vect.y ) {
            final_pos = min_height_vect;
        }
        else if (final_pos.y >= max_height_vect.y) {
            final_pos = max_height_vect;
        }
        else {
            //Do nothing
        }

        _scroll_bar->setPosition(final_pos);
        _old_mouse_pos = mouse_pos;
    }

    void Text_area::register_events() {

        auto& event_manager = events::SFML_event_manager::instance();

        event_manager.register_callback(
            events::SFML_event_type::MOUSE_MOVED,
            [this](events::SFML_event_data const&) {
                _handle_event__mouse_moved();
            },
            listener_id()
        );

        event_manager.register_callback(
            events::SFML_event_type::MOUSE_BUTTON_LEFT_HELD,
            [this](events::SFML_event_data const&) {
                _handle_event__mouse_button_left_held();
            },
            listener_id()
        );
    }

    void Text_area::_handle_event__mouse_moved() {

        NULL_CHECK_VOID(_scroll_bar)
        NULL_CHECK_VOID(_rect)

        const auto mouse_pixel_coords { sf::Mouse::getPosition(_window) };
        const auto mouse_pos { _window.mapPixelToCoords(mouse_pixel_coords) };

        //Checking if we are hovering the scroll bar
        if (_scroll_bar->getGlobalBounds().contains(mouse_pos)) {
            add_state(State::HOVERING_BAR);
        }
        else { remove_state(State::HOVERING_BAR); }

        //Checking if we are hovering the _rect -- Area the user types text into
        if (_rect->getGlobalBounds().contains(mouse_pos)) {
            add_state(State::HOVERING_RECT);
        }
        else { remove_state(State::HOVERING_RECT); }
    }

    void Text_area::_handle_event__mouse_button_left_held() {

        //Current mouse pos for this frame
        const auto mouse_pixel_coords { sf::Mouse::getPosition(_window) };
        const auto mouse_pos { _window.mapPixelToCoords(mouse_pixel_coords) };

        if (has_state(State::HOVERING_BAR | State::SCROLLABLE) && !has_state(State::DRAGGING)) {
            add_state(State::DRAGGING);
            //First frame we will just remember where the mouse was,
            //frames fire so fast the user should not even notice a delay until next frame.
            _old_mouse_pos = mouse_pos;
        }
        else if (has_state(State::HOVERING_BAR | State::SCROLLABLE | State::DRAGGING)) {
            //Second frame and onwards of dragging
            //Now we snap the bar and adjust the view contents of our text area
            scrolling(mouse_pos);
        }
        else {
            //Reset mouse pos to zero, dragging finished
            remove_state(State::DRAGGING);
            _old_mouse_pos = {0.0f , 0.0f};
        }
    }
}
