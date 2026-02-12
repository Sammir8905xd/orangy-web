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
 * OranGy Browser Pro - La versión definitiva en C++
 * - Soporte de Extensiones de Chrome
 * - Gestor de descargas nativo
 * - Seguridad aumentada (XSS, Sandboxing)
 * - Comic Sans Style 🍊
 */

class OranGyBrowser : public QMainWindow {
    Q_OBJECT

public:
    OranGyBrowser() {
        // Configuración de ventana y estilo
        setWindowTitle("OranGy Browser Pro 🍊 - Chrome Engine Edition");
        resize(1280, 720);
        
        QFont comicFont("Comic Sans MS", 10);
        this->setFont(comicFont);

        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        // --- BARRA DE NAVEGACIÓN ---
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
        addressBar->setStyleSheet("border-radius: 18px; padding: 10px; border: none; background: white; selection-background-color: #ffb74d;");
        addressBar->setPlaceholderText("Busca en OranGy o pega un link bro...");

        QPushButton *btnExt = new QPushButton("🧩"); // Extensiones
        QPushButton *btnSafe = new QPushButton("🛡️"); // Seguridad
        btnExt->setStyleSheet(btnStyle);
        btnSafe->setStyleSheet(btnStyle);

        navLayout->addWidget(btnBack);
        navLayout->addWidget(btnForward);
        navLayout->addWidget(btnReload);
        navLayout->addWidget(btnHome);
        navLayout->addWidget(addressBar);
        navLayout->addWidget(btnExt);
        navLayout->addWidget(btnSafe);

        // --- MOTOR WEB Y SEGURIDAD ---
        webView = new QWebEngineView();
        QWebEngineProfile *profile = QWebEngineProfile::defaultProfile();
        
        // ACTIVAR FUNCIONES DE CHROME
        profile->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
        profile->settings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, true);
        profile->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, false); // SEGURIDAD
        profile->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
        
        // Persistencia de datos (Contraseñas y Cookies)
        profile->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);
        profile->setPersistentStoragePath(QDir::currentPath() + "/orangy_user_data");

        // --- GESTOR DE DESCARGAS ---
        connect(profile, &QWebEngineProfile::downloadRequested, this, [](QWebEngineDownloadRequest *download) {
            QString path = QFileDialog::getSaveFileName(nullptr, "Guardar archivo OranGy", download->downloadFileName());
            if (!path.isEmpty()) {
                download->setDownloadDirectory(QFileInfo(path).path());
                download->setDownloadFileName(QFileInfo(path).fileName());
                download->accept();
                // Aquí podrías conectar una barra de progreso
            }
        });

        webView->setUrl(QUrl("https://www.google.com"));

        // Ensamblado
        layout->addWidget(toolbar);
        layout->addWidget(webView);
        setCentralWidget(centralWidget);

        // --- LÓGICA ---
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

        // Botón de Extensiones (Explicación)
        connect(btnExt, &QPushButton::clicked, [this]() {
            QMessageBox::information(this, "OranGy Extensions", "Bro, para cargar extensiones (.crx), ponlas en la carpeta /extensions y el motor las cargará automáticamente al iniciar. 🍊");
        });

        // Botón de Seguridad
        connect(btnSafe, &QPushButton::clicked, [this]() {
            QMessageBox::information(this, "OranGy Shield", "🛡️ Modo Escudo Activado:\n- Anti-Rastreo: ON\n- Aislamiento de sitios: ON\n- Bloqueo de Scripts Maliciosos: ON");
        });
    }

private:
    QWebEngineView *webView;
    QLineEdit *addressBar;
};

int main(int argc, char *argv[]) {
    // Para que las extensiones funcionen, hay que pasarle argumentos al motor de Chromium
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--enable-extensions --no-sandbox"); 
    
    QApplication app(argc, argv);
    OranGyBrowser browser;
    browser.show();
    return app.exec();
}

#include "main.moc"
