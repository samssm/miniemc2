#    Copyright 2007 John Kasunich and Jeff Epler
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

import rs274.OpenGLTk, Tkinter
from minigl import *
from math import *

class Collection:
    def __init__(self, parts):
	self.parts = parts

    def traverse(self):
	for p in self.parts:
	    if hasattr(p, "apply"):
		p.apply()
	    if hasattr(p, "capture"):
		p.capture()
	    if hasattr(p, "draw"):
		p.draw()
	    if hasattr(p, "traverse"):
		p.traverse()
	    if hasattr(p, "unapply"):
		p.unapply()

class Translate(Collection):
    def __init__(self, parts, x, y, z):
	self.parts = parts
	self.where = x, y, z

    def apply(self):
	glPushMatrix()
	glTranslatef(*self.where)

    def unapply(self):
	glPopMatrix()

class HalTranslate(Collection):
    def __init__(self, parts, comp, var, x, y, z):
	self.parts = parts
	self.where = x, y, z
	self.comp = comp
	self.var = var

    def apply(self):
	x, y, z = self.where
	v = self.comp[self.var]
	
	glPushMatrix()
	glTranslatef(x*v, y*v, z*v)

    def unapply(self):
	glPopMatrix()


class HalRotate(Collection):
    def __init__(self, parts, comp, var, th, x, y, z):
	self.parts = parts
	self.where = th, x, y, z
	self.comp = comp
	self.var = var

    def apply(self):
	th, x, y, z = self.where
	glPushMatrix()
	glRotatef(th * self.comp[self.var], x, y, z)

    def unapply(self):
	glPopMatrix()


class Rotate(Collection):
    def __init__(self, parts, th, x, y, z):
	self.parts = parts
	self.where = th, x, y, z

    def apply(self):
	th, x, y, z = self.where
	glPushMatrix()
	glRotatef(th, x, y, z)

    def unapply(self):
	glPopMatrix()


class Track(Collection):
    '''move and rotate an object to point from one capture()'d 
	coordinate system to another.
	we need "world" to convert coordinates from GL_MODELVIEW coordinates
	to our coordinate system'''
    def __init__(self, parts, position, target, world):
	self.parts = parts
	self.target = target
	self.position = position
	self.world2view = world
	
    def angle_to(self,x,y,z):
	'''returns polar coordinates in degrees to a point from the origin
	a rotates around the x-axis; b rotates around the y axis; r is the distance'''
	a=90; b=90; r=0; 
	#i hope you arent using this function for anything serious...
	#this function exhibits gimbal lock below z=0.2 anyway
	if (abs(z) < 0.01):
		z = 0.01
	sign_z = 1;
	#if (z<0):
	#	a=-a
	#	b=-b
		
	if(x!=0):
		b = atan(z/x)*(360/(2*pi))
	
	if(x<0):
		b+= 180

	if(y!=0):
		a = atan(z/y)*(360/(2*pi))
	
	if(y<0):
		a+=180
		
	#if (z<0):
	#	b=-b
	#	a=-a
	r = sqrt(x**2+y**2+z**2)
	
	return([a,b,r])
	
	#bugger all this
	#getting world coords should go in its own function.
    def map_coords(self,tx,ty,tz,transform):
	# now we have to transform them to the world frame
	wx = tx*transform[0]+ty*transform[4]+tz*transform[8]+transform[12]
	wy = tx*transform[1]+ty*transform[5]+tz*transform[9]+transform[13]
	wz = tx*transform[2]+ty*transform[6]+tz*transform[10]+transform[14]
	return([wx,wy,wz])
	
	
    def apply(self):
	#matrix = glGetDoublev(GL_MODELVIEW_MATRIX)
	#print help(self.position)
	
	#make sure we have something to work with first
	if (self.world2view.t == []):
		#something's borkled - give up
		print "vismach.py: Track: why am i here? world is not in the scene yet"
		glPushMatrix()
		return
	
	view2world = invert(self.world2view.t)
	
	px, py, pz = self.position.t[12:15]
	px, py, pz = self.map_coords(px,py,pz,view2world)
	tx, ty, tz = self.target.t[12:15]
	tx, ty, tz = self.map_coords(tx,ty,tz,view2world)
	dx = tx - px; dy = ty - py; dz = tz - pz;
	[a,b,r] = self.angle_to(dx,dy,dz)
	if(hasattr(HUD, "debug_track") and HUD.debug_track == 1):
		HUD.strs = []
		HUD.strs += ["current coords: %3.4f %3.4f %3.4f " % (px, py, pz)]
		HUD.strs += ["target coords: %3.4f %3.4f %3.4f" %  (tx, ty, tz)]
		HUD.strs += ["a,b,r: %3.4f %3.4f %3.4f" %  (a,b,r)]
	b += 90
	a += 90
	glPushMatrix()
	glTranslatef(px,py,pz)
	glRotatef(a,1,0,0)
	glRotatef(b,0,1,0)


    def unapply(self):
	glPopMatrix()


# give endpoint X values and radii
# resulting cylinder is on the X axis
class CylinderX:
    def __init__(self, x1, r1, x2, r2):
	self.coords = x1, r1, x2, r2
	self.q = gluNewQuadric()

    def draw(self):
	x1, r1, x2, r2 = self.coords
	if x1 > x2:
	    tmp = x1
	    x1 = x2
	    x2 = tmp
	    tmp = r1
	    r1 = r2
	    r2 = tmp
	glPushMatrix()
	# GL creates cylinders along Z, so need to rotate
	z1 = x1
	z2 = x2
	glRotatef(90,0,1,0)
	# need to translate the whole thing to z1
	glTranslatef(0,0,z1)
	# the cylinder starts out at Z=0
	gluCylinder(self.q, r1, r2, z2-z1, 32, 1)
	# bottom cap
	glRotatef(180,1,0,0)
	gluDisk(self.q, 0, r1, 32, 1)
	glRotatef(180,1,0,0)
	# the top cap needs flipped and translated
	glPushMatrix()
	glTranslatef(0,0,z2-z1)
	gluDisk(self.q, 0, r2, 32, 1)
	glPopMatrix()
	glPopMatrix()

# give endpoint Y values and radii
# resulting cylinder is on the Y axis
class CylinderY:
    def __init__(self, y1, r1, y2, r2):
	self.coords = y1, r1, y2, r2
	self.q = gluNewQuadric()

    def draw(self):
	y1, r1, y2, r2 = self.coords
	if y1 > y2:
	    tmp = y1
	    y1 = y2
	    y2 = tmp
	    tmp = r1
	    r1 = r2
	    r2 = tmp
	glPushMatrix()
	# GL creates cylinders along Z, so need to rotate
	z1 = y1
	z2 = y2
	glRotatef(90,1,0,0)
	# need to translate the whole thing to z1
	glTranslatef(0,0,z1)
	# the cylinder starts out at Z=0
	gluCylinder(self.q, r1, r2, z2-z1, 32, 1)
	# bottom cap
	glRotatef(180,1,0,0)
	gluDisk(self.q, 0, r1, 32, 1)
	glRotatef(180,1,0,0)
	# the top cap needs flipped and translated
	glPushMatrix()
	glTranslatef(0,0,z2-z1)
	gluDisk(self.q, 0, r2, 32, 1)
	glPopMatrix()
	glPopMatrix()

# give endpoint Z values and radii
# resulting cylinder is on the Z axis
class CylinderZ:
    def __init__(self, z1, r1, z2, r2):
	self.coords = z1, r1, z2, r2
	self.q = gluNewQuadric()

    def draw(self):
	z1, r1, z2, r2 = self.coords
	if z1 > z2:
	    tmp = z1
	    z1 = z2
	    z2 = tmp
	    tmp = r1
	    r1 = r2
	    r2 = tmp
	# need to translate the whole thing to z1
	glPushMatrix()
	glTranslatef(0,0,z1)
	# the cylinder starts out at Z=0
	gluCylinder(self.q, r1, r2, z2-z1, 32, 1)
	# bottom cap
	glRotatef(180,1,0,0)
	gluDisk(self.q, 0, r1, 32, 1)
	glRotatef(180,1,0,0)
	# the top cap needs flipped and translated
	glPushMatrix()
	glTranslatef(0,0,z2-z1)
	gluDisk(self.q, 0, r2, 32, 1)
	glPopMatrix()
	glPopMatrix()

# give center and radius
class Sphere:
    def __init__(self, x, y, z, r):
	self.coords = x, y, z, r
	self.q = gluNewQuadric()

    def draw(self):
	x, y, z, r = self.coords
	# need to translate the whole thing to x,y,z
	glPushMatrix()
	glTranslatef(x,y,z)
	# the cylinder starts out at the origin
	gluSphere(self.q, r, 32, 16)
	glPopMatrix()

# six coordinate version - specify each side of the box
class Box:
    def __init__(self, x1, y1, z1, x2, y2, z2):
        self.coords = x1, y1, z1, x2, y2, z2

    def draw(self):
        x1, y1, z1, x2, y2, z2 = self.coords
        if x1 > x2:
	    tmp = x1
	    x1 = x2
	    x2 = tmp
        if y1 > y2:
	    tmp = y1
	    y1 = y2
	    y2 = tmp
        if z1 > z2:
	    tmp = z1
	    z1 = z2
	    z2 = tmp

        glBegin(GL_QUADS)
	# bottom face
        glNormal3f(0,0,1)
        glVertex3f(x2, y1, z1)
        glVertex3f(x1, y1, z1)
        glVertex3f(x1, y2, z1)
        glVertex3f(x2, y2, z1)
	# positive X face
        glNormal3f(1,0,0)
        glVertex3f(x2, y1, z1)
        glVertex3f(x2, y2, z1)
        glVertex3f(x2, y2, z2)
        glVertex3f(x2, y1, z2)
	# positive Y face
        glNormal3f(0,1,0)
        glVertex3f(x1, y2, z1)
        glVertex3f(x1, y2, z2)
        glVertex3f(x2, y2, z2)
        glVertex3f(x2, y2, z1)
	# negative Y face
        glNormal3f(0,-1,0)
        glVertex3f(x2, y1, z2)
        glVertex3f(x1, y1, z2)
        glVertex3f(x1, y1, z1)
        glVertex3f(x2, y1, z1)
	# negative X face
        glNormal3f(-1,0,0)
        glVertex3f(x1, y1, z1)
        glVertex3f(x1, y1, z2)
        glVertex3f(x1, y2, z2)
        glVertex3f(x1, y2, z1)
	# top face
        glNormal3f(0,0,-1)
        glVertex3f(x1, y2, z2)
        glVertex3f(x1, y1, z2)
        glVertex3f(x2, y1, z2)
        glVertex3f(x2, y2, z2)
        glEnd()

# specify the width in X and Y, and the height in Z
# the box is centered on the origin
class BoxCentered(Box):
    def __init__(self, xw, yw, zw):
        self.coords = -xw/2.0, -yw/2.0, -zw/2.0, xw/2.0, yw/2.0, zw/2.0

# specify the width in X and Y, and the height in Z
# the box is centered in X and Y, and runs from Z=0 up
# (or down) to the specified Z value
class BoxCenteredXY(Box):
    def __init__(self, xw, yw, zw):
        self.coords = -xw/2.0, -yw/2.0, 0, xw/2.0, yw/2.0, zw

# capture current transformation matrix
# note that this tranforms from the current coordinate system
# to the viewport system, NOT to the world system
class Capture:
    def __init__(self):
	self.t = []

    def capture(self):
	self.t = glGetDoublev(GL_MODELVIEW_MATRIX)

# function to invert a transform matrix
# based on http://steve.hollasch.net/cgindex/math/matrix/afforthinv.c
# with simplifications since we don't do scaling

# This function inverts a 4x4 matrix that is affine and orthogonal.  In
# other words, the perspective components are [0 0 0 1], and the basis
# vectors are orthogonal to each other.  In addition, the matrix must
# not do scaling

def invert(src):
	# make a copy
	inv=src[:]
	# The inverse of the upper 3x3 is the transpose (since the basis
	# vectors are orthogonal to each other.
	inv[1],inv[4] = inv[4],inv[1]
	inv[2],inv[8] = inv[8],inv[2]
	inv[6],inv[9] = inv[9],inv[6]
	# The inverse of the translation component is just the negation
	# of the translation after dotting with the new upper3x3 rows. */	
	inv[12] = -(src[12]*inv[0] + src[13]*inv[4] + src[14]*inv[8])
	inv[13] = -(src[12]*inv[1] + src[13]*inv[5] + src[14]*inv[9])
	inv[14] = -(src[12]*inv[2] + src[13]*inv[6] + src[14]*inv[10])
	return inv

class Hud(object):
	'''head up display - draws a semi-transparent text box.
	use HUD.strs for things that must be updated constantly,
	and HUD.show("stuff") for one-shot things like error messages'''
	def __init__(self,  showme=1):
		self.app = []
		self.strs = []
		self.messages = []
		self.showme = 0
		
	def show(self, string="xyzzy"):
		self.showme = 1
		if string != "xyzzy":
			self.messages += [str(string)]
		
	def hide(self):
		self.showme = 0
		
	def clear(self):
		self.messages = []
		
	def draw(self):
		drawtext = self.strs + self.messages
		self.lines = len(drawtext)
		#draw head-up-display
		#see axis.py for more font/color configurability
		if ((self.showme == 0) or (self.lines == 0)):
			return
		
		glMatrixMode(GL_PROJECTION)
		glPushMatrix()
		glLoadIdentity()
		
		#pointer to font?
		fontbase = int(self.app.loadbitmapfont("9x15"))
		char_width, char_height = 9, 15
		xmargin,ymargin = 5,5
		ypos = float(self.app.winfo_height())
		
		glOrtho(0.0, self.app.winfo_width(), 0.0, ypos, -1.0, 1.0)
		glMatrixMode(GL_MODELVIEW)
		glPushMatrix()
		glLoadIdentity()
		
		#draw the text box
		maxlen = max([len(p) for p in drawtext])
		box_width = maxlen * char_width
		glDepthFunc(GL_ALWAYS)
		glDepthMask(GL_FALSE)
		glDisable(GL_LIGHTING)
		glEnable(GL_BLEND)
		glBlendFunc(GL_ONE, GL_CONSTANT_ALPHA)
		glColor3f(0.2,0,0)
		glBlendColor(0,0,0,0.5) #rgba
		glBegin(GL_QUADS)
		glVertex3f(0, ypos, 1) #upper left
		glVertex3f(0, ypos - 2*ymargin - char_height*len(drawtext), 1) #lower left
		glVertex3f(box_width+2*xmargin, ypos - 2*ymargin - char_height*len(drawtext), 1) #lower right
		glVertex3f(box_width+2*xmargin,  ypos , 1) #upper right
		glEnd()
		glDisable(GL_BLEND)
		glEnable(GL_LIGHTING)
		
		#fill the box with text
		maxlen = 0
		ypos -= char_height+ymargin
		i=0
		glDisable(GL_LIGHTING)
		glColor3f(0.9,0.9,0.9)
		for string in drawtext:
			maxlen = max(maxlen, len(string))
		#	if i < len(homed) and homed[i]:
		#		glRasterPos2i(6, ypos)
		#		glBitmap(13, 16, 0, 3, 17, 0, homeicon)
			glRasterPos2i(xmargin, int(ypos))
			for char in string:
				glCallList(fontbase + ord(char))
		#	if i < len(homed) and limit[i]:
		#		glBitmap(13, 16, -5, 3, 17, 0, limiticon)
			ypos -= char_height
			i = i + 1
		glDepthFunc(GL_LESS)
		glDepthMask(GL_TRUE)
		glEnable(GL_LIGHTING)
	
		glPopMatrix()
		glMatrixMode(GL_PROJECTION)
		glPopMatrix()
		glMatrixMode(GL_MODELVIEW)


class O(rs274.OpenGLTk.Opengl):
    def __init__(self, *args, **kw):
        rs274.OpenGLTk.Opengl.__init__(self, *args, **kw)
        self.r_back = self.g_back = self.b_back = 0
        self.bind('<Button-4>', self.zoomin)
        self.bind('<Button-5>', self.zoomout)
	#self.q1 = gluNewQuadric()
	#self.q2 = gluNewQuadric()
	#self.q3 = gluNewQuadric()
	self.plotdata = []
	self.plotlen = 4000
	#does not show HUD by default
	self.hud = Hud()

    def zoomin(self, event):
        self.distance = self.distance / 1.1
        self.tkRedraw()
  
    def zoomout(self, event):
        self.distance = self.distance * 1.1
        self.tkRedraw()
  
    def basic_lighting(self):
        self.activate()
        glLightfv(GL_LIGHT0, GL_POSITION, (1, -1, .5, 0))
        glLightfv(GL_LIGHT0, GL_AMBIENT, (.2,.2,.2,0))
        glLightfv(GL_LIGHT0, GL_DIFFUSE, (.6,.6,.4,0))
        glLightfv(GL_LIGHT0+1, GL_POSITION, (-1, -1, .5, 0))
        glLightfv(GL_LIGHT0+1, GL_AMBIENT, (.0,.0,.0,0))
        glLightfv(GL_LIGHT0+1, GL_DIFFUSE, (.0,.0,.4,0))
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (1,1,1,0))
	glEnable(GL_CULL_FACE)
        glEnable(GL_LIGHTING)
        glEnable(GL_LIGHT0)
        glEnable(GL_LIGHT0+1)
        glDepthFunc(GL_LESS)
        glEnable(GL_DEPTH_TEST)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
  

		

    def redraw(self, *args):
        if self.winfo_width() == 1: return
        self.model.traverse()
	# current coords: world
	# the matrices tool2view, work2view, and world2view
	# transform from tool/work/world coords to viewport coords
	# if we want to draw in tool coords, we need to do
	# "tool -> view -> world" (since the current frame is world)
	# and if we want to draw in work coords, we need
	# "work -> view -> world".  For both, we need to invert
	# the world2view matrix to do the second step
	view2world = invert(self.world2view.t)
	# likewise, for backplot, we want to transform the tooltip
	# position from tool coords (where it is [0,0,0]) to work
	# coords, so we need tool -> view -> work
	# so lets also invert the work2view matrix
	view2work = invert(self.work2view.t)

	# since backplot lines only need vertexes, not orientation,
	# and the tooltip is at the origin, getting the tool coords
	# is easy
	tx, ty, tz = self.tool2view.t[12:15]
	# now we have to transform them to the work frame
	wx = tx*view2work[0]+ty*view2work[4]+tz*view2work[8]+view2work[12]
	wy = tx*view2work[1]+ty*view2work[5]+tz*view2work[9]+view2work[13]
	wz = tx*view2work[2]+ty*view2work[6]+tz*view2work[10]+view2work[14]
	# wx, wy, wz are the values to use for backplot
	# so we save them in a buffer
        if len(self.plotdata) == self.plotlen:
	    del self.plotdata[:self.plotlen / 10]
	point = [ wx, wy, wz ]
	if not self.plotdata or point != self.plotdata[-1]:
	    self.plotdata.append(point)

	# now lets draw something in the tool coordinate system
	#glPushMatrix()
	# matrixes take effect in reverse order, so the next
	# two lines do "tool -> view -> world"
	#glMultMatrixd(view2world)
	#glMultMatrixd(self.tool2view.t)

	# do drawing here
	# cylinder normally goes to +Z, we want it down
	#glTranslatef(0,0,-60)
	#gluCylinder(self.q1, 20, 20, 60, 32, 16)

	# back to world coords
	#glPopMatrix()
	
	
	# we can also draw in the work coord system
	glPushMatrix()
	# "work -> view -> world"
	glMultMatrixd(view2world)
	glMultMatrixd(self.work2view.t)
	# now we can draw in work coords, and whatever we draw
	# will move with the work, (if the work is attached to
	# a table or indexer or something that moves with
	# respect to the world

	# just a test object, sitting on the table
	#gluCylinder(self.q2, 40, 20, 60, 32, 16)
	
	#draw head up display
	if(hasattr(self.hud, "draw")):
		self.hud.draw()

	# draw backplot
	glDisable(GL_LIGHTING)
        glBegin(GL_LINE_STRIP)
	for p in self.plotdata:
	    glVertex3f(*p)
	glEnd()
	glEnable(GL_LIGHTING)

	# back to world again
	glPopMatrix()


def main(model, tool, work, size=10, hud=0):
    app = Tkinter.Tk()

    t = O(app, double=1, depth=1)

    #there's probably a better way of doing this
    global HUD
    HUD = 0
    if(hud != 0 and hasattr(hud, "app")):
    	HUD = hud
		#point our app at the global
    	t.hud = HUD

    t.hud.app = t #HUD needs to know where to draw
	
    # need to capture the world coordinate system
    world = Capture()

    t.model = Collection([model, world])
    t.distance = size * 3
    t.near = size * 0.01
    t.far = size * 10.0
    t.tool2view = tool
    t.world2view = world
    t.work2view = work

    t.pack(fill="both", expand=1)

    def update():
	t.tkRedraw()
	t.after(100, update)
    update()
    try:
	app.mainloop()
    except KeyboardInterrupt:
	raise SystemExit
