// perkunas_events.h

#ifndef INCLUDED_PERKUNAS_EVENTS
#define INCLUDED_PERKUNAS_EVENTS

#include "perkunas_types.h"

namespace perkunas
{
	namespace event
	{
		//enum class EventType
		//{
	 //       FIRSTEVENT     = 0,     /**< Unused (do not remove) */
  //          /* Application events */
  //          QUIT           = 0x100, /**< User-requested quit */
  //          /* These application events have special meaning on iOS, see README-ios.md for details */
  //          APP_TERMINATING,        /**< The application is being terminated by the OS
  //                                           Called on iOS in applicationWillTerminate()
  //                                           Called on Android in onDestroy()*/
  //          APP_LOWMEMORY,          /**< The application is low on memory, free memory if possible.
  //                                           Called on iOS in applicationDidReceiveMemoryWarning()
  //                                           Called on Android in onLowMemory() */
  //          APP_WILLENTERBACKGROUND, /**< The application is about to enter the background
  //                                           Called on iOS in applicationWillResignActive()
  //                                           Called on Android in onPause()*/
  //          APP_DIDENTERBACKGROUND, /**< The application did enter the background and may not get CPU for some time
  //                                           Called on iOS in applicationDidEnterBackground()
  //                                           Called on Android in onPause()*/
  //          APP_WILLENTERFOREGROUND, /**< The application is about to enter the foreground
  //                                           Called on iOS in applicationWillEnterForeground()
  //                                           Called on Android in onResume() */
  //          APP_DIDENTERFOREGROUND, /**< The application is now interactive
  //                                           Called on iOS in applicationDidBecomeActive()
  //                                           Called on Android in onResume()*/
  //          /* Display events */
  //          DISPLAYEVENT   = 0x150,  /**< Display state change */
  //          /* Window events */
  //          WINDOWEVENT    = 0x200, /**< Window state change */
  //          SYSWMEVENT,             /**< System specific event */
  //          /* Keyboard events */
  //          KEYDOWN        = 0x300, /**< Key pressed */
  //          KEYUP,                  /**< Key released */
  //          TEXTEDITING,            /**< Keyboard text editing (composition) */
  //          TEXTINPUT,              /**< Keyboard text input */
  //          KEYMAPCHANGED,          /**< Keymap changed due to a system event such as an
  //                                           input language or keyboard layout change. */
  //          /* Mouse events */
  //          MOUSEMOTION    = 0x400, /**< Mouse moved */
  //          MOUSEBUTTONDOWN,        /**< Mouse button pressed */
  //          MOUSEBUTTONUP,          /**< Mouse button released */
  //          MOUSEWHEEL,             /**< Mouse wheel motion */
  //          /* Joystick events */
  //          JOYAXISMOTION  = 0x600, /**< Joystick axis motion */
  //          JOYBALLMOTION,          /**< Joystick trackball motion */
  //          JOYHATMOTION,           /**< Joystick hat position change */
  //          JOYBUTTONDOWN,          /**< Joystick button pressed */
  //          JOYBUTTONUP,            /**< Joystick button released */
  //          JOYDEVICEADDED,         /**< A new joystick has been inserted into the system */
  //          JOYDEVICEREMOVED,       /**< An opened joystick has been removed */
  //          /* Game controller events */
  //          CONTROLLERAXISMOTION  = 0x650, /**< Game controller axis motion */
  //          CONTROLLERBUTTONDOWN,          /**< Game controller button pressed */
  //          CONTROLLERBUTTONUP,            /**< Game controller button released */
  //          CONTROLLERDEVICEADDED,         /**< A new Game controller has been inserted into the system */
  //          CONTROLLERDEVICEREMOVED,       /**< An opened Game controller has been removed */
  //          CONTROLLERDEVICEREMAPPED,      /**< The controller mapping was updated */
  //          /* Touch events */
  //          FINGERDOWN      = 0x700,
  //          FINGERUP,
  //          FINGERMOTION,
  //          /* Gesture events */
  //          DOLLARGESTURE   = 0x800,
  //          DOLLARRECORD,
  //          MULTIGESTURE,
  //          /* Clipboard events */
  //          CLIPBOARDUPDATE = 0x900, /**< The clipboard changed */
  //          /* Drag and drop events */
  //          DROPFILE        = 0x1000, /**< The system requests a file open */
  //          DROPTEXT,                 /**< text/plain drag-and-drop event */
  //          DROPBEGIN,                /**< A new set of drops is beginning (NULL filename) */
  //          DROPCOMPLETE,             /**< Current set of drops is now complete (NULL filename) */
  //          /* Audio hotplug events */
  //          AUDIODEVICEADDED = 0x1100, /**< A new audio device is available */
  //          AUDIODEVICEREMOVED,        /**< An audio device has been removed. */
  //          /* Sensor events */
  //          SENSORUPDATE = 0x1200,     /**< A sensor was updated */
  //          /* Render events */
  //          RENDER_TARGETS_RESET = 0x2000, /**< The render targets have been reset and their contents need to be updated */
  //          RENDER_DEVICE_RESET, /**< The device has been reset and all textures need to be recreated */
  //          /** Events ::USEREVENT through ::LASTEVENT are for your use,
  //           *  and should be allocated with RegisterEvents()
  //           */
  //          USEREVENT    = 0x8000,
  //          /**
  //           *  This last event is only for bounding internal arrays
  //           */
  //          LASTEVENT    = 0xFFFF
		//};

        class Common 
        {
        public:
            Common(video::WindowID p_window_id) 
                : m_window_id(p_window_id) {}
            video::WindowID m_window_id { static_cast<video::WindowID>(~0) };
            virtual ~Common() = default;
        };
        namespace window
        {
            class Base : protected Common
            {
            public:
                enum class PossibleID
			    {
				    NONE,           /**< Never used */
				    SHOWN,          /**< Window has been shown */
				    HIDDEN,         /**< Window has been hidden */
				    EXPOSED,        /**< Window has been exposed and should be
						     redrawn */
				    MOVED,          /**< Window has been moved to data1, data2
					     */
				    RESIZED,        /**< Window has been resized to data1xdata2 */
				    SIZE_CHANGED,   /**< The window size has changed, either as
						     a result of an API call or through the
						     system or user changing the window size. */
				    MINIMIZED,      /**< Window has been minimized */
				    MAXIMIZED,      /**< Window has been maximized */
				    RESTORED,       /**< Window has been restored to normal size
						     and position */
				    ENTER,          /**< Window has gained mouse focus */
				    LEAVE,          /**< Window has lost mouse focus */
				    FOCUS_GAINED,   /**< Window has gained keyboard focus */
				    FOCUS_LOST,     /**< Window has lost keyboard focus */
				    CLOSE,          /**< The window manager requests that the window be closed */
				    TAKE_FOCUS,     /**< Window is being offered a focus (should SetWindowInputFocus() on itself or a subwindow, or ignore) */
				    HIT_TEST,        /**< Window had a hit test that wasn't HITTEST_NORMAL. */
				    COUNT
			    };

            public:
                enum class Type
                {
                    Unknown = -1,
                    Visibility,
                    Move,
                    Size,
                    Status,
                    Focus,
                    Count
                };
                const Type m_type_id = Type::Unknown;

                Base(video::WindowID p_window_id, Type p_type_id) 
                    : Common(p_window_id)
                    , m_type_id(p_type_id) {};
            };
            class Visiblity : private window::Base
            {
            public:
                enum class ID
                { 
                    Shown = static_cast<int>(PossibleID::SHOWN),
                    Hidden = static_cast<int>(PossibleID::HIDDEN),
                    Exposed = static_cast<int>(PossibleID::EXPOSED)
                };
                ID m_id;

                explicit Visiblity(video::WindowID p_window_id, ID p_id)
                    : Base(p_window_id, Type::Status)
                    , m_id(p_id) { };
            };
            class Move : private window::Base
            {
            public:
                typedef geometry::Point<int> Position;
                Position m_position;

                explicit Move(video::WindowID p_window_id,
                              const Position& p_position)
                    : Base(p_window_id, Type::Move)
                    , m_position(p_position) { };
            };
            class Resize : private window::Base
            {
            public:
                typedef geometry::Point<int> Size;
                Size m_size;

                explicit Resize(video::WindowID p_window_id, const Size& p_size)
                    : Base(p_window_id, Type::Size)
                    , m_size(p_size) { };
            };
            class Status : private window::Base
            {
            public:
                enum class ID
                {
                    Minimised = static_cast<int>(PossibleID::MINIMIZED),
                    Maximised = static_cast<int>(PossibleID::MAXIMIZED),
                    Restored = static_cast<int>(PossibleID::RESTORED),
                    Close = static_cast<int>(PossibleID::CLOSE)
                };
                ID m_id;

                explicit Status(video::WindowID p_window_id, ID p_id)
                    : Base(p_window_id, Type::Status)
                    , m_id(p_id) { };
            };
            class Focus : private window::Base
            {
            public:
                enum class ID
                {
                    MouseGained = static_cast<int>(PossibleID::ENTER),
                    MouseLost = static_cast<int>(PossibleID::LEAVE),
                    KeyboardGained = static_cast<int>(PossibleID::FOCUS_GAINED),
                    KeyboardLost = static_cast<int>(PossibleID::FOCUS_LOST)
                };
                ID m_id;

                explicit Focus(video::WindowID p_window_id, ID p_id)
                    : Base(p_window_id, Type::Focus)
                    , m_id(p_id) {};
            };
            class Close : private window::Base
            {
                
            };

            // TODO: move to implementation
            constexpr int id_lookup[static_cast<int>(Base::PossibleID::COUNT)]
            {
                 -1,                                            //NONE,       
                 static_cast<int>(Base::Type::Visibility),      //SHOWN,      
                 static_cast<int>(Base::Type::Visibility),      //HIDDEN,       
                 static_cast<int>(Base::Type::Visibility),      //EXPOSED, 
                 static_cast<int>(Base::Type::Move),            //MOVED, 
                 static_cast<int>(Base::Type::Size),            //RESIZED,  
				 -1,  /* No type needed */                      //SIZE_CHANGED, 
                 static_cast<int>(Base::Type::Status),          //MINIMIZED,    
                 static_cast<int>(Base::Type::Status),          //MAXIMIZED, 
                 static_cast<int>(Base::Type::Status),          //RESTORED,   
                 static_cast<int>(Base::Type::Focus),	        //ENTER,    
                 static_cast<int>(Base::Type::Focus), 	        //LEAVE, 
                 static_cast<int>(Base::Type::Focus),           //FOCUS_GAINED,
                 static_cast<int>(Base::Type::Focus),           //FOCUS_LOST,  
                 static_cast<int>(Base::Type::Status),          //CLOSE,   
				 -1,  /* Not implemented */                     //TAKE_FOCUS,
				 -1,  /* Not implemented */                     //HIT_TEST, 
            };
        }
        namespace input
        {
            class Base : public Common
            {
            protected:
                enum class State
                {
                    Unknown = -1,
                    Released,
                    Pressed
                };

            public:
                enum class Type
                {
                    Unknown = -1,
                    Keyboard,
                    MouseButton,
                    MouseWheel,
                    MouseMotion,
                    Count
                };
                const Type m_type_id = Type::Unknown;

             protected:
                Base(video::WindowID p_window_id, Type p_type_id)
                    : Common(p_window_id)
                    , m_type_id(p_type_id) {};
            };
            class Keyboard : private input::Base
            {
            public:
                enum class Key
                {
                    Unknown = 0,
                    A = 4,
                    B = 5,
                    C = 6,
                    D = 7,
                    E = 8,
                    F = 9,
                    G = 10,
                    H = 11,
                    I = 12,
                    J = 13,
                    K = 14,
                    L = 15,
                    M = 16,
                    N = 17,
                    O = 18,
                    P = 19,
                    Q = 20,
                    R = 21,
                    S = 22,
                    T = 23,
                    U = 24,
                    V = 25,
                    W = 26,
                    X = 27,
                    Y = 28,
                    Z = 29,
            
                    Num_1 = 30,
                    Num_2 = 31,
                    Num_3 = 32,
                    Num_4 = 33,
                    Num_5 = 34,
                    Num_6 = 35,
                    Num_7 = 36,
                    Num_8 = 37,
                    Num_9 = 38,
                    Num_0 = 39,
            
                    Return = 40,
                    Escape = 41,
                    Backspace = 42,
                    Tab = 43,
                    Space = 44,
            
                    Minus = 45,
                    Equals = 46,
                    LeftBracket = 47,
                    RightBracket = 48,
                    Backslash = 49, /**< Located at the lower left of the return
                                                  *   key on ISO keyboards and at the right end
                                                  *   of the QWERTY row on ANSI keyboards.
                                                  *   Produces REVERSE SOLIDUS (backslash) and
                                                  *   VERTICAL LINE in a US layout, REVERSE
                                                  *   SOLIDUS and VERTICAL LINE in a UK Mac
                                                  *   layout, NUMBER SIGN and TILDE in a UK
                                                  *   Windows layout, DOLLAR SIGN and POUND SIGN
                                                  *   in a Swiss German layout, NUMBER SIGN and
                                                  *   APOSTROPHE in a German layout, GRAVE
                                                  *   ACCENT and POUND SIGN in a French Mac
                                                  *   layout, and ASTERISK and MICRO SIGN in a
                                                  *   French Windows layout.
                                                  */
                    NonusHash = 50, /**< ISO USB keyboards actually use this code
                                                  *   instead of 49 for the same key, but all
                                                  *   OSes I've seen treat the two codes
                                                  *   identically. So, as an implementor, unless
                                                  *   your keyboard generates both of those
                                                  *   codes and your OS treats them differently,
                                                  *   you should generate BACKSLASH
                                                  *   instead of this code. As a user, you
                                                  *   should not rely on this code because SDL
                                                  *   will never generate it with most (all?)
                                                  *   keyboards.
                                                  */
                    Semicolon = 51,
                    Apostrophe = 52,
                    Grave = 53, /**< Located in the top left corner (on both ANSI
                                              *   and ISO keyboards). Produces GRAVE ACCENT and
                                              *   TILDE in a US Windows layout and in US and UK
                                              *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                                              *   and NOT SIGN in a UK Windows layout, SECTION
                                              *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                                              *   layouts on ISO keyboards, SECTION SIGN and
                                              *   DEGREE SIGN in a Swiss German layout (Mac:
                                              *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                                              *   DEGREE SIGN in a German layout (Mac: only on
                                              *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                                              *   French Windows layout, COMMERCIAL AT and
                                              *   NUMBER SIGN in a French Mac layout on ISO
                                              *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                                              *   SIGN in a Swiss German, German, or French Mac
                                              *   layout on ANSI keyboards.
                                              */
                    Comma = 54,
                    Period = 55,
                    Slash = 56,
            
                    Capslock = 57,
            
                    F1 = 58,
                    F2 = 59,
                    F3 = 60,
                    F4 = 61,
                    F5 = 62,
                    F6 = 63,
                    F7 = 64,
                    F8 = 65,
                    F9 = 66,
                    F10 = 67,
                    F11 = 68,
                    F12 = 69,
            
                    PrintScreen = 70,
                    ScrollLock = 71,
                    Pause = 72,
                    Insert = 73, /**< insert on PC, help on some Mac keyboards (but
                                                   does send code 73, not 117) */
                    Home = 74,
                    PageUp = 75,
                    Delete = 76,
                    End = 77,
                    PageDown= 78,
                    Right = 79,
                    Left = 80,
                    Down = 81,
                    Up = 82,
            
                    NumLockClear = 83, /**< num lock on PC, clear on Mac keyboards
                                                     */
                    KpDivide = 84,
                    KpMultiply = 85,
                    KpMinus = 86,
                    KpPlus = 87,
                    KpEnter = 88,
                    Kp1 = 89,
                    Kp2 = 90,
                    Kp3 = 91,
                    Kp4 = 92,
                    Kp5 = 93,
                    Kp6 = 94,
                    Kp7 = 95,
                    Kp8 = 96,
                    Kp9 = 97,
                    Kp0 = 98,
                    KpPeriod = 99,
            
                    NonusBackslash = 100, /**< This is the additional key that ISO
                                                        *   keyboards have over ANSI ones,
                                                        *   located between left shift and Y.
                                                        *   Produces GRAVE ACCENT and TILDE in a
                                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                                        *   (backslash) and VERTICAL LINE in a
                                                        *   US or UK Windows layout, and
                                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                                        *   in a Swiss German, German, or French
                                                        *   layout. */
                    Application = 101, /**< windows contextual menu, compose */
                    Power = 102, /**< The USB document says this is a status flag,
                                               *   not a physical key - but some Mac keyboards
                                               *   do have a power key. */
                    KpEquals = 103,
                    F13 = 104,
                    F14 = 105,
                    F15 = 106,
                    F16 = 107,
                    F17 = 108,
                    F18 = 109,
                    F19 = 110,
                    F20 = 111,
                    F21 = 112,
                    F22 = 113,
                    F23 = 114,
                    F24 = 115,
                    Execute = 116,
                    Help = 117,
                    Menu = 118,
                    Select = 119,
                    Stop = 120,
                    Again = 121,   /**< redo */
                    Undo = 122,
                    Cut = 123,
                    Copy = 124,
                    Paste = 125,
                    Find = 126,
                    Mute = 127,
                    VolumeUp = 128,
                    VolumeDown = 129,
                    KpDown = 133,
                    KpEqualsAs400 = 134,
            
                    International1 = 135, /**< used on Asian keyboards, see footnotes in USB doc */
                    International2 = 136,
                    International3 = 137, /**< Yen */
                    International4 = 138,
                    International5 = 139,
                    International6 = 140,
                    International7 = 141,
                    International8 = 142,
                    International9 = 143,
                    Lang1 = 144, /**< Hangul/English toggle */
                    Lang2 = 145, /**< Hanja conversion */
                    Lang3 = 146, /**< Katakana */
                    Lang4 = 147, /**< Hiragana */
                    Lang5 = 148, /**< Zenkaku/Hankaku */
                    Lang6 = 149, /**< reserved */
                    Lang7 = 150, /**< reserved */
                    Lang8 = 151, /**< reserved */
                    Lang9 = 152, /**< reserved */
            
                    AltErase = 153, /**< Erase-Eaze */
                    SysReq = 154,
                    Cancel = 155,
                    Clear = 156,
                    Prior = 157,
                    Return2 = 158,
                    Separator = 159,
                    Out = 160,
                    Oper = 161,
                    ClearAgain = 162,
                    CrSel = 163,
                    ExSel = 164,
            
                    Kp00 = 176,
                    Kp000 = 177,
                    ThousandSeparator = 178,
                    DecimalSeparator = 179,
                    CurrencyUnit = 180,
                    CurrencySubUnit = 181,
                    KpLeftParen = 182,
                    KpRightParen = 183,
                    KpLeftBrace = 184,
                    KpRightBrace = 185,
                    KpTab = 186,
                    KpBackspace = 187,
                    KpA = 188,
                    KpB = 189,
                    KpC = 190,
                    KpD = 191,
                    KpE = 192,
                    KpF = 193,
                    KpXor = 194,
                    KpPower = 195,
                    KpPercent = 196,
                    KpLess = 197,
                    KpGreater = 198,
                    KpAmpersand = 199,
                    KpDblAmpersand = 200,
                    KpVerticalBar = 201,
                    KpDblVericalbar = 202,
                    KpColon = 203,
                    KpHash = 204,
                    KpSpace = 205,
                    KpAt = 206,
                    KpExclam = 207,
                    KpMemStore = 208,
                    KpMemRecall = 209,
                    KpMemClear = 210,
                    KpMemAdd = 211,
                    KpMemSubtract = 212,
                    KpMemMultiply = 213,
                    KpMemDivide = 214,
                    KpPlusMinus = 215,
                    KpClear = 216,
                    KpClearEntry = 217,
                    KpBinary = 218,
                    KpOctal = 219,
                    KpDecimal = 220,
                    KpHexaDecimal = 221,
            
                    LCTRL = 224,
                    LSHIFT = 225,
                    LALT = 226, /**< alt, option */
                    LGUI = 227, /**< windows, command (apple), meta */
                    RCTRL = 228,
                    RSHIFT = 229,
                    RALT = 230, /**< alt gr, option */
                    RGUI = 231, /**< windows, command (apple), meta */
            
                    Mode = 257,    /**< I'm not sure if this is really not covered
                                                 *   by any of the above, but since there's a
                                                 *   special KMOD_MODE for it I'm adding it here
                                                 */
            
                    /* @} *//* Usage page 0x07 */
            
                    /**
                     *  \name Usage page 0x0C
                     *
                     *  These values are mapped from usage page 0x0C (USB consumer page).
                     */
                    /* @{ */
            
                    AudioNext = 258,
                    AudioPrev = 259,
                    AudioStop = 260,
                    AudioPlay = 261,
                    AudioMute = 262,
                    MediaSelect = 263,
                    WWW = 264,
                    Mail = 265,
                    Calculator = 266,
                    Computer = 267,
                    AcSearch = 268,
                    AcHome = 269,
                    AcBack = 270,
                    AcForward = 271,
                    AcStop = 272,
                    AcRefresh = 273,
                    AcBookmarks = 274,
            
                    /* @} *//* Usage page 0x0C */
            
                    /**
                     *  \name Walther keys
                     *
                     *  These are values that Christian Walther added (for mac keyboard?).
                     */
                    /* @{ */
            
                    BrightnessDown = 275,
                    BrightnessUp = 276,
                    DisplaySwitch = 277, /**< display mirroring/dual display
                                                           switch, video mode switch */
                    KBDILLUMTOGGLE = 278,
                    KBDILLUMDOWN = 279,
                    KBDILLUMUP = 280,
                    Eject = 281,
                    Sleep = 282,
            
                    App1 = 283,
                    App2 = 284,
            
                    /* @} *//* Walther keys */
            
                    /**
                     *  \name Usage page 0x0C (additional media keys)
                     *
                     *  These values are mapped from usage page 0x0C (USB consumer page).
                     */
                    /* @{ */
            
                    AudioRewind = 285,
                    AudioFastForward = 286,
            
                    /* @} *//* Usage page 0x0C (additional media keys) */
            
                    /* Add any other keys here. */
            
                    CodeCount = 512 /**< not a key, just marks the number of scancodes
                                                 for array bounds */
                };
                enum class Modifier
                {
                    None = 0x0000,
                    LSHIFT = 0x0001,
                    RSHIFT = 0x0002,
                    LCTRL = 0x0040,
                    RCTRL = 0x0080,
                    LALT = 0x0100,
                    RALT = 0x0200,
                    LGUI = 0x0400,
                    RGUI = 0x0800,
                    Num = 0x1000,
                    Caps = 0x2000,
                    Mode = 0x4000,
                    Reserved = 0x8000,
                    SHIFT = LSHIFT | RSHIFT,
                    CTRL = LCTRL | RCTRL,
                    ALT = LALT | RALT,
                    GUI = LGUI | RGUI,
                };
                using State = State;

                struct KeyAndState
                {
                    Key m_code = Key::Unknown;
                    Keyboard::State m_state = Keyboard::State::Unknown;

                    explicit KeyAndState(Key p_code, Keyboard::State p_state)
                        : m_code(p_code)
                        , m_state(p_state) { }
                };

                explicit Keyboard(video::WindowID p_window_id, KeyAndState p_key_and_state, Modifier p_modifier)
                    : Base(p_window_id, Type::Keyboard)
                    , m_key_and_state(p_key_and_state)
                    , m_modifier(p_modifier) { }

                KeyAndState m_key_and_state { Key::Unknown, State::Unknown };
                Modifier m_modifier = Modifier::None;
            };

            namespace mouse
            {
                class Button : private input::Base
                {
                public:
                    enum class Code
                    {
                        Unknown = -1,
                        Left = 1,
                        Middle,
                        Right,
                        Extra0,
                        Extra1,
                        Count
                    };
                    using State = State;
                    
                    explicit Button(video::WindowID p_window_id, Code p_code, State p_state)
                        : Base(p_window_id, Type::MouseButton)
                        , m_code(p_code)
                        , m_state(p_state) { }

                    Code m_code = Code::Unknown;
                    Button::State m_state = Button::State::Unknown;
                };
                class Motion : private input::Base
                {
                public:
                    typedef geometry::Point<int> Coordinate;
                    typedef geometry::Point<int> TravelDistance;
                    explicit Motion(video::WindowID p_window_id,
                                    const Coordinate& p_coordinate,
                                    const TravelDistance& p_travel_distance)
                        : Base(p_window_id, Type::MouseMotion)
                        , m_coordinate(p_coordinate)
                        , m_travel_distance(p_travel_distance) { }

                    Coordinate m_coordinate{ 0, 0 };
                    TravelDistance m_travel_distance{ 0, 0 };
                };
                class Wheel : private input::Base
                {
                public:
                    typedef geometry::Point<int> ScrollAmount;
                    explicit Wheel(video::WindowID p_window_id,
                                   const ScrollAmount& p_coordinate)
                        : Base(p_window_id, Type::MouseWheel)
                        , m_scroll_amount(p_coordinate) { }

                    ScrollAmount m_scroll_amount{ 0, 0 };
                };
            }
        }
        namespace callback
        {
            class Window
            {
            public:
                virtual void on_window_move(const prk::event::window::Move& event) = 0;
                virtual void on_window_resize(const prk::event::window::Resize& event) = 0;
                virtual void on_window_focus(const prk::event::window::Focus& event) = 0;
                virtual void on_window_status(const prk::event::window::Status& event) = 0;
                virtual void on_window_visibility(const prk::event::window::Visiblity& event) = 0;
            };

            class Input
            {
            public:
                virtual void on_keyboard(const prk::event::input::Keyboard& event) = 0;
                virtual void on_mouse_button(const prk::event::input::mouse::Button& event) = 0;
                virtual void on_mouse_wheel(const prk::event::input::mouse::Wheel& event) = 0;
                virtual void on_mouse_motion(const prk::event::input::mouse::Motion& event) = 0;
            };
        }
        namespace concepts
        {
            template<typename CallbackType>
            concept HasWindowCallback = std::is_base_of <callback::Window, CallbackType>::value;
            template<typename CallbackType>
            concept HasInputCallback = std::is_base_of <callback::Input, CallbackType>::value;
            template<typename CallbackType>
            concept IsCallback = HasWindowCallback<CallbackType> ||
                                 HasInputCallback<CallbackType>;
        }
	}
}

#endif // !INCLUDED_PERKUNAS_EVENTS