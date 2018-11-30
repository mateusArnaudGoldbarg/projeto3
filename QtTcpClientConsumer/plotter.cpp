#include "plotter.h"
#include "plotter.h"

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <vector>
#include <cmath>

using namespace std;



Plotter::Plotter(QWidget *parent) : QWidget(parent)
{

}

void Plotter::Plotter::paintEvent(QPaintEvent *event)
{
      QPainter painter(this);
      QBrush brush;
      QPen pen;

      QPen pengrid;
      int igridH=0;
      int igridV=0;

      // habilita o anti aliasing (atenua o
      // efeito de serrilhado nas figuras geometricas)
      painter.setRenderHint(QPainter::Antialiasing);

      // brush com a cor preta com preenchimento
      // solido
      brush.setColor(QColor(0,0,0));
      brush.setStyle(Qt::SolidPattern);
      // informa ao painter qual o pincel atual
      painter.setBrush(brush);

      // pen com cor verde e dois pixels de largura
      pen.setColor(QColor(0,255,0));
      pen.setWidth(2);

      // informa ao painter qual o pen atual
      painter.setPen(pen);

      // desenha um retangulo tomando toda a area
      // do componente
      painter.drawRect(0,0,width(), height());

      // muda o estilo do tracejado
      pen.setStyle(Qt::DashLine);
      pen.setWidth(1);
      // comunica ao painter a nova caneta
      painter.setPen(pen);


      pengrid.setStyle(Qt::DashLine);
      pengrid.setWidth(1);
      pengrid.setColor(QColor(255,255,255));
      painter.setPen(pengrid);


      while( igridV< width()){

          painter.drawLine(igridV, 0, igridV, height());
          igridV+=width()/15;
      }

      while (igridH<height()){
          painter.drawLine(0,igridH, width(), igridH);
          igridH+=height()/15;
      }

      pen.setStyle(Qt::SolidLine);
      pen.setWidth(2);
      pen.setColor(QColor(0,255,0));
      painter.setPen(pen);



      for(int i=0; i<(ptempo.size()-1) && ptempo.size()!=0; i++){
          painter.drawLine(
                        width()-width()*ptempo[i], height()-height()*(pvalores[i]*0.01), width()-width()*ptempo[i+1], height()-height()*(pvalores[i+1]*0.01));
       }
}

void Plotter::armazenaPontos(vector<float> _ptempo, vector<float> _pvalores)
{
   pvalores = _pvalores;
   ptempo = _ptempo;

   repaint();
}


