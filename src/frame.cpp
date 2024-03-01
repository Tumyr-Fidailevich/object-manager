#include "frame.h"

Frame::Frame(QWidget* parent) : QFrame(parent)
{
    setupUi();
    connectSlots();
}

QJsonObject Frame::createJsonDump()
{
    QJsonObject dump;

    QJsonObject propertyObject;

    QString objectName = _comboBox->currentText();

    for (int i = 0; i < _propertyVLayout->count() - 1; i++) {
        auto [propertyName, description] = getPropetryAndDescriptionByIndex(i);

        if(!propertyName.isEmpty()) propertyObject[propertyName] = QJsonValue(description);
    }
    dump[objectName] = propertyObject;
    return dump;
}

void Frame::forwardDeleteButtonPressed()
{
    emit deleteSelf();
}

void Frame::addProperty()
{
    QLineEdit* newProperty = new QLineEdit();
    QLineEdit* newDescription = new QLineEdit();
    addNewPropertyAndDescription(newProperty, newDescription);
}

void Frame::removeProperty()
{
    if(!(_propertyVLayout->count() <= 2))
    {
        auto itemToRemoveIndex = _propertyVLayout->count() - 2;
        auto propertyItemToRemove = _propertyVLayout->itemAt(itemToRemoveIndex);
        auto descriptionItemToRemove = _descriptionVLayout->itemAt(itemToRemoveIndex);
        _propertyVLayout->removeItem(propertyItemToRemove);
        _descriptionVLayout->removeItem(descriptionItemToRemove);
        if(propertyItemToRemove->widget() && descriptionItemToRemove->widget())
        {
            delete propertyItemToRemove->widget();
            delete descriptionItemToRemove->widget();
            propertyItemToRemove->widget()->setParent(nullptr);
            descriptionItemToRemove->widget()->setParent(nullptr);
        }
        delete propertyItemToRemove;
        delete descriptionItemToRemove;
    }
}

void Frame::changeComboBoxIndex(int index)
{
    QFile file(":/json/defaoult_objects.json");
    auto objectName = _comboBox->itemText(index);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        auto jsonData = file.readAll();
        auto jsonDocument = QJsonDocument::fromJson(jsonData);
        if (!jsonDocument.isNull()) {
            auto rootJsonObject = jsonDocument.object();
            QJsonObject propertyObject = rootJsonObject[objectName].toObject()["Property"].toObject();

            clearPropertiesAndDescriptionsLayouts();

            for (auto it = propertyObject.begin(); it != propertyObject.end(); ++it) {
                QString key = it.key();
                QString value = it.value().toString();

                auto propertyLineEdit = new QLineEdit();
                auto descriptionLineEdit = new QLineEdit();

                propertyLineEdit->setText(key);
                descriptionLineEdit->setText(value);

                addNewPropertyAndDescription(propertyLineEdit, descriptionLineEdit);
            }
        }
    }
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

void Frame::clearPropertiesAndDescriptionsLayouts()
{
    while(_propertyVLayout->count() > 1)
    {
        auto propertyItem = _propertyVLayout->takeAt(0);
        auto descriptionItem = _descriptionVLayout->takeAt(0);
        if(propertyItem->widget())
        {
            delete propertyItem->widget();
        }
        if(descriptionItem->widget())
        {
            delete descriptionItem->widget();
        }
        delete propertyItem;
        delete descriptionItem;
    }
}

void Frame::addNewPropertyAndDescription(QLineEdit* propertyLineEdit, QLineEdit* descriptionLineEdit)
{
    _propertyVLayout->insertWidget(_propertyVLayout->count() - 2, propertyLineEdit);
    _descriptionVLayout->insertWidget(_descriptionVLayout->count() - 2, descriptionLineEdit);
}

std::pair<QString, QString> Frame::getPropetryAndDescriptionByIndex(int index)
{
    QLayoutItem* propertyItem = _propertyVLayout->itemAt(index);
    QLayoutItem* descriptionItem = _descriptionVLayout->itemAt(index);
    QString propertyName;
    QString description;
    if (propertyItem && propertyItem->widget())
    {
        propertyName = qobject_cast<QLineEdit*>(propertyItem->widget())->text();
    }
    if(descriptionItem && descriptionItem->widget())
    {
        description = qobject_cast<QLineEdit*>(descriptionItem->widget())->text();
    }
    return {propertyName, description};
}

