#pragma once

#include <KTextEditor/Plugin>
#include <KTextEditor/MainWindow>
#include <KXMLGUIClient>
#include <QDockWidget>
#include <QLabel>

class ImagePreviewPlugin : public KTextEditor::Plugin
{
    Q_OBJECT
public:
    explicit ImagePreviewPlugin(QObject *parent = nullptr, const QList<QVariant> & = QList<QVariant>())
        : KTextEditor::Plugin(parent) {}

    QObject *createView(KTextEditor::MainWindow *mainWindow) override;
};

class ImagePreviewPluginView : public QObject, public KXMLGUIClient
{
    Q_OBJECT
public:
    explicit ImagePreviewPluginView(ImagePreviewPlugin *plugin, KTextEditor::MainWindow *mainWindow);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private Q_SLOTS:
    void onViewChanged(KTextEditor::View *view);

private:
    KTextEditor::MainWindow *m_mainWindow = nullptr;
    QDockWidget *m_toolview = nullptr;
    QLabel *m_imageLabel = nullptr;
    QPixmap m_originalPixmap;
};
