/*****************************************************************************/
/*  arc.cpp - plugin example for LibreCAD                                 */
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
#include "arc.h"

QString LC_arc::name() const
 {
     return (tr("arc plugin"));
 }

PluginCapabilities LC_arc::getCapabilities() const
{
    PluginCapabilities pluginCapabilities;
    pluginCapabilities.menuEntryPoints
            << PluginMenuLocation("Help", tr("arc plugin"));
    return pluginCapabilities;
}

void LC_arc::execComm(Document_Interface *doc,
                             QWidget *parent, QString cmd)
{
    Q_UNUSED(doc);
    Q_UNUSED(cmd);
    lc_arcdlg pdt(parent);
    int result =  pdt.exec();
    if (result == QDialog::Accepted)
        pdt.procesAction(doc);
}




/*****************************/
lc_arcdlg::lc_arcdlg(QWidget *parent) :  QDialog(parent)
{
    setWindowTitle(tr("Draw arc"));
    QLabel *label;

    QDoubleValidator *val = new QDoubleValidator(0);
    QGridLayout *mainLayout = new QGridLayout;

    label = new QLabel(tr("Start X:"));
    mainLayout->addWidget(label, 1, 1);
    startxedit = new QLineEdit();
    startxedit->setValidator(val);
    mainLayout->addWidget(startxedit, 2, 1);

    label = new QLabel(tr("Start Y:"));
    mainLayout->addWidget(label, 3, 1);
    startyedit = new QLineEdit();
    startyedit->setValidator(val);
    mainLayout->addWidget(startyedit, 4, 1);

    label = new QLabel(tr("Enter radius:"));
    mainLayout->addWidget(label, 1, 4);
    radiusedit = new QLineEdit();
    radiusedit->setValidator(val);
    mainLayout->addWidget(radiusedit, 2, 4);
    
    label = new QLabel(tr("Enter a1x:"));
    mainLayout->addWidget(label, 1, 2);
    a1xedit = new QLineEdit();
    a1xedit->setValidator(val);
    mainLayout->addWidget(a1xedit, 2, 2);

    label = new QLabel(tr("Enter a1y:"));
    mainLayout->addWidget(label, 3, 2);
    a1yedit = new QLineEdit();
    a1yedit->setValidator(val);
    mainLayout->addWidget(a1yedit, 4, 2);

       label = new QLabel(tr("Enter a2x:"));
    mainLayout->addWidget(label, 1, 3);
    a2xedit = new QLineEdit();
    a2xedit->setValidator(val);
    mainLayout->addWidget(a2xedit, 2, 3);

     label = new QLabel(tr("Enter a2y:"));
    mainLayout->addWidget(label, 3, 3);
    a2yedit = new QLineEdit();
    a2yedit->setValidator(val);
    mainLayout->addWidget(a2yedit, 4, 3);

    /*label = new QLabel(tr("End X:"));
    mainLayout->addWidget(label, 2, 0);
    endxedit = new QLineEdit();
    endxedit->setValidator(val);
    mainLayout->addWidget(endxedit, 3, 0);

    label = new QLabel(tr("End Y:"));
    mainLayout->addWidget(label, 2, 1);
    endyedit = new QLineEdit();
    endyedit->setValidator(val);
    mainLayout->addWidget(endyedit, 3, 1);*/


    QHBoxLayout *loaccept = new QHBoxLayout;
    QPushButton *acceptbut = new QPushButton(tr("Accept"));
    loaccept->addStretch();
    loaccept->addWidget(acceptbut);
    mainLayout->addLayout(loaccept, 1, 5);

    QPushButton *cancelbut = new QPushButton(tr("Cancel"));
    QHBoxLayout *locancel = new QHBoxLayout;
    locancel->addWidget(cancelbut);
    locancel->addStretch();
    mainLayout->addLayout(locancel, 3, 5);

    setLayout(mainLayout);
    readSettings();

    connect(cancelbut, SIGNAL(clicked()), this, SLOT(reject()));
    connect(acceptbut, SIGNAL(clicked()), this, SLOT(checkAccept()));
}


bool lc_arcdlg::failGUI(QString *msg)
{
    if (startxedit->text().isEmpty()) {msg->insert(0, tr("Start X is empty")); return true;}
    if (startyedit->text().isEmpty()) {msg->insert(0, tr("Start Y is empty")); return true;}
  
    if (radiusedit->text().isEmpty()) {msg->insert(0, tr(" radius field is empty")); return true;}

    if (a1xedit->text().isEmpty()) {msg->insert(0, tr("a1x is empty")); return true;}
    if (a1yedit->text().isEmpty()) {msg->insert(0, tr("a1y is empty")); return true;}
    if (a2xedit->text().isEmpty()) {msg->insert(0, tr("a2x is empty")); return true;}
    if (a2yedit->text().isEmpty()) {msg->insert(0, tr("a2y is empty")); return true;}
  
  
  
   return false;
  
}







  



void lc_arcdlg::procesAction(Document_Interface *doc)
{
    Q_UNUSED(doc);
    QPointF start;
    qreal radius, a1, a2;
    
    start.setX(startxedit->text().toDouble());
    start.setY(startyedit->text().toDouble());
    radius=radiusedit->text().toDouble();
   a1=a1xedit->text().toDouble();
   a1=a1yedit->text().toDouble();
    a2=a2xedit->text().toDouble();
    a2=a2yedit->text().toDouble();
     

   /* end.setX(endxedit->text().toDouble());
    end.setY(endyedit->text().toDouble());*/

    doc->addArc(&start,radius,a1,a2);
}

void lc_arcdlg::checkAccept()
{

    errmsg.clear();
    if (failGUI(&errmsg)) {
        QMessageBox::critical ( this, "arc plugin", errmsg );
        errmsg.clear();
        return;
    }
    accept();
}


lc_arcdlg::~lc_arcdlg()
{
}
void lc_arcdlg::closeEvent(QCloseEvent *event)
 {
    writeSettings();
    QWidget::closeEvent(event);
 }


void lc_arcdlg::readSettings()
 {
    QString str;
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "arc_plugin");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(430,140)).toSize();

    startxedit->setText( settings.value("startx", 0.5).toString() );
    startyedit->setText( settings.value("starty", 0.5).toString() );
    radiusedit->setText( settings.value("radius", 0.5).toString() );

    a1xedit->setText( settings.value("a1x", 0.5).toString() );

  a1yedit->setText( settings.value("a1y", 0.5).toString() );


 a2xedit->setText( settings.value("a2x", 0.5).toString() );

  a2yedit->setText( settings.value("a2y", 0.5).toString() );
  

  
  
   /* endxedit->setText( settings.value("endx", 3.5).toString() );
    endyedit->setText( settings.value("endy", 3.5).toString() );*/

    resize(size);
    move(pos);
 }

void lc_arcdlg::writeSettings()
 {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "arc_plugin");
    settings.setValue("pos", pos());
    settings.setValue("size", size());

    settings.setValue("startx", startxedit->text());
    settings.setValue("starty", startyedit->text());
     settings.setValue("radius", radiusedit->text());
      settings.setValue("a1x", a1xedit->text());
        settings.setValue("a1y",a1yedit->text());
       settings.setValue("a2x", a2xedit->text());
      settings.setValue("a2y", a2yedit->text());


   
     
   /* settings.setValue("endx", endxedit->text());
    settings.setValue("endy", endyedit->text());*/
 }

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(lc_arc, LC_arc);
#endif
