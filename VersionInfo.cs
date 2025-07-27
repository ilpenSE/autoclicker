using System;

namespace AutoClicker
{
    public class VersionInfo
    {
        public int Major { get; set; }
        public int Minor { get; set; }
        public int Patch { get; set; }
        public int PreRelease { get; set; }

        public static VersionInfo Parse(string version)
        {
            if (version.StartsWith("v", StringComparison.OrdinalIgnoreCase))
                version = version.Substring(1);

            VersionInfo result = new VersionInfo();

            string[] preParts = version.Split(new[] { "-pre" }, StringSplitOptions.None);
            string[] numbers = preParts[0].Split('.');

            if (numbers.Length > 0) result.Major = int.Parse(numbers[0]);
            if (numbers.Length > 1) result.Minor = int.Parse(numbers[1]);
            if (numbers.Length > 2) result.Patch = int.Parse(numbers[2]);

            if (preParts.Length > 1)
                result.PreRelease = int.Parse(preParts[1]);

            return result;
        }

        public static VersionInfo FromSettings(string versionCode, int preRelease)
        {
            VersionInfo info = Parse(versionCode);
            info.PreRelease = preRelease;
            return info;
        }

        public override bool Equals(object obj)
        {
            var other = obj as VersionInfo;
            if (other == null) return false;

            return Major == other.Major &&
                   Minor == other.Minor &&
                   Patch == other.Patch &&
                   PreRelease == other.PreRelease;
        }

        public override int GetHashCode()
        {
            return Major ^ Minor ^ Patch ^ PreRelease;
        }

        public static VersionInfo ParseTag(string tag)
        {
            // Örnek tag: "autoclicker3", "autoclicker3_1", "autoclicker4-pre1"
            string versionPart = tag.Replace("autoclicker", ""); // "3" veya "3_1" veya "4-pre1"

            int major = 1, minor = 0, patch = 0, preRelease = 0;

            if (versionPart.Contains("_"))
            {
                var parts = versionPart.Split('_'); // "3_1" -> ["3", "1"]
                major = 1;
                int.TryParse(parts[0], out minor);
                int.TryParse(parts[1], out patch);
            }
            else if (versionPart.Contains("-pre"))
            {
                var preParts = versionPart.Split(new[] { "-pre" }, StringSplitOptions.None);
                int.TryParse(preParts[0], out minor);
                int.TryParse(preParts[1], out preRelease);
                major = 1;
                patch = 0;
            }
            else
            {
                int.TryParse(versionPart, out minor);
                major = 1;
                patch = 0;
            }

            return new VersionInfo
            {
                Major = major,
                Minor = minor,
                Patch = patch,
                PreRelease = preRelease
            };
        }



        public bool IsNewerThan(VersionInfo other)
        {
            Console.WriteLine($"Compare Versions: GitHub={Major}.{Minor}.{Patch}-pre{PreRelease} This Program={other.Major}.{other.Minor}.{other.Patch}-pre{other.PreRelease}");

            if (Major != other.Major)
                return Major > other.Major;
            if (Minor != other.Minor)
                return Minor > other.Minor;
            if (Patch != other.Patch)
                return Patch > other.Patch;

            if (PreRelease == 0 && other.PreRelease > 0)
                return true;
            if (PreRelease > 0 && other.PreRelease == 0)
                return false;

            return false;
        }


        public string ToFullString()
        {
            string version = $"{Major}.{Minor}.{Patch}";
            if (PreRelease > 0)
                version += $"-pre{PreRelease}";
            return version;
        }

        public override string ToString()
        {
            return ToFullString();
        }
    }

    public static class VersionInfoProvider
    {
        public static readonly VersionInfo Current = VersionInfo.Parse(AutoClicker.MainMenu.VERSION_STRING);
    }
}
