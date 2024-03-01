#include "frame.h"

Frame::Frame(QWidget* parent) : QFrame(parent)
{
    setupUi();
    connectSlots();
}

void Frame::forwardDeleteButtonPressed()
{
    emit deleteSelf();
}

void Frame::addProperty()
{
    QLineEdit* newProperty = new QLineEdit();
    QLineEdit* newDescription = new QLineEdit();
    _propertyVLayout->insertWidget(_propertyVLayout->count() - 1, newProperty);
    _descriptionVLayout->insertWidget(_descriptionVLayout->count() - 1, newDescription);
}

void Frame::removeProperty()
{
    if(!(_propertyVLayout->count() == 2))
    {
        auto itemToRemoveIndex = _descriptionHLayout->count() - 2;
        auto itemToRemove = _propertyVLayout->itemAt(itemToRemoveIndex);
        _propertyVLayout->removeItem(itemToRemove);
        if(itemToRemove->widget())
        {
            delete itemToRemove->widget();
            itemToRemove->widget()->setParent(nullptr);
        }
        delete itemToRemove;
    }
}

void Frame::changeComboBoxIndex(int index)
{

}

void Frame::setupUi()
{
    //Main
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
}

void Frame::connectSlots()
{
    connect(_buttonAddProperty, &QPushButton::pressed, this, &Frame::addProperty);
    connect(_buttonRemoveProperty, &QPushButton::pressed, this, &Frame::removeProperty);
    connect(_buttonDeleteFrame, &QPushButton::pressed, this, &Frame::forwardDeleteButtonPressed);
    connect(_comboBox, &QComboBox::currentIndexChanged, this, &Frame::changeComboBoxIndex);
}


