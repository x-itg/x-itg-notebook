/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_6;
    QFrame *line_4;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_7;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_45;
    QLabel *label_5;
    QLabel *label_8;
    QHBoxLayout *horizontalLayout_5;
    QComboBox *PortBox;
    QComboBox *StopBox;
    QComboBox *ParityBox;
    QComboBox *BitNumBox;
    QComboBox *BaudBox;
    QHBoxLayout *horizontalLayout;
    QPushButton *openButton;
    QPushButton *sendButton;
    QPushButton *setButton;
    QComboBox *Box_ReadMode;
    QHBoxLayout *horizontalLayout_7;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_10;
    QTextEdit *textEdit;
    QLabel *label_44;
    QHBoxLayout *horizontalLayout_20;
    QHBoxLayout *horizontalLayout_22;
    QLabel *label_48;
    QProgressBar *progressBar;
    QHBoxLayout *horizontalLayout_27;
    QLineEdit *FilePath;
    QPushButton *ChooseFile;
    QPushButton *GetNewFile;
    QPushButton *escBoot;
    QComboBox *Box_updateMode;
    QHBoxLayout *horizontalLayout_23;
    QPushButton *readVersion;
    QLineEdit *Edit_Version;
    QComboBox *BoxMode;
    QPushButton *EnterBoot;
    QHBoxLayout *horizontalLayout_24;
    QPushButton *UpdateCode;
    QHBoxLayout *horizontalLayout_21;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_11;
    QComboBox *setMacMODE;
    QComboBox *setMacMODE_XY;
    QPushButton *readd0;
    QPushButton *clearButton;
    QHBoxLayout *horizontalLayout_19;
    QHBoxLayout *horizontalLayout_17;
    QTextEdit *p0Edit;
    QHBoxLayout *horizontalLayout_18;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(899, 989);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 489, 981));
        verticalLayout_6 = new QVBoxLayout(layoutWidget);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        line_4 = new QFrame(layoutWidget);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        verticalLayout_6->addWidget(line_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_6->addWidget(label_6);

        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_6->addWidget(label_7);

        label_45 = new QLabel(layoutWidget);
        label_45->setObjectName(QStringLiteral("label_45"));

        horizontalLayout_6->addWidget(label_45);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_6->addWidget(label_5);

        label_8 = new QLabel(layoutWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_6->addWidget(label_8);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        PortBox = new QComboBox(layoutWidget);
        PortBox->setObjectName(QStringLiteral("PortBox"));

        horizontalLayout_5->addWidget(PortBox);

        StopBox = new QComboBox(layoutWidget);
        StopBox->setObjectName(QStringLiteral("StopBox"));

        horizontalLayout_5->addWidget(StopBox);

        ParityBox = new QComboBox(layoutWidget);
        ParityBox->setObjectName(QStringLiteral("ParityBox"));

        horizontalLayout_5->addWidget(ParityBox);

        BitNumBox = new QComboBox(layoutWidget);
        BitNumBox->setObjectName(QStringLiteral("BitNumBox"));

        horizontalLayout_5->addWidget(BitNumBox);

        BaudBox = new QComboBox(layoutWidget);
        BaudBox->setObjectName(QStringLiteral("BaudBox"));

        horizontalLayout_5->addWidget(BaudBox);


        verticalLayout_2->addLayout(horizontalLayout_5);


        verticalLayout_7->addLayout(verticalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        openButton = new QPushButton(layoutWidget);
        openButton->setObjectName(QStringLiteral("openButton"));

        horizontalLayout->addWidget(openButton);

        sendButton = new QPushButton(layoutWidget);
        sendButton->setObjectName(QStringLiteral("sendButton"));

        horizontalLayout->addWidget(sendButton);

        setButton = new QPushButton(layoutWidget);
        setButton->setObjectName(QStringLiteral("setButton"));

        horizontalLayout->addWidget(setButton);

        Box_ReadMode = new QComboBox(layoutWidget);
        Box_ReadMode->setObjectName(QStringLiteral("Box_ReadMode"));

        horizontalLayout->addWidget(Box_ReadMode);


        verticalLayout_7->addLayout(horizontalLayout);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));

        verticalLayout_7->addLayout(horizontalLayout_7);


        verticalLayout_5->addLayout(verticalLayout_7);


        verticalLayout_6->addLayout(verticalLayout_5);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        textEdit = new QTextEdit(layoutWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        verticalLayout_10->addWidget(textEdit);

        label_44 = new QLabel(layoutWidget);
        label_44->setObjectName(QStringLiteral("label_44"));
        QFont font;
        font.setFamily(QStringLiteral("Agency FB"));
        font.setPointSize(12);
        label_44->setFont(font);

        verticalLayout_10->addWidget(label_44);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setSpacing(6);
        horizontalLayout_20->setObjectName(QStringLiteral("horizontalLayout_20"));

        verticalLayout_10->addLayout(horizontalLayout_20);

        horizontalLayout_22 = new QHBoxLayout();
        horizontalLayout_22->setSpacing(6);
        horizontalLayout_22->setObjectName(QStringLiteral("horizontalLayout_22"));
        label_48 = new QLabel(layoutWidget);
        label_48->setObjectName(QStringLiteral("label_48"));
        label_48->setFont(font);

        horizontalLayout_22->addWidget(label_48);

        progressBar = new QProgressBar(layoutWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(0);

        horizontalLayout_22->addWidget(progressBar);


        verticalLayout_10->addLayout(horizontalLayout_22);

        horizontalLayout_27 = new QHBoxLayout();
        horizontalLayout_27->setSpacing(6);
        horizontalLayout_27->setObjectName(QStringLiteral("horizontalLayout_27"));
        FilePath = new QLineEdit(layoutWidget);
        FilePath->setObjectName(QStringLiteral("FilePath"));
        FilePath->setFont(font);

        horizontalLayout_27->addWidget(FilePath);

        ChooseFile = new QPushButton(layoutWidget);
        ChooseFile->setObjectName(QStringLiteral("ChooseFile"));
        QFont font1;
        font1.setFamily(QStringLiteral("Agency FB"));
        font1.setPointSize(10);
        ChooseFile->setFont(font1);

        horizontalLayout_27->addWidget(ChooseFile);

        GetNewFile = new QPushButton(layoutWidget);
        GetNewFile->setObjectName(QStringLiteral("GetNewFile"));
        GetNewFile->setFont(font1);

        horizontalLayout_27->addWidget(GetNewFile);

        escBoot = new QPushButton(layoutWidget);
        escBoot->setObjectName(QStringLiteral("escBoot"));
        escBoot->setFont(font1);

        horizontalLayout_27->addWidget(escBoot);


        verticalLayout_10->addLayout(horizontalLayout_27);

        Box_updateMode = new QComboBox(layoutWidget);
        Box_updateMode->setObjectName(QStringLiteral("Box_updateMode"));
        Box_updateMode->setSizeIncrement(QSize(0, 0));
        Box_updateMode->setBaseSize(QSize(0, 0));
        Box_updateMode->setIconSize(QSize(32, 32));

        verticalLayout_10->addWidget(Box_updateMode);

        horizontalLayout_23 = new QHBoxLayout();
        horizontalLayout_23->setSpacing(6);
        horizontalLayout_23->setObjectName(QStringLiteral("horizontalLayout_23"));
        readVersion = new QPushButton(layoutWidget);
        readVersion->setObjectName(QStringLiteral("readVersion"));
        readVersion->setFont(font1);

        horizontalLayout_23->addWidget(readVersion);

        Edit_Version = new QLineEdit(layoutWidget);
        Edit_Version->setObjectName(QStringLiteral("Edit_Version"));
        Edit_Version->setFont(font);

        horizontalLayout_23->addWidget(Edit_Version);

        BoxMode = new QComboBox(layoutWidget);
        BoxMode->setObjectName(QStringLiteral("BoxMode"));

        horizontalLayout_23->addWidget(BoxMode);

        EnterBoot = new QPushButton(layoutWidget);
        EnterBoot->setObjectName(QStringLiteral("EnterBoot"));
        QFont font2;
        font2.setFamily(QStringLiteral("Agency FB"));
        font2.setPointSize(16);
        font2.setBold(false);
        font2.setUnderline(true);
        font2.setWeight(50);
        EnterBoot->setFont(font2);

        horizontalLayout_23->addWidget(EnterBoot);


        verticalLayout_10->addLayout(horizontalLayout_23);

        horizontalLayout_24 = new QHBoxLayout();
        horizontalLayout_24->setSpacing(6);
        horizontalLayout_24->setObjectName(QStringLiteral("horizontalLayout_24"));
        UpdateCode = new QPushButton(layoutWidget);
        UpdateCode->setObjectName(QStringLiteral("UpdateCode"));
        UpdateCode->setFont(font2);

        horizontalLayout_24->addWidget(UpdateCode);


        verticalLayout_10->addLayout(horizontalLayout_24);

        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setSpacing(6);
        horizontalLayout_21->setObjectName(QStringLiteral("horizontalLayout_21"));

        verticalLayout_10->addLayout(horizontalLayout_21);


        horizontalLayout_2->addLayout(verticalLayout_10);


        verticalLayout_6->addLayout(horizontalLayout_2);

        verticalLayoutWidget_2 = new QWidget(centralWidget);
        verticalLayoutWidget_2->setObjectName(QStringLiteral("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(500, 10, 381, 981));
        verticalLayout_11 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setContentsMargins(11, 11, 11, 11);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        verticalLayout_11->setContentsMargins(0, 0, 0, 0);
        setMacMODE = new QComboBox(verticalLayoutWidget_2);
        setMacMODE->setObjectName(QStringLiteral("setMacMODE"));

        verticalLayout_11->addWidget(setMacMODE);

        setMacMODE_XY = new QComboBox(verticalLayoutWidget_2);
        setMacMODE_XY->setObjectName(QStringLiteral("setMacMODE_XY"));

        verticalLayout_11->addWidget(setMacMODE_XY);

        readd0 = new QPushButton(verticalLayoutWidget_2);
        readd0->setObjectName(QStringLiteral("readd0"));

        verticalLayout_11->addWidget(readd0);

        clearButton = new QPushButton(verticalLayoutWidget_2);
        clearButton->setObjectName(QStringLiteral("clearButton"));

        verticalLayout_11->addWidget(clearButton);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        p0Edit = new QTextEdit(verticalLayoutWidget_2);
        p0Edit->setObjectName(QStringLiteral("p0Edit"));

        horizontalLayout_17->addWidget(p0Edit);


        horizontalLayout_19->addLayout(horizontalLayout_17);


        verticalLayout_11->addLayout(horizontalLayout_19);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setObjectName(QStringLiteral("horizontalLayout_18"));

        verticalLayout_11->addLayout(horizontalLayout_18);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        BaudBox->setCurrentIndex(0);
        Box_ReadMode->setCurrentIndex(0);
        Box_updateMode->setCurrentIndex(0);
        BoxMode->setCurrentIndex(0);
        setMacMODE->setCurrentIndex(0);
        setMacMODE_XY->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        label_6->setText(QApplication::translate("MainWindow", "\346\240\241\351\252\214\344\275\215\357\274\232", 0));
        label_7->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\344\275\215\357\274\232", 0));
        label_45->setText(QApplication::translate("MainWindow", "\344\270\262\345\217\243\351\200\211\346\213\251\357\274\232", 0));
        label_5->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\344\275\215\357\274\232", 0));
        label_8->setText(QApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207\357\274\232", 0));
        StopBox->clear();
        StopBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "1", 0)
         << QApplication::translate("MainWindow", "2", 0)
        );
        ParityBox->clear();
        ParityBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "0", 0)
        );
        BitNumBox->clear();
        BitNumBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "8", 0)
        );
        BaudBox->clear();
        BaudBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "1200", 0)
         << QApplication::translate("MainWindow", "2400", 0)
         << QApplication::translate("MainWindow", "4800", 0)
         << QApplication::translate("MainWindow", "9600", 0)
         << QApplication::translate("MainWindow", "19200", 0)
         << QApplication::translate("MainWindow", "38400", 0)
         << QApplication::translate("MainWindow", "115200", 0)
        );
        openButton->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\344\270\262\345\217\243", 0));
        sendButton->setText(QApplication::translate("MainWindow", "\350\257\273\345\217\226\346\225\260\346\215\256", 0));
        setButton->setText(QApplication::translate("MainWindow", "\350\256\276\347\275\256\345\217\202\346\225\260", 0));
        Box_ReadMode->clear();
        Box_ReadMode->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\346\211\213\345\212\250\350\257\273\345\217\226", 0)
         << QApplication::translate("MainWindow", "\350\207\252\345\212\250\350\257\273\345\217\226", 0)
        );
        label_44->setText(QApplication::translate("MainWindow", "Device interaction information:", 0));
        label_48->setText(QApplication::translate("MainWindow", "\345\215\207\347\272\247\350\277\233\345\272\246\n"
"Upgrade Progress:", 0));
        FilePath->setText(QApplication::translate("MainWindow", "D:/5G_VER_D2AS.bin", 0));
        ChooseFile->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\346\226\207\344\273\266\n"
"File Choose", 0));
        GetNewFile->setText(QApplication::translate("MainWindow", "\350\216\267\345\217\226\345\275\223\345\211\215\350\267\257\345\276\204\344\270\213\346\234\200\346\226\260bin\346\226\207\344\273\266\n"
"Get New Bin File", 0));
        escBoot->setText(QApplication::translate("MainWindow", "\350\277\233\345\205\245APP\n"
"ENTER APP", 0));
        Box_updateMode->clear();
        Box_updateMode->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\344\270\262\345\217\243\345\215\207\347\272\247", 0)
         << QApplication::translate("MainWindow", "\350\275\254\345\217\221CAN", 0)
        );
        readVersion->setText(QApplication::translate("MainWindow", "\350\257\273\345\217\226APP\347\211\210\346\234\254\345\217\267\n"
"Read APP Verion", 0));
        Edit_Version->setText(QString());
        BoxMode->clear();
        BoxMode->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Boot", 0)
         << QApplication::translate("MainWindow", "App", 0)
        );
        EnterBoot->setText(QApplication::translate("MainWindow", "\350\277\233\345\205\245\345\215\207\347\272\247\346\250\241\345\274\217\n"
" Enter Bootloader", 0));
        UpdateCode->setText(QApplication::translate("MainWindow", "\347\241\256\350\256\244\345\215\207\347\272\247\345\274\200\345\247\213\345\217\221\351\200\201\346\226\207\344\273\266\n"
"Confirm the upgrade Start Send data", 0));
        setMacMODE->clear();
        setMacMODE->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\350\207\252\345\212\250\346\250\241\345\274\217", 0)
         << QApplication::translate("MainWindow", "\346\211\213\345\212\250\343\200\220\345\274\200\343\200\221\351\243\216\346\211\207", 0)
         << QApplication::translate("MainWindow", "\346\211\213\345\212\250\343\200\220\345\205\263\343\200\221\351\243\216\346\211\207", 0)
        );
        setMacMODE_XY->clear();
        setMacMODE_XY->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\351\235\236\346\266\210\351\237\263", 0)
         << QApplication::translate("MainWindow", "\346\266\210\351\237\263", 0)
         << QString()
        );
        readd0->setText(QApplication::translate("MainWindow", "\350\257\273\345\217\226DATA[0]", 0));
        clearButton->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272\346\216\245\346\224\266", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
