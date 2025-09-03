# 🤝 Contributing to Auto Clicker 2.0

Hello! Thanks for wanting to contribute to the Auto Clicker 2.0 project! 🎉 This guide explains step by step how you can contribute. Don't worry, we've prepared it to help developers of all levels.

## 📚 Table of Contents

- [🚀 Before You Start](#-before-you-start)
- [⚙️ Setting Up Development Environment](#️-setting-up-development-environment)
- [🎯 How to Contribute](#-how-to-contribute)
- [📝 Code Style Guidelines](#-code-style-guidelines)
- [🧪 Testing](#-testing)
- [🔄 Pull Request Process](#-pull-request-process)
- [🐛 Bug Reporting](#-bug-reporting)
- [🔐 Security](#-security)

## 🚀 Before You Start

### 🎯 How Can You Help?

- **🐛 Bug fixes**: Solving existing problems
- **✨ New features**: Expanding macro functionality
- **🎨 UI/UX improvements**: Enhancing Fluent Design
- **🌍 Translations**: Adding new language support
- **📖 Documentation**: Improving README and help files
- **⚡ Performance**: Making the app faster

### 🛠️ What Do You Need to Know?

- **C++**: Basic C++ knowledge (pointers, classes, STL)
- **Qt Framework**: Qt Creator, Widgets, Signals/Slots
- **SQLite**: Simple database operations
- **JSON**: Understanding configuration files
- **Git**: Version control

**💡 Tip**: If you're new, start with issues tagged `good first issue`!

## ⚙️ Setting Up Development Environment

### 📋 Requirements

- **Qt 6.2+** (with Qt Creator)
- **C++17** compatible compiler
- **SQLite3** (usually comes with Qt)
- **Git**
- **Windows SDK** (for Windows development)

### 🔧 Installation Steps

1. **Fork the repository** 🍴
   ```bash
   # Click Fork button on GitHub, then:
   git clone https://github.com/your-username/autoclicker.git
   cd autoclicker
   ```

2. **Open Qt Creator** 🖥️
   - "Open Project" → Select `CMakeLists.txt` or `.pro` file
   - Choose Build Kit (Desktop Qt 6.x.x MSVC2022 64bit)

3. **Check dependencies** ✅
   ```cpp
   // These header files should be available:
   #include <QtWidgets>
   #include <QtSql>
   #include <QApplication>
   ```

4. **Build the project** 🔨
   - Build with Ctrl+B
   - If no errors, run with: Ctrl+R

### 📁 Understanding Project Structure

```
autoclicker/
├── src/                    # Main source code
│   ├── main.cpp           # Entry point
│   ├── mainwindow.cpp     # Main window
│   ├── macroengine.cpp    # Macro engine
│   └── database.cpp       # SQLite operations
├── ui/                    # Qt UI files
│   ├── mainwindow.ui
│   └── settings.ui
├── resources/             # QSS, icons, translations
│   ├── themes/
│   └── translations/
├── build/                 # Build outputs
└── README.md
```

## 🎯 How to Contribute

### 1. 🔍 Choose or Create an Issue

- **Look at existing issues**: Check problems waiting to be solved
- **Create new issue**: For a new feature or bug
- **Ask "Can I take this?"**: Comment on the issue to show you want to work on it

### 2. 🌿 Create a Branch

```bash
# Create updated branch from main branch
git checkout main
git pull upstream main
git checkout -b feature/new-feature-name

# Example branch names:
# feature/theme-editor
# fix/sqlite-connection-bug
# docs/update-readme-en
```

### 3. 💻 Coding

```cpp
// Example: Adding a new macro action
class HoverAction : public MacroAction {
private:
    QPoint targetPosition;
    int hoverDuration;

public:
    HoverAction(QPoint pos, int duration) 
        : targetPosition(pos), hoverDuration(duration) {}
    
    void execute() override {
        // Move mouse to specified position
        setCursor(targetPosition);
        QThread::msleep(hoverDuration);
    }
};
```

### 4. 🧪 Testing

```cpp
// Unit test example
void MacroEngineTest::testHoverAction() {
    HoverAction hover(QPoint(100, 200), 1000);
    QPoint initialPos = QCursor::pos();
    
    hover.execute();
    
    QPoint finalPos = QCursor::pos();
    QCOMPARE(finalPos, QPoint(100, 200));
}
```

## 📝 Code Style Guidelines

### 🎨 C++ Style Guide

```cpp
// ✅ CORRECT - PascalCase class names
class MacroEngine {
private:
    int clickInterval;        // camelCase variables
    bool isRunning;
    
public:
    // camelCase method names
    void startMacro();
    void stopMacro();
    
    // Const correctness
    int getInterval() const;
};

// ✅ CORRECT - Header guards
#ifndef MACROENGINE_H
#define MACROENGINE_H
// code...
#endif

// ✅ CORRECT - Namespace usage
using namespace std;  // Only in .cpp files!
```

### 🗂️ File Organization

```cpp
// header file (.h)
class MainWindow : public QMainWindow {
    Q_OBJECT  // Qt macro
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private slots:  // Qt slots
    void onStartClicked();
    void onStopClicked();
    
private:
    Ui::MainWindow *ui;
    MacroEngine *engine;
};
```

### 🌍 Translation System

```cpp
// Making texts ready for translation
QString message = tr("Macro started");        // ✅ CORRECT
QString error = "Error occurred";             // ❌ WRONG - not translatable

// In .ts files:
// "Macro started" → "Makro başlatıldı"
```

## 🧪 Testing

### 🔧 Manual Test Checklist

**When adding new features:**
- [ ] Does UI work correctly?
- [ ] Are translations available in all languages?
- [ ] Does it save correctly to SQLite?
- [ ] Does appearance break when changing themes?
- [ ] Do hotkeys work?

### 🏗️ Build Test

```bash
# Clean build
cd build
make clean  # or Clean All in Qt Creator
make        # or Build All

# Release build test
qmake CONFIG+=release
make
```

### 💾 Data Integrity Test

```cpp
// Settings.json test
QJsonObject settings;
settings["Language"] = "en_US";
settings["Theme"] = "dark";
settings["DefaultHotkey"] = "Ctrl + F6";

// SQLite test
QSqlQuery query;
query.prepare("INSERT INTO Macros (name, description) VALUES (?, ?)");
query.addBindValue("Test Macro");
query.addBindValue("Test description");
bool success = query.exec();
```

## 🔄 Pull Request Process

### 📝 PR Template

Use this when creating a pull request:

```markdown
## 🎯 What Changed?
Brief and clear description...

## 🔧 How Was It Tested?
- [ ] Manual testing done
- [ ] Tested in all languages  
- [ ] Theme compatibility checked

## 📸 Screenshot
(If there are UI changes)

## ⚠️ Breaking Change?
- [ ] Yes, affects existing settings
- [ ] No, backward compatible

## 🔗 Related Issue
Fixes #123
```

### ✅ PR Checklist

**Before submitting:**
- [ ] Code compiles and runs
- [ ] Commit messages are descriptive
- [ ] New code is commented
- [ ] UI changes have screenshots
- [ ] Translation files are updated

**Review process:**
1. **Automated checks** - Is build successful?
2. **Code review** - At least 1 maintainer approval
3. **Manual testing** - Does feature work as expected?
4. **Merge** - Congratulations! 🎉

## 🐛 Bug Reporting

### 🔍 Bug Report Template

```markdown
## 🐛 Bug Description
What did you expect to happen? What happened?

## 💻 System Information
- OS: Windows 10/11 
- Qt Version: 6.2.1
- App Version: 2.0-beta1

## 🔁 How to Reproduce?
1. Click "New Macro" from main menu
2. Add "Mouse Click"
3. Enter coordinates (0,0)
4. Press Start button
5. 💥 Error occurs

## 📊 Log Files
Attach relevant .log file from AppData/Roaming/AutoClicker2/logs/

## 📸 Screenshot
(Error screen if available)
```

### 🎯 Feature Request Template

```markdown
## 💡 Feature Idea
What problem do you want to solve?

## 🎨 How Should It Work?
Detailed description...

## 🎯 Use Case
"When I'm playing games... I want to do..."

## 🔧 Alternative Solutions
How else could this be done?

## ➕ Additional Information
Examples from similar applications if available
```

## 🔐 Security

### 🚨 Security Vulnerability Reporting

**⚠️ WARNING**: DO NOT report security vulnerabilities as public issues!

Instead:
1. **Send email**: lastplayer8945@gmail.com
2. **Use "SECURITY" in subject**
3. **Provide detailed description**:
   - Type of vulnerability
   - How it was discovered  
   - Potential impact
   - Suggested solution

### 🛡️ Secure Coding

```cpp
// ✅ CORRECT - Input validation
void setClickInterval(int interval) {
    if (interval < 1 || interval > 86400000) {  // Max 1 day
        mwarn() << "Invalid interval value:" << interval;
        return;
    }
    this->interval = interval;
}

// ✅ CORRECT - SQLite injection prevention
QSqlQuery query;
query.prepare("SELECT * FROM Macros WHERE name = ?");
query.addBindValue(userInput);  // Automatic escaping
```

## 🎉 Contribution Recognition System

### 🏆 Contributors Hall of Fame

Contributors are recognized in:
- **Contributors section** in README.md
- **Special thanks** in About dialog
- **Mentioned** in release notes
- **Special role** on Discord server (future)

### 🥇 Contribution Types

- 🐛 **Bug Hunter**: Those who find bugs
- ✨ **Feature Creator**: Those who add new features  
- 🎨 **Design Master**: UI/UX improvers
- 🌍 **Translator**: Those who do translations
- 📚 **Documentor**: Documentation writers
- 🔧 **Code Reviewer**: Mentors who review code

## 🆘 Getting Help

### 🤔 Stuck Somewhere?

1. **Use GitHub Discussions** - For Q&A
2. **Create issue** - With `question` label
3. **Send email** - lastplayer8945@gmail.com
4. **Social media** - [@ilpenwastaken](https://x.com/ilpenwastaken)

### 🎓 Learning Resources

- **Qt Documentation**: https://doc.qt.io/
- **C++ Reference**: https://cppreference.com/
- **SQLite Tutorial**: https://sqlitetutorial.net/
- **Git Tutorial**: https://learngitbranching.js.org/

## 🤝 Code of Conduct

### ✅ Do's

- 🤗 **Be respectful** - Everyone is learning
- 💡 **Give constructive criticism** - Solution-focused
- 🎯 **Stay on topic** - No personal attacks
- 🌍 **Be inclusive** - Everyone is welcome

### ❌ Don'ts

- 😤 Use offensive language
- 🏃 Steal work (check the issue first)
- 📧 Spam
- 🔒 Share others' private information

---

## 🚀 Ready to Start?

1. **⭐ Star the repository** (for motivation!)
2. **🍴 Fork** and clone
3. **🔍 Find your first issue** (with `good first issue` tag)
4. **💬 Comment "working on this"**
5. **🚀 Start coding!**

**We're always here for your questions! 🤗**

# 🤝 Auto Clicker 2.0'a Katkıda Bulunma Rehberi

Merhaba! Auto Clicker 2.0 projesine katkıda bulunmak istediğin için teşekkürler! 🎉 Bu rehber, nasıl katkıda bulunabileceğini adım adım anlatıyor. Merak etme, her seviyeden geliştiriciye yardımcı olacak şekilde hazırladık.

## 📚 İçindekiler

- [🚀 Başlamadan Önce](#-başlamadan-önce)
- [⚙️ Geliştirme Ortamını Kurma](#️-geliştirme-ortamını-kurma)
- [🎯 Nasıl Katkı Yapılır](#-nasıl-katkı-yapılır)
- [📝 Kod Yazım Standartları](#-kod-yazım-standartları)
- [🧪 Test Etme](#-test-etme)
- [🔄 Pull Request Süreci](#-pull-request-süreci)
- [🐛 Hata Bildirme](#-hata-bildirme)
- [🔐 Güvenlik](#-güvenlik)

## 🚀 Başlamadan Önce

### 🎯 Hangi Konularda Yardım Edebilirsin?

- **🐛 Hata düzeltmeleri**: Var olan sorunları çözmek
- **✨ Yeni özellikler**: Makro işlevselliğini genişletmek
- **🎨 UI/UX iyileştirmeleri**: Fluent Design'ı geliştirmek
- **🌍 Çeviriler**: Yeni dil desteği eklemek
- **📖 Dokümantasyon**: README ve yardım dosyalarını iyileştirmek
- **⚡ Performans**: Uygulamayı daha hızlı hale getirmek

### 🛠️ Ne Bilmen Gerekiyor?

- **C++**: Temel C++ bilgisi (pointerlar, classlar, STL)
- **Qt Framework**: Qt Creator, Widgets, Signals/Slots
- **SQLite**: Basit veritabanı işlemleri
- **JSON**: Ayar dosyalarını anlama
- **Git**: Versiyon kontrolü

**💡 İpucu**: Yeni başlıyorsan `good first issue` etiketli konulardan başla!

## ⚙️ Geliştirme Ortamını Kurma

### 📋 Gereksinimler

- **Qt 6.2+** (Qt Creator ile birlikte)
- **C++17** destekleyen derleyici
- **SQLite3** (genelde Qt ile gelir)
- **Git**
- **Windows SDK** (Windows geliştirmesi için)

### 🔧 Kurulum Adımları

1. **Repository'yi fork et** 🍴
   ```bash
   # GitHub'da Fork butonuna tıkla, sonra:
   git clone https://github.com/senin-kullanici-adin/autoclicker.git
   cd autoclicker
   ```

2. **Qt Creator'ı aç** 🖥️
   - "Open Project" → `CMakeLists.txt` veya `.pro` dosyasını seç
   - Build Kit'ini seç (Desktop Qt 6.x.x MSVC2022 64bit)

3. **Bağımlılıkları kontrol et** ✅
   ```cpp
   // Bu başlık dosyaları yüklenmeli:
   #include <QtWidgets>
   #include <QtSql>
   #include <QApplication>
   ```

4. **Projeyi derle** 🔨
   - Ctrl+B ile derle
   - Hata yoksa çalıştır: Ctrl+R

### 📁 Proje Yapısını Anlama

```
autoclicker/
├── src/                    # Ana kaynak kodlar
│   ├── main.cpp           # Giriş noktası
│   ├── mainwindow.cpp     # Ana pencere
│   ├── macroengine.cpp    # Makro motoru
│   └── database.cpp       # SQLite işlemleri
├── ui/                    # Qt UI dosyaları
│   ├── mainwindow.ui
│   └── settings.ui
├── resources/             # QSS, ikonlar, çeviriler
│   ├── themes/
│   └── translations/
├── build/                 # Derleme çıktıları
└── README.md
```

## 🎯 Nasıl Katkı Yapılır

### 1. 🔍 Issue Seçme veya Oluşturma

- **Mevcut issue'lara bak**: Çözülmeyi bekleyen sorunları incele
- **Yeni issue oluştur**: Yeni bir özellik veya hata için
- **"Atanabilir mi?" diye sor**: Issue'ya yorum yaparak çalışmak istediğini belirt

### 2. 🌿 Branch Oluşturma

```bash
# Ana branch'ten güncel branch oluştur
git checkout main
git pull upstream main
git checkout -b feature/yeni-ozellik-adi

# Örnek branch isimleri:
# feature/theme-editor
# fix/sqlite-connection-bug
# docs/update-readme-tr
```

### 3. 💻 Kodlama

```cpp
// Örnek: Yeni bir makro aksiyonu ekleme
class HoverAction : public MacroAction {
private:
    QPoint targetPosition;
    int hoverDuration;

public:
    HoverAction(QPoint pos, int duration) 
        : targetPosition(pos), hoverDuration(duration) {}
    
    void execute() override {
        // Fareyi belirtilen konuma getir
        setCursor(targetPosition);
        QThread::msleep(hoverDuration);
    }
};
```

### 4. 🧪 Test Etme

```cpp
// Unit test örneği
void MacroEngineTest::testHoverAction() {
    HoverAction hover(QPoint(100, 200), 1000);
    QPoint initialPos = QCursor::pos();
    
    hover.execute();
    
    QPoint finalPos = QCursor::pos();
    QCOMPARE(finalPos, QPoint(100, 200));
}
```

## 📝 Kod Yazım Standartları

### 🎨 C++ Stil Rehberi

```cpp
// ✅ DOĞRU - PascalCase class isimleri
class MacroEngine {
private:
    int clickInterval;        // camelCase değişkenler
    bool isRunning;
    
public:
    // camelCase metod isimleri
    void startMacro();
    void stopMacro();
    
    // Const correctness
    int getInterval() const;
};

// ✅ DOĞRU - Header guard'lar
#ifndef MACROENGINE_H
#define MACROENGINE_H
// kod...
#endif

// ✅ DOĞRU - Namespace kullanımı
using namespace std;  // Sadece .cpp dosyalarında!
```

### 🗂️ Dosya Organizasyonu

```cpp
// header dosyası (.h)
class MainWindow : public QMainWindow {
    Q_OBJECT  // Qt makrosu
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private slots:  // Qt slot'ları
    void onStartClicked();
    void onStopClicked();
    
private:
    Ui::MainWindow *ui;
    MacroEngine *engine;
};
```

### 🌍 Çeviri Sistemi

```cpp
// Metinleri çeviriye hazır hale getirme
QString message = tr("Makro başlatıldı");  // ✅ DOĞRU
QString error = "Error occurred";          // ❌ YANLIŞ - çevrilemiyor

// .ts dosyalarında:
// "Makro başlatıldı" → "Macro started"
```

## 🧪 Test Etme

### 🔧 Manuel Test Checklist

**Yeni özellik eklediğinde:**
- [ ] UI doğru çalışıyor mu?
- [ ] Tüm dillerde çeviri var mı?
- [ ] SQLite'a doğru kaydediyor mu?
- [ ] Tema değişikliğinde görünüm bozuluyor mu?
- [ ] Hotkey'ler çalışıyor mu?

### 🏗️ Build Test

```bash
# Temiz build
cd build
make clean  # veya Qt Creator'da Clean All
make        # veya Build All

# Release build test
qmake CONFIG+=release
make
```

### 💾 Veri Bütünlüğü Testi

```cpp
// Settings.json testi
QJsonObject settings;
settings["Language"] = "tr_TR";
settings["Theme"] = "dark";
settings["DefaultHotkey"] = "Ctrl + F6";

// SQLite testi
QSqlQuery query;
query.prepare("INSERT INTO Macros (name, description) VALUES (?, ?)");
query.addBindValue("Test Makro");
query.addBindValue("Test açıklaması");
bool success = query.exec();
```

## 🔄 Pull Request Süreci

### 📝 PR Template

Pull request oluştururken şunu kullan:

```markdown
## 🎯 Ne Değişti?
Kısa ve net açıklama...

## 🔧 Nasıl Test Edildi?
- [ ] Manuel test yapıldı
- [ ] Tüm dillerde test edildi  
- [ ] Tema uyumluluğu kontrol edildi

## 📸 Ekran Görüntüsü
(UI değişikliği varsa)

## ⚠️ Breaking Change?
- [ ] Evet, mevcut ayarları etkiler
- [ ] Hayır, geriye uyumlu

## 🔗 İlgili Issue
Fixes #123
```

### ✅ PR Kontrol Listesi

**Göndermeden önce:**
- [ ] Kod derlenip çalışıyor
- [ ] Commit mesajları açıklayıcı
- [ ] Yeni kod comment'li
- [ ] UI değişiklikleri screenshot'lı
- [ ] Çeviri dosyaları güncel

**İnceleme süreci:**
1. **Otomatik kontroller** - Build başarılı mı?
2. **Kod incelemesi** - En az 1 maintainer onayı
3. **Manuel test** - Özellik beklendiği gibi çalışıyor mu?
4. **Merge** - Tebrikler! 🎉

## 🐛 Hata Bildirme

### 🔍 Hata Raporu Template'i

```markdown
## 🐛 Hata Açıklaması
Ne olmasını bekliyordun? Ne oldu?

## 💻 Sistem Bilgileri
- OS: Windows 10/11 
- Qt Version: 6.2.1
- App Version: 2.0-beta1

## 🔁 Nasıl Tekrarlanır?
1. Ana menüden "Yeni Makro" tıkla
2. "Fare Tıklama" ekle
3. Koordinat (0,0) gir
4. Başlat butonuna bas
5. 💥 Hata oluşuyor

## 📊 Log Dosyaları
AppData/Roaming/AutoClicker2/logs/ klasöründen ilgili .log dosyasını ekle

## 📸 Ekran Görüntüsü
(Varsa hata ekranı)
```

### 🎯 Özellik İsteği Template'i

```markdown
## 💡 Özellik İdeası
Hangi sorunu çözmek istiyorsun?

## 🎨 Nasıl Çalışsın?
Detaylı açıklama...

## 🎯 Kullanım Senaryosu
"Ben oyun oynarken... yapmak istiyorum"

## 🔧 Alternatif Çözümler
Başka nasıl yapılabilir?

## ➕ Ek Bilgiler
Varsa benzer uygulamalardan örnekler
```

## 🔐 Güvenlik

### 🚨 Güvenlik Açığı Bildirme

**⚠️ UYARI**: Güvenlik açıklarını public issue olarak AÇMA!

Bunun yerine:
1. **Email gönder**: lastplayer8945@gmail.com
2. **"SECURITY" başlığı kullan**
3. **Detaylı açıklama yap**:
   - Açığın türü
   - Nasıl keşfedildiği  
   - Potansiyel etkisi
   - Önerilen çözüm

### 🛡️ Güvenli Kodlama

```cpp
// ✅ DOĞRU - Input validation
void setClickInterval(int interval) {
    if (interval < 1 || interval > 86400000) {  // Max 1 gün
        mwarn() << "Geçersiz interval değeri:" << interval;
        return;
    }
    this->interval = interval;
}

// ✅ DOĞRU - SQLite injection prevention
QSqlQuery query;
query.prepare("SELECT * FROM Macros WHERE name = ?");
query.addBindValue(userInput);  // Otomatik escape
```

## 🎉 Katkı Tanıma Sistemi

### 🏆 Contributors Hall of Fame

Katkıda bulunanlar şuralarda tanınır:
- **README.md**'de Contributors bölümü
- **About dialog**'unda özel teşekkür
- **Release notes**'larda bahsedilme
- **Discord server**'da özel rol (gelecekte)

### 🥇 Katkı Türleri

- 🐛 **Bug Hunter**: Hata bulanlar
- ✨ **Feature Creator**: Yeni özellik ekleyenler  
- 🎨 **Design Master**: UI/UX iyileştirenleri
- 🌍 **Translator**: Çeviri yapanlar
- 📚 **Documentor**: Dokümantasyon yazanlar
- 🔧 **Code Reviewer**: Kod inceleyen mentorlar

## 🆘 Yardım Alma

### 🤔 Takıldığın Yerler?

1. **GitHub Discussions** kullan - Soru-cevap için
2. **Issue oluştur** - `question` label'ıyla
3. **Email gönder** - lastplayer8945@gmail.com
4. **Social medya** - [@ilpenwastaken](https://x.com/ilpenwastaken)

### 🎓 Öğrenme Kaynakları

- **Qt Documentation**: https://doc.qt.io/
- **C++ Reference**: https://cppreference.com/
- **SQLite Tutorial**: https://sqlitetutorial.net/
- **Git Tutorial**: https://learngitbranching.js.org/

## 🤝 Davranış Kuralları

### ✅ Yapılacaklar

- 🤗 **Saygılı ol** - Herkes öğreniyor
- 💡 **Yapıcı eleştiri yap** - Çözüm odaklı
- 🎯 **Konu odaklı kal** - Kişisel saldırı yok
- 🌍 **İnklusif ol** - Herkes hoş karşılanır

### ❌ Yapılmayacaklar

- 😤 Kaba dil kullanma
- 🏃 İş çalma (issue'yu kontrol et)
- 📧 Spam yapma
- 🔒 Başkalarının bilgilerini paylaşma

---

## 🚀 Başlamaya Hazır mısın?

1. **⭐ Repository'yi star'la** (motivasyon için!)
2. **🍴 Fork et** ve clone'la
3. **🔍 İlk issue'nu bul** (`good first issue` tag'ı ile)
4. **💬 Yorumlarda "üzerinde çalışıyorum" yaz**
5. **🚀 Kodlamaya başla!**

**Sorularınız için her zaman buradayız! 🤗**