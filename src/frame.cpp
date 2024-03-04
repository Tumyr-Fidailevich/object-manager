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

void Frame::reInitializeFromJson(const QJsonObject& jsonObject)
{
    clearPropertiesAndDescriptionsLayouts();

    for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
        QString key = it.key();
        QString value = it.value().toString();

        auto propertyLineEdit = new QLineEdit(this);
        auto descriptionLineEdit = new QLineEdit(this);

        propertyLineEdit->setText(key);
        descriptionLineEdit->setText(value);

        addNewPropertyAndDescription(propertyLineEdit, descriptionLineEdit);
    }
}

void Frame::setObjectName(const QString& objectName)
{
    _comboBox->setCurrentIndex(-1);
    _comboBox->setCurrentText(objectName);
}

void Frame::forwardDeleteButtonPressed()
{
    emit deleteSelf();
}

void Frame::addProperty()
{
    QLineEdit* newProperty = new QLineEdit(this);
    QLineEdit* newDescription = new QLineEdit(this);
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
            propertyItemToRemove->widget()->setParent(nullptr);
            descriptionItemToRemove->widget()->setParent(nullptr);
            delete propertyItemToRemove->widget();
            delete descriptionItemToRemove->widget();
        }
        delete propertyItemToRemove;
        delete descriptionItemToRemove;
    }
}

void Frame::changeComboBoxIndex(int index)
{
    QFile file(":/json/default_objects.json");
    auto objectName = _comboBox->itemText(index);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    auto jsonData = file.readAll();
    file.close();

    auto jsonDocument = QJsonDocument::fromJson(jsonData);
    if (jsonDocument.isNull()) return;

    auto rootJsonObject = jsonDocument.object();
    auto propertyObject = rootJsonObject[objectName].toObject()["Property"].toObject();

    reInitializeFromJson(propertyObject);
}

void Frame::setupUi()
{
    //Frame
    this->setFrameShape(QFrame::WinPanel);
    this->setFrameShadow(QFrame::Plain);
    _mainVLayout = new QVBoxLayout(this);
    _mainHLayout = new QHBoxLayout(this);
    _comboBox = new QComboBox(this);

    //ComboBox
    QSizePolicy comboBoxSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    comboBoxSizePolicy.setHorizontalStretch(0);
    comboBoxSizePolicy.setVerticalStretch(0);
    comboBoxSizePolicy.setHeightForWidth(_comboBox->sizePolicy().hasHeightForWidth());
    _comboBox->setSizePolicy(comboBoxSizePolicy);
    _comboBox->setCurrentIndex(-1);
    _comboBox->setEditable(true);
    _mainHLayout->addWidget(_comboBox);

    //PropertyLayout
    _propertyVLayout = new QVBoxLayout(this);
    _propertyVLayout->setSizeConstraint(QLayout::SetMinimumSize);
    auto propertyLineEdit  = new QLineEdit(this);
    QSizePolicy lineEditSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    lineEditSizePolicy.setHorizontalStretch(0);
    lineEditSizePolicy.setVerticalStretch(0);
    lineEditSizePolicy.setHeightForWidth(propertyLineEdit ->sizePolicy().hasHeightForWidth());
    propertyLineEdit ->setSizePolicy(lineEditSizePolicy);
    _propertyVLayout->addWidget(propertyLineEdit);

    //ButtonsLayout
    _buttonsHLayout = new QHBoxLayout(this);
    _buttonAddProperty = new QPushButton(this);
    _buttonAddProperty->setText("+");
    _buttonRemoveProperty = new QPushButton(this);
    _buttonRemoveProperty->setText("-");

    _buttonsHLayout->addWidget(_buttonAddProperty);
    _buttonsHLayout->addWidget(_buttonRemoveProperty);
    _propertyVLayout->addLayout(_buttonsHLayout);

    _mainHLayout->addLayout(_propertyVLayout);

    //DescriptionLayout
    _descriptionVLayout = new QVBoxLayout();
    auto descriptionLineEdit = new QLineEdit();
    _descriptionVLayout->addWidget(descriptionLineEdit);
    _descriptionHLayout = new QHBoxLayout();
    _descriptionVLayout->addLayout(_descriptionHLayout);

    _mainHLayout->addLayout(_descriptionVLayout);


    //DeleteButtonLayout
    _deleteButtonHLayout = new QHBoxLayout();
    _deleteButtonHSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _buttonDeleteFrame = new QPushButton(this);
    _buttonDeleteFrame->setText("-");

    _deleteButtonHLayout->addItem(_deleteButtonHSpacer);
    _deleteButtonHLayout->addWidget(_buttonDeleteFrame);

    //AddLayouts
    _mainVLayout->addLayout(_mainHLayout);
    _mainVLayout->addLayout(_deleteButtonHLayout);

    addDefaultItemsToComboBox();
}

void Frame::connectSlots()
{
    connect(_buttonAddProperty, &QPushButton::pressed, this, &Frame::addProperty);
    connect(_buttonRemoveProperty, &QPushButton::pressed, this, &Frame::removeProperty);
    connect(_buttonDeleteFrame, &QPushButton::pressed, this, &Frame::forwardDeleteButtonPressed);
    connect(_comboBox, &QComboBox::activated, this, &Frame::changeComboBoxIndex);
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
    int widgetsCount = _propertyVLayout->count();
    int index = widgetsCount > 1 ?  widgetsCount - 1 : 0;
    _propertyVLayout->insertWidget(index, propertyLineEdit);
    _descriptionVLayout->insertWidget(index, descriptionLineEdit);
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

void Frame::addDefaultItemsToComboBox()
{
    QFile file(":/json/default_objects.json");
    if (!file.open(QIODevice::ReadOnly)) return;
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(jsonData);

    if(document.isNull()) return;

    QJsonObject rootObject = document.object();

    int index = 0;
    for (auto it = rootObject.begin(); it != rootObject.end(); it++)
    {
        _comboBox->addItem(QString());
    }
    for (auto it = rootObject.begin(); it != rootObject.end(); it++, index++)
    {
        _comboBox->setItemText(index, it.key());
    }

    _comboBox->setCurrentIndex(-1);
}
