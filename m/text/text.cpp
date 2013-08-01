/*****************************************************************************/
/*  sample.cpp - plugin example for LibreCAD                                 */
/*                                                                           */
/*  Copyright (C) 2011 Rallaz, rallazz@gmail.com                             */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 2 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSettings>
#include <QMessageBox>
#include <QDoubleValidator>
#include <QGroupBox>
#include "document_interface.h"
#include "sample.h"

QString LC_Sample::name() const
 {
     return (tr("Text plugin"));
 }

PluginCapabilities LC_Sample::getCapabilities() const
{
    PluginCapabilities pluginCapabilities;
    pluginCapabilities.menuEntryPoints
            << PluginMenuLocation("Help", tr("Text Plugin"));
    return pluginCapabilities;
}

void LC_Sample::execComm(Document_Interface *doc,
                             QWidget *parent, QString cmd)
{
    Q_UNUSED(doc);
    Q_UNUSED(cmd);
    lc_Sampledlg pdt(parent);
    int result =  pdt.exec();
    if (result == QDialog::Accepted)
        pdt.procesAction(doc);
}




/*****************************/
lc_Sampledlg::lc_Sampledlg(QWidget *parent) :  QDialog(parent)
{
    setWindowTitle(tr("Text Display"));
    QLabel *label;

    QDoubleValidator *val = new QDoubleValidator(0);
    QGridLayout *mainLayout = new QGridLayout;

   /* QLabel *alignmentLabel = new QLabel(tr("Verical Alignment"));
    QComboBox *alignmentComboBox = new QComboBox;
    alignmentComboBox->addItem(tr("Left"));
    alignmentComboBox->addItem(tr("Centered"));
    alignmentComboBox->addItem(tr("Right"));
*/
  /*  mainLayout->addWidget(alignmentLabel, 0, 1);
    mainLayout>addWidget(alignmentComboBox, 0, 1);

    mainLayout-> setLayout(alignmentLayout);
  */
    label= new QLabel("Enter text");
    mainLayout->addWidget(label, 0, 0);
    textedit = new QLineEdit();
    //textedit->setValidator(val);
    mainLayout->addWidget(textedit, 1, 0);

    label = new QLabel(tr("Enter X:"));
    mainLayout->addWidget(label, 2, 0);
    startxedit = new QLineEdit();
    startxedit->setValidator(val);
    mainLayout->addWidget(startxedit, 3, 0);

    label = new QLabel(tr("Start Y:"));
    mainLayout->addWidget(label, 2, 1);
    startyedit = new QLineEdit();
    startyedit->setValidator(val);
    mainLayout->addWidget(startyedit, 3, 1);

    label = new QLabel(tr("Enter height"));
    mainLayout->addWidget(label, 4, 0);
    heightedit = new QLineEdit();
    heightedit->setValidator(val);
    mainLayout->addWidget(heightedit, 5, 0);

    label = new QLabel(tr("Enter angle"));
    mainLayout->addWidget(label, 4, 1);
    angleedit = new QLineEdit();
    angleedit->setValidator(val);
    mainLayout->addWidget(angleedit, 5, 1);



    QHBoxLayout *loaccept = new QHBoxLayout;
    QPushButton *acceptbut = new QPushButton(tr("Accept"));
    loaccept->addStretch();
    loaccept->addWidget(acceptbut);
    mainLayout->addLayout(loaccept, 6, 0);

    QPushButton *cancelbut = new QPushButton(tr("Cancel"));
    QHBoxLayout *locancel = new QHBoxLayout;
    locancel->addWidget(cancelbut);
    locancel->addStretch();
    mainLayout->addLayout(locancel, 6, 1);

    setLayout(mainLayout);
    readSettings();

    connect(cancelbut, SIGNAL(clicked()), this, SLOT(reject()));
    connect(acceptbut, SIGNAL(clicked()), this, SLOT(checkAccept()));
   // connect(alignmentComboBox, SIGNAL(activated(int)),this, SLOT(alignmentChanged(int)));
}


bool lc_Sampledlg::failGUI(QString *msg)
{
    if (startxedit->text().isEmpty()) {msg->insert(0, tr("Start X is empty")); return true;}
    if (startyedit->text().isEmpty()) {msg->insert(0, tr("Start Y is empty")); return true;}
    if (heightedit->text().isEmpty()) {msg->insert(0, tr("Height field is empty")); return true;}
    if (angleedit->text().isEmpty()) {msg->insert(0, tr("Angle field is empty")); return true;}
    if (textedit->text().isEmpty()) {msg->insert(0, tr("Text field is empty")); return true;}
    return false;
}


void lc_Sampledlg::procesAction(Document_Interface *doc)
{
    //Q_UNUSED(doc);
    QPointF start;
    QString txt;
    QString sty;
    double height;
    double angle;
    DPI::HAlign ha=DPI::HAlignLeft;
    DPI::VAlign va=DPI::VAlignTop;
     start.setX(startxedit->text().toDouble());
     start.setY(startyedit->text().toDouble());
     height= heightedit->text().toDouble();
     angle= angleedit->text().toDouble();
     txt.append(textedit->text());
    //int total = startyedit + heightedit;
    // end.setX(endxedit->text().toDouble());
    //end.setY(endyedit->text().toDouble());

    doc->addText(txt,sty,&start,height,angle,ha, va);
}

void lc_Sampledlg::checkAccept()
{

    errmsg.clear();
    if (failGUI(&errmsg)) {
        QMessageBox::critical ( this, "Error Display", errmsg );
        errmsg.clear();
        return;
    }
    accept();
}


lc_Sampledlg::~lc_Sampledlg()
{
}
void lc_Sampledlg::closeEvent(QCloseEvent *event)
 {
    writeSettings();
    QWidget::closeEvent(event);
 }


void lc_Sampledlg::readSettings()
 {
    QString str;
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "sample_plugin");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(430,140)).toSize();

    startxedit->setText( settings.value("startx", 0.0).toString() );
    startyedit->setText( settings.value("starty", 0.0).toString() );
    heightedit->setText(settings.value("height",0.0).toString());
    angleedit->setText( settings.value("angle", 0.0).toString() );
    textedit->setText( settings.value("Text").toString() );

    resize(size);
    move(pos);
 }
/*void lc_Sampledlg::actualcoordinate()
{

}*/

void lc_Sampledlg::writeSettings()
 {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "sample_plugin");
    settings.setValue("pos", pos());
    settings.setValue("size", size());

    settings.setValue("Startx",startxedit->text());
    settings.setValue("starty", startyedit->text());
    settings.setValue("height",heightedit->text());
    settings.setValue("angle", angleedit->text());
    settings.setValue("Text", textedit->text());

}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(lc_sample, LC_Sample);
#endif

