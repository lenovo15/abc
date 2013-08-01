/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file gpl-2.0.txt included in the
** packaging of this file.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** This copyright notice MUST APPEAR in all copies of the script!
**
**********************************************************************/

#include "rs_actiondrawstar.h"

#include <QAction>
#include "rs_dialogfactory.h"
#include "rs_graphicview.h"
#include "rs_commandevent.h"
#include "rs_point.h"



RS_ActionDrawStar::RS_ActionDrawStar(RS_EntityContainer& container,
                                       RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Draw Points",
                           container, graphicView) {}


QAction* RS_ActionDrawStar::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
        // (tr("Points")
        QAction* action = new QAction(tr("&Points"),  NULL);
        action->setIcon(QIcon(":/extui/2.png"));
    //action->zetStatusTip(tr("Draw Points"));

    return action;
}


void RS_ActionDrawStar::trigger() {
    if (pt.valid){
        RS_Point* point = new RS_Point(container, RS_PointData(pt));
        container->addEntity(point);
        {int i,j,k,l;       //star
        for(i=0;i<10;i++)
       {

           pt.x=i+pt.x;
           pt.y=i+pt.y;

        RS_Point* point1 = new RS_Point(container, RS_PointData(pt));
        container->addEntity(point1);}

        for(i=0;i<10;i++)
       { pt.x=pt.x-i;
         pt.y=pt.y-i;}


        for(j=0;j<10;j++)
       {
         pt.x=pt.x-j;
         pt.y=pt.y-j;

        RS_Point* point3 = new RS_Point(container, RS_PointData(pt));
        container->addEntity(point3);}

        for(j=0;j<10;j++)
       {
         pt.x=pt.x+j;
         pt.y=pt.y+j;}

        for(k=0;k<10;k++)
       {
         pt.x=pt.x-k;
         pt.y=pt.y+k;

        RS_Point* point4 = new RS_Point(container, RS_PointData(pt));
        container->addEntity(point4);}

        for(k=0;k<10;k++)
       {
         pt.x=pt.x+k;
         pt.y=pt.y-k;}


        for(l=0;l<10;l++)
       {
         pt.x=pt.x+l;
         pt.y=pt.y-l;

        RS_Point* point5 = new RS_Point(container, RS_PointData(pt));
        container->addEntity(point5);}




        }





       // pt.x=2*pt.x;
        // pt.y=2*pt.y;
       //  RS_Point* point1 = new RS_Point(container, RS_PointData(pt));
       // container->addEntity(point1);

        if (document) {
            document->startUndoCycle();
            document->addUndoable(point);
            document->endUndoCycle();
        }

                graphicView->redraw(RS2::RedrawDrawing);
        graphicView->moveRelativeZero(pt);

}
        }



void RS_ActionDrawStar::mouseMoveEvent(QMouseEvent* e) {
    snapPoint(e);
}



void RS_ActionDrawStar::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button()==Qt::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    } else if (e->button()==Qt::RightButton) {
        init(getStatus()-1);
    }
}



void RS_ActionDrawStar::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector mouse = e->getCoordinate();

    pt = mouse;
    trigger();
}



void RS_ActionDrawStar::commandEvent(RS_CommandEvent* e) {
    QString c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
        }
        return;
    }
}



QStringList RS_ActionDrawStar::getAvailableCommands() {
    QStringList cmd;
    return cmd;
}


void RS_ActionDrawStar::updateMouseButtonHints() {
    if (RS_DIALOGFACTORY!=NULL) {
        switch (getStatus()) {
        case 0:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify location"), tr("Cancel"));
            break;
        default:
            RS_DIALOGFACTORY->updateMouseWidget("", "");
            break;
        }
    }
}



void RS_ActionDrawStar::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



//void RS_ActionDrawPoint::updateToolBar() {
//    if (RS_DIALOGFACTORY!=NULL) {
//        if (isFinished()) {
//            RS_DIALOGFACTORY->resetToolBar();
//        }
//    }
//}

// EOF

