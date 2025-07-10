using AutoClicker;
using System;
using System.IO;
using System.Text.Json;
using System.Windows.Forms;

public class SettingsModel
{
    public string Hotkey { get; set; } = SettingsManager.DefaultKey.ToString();
    public int LanguageIndex { get; set; } = (int)SettingsManager.DefaultLanguage;
    public bool FirstRun { get; set; } = true;
    public string ActiveMacro { get; set; } = SettingsManager.DefaultMacroName;
}

public static class SettingsManager
{
    public static readonly Keys DefaultKey = Keys.F6;
    public static readonly Language DefaultLanguage = Language.ENGLISH;
    public static readonly string DefaultMacroName = "DEFAULT";

    public static SettingsModel Load()
    {
        if (!File.Exists(Program.settingsPath))
            return new SettingsModel();

        string json = File.ReadAllText(Program.settingsPath);
        try
        {
            return JsonSerializer.Deserialize<SettingsModel>(json) ?? new SettingsModel();
        }
        catch (Exception ex)
        {
            throw ex;
        }
    }

    public static void Save(SettingsModel settings)
    {
        string json = JsonSerializer.Serialize(settings, new JsonSerializerOptions { WriteIndented = true });
        File.WriteAllText(Program.settingsPath, json);
    }
}
