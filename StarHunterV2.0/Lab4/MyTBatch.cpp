/*
 *  MyTBatch.cpp
 *

Copyright (c) 2007-2009, Richard S. Wright Jr.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list 
of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this list 
of conditions and the following disclaimer in the documentation and/or other 
materials provided with the distribution.

Neither the name of Richard S. Wright Jr. nor the names of other contributors may be used 
to endorse or promote products derived from this software without specific prior 
written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR 
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


 *  This class allows you to simply add triangles as if this class were a 
 *  container. The AddTriangle() function searches the current list of triangles
 *  and determines if the vertex/normal/texcoord is a duplicate. If so, it addes
 *  an entry to the index array instead of the list of vertices.
 *  When finished, call EndMesh() to free up extra unneeded memory that is reserved
 *  as workspace when you call BeginMesh().
 *
 *  This class can easily be extended to contain other vertex attributes, and to 
 *  save itself and load itself from disk (thus forming the beginnings of a custom
 *  model file format).
 *
 */
#include "MyTBatch.h"
#include <GLShaderManager.h>

//////////////////////// TEMPORARY TEMPORARY TEMPORARY - On SnowLeopard this is suppored, but GLEW doens't hook up properly
//////////////////////// Fixed probably in 10.6.3
#ifdef __APPLE__
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glDeleteVertexArrays  glDeleteVertexArraysAPPLE
#define glBindVertexArray	glBindVertexArrayAPPLE
#endif


///////////////////////////////////////////////////////////
// Constructor, does what constructors do... set everything to zero or NULL
MyTBatch::MyTBatch(void)
    {
    pIndexes = NULL;
    pVerts = NULL;
    pNorms = NULL;
    pTexCoords = NULL;
    
    nMaxIndexes = 0;
    nNumIndexes = 0;
    nNumVerts = 0;
    }
    
////////////////////////////////////////////////////////////
// Free any dynamically allocated memory. For those C programmers
// coming to C++, it is perfectly valid to delete a NULL pointer.
MyTBatch::~MyTBatch(void)
    {
    // Just in case these still are allocated when the object is destroyed
    delete [] pIndexes;
    delete [] pVerts;
    delete [] pNorms;
    delete [] pTexCoords;
    
    // Delete buffer objects
    glDeleteBuffers(4, vbo);
    glDeleteVertexArrays(1, &vao);

    }
    
void MyTBatch::BeginMesh(GLuint nMaxVerts)
    {
    delete [] pIndexes;
    delete [] pVerts;
    delete [] pNorms;
    delete [] pTexCoords;
    
    nMaxIndexes = nMaxVerts;
    nNumIndexes = 0;
    nNumVerts = 0;

    pIndexes = new GLushort[nMaxIndexes];
    pVerts = new M3DVector3f[nMaxIndexes];
    pNorms = new M3DVector3f[nMaxIndexes];
    pTexCoords = new M3DVector2f[nMaxIndexes];
    }
  
void MyTBatch::AddTriangle(M3DVector3f verts[3], M3DVector3f vNorms[3], M3DVector2f vTexCoords[3])
    {

	for(GLuint iVertex = 0; iVertex < 3; iVertex++){
		memcpy(pVerts[nNumVerts], verts[iVertex], sizeof(float)*3);
		memcpy(pNorms[nNumVerts], vNorms[iVertex], sizeof(float)*3);
		memcpy(pTexCoords[nNumVerts], vTexCoords[iVertex], sizeof(float)*2);
		pIndexes[nNumIndexes] = nNumVerts;
		nNumIndexes++; 
		nNumVerts++;
	}   
}
    


//////////////////////////////////////////////////////////////////
// Compact the data. This is a nice utility, but you should really
// save the results of the indexing for future use if the model data
// is static (doesn't change).
void MyTBatch::End(void)
    {

	// Create the master vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    
    // Create the buffer objects
    glGenBuffers(4, vbo);
    
    // Copy data to video memory
    // Vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_DATA]);
	glEnableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nNumVerts*3, pVerts, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);

    
    // Normal data
    glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_DATA]);
	glEnableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nNumVerts*3, pNorms, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // Texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXTURE_DATA]);
	glEnableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nNumVerts*2, pTexCoords, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(GLT_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    // Indexes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_DATA]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*nNumIndexes, pIndexes, GL_DYNAMIC_DRAW);
	

	// Done
	glBindVertexArray(0);
    // Free older, larger arrays
    delete [] pIndexes;
    delete [] pVerts;
    delete [] pNorms;
    delete [] pTexCoords;

    // Reasign pointers so they are marked as unused
    pIndexes = NULL;
    pVerts = NULL;
    pNorms = NULL;
    pTexCoords = NULL;


    }

//////////////////////////////////////////////////////////////////////////
// Draw - make sure you call glEnableClientState for these arrays
void MyTBatch::Draw(void) 
	{
	glBindVertexArray(vao);
	
    glDrawElements(GL_TRIANGLES, nNumIndexes, GL_UNSIGNED_SHORT, 0);
    
	glBindVertexArray(0);

	}    


