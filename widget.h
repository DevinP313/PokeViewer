#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>
#include <QPixmap>
#include <QImage>
#include <QDebug>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    int n;
    QVBoxLayout* myVBoxLayout;
    QLineEdit* myEdit;
    QPushButton* myButton;
    QGridLayout* myGrid;
    QLabel* myLabels;
    QImage img[100];
    int n_image_rec;

public slots:
    void onSubmitButton();
    void request_json(QString urlStr);
    void request_image(QString urlStr, int index);
    void response_json(QNetworkReply *reply);
    void response_image(QNetworkReply *reply);
    void setupImageGrid();

protected:
    void paintEvent(QPaintEvent *evt);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
