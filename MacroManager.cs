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
    public int Interval { get; set; } = 100;
    public RepeatTimes RepeatTimes { get; set; } = new RepeatTimes();
    public ClickMod ClickMode { get; set; } = ClickMod.CLICK;
    public int HoldDuration { get; set; } = 1000;

    public override bool Equals(object obj)
    {
        var other = obj as MacroModel;
        if (other == null)
            return false;

        return Name == other.Name &&
               Description == other.Description &&
               MouseButton == other.MouseButton &&
               ClickType == other.ClickType &&
               Interval == other.Interval &&
               HoldDuration == other.HoldDuration &&
               ClickMode == other.ClickMode &&
               Position.Equals(other.Position) &&
               RepeatTimes.Equals(other.RepeatTimes);
    }


    public override int GetHashCode()
    {
        unchecked // taşma olursa hata verme
        {
            int hash = 17;
            hash = hash * 23 + (Name?.GetHashCode() ?? 0);
            hash = hash * 23 + (Description?.GetHashCode() ?? 0);
            hash = hash * 23 + MouseButton.GetHashCode();
            hash = hash * 23 + ClickType.GetHashCode();
            hash = hash * 23 + Interval.GetHashCode();
            hash = hash * 23 + HoldDuration.GetHashCode();
            hash = hash * 23 + ClickMode.GetHashCode();
            hash = hash * 23 + (Position?.GetHashCode() ?? 0);
            hash = hash * 23 + (RepeatTimes?.GetHashCode() ?? 0);
            return hash;
        }
    }


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
    public int DefaultInterval { get; private set; } = 100;
    public int DefaultHoldDuration { get; private set; } = 1000;
}

public static class MacroManager
{
    public static Dictionary<string, MacroModel> Macros { get; private set; }

    static MacroManager()
    {
        Macros = LoadAll();
        Macros = FixMacros(Macros);

        MacroModel defaultsInJSON = Macros["DEFAULT"];
        MacroModel macroDefault = GetDefaultMacro();
        if (!defaultsInJSON.Equals(macroDefault))
        {
            Macros["DEFAULT"] = macroDefault;
        }

        SaveAll(Macros);
    }

    private static Dictionary<string, MacroModel> LoadAll()
    {
        try
        {
            // Dosya yoksa oluştur
            if (!File.Exists(Program.macrosPath))
            {
                File.WriteAllText(Program.macrosPath, "{}");
                return new Dictionary<string, MacroModel>();
            }

            // İçerik boşsa düzelt
            string rawText = File.ReadAllText(Program.macrosPath);
            if (string.IsNullOrWhiteSpace(rawText))
            {
                File.WriteAllText(Program.macrosPath, "{}");
                return new Dictionary<string, MacroModel>();
            }

            // JSON deserialize etmeyi dene
            var rawMacros = JsonSerializer.Deserialize<Dictionary<string, MacroModel>>(rawText)
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
        catch (JsonException)
        {
            // JSON bozuksa dosyayı sıfırla
            File.WriteAllText(Program.macrosPath, "{}");
            return new Dictionary<string, MacroModel>();
        }
        catch (Exception ex)
        {
            Console.WriteLine("MacroManager Load Error: " + ex.Message);
            return new Dictionary<string, MacroModel>();
        }
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
        MacroModel macroDefault = GetDefaultMacro();

        macros["DEFAULT"] = macroDefault;
        SaveAll(macros);
    }

    public static MacroModel GetDefaultMacro()
    {
        DefaultMacro def = new DefaultMacro();
        return new MacroModel
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
