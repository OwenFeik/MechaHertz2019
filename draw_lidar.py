# from rplidar import RPLidar
from math import sin,cos,radians,sqrt
import cv2

# lidar=RPLidar('COM3')
# data=lidar.iter_scans()

import json # Use dummy data for no lidar
with open('dummydata.json','r') as f:
    data=json.load(f)

def make_point(bearing,distance):
    """Bearing is the bearing of the lidar reading in degrees, distance is the distance in mm.\nReturns an (x,y) point."""
    bearing=radians(bearing)

    x=int((sin(bearing)*distance))
    y=int(-1*((cos(bearing)*distance)))
    
    return Point(x,y)

def in_range(point_a,point_b,dist):
    """Points a and b are two (x,y) coordinates, dist is the maximum allowable distance betwee them.\nReturns True or False based on pythagorean distance."""

    a=max(point_a[0],point_b[0])-min(point_a[0],point_b[0])
    b=max(point_a[1],point_b[1])-min(point_a[1],point_b[1])

    if sqrt(a**2+b**2)<dist:
        return True
    return False

class Point():
    def __init__(self,x,y,alt=None):
        self.x=x
        self.y=y
        self.alt=alt

    @property
    def point(self):
        return (self.x,self.y)


class Line(): # Series of points with similar x or y values
    def __init__(self,direction,points=None):
        self.direction=direction
        self.points=points if points else []            

    def __str__(self):
        return str(self.points)

    def draw_self(self,img):
        if self.direction=='h':
            colour=(255,0,0)
        else:
            colour=(0,0,255)

        for i in range(1,len(self.points)):
            cv2.line(img,self.points[i].alt,self.points[i-1].alt,colour,2)

        return img

class Shape(): # Cloud of nearby points for analysis
    def __init__(self,points=None):
        self.points=points if points else []

    def find_lines(self):
        _lines=[]
        line=None

        _prev=None
        for _point in self.points:
            if _prev:
                if abs(_point.x-_prev.x)<=10:
                    if line and line.direction=='v':
                        line.points.append(_point)
                    elif line:
                        _lines.append(line)
                        line=Line('v',[_prev,_point])
                    else:
                        line=Line('v',[_prev,_point])
                elif abs(_point.y-_prev.y)<=10:
                    if line and line.direction=='h':
                        line.points.append(_point)
                    elif line:
                        _lines.append(line)
                        line=Line('h',[_prev,_point])
                    else:
                        line=Line('h',[_prev,_point])
            _prev=_point
        if line:
            _lines.append(line)

        return _lines

colour=(0,255,0) # Point draw colour
max_dist=50 # Max distance to consider 2 points of the same object

# Scan looks like this:
# [(quality, bearing, distance), (12, 345.25, 450)]

go=True
while go:
    prev=make_point(0,0)
    for i,scan in enumerate(data):
        base=cv2.imread('base.png')
        shapes=[]
        shape=None
        for reading in scan:

            point=make_point(reading[1],reading[2]).point
            r_point=make_point(reading[1]-i,reading[2])
            r_point.alt=point

            cv2.circle(base,point,2,colour,2)
            
            if in_range(r_point.point,prev.point,max_dist):
                if shape:
                    shape.points.append(r_point)
                else:
                    shape=Shape([prev,r_point])
            else:
                if shape:
                    shapes.append(shape)
                shape=None

            prev=r_point

        if shape:
            shapes.append(shape)

        lines=[]
        for shape in shapes:
            lines.extend(shape.find_lines())

        for line in lines:
            base=line.draw_self(base)

        cv2.imshow('scan',base)
        key=cv2.waitKey(30) & 0xFF
        if key==ord('q'):
            cv2.destroyAllWindows()
            go=False
            break
