using AutoClicker;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.Json;

public class MacroModel
{
    public string Name { get; set; } = "";
    public string Description { get; set; } = "";
    public MouseButton MouseButton { get; set; } = MouseButton.LEFT;
    public ClickType ClickType { get; set; } = ClickType.SINGLE;
    public Position Position { get; set; } = new Position();
    public int Interval { get; set; } = 0;
    public RepeatTimes RepeatTimes { get; set; } = new RepeatTimes();
}

public class DefaultMacro
{
    public MouseButton DefaultMouseButton { get; private set; } = MouseButton.LEFT;
    public ClickType DefaultClickType { get; private set; } = ClickType.SINGLE;
    public Position DefaultPosition { get; private set; } = new Position
    {
        X = 0,
        Y = 0,
        CurrentPosition = true
    };
    public RepeatTimes DefaultRepeatTimes { get; private set; } = new RepeatTimes
    {
        Count = 1,
        RepeatForever = true
    };
    public int DefaultInterval { get; private set; } = 0;
}

public static class MacroManager
{
    public static Dictionary<string, MacroModel> LoadAll()
    {
        if (!File.Exists(Program.macrosPath))
            return new Dictionary<string, MacroModel>();

        string json = File.ReadAllText(Program.macrosPath);
        return JsonSerializer.Deserialize<Dictionary<string, MacroModel>>(json)
               ?? new Dictionary<string, MacroModel>();
    }
    public static void DeleteMacro(string name)
    {
        var all = LoadAll();
        if (all.Remove(name))
        {
            SaveAll(all);
        }
    }

    public static void AddDefaults()
    {
        DefaultMacro defmacro = new DefaultMacro();
        MacroModel macroDefault = new MacroModel
        {
            Name = "DEFAULT",
            Description = "The defaults.",
            MouseButton = defmacro.DefaultMouseButton,
            ClickType = defmacro.DefaultClickType,
            Position = defmacro.DefaultPosition,
            Interval = defmacro.DefaultInterval,
            RepeatTimes = defmacro.DefaultRepeatTimes
        };

        SaveMacro("DEFAULT", macroDefault);
    }

    public static void SaveAll(Dictionary<string, MacroModel> macros)
    {
        string json = JsonSerializer.Serialize(macros, new JsonSerializerOptions { WriteIndented = true });
        File.WriteAllText(Program.macrosPath, json);
    }

    public static void SaveMacro(string name, MacroModel macro)
    {
        var all = LoadAll();
        all[name] = macro;
        SaveAll(all);
    }

    public static List<string> GetAllMacroNames()
    {
        if (!File.Exists(Program.macrosPath))
            return new List<string>();

        string json = File.ReadAllText(Program.macrosPath);
        var dict = JsonSerializer.Deserialize<Dictionary<string, MacroModel>>(json);
        return dict?.Keys.ToList() ?? new List<string>();
    }


    public static MacroModel GetMacroByName(string name)
    {
        var all = LoadAll();
        return all.TryGetValue(name, out var macro) ? macro : null;
    }
}
