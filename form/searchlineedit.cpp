#include "searchlineedit.h"

#include <QHBoxLayout>
#include <QLabel>

SearchLineEdit::SearchLineEdit(QWidget *parent)
	:QLineEdit(parent)
{
	QLabel *label = new QLabel;
	label->setObjectName("searchImgLabel");
	label->setMinimumSize(16, 16);
	label->setMaximumSize(16, 16);
	label->setStyleSheet("QLabel#searchImgLabel{"
						 "	border-image: url(:/Icon/search.png);"
						 "}");

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(label);
	mainLayout->addStretch();
	mainLayout->setContentsMargins(8, 0, 0, 0);

	setTextMargins(16 + 8 + 2, 0, 0, 0);
	setContentsMargins(0, 0, 0, 0);
	setLayout(mainLayout);
	setStyleSheet("SearchLineEdit{"
				  "	border-radius: 5px;"
				  "	font-size: 13px;"
				  "	color: black;"
				  "	background-color: rgb(255, 255, 255);"
				  "	border: 1px solid #D1D1D1;"
				  "}"
				  "SearchLineEdit:hover{"
				  "	border: 1px solid #1583DD;"
				  "}");
}
