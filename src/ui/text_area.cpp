// ============================================================================
// Includes
// ----------------------------------------------------------------------------
#include "ui/include/text_area.hpp"

#include "resources/include/asset_registry.hpp"
#include "events/include/sfml_event_manager.hpp"

#include "utils/include/logger.hpp"
#include "utils/include/colors.hpp"
#include "utils/include/null_check.hpp"

#include <string>

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
        , _content_area(std::make_unique<sf::RectangleShape>())
        , _caret(Caret{ .rect = std::make_unique<sf::RectangleShape>(), .index = 0})
        , _view(std::make_unique<sf::View>())
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

            LOG(utils::Log_lvl::TRACE) << "Created text_area " << _data.name;
        }
        else {
            LOG(utils::Log_lvl::WARN) << "Failed to load " << _font_name << " font for text_area " << widget_name;
        }
        register_events();
        set_view();
        reposition_caret(0);
    }

    //--------------------------
    // Class specific functions
    //--------------------------

    // WIP !!!
    void Text_area::draw() {

        NULL_CHECK_VOID(_content_area)
        NULL_CHECK_VOID(_bar_background)
        NULL_CHECK_VOID(_scroll_bar)
        NULL_CHECK_VOID(_text)

        const sf::View old_view = _window.getView();

        if (!has_state(State::HIDDEN)) {
            _window.draw(*_content_area);
            if (has_state(State::SCROLLABLE)) { _window.draw(*_bar_background); }
            if (has_state(State::SCROLLABLE)) { _window.draw(*_scroll_bar); }
            _window.setView(*_view);
            _window.draw(*_text);
            if (_caret.rect && has_state(State::FOCUSED)) {
                const auto& rect { _caret.rect };
                _window.draw(*rect);
            };
            _window.setView(old_view);
        }
    }

    //Not implemented -- Final decision
    void Text_area::update(float const dt) {
        LOG(utils::Log_lvl::WARN) << "Attempting to use unimplemented function.";
        static_cast<void> (dt);
    }

    //--------------------------
    // States
    //--------------------------
    void Text_area::set_state(const uint8_t state) { _state = state; }
    void Text_area::add_state(const uint8_t state) { _state |= state; }
    void Text_area::remove_state(const uint8_t state) { _state &= (~state); }
    bool Text_area::has_state(const uint8_t state) const {
        if ( (_state & state) == state) {
            return true;
        }
        else { return false; }
    }

    //--------------------------
    // Getters
    //--------------------------
    sf::Vector2f Text_area::get_pos() const {

        NULL_CHECK(_content_area)
        return _content_area->getPosition();
    }

    sf::Vector2f Text_area::get_size() const {

        NULL_CHECK(_content_area)
        sf::Vector2f total_size { _content_area->getSize() };
        total_size.x += get_scoll_bar_width();
        return total_size;
    }

    //Not implemented - !!!YET!!!
    sf::Vector2f Text_area::get_scale() const {
        return {};
    }

    sf::Color Text_area::get_content_area_color() const {

        NULL_CHECK(_content_area)
        return _content_area->getFillColor();
    }

    sf::Vector2f Text_area::get_content_area_size() const {

        NULL_CHECK(_content_area)
        return _content_area->getSize();
    }

    sf::Color Text_area::get_caret_color() const {
        NULL_CHECK(_caret.rect)
        return _caret.rect->getFillColor();
    }

    float Text_area::get_scoll_bar_width() const {

        NULL_CHECK(_bar_background)
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

    bool Text_area::is_typeable() const {

        if (has_state(State::TYPING_DISABLED)) { return false; }
        else { return true; }
    }

    //--------------------------
    // Setters
    //--------------------------

    //Arrows not added yet !!!
    void Text_area::set_pos(sf::Vector2f const pos) {

        NULL_CHECK_VOID(_content_area)

        //Set the typing text area position
        _content_area->setPosition(pos);
        reposition_scroll_bar();
        resize_scroll_bar(); //This function will be deleted from here later
        set_view();
        reposition_caret(0);
        //In future text will need to wrap up again
        //In future scrollbar will clamp to the top
    }

    void Text_area::set_size(sf::Vector2f const size) {

        NULL_CHECK_VOID(_content_area)
        NULL_CHECK_VOID(_bar_background)

        _content_area->setSize(size);

        sf::Vector2f bg_bar_size { _bar_background->getSize() };
        bg_bar_size.y = size.y;
        _bar_background->setSize(bg_bar_size);

        //The text_area size has changed, scrollbar will reset to the top
        //along with all contents
        reposition_scroll_bar();
        set_view();
        reposition_caret(0);
        //In future text will need to wrap up again
    }

    //Not implemented !!!YET!!!
    void Text_area::set_scale(sf::Vector2f const scale) {
        static_cast<void> (scale);
    }

    void Text_area::set_content_area_color(const sf::Color color) {

        NULL_CHECK_VOID(_content_area)
        _content_area->setFillColor(color);
    }

    void Text_area::set_content_area_width(const float width) {

        NULL_CHECK_VOID(_content_area)

        sf::Vector2f new_size { _content_area->getSize() };
        new_size.x = width;
        _content_area->setSize(new_size);
        reposition_scroll_bar();
        set_view();
        reposition_caret(0);
    }

    void Text_area::set_content_area_height(const float height) {

        NULL_CHECK_VOID(_content_area)
        NULL_CHECK_VOID(_bar_background)

        sf::Vector2f new_size { _content_area->getSize() };
        new_size.y = height;
        _content_area->setSize(new_size);

        new_size = _bar_background->getSize();
        new_size.y = height;
        _bar_background->setSize(new_size);

        reposition_scroll_bar();
        set_view();
        reposition_caret(0);

    }

    void Text_area::set_caret_color(const sf::Color color) {
        NULL_CHECK_VOID(_caret.rect)
        _caret.rect->setFillColor(color);
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

    void Text_area::set_typeable(const bool flag) {
        if (flag) { remove_state(State::TYPING_DISABLED); }
        else { add_state(State::TYPING_DISABLED); }
    }

    //--------------------------
    // Private functions
    //--------------------------
    void Text_area::reposition_scroll_bar() {

        NULL_CHECK_VOID(_content_area)
        NULL_CHECK_VOID(_bar_background)
        NULL_CHECK_VOID(_scroll_bar)

        const sf::Vector2f rect_pos { _content_area->getPosition() };
        const sf::Vector2f rect_size { _content_area->getSize() };
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

    void Text_area::reposition_caret(const size_t snap_to_pos) {

        NULL_CHECK_VOID(_caret.rect)
        NULL_CHECK_VOID(_text)
        NULL_CHECK_VOID(_font)

        if (snap_to_pos > _text->getString().getSize()) {
            //Do nothing, cannot snap to a string pos that does not exist
        }
        //edge case, there is no characters currently
        else if (snap_to_pos == 0 && _text->getString().getSize() == 0) {

            sf::String string { _text->getString() };
            string.insert(0,'a');
            _text->setString(string);

            const auto& shaped_glyphs { _text->getShapedGlyphs() };
            const float caret_height { _font->getAscent(_text->getCharacterSize()) - _font->getDescent(_text->getCharacterSize()) };
            const float caret_width { _text->getLetterSpacing() * _text->getCharacterSize() * 0.06f};
            _caret.rect->setSize({caret_width, caret_height});

            const sf::Vector2f pen_line_pos { shaped_glyphs[0].position };
            sf::Vector2f caret_pos { pen_line_pos };

            caret_pos.x += shaped_glyphs[0].glyph.bounds.position.x;
            caret_pos.y -= _font->getAscent(_text->getCharacterSize());
            caret_pos = _text->getTransform().transformPoint(caret_pos);
            caret_pos.x -= _caret.rect->getSize().x;

            _caret.rect->setPosition(caret_pos);

            string = _text->getString();
            string.erase(0,1);
            _text->setString(string);
        }
        //Going to the start of the line
        else if (snap_to_pos == 0) {

            const auto& shaped_glyphs { _text->getShapedGlyphs() };
            const float caret_height { _font->getAscent(_text->getCharacterSize()) - _font->getDescent(_text->getCharacterSize()) };
            const float caret_width { _text->getLetterSpacing() * _text->getCharacterSize() * 0.06f};
            _caret.rect->setSize({caret_width, caret_height});

            const sf::Vector2f pen_line_pos { shaped_glyphs[0].position };
            sf::Vector2f caret_pos { pen_line_pos };

            caret_pos.x += shaped_glyphs[0].glyph.bounds.position.x;
            caret_pos.y -= _font->getAscent(_text->getCharacterSize());
            caret_pos = _text->getTransform().transformPoint(caret_pos);
            caret_pos.x -= _caret.rect->getSize().x;

            _caret.rect->setPosition(caret_pos);
        }
        else {
            //Newline character was entered
            if (_text->getString()[snap_to_pos - 1] == U'\n') {

                sf::String string { _text->getString() };
                string.insert(snap_to_pos,'a');
                _text->setString(string);

                const auto& shaped_glyphs { _text->getShapedGlyphs() };
                const float caret_height { _font->getAscent(_text->getCharacterSize()) - _font->getDescent(_text->getCharacterSize()) };
                const float caret_width { _text->getLetterSpacing() * _text->getCharacterSize() * 0.06f};
                _caret.rect->setSize({caret_width, caret_height});

                const sf::Vector2f pen_line_pos { shaped_glyphs[snap_to_pos].position };
                sf::Vector2f caret_pos { pen_line_pos };

                caret_pos.x += shaped_glyphs[snap_to_pos].glyph.bounds.position.x;
                caret_pos.y -= _font->getAscent(_text->getCharacterSize());
                caret_pos = _text->getTransform().transformPoint(caret_pos);
                caret_pos.x -= _caret.rect->getSize().x;

                _caret.rect->setPosition(caret_pos);

                string = _text->getString();
                string.erase(snap_to_pos,1);
                _text->setString(string);
            }
            //Normal character was entered
            else {

                const auto& shaped_glyphs { _text->getShapedGlyphs() };
                const float caret_height { _font->getAscent(_text->getCharacterSize()) - _font->getDescent(_text->getCharacterSize()) };
                const float caret_width { _text->getLetterSpacing() * _text->getCharacterSize() * 0.06f};
                _caret.rect->setSize({caret_width, caret_height});

                const sf::Vector2f pen_line_pos { shaped_glyphs[snap_to_pos - 1].position };
                sf::Vector2f caret_pos { pen_line_pos };

                caret_pos.x += shaped_glyphs[snap_to_pos - 1].glyph.bounds.position.x +
                              shaped_glyphs[snap_to_pos - 1].glyph.bounds.size.x;
                caret_pos.y -= _font->getAscent(_text->getCharacterSize());

                caret_pos = _text->getTransform().transformPoint(caret_pos);
                caret_pos.x -= _caret.rect->getSize().x;

                _caret.rect->setPosition(caret_pos);
            }
        }
    }

    void move_caret(const size_t delta);

    void Text_area::set_view() {

        NULL_CHECK_VOID(_content_area)
        NULL_CHECK_VOID(_view)

        const sf::Vector2f box_pos { _content_area->getPosition() };
        const sf::Vector2f box_size { _content_area->getSize() };
        const sf::Vector2f window_size { _window.getSize() };

        const sf::FloatRect view_setup(
            { box_pos.x  / window_size.x, box_pos.y  / window_size.y },
            { box_size.x / window_size.x, box_size.y / window_size.y }
        );

        if (box_size.x == 0 || box_size.y == 0) {
            LOG(Log_lvl::WARN) << "View setup skipped for text area.\n"
            "View cannot be applied to a 0.0 size for X and Y values.\n"
            "Text will default write to 0.0f position.";
            return;
        }

        _view->setSize(box_size);
        _view->setCenter(_content_area->getGlobalBounds().getCenter());
        _view->setViewport(view_setup);
        _view->setScissor(view_setup);

        // Set text position to the view
        _text->setPosition(box_pos);
        // X and Y axis padding
        _text->move({4.5f, 0.0f});
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
        const float max_height_y { min_height_y + get_content_area_size().y };
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
    }

    //Implement Caret before continuing :) -- Refactor with the caret
    void Text_area::build_string(const char32_t unicode) {

        /*
        Not all may be used or implemented below, but it's good to keep a simple reference to
        some common code values in the case that I have to do logic checks on unicode values.
        And if I ever need to add them I will simply just refer to my enum list below.

        For example: if we pressed backspace I need to know
        it was backspace and handle this unique case.
        */
    enum Unicodes : char32_t {
        BACKSPACE       = 8,      // '\b'
        TABSTOP         = 9,      // '\t'
        LINE_FEED       = 10,     // '\n'  Enter on Unix
        CARRIAGE_RETURN = 13,     // '\r'  Enter on Windows
        ESCAPE          = 27,     // '\x1b'
        SPACE           = 32,     // ' '
        DELETE          = 127,    // DEL key
    };

        NULL_CHECK_VOID(_text)
        sf::String string { _text->getString() };

        if (unicode == Unicodes::BACKSPACE) {
            if (!string.isEmpty()) {
                string.erase(string.getSize() - 1, 1);
                _text->setString(string);
                _caret.index -= 1;
                reposition_caret(_caret.index);
            }
        }
        else if (unicode == Unicodes::CARRIAGE_RETURN) {
            string.insert(string.getSize(), '\n');
            _text->setString(string);
            _caret.index += 1;
            reposition_caret(_caret.index);
        }
        else {
            string.insert(string.getSize(), unicode);
            _text->setString(string);
            _caret.index += 1;
            reposition_caret(_caret.index);
        }
    }

    void Text_area::register_events() {

        auto& event_manager = events::SFML_event_manager::instance();

        event_manager.register_callback(
            events::SFML_event_type::MOUSE_BUTTON_LEFT_HELD,
            [this](events::SFML_event_data const&) {
                _handle_event__mouse_button_left_held();
            },
            listener_id()
        );

        event_manager.register_callback(
            events::SFML_event_type::MOUSE_BUTTON_LEFT_RELEASE,
            [this](events::SFML_event_data const&) {
                _handle_event__mouse_button_left_release();
            },
            listener_id()
        );

        event_manager.register_callback(
            events::SFML_event_type::WINDOW_TEXT_ENTERED,
            [this](events::SFML_event_data const& event_data) {
                _handle_event__window_text_entered(event_data.sfml_event);
            },
            listener_id()
        );
    }

    void Text_area::_handle_event__mouse_button_left_held() {

        NULL_CHECK_VOID(_bar_background)
        const auto mouse_pixels {sf::Mouse::getPosition(_window) };
        const auto mouse_pos {_window.mapPixelToCoords(mouse_pixels) };

        // We are currently dragging
        // lenience will be taken into account for a better user experience
        if (has_state(State::DRAGGING | State::SCROLLABLE)) {
            const sf::Vector2f bar_pos { _bar_background->getPosition() };
            const sf::Vector2f bar_size { _bar_background->getSize() };

            const float min_lenience { bar_pos.x  - 50.0f };
            const float max_lenience { bar_pos.x + bar_size.x + 50.0f };

            if ( !( mouse_pos.x >= min_lenience && mouse_pos.x <= max_lenience) ) {
                remove_state(State::DRAGGING);
                _old_mouse_pos = { 0.0f , 0.0f };
            }
            else {
                scrolling(mouse_pos);
                _old_mouse_pos = mouse_pos;
            }
        }
        //Not currently dragging -- Check if in bounds to drag
        else if (_scroll_bar->getGlobalBounds().contains(mouse_pos) && has_state(State::SCROLLABLE)) {
            //First frame we will just remember where the mouse was,
            //frames fire so fast the user should not even notice a delay until next frame.
            add_state(State::DRAGGING);
            _old_mouse_pos = mouse_pos;
        }
        //Turn dragging off and reset old mouse pos to default
        //This will fire when SCROLLABLE turns to false and we were mid scrolling and need to revoke the task
        else if (has_state(State::DRAGGING)) {
            remove_state(State::DRAGGING);
            _old_mouse_pos = { 0.0f , 0.0f };
        }
        else { /*Mouse was held outside the bounds of our scrollbar. Simply do nothing*/ }
    }

    void Text_area::_handle_event__mouse_button_left_release() {

        NULL_CHECK_VOID(_content_area)
        const auto mouse_pixels { sf::Mouse::getPosition(_window) };
        const auto mouse_pos { _window.mapPixelToCoords(mouse_pixels) };

        /*
        Left release is used to enter the content area
        It will give focus and then if we have focus pressing keys on the keyboard will enable
        you to type and edit the sf::Text object
        */
        if (has_state(State::DRAGGING)) {
            remove_state(State::DRAGGING);
            _old_mouse_pos = { 0.0f , 0.0f };
        }
        else if (_content_area->getGlobalBounds().contains(mouse_pos)) {
            add_state(State::FOCUSED);
            LOG(Log_lvl::DEBUG) << "Focus gained.";
        }
        else {
            remove_state(State::FOCUSED);
            LOG(Log_lvl::DEBUG) << "Focus lost.";
        }
    }

    void Text_area::_handle_event__window_text_entered(const sf::Event& event_data) {

        const auto* const text_entered = event_data.getIf<sf::Event::TextEntered>();
        if ((text_entered)
            && (has_state(State::FOCUSED))
            && (!has_state(State::TYPING_DISABLED))
        ) {
            build_string(text_entered->unicode);
        }
    }
}
