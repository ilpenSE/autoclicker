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

        public static async Task<string> CheckPreReleaseAvailable()
        {
            // limit releases by 5 property
            const string allReleasesUrl = "https://api.github.com/repos/ilpenSE/autoclicker/releases?per_page=5";
            try
            {
                using (HttpClient client = new HttpClient())
                {
                    client.DefaultRequestHeaders.UserAgent.ParseAdd("AutoClicker");

                    HttpResponseMessage response = await client.GetAsync(allReleasesUrl);
                    response.EnsureSuccessStatusCode();

                    string json = await response.Content.ReadAsStringAsync();
                    JsonDocument doc = JsonDocument.Parse(json);

                    VersionInfo current = VersionInfo.Parse(AutoClicker.MainMenu.VERSION_STRING);

                    foreach (JsonElement release in doc.RootElement.EnumerateArray())
                    {
                        bool isPreRelease = release.GetProperty("prerelease").GetBoolean();
                        if (!isPreRelease) continue;

                        string tag = release.GetProperty("tag_name").GetString();
                        if (string.IsNullOrEmpty(tag)) continue;

                        VersionInfo preReleaseVersion = VersionInfo.ParseTag(tag);

                        if (preReleaseVersion.IsNewerThan(current))
                            return release.GetProperty("html_url").GetString();
                    }

                    return "";
                }
            }
            catch (HttpRequestException)
            {
                Console.WriteLine("Internet connection not found, pre-release update checking bypassed.");
                return "";
            }
            catch (Exception ex)
            {
                Console.WriteLine("Pre-release update checker failed: " + ex.Message);
                return "";
            }
        }

    }

}
