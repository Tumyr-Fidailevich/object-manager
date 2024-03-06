#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QCloseEvent>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <memory>
#include "frame.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    Frame* createFrame();

    void deleteFrame();

    void saveActionSlot();

    void saveAsActionSlot();

    void openActionSlot();

    void newActionSlot();

    void versionActionSlot();

    void updateSavedStatus(bool status);

private:
    std::unique_ptr<Ui::MainWindow> _ui;
    bool _saved;
    QString _absolutePath;

    void closeEvent(QCloseEvent *event) override;

    void setupActions();

    void connectSlots();

    QFileDialog* createFileDialog(QWidget* parent = nullptr);

    QMessageBox::StandardButton createQuestionMessageBox();

    QMessageBox::StandardButton createVersionMessageBox();

    QMessageBox::StandardButton createErrorMessageBox(const QString& messageTitle, const QString& message);

    bool save(const QString& absolutePath);

    QJsonObject open(const QString& absolutePath);

    void processUnsavedDocument();

    void clearFrames();

    QString getDestinationFilePathByQuestionWindow();
};
#endif // MAINWINDOW_H
