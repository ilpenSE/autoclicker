# Auto Clicker 2.0 - Fully Revamped Macro Clicker

C++ ile yazıldı, Qt kullanılarak oluşturuldu ve Fluent UI ile modernleştirildi.

---

## Overview / Genel Bakış

Auto Clicker 2.0 artık tamamen modern, hızlı ve özelleştirilebilir bir makro tıklayıcıdır. Fare ve klavye makrolarınızı kolayca oluşturabilir, kaydedebilir ve yönetebilirsiniz. Her makro için ayrı veya global kısayol tuşları belirleyebilirsiniz.  

All user data, macros, themes, and settings are stored in your **AppData/Roaming/AutoClicker2** folder.

## Screenshots / Ekran Görüntüleri
### English UI
<img width="901" height="627" alt="ss1_en" src="https://github.com/user-attachments/assets/97b02017-7251-463f-96db-bbefc8ad65f6" />

<img width="900" height="630" alt="ss2_en" src="https://github.com/user-attachments/assets/d1e585b6-6d68-4c1c-84b8-7ef1d32fb057" />

<img width="902" height="628" alt="ss3_en" src="https://github.com/user-attachments/assets/48f1d038-37f7-4b7e-8652-84727f5be491" />

<img width="899" height="627" alt="ss4_en" src="https://github.com/user-attachments/assets/fca9a37b-692d-4865-b16c-0f4b8d52f09a" />

### Türkçe Arayüz
<img width="901" height="633" alt="ss1_tr" src="https://github.com/user-attachments/assets/38cc4a9c-a753-4805-b031-89da2ec38943" />

<img width="901" height="626" alt="ss2_tr" src="https://github.com/user-attachments/assets/091ba2d1-b598-4c20-a446-2bba896949cf" />

<img width="899" height="628" alt="ss3_tr" src="https://github.com/user-attachments/assets/87cecd75-ed26-41fa-91a8-0d9e0241da2b" />

<img width="899" height="627" alt="ss4_tr" src="https://github.com/user-attachments/assets/81f0e32c-127a-4e57-b9d6-c2b300189c91" />

---

## Features / Özellikler

- **Customizable Themes / Özelleştirilebilir Temalar**  
  - Dark, Light ve kendi `.qss` dosyanızla oluşturabileceğiniz özel temalar.
- **Languages / Diller**  
  - English, Turkish, German, Italian, French, Russian. Her kullanıcı kendi dilini seçebilir.
- **Macros / Makrolar**  
  - Fully customizable mouse and keyboard actions: click, hold, hover.  
  - Unlimited actions per macro.  
  - Mouse actions include: CLICK (interval clicker), HOLD (press and hold), HOVER (move over a position).  
  - Keyboard actions include: CLICK (interval) and HOLD.  
  - All actions have configurable repeat count, interval, hold duration, click count, and mouse button (if applicable).

---

## File Structure / Dosya Yapısı (AppData)

```
AppData/Roaming/AutoClicker2
├─ logs/
│  └─ 12.08.2025-13:00:00.log
├─ themes/
│  ├─ dark.css
│  ├─ light.css
│  └─ mytheme.css
├─ settings.json
└─ macros.sqlite
```

- `settings.json` stores app preferences like language, active macro, theme, and global hotkey.  
- `macros.sqlite` stores all macro definitions and actions.

---

### Settings.json Example

```json
{
  "GlobalHotkey": "Shift + F6",
  "Language": "en",
  "FirstRun": false,
  "Version": "2.0-beta1",
  "Theme": "dark",
  "ActiveMacro": 0
}
```

---

### Macros.sqlite Overview

**Tables:** `Macros`, `MacroActions`  

**Macros Table:**
- `id` | `name` | `description` | `hotkey`  
- If `hotkey` = DEF, the global hotkey from settings is used.  
- Hotkey modifiers: Ctrl, Shift, Alt (LWin, RWin, Fn, Enter are not allowed).  

**MacroActions Table:**
- `id` | `macro_id` | `order` | `action_type` | `click_type` | `repeat` | `position` | `current_pos` | `interval` | `hold_duration` | `hover_duration` | `click_count` | `mouse_button`  
- `repeat = 0` means unlimited.  
- `position`, `action_type`, `click_type` are strings; others are integers.  
- Some fields are ignored for certain action types (e.g., hover duration for keyboard).  
- The default macro `DEFAULT` is automatically created if none exist and cannot be deleted.

---

## Simple Explanation for Users / Kullanıcılar için Basit Açıklama

- You can make the app **look the way you want** using themes.  
- You can **create macros** for mouse and keyboard, decide exactly what they do, how many times, how fast, etc.  
- All data is saved in your **AppData folder**, so nothing gets lost.  
- You don’t need to worry about the files; the app handles SQLite and JSON automatically.  

- Uygulamanın görünümünü **temalarla değiştirebilirsiniz**.  
- Fare ve klavye makroları **tam olarak istediğiniz gibi** oluşturabilirsiniz: tıklama sayısı, aralık, basılı tutma süresi vs.  
- Tüm veriler **AppData klasöründe** saklanır, kaybolmaz.  
- Dosyalarla uğraşmanız gerekmez, SQLite ve JSON otomatik olarak yönetilir.

---

## Automatic Updater / Otomatik Güncelleyici

- The app includes `AutoClickerUpdater.exe`.  
- It checks GitHub for new versions, downloads, and installs automatically.  
- If no update is available, it opens the main app directly.

- Uygulama `AutoClickerUpdater.exe` ile birlikte gelir.  
- GitHub’dan yeni sürümleri kontrol eder, indirir ve yükler.  
- Güncelleme yoksa direkt ana uygulama açılır.

