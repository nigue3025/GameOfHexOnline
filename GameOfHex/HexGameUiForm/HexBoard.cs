using System;
using System.Collections.Generic;
using System.Drawing;

using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HexGameUiForm
{
    public class GraphicalElement
    {
        public Rectangle bound = Rectangle.Empty;
        public Color myColor=Color.Aqua;

        Random rnd = new Random();
        //public Random staticRnd;
        public Point startPoint;
        public Point endPoint;
        public bool hasRelation = false;
        public GraphicalElement(Rectangle rec)
        {
            bound = rec;

        }
        public virtual void drawElement(Graphics g)
        {
           // g.FillRectangle(new SolidBrush(myColor), bound);
         g.FillEllipse(new SolidBrush(myColor), bound);

            // g.FillRectangle(new SolidBrush(Color.Green),g);
        }
        public virtual void drawCircle(Graphics g)
        {
            g.FillEllipse(new SolidBrush(myColor), bound);
            endPoint.Y = bound.Top;
            endPoint.X = bound.Left;
        }

        public virtual bool contain(Point pt)
        {
            return bound.Contains(pt);
        }

    }



    public class Placement : GraphicalElement
    {
        public Placement(Rectangle bound):base(bound)
        {
            myColor = Color.DarkGray;
        }
    }

    public class BlueChess : Placement //First Player
    {
        public BlueChess(Rectangle bound) : base(bound)
        {
            myColor = Color.Blue;
        }
    }
    public class RedChess : Placement //Second Player
    {
        public RedChess(Rectangle bound) : base(bound)
        {
            myColor = Color.Red;
        }
    }



    public class HexChessBoard
    {
        public int size;
        public List<GraphicalElement> Placements;
        int node_length = 25;

        public HexChessBoard(Graphics graphics, Rectangle wholebound,int size=5)
        {
            this.size = size;
            Placements = new List<GraphicalElement>();
            int width_gap = (int)((wholebound.Right - wholebound.Left) / (2 * size*2));
            int height_gap = (int)((wholebound.Bottom - wholebound.Top) / (size));           
            for (int i = 0; i < this.size; ++i)
                for(int j=0;j<this.size;++j)               
                        Placements.Add(new Placement(new Rectangle(3 + 2 * width_gap * j + i * 2 * width_gap, 10 + height_gap * i, node_length, node_length)));
            paintPlacement(graphics, wholebound);
        }

        public void paintPlacement(Graphics graphics, Rectangle wholebound)
        {

            foreach (var placement in Placements)
                placement.drawElement(graphics);
        //    for (int i = 0; i < Placements.Count; ++i)           
          //      Placements[i].drawElement(graphics);
            
        }

        public void AddPlacement(Placement placement)
        {
            Placements.Add(placement);
        }


    }
}
