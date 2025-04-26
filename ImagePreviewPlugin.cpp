#include "ImagePreviewPlugin.h"

#include <KPluginFactory>
#include <KLocalizedString>
#include <QVBoxLayout>
#include <QPixmap>
#include <QFileInfo>
#include <QResizeEvent>
#include <KTextEditor/Document>
#include <KTextEditor/View>

#include <QMainWindow>

K_PLUGIN_FACTORY_WITH_JSON(ImagePreviewPluginFactory, "plugin.json", registerPlugin<ImagePreviewPlugin>();)

QObject *ImagePreviewPlugin::createView(KTextEditor::MainWindow *mainWindow)
{
    return new ImagePreviewPluginView(this, mainWindow);
}

ImagePreviewPluginView::ImagePreviewPluginView(ImagePreviewPlugin *plugin, KTextEditor::MainWindow *mainWindow)
: m_mainWindow(mainWindow)
{
    (void)plugin;

    m_toolview = new QDockWidget(i18n("Image Preview"), qobject_cast<QWidget *>(mainWindow->window()));
    QWidget *content = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(content);

    m_imageLabel = new QLabel;
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_imageLabel->installEventFilter(this);

    layout->addWidget(m_imageLabel);
    content->setLayout(layout);
    m_toolview->setWidget(content);

    qobject_cast<QMainWindow *>(mainWindow->window())->addDockWidget(Qt::RightDockWidgetArea, m_toolview);
    m_toolview->hide();

    m_imageLabel->installEventFilter(this);

    connect(m_mainWindow, &KTextEditor::MainWindow::viewChanged, this, &ImagePreviewPluginView::onViewChanged);
}

void ImagePreviewPluginView::onViewChanged(KTextEditor::View *view)
{
    if (!view || !view->document())
        return;

    const QString filePath = view->document()->url().toLocalFile();
    QFileInfo fi(filePath);
    if (fi.suffix().compare(QStringLiteral("png"), Qt::CaseInsensitive) == 0 ||
        fi.suffix().compare(QStringLiteral("jpg"), Qt::CaseInsensitive) == 0 ||
        fi.suffix().compare(QStringLiteral("jpeg"), Qt::CaseInsensitive) == 0 ||
        fi.suffix().compare(QStringLiteral("gif"), Qt::CaseInsensitive) == 0) {

        m_originalPixmap = QPixmap(filePath);

    if (!m_originalPixmap.isNull()) {
        m_imageLabel->setPixmap(m_originalPixmap.scaled(
            m_toolview->size(),
                                                        Qt::KeepAspectRatio,
                                                        Qt::SmoothTransformation
        ));
        m_toolview->show();
    }
        } else {
            m_toolview->hide();
        }
}

bool ImagePreviewPluginView::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_imageLabel && event->type() == QEvent::Resize) {
        if (!m_originalPixmap.isNull()) {
            QSize labelSize = m_imageLabel->size();

            m_imageLabel->setPixmap(m_originalPixmap.scaled(
                labelSize,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
            ));
        }
    }
    return false;
}

#include "ImagePreviewPlugin.moc"
