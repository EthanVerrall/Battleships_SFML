// ============================================================================
// Includes
// ----------------------------------------------------------------------------
#include "ui/include/text_area.hpp"
#include "resources/include/asset_registry.hpp"
#include "utils/include/logger.hpp"
#include "utils/include/colors.hpp"

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::ui {

// ============================================================================
// Class Text_area
// ----------------------------------------------------------------------------

    //--------------------------
    // Constructor / Destructor
    //--------------------------
    Text_area::Text_area(
        sf::RenderWindow& window, const
        std::string_view widget_name
        )
        : Widget(window, Widget_data{.name = widget_name, .type = Widget_type::TEXT_AREA})
        , _state(State::DEFAULT)
        , _rect(std::make_unique<sf::RectangleShape>())
        , _scroll_bar(std::make_unique<sf::RectangleShape>())
        , _caret(std::make_unique<sf::RectangleShape>())
        , _font_name("pixel_bold")
    {
        if (auto temp_font = resources::Asset_registry::load_font(_font_name)) {
            _font = std::move(temp_font);
            _text = std::make_unique<sf::Text> (*_font);
            LOG(utils::Log_lvl::TRACE) << "Created text_area " << _data.name;
        }
        else {
            LOG(utils::Log_lvl::WARN) << "Failed to load " << _font_name << " font for text_area " << widget_name;
        }
    }

    //--------------------------
    // Class specific functions
    //--------------------------
    void Text_area::draw() {
        if (_rect) { _window.draw(*_rect); }
        if (_scroll_bar && has_state(State::SCROLLABLE)) { _window.draw(*_scroll_bar); }
    }

    void Text_area::update(float const dt) { //Not implemented
        //Do nothing for now
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

        if (_rect) { return _rect->getGlobalBounds().position; }
        else {
            LOG(utils::Log_lvl::WARN) << _data.name << " _rect member is nullptr.";
            return sf::Vector2f {0.0f, 0.0f};
        }
    }

    sf::Vector2f Text_area::get_size() const {

        if (_rect) { return _rect->getGlobalBounds().size; }
        else {
            LOG(utils::Log_lvl::WARN) << _data.name << " _rect member is nullptr.";
            return sf::Vector2f {0.0f, 0.0f};
        }
    }

    sf::Vector2f Text_area::get_scale() const { return {}; } //Not implemented

    sf::Color Text_area::get_rect_color() const {
        if (_rect) { return _rect->getFillColor(); }
        else {
            LOG(utils::Log_lvl::WARN) << _data.name << " _rect member is nullptr.";
            return utils::colors::TEXT_AREA_RECT;
        }
    }

    float Text_area::get_scroll_bar_width() const {
        if (_scroll_bar) {  return _scroll_bar->getSize().x; }
        else { LOG(utils::Log_lvl::WARN) << _data.name << " _scroll_bar member is nullptr.";
            return 0.0f;
        }
    }

    sf::Color Text_area::get_scroll_bar_color() const {
        if (_scroll_bar) {  return _scroll_bar->getFillColor(); }
        else { LOG(utils::Log_lvl::WARN) << _data.name << " _scroll_bar member is nullptr.";
            return utils::colors::TEXT_AREA_SCROLL_BAR;
        }
    }

    bool Text_area::is_scrollable() const {
        if (has_state(State::SCROLLABLE)) { return true; }
        else { return false; }
    }

    //--------------------------
    // Setters
    //--------------------------
    void Text_area::set_pos(sf::Vector2f const pos) {
        if (_rect) { _rect->setPosition(pos); }
        else { LOG(utils::Log_lvl::WARN) << _data.name << " _rect member is nullptr."; }

        if (_scroll_bar) {
            const float scroll_bar_width { get_scroll_bar_width() };
            const sf::Vector2f rect_pos { get_pos() };
            const sf::Vector2f rect_size { get_size() };

            const sf::Vector2f scroll_bar_pos { (rect_pos.x + rect_size.x) - scroll_bar_width ,rect_pos.y };
            _scroll_bar->setPosition(scroll_bar_pos);
        }
        else {
            LOG(utils::Log_lvl::WARN) << _data.name << " _scroll_bar member is nullptr.";
        }
    }

    void Text_area::set_size(sf::Vector2f const size) {
        if (_rect) { _rect->setSize(size); }
        else { LOG(utils::Log_lvl::WARN) << _data.name << " _rect member is nullptr."; }

        if (_scroll_bar) {
            sf::Vector2f temp { _scroll_bar->getSize() };
            temp.y = size.y;
            _scroll_bar->setSize(temp);
        }
        else {
            LOG(utils::Log_lvl::WARN) << _data.name << " _scroll_bar member is nullptr.";
        }
    }

    void Text_area::set_scale(sf::Vector2f const scale) { static_cast<void> (scale); } //Not implemented

    void Text_area::set_rect_color(const sf::Color color) {
        if (_rect) { _rect->setFillColor(color); }
        else { LOG(utils::Log_lvl::WARN) << _data.name << " _rect member is nullptr."; }
    }

    void Text_area::set_scroll_bar_width(const float width) {
        if (_scroll_bar) {
            sf::Vector2f temp {_scroll_bar->getSize() };
            temp.x = width;
            _scroll_bar->setSize(temp);
        }
        else { LOG(utils::Log_lvl::WARN) << _data.name << " _scroll_bar member is nullptr."; }
    }

    void Text_area::set_scroll_bar_color(const sf::Color color) {
        if (_scroll_bar) {
            _scroll_bar->setFillColor(color);
        }
        else { LOG(utils::Log_lvl::WARN) << _data.name << " _scroll_bar member is nullptr."; }
    }

    void Text_area::set_scrollable(const bool flag) {
        if (flag) { add_state(State::SCROLLABLE); }
        else { remove_state(State::SCROLLABLE); }
    }
}
