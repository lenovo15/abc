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
#include<QColor>

#include "document_interface.h"
#include "sample.h"

QString LC_Sample::name() const
 {
     return (tr("views"));
 }

PluginCapabilities LC_Sample::getCapabilities() const
{
    PluginCapabilities pluginCapabilities;
    pluginCapabilities.menuEntryPoints
            << PluginMenuLocation("Help", tr("views"));
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
    setWindowTitle(tr("Draw line"));
    QLabel *label;

    QDoubleValidator *val = new QDoubleValidator(0);
    QGridLayout *mainLayout = new QGridLayout;

         label = new QLabel(tr("Enter Start x Coordinate:"));
         mainLayout->addWidget(label, 1,1);
         startxedit = new QLineEdit();
         startxedit->setValidator(val);
         mainLayout->addWidget(startxedit, 1,2);

         label = new QLabel(tr("Enter Start y Coordinate:"));
         mainLayout->addWidget(label, 1,3);
         startyedit = new QLineEdit();
         startyedit->setValidator(val);
         mainLayout->addWidget(startyedit, 1,4);


         label = new QLabel(tr("Length:"));
         mainLayout->addWidget(label, 2,1);
         lengthedit = new QLineEdit();
         lengthedit->setValidator(val);
         mainLayout->addWidget(lengthedit, 2, 2);

        label = new QLabel(tr("Height:"));
        mainLayout->addWidget(label, 2,3);
        heightedit = new QLineEdit();
        heightedit->setValidator(val);
        mainLayout->addWidget(heightedit, 2,4);

        label = new QLabel(tr("Width:"));
        mainLayout->addWidget(label, 3,1);
        widthedit = new QLineEdit();
        widthedit->setValidator(val);
        mainLayout->addWidget(widthedit, 3,2);


        label = new QLabel(tr("Cylinder Height:"));
        mainLayout->addWidget(label, 3, 3);
        cycheight = new QLineEdit();
        cycheight->setValidator(val);
        mainLayout->addWidget(cycheight,3,4);

        label = new QLabel(tr("Cylinder Radius:"));
        mainLayout->addWidget(label, 4,1);
        cycradius = new QLineEdit();
        cycradius->setValidator(val);
        mainLayout->addWidget(cycradius, 4,2);

        label = new QLabel(tr("Spacing between views:"));
        mainLayout->addWidget(label, 4,3);
        spaceedit = new QLineEdit();
        spaceedit->setValidator(val);
        mainLayout->addWidget(spaceedit, 4, 4);


        label = new QLabel(tr("Draw topview:"));
        mainLayout->addWidget(label, 5,1);

        label = new QLabel(tr("Layer:"));
        mainLayout->addWidget(label, 6,1);
        toplayeredit = new QLineEdit();
        toplayeredit->setValidator(val);
        mainLayout->addWidget(toplayeredit, 6,2);


        label = new QLabel(tr("Draw frontview:"));
        mainLayout->addWidget(label, 7,1);

        label = new QLabel(tr("Layer:"));
        mainLayout->addWidget(label, 8,1);
        frontlayeredit = new QLineEdit();
        frontlayeredit->setValidator(val);
        mainLayout->addWidget(frontlayeredit, 8,2);



        label = new QLabel(tr("Draw sideview:"));
        mainLayout->addWidget(label, 9,1);

        label = new QLabel(tr("Layer:"));
        mainLayout->addWidget(label, 10,1);
        sidelayeredit = new QLineEdit();
        sidelayeredit->setValidator(val);
        mainLayout->addWidget(sidelayeredit, 10,2);





        QHBoxLayout *loaccept = new QHBoxLayout;
        QPushButton *acceptbut = new QPushButton(tr("Accept"));
        loaccept->addStretch();
        loaccept->addWidget(acceptbut);
        mainLayout->addLayout(loaccept, 11,1);
        QPushButton *cancelbut = new QPushButton(tr("Cancel"));
        QHBoxLayout *locancel = new QHBoxLayout;
        locancel->addWidget(cancelbut);
        locancel->addStretch();
        mainLayout->addLayout(locancel, 11,2);

        setLayout(mainLayout);
        readSettings();

        connect(cancelbut, SIGNAL(clicked()), this, SLOT(reject()));
        connect(acceptbut, SIGNAL(clicked()), this, SLOT(checkAccept()));
    }


    bool lc_Sampledlg::failGUI(QString *msg)
    {
        if (startxedit->text().isEmpty()) {msg->insert(0, tr("x coordinate field is empty")); return true;}
        if (startyedit->text().isEmpty()) {msg->insert(0, tr("y coordinate is empty")); return true;}
        if (lengthedit->text().isEmpty()) {msg->insert(0, tr("Length is empty")); return true;}
        if (widthedit->text().isEmpty()) {msg->insert(0, tr("Width is empty")); return true;}
        if (heightedit->text().isEmpty()) {msg->insert(0, tr("Height is empty")); return true;}
        if (cycheight->text().isEmpty()) {msg->insert(0, tr("Cylinder Height is empty")); return true;}
        if (cycradius->text().isEmpty()) {msg->insert(0, tr("Cylinder Radius is empty")); return true;}
        if (spaceedit->text().isEmpty()) {msg->insert(0, tr("Space field is empty")); return true;}

       // if (toplayeredit->text().isEmpty()) {msg->insert(0, tr("top layer is empty")); return true;}
      //  if (frontlayeredit->text().isEmpty()) {msg->insert(0, tr("front layer is empty")); return true;}
      //  if (sidelayeredit->text().isEmpty()) {msg->insert(0, tr("side layer is empty")); return true;}


        return false;
    }

    void lc_Sampledlg::procesAction(Document_Interface *doc)
    {
        Q_UNUSED(doc);
       double leng, breadth,height,r,CH,x; // r=radius, CH=Cylinder Height, x=spacing

        leng= lengthedit->text().toDouble();
         breadth= widthedit->text().toDouble();
         height= heightedit->text().toDouble();
         r= cycradius->text().toDouble();
         CH= cycheight->text().toDouble();
         x=spaceedit->text().toDouble();


         doc->setLayer("1st");
         doc->setCurrentLayerProperties(QColor ( 255, 0,0,255), DPI::Width14, DPI::SolidLine);

         topview(leng,breadth,r,doc);

         doc->setLayer("2nd");
         doc->setCurrentLayerProperties(QColor ( 0,225,0,255), DPI::Width14, DPI::SolidLine);
         frontview(height,leng, breadth, x,CH,r,doc);

         doc->setLayer("3rd");
         doc->setCurrentLayerProperties(QColor ( 0,0,225,255), DPI::Width14, DPI::SolidLine);
         sideview( x,breadth, leng, height, r, CH,doc);
    }


void lc_Sampledlg::checkAccept()


{

    errmsg.clear();
    if (failGUI(&errmsg)) {
        QMessageBox::critical ( this, "views", errmsg );
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
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "views");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(430,140)).toSize();

        startxedit->setText( settings.value("x coordinate", 0).toString() );
        startyedit->setText( settings.value("y coordinate", 0).toString() );
        lengthedit->setText( settings.value("Length", 0.0).toString() );
        widthedit->setText( settings.value(" Width", 0.0).toString() );
        heightedit->setText( settings.value("Height", 0.0).toString() );
        cycheight->setText( settings.value("Cylinder Height", 0.0).toString() );
        cycradius->setText( settings.value("Cylinder Radius", 0.0).toString() );
        spaceedit->setText( settings.value("Spacing between Views", 0.0).toString() );

      //  toplayeredit->setText( settings.value("layertop", 0.0).toString() );
      //  frontlayeredit->setText( settings.value("layerfront", 0.0).toString() );
      //  sidelayeredit->setText( settings.value("layerside", 0.0).toString() );


        resize(size);
    move(pos);








 }

void lc_Sampledlg::writeSettings()
 {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "views");
    settings.setValue("pos", pos());
    settings.setValue("size", size());

        settings.setValue("Start x coordinate",  startxedit->text());
        settings.setValue("Start y coordinate",  startyedit->text());
        settings.setValue("Length",  lengthedit->text());
        settings.setValue("Width",  widthedit->text());
        settings.setValue("Height",heightedit->text());
        settings.setValue("Cylinder Height", cycheight->text());
        settings.setValue("Cylinder Radius", cycradius->text());

      //  settings.setValue("layertop", toplayeredit->text());
       // settings.setValue("layerfront", frontlayeredit->text());
       // settings.setValue("layerside", sidelayeredit->text());


}

void lc_Sampledlg::topview(double leng, double breadth, double r,Document_Interface *doc)
{

   // doc->setLayer("t");
   // doc->setCurrentLayerProperties(QColor ( 255, 0,0,127), DPI::Width20, DPI::SolidLine);



    QPointF Coord_a,Coord_b(0.0,0.0+breadth),Coord_c(leng,breadth),Coord_d(0.0+leng,0);// Top View= L*B  L=30,B=20
    Coord_a.setX(startxedit->text().toDouble());
    Coord_a.setY(startyedit->text().toDouble());
    Coord_b +=Coord_a;
    Coord_c +=Coord_a;
    Coord_d +=Coord_a;
    QPointF c_center(leng/2,breadth/2);
    c_center += Coord_a;

    doc->addLine(&Coord_a, &Coord_b); // coord_a to Coord_d -> for TOP view
    doc->addLine(&Coord_b, &Coord_c);
    doc->addLine(&Coord_c, &Coord_d);
    Coord_a.setX(startxedit->text().toDouble());
    Coord_a.setY(startyedit->text().toDouble());
    Coord_b +=Coord_a;
    doc->addLine(&Coord_d, &Coord_a);
    doc-> addCircle(&c_center,r);

}

void lc_Sampledlg::frontview(double height, double leng,double breadth, double x, double CH,double r,Document_Interface *doc)
{
   // doc->setLayer("f");
    //doc->setCurrentLayerProperties(QColor ( 0, 225,0,127), DPI::Width16, DPI::SolidLine);


    QPointF Coord_a;
    Coord_a.setX(startxedit->text().toDouble());
     Coord_a.setY(startyedit->text().toDouble());

    QPointF Coord_e(0.0,x+breadth), Coord_f(0.0,height);//front view
    Coord_e += Coord_a, Coord_f+= Coord_e;
    QPointF Coord_g(leng,0.0), Coord_h(leng,0.0);
    Coord_g += Coord_f,Coord_h += Coord_e;
    QPointF mid1_l(leng/2-r,0.0), mid2_l(leng/2+r,0.0); //tank  front view
    mid1_l += Coord_f, mid2_l += Coord_f;

    QPointF ch1(0.0,CH), ch2(0.0,CH);
    ch1 += mid1_l, ch2 +=mid2_l;

    doc->addLine(&Coord_e, &Coord_f);//coord_e to coor_h for Front view
    doc->addLine(&Coord_f, &Coord_g );
    doc->addLine(&Coord_g, &Coord_h);
    doc->addLine(&Coord_h, & Coord_e);

    doc->addLine(&mid1_l,&ch1); // tank of front view
    doc->addLine(&mid2_l,&ch2);
    doc->addLine(&ch1,&ch2);
}

void lc_Sampledlg::sideview(double x,double breadth,double leng,double height,double r, double CH,Document_Interface *doc)

{

    //doc->setLayer("s");
    //doc->setCurrentLayerProperties(QColor ( 0, 0,225,127), DPI::Width18, DPI::SolidLine);


    QPointF Coord_a;
    Coord_a.setX(startxedit->text().toDouble());
     Coord_a.setY(startyedit->text().toDouble());
    QPointF Coord_i(0.0+x+leng,0.0+x+breadth), Coord_j(breadth,0.0); // side view
    Coord_i += Coord_a, Coord_j += Coord_i;
    QPointF Coord_k(0.0,height),Coord_l(breadth,0.0);
    Coord_k += Coord_i, Coord_l += Coord_k;

    QPointF mid3_b(breadth/2-r,0.0), mid4_b(breadth/2+r,0.0); //tank side view
    mid3_b += Coord_k, mid4_b += Coord_k;
    QPointF ch3(0.0,CH), ch4(0.0,CH);
    ch3 += mid3_b, ch4 +=mid4_b;

    doc->addLine(&Coord_l, &Coord_j); //Coord_i to Coord_l for side view
    doc->addLine(&Coord_i, &Coord_j);
    doc->addLine(&Coord_i, &Coord_k);
    doc->addLine(&Coord_k, &Coord_l);

    doc->addLine(&mid3_b, &ch3);// side view of tank
    doc->addLine(&mid4_b,&ch4);
    doc->addLine(&ch3, &ch4);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(lc_sample, LC_Sample);
#endif

