public class Position
{
    public int X { get; set; } = 0;
    public int Y { get; set; } = 0;
    public bool CurrentPosition { get; set; } = true;

    public override bool Equals(object obj)
    {
        var other = obj as Position;
        if (other == null)
            return false;

        return X == other.X &&
               Y == other.Y &&
               CurrentPosition == other.CurrentPosition;
    }

    public override int GetHashCode()
    {
        unchecked
        {
            int hash = 17;
            hash = hash * 23 + X.GetHashCode();
            hash = hash * 23 + Y.GetHashCode();
            hash = hash * 23 + CurrentPosition.GetHashCode();
            return hash;
        }
    }
}


public class RepeatTimes
{
    public int Count { get; set; } = 1;
    public bool RepeatForever { get; set; } = true;

    public override bool Equals(object obj)
    {
        var other = obj as RepeatTimes;
        if (other == null)
            return false;

        return Count == other.Count &&
               RepeatForever == other.RepeatForever;
    }

    public override int GetHashCode()
    {
        unchecked
        {
            int hash = 17;
            hash = hash * 23 + Count.GetHashCode();
            hash = hash * 23 + RepeatForever.GetHashCode();
            return hash;
        }
    }
}

public class HoldOpts
{
    public int Duration { get; set; } = 1000;
    public bool HoldForever { get; set; } = false;

    public override bool Equals(object obj)
    {
        var other = obj as HoldOpts;
        if (other == null)
            return false;

        return Duration == other.Duration &&
               HoldForever == other.HoldForever;
    }

    public override int GetHashCode()
    {
        unchecked
        {
            int hash = 17;
            hash = hash * 23 + Duration.GetHashCode();
            hash = hash * 23 + HoldForever.GetHashCode();
            return hash;
        }
    }
}

public enum MouseButton
{
    LEFT = 0,
    RIGHT = 1,
    MIDDLE = 2
}

public enum ClickType
{
    SINGLE = 0,
    DOUBLE = 1,
    TRIPLE = 2,
    QUADRUPLE = 3
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
    FRENCH = 3,
    ITALIAN = 4
}