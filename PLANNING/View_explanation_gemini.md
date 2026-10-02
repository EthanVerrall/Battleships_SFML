================================================================================
                    SFML 2D RENDER PIPELINE CHEAT SHEET
================================================================================

   [ GAME WORLD ]                         [ MONITOR SCREEN ]
(World Space: Pixels)                  (Screen Space: Percentages)

 +------------------+                   +------------------+
 |  setSize()       |                   |  setViewport()   |
 |  setCenter()     |  ===============> |  setScissor()    |
 |  (Camera Lens)   |    PROJECTION     |  (Screen Cutout) |
 +------------------+                   +------------------+

--------------------------------------------------------------------------------
1. THE 4 CORE FUNCTIONS BREAKDOWN
--------------------------------------------------------------------------------

STAGE 1: Capturing the Game World (World Space)

• setCenter(x, y) — Where the camera points
  - Unit: World Pixels (sf::Vector2f)
  - What it does: Sets the exact X/Y coordinate in your game world that sits
    directly in the middle of the camera lens.
  - Team Explanation: "This is the physical location in our game world that
    the camera is aiming at."

• setSize(width, height) — How wide the camera lens is
  - Unit: World Pixels (sf::Vector2f)
  - What it does: Defines how big of an area in the game world the camera
    captures. Changing this changes zoom (smaller = zoomed in, larger = zoomed out).
  - Team Explanation: "This is the field of view of our camera lens. It
    determines how much of the world fits into one frame."

--------------------------------------------------------------------------------

STAGE 2: Displaying on the Monitor (Screen Space)

• setViewport(sf::FloatRect) — Where and how big the camera feed appears
  - Unit: Screen Percentages / Ratios (0.0f to 1.0f)
  - What it does: Takes the camera's image feed and scales/positions it onto
    a specific box on the physical window.
  - Team Explanation: "This is the screen real estate assigned to the camera
    feed. If the viewport's aspect ratio doesn't match setSize, the image stretches."

• setScissor(sf::FloatRect) — The pixel mask that crops overflowing graphics
  - Unit: Screen Percentages / Ratios (0.0f to 1.0f)
  - What it does: Refuses to paint any GPU pixels outside the specified
    rectangular region on the monitor, providing a hard clipping boundary.
  - Team Explanation: "This is a stencil taped to the monitor screen. It
    hard-cuts any text, sprites, or borders that bleed past our UI boundaries."

--------------------------------------------------------------------------------
2. EXAMPLE SCENARIO: Minimap on a 1920x1080 Screen
--------------------------------------------------------------------------------

Imagine a game with a massive 10,000 x 10,000 pixel world. You want a
300 x 300 pixel Minimap in the top-right corner of the player's screen
(at X = 1620, Y = 0).

sf::View minimap_view;

// STAGE 1: CAPTURE THE WORLD
minimap_view.setCenter({5000.0f, 5000.0f}); // Point camera at world center
minimap_view.setSize({3000.0f, 3000.0f});   // Capture 3000x3000 area around player

// STAGE 2: DISPLAY ON MONITOR (Convert Pixels to 0.0 - 1.0 Percentages)
// X% = 1620 / 1920 = 0.84375f
// Y% = 0 / 1080    = 0.0f
// W% = 300 / 1920  = 0.15625f
// H% = 300 / 1080  = 0.27777f

// Project 3000x3000 world feed into top-right 300x300 screen box
minimap_view.setViewport(sf::FloatRect({0.84375f, 0.0f}, {0.15625f, 0.27777f}));

// Ensure map icons or radar circles never bleed past the minimap box
minimap_view.setScissor(sf::FloatRect({0.84375f, 0.0f}, {0.15625f, 0.27777f}));


WHAT HAPPENS WHEN YOU DRAW:
1. setCenter & setSize capture a 3000 x 3000 area around world coordinate (5000, 5000).
2. setViewport scales that 3000 x 3000 camera shot down to a 300 x 300 square
   in the top-right screen corner.
3. setScissor ensures that any icon moving to the edge gets cleanly clipped
   instead of spilling onto the main game HUD.

--------------------------------------------------------------------------------
3. QUICK REFERENCE SUMMARY TABLE
--------------------------------------------------------------------------------

+--------------+--------------+------------------+------------------------------------+
| Function     | Space Type   | Input Type       | Mental Model                       |
+--------------+--------------+------------------+------------------------------------+
| setCenter()  | World Space  | Pixels           | Aiming the physical camera lens    |
| setSize()    | World Space  | Pixels           | Zoom / Field of view size          |
| setViewport()| Screen Space | Ratio 0.0 – 1.0  | Projecting feed onto monitor       |
| setScissor() | Screen Space | Ratio 0.0 – 1.0  | Cutting a stencil mask on display  |
+--------------+--------------+------------------+------------------------------------+