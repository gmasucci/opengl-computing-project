#include "terrain.h"

using namespace std;

terrain::terrain(char* fname,float scaleIn,GLuint*tex,GLGeometryTransform *pGLGTin)
{

	string loadIt = "Textures/PGM/";
	loadIt = loadIt.append(fname);


	this->pGLGT=pGLGTin;
	this->myTex=tex;
	scale=scaleIn;
	ifstream::pos_type fsize;
	char * memblock;
	
	ifstream file (loadIt.c_str(), ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		fsize = file.tellg();
		memblock = new char [fsize];
		file.seekg (0, ios::beg);
		file.read (memblock, fsize);
		file.close();
		cout << "File " << fname << " loaded. Building Terrain..." << endl;
	
	
	stringstream strstream(memblock); 
	string check;
	int maxval,y;
	size=0;
	
	
	strstream >> check >> size >> size >> maxval;

	
	
	if(check.compare(0,2,"P2")!=0){
		//not PGM-ASCII;
		MessageBox(NULL, "Incorrect File Format!\nFile is not PGM-ASCII", "Oh Crap!", MB_OK | MB_ICONERROR);
	}else{

			
	numVerts = size*size;
	
	if(this->size>128 || this->size <2)
		MessageBox(NULL, "Terrain file is broken!\nI'll try and keep going but you'll get\nan exception down the line somewhere...", "Fatal error!", MB_OK | MB_ICONERROR);
	float halfSize = size / 2.0;
	yscale=0.05;

		//heights = new unsigned char[size*size]; //buffer
		//mesh = new float[(numVerts*3)];
		vertex = new Vec3[numVerts*6];
		normal = new Vec3[numVerts*6];
		heights = new unsigned char[numVerts];
		glPointSize(5.0f);
		
		
		
		int xc=0,yc=1,zc=2;
		int x=0,z=0,v=0;
		for(int x = 0; x<size;x++){
			for(int z=0;z<size;z++){
				strstream >> y;
				int pos=z+(x*size);
				heights[pos]=(int)y;

				vertex[pos].x = x*scale;
				vertex[pos].y = y*this->yscale;
				vertex[pos].z = z*scale;

			}
		}

		this->avgNormals();

		myTri.BeginMesh((numVerts*6)); //6 = 3 per triangle, 2 tris per x,z coord

		float step=1;
		for(int x = 0; x<size-1;x++){
			for(int z=0;z<size-1;z++){
				drawV(x, z);
			}
		}
		myTri.End();

	}
	}

	cout	<< "Terrain Generated!" << endl
			<< "Indices in terrain   = " << myTri.GetIndexCount() << endl
			<< "Vertices in terrain  = " << myTri.GetVertexCount() << endl;

	grassShader = gltLoadShaderPairWithAttributes("Shaders/noSpec.vert", "Shaders/noSpec.frag", 3, GLT_ATTRIBUTE_VERTEX, "vVertex",
			GLT_ATTRIBUTE_NORMAL, "vNormal", GLT_ATTRIBUTE_TEXTURE0, "vTexture0");

	locAmbient = glGetUniformLocation(grassShader, "ambientColor");
	locDiffuse = glGetUniformLocation(grassShader, "diffuseColor");
	locSpecular = glGetUniformLocation(grassShader, "specularColor");
	locLight = glGetUniformLocation(grassShader, "vLightPosition");
	locMVP = glGetUniformLocation(grassShader, "mvpMatrix");
	locMV  = glGetUniformLocation(grassShader, "mvMatrix");
	locNM  = glGetUniformLocation(grassShader, "normalMatrix");
	locTexture = glGetUniformLocation(grassShader, "colorMap");

}

float terrain::getHeight(float x, float z){
	return yscale*(this->heights[int(z +(x*size))]);
}
#define LERP(a,b,t)   a + ((b-a)*t)


float terrain::getHeightAt(float x, float z){

	float BL= getHeight(int(x/scale),int(z/scale));
	float BR= getHeight(int(x/scale),int((z+1)/scale));
	float TL= getHeight(int((x+1)/scale),int(z/scale));
	float TR= getHeight(int((x+1)/scale),int((z+1)/scale));
	
	float dx= (x) - ((int)x); 
	float dz= (z) - ((int)z); 

	float p1=LERP(BL,BR,dz);
	float p2=LERP(TL,TR,dz);
	float p3=LERP(p1,p2,dx);

	float retval=0;

	return p3;


}

void terrain::getCPAt(float*result,float x, float z){
	float BL= getHeight(int(x/scale),int(z/scale));
	float BR= getHeight(int(x/scale),int((z+1)/scale));
	float TL= getHeight(int((x+1)/scale),int(z/scale));
	float TR= getHeight(int((x+1)/scale),int((z+1)/scale));
	
	float dx= (x) - ((int)x); 
	float dz= (z) - ((int)z); 

	M3DVector3f a,b,r1,r2,cross1,cross2,res;
	a[0]=x/scale;a[1]=BL;a[2]=z/scale;
	b[0]=x/scale;b[1]=BR;b[2]=(z+1)/scale;
	m3dSubtractVectors3(r1,a,b);
	b[0]=(x+1)/scale;b[1]=TL;b[2]=(z)/scale;
	m3dSubtractVectors3(r2,a,b);
	m3dCrossProduct3(cross1,r1,r2);

	a[0]=(x+1)/scale;a[1]=TR;a[2]=(z+1)/scale;
	m3dSubtractVectors3(r1,a,b);
	b[0]=x/scale;b[1]=BR;b[2]=(z+1)/scale;
	m3dSubtractVectors3(r2,a,b);
	m3dCrossProduct3(cross2,r1,r2);

	m3dAddVectors3(res,cross1,cross2);
	m3dNormalizeVector3(res);
	m3dCopyVector3(result,res);

}

void terrain::avgNormals(){
	//private function to average out the normals in the terrain.
	//per vertex
	//	work out what indexes would be where.
	//	check if that ind`ex exists
	//
	//vertices[] contains every vertex in the terrain, even if they are at the exact same place in space;
	// we need this for the normals, so we can smooth that shit out... + we need a normal for every vertex
	// bear in mind this will all need to be done BEFORE we go to insert it into the batch.

	for(int x = 0; x<size;x++){
		for(int z=0;z<size;z++){
			int BL=z+(x*size);
			int BR=BL+1;
			int TL=z+((x+1)*size);
			int TR=TL+1;

			Vec3 tris[3],tmpNorm;
			
			tris[0] = vertex[BL];
			tris[1] = vertex[BR];
			tris[2] = vertex[TL];
			
			tmpNorm = loaders::normGen(tris);
			
			normal[BL] = tmpNorm;
			normal[BR] = tmpNorm;
			normal[TL] = tmpNorm;
			
			tris[0] = vertex[TL];
			tris[1] = vertex[BR];
			tris[2] = vertex[TR];
			
			tmpNorm = loaders::normGen(tris);

			normal[TL] = tmpNorm;
			normal[BR] = tmpNorm;
			normal[TR] = tmpNorm;
		}
	}

	// normals[] filled. Lets smooth the fuck outta it.

	for(int x = 1; x<size;x++){
		for(int z=1;z<size;z++){
			int CR	= z+(x*size);
			int UP	= z+((x+1)*size);
			int LF	= (z-1)+(x*size);
			int DN	= z+((x-1)*size);
			int RT	= (z+1)+(x*size);
			
			Vec3 smoothedNormal;

			smoothedNormal.x = normal[LF].x + normal[RT].x + normal[UP].x + normal[DN].x;
			smoothedNormal.y = normal[LF].y + normal[RT].y + normal[UP].y + normal[DN].y;
			smoothedNormal.z = normal[LF].z + normal[RT].z + normal[UP].z + normal[DN].z;
			
			double len = smoothedNormal.length();

			smoothedNormal.x *= 1.0/len;
			smoothedNormal.y *= 1.0/len;
			smoothedNormal.z *= 1.0/len;

			normal[CR] = smoothedNormal;


		}
	}



}



void terrain::drawV(const int x,const int z){
				M3DVector3f verts[3],norms[3],temp;
				M3DVector2f tex[3];
				
				int BL=z+(x*size);
				int BR=BL+1;
				int TL=z+((x+1)*size);
				int TR=TL+1;

				float nx =x*scale;
				float nz =z*scale;
				float step=scale;
				
				verts[0][0]=nx;
				verts[0][1]=getHeight(x,z);
				verts[0][2]=nz;

				verts[1][0]=nx;
				verts[1][1]=getHeight(x,z+1);
				verts[1][2]=nz+step;

				verts[2][0]=nx+step;
				verts[2][1]=getHeight(x+1,z);
				verts[2][2]=nz;

				norms[0][0]=normal[BL].x;
				norms[0][1]=normal[BL].y;
				norms[0][2]=normal[BL].z;
				
				norms[1][0]=normal[BR].x;
				norms[1][1]=normal[BR].y;
				norms[1][2]=normal[BR].z;
				
				norms[2][0]=normal[TL].x;
				norms[2][1]=normal[TL].y;
				norms[2][2]=normal[TL].z;
				

				tex[0][0] = 0;tex[0][1] = 0;
				tex[1][0] = 0;tex[1][1] = 1;
				tex[2][0] = 1;tex[2][1] = 0;

				myTri.AddTriangle(verts,norms,tex);
				//////////   tri 2
				verts[0][0]=nx+step;
				verts[0][1]=getHeight(x+1,z);
				verts[0][2]=nz;

				verts[1][0]=nx;
				verts[1][1]=getHeight(x,z+1);
				verts[1][2]=nz+step;

				verts[2][0]=nx+step;
				verts[2][1]=getHeight(x+1,z+1 );
				verts[2][2]=nz+step;

				norms[0][0]=normal[TL].x;
				norms[0][1]=normal[TL].y;
				norms[0][2]=normal[TL].z;
				
				norms[1][0]=normal[BR].x;
				norms[1][1]=normal[BR].y;
				norms[1][2]=normal[BR].z;
				
				norms[2][0]=normal[TR].x;
				norms[2][1]=normal[TR].y;
				norms[2][2]=normal[TR].z;
				
				tex[0][0] = 1;tex[0][1] = 0;
				tex[1][0] = 0;tex[1][1] = 1;
				tex[2][0] = 1;tex[2][1] = 1;

				myTri.AddTriangle(verts,norms,tex);
	

}

void terrain::drawMe(Camera *camIn){
		
	M3DVector4f vEyeLight,vLight = {128.0f, 128.0f, -128.0f ,1.0f};
	M3DMatrix44f *mx;
	mx = camIn->getMx();
    m3dTransformVector4(vEyeLight, vLight, *mx);
		
	GLfloat vAmbientColor[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat vDiffuseColor[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat vSpecularColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glBindTexture(GL_TEXTURE_2D,myTex[0]);
	 	glUseProgram(grassShader);
		glUniform4fv(locAmbient, 1, vAmbientColor);
		glUniform4fv(locDiffuse, 1, vDiffuseColor);
		glUniform4fv(locSpecular, 1, vSpecularColor);
		glUniform3fv(locLight, 1, vEyeLight);
		glUniformMatrix4fv(locMVP, 1, GL_FALSE, pGLGT->GetModelViewProjectionMatrix());
		glUniformMatrix4fv(locMV, 1, GL_FALSE, pGLGT->GetModelViewMatrix());
		glUniformMatrix3fv(locNM, 1, GL_FALSE, pGLGT->GetNormalMatrix());
		glUniform1i(locTexture, 0);

	myTri.Draw();
	
	
	
}
terrain::~terrain(){
	delete[] heights;
}