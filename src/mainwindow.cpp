#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
    , _absolutePath()
{
    _ui->setupUi(this);
    updateSavedStatus(false);
    setupActions();
    connectSlots();
}

MainWindow::~MainWindow() {}

Frame* MainWindow::createFrame()
{
    auto newFrame = new Frame(this);

    connect(newFrame, &Frame::deleteSelf, this, &MainWindow::deleteFrame);
    connect(newFrame, &Frame::textChanged, this, &MainWindow::updateSavedStatus);
    connect(newFrame, &Frame::changeProperties, this, &MainWindow::updateSavedStatus);

    updateSavedStatus(false);

    _ui->scrollAreaVLayout->addWidget(newFrame);

    return newFrame;
}

void MainWindow::deleteFrame()
{
    auto frameForDelete = qobject_cast<QFrame*>(sender());
    if(frameForDelete)
    {
        _ui->scrollAreaVLayout->removeWidget(frameForDelete);
        frameForDelete->deleteLater();
    }
}

void MainWindow::saveActionSlot()
{
    if(!_saved)
    {
        if(_absolutePath.isEmpty()) _absolutePath = getDestinationFilePathByQuestionWindow("Choose file to save", QFileDialog::AcceptSave);

        if(!_absolutePath.isEmpty())
        {
            updateSavedStatus(save(_absolutePath));
        }
    }
}

void MainWindow::saveAsActionSlot()
{
    auto absolutePath = getDestinationFilePathByQuestionWindow("Choose file to save", QFileDialog::AcceptSave);
    save(absolutePath);
}


void MainWindow::openActionSlot()
{
    if(!_saved)
    {
        auto questionResult = createQuestionMessageBox();

        if(questionResult == QMessageBox::Save)
        {
            if(_absolutePath.isEmpty()) _absolutePath = getDestinationFilePathByQuestionWindow("Choose file to save", QFileDialog::AcceptSave);

            if(!_absolutePath.isEmpty())
            {
                updateSavedStatus(save(_absolutePath));
            }
        }
    }

    _absolutePath = getDestinationFilePathByQuestionWindow("Choose file to open", QFileDialog::AcceptOpen);

    if(!_absolutePath.isEmpty())
    {
        clearFrames();
        auto rootJsonObject = open(_absolutePath);
        if(rootJsonObject.isEmpty() || !rootJsonObject.contains("version"))
        {
            _absolutePath.clear();
            updateSavedStatus(false);
            return;
        }

        for(auto it = rootJsonObject.begin(); it != rootJsonObject.end(); it++)
        {
            if(it.key() == "version") continue;
            auto objectName = it.key();
            auto propertyObject = it->toObject()["Property"].toObject();
            auto newFrame = createFrame();
            newFrame->reInitializeFromJson(propertyObject);
            newFrame->setObjectName(objectName);
        }
        updateSavedStatus(true);
    }
}

void MainWindow::newActionSlot()
{
    if(!_saved)
    {
        auto questionResult = createQuestionMessageBox();

        if(questionResult == QMessageBox::Save)
        {
            if(_absolutePath.isEmpty()) _absolutePath = getDestinationFilePathByQuestionWindow("Choose file to save", QFileDialog::AcceptSave);

            if(!_absolutePath.isEmpty())
            {
                updateSavedStatus(save(_absolutePath));
            }
        }
    }

    clearFrames();

    _absolutePath.clear();

    updateSavedStatus(false);

}

void MainWindow::versionActionSlot()
{
    createVersionMessageBox();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!_saved)
    {
        auto questionResult = createQuestionMessageBox();

        if(questionResult == QMessageBox::Save)
        {
            if(_absolutePath.isEmpty()) _absolutePath = getDestinationFilePathByQuestionWindow("Choose file to save", QFileDialog::AcceptSave);

            if(!_absolutePath.isEmpty())
            {
                updateSavedStatus(save(_absolutePath));
            }
        }
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

void MainWindow::connectSlots()
{
    connect(_ui->buttonAddFrame, &QPushButton::pressed, this, &MainWindow::createFrame);
}

QFileDialog* MainWindow::createFileDialog(const QString& title, QFileDialog::AcceptMode acceptMode, QWidget* parent)
{
    auto fileDialog = new QFileDialog(parent);
    fileDialog->setWindowTitle(title);
    fileDialog->setAcceptMode(acceptMode);
    fileDialog->setNameFilter("JSON files (*.json)");
    fileDialog->setDefaultSuffix("json");
    return fileDialog;
}

QMessageBox::StandardButton MainWindow::createQuestionMessageBox()
{
    return QMessageBox::question(this, "Question", "Do you want to save changes?",
                                      QMessageBox::Save | QMessageBox::Discard);
}

QMessageBox::StandardButton MainWindow::createVersionMessageBox()
{
    return  QMessageBox::information(this, "Info", "Version 1.0");
}

QMessageBox::StandardButton MainWindow::createErrorMessageBox(const QString& messageTitle, const QString& message)
{
    return QMessageBox::critical(this, tr(messageTitle.toStdString().c_str()), tr(messageTitle.toStdString().c_str()));
}

bool MainWindow::save(const QString& absolutePath)
{
    QJsonObject resultJsonObject;

    for(int i = 0; i < _ui->scrollAreaVLayout->count(); i++)
    {
        auto frame = qobject_cast<Frame*>(_ui->scrollAreaVLayout->itemAt(i)->widget());
        auto frameJson = frame->createJsonDump();
        resultJsonObject[frameJson.keys().constFirst()] = frameJson.value(frameJson.keys().constFirst());
    }

    resultJsonObject["version"] = 1.0;

    QFile file(absolutePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        createErrorMessageBox("Error", "Cannot open file");
        return false;
    }

    QJsonDocument jsonDocument(resultJsonObject);

    file.write(jsonDocument.toJson());
    file.close();
    return true;
}

QJsonObject MainWindow::open(const QString& absolutePath)
{
    QFile file(absolutePath);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        createErrorMessageBox("Error", "Cannot open file");
        return {};
    }
    auto jsonData = file.readAll();
    file.close();

    auto jsonDocument = QJsonDocument::fromJson(jsonData);
    if (jsonDocument.isNull()) {
        createErrorMessageBox("Error", "Null json object");
        return {};
    }

    auto rootJsonObject = jsonDocument.object();
    return rootJsonObject;
}

void MainWindow::updateSavedStatus(bool saved)
{
    if(saved)
    {
        this->setWindowTitle(QFileInfo(_absolutePath).fileName());
    }else
    {
        if(_absolutePath.isEmpty())
            this->setWindowTitle("*" + QString("unsaved.json"));
        else
            this->setWindowTitle("*" + QFileInfo(_absolutePath).fileName());
    }
    _saved = saved;
}

void MainWindow::clearFrames()
{
    while(!_ui->scrollAreaVLayout->isEmpty())
    {
        auto itemToRemove = _ui->scrollAreaVLayout->itemAt(0);
        _ui->scrollAreaVLayout->removeItem(itemToRemove);
        if(itemToRemove->widget())
        {
            itemToRemove->widget()->setParent(nullptr);
            delete itemToRemove->widget();
        }
        delete itemToRemove;
    }
}

QString MainWindow::getDestinationFilePathByQuestionWindow(const QString& title, QFileDialog::AcceptMode acceptMode)
{
    auto fileDialog = createFileDialog(title, acceptMode);
    fileDialog->exec();

    auto selectedFiles = fileDialog->selectedFiles();
    if(selectedFiles.isEmpty()) return {};
    auto selectedFile = selectedFiles.first();
    QFileInfo fileInfo(selectedFile);

    return fileInfo.absoluteFilePath();
}
