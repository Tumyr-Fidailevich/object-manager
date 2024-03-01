#include "frame.h"

Frame::Frame(QWidget* parent) : QFrame(parent)
{
    this->setFrameShape(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Raised);

    _mainVLayout = new QVBoxLayout(this);
    _mainHLayout = new QHBoxLayout();
    _deleteButtonHLayout = new QHBoxLayout();
    _mainVLayout->addLayout(_mainHLayout);
    _mainVLayout->addLayout(_deleteButtonHLayout);


    //Delete button
    _deleteButtonHSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _buttonDeleteFrame = new QPushButton();
    _deleteButtonHLayout->addItem(_deleteButtonHSpacer);
    _deleteButtonHLayout->addWidget(_buttonDeleteFrame);


    //ComboBox

    _comboBox = new QComboBox();
    _mainHLayout->addWidget(_comboBox);

    //PropertyLayout
    _propertyVLayout = new QVBoxLayout();
    _mainHLayout->addLayout(_propertyVLayout);
    QLineEdit* propertyLineEdit = new QLineEdit();
    _propertyVLayout->addWidget(propertyLineEdit);

    _buttonAddProperty = new QPushButton();
    _buttonRemoveProperty = new QPushButton();
    _buttonsHLayout = new QHBoxLayout();
    _propertyVLayout->addLayout(_buttonsHLayout);
    _buttonsHLayout->addWidget(_buttonAddProperty);
    _buttonsHLayout->addWidget(_buttonRemoveProperty);

    //DesccriptionLayout
    _descriptionVLayout = new QVBoxLayout();
    _mainHLayout->addLayout(_descriptionVLayout);
    QLineEdit* descriptionLineEdit = new QLineEdit();
    _descriptionVLayout->addWidget(descriptionLineEdit);

    _descriptionHLayout = new QHBoxLayout();
    _descriptionVLayout->addLayout(_descriptionHLayout);

    connectSlots();
}

void Frame::forwardDeleteButtonPressed()
{
    emit deleteSelf();
}

void Frame::addProperty()
{

}

void Frame::removeProperty()
{

}

void Frame::connectSlots()
{
    connect(_buttonAddProperty, &QPushButton::pressed, this, &Frame::addProperty);
    connect(_buttonRemoveProperty, &QPushButton::pressed, this, &Frame::removeProperty);
    connect(_buttonDeleteFrame, &QPushButton::pressed, this, &Frame::forwardDeleteButtonPressed);
}
