#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QDialog>
#include <memory>

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
    void createCell();

    void deleteCell();

    void saveActionSlot();

    void saveAsActionSlot();

    void openActionSlot();

    void newActionSlot();

    void versionActionSlot();

private:
    std::unique_ptr<Ui::MainWindow> _ui;
    bool _saved;

    void closeEvent(QCloseEvent *event) override;

    void setupActions();

    void createSaveChangesQuestionWindow();
};
#endif // MAINWINDOW_H
