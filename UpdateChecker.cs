using System;
using System.Net.Http;
using System.Text.Json;
using System.Threading.Tasks;

namespace AutoClicker
{
    public static class UpdateChecker
    {
        private const string RepoApiUrl = "https://api.github.com/repos/ilpenSE/autoclicker/releases/latest";

        public static async Task<bool> CheckUpdateAvailable()
        {
            try
            {
                using (HttpClient client = new HttpClient())
                {
                    client.DefaultRequestHeaders.UserAgent.ParseAdd("AutoClicker");

                    HttpResponseMessage response = await client.GetAsync(RepoApiUrl);
                    response.EnsureSuccessStatusCode();

                    string json = await response.Content.ReadAsStringAsync();
                    using (JsonDocument doc = JsonDocument.Parse(json))
                    {
                        JsonElement root = doc.RootElement;

                        string tag = root.GetProperty("tag_name").GetString();
                        if (string.IsNullOrEmpty(tag))
                            return false;

                        VersionInfo current = VersionInfo.Parse(AutoClicker.MainMenu.VERSION_STRING);
                        VersionInfo latest = VersionInfo.ParseTag(tag);

                        return latest.IsNewerThan(current);
                    }
                }
            }
            catch (HttpRequestException)
            {
                Console.WriteLine("Internet connection not found, update checking bypassed.");
                return false;
            }
            catch (Exception ex)
            {
                Console.WriteLine("Update checker failed: " + ex.Message);
                return false;
            }
        }
    }

}
