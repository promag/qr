#include <QApplication>
#include <QClipboard>
#include <QIcon>
#include <QLocale>
#include <QMenu>
#include <QQmlApplicationEngine>
#include <QQuickWidget>
#include <QSystemTrayIcon>
#include <QTranslator>
#include <QQuickStyle>
#include <QQuickWidget>
#include <QWidgetAction>
#include <QWindow>

#include <QZXing.h>

class Dummy : public QWidget
{
public:
    Dummy(QWidget* parent) : QWidget(parent) {}
    QSize sizeHint() const override { return { 200, 120}; }
    QSize minimumSizeHint() const override { return { 200, 120}; }
};

class WindowAction : public QWidgetAction
{
public:
    explicit WindowAction(QWindow* window, QObject *parent)
        : QWidgetAction(parent)
        , m_window(window) {}
protected:
    virtual QWidget *createWidget(QWidget *parent)
    {
        auto dummy = new Dummy(parent);
        auto x = QWidget::createWindowContainer(m_window, dummy);
        x->resize(200, 120);
        return dummy;
    }
private:
    QWindow* m_window;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "qr_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QQuickStyle::setStyle("Material");
    QQuickWindow::setDefaultAlphaBuffer(true);

    QQmlApplicationEngine engine;

    QZXing::registerQMLTypes();
    QZXing::registerQMLImageProvider(engine);

    engine.load(QStringLiteral("qrc:/qr/scanner.qml"));
    auto scanner = static_cast<QWindow*>(engine.rootObjects().at(0));

    WindowAction scanner_action(scanner, nullptr);

    QMenu menu;
    menu.addAction(&scanner_action);
    menu.addSeparator();
    menu.addAction("Quit", &app, &QApplication::quit);

    QSystemTrayIcon tray_icon;
    tray_icon.setIcon(QIcon(QStringLiteral(":/qr/icon.svg")));
    tray_icon.setContextMenu(&menu);
    tray_icon.show();

    QObject::connect(scanner, &QWindow::windowTitleChanged, [&](const QString& title) {
        if (!title.isEmpty()) {
            app.clipboard()->setText(title);
            scanner_action.trigger();
        }
    });
    QObject::connect(&menu, &QMenu::aboutToShow, [=] {
        scanner->setVisible(true);
    });
    QObject::connect(&menu, &QMenu::aboutToHide, [=] {
        scanner->setVisible(false);
    });
    return app.exec();
}
