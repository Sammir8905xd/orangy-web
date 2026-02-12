#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QWebEnginePage>
#include <QWebEngineDownloadRequest>
#include <QWebEngineSettings>
#include <QFileDialog>
#include <QProgressBar>
#include <QFont>
#include <QMessageBox>
#include <QDir>

/**
 * OranGy Browser Pro - Versión Definitiva
 * Archivo: Orangy.cpp
 */

class OranGyBrowser : public QMainWindow {
    Q_OBJECT

public:
    OranGyBrowser() {
        setWindowTitle("OranGy Browser Pro 🍊");
        resize(1280, 720);
        
        QFont comicFont("Comic Sans MS", 10);
        this->setFont(comicFont);

        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        QWidget *toolbar = new QWidget();
        toolbar->setStyleSheet("background-color: #ff9800; border-bottom: 2px solid #e68a00;");
        QHBoxLayout *navLayout = new QHBoxLayout(toolbar);

        QString btnStyle = "QPushButton { background-color: #ffb74d; border-radius: 15px; padding: 5px; font-size: 18px; min-width: 45px; border: 1px solid #e68a00; } "
                           "QPushButton:hover { background-color: #ffa726; }";

        QPushButton *btnBack = new QPushButton("⬅️");
        QPushButton *btnForward = new QPushButton("➡️");
        QPushButton *btnReload = new QPushButton("🔄");
        QPushButton *btnHome = new QPushButton("🏠");
        
        btnBack->setStyleSheet(btnStyle);
        btnForward->setStyleSheet(btnStyle);
        btnReload->setStyleSheet(btnStyle);
        btnHome->setStyleSheet(btnStyle);

        addressBar = new QLineEdit();
        addressBar->setStyleSheet("border-radius: 18px; padding: 10px; border: none; background: white;");
        addressBar->setPlaceholderText("Busca en OranGy bro...");

        navLayout->addWidget(btnBack);
        navLayout->addWidget(btnForward);
        navLayout->addWidget(btnReload);
        navLayout->addWidget(btnHome);
        navLayout->addWidget(addressBar);

        webView = new QWebEngineView();
        QWebEngineProfile *profile = QWebEngineProfile::defaultProfile();
        
        // Configuración de persistencia y seguridad
        profile->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
        profile->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
        QString dataPath = QDir::homePath() + "/OranGyBrowser_Data";
        profile->setPersistentStoragePath(dataPath);

        webView->setUrl(QUrl("https://www.google.com"));

        layout->addWidget(toolbar);
        layout->addWidget(webView);
        setCentralWidget(centralWidget);

        // Conexiones de botones
        connect(btnBack, &QPushButton::clicked, webView, &QWebEngineView::back);
        connect(btnForward, &QPushButton::clicked, webView, &QWebEngineView::forward);
        connect(btnReload, &QPushButton::clicked, webView, &QWebEngineView::reload);
        connect(btnHome, &QPushButton::clicked, [this]() { webView->setUrl(QUrl("https://www.google.com")); });
        
        connect(addressBar, &QLineEdit::returnPressed, [this]() {
            QString text = addressBar->text();
            if(!text.contains("://") && !text.contains(".")) {
                webView->setUrl(QUrl("https://www.google.com/search?q=" + text));
            } else {
                if(!text.startsWith("http")) text = "https://" + text;
                webView->setUrl(QUrl(text));
            }
        });

        connect(webView, &QWebEngineView::urlChanged, [this](const QUrl &url) {
            addressBar->setText(url.toString());
        });
    }

private:
    QWebEngineView *webView;
    QLineEdit *addressBar;
};

int main(int argc, char *argv[]) {
    // Flags para extensiones y estabilidad en CI
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--enable-extensions --no-sandbox --disable-setuid-sandbox"); 
    
    QApplication app(argc, argv);
    OranGyBrowser browser;
    browser.show();
    return app.exec();
}

// IMPORTANTE: Se eliminó el #include "Orangy.moc" para que el compilador no busque archivos inexistentes.
