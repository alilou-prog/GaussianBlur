#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QString>
#include <QGraphicsScene>

#include <QFileDialog>
#include <QDebug>
#include <QColor>

#include <math.h>
#include "matrix.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mRessourceDir("/home/ali/Pictures")
    , mImage{nullptr}, mBlurredImage{nullptr}
{
    ui->setupUi(this);
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::load);
    connect(ui->displayButton, &QPushButton::clicked, this, &MainWindow::display);
    connect(ui->transformButton, &QPushButton::clicked, this, &MainWindow::transform);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::clear);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(mImage)
        delete mImage;
    if(mBlurredImage)
        delete mBlurredImage;
}

// load an mImage from a file. We can only load one mImage
void MainWindow::load()
{
    if(mImage != nullptr)
    {
        std::cerr << "An image is already loaded, display it!" << std::endl;
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Load Image"),
                                                    mRessourceDir,
                                                    tr("Image Files (*.png *.jpeg)"));
    if (fileName == "")
        return;

    // create a QImage
    mImage = new QImage(fileName);
}

void MainWindow::display()
{
    if(!mImage)
    {
        qDebug() << "No image is loaded";
        return;
    }
    QPixmap p {QPixmap::fromImage(*mImage)};

    if( ! ui->graphicsView->scene())
    {
        qDebug() << "No scene !";
        auto * scene = new QGraphicsScene(this);
        ui->graphicsView->setScene(scene);
    }

    ui->graphicsView->scene()->addPixmap(p);
}

void MainWindow::clear()
{
    if(ui->graphicsView->scene())
    {
        ui->graphicsView->scene()->clear();
    }
}

matrix<double> createKernel(int radius)
{
    // make kernelWidth odd
    int kernelWidth = (radius * 2 + 1);

    // width = nRows = nCols
    matrix<double> kernel(kernelWidth, kernelWidth);

    // computations needed for the G(x,y) functions and does not depend on x and y
    int sigma { std::max((radius / 2 ), 1) };
    double leftExp = (double) 1 / (2 * M_PI * sigma * sigma);
    double expDenom = (double) (2 * sigma * sigma);

    // sum is needed to normalize the kernel (all values sums up to 1)
    double sum{0};

    // populating kernel matrix
    for(int kerX{-radius}; kerX < radius; ++kerX)
    {
        for(int kerY{-radius}; kerY < radius; ++kerY)
        {
            double expNom = (double) (- (kerX*kerX + kerY*kerY));
            double kernelValue = leftExp * exp(expNom / expDenom);

            kernel[kerX + radius][kerY + radius] = kernelValue;

            sum += kernelValue;
        }
    }
    // populating kernel matrix

    // normalize the kernel
    for(int kerX{-radius}; kerX < radius; ++kerX)
    {
        for(int kerY{-radius}; kerY < radius; ++kerY)
        {
            kernel[kerX + radius][kerY + radius] /= sum;
        }
    }
    // normalize the kernel

    return kernel;
}

void MainWindow::transform()
{
    if(! ui->graphicsView->scene())
    {
        std::cerr << "No scene" << std::endl;
    }

    mBlurredImage = new QImage(transformHelper(7)); // use move constructor
    if(mBlurredImage != nullptr)
    {
        QPixmap p {QPixmap::fromImage(*mBlurredImage)};
        ui->graphicsView->scene()->addPixmap(p);
    }
}

// gaussian blur filter : return a Blurred Image
QImage MainWindow::transformHelper(int radius)
{
    if(!mImage)
        return {};

    QImage blurredImage (mImage->width(), mImage->height(), mImage->format());

    matrix<double> kernel = createKernel(radius);

    for(int x{radius + 1}; x < mImage->width() - radius; ++x)
    {
        for(int y{radius + 1}; y < mImage->height() - radius ; ++y)
        {
            // for each pixel (x,y) :
            // we will compute the new rgb values
            uint red{0}, green{0}, blue{0};

            // kernel matrix iteration
            for(int kerX{-radius}; kerX <= radius; ++kerX)
            {
                for(int kerY{-radius}; kerY <= radius; ++kerY)
                {
                    // this is the "weighted value" from the kernel matrix
                    // it characterise (kerX, kerY)
                    double kernelValue { kernel[kerX+radius][kerY+radius] };

                    // color += pixel(x+kerX, y+kerY).color * weightedValue (which is kernelValue)
                    red += qRed( (mImage->pixel(x+kerX, y+kerY)) ) * kernelValue;
                    green += qGreen ( (mImage->pixel(x+kerX, y+kerY)) ) * kernelValue;
                    blue += qBlue ( (mImage->pixel(x+kerX, y+kerY)) ) * kernelValue;
                }
            }
            // kernel matrix iteration

            QColor color(red, green, blue);
            // now set the pixel new value
            blurredImage.setPixelColor(x,y, color);
        }
    }

    return blurredImage;
}
