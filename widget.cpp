#include "widget.h"
#include "ui_widget.h"
#include <QPainter>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    n = 0;

    // Set up UI

    myVBoxLayout = new QVBoxLayout(this);

    this->setLayout(myVBoxLayout);

    myEdit = new QLineEdit(this);
    myButton = new QPushButton("Submit", this);
    myGrid = new QGridLayout(this);

    myVBoxLayout->addWidget(myEdit);
    myVBoxLayout->addWidget(myButton);
    myVBoxLayout->addLayout(myGrid);

    connect(myButton, &QPushButton::clicked, this, &Widget::onSubmitButton);

    myLabels = nullptr;

    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onSubmitButton()
{
    // Get the Pokémon name from the user
    QString pokemonName = myEdit->text().toLower();

    // Construct the PokeAPI URL using the entered Pokémon name
    QString urlStr = "https://pokeapi.co/api/v2/pokemon/" + pokemonName;
    request_json(urlStr);
}


void Widget::request_json(QString urlStr)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);


    QUrl myUrl(urlStr);

    QNetworkRequest myRequest(myUrl);

    connect(manager, &QNetworkAccessManager::finished,
            this, &Widget::response_json);

    manager->get(myRequest); // Make the get request
}

void Widget::request_image(QString urlStr, int index)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QUrl myUrl(urlStr);

    QNetworkRequest myRequest(myUrl);

    myRequest.setAttribute(QNetworkRequest::User, QVariant(index));

    connect(manager, &QNetworkAccessManager::finished,
            this, &Widget::response_image);

    manager->get(myRequest);
}


void Widget::response_json(QNetworkReply *reply)
{
//    if (reply->error() != QNetworkReply::NoError)
//    {
//        qDebug() << "Error downloading Pokemon information:" << reply->errorString();
//        return;
//    }

    QByteArray *myByteArray = new QByteArray();
    myByteArray->append(reply->readAll());

    QJsonDocument myJson = QJsonDocument::fromJson(*myByteArray);

    QJsonObject obj = myJson.object();

    // Extract and print name of Pokémon
    QString name = obj.value("name").toString();
    qDebug() << "Name: " << name;

    // Extract and print the first sprite URL
    QJsonObject sprites = obj.value("sprites").toObject();
    QString spriteUrl = sprites.value("front_default").toString();
    qDebug() << "Sprite URL: " << spriteUrl;

    // Extract and print type(s)
    QStringList types;
    QJsonArray typesArray = obj.value("types").toArray();
    for (int i = 0; i < typesArray.size(); ++i) {
        QString typeName = typesArray[i].toObject().value("type").toObject().value("name").toString();
        types.append(typeName);
    }
    QString typesString = types.join(", ");
    qDebug() << "Type(s): " << typesString;

    // Extract and print ability(ies)
    QStringList abilities;
    QJsonArray abilitiesArray = obj.value("abilities").toArray();
    for (int i = 0; i < abilitiesArray.size(); ++i) {
        QString abilityName = abilitiesArray[i].toObject().value("ability").toObject().value("name").toString();
        abilities.append(abilityName);
    }
    QString abilitiesString = abilities.join(", ");
    qDebug() << "Ability(ies): " << abilitiesString;

    // Extract and print weight
    double weight = obj.value("weight").toDouble() / 10.0;
    qDebug() << "Weight: " << weight;

    // Load the sprite image
    QNetworkAccessManager *imageManager = new QNetworkAccessManager(this);
    QNetworkRequest imageRequest;
    imageRequest.setUrl(QUrl(spriteUrl));

    connect(imageManager, &QNetworkAccessManager::finished,
            this, [=](QNetworkReply *imageReply) {
                if (imageReply->error() != QNetworkReply::NoError) {
                    qDebug() << "Error downloading sprite image:" << imageReply->errorString();
                    return;
                }

                QPixmap spritePixmap;
                if (!spritePixmap.loadFromData(imageReply->readAll())) {
                    qDebug() << "Error loading sprite image.";
                    return;
                }

                // Display the information on the widget screen
                QLabel *nameLabel = new QLabel("Name: " + name, this);
                myVBoxLayout->addWidget(nameLabel);

                QLabel *typesLabel = new QLabel("Type(s): " + typesString, this);
                myVBoxLayout->addWidget(typesLabel);

                QLabel *abilitiesLabel = new QLabel("Ability(ies): " + abilitiesString, this);
                myVBoxLayout->addWidget(abilitiesLabel);

                //QLabel *weightLabel = new QLabel("Weight: " + QString::number(weight), this);
                QLabel *weightLabel = new QLabel("Weight: " + QString::number(weight, 'f', 1) + " kg", this);
                myVBoxLayout->addWidget(weightLabel);

                // Display the sprite image on the widget screen
                QLabel *spriteLabel = new QLabel(this);
                spriteLabel->setPixmap(spritePixmap);
                myVBoxLayout->addWidget(spriteLabel);

                imageManager->deleteLater();
            });

    imageManager->get(imageRequest);
}

void Widget::response_image(QNetworkReply *reply)
{
    // Get the request that generated this response
    QNetworkRequest myRequest = reply->request();

    // Retrieve the index i that we put in the request
    // This indicates where to put the image in our array
    QVariant myVariant = myRequest.attribute(QNetworkRequest::User);

    int original_i = myVariant.toInt();

    qDebug() << "original_i: " << original_i;

    img[original_i].loadFromData(reply->readAll());

    n_image_rec = n_image_rec + 1;

    if (n == n_image_rec)
    {
        setupImageGrid();
        repaint();
    }
}

void Widget::setupImageGrid()
{
    delete[] myLabels;

    myLabels = new QLabel[n];

    int cols = 7;
    int rows = n / cols;

    int leftovers = n % cols;

    qDebug() << "rows: " << rows << "   leftovers: " << leftovers;

    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            int k = cols * i + j;
            myLabels[k].setPixmap(QPixmap::fromImage(img[k]));

            myGrid->addWidget(&myLabels[k], i, j);
        }
    }

    for(int j=0;j<leftovers;j++)
    {
        int k = rows * cols + j;

        myLabels[k].setPixmap(QPixmap::fromImage(img[k]));

        myGrid->addWidget(&myLabels[k], rows, j);
    }
}

//Paints onto the Widget
void Widget::paintEvent(QPaintEvent *evt)
{
    QPainter painter(this);

}


