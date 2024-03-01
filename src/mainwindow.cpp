#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
    , _saved(true)
{
    _ui->setupUi(this);
    this->setWindowTitle("unsaved.json");
    setupActions();
}

MainWindow::~MainWindow() {}

void MainWindow::createCell()
{

}

void MainWindow::deleteCell()
{

}

void MainWindow::saveActionSlot()
{

}

void MainWindow::saveAsActionSlot()
{

}

void MainWindow::openActionSlot()
{

}

void MainWindow::newActionSlot()
{

}

void MainWindow::versionActionSlot()
{
    QDialog* versionDialog = new QDialog();
    versionDialog->setAttribute(Qt::WA_DeleteOnClose);

    versionDialog->setWindowTitle("Version");
    versionDialog->exec();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!_saved)
    {

    }
    event->accept();
}

void MainWindow::setupActions()
{
    connect(_ui->actionOpen, &QAction::triggered, this, &MainWindow::openActionSlot);
    connect(_ui->actionNew, &QAction::triggered, this, &MainWindow::newActionSlot);
    connect(_ui->actionSave, &QAction::triggered, this, &MainWindow::saveActionSlot);
    connect(_ui->actionSave_as, &QAction::triggered, this, &MainWindow::saveAsActionSlot);
    connect(_ui->actionVersion, &QAction::triggered, this, &MainWindow::versionActionSlot);
}

void MainWindow::createSaveChangesQuestionWindow()
{

}
