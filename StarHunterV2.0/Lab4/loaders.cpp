#include "loaders.h"
using namespace std;

void loaders::tex(char *fname, GLuint *texID){

	// texture dimensions and data buffer
	bool success = true;
	GLint width, height, components;
	GLenum format;
	GLbyte *pBytes;

	string loadIt = "Textures/";
	loadIt = loadIt.append(fname);

	// load file - using GLTools library
    pBytes = gltReadTGABits(loadIt.c_str(), &width, &height, &components, &format);
	if (pBytes == NULL){
		pBytes = gltReadTGABits("Textures/e.tga", &width, &height, &components, &format);
		success = false;
	}

	// bind texture and set parameters
	glBindTexture(GL_TEXTURE_2D, *texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D,0,components,width, height, 0,
		format, GL_UNSIGNED_BYTE, pBytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	// texture loaded, free the temporary buffer
	free(pBytes);
	if(!success){
		cout << "Texture " << fname << " NOT loaded!\nReplacement Texture loaded instead." << endl;
	}else{
		cout << "Texture " << fname << " loaded" << endl;
	}
}
void loaders::cubemap(char **files,GLuint *cubeTexture){

	GLenum  cube[6] = {  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                     GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                     GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                     GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                     GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                     GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

	GLbyte *pBytes;
    GLint iWidth, iHeight, iComponents;
    GLenum eFormat;
    int i;
       
    // Cull backs of polygons
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
        
    glGenTextures(1, cubeTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, *cubeTexture);
        
    // Set up texture maps        
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);       
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
	// Load Cube Map images
    for(i = 0; i < 6; i++)
        {        
        // Load this texture map
        pBytes = gltReadTGABits(files[i], &iWidth, &iHeight, &iComponents, &eFormat);
        glTexImage2D(cube[i], 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes);
		if(pBytes==NULL){
			cerr << "Something went wrong loading texture: " << files[i] << endl;
			MessageBox(NULL, "Skybox textures did not load correctly!\nGame's a bogey!", "Fail!", MB_OK);
			glutLeaveMainLoop();
		}
        free(pBytes);
        }
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	cout << "Skybox textures loaded." << endl;
}
void loaders::normGen(M3DVector3f v[3], M3DVector3f normal)				// Calculates Normal For A triangle Using 3 Points
{
	M3DVector3f v1,v2;			
		
	v1[0] = v[0][0] - v[1][0];				
	v1[1] = v[0][1] - v[1][1];				
	v1[2] = v[0][2] - v[1][2];
	
	v2[0] = v[1][0] - v[2][0];					
	v2[1] = v[1][1] - v[2][1];					
	v2[2] = v[1][2] - v[2][2];

	m3dCrossProduct3(normal,v1,v2);
	m3dNormalizeVector3(normal);
}
void loaders::normGen(float *v[3], float* normal)				// Calculates Normal For A triangle Using 3 Points
{
	M3DVector3f v1,v2;			
		
	v1[0] = v[0][0] - v[1][0];				
	v1[1] = v[0][1] - v[1][1];				
	v1[2] = v[0][2] - v[1][2];
	
	v2[0] = v[1][0] - v[2][0];					
	v2[1] = v[1][1] - v[2][1];					
	v2[2] = v[1][2] - v[2][2];

	m3dCrossProduct3(normal,v1,v2);
	m3dNormalizeVector3(normal);
}
void loaders::uwsm(char *fname, GLTriangleBatch *obj){
	
	string loadIt = "Models/UWSM/";
	loadIt = loadIt.append(fname);

	ifstream::pos_type size;
	char * memblock;

	ifstream file (loadIt.c_str(), ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char [size];
		file.seekg (0, ios::beg);
		file.read (memblock, size);
		file.close();
	}
	else
	{	// Creates empty triangle mesh if file fails to load
		cerr << "Unable to open file.\nLoading Empty mesh instead of " << fname << endl;
		obj->BeginMesh(0);
		obj->End();
		return;
	}
	
	stringstream strstream(memblock); 	// Copy file into a stringstream

	string magicnumber;					// Basic check: is this a uwsm file?
	strstream >> magicnumber;
	if (magicnumber.compare(0,4,"uwsm")!=0)
	{	// could also use the version number, but not doing that just now
		cerr << "File " << fname << " is not a uwsm file.\nLoading Empty mesh." << endl;
		obj->BeginMesh(0);
		obj->End();
		return;
	}

	// Read geometry
	int i, count; //count is number of floats, ie verts x 3
	strstream >> count;
	M3DVector3f *vertices = new M3DVector3f[count/3];
	for (i=0;i<count/3;i++)
		strstream >> vertices[i][0] >> vertices[i][1] >> vertices[i][2];

	// Read normals
	strstream >> count; // how many normals? normally will be same as vertices
	M3DVector3f *normals = new M3DVector3f[count/3];
		for (i=0;i<count/3;i++)
			strstream >> normals[i][0] >> normals[i][1] >> normals[i][2];	

	// Read texture coordinates
	strstream >> count; // how many texture coords? need not be same as vertices
	M3DVector2f *uv = new M3DVector2f[count/2];
	if (count > 0)	// model has UV coordinates
		for (i=0;i<count/2;i++)
			strstream >> uv[i][0] >> uv[i][1]; 
	
	int triangles;
	strstream >> triangles; // how many *triangles* in mesh?

	// Add data to GLTools TriangleBatch object
	// GLTriangleBatch will remove identical vertices, and free any unused memory

	obj->BeginMesh(triangles*3); 
	int v0, v1, v2, t0, t1, t2;
	M3DVector3f triangleVerts[3], triangleNorms[3]; 
	M3DVector2f triangleTex[3];
	for (i=0;i<triangles;i++)
	{
		// vertex & texture indices for one triangle
		strstream >> v0 >> t0 >> v1 >> t1 >> v2 >> t2;
		m3dCopyVector3(triangleVerts[0],vertices[v0]);
		m3dCopyVector3(triangleVerts[1],vertices[v1]);
		m3dCopyVector3(triangleVerts[2],vertices[v2]);

		m3dCopyVector3(triangleNorms[0],normals[v0]);
		m3dCopyVector3(triangleNorms[1],normals[v1]);
		m3dCopyVector3(triangleNorms[2],normals[v2]);

		m3dCopyVector2(triangleTex[0],uv[t0]);
		m3dCopyVector2(triangleTex[1],uv[t1]);
		m3dCopyVector2(triangleTex[2],uv[t2]);
		obj->AddTriangle(triangleVerts,triangleNorms,triangleTex);
	}

	obj->End();

	delete [] memblock;
	delete [] vertices; 
	delete [] normals;
	delete [] uv;
	cout << "Model " << fname << " loaded." << endl;
}
void loaders::uwsm(char *fname, MyTBatch *obj,Vec3 *cmax, Vec3 *cmin){
	
	ifstream::pos_type size;
	char * memblock;

	string loadIt = "Models/UWSM/";
	loadIt = loadIt.append(fname);
	loadIt.append(".uwsm");

	ifstream file (loadIt.c_str(), ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char [size];
		file.seekg (0, ios::beg);
		file.read (memblock, size);
		file.close();
	}
	else
	{	// Creates empty triangle mesh if file fails to load
		cerr << "Unable to open file.\nLoading Empty mesh instead of " << fname << endl;
		obj->BeginMesh(0);
		obj->End();
		return;
	}
	
	stringstream strstream(memblock); 	// Copy file into a stringstream

	string magicnumber;					// Basic check: is this a uwsm file?
	strstream >> magicnumber;
	if (magicnumber.compare(0,4,"uwsm")!=0)
	{	// could also use the version number, but not doing that just now
		cerr << "File " << fname << " is not a uwsm file.\nLoading Empty mesh." << endl;
		obj->BeginMesh(0);
		obj->End();
		return;
	}

	// Read geometry
	int i, count; //count is number of floats, ie verts x 3
	strstream >> count;
	M3DVector3f *vertices = new M3DVector3f[count/3];
	for (i=0;i<count/3;i++){
		strstream >> vertices[i][0] >> vertices[i][1] >> vertices[i][2];

	}
	// Read normals
	strstream >> count; // how many normals? normally will be same as vertices
	M3DVector3f *normals = new M3DVector3f[count/3];
		for (i=0;i<count/3;i++){
			strstream >> normals[i][0] >> normals[i][1] >> normals[i][2];	

		}
	// Read texture coordinates
	strstream >> count; // how many texture coords? need not be same as vertices
	M3DVector2f *uv = new M3DVector2f[count/2];
	if (count > 0)	// model has UV coordinates
		for (i=0;i<count/2;i++)
			strstream >> uv[i][0] >> uv[i][1];
	
	int triangles;
	strstream >> triangles; // how many *triangles* in mesh?

	obj->BeginMesh(triangles*3); 
	int v0, v1, v2, t0, t1, t2;
	M3DVector3f triangleVerts[3], triangleNorms[3]; 
	M3DVector2f triangleTex[3];
	for (i=0;i<triangles;i++)
	{
		// vertex & texture indices for one triangle
		strstream >> v0 >> t0 >> v1 >> t1 >> v2 >> t2;

		m3dCopyVector3(triangleVerts[0],vertices[v0]);
		m3dCopyVector3(triangleVerts[1],vertices[v1]);
		m3dCopyVector3(triangleVerts[2],vertices[v2]);

		m3dCopyVector3(triangleNorms[0],normals[v0]);
		m3dCopyVector3(triangleNorms[1],normals[v1]);
		m3dCopyVector3(triangleNorms[2],normals[v2]);

		m3dCopyVector2(triangleTex[0],uv[t0]);
		m3dCopyVector2(triangleTex[1],uv[t1]);
		m3dCopyVector2(triangleTex[2],uv[t2]);
		obj->AddTriangle(triangleVerts,triangleNorms,triangleTex);
	}

	obj->End();

	delete [] memblock;
	delete [] vertices; 
	delete [] normals;
	delete [] uv;
	cout << "Model " << fname << " loaded." << endl;
}
int loaders::uwsmMultiCheck(char *fname){

	int numParts=0;

	ifstream::pos_type size;
	char * memblock = NULL;
	

	string loadIt = "./Models/UWSM/Multipart/";
	loadIt = loadIt.append(fname);
	loadIt = loadIt.append("/");
	loadIt = loadIt.append(fname);
	loadIt = loadIt.append(".multi");

	ifstream file (loadIt.c_str(), ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char [size];
		file.seekg (0, ios::beg);
		file.read (memblock, size);
		file.close();
		cout << "Checking " << fname << " for list of files...";

		stringstream strstream(memblock); 	// Copy file into a stringstream

		string check;					// Basic check: is this a uwsm file?
		

		strstream >> check;
		if (check.compare(0,9,"multiUWSM")!=0)
		{	
			cout << endl;
		}else{
			strstream >> numParts;
			cout << "Success!" << endl;
		}
		delete[] memblock;
	}
	else
	{	// Creates empty triangle mesh if file fails to load
		cerr  << endl << "Unable to open list of files.\nLoading Empty mesh instead of " << fname << endl;
	}
	
	
	return numParts;
}
void loaders::uwsmMultiLoad(char *fname, MyTBatch *obj,Vec3 *cmax, Vec3 *cmin){
	ifstream::pos_type size;
	char * memblock;

	string loadIt = "./Models/UWSM/Multipart/";
	loadIt = loadIt.append(fname);
	loadIt = loadIt.append("/");
	string name = loadIt;
	string nameTmp = name;
	loadIt = loadIt.append(fname);
	loadIt = loadIt.append(".multi");

	ifstream file (loadIt.c_str(), ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char [size];
		file.seekg (0, ios::beg);
		file.read (memblock, size);
		file.close();
		cout << "Loading individual meshes..." << endl;

		stringstream strstream(memblock);

		string temp;
		int number;
		string filename;
		//char *name;
		
		strstream >> temp >> number;
	
		for(int i=0;i<number;i++){
			strstream >> filename;
			//name = new char[filename.length()];
			//strcpy(name,filename.c_str());
			nameTmp.append(filename);
			cout << "     ";
			loaders::uwsmComponent(nameTmp.c_str(),&obj[i],cmax,cmin);
			nameTmp = name;
		}
		delete[] memblock;
	
	}
	else
	{	// Creates empty triangle mesh if file fails to load
		cerr << "Unable to open list of bodyparts.\nLoading Empty mesh instead of " << fname << endl;
	}
	
	//delete[] name;
	
}
void loaders::uwsmComponent(const char *fname, MyTBatch *obj,Vec3 *cmax, Vec3 *cmin){
	
	ifstream::pos_type size;
	char * memblock;

	string loadIt = "";
	loadIt = loadIt.append(fname);

	ifstream file (loadIt.c_str(), ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char [size];
		file.seekg (0, ios::beg);
		file.read (memblock, size);
		file.close();
	}
	else
	{	// Creates empty triangle mesh if file fails to load
		cerr << "Unable to open file.\nLoading Empty mesh instead of " << fname << endl;
		obj->BeginMesh(0);
		obj->End();
		return;
	}
	
	stringstream strstream(memblock); 	// Copy file into a stringstream

	string magicnumber;					// Basic check: is this a uwsm file?
	strstream >> magicnumber;
	if (magicnumber.compare(0,4,"uwsm")!=0)
	{	// could also use the version number, but not doing that just now
		cerr << "File " << fname << " is not a uwsm file.\nLoading Empty mesh." << endl;
		obj->BeginMesh(0);
		obj->End();
		return;
	}

	// Read geometry
	int i, count; //count is number of floats, ie verts x 3
	strstream >> count;
	M3DVector3f *vertices = new M3DVector3f[count/3];
	for (i=0;i<count/3;i++){
		strstream >> vertices[i][0] >> vertices[i][1] >> vertices[i][2];
		if(vertices[i][0] > cmax->x){cmax->x = (double) vertices[i][0];}
		if(vertices[i][1] > cmax->y){cmax->y = (double)vertices[i][1];}
		if(vertices[i][2] > cmax->z){cmax->z = (double)vertices[i][2];}
		if(vertices[i][0] < cmin->x){cmin->x = (double)vertices[i][0];}
		if(vertices[i][1] < cmin->y){cmin->y = (double)vertices[i][1];}
		if(vertices[i][2] < cmin->z){cmin->z = (double)vertices[i][2];}
	}
	// Read normals
	strstream >> count; // how many normals? normally will be same as vertices
	M3DVector3f *normals = new M3DVector3f[count/3];
		for (i=0;i<count/3;i++){
			strstream >> normals[i][0] >> normals[i][1] >> normals[i][2];	

		}
	// Read texture coordinates
	strstream >> count; // how many texture coords? need not be same as vertices
	M3DVector2f *uv = new M3DVector2f[count/2];
	if (count > 0)	// model has UV coordinates
		for (i=0;i<count/2;i++)
			strstream >> uv[i][0] >> uv[i][1];
	
	int triangles;
	strstream >> triangles; // how many *triangles* in mesh?

	obj->BeginMesh(triangles*3); 
	int v0, v1, v2, t0, t1, t2;
	M3DVector3f triangleVerts[3], triangleNorms[3]; 
	M3DVector2f triangleTex[3];
	for (i=0;i<triangles;i++)
	{
		// vertex & texture indices for one triangle
		strstream >> v0 >> t0 >> v1 >> t1 >> v2 >> t2;

		m3dCopyVector3(triangleVerts[0],vertices[v0]);
		m3dCopyVector3(triangleVerts[1],vertices[v1]);
		m3dCopyVector3(triangleVerts[2],vertices[v2]);

		m3dCopyVector3(triangleNorms[0],normals[v0]);
		m3dCopyVector3(triangleNorms[1],normals[v1]);
		m3dCopyVector3(triangleNorms[2],normals[v2]);

		m3dCopyVector2(triangleTex[0],uv[t0]);
		m3dCopyVector2(triangleTex[1],uv[t1]);
		m3dCopyVector2(triangleTex[2],uv[t2]);
		obj->AddTriangle(triangleVerts,triangleNorms,triangleTex);
	}

	obj->End();

	delete [] memblock;
	delete [] vertices; 
	delete [] normals;
	delete [] uv;
	cout << "Model component " << fname << " loaded." << endl;
}

void loaders::overlay(char *fname,GLuint *texID){


	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;

	string loadIt = "Textures/Overlays/";
	loadIt = loadIt.append(fname);
	
	// Read the texture bits
	pBits = gltReadTGABits(loadIt.c_str(), &nWidth, &nHeight, &nComponents, &eFormat);
	if(pBits == NULL) 
	{
		cerr << "Something went wrong loading texture " << fname << endl;
		MessageBox(NULL, "An overlay did not load correctly!\nCheck console for details!", "Fail!", MB_OK | MB_ICONERROR);
	}else{
	
	glBindTexture(GL_TEXTURE_RECTANGLE,*texID);

	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, nComponents, nWidth, nHeight, 0,
				 eFormat, GL_UNSIGNED_BYTE, pBits);
	}
    free(pBits);
	cout << "Overlay " << fname << " loaded." << endl;

}
void *loaders::sound(char *fname){
	
	FILE *fp;
	int len;
	void *data;

	string loadIt = "Sounds/";
	loadIt = loadIt.append(fname);

	fp = fopen(loadIt.c_str(),"rb");
	fseek(fp,0,SEEK_END);
	len = ftell(fp);
	data = malloc(len);
	rewind(fp);
	fread(data,len,1,fp);
	fclose(fp);
	return data;
}

Vec3 loaders::normGen(Vec3 v[3]){
	M3DVector3f v1,v2,normal;			
	Vec3 rNorm;

	v1[0] = v[0].x - v[1].x;				
	v1[1] = v[0].y - v[1].y;				
	v1[2] = v[0].z - v[1].z;
	
	v2[0] = v[1].x - v[2].x;					
	v2[1] = v[1].y - v[2].y;					
	v2[2] = v[1].z - v[2].z;

	m3dCrossProduct3(normal,v1,v2);
	m3dNormalizeVector3(normal);

	rNorm.x = normal[0];
	rNorm.y = normal[1];
	rNorm.z = normal[2];
	return rNorm;
}