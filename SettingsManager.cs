using AutoClicker;
using System;
using System.Globalization;
using System.IO;
using System.Text.Json;
using System.Windows.Forms;

public class SettingsModel
{
    public string Hotkey { get; set; } = "F6";
    public int LanguageIndex { get; set; } = 0;
    public bool FirstRun { get; set; } = true;
    public string ActiveMacro { get; set; } = "DEFAULT";
    public string VersionCode { get; set; } = AutoClicker.MainMenu.VERSION_STRING;
    public int PreRelease { get; set; } = 0;
}

public static class SettingsManager
{
    public static readonly Keys DefaultKey = Keys.F6;
    public static readonly Language DefaultLanguage = Language.ENGLISH;
    public static readonly string DefaultMacroName = "DEFAULT";

    private static readonly JsonSerializerOptions JsonOptions = new JsonSerializerOptions
    {
        WriteIndented = true,
        PropertyNameCaseInsensitive = true,
    };

    public static SettingsModel Settings { get; private set; }

    public static SettingsModel Load()
    {
        if (!File.Exists(Program.settingsPath))
        {
            Console.WriteLine("Settings dosyası bulunamadı. Yeni ayarlar oluşturuluyor.");
            Settings = CreateDefaultSettings();
            Save(Settings);
            return Settings;
        }

        string json = "";
        try
        {
            json = File.ReadAllText(Program.settingsPath);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Settings dosyası okunamadı: {ex.Message}");
            Settings = CreateDefaultSettings();
            Save(Settings);
            return Settings;
        }

        try
        {
            Settings = JsonSerializer.Deserialize<SettingsModel>(json, JsonOptions) ?? CreateDefaultSettings();
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Settings parse hatası: {ex.Message}");
            Settings = CreateDefaultSettings();
        }

        Settings = ApplyVersionMigration(Settings);
        Settings = FillMissingDefaults(Settings);
        Save(Settings);

        return Settings;
    }

    public static void Save(SettingsModel settings)
    {
        try
        {
            var json = JsonSerializer.Serialize(settings, JsonOptions);
            File.WriteAllText(Program.settingsPath, json);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Settings kaydedilirken hata: {ex.Message}");
        }
    }

    private static SettingsModel CreateDefaultSettings()
    {
        return new SettingsModel
        {
            Hotkey = DefaultKey.ToString(),
            LanguageIndex = (int)DefaultLanguage,
            FirstRun = true,
            ActiveMacro = DefaultMacroName,
            VersionCode = AutoClicker.MainMenu.VERSION_STRING,
            PreRelease = 0
        };
    }

    private static SettingsModel ApplyVersionMigration(SettingsModel model)
    {
        var currentVersion = VersionInfoProvider.Current;

        VersionInfo loadedVersion;
        try
        {
            loadedVersion = VersionInfo.FromSettings(model.VersionCode, model.PreRelease);
        }
        catch
        {
            loadedVersion = new VersionInfo { Major = 1, Minor = 0, Patch = 0, PreRelease = 0 };
        }

        bool needsUpdate = !currentVersion.Equals(loadedVersion);

        if (needsUpdate)
        {
            Console.WriteLine("Settings versiyonu güncelleniyor...");
            model.VersionCode = currentVersion.ToFullString();
            model.PreRelease = currentVersion.PreRelease;
        }

        return model;
    }

    private static SettingsModel FillMissingDefaults(SettingsModel model)
    {
        if (string.IsNullOrWhiteSpace(model.Hotkey))
            model.Hotkey = "F6";

        if (model.LanguageIndex < 0)
            model.LanguageIndex = 0;

        if (string.IsNullOrWhiteSpace(model.ActiveMacro))
            model.ActiveMacro = "DEFAULT";

        return model;
    }

    public static void LoadSettings()
    {
        Settings = Load();
    }
}
