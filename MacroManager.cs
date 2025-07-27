using AutoClicker;
using AutoClicker.Properties;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
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
    public ClickMod ClickMode { get; set; } = ClickMod.CLICK;
    public int HoldDuration { get; set; } = 1;
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
    public ClickMod DefaultClickMod { get; private set; } = ClickMod.CLICK;
    public int DefaultInterval { get; private set; } = 0;
    public int DefaultHoldDuration { get; private set; } = 1;
}

public static class MacroManager
{
    public static Dictionary<string, MacroModel> Macros { get; private set; }

    static MacroManager()
    {
        Macros = LoadAll();
        Macros = FixMacros(Macros);
        SaveAll(Macros);
    }

    private static Dictionary<string, MacroModel> LoadAll()
    {
        if (!File.Exists(Program.macrosPath))
            return new Dictionary<string, MacroModel>();

        string json = File.ReadAllText(Program.macrosPath);
        var rawMacros = JsonSerializer.Deserialize<Dictionary<string, MacroModel>>(json)
                     ?? new Dictionary<string, MacroModel>();

        var macros = new Dictionary<string, MacroModel>();
        int unnamedCounter = 1;

        foreach (var kvp in rawMacros)
        {
            var key = kvp.Key?.Trim() ?? "";
            var macro = kvp.Value;

            
            if (string.IsNullOrEmpty(key))
            {
                // Key boşsa otomatik isim ver
                key = $"{Resources.unnamed_macro} {unnamedCounter++}";
                if (macro == null) macro = new MacroModel();
                macro.Name = key;
                macros[key] = macro;
                continue;
            }

            // name yoksa oluştur
            if (string.IsNullOrWhiteSpace(kvp.Value.Name))
            {
                // Name yoksa ata
                kvp.Value.Name = kvp.Key;
            }

            // description yoksa ata
            if (string.IsNullOrWhiteSpace(macro.Description))
            {
                macro.Description = Resources.nodesc;
            }

            // Key ile Name uyuşmuyorsa Name'i güncelle
            if (macro == null) macro = new MacroModel();
            if (string.IsNullOrWhiteSpace(macro.Name) || macro.Name != key)
                macro.Name = key;

            macros[key] = macro;
        }

        return macros;
    }



    private static Dictionary<string, MacroModel> FixMacros(Dictionary<string, MacroModel> macros)
    {
        var def = new DefaultMacro();

        foreach (var macro in macros.Values)
        {
            if (string.IsNullOrWhiteSpace(macro.Name))
                macro.Name = Resources.unnamed_macro;

            if (macro.Description == null)
                macro.Description = Resources.nodesc;

            if (!Enum.IsDefined(typeof(MouseButton), macro.MouseButton))
                macro.MouseButton = def.DefaultMouseButton;

            if (!Enum.IsDefined(typeof(ClickType), macro.ClickType))
                macro.ClickType = def.DefaultClickType;

            if (macro.Position == null)
                macro.Position = def.DefaultPosition;
            else
            {
                if (macro.Position.X < 0) macro.Position.X = def.DefaultPosition.X;
                if (macro.Position.Y < 0) macro.Position.Y = def.DefaultPosition.Y;
            }

            if (macro.Interval < 0)
                macro.Interval = def.DefaultInterval;

            if (macro.RepeatTimes == null)
                macro.RepeatTimes = def.DefaultRepeatTimes;
            else
            {
                if (macro.RepeatTimes.Count < 0)
                    macro.RepeatTimes.Count = def.DefaultRepeatTimes.Count;
            }

            if (!Enum.IsDefined(typeof(ClickMod), macro.ClickMode))
                macro.ClickMode = def.DefaultClickMod;

            if (macro.HoldDuration <= 0)
                macro.HoldDuration = def.DefaultHoldDuration;
        }

        // DEFAULT makro yoksa ekle
        if (!macros.ContainsKey("DEFAULT"))
        {
            AddDefaults(macros);
        }

        return macros;
    }

    public static void AddDefaults(Dictionary<string, MacroModel> macros)
    {
        DefaultMacro def = new DefaultMacro();
        MacroModel macroDefault = new MacroModel
        {
            Name = "DEFAULT",
            Description = "The defaults.",
            MouseButton = def.DefaultMouseButton,
            ClickType = def.DefaultClickType,
            Position = def.DefaultPosition,
            Interval = def.DefaultInterval,
            RepeatTimes = def.DefaultRepeatTimes,
            ClickMode = def.DefaultClickMod,
            HoldDuration = def.DefaultHoldDuration
        };

        macros["DEFAULT"] = macroDefault;
        SaveAll(macros);
    }

    public static void CheckDefaults()
    {
        if (!Macros.ContainsKey("DEFAULT"))
        {
            AddDefaults(Macros);
        }
    }

    public static void DeleteMacro(string name)
    {
        if (Macros.Remove(name))
        {
            SaveAll(Macros);
        }
    }


    public static void SaveAll(Dictionary<string, MacroModel> macros)
    {
        try
        {
            string json = JsonSerializer.Serialize(macros, new JsonSerializerOptions { WriteIndented = true });
            File.WriteAllText(Program.macrosPath, json);
        }
        catch (Exception ex)
        {
            Console.WriteLine("Macro kaydedilirken hata: " + ex.Message);
        }
    }

    public static void SaveMacro(string name, MacroModel macro)
    {
        Macros[name] = macro;
        SaveAll(Macros);
    }

    public static List<string> GetAllMacroNames()
    {
        return Macros.Keys.ToList();
    }

    public static MacroModel GetMacroByName(string name)
    {
        return Macros.TryGetValue(name, out var macro) ? macro : null;
    }
}
