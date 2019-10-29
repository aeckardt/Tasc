#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QDialog>
#include <QIcon>

QT_BEGIN_NAMESPACE
class QImage;
class QScrollArea;
class QScrollBar;
class QVBoxLayout;
class QToolBar;
class QLineEdit;
class QComboBox;
QT_END_NAMESPACE

class ScalableImage;
class LabelEventFilter;
class ZoomToolButtons;

class ImageView : public QDialog
{
    Q_OBJECT

public:
    ImageView();

    void showImage(const QImage &image);

protected:
    void keyPressEvent(QKeyEvent *) override;

private slots:
    void copy();
    void zoomIn();
    void zoomOut();
    void fitToWindow();
    void actualSize();
    void lineEditReturn();
    void comboBoxChange();

private:
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    const QImage *image;
    ScalableImage *scalableImage;
    QScrollArea *scrollArea;
    QToolBar *toolBar;
    QVBoxLayout *mainLayout;
    ZoomToolButtons *zoomToolButtons;
    QLineEdit *scaleEdit;
    QComboBox *scaleComboBox;
    LabelEventFilter *labelEventFilter;

    double scaleFactor;
    QString scaleStr;

    friend LabelEventFilter;
    friend ZoomToolButtons;
};

class ScalableImage : public QWidget
{
    Q_OBJECT

public:
    ScalableImage(QWidget *parent = nullptr);

    void setPixmap(const QPixmap &pixmap);
    void resize(const QSize &);

    const QPixmap *pixmap() const { return &_pixmap; }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap _pixmap;
};

class LabelEventFilter : public QObject
{
    Q_OBJECT

public:
    LabelEventFilter(ImageView *parent) : QObject() { this->parent = parent; }

protected:
    bool eventFilter(QObject *, QEvent *) override;

private:
    ImageView *parent;

    QPoint dragPosition;
};

class ZoomToolButtons : public QWidget
{
    Q_OBJECT

public:
    ZoomToolButtons(ImageView *parent);

    void setZoomInEnabled(bool enabled) { zoomIn.enabled = enabled; }
    void setZoomOutEnabled(bool enabled) { zoomOut.enabled = enabled; }
    void setZoomToFitEnabled(bool enabled) { zoomToFit.enabled = enabled; }
    void setZoomActualSizeEnabled(bool enabled) { zoomActualSize.enabled = enabled; }

protected:
    bool event(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    QSize sizeHint() const override;

private:
    struct ToolButton
    {
        QIcon icon;
        QRect rect;
        bool pressed;
        bool enabled;
    };

    void drawButton(QPainter &painter, const ToolButton &toolButton) const;
    bool buttonPressed(const QPoint &pos, ToolButton &toolButton);

    ImageView *parent;

    ToolButton zoomIn;
    ToolButton zoomOut;
    ToolButton zoomToFit;
    ToolButton zoomActualSize;
};

#endif // IMAGEVIEW_H
