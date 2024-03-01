#ifndef FRAME_H
#define FRAME_H

#include <QFrame>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QSpacerItem>
#include <QLineEdit>

class Frame : QFrame
{
    Q_OBJECT

public:
    Frame(QWidget* parent = nullptr);
signals:
    void deleteSelf();
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
};

#endif // FRAME_H
