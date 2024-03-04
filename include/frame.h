#ifndef FRAME_H
#define FRAME_H

#include <QFrame>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QSpacerItem>
#include <QLineEdit>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

class Frame : public QFrame
{
    Q_OBJECT

public:
    Frame(QWidget* parent = nullptr);

    QJsonObject createJsonDump();

    void reInitializeFromJson(const QJsonObject& jsonObject);

    void setObjectName(const QString& objectName);

signals:
    void deleteSelf();

private slots:
    void forwardDeleteButtonPressed();

    void addProperty();

    void removeProperty();

    void changeComboBoxIndex(int index);

private:
    QPushButton* _buttonAddProperty;
    QPushButton* _buttonRemoveProperty;
    QPushButton* _buttonDeleteFrame;

    QComboBox* _comboBox;

    QVBoxLayout* _mainVLayout;
    QHBoxLayout* _mainHLayout;
    QVBoxLayout* _propertyVLayout;
    QVBoxLayout* _descriptionVLayout;
    QHBoxLayout* _descriptionHLayout;
    QHBoxLayout* _buttonsHLayout;
    QHBoxLayout* _deleteButtonHLayout;
    QSpacerItem* _deleteButtonHSpacer;

    void setupUi();

    void connectSlots();

    void clearPropertiesAndDescriptionsLayouts();

    void addNewPropertyAndDescription(QLineEdit* propertyLineEdit, QLineEdit* descriptionLineEdit);

    std::pair<QString, QString> getPropetryAndDescriptionByIndex(int index);

    void addDefaultItemsToComboBox();
};

#endif // FRAME_H
