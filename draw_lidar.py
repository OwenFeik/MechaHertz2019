from rplidar import RPLidar
from math import sin,cos,radians
import cv2

lidar=RPLidar('COM3')
data=lidar.iter_scans()

# import json # Use dummy data for no lidar
# with open('data/dummydata.json','r') as f:
#     data=json.load(f)

def make_point(bearing,distance):
    """Bearing is the bearing of the lidar reading in degrees, distance is the distance in mm.\nReturns an (x,y) point."""
    bearing=radians(bearing)

    x=int((sin(bearing)*distance)/4)
    y=int(-1*((cos(bearing)*distance))/4)
    
    return (x,y)

class Map():
    def __init__(self,points=None):
        """The map class is used to hold a set of points on a cartesian plane. It can then be used to perform analysis on these points."""
        self.points=points if points else []

    def find_edges(self):
        """This function finds the 2 x and 2 y values with the highest frequency of points. These are likely to correspond to the edges of the field."""
        
        # This block is repeated four times in order to find the edge to the left, right, in front and behind the bot.
        top_c=0 # Highest count of points.
        top_y=0 # y value at which this count occurs.
        for i in range(1,500,10): # The point cloud ranges from -500 to 500. We do step 10 to speed things up and allow for small deviations.
            count=0 # Count of points in this region.
            for p in self.points:
                if abs(p[1]-i)<10: # If the point's y value is within 10 of this one.
                    count+=1*i # Increment the count by the y value; we use this multiplier because the density of points lowers the further away we get (inverse square)

            if count>top_c: # If this value is greater than the top, this is the new top.
                top_c=count
                top_y=i
        
        bot_c=0
        bot_y=0
        for i in range(-500,0,10):
            count=0
            for p in self.points:
                if abs(p[1]-i)<10:
                    count+=1*-i

            if count>bot_c:
                bot_c=count
                bot_y=i

        lft_c=0
        lft_x=0
        for i in range(-500,0,10):
            count=0
            for p in self.points:
                if abs(p[0]-i)<10:
                    count+=1*-i

            if count>lft_c:
                lft_c=count
                lft_x=i

        rgt_c=0
        rgt_x=0
        for i in range(1,500,10):
            count=0
            for p in self.points:
                if abs(p[0]-i)<10:
                    count+=1*i

            if count>rgt_c:
                rgt_c=count
                rgt_x=i

        return lft_x,rgt_x,top_y,bot_y

point_colour=(0,255,0) # Aesthetic.
line_colour=(0,0,255) # Aesthetic.
max_dist=50 # Max distance to consider 2 points of the same object
angle=-2 # The angle by which the points will be rotated around the origin. In actual use, this would be done with a compass reading.

# Scan looks like this:
# [(quality, bearing, distance), (12, 345.25, 450)]

go=True
while go:
    for scan in data:
        base=cv2.imread('data/base.png')
        m=Map()
        for reading in scan:
            point=make_point(reading[1]+angle,reading[2])
            m.points.append(point)

            cv2.circle(base,(point[0]+500,point[1]+500),2,point_colour,2)

        l,r,t,b=m.find_edges()
        cv2.line(base,(l+500,0),(l+500,1000),line_colour)
        cv2.line(base,(r+500,0),(r+500,1000),line_colour)
        cv2.line(base,(0,t+500),(1000,t+500),line_colour)
        cv2.line(base,(0,b+500),(1000,b+500),line_colour)

        cv2.imshow('scan',base)
        key=cv2.waitKey(30) & 0xFF
        if key==ord('q'):
            cv2.destroyAllWindows()
            go=False
            break
