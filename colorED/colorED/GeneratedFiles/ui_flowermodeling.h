/********************************************************************************
** Form generated from reading UI file 'flowermodeling.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FLOWERMODELING_H
#define UI_FLOWERMODELING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_flowermodelingClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab_ED;
    QGridLayout *gridLayout_2;
    QPushButton *Button_scan;
    QPushButton *Button_center;
    QPushButton *Button_paint;
    QSlider *Slider_brush;
    QLineEdit *Edit_brush;
    QLabel *label;
    QPushButton *Button_done;
    QLineEdit *Edit_image;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_3;
    QGraphicsView *graphics_image;
    QWidget *tab_confitting;
    QGridLayout *gridLayout_4;
    QPushButton *Button_surface;
    QPushButton *Button_template;
    QScrollArea *scrollArea_cone;
    QWidget *scrollAreaWidgetContents_2;
    QWidget *tab_mesh;
    QGridLayout *gridLayout_5;
    QPushButton *Button_meshfitting;
    QScrollArea *scrollArea_mesh;
    QWidget *scrollAreaWidgetContents_3;
    QPushButton *Button_mesh;
    QLabel *label_state;
    QPushButton *Button_Occ;
    QPushButton *Button_Texture;
    QWidget *tab_multi;
    QGridLayout *gridLayout_6;
    QLineEdit *Edit_mesh1;
    QPushButton *Button_mesh1;
    QLineEdit *Edit_mesh2;
    QPushButton *Button_mesh2;
    QPushButton *Button_mult;
    QScrollArea *scrollArea_mult;
    QWidget *scrollAreaWidgetContents_4;
    QLabel *label_mult;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *flowermodelingClass)
    {
        if (flowermodelingClass->objectName().isEmpty())
            flowermodelingClass->setObjectName(QStringLiteral("flowermodelingClass"));
        flowermodelingClass->resize(665, 679);
        centralWidget = new QWidget(flowermodelingClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab_ED = new QWidget();
        tab_ED->setObjectName(QStringLiteral("tab_ED"));
        gridLayout_2 = new QGridLayout(tab_ED);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        Button_scan = new QPushButton(tab_ED);
        Button_scan->setObjectName(QStringLiteral("Button_scan"));

        gridLayout_2->addWidget(Button_scan, 1, 3, 1, 1);

        Button_center = new QPushButton(tab_ED);
        Button_center->setObjectName(QStringLiteral("Button_center"));

        gridLayout_2->addWidget(Button_center, 1, 4, 1, 1);

        Button_paint = new QPushButton(tab_ED);
        Button_paint->setObjectName(QStringLiteral("Button_paint"));

        gridLayout_2->addWidget(Button_paint, 2, 3, 1, 1);

        Slider_brush = new QSlider(tab_ED);
        Slider_brush->setObjectName(QStringLiteral("Slider_brush"));
        Slider_brush->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(Slider_brush, 2, 1, 1, 1);

        Edit_brush = new QLineEdit(tab_ED);
        Edit_brush->setObjectName(QStringLiteral("Edit_brush"));
        Edit_brush->setReadOnly(true);

        gridLayout_2->addWidget(Edit_brush, 2, 2, 1, 1);

        label = new QLabel(tab_ED);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_2->addWidget(label, 2, 0, 1, 1);

        Button_done = new QPushButton(tab_ED);
        Button_done->setObjectName(QStringLiteral("Button_done"));

        gridLayout_2->addWidget(Button_done, 2, 4, 1, 1);

        Edit_image = new QLineEdit(tab_ED);
        Edit_image->setObjectName(QStringLiteral("Edit_image"));
        Edit_image->setReadOnly(true);

        gridLayout_2->addWidget(Edit_image, 1, 0, 1, 3);

        scrollArea = new QScrollArea(tab_ED);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 621, 501));
        gridLayout_3 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        graphics_image = new QGraphicsView(scrollAreaWidgetContents);
        graphics_image->setObjectName(QStringLiteral("graphics_image"));

        gridLayout_3->addWidget(graphics_image, 0, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout_2->addWidget(scrollArea, 0, 0, 1, 5);

        tabWidget->addTab(tab_ED, QString());
        tab_confitting = new QWidget();
        tab_confitting->setObjectName(QStringLiteral("tab_confitting"));
        gridLayout_4 = new QGridLayout(tab_confitting);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        Button_surface = new QPushButton(tab_confitting);
        Button_surface->setObjectName(QStringLiteral("Button_surface"));

        gridLayout_4->addWidget(Button_surface, 1, 1, 1, 1);

        Button_template = new QPushButton(tab_confitting);
        Button_template->setObjectName(QStringLiteral("Button_template"));

        gridLayout_4->addWidget(Button_template, 1, 0, 1, 1);

        scrollArea_cone = new QScrollArea(tab_confitting);
        scrollArea_cone->setObjectName(QStringLiteral("scrollArea_cone"));
        scrollArea_cone->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 98, 28));
        scrollArea_cone->setWidget(scrollAreaWidgetContents_2);

        gridLayout_4->addWidget(scrollArea_cone, 0, 0, 1, 2);

        tabWidget->addTab(tab_confitting, QString());
        tab_mesh = new QWidget();
        tab_mesh->setObjectName(QStringLiteral("tab_mesh"));
        gridLayout_5 = new QGridLayout(tab_mesh);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        Button_meshfitting = new QPushButton(tab_mesh);
        Button_meshfitting->setObjectName(QStringLiteral("Button_meshfitting"));

        gridLayout_5->addWidget(Button_meshfitting, 2, 0, 1, 1);

        scrollArea_mesh = new QScrollArea(tab_mesh);
        scrollArea_mesh->setObjectName(QStringLiteral("scrollArea_mesh"));
        scrollArea_mesh->setWidgetResizable(true);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName(QStringLiteral("scrollAreaWidgetContents_3"));
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 621, 472));
        scrollArea_mesh->setWidget(scrollAreaWidgetContents_3);

        gridLayout_5->addWidget(scrollArea_mesh, 0, 0, 1, 2);

        Button_mesh = new QPushButton(tab_mesh);
        Button_mesh->setObjectName(QStringLiteral("Button_mesh"));

        gridLayout_5->addWidget(Button_mesh, 1, 0, 1, 1);

        label_state = new QLabel(tab_mesh);
        label_state->setObjectName(QStringLiteral("label_state"));

        gridLayout_5->addWidget(label_state, 1, 1, 2, 1);

        Button_Occ = new QPushButton(tab_mesh);
        Button_Occ->setObjectName(QStringLiteral("Button_Occ"));

        gridLayout_5->addWidget(Button_Occ, 3, 0, 1, 1);

        Button_Texture = new QPushButton(tab_mesh);
        Button_Texture->setObjectName(QStringLiteral("Button_Texture"));

        gridLayout_5->addWidget(Button_Texture, 3, 1, 1, 1);

        tabWidget->addTab(tab_mesh, QString());
        tab_multi = new QWidget();
        tab_multi->setObjectName(QStringLiteral("tab_multi"));
        gridLayout_6 = new QGridLayout(tab_multi);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        Edit_mesh1 = new QLineEdit(tab_multi);
        Edit_mesh1->setObjectName(QStringLiteral("Edit_mesh1"));

        gridLayout_6->addWidget(Edit_mesh1, 1, 0, 1, 1);

        Button_mesh1 = new QPushButton(tab_multi);
        Button_mesh1->setObjectName(QStringLiteral("Button_mesh1"));

        gridLayout_6->addWidget(Button_mesh1, 1, 1, 1, 1);

        Edit_mesh2 = new QLineEdit(tab_multi);
        Edit_mesh2->setObjectName(QStringLiteral("Edit_mesh2"));

        gridLayout_6->addWidget(Edit_mesh2, 2, 0, 1, 1);

        Button_mesh2 = new QPushButton(tab_multi);
        Button_mesh2->setObjectName(QStringLiteral("Button_mesh2"));

        gridLayout_6->addWidget(Button_mesh2, 2, 1, 1, 1);

        Button_mult = new QPushButton(tab_multi);
        Button_mult->setObjectName(QStringLiteral("Button_mult"));

        gridLayout_6->addWidget(Button_mult, 3, 1, 1, 1);

        scrollArea_mult = new QScrollArea(tab_multi);
        scrollArea_mult->setObjectName(QStringLiteral("scrollArea_mult"));
        scrollArea_mult->setWidgetResizable(true);
        scrollAreaWidgetContents_4 = new QWidget();
        scrollAreaWidgetContents_4->setObjectName(QStringLiteral("scrollAreaWidgetContents_4"));
        scrollAreaWidgetContents_4->setGeometry(QRect(0, 0, 98, 28));
        scrollArea_mult->setWidget(scrollAreaWidgetContents_4);

        gridLayout_6->addWidget(scrollArea_mult, 0, 0, 1, 2);

        label_mult = new QLabel(tab_multi);
        label_mult->setObjectName(QStringLiteral("label_mult"));

        gridLayout_6->addWidget(label_mult, 3, 0, 1, 1);

        tabWidget->addTab(tab_multi, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        flowermodelingClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(flowermodelingClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 665, 23));
        flowermodelingClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(flowermodelingClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        flowermodelingClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(flowermodelingClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        flowermodelingClass->setStatusBar(statusBar);

        retranslateUi(flowermodelingClass);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(flowermodelingClass);
    } // setupUi

    void retranslateUi(QMainWindow *flowermodelingClass)
    {
        flowermodelingClass->setWindowTitle(QApplication::translate("flowermodelingClass", "flowermodeling", 0));
        Button_scan->setText(QApplication::translate("flowermodelingClass", "\351\200\211\346\213\251\345\233\276\347\211\207", 0));
        Button_center->setText(QApplication::translate("flowermodelingClass", "\351\200\211\345\217\226\350\212\261\345\277\203", 0));
        Button_paint->setText(QApplication::translate("flowermodelingClass", "\351\200\211\346\213\251\345\214\272\345\237\237", 0));
        label->setText(QApplication::translate("flowermodelingClass", "\347\254\224\345\210\267\345\260\272\345\257\270\357\274\232", 0));
        Button_done->setText(QApplication::translate("flowermodelingClass", "\351\200\211\346\213\251\345\256\214\346\210\220", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_ED), QApplication::translate("flowermodelingClass", "edge detection", 0));
        Button_surface->setText(QApplication::translate("flowermodelingClass", "\345\234\206\351\224\245\345\217\230\345\275\242", 0));
        Button_template->setText(QApplication::translate("flowermodelingClass", "\350\216\267\345\217\226\350\212\261\347\223\243\346\250\241\346\235\277", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_confitting), QApplication::translate("flowermodelingClass", "cone fitting", 0));
        Button_meshfitting->setText(QApplication::translate("flowermodelingClass", "\347\275\221\346\240\274\345\217\230\345\275\242", 0));
        Button_mesh->setText(QApplication::translate("flowermodelingClass", "\347\224\237\346\210\220\347\275\221\346\240\274", 0));
        label_state->setText(QString());
        Button_Occ->setText(QApplication::translate("flowermodelingClass", "\351\201\256\346\214\241\345\217\230\345\275\242", 0));
        Button_Texture->setText(QApplication::translate("flowermodelingClass", "\347\272\271\347\220\206\350\264\264\345\233\276", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_mesh), QApplication::translate("flowermodelingClass", "mesh fitting", 0));
        Button_mesh1->setText(QApplication::translate("flowermodelingClass", "\351\200\211\346\213\251\347\275\221\346\240\2741", 0));
        Button_mesh2->setText(QApplication::translate("flowermodelingClass", "\351\200\211\346\213\251\347\275\221\346\240\2742", 0));
        Button_mult->setText(QApplication::translate("flowermodelingClass", "\345\220\210\345\271\266", 0));
        label_mult->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_multi), QApplication::translate("flowermodelingClass", "multi-layer", 0));
    } // retranslateUi

};

namespace Ui {
    class flowermodelingClass: public Ui_flowermodelingClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FLOWERMODELING_H
