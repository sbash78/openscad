#include "parametercombobox.h"
#include "ignoreWheelWhenNotFocused.h"

ParameterComboBox::ParameterComboBox(QWidget *parent, ParameterObject *parameterobject, DescLoD descriptionLoD)
	: ParameterVirtualWidget(parent, parameterobject, descriptionLoD)
{
	setValue();
	connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onChanged(int)));

	IgnoreWheelWhenNotFocused *ignoreWheelWhenNotFocused = new IgnoreWheelWhenNotFocused(this);
	comboBox->installEventFilter(ignoreWheelWhenNotFocused);
}

void ParameterComboBox::onChanged(int idx)
{
	if(!this->suppressUpdate){
		if (object->dvt == Value::ValueType::STRING) {
			const std::string v = comboBox->itemData(idx).toString().toStdString();
			object->value = ValuePtr(v);
		} else {
			const double v = comboBox->itemData(idx).toDouble();
			object->value = ValuePtr(v);
		}
		object->focus = true;
		emit changed();
	}
}

void ParameterComboBox::setParameterFocus()
{
	this->comboBox->setFocus();
	object->focus = false;
}

void ParameterComboBox::setValue()
{
	this->suppressUpdate=true;
	this->stackedWidgetBelow->setCurrentWidget(this->pageComboBox);
	this->pageComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
	this->stackedWidgetRight->hide();
	comboBox->clear();
	const Value::VectorType& vec = object->values->toVector();
	for (const auto &textData : vec) {
		QString text, data;
		if (textData->toVector().size() > 1) {
			text = QString::fromStdString(textData->toVector()[1]->toString());
			data = QString::fromStdString(textData->toVector()[0]->toString());
		} else {
			text = QString::fromStdString(textData->toString());
			data = text;
			
		}
		comboBox->addItem(text, QVariant(data));
	}
	QString defaultData = QString::fromStdString(object->value->toString());
	int idx = comboBox->findData(QVariant(defaultData));
	if (idx >= 0) {
		comboBox->setCurrentIndex(idx);
	}
	this->suppressUpdate=false;
}
