using System;
using System.Collections.Generic;
using System.Drawing.Text;
using System.Drawing;
using System.IO;
using System.Reflection;

namespace AutoClicker
{
    public static class FontLoader
    {
        private static readonly PrivateFontCollection _fontCollection = new PrivateFontCollection();
        private static readonly Dictionary<string, FontFamily> _fontFamilies = new Dictionary<string, FontFamily>();

        static FontLoader()
        {
            LoadFontsFromResources();
        }

        private static void LoadFontsFromResources()
        {
            var assembly = Assembly.GetExecutingAssembly();
            string[] resourceNames = assembly.GetManifestResourceNames();

            foreach (var resourceName in resourceNames)
            {
                if (resourceName.EndsWith(".otf", StringComparison.OrdinalIgnoreCase))
                {
                    using (Stream stream = assembly.GetManifestResourceStream(resourceName))
                    {
                        if (stream == null)
                            continue;

                        byte[] fontData = new byte[stream.Length];
                        stream.Read(fontData, 0, fontData.Length);

                        IntPtr fontPtr = System.Runtime.InteropServices.Marshal.AllocCoTaskMem(fontData.Length);
                        System.Runtime.InteropServices.Marshal.Copy(fontData, 0, fontPtr, fontData.Length);
                        _fontCollection.AddMemoryFont(fontPtr, fontData.Length);

                        try
                        {
                            string fontName = Path.GetFileNameWithoutExtension(resourceName);
                            var family = _fontCollection.Families[_fontCollection.Families.Length - 1]; // last added font
                            _fontFamilies[fontName] = family;
                        }
                        catch (Exception ex)
                        {
                            Console.WriteLine($"Error while adding font: {ex.Message}");
                        }
                    }
                }
            }
        }

        public static FontFamily GetFontFamily(string name)
        {
            if (_fontFamilies.TryGetValue(name, out var family))
                return family;

            throw new ArgumentException($"Font named '{name}' not found.");
        }

        public static Font GetRegular(float size)
        {
            // returns "AutoClicker.Gilroy-Regular"
            return CreateFont("AutoClicker.Gilroy-Regular", size);
        }
        public static Font GetBold(float size)
        {
            // returns "AutoClicker.Gilroy-Bold"
            return CreateFont("AutoClicker.Gilroy-Regular", size, FontStyle.Bold);
        }
        public static Font GetThin(float size)
        {
            // returns "AutoClicker.Gilroy-Thin"
            return CreateFont("AutoClicker.Gilroy-Thin", size);
        }
        public static Font CreateFont(string name, float size, FontStyle style = FontStyle.Regular)
        {
            var family = GetFontFamily(name);
            return new Font(family, size, style, GraphicsUnit.Point);
        }
    }
}
