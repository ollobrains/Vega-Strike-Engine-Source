/* 
 * Vega Strike
 * Copyright (C) 2001-2002 Daniel Horn & Alan Shieh
 * 
 * http://vegastrike.sourceforge.net/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#include <string.h>

#include "vegastrike.h"
#include "gfxlib.h"
#include "gl_globals.h"
extern BOOL bTex0;
extern BOOL bTex1;

BOOL /*GFXDRVAPI*/ GFXBeginScene()
{
	statsqueue.push(GFXStats());
	GFXClear();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // bad this should instead load the cached view matrix
	return TRUE;
}

BOOL /*GFXDRVAPI*/ GFXEndScene()
{
  //	glFinish();//finish all drawing commandes
	
        glFlush();
	glutSwapBuffers(); //swap the buffers
	char temp[128];
	/*	sprintf(temp, "Frame stats for frame # %d: %d Tris, %d Quads, %d Points, %d total vertices drawn in %d milliseconds.\n",
		statsqueue.size(),
		statsqueue.back().drawnTris,
		statsqueue.back().drawnQuads,
		statsqueue.back().drawnPoints,
		statsqueue.back().total(),
		statsqueue.back().elapsedTime());
		printf(temp);*/  ///FIXME VEGASTRIKE
	return TRUE;
}

BOOL /*GFXDRVAPI*/ GFXClear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return TRUE;
}

BOOL /*GFXDRVAPI*/ GFXCapture(char *filename)
{
	return FALSE;
}

BOOL /*GFXDRVAPI*/ GFXBegin(enum PRIMITIVE ptype)
{
	GLenum mode;
	switch(ptype)
	{
	case TRIANGLES:
		statsqueue.back() += GFXStats(1, 0, 0);
		mode = GL_TRIANGLES;
		break;
	case TRIANGLE_STRIP:
		mode = GL_TRIANGLE_STRIP;
		break;
	case TRIANGLE_FAN:
		mode = GL_TRIANGLE_FAN;
		break;
	case QUADS:
		statsqueue.back() += GFXStats(0, 1, 0);
		mode = GL_QUADS;
		break;
	case POLYGON:
		mode = GL_POLYGON;
		break;
	}
	if(g_game.Multitexture) {
		glActiveTextureARB(GL_TEXTURE0_ARB);	
		if(bTex0)
			glEnable (GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);
		glActiveTextureARB(GL_TEXTURE1_ARB);	
		if(bTex1)
			glEnable (GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);
		glActiveTextureARB(GL_TEXTURE0_ARB);	
	}
	else {
		if(bTex0) {
			glEnable(GL_TEXTURE_2D);
		}
	}

	glBegin(mode);

	return TRUE;
}

void /*GFXDRVAPI*/ GFXColor4f(float r, float g, float b, float a)
{
	glColor4f(r,g,b,a);
}

void /*GFXDRVAPI*/ GFXTexCoord2f(float s, float t)
{
	glTexCoord2f(s,t);
}

void /*GFXDRVAPI*/ GFXTexCoord4f(float s, float t, float u, float v)
{
	if(g_game.Multitexture)
	{
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, s,t);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, u,v);
	}
	else {
		glTexCoord2f(s,t);
	}
}

void /*GFXDRVAPI*/ GFXNormal3f(float i, float j, float k)
{
	glNormal3f(i,j,k);
}

void /*GFXDRVAPI*/ GFXNormal(Vector n)
{
	glNormal3f(n.i,n.j,n.k);
}

void /*GFXDRVAPI*/ GFXVertex3f(float x, float y, float z)
{
	glVertex3f(x,y,z);
}

BOOL /*GFXDRVAPI*/ GFXEnd()
{
	glEnd();
	return FALSE;
}
