# Auto Clicker 2.0 - Fully Revamped Macro Clicker 🖱️

![Platform](https://img.shields.io/badge/platform-Windows-0078d4?style=for-the-badge&logo=windows&logoColor=white)
![Language](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Framework](https://img.shields.io/badge/Qt-41CD52?style=for-the-badge&logo=qt&logoColor=white)
![License](https://img.shields.io/badge/license-MIT-brightgreen?style=for-the-badge)

![Version](https://img.shields.io/badge/version-2.0_beta-orange)
![UI](https://img.shields.io/badge/UI-Fluent_Design-0078d4)
![Database](https://img.shields.io/badge/database-SQLite-07405E)

**Languages:** 
![English](https://img.shields.io/badge/English-✓-blue)
![Turkish](https://img.shields.io/badge/Türkçe-✓-red)
![German](https://img.shields.io/badge/Deutsch-✓-black)
![Italian](https://img.shields.io/badge/Italiano-✓-green)
![French](https://img.shields.io/badge/Français-✓-lightblue)

Written in C++, created with Qt, modernized with Fluent UI.

## Overview 📋

Auto Clicker 2.0 is a completely modern, fast and customizable macro clicker. You can easily create, save and manage your mouse and keyboard macros. You can set separate or global hotkeys for each macro.

All user data, macros, themes, and settings are stored in your **AppData/Roaming/AutoClicker2** folder.

## Screenshots 📸

### Main Window
<img width="901" height="627" alt="ss1_en" src="https://github.com/user-attachments/assets/97b02017-7251-463f-96db-bbefc8ad65f6" />

### Macro Selection Window
<img width="900" height="630" alt="ss2_en" src="https://github.com/user-attachments/assets/d1e585b6-6d68-4c1c-84b8-7ef1d32fb057" />

### Settings Window
<img width="902" height="628" alt="ss3_en" src="https://github.com/user-attachments/assets/48f1d038-37f7-4b7e-8652-84727f5be491" />

### About Window
<img width="899" height="627" alt="ss4_en" src="https://github.com/user-attachments/assets/fca9a37b-692d-4865-b16c-0f4b8d52f09a" />

## Features ✨

- **🎨 Customizable Themes**
  - Dark, Light and your custom themes by creating your `.qss` file.

- **🌍 Languages**
  - English, Turkish, German, Italian, French.
  - Every user can select their language.

- **⚙️ Macros**
  - Fully customizable mouse and keyboard actions: click, hold, hover.
  - Unlimited actions per macro.
  - Mouse actions include: CLICK (interval clicker), HOLD (press and hold), HOVER (move over a position).
  - Keyboard actions include: CLICK (interval) and HOLD.
  - All actions have configurable repeat count, interval, hold duration, click count, and mouse button (if applicable).

## File Structure 📁 (AppData)

```
AppData/Roaming/AutoClicker2
├─ logs/
│  └─ 12.08.2025-13:00:00.log
├─ themes/
│  ├─ dark.qss
│  ├─ light.qss
│  └─ mytheme.qss
├─ settings.json
├─ themes.json
└─ macros.sqlite
```

- `settings.json` stores app preferences like language, active macro, theme, and global hotkey.
- `macros.sqlite` stores all macro definitions and actions.

### Settings.json Example 📝

```json
{
  "DefaultHotkey": "Shift + F6",
  "Language": "en_US",
  "FirstRun": false,
  "Version": "2.0-beta1",
  "Theme": "dark",
  "ActiveMacro": 0
}
```

### Macros.sqlite Overview 🗄️

**Tables:** `Macros`, `MacroActions`

**Macros Table:**
- `id` | `name` | `description` | `hotkey`
- If `hotkey` = DEF, the global hotkey from settings is used.
- Hotkey modifiers: Ctrl, Shift, Alt (LWin, RWin, Fn, Enter are not allowed).

**MacroActions Table:**
- `id` | `macro_id` | `order` | `action_type` | `click_type` | `repeat` | `position` | `current_pos` | `interval` | `hold_duration` | `click_count` | `mouse_button` | `key_name`
- `repeat = 0` means unlimited.
- `position`, `action_type`, `click_type`, `mouse_button`, `key_name` are strings; others are integers.
- Some fields are ignored for certain action types (e.g., mouse button for keyboard or key_name for mouse).
- The default macro `DEFAULT` is automatically created if none exist and cannot be deleted.

## Simple Explanation for Users 💡

- You can make the app **look the way you want** using themes.
- You can **create macros** for mouse and keyboard, decide exactly what they do, how many times, how fast, etc.
- All data is saved in your **AppData folder**, so nothing gets lost.
- You don't need to worry about the files; the app handles SQLite and JSON automatically.

## Automatic Updater 🔄

- The app includes `AutoClickerUpdater.exe`.
- It checks GitHub for new versions, downloads, and installs automatically.
- If no update is available, it opens the main app directly.

## About 📞

For your suggestions: lastplayer8945@gmail.com

[Instagram](https://instagram.com/ilpen.25)

[YouTube](https://youtube.com/@ilpenwastaken)

[X](https://x.com/ilpenwastaken)

# 🖱️ Auto Clicker 2.0 - Tamamen Yenilenmiş Makro Tıklayıcı

C++ ile yazıldı, Qt kullanılarak oluşturuldu ve Fluent UI ile modernleştirildi.

## 📋 Genel Bakış

Auto Clicker 2.0 tamamen modern, hızlı ve özelleştirilebilir bir makro tıklayıcıdır. Fare ve klavye makrolarınızı kolayca oluşturabilir, kaydedebilir ve yönetebilirsiniz. Her makro için ayrı veya global kısayol tuşları belirleyebilirsiniz.

Tüm kullanıcı verileri, makrolar, temalar ve ayarlar **AppData/Roaming/AutoClicker2** klasöründe saklanır.

## 📸 Ekran Görüntüleri

### Ana Menü
<img width="901" height="633" alt="ss1_tr" src="https://github.com/user-attachments/assets/38cc4a9c-a753-4805-b031-89da2ec38943" />

### Makro Seçim Ekranı
<img width="901" height="626" alt="ss2_tr" src="https://github.com/user-attachments/assets/091ba2d1-b598-4c20-a446-2bba896949cf" />

### Ayarlar Menüsü
<img width="899" height="628" alt="ss3_tr" src="https://github.com/user-attachments/assets/87cecd75-ed26-41fa-91a8-0d9e0241da2b" />

### Hakkında Menüsü
<img width="899" height="627" alt="ss4_tr" src="https://github.com/user-attachments/assets/81f0e32c-127a-4e57-b9d6-c2b300189c91" />

## ✨ Özellikler

- **🎨 Özelleştirilebilir Temalar**
  - Dark, Light ve kendi `.qss` dosyanızla oluşturabileceğiniz özel temalar.

- **🌍 Diller**
  - İngilizce, Türkçe, Almanca, İtalyanca, Fransızca.
  - Her kullanıcı kendi dilini seçebilir.

- **⚙️ Makrolar**
  - Tamamen özelleştirilebilir fare ve klavye eylemleri: tıklama, basılı tutma, üzerinde gezinme.
  - Her makro için sınırsız eylem.
  - Fare eylemleri: CLICK (aralıklı tıklayıcı), HOLD (basılı tutma), HOVER (konumun üzerinde gezinme).
  - Klavye eylemleri: CLICK (aralıklı) ve HOLD.
  - Tüm eylemler yapılandırılabilir tekrar sayısı, aralık, basılı tutma süresi, tıklama sayısı ve fare düğmesi (uygunsa) içerir.

## 📁 Dosya Yapısı (AppData)

```
AppData/Roaming/AutoClicker2
├─ logs/
│  └─ 12.08.2025-13:00:00.log
├─ themes/
│  ├─ dark.qss
│  ├─ light.qss
│  └─ mytheme.qss
├─ settings.json
├─ themes.json
└─ macros.sqlite
```

- `settings.json` dil, aktif makro, tema ve global kısayol gibi uygulama tercihlerini saklar.
- `macros.sqlite` tüm makro tanımları ve eylemlerini saklar.

### 📝 Settings.json Örneği

```json
{
  "DefaultHotkey": "Shift + F6",
  "Language": "tr_TR",
  "FirstRun": false,
  "Version": "2.0-beta1",
  "Theme": "dark",
  "ActiveMacro": 0
}
```

### 🗄️ Macros.sqlite Genel Bakış

**Tablolar:** `Macros`, `MacroActions`

**Macros Tablosu:**
- `id` | `name` | `description` | `hotkey`
- Eğer `hotkey` = DEF ise, ayarlardan global kısayol kullanılır.
- Kısayol değiştiricileri: Ctrl, Shift, Alt (LWin, RWin, Fn, Enter izin verilmez).

**MacroActions Tablosu:**
- `id` | `macro_id` | `order` | `action_type` | `click_type` | `repeat` | `position` | `current_pos` | `interval` | `hold_duration` | `click_count` | `mouse_button` | `key_name`
- `repeat = 0` sınırsız anlamına gelir.
- `position`, `action_type`, `click_type`, `mouse_button`, `key_name` string'lerdir; diğerleri integer'dır.
- Bazı alanlar belirli eylem türleri için yoksayılır (örn., klavye için mouse button veya mouse için key_name).
- Varsayılan `DEFAULT` makrosu hiçbiri yoksa otomatik oluşturulur ve silinemez.

## 💡 Kullanıcılar için Basit Açıklama

- Uygulamanın görünümünü **temalarla istediğiniz gibi değiştirebilirsiniz**.
- Fare ve klavye makroları **tam olarak istediğiniz gibi** oluşturabilirsiniz: tıklama sayısı, aralık, basılı tutma süresi vs.
- Tüm veriler **AppData klasöründe** saklanır, kaybolmaz.
- Dosyalarla uğraşmanız gerekmez, SQLite ve JSON otomatik olarak yönetilir.

## 🔄 Otomatik Güncelleyici

- Uygulama `AutoClickerUpdater.exe` ile birlikte gelir.
- GitHub'dan yeni sürümleri kontrol eder, indirir ve yükler.
- Güncelleme yoksa direkt ana uygulama açılır.

## 📞 Hakkında

Önerileriniz için: lastplayer8945@gmail.com

[Instagram](https://instagram.com/ilpen.25)

[YouTube](https://youtube.com/@ilpenwastaken)

[X](https://x.com/ilpenwastaken)
