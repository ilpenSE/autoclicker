# Auto Clicker 2.0-beta1
## English
Written in C++ using Qt, modernized with Fluent UI.

### NEW MACRO SYSTEM
- Macros are now stored in a SQLite database and accessed from there.
- Each macro will either have its own shortcut key or use the program's global key.
- Macros consist of actions instead of predefined settings.
- Each action has a specific type: Mouse or Keyboard.
- You can now create not only mouse macros but also keyboard macros.
- All action settings are applicable to both types, but settings like "Mouse Button" are invalid for keyboard actions.
- Other action settings remain the same as the old application, with a few modifications.

#### Action Settings
- Click Type: CLICK, HOVER, HOLD (Hover is specific to mouse; not applicable to keyboards.)
- Mouse Button: This mouse-specific setting determines which mouse button to click (left, right, middle).
- Position: Fixed or Current. This setting applies to the mouse and determines where the click occurs. You can use the "save position" feature in the Extra Settings tab by moving your mouse to a position and pressing Ctrl + S.
- Repeat: Any natural number. Applies to both keyboard and mouse. Determines how many times the click or press occurs (0 means unlimited, previously called "repeat until stopped").
- Interval: Time in milliseconds between clicks or presses.
- Hold Duration (ms): Specific to the mouse HOLD type. Determines how long a mouse button will be held.
- Click Amount: Number of consecutive clicks or key presses. You can set this by entering a number (previously called Click Type).

### NEW: FULLY CUSTOMIZABLE THEMES AND INTERFACE
- You can now create your own `.qss` file in the `AppData/Roaming/AutoClicker2/themes` folder (copy and modify one of the existing files is recommended). Then register it in `themes.json` to use your custom theme.
- Using your QSS script, you can change all colors, interface width, height, and other settings. However, embedded icons and module locations cannot be changed.
- If you don’t want to write your own QSS, you can use the built-in Dark (Fluent Dark) and Light (Fluent Light) themes. You can even customize them, but you cannot delete them or change their names in `themes.json`.
- Start/Stop buttons have been removed; you can see the shortcut key next to the active macro label and check whether the clicker is running via the UI lock and window title.

### NEW: PATCH NOTES
- In the Settings tab, a large box will display all patch notes starting from version 2.0-beta1.
- These patch notes are fetched from GitHub and displayed in Markdown format.

### IMPROVED AUTOMATIC UPDATER
- The `AutoClickerUpdater.exe` in the application folder automatically fetches the latest releases from GitHub.
- If an update is available, it shows a progress bar and downloads & runs the new installer.
- If no update is available, it directly launches the main application.

## Türkçe
C++ ile yazıldı, Qt kullanılarak oluşturuldu, Fluent UI ile modernleştirildi.

### YENİ MAKRO SİSTEMİ
- Makrolar artık SQLite dosyası içinde tutuluyor ve erişimi ordan yapılıyor.
- Her makro ya kendine özel kısayol tuşuna ya da programın global tuşuna sahip olacak.
- Makrolar hazır ayarlar yerine aksiyonlara sahip olacak.
- Aksiyonların belirli bir tipi var: Fare ve Klavye
- Artık sadece fare değil, klavye makroları da oluşturabilirsiniz.
- Aksiyonlardaki tüm ayarlar her tip için geçerlidir ancak "Fare Düğmesi" gibi ayarlar klavye için geçerli olmadığından burası klavye tipi aksiyonlarda geçersiz sayılacaktır.
- Aksiyonların diğer ayarları eski uygulamada olduğu gibi ancak birkaç değiştirme var.

#### Aksiyon Ayarları
- Tıklama Tipi: TIKLAMA (CLICK), ÜZERİNE GELME (HOVER), BASILI TUT (HOLD) (Hover kısmı fareye özgüdür, klavyelerde geçerli değildir.)
- Fare Düğmesi: Fareye özgü olan bu ayar farede hangi tuşa tıklanacağını belirler. (sol, sağ, orta)
- Konum: Sabit ya da Mevcut, bu ayar fare için geçerlidir ve farenin nerde tıklama yapacağını belirler. Ek ayarlar sekmesinden konum kaydetme özelliğini kullanarak farenizi bir yere götürüp Ctrl + S'ye basarak o konumu geçirebilirsiniz.
- Tekrar: Tüm doğal sayılar, hem klavye hem fare için geçerli olan bu ayar, tıklamanın kaç kez yapılacağını belirler. (0 olduğunda sınırsız eski adıyla "durdurana kadar tekrar et" olur)
- Aralık: Tıklamalar ya da basmalar arası süreyi milisaniye cinsinden belirler.
- Basılı tutma süresi (ms): Farenin HOLD tipine özgüdür ve belirli bir fare düğmesine ne kadar süre boyunca basılacağını belirler.
- Tıklama Miktarı: Fare veya klavyenin o tuşa art arda kaç kez basacağını belirler. Sayı girerek bunu ayarlayabilirsiniz. (eski ismiyle tıklama tipi / click type)

### YENİ: TAMAMEN ÖZELLEŞTİRİLEBİLİR TEMALAR VE ARAYÜZ
- Artık AppData/Roaming/AutoClicker2'de themes klasörüne kendi `.qss` dosyanızı yazıp (orda var olan dosyalardan birini kopyalayıp değişiklik yapmanız önerilir) daha sonradan themes.json'da dosyanızı kaydederseniz kendi temanızı kullanabilirsiniz.
- Kendi QSS scriptinizle tüm renkleri, arayüz genişlik, yükseklik ve diğer ayarlamaları değiştirebilirsiniz. Ancak uygulama içine gömülü ikonları ve modüllerin yerlerini değiştiremezsiniz.
- Eğer kendi QSS'inizi yazmak istemiyorsanız uygulama içinde hazır gelen Koyu (Fluent Dark) ve Açık (Fluent Light) temaları kullanabilirsiniz hatta onları değiştirebilirsiniz. (Ancak silemezsiniz veya themes.json'da isimlerinde oynama yapamazsınız)
- Başlat/Durdur butonları kaldırıldı; kısayol tuşunu aktif makro yazısının yanında görebilir, tıklayıcının çalışıp çalışmadığını arayüz kilidi ve pencere başlığından anlayabilirsiniz.

### YENİ: YAMA NOTLARI
- Ayarlar sekmesinde büyük bir kutuda 2.0-beta1'den itibaren var olan tüm yama notlarını görebileceksiniz.
- Bu yama notları GitHub'dan çekilip markdown formatında gösterilecektir.

### DAHA İYİ OTOMATİK GÜNCELLEYİCİ
- Uygulamanın yüklü olduğu klasörde `AutoClickerUpdater.exe` dosyasında GitHub'daki en son sürümleri çekip kendisi otomatik güncelleme yapıyor.
- Bu uygulama eğer güncelleme varsa bir ilerleme bar'ı gösterip yeni güncellemenin yükleyicisini indirip açar.
- Eğer güncelleme yoksa direkt ana uygulamayı açar.

### DİĞER
- Eski sürümlerde olan güncelleme kontrolcüsü, JSON kontrolcüsü ve onarıcı bunda da aynen varlar.