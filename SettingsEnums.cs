using System.Drawing;
using System.Text.Json.Serialization;

public class Position
{
    public int X { get; set; } = 0;
    public int Y { get; set; } = 0;
    public bool CurrentPosition { get; set; } = true;
}

public class RepeatTimes
{
    public int Count { get; set; } = 1;
    public bool RepeatForever { get; set; } = true;
}

public enum MouseButton
{
    LEFT = 0,
    RIGHT = 1,
    MIDDLE = 2
}

public enum ClickType
{
    SINGLE = 1,
    DOUBLE = 2,
    TRIPLE = 3,
    QUADRUPLE = 4
}

public enum ClickMod
{
    CLICK = 0,
    HOLD = 1
}

public enum Language
{
    ENGLISH = 0,
    TURKISH = 1,
    GERMAN = 2,
    FRENCH = 3
}