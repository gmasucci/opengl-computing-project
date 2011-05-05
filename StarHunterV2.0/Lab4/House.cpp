#include "House.h"


House::House(int mode,int numhouses,GLuint *tex,Terrain *map,GLGeometryTransform *pGLGTin,Camera *camIn)
{
	clones = numhouses;
	houseFrame = new GLFrame[clones];

	char* fname = "house";
	this->multi = true;
	numParts = Loaders::uwsmMultiCheck(fname);
	mesh = new MyTBatch[numParts];
	Vec3 tmax,tmin;
	Loaders::uwsmMultiLoad(fname,mesh,&tmax,&tmin);

	this->max = tmax;
	this->min = tmin;

	theTerrain = map;
	this->pGLGT = pGLGTin;
	this->myTex = tex;
	this->scaleValue=1.0f;
	this->radius = 0.4f;
	this->ptype = 0;
	this->theCam = camIn;
	this->mode = mode;
	myS = new Shader(camIn,"Shaders/nospec.vert","Shaders/nospec.frag",&myTex[0]);

	m3dRotationMatrix44(mx4,D2R*90,0.0,1.0,0.0);
	m3dExtractRotationMatrix33(mx3,mx4);

	setup();
}

void House::setup(){
	float tmp;
	switch(mode){
	case 0:
		//Rotate max,min collision points by rotation matrix.
		M3DVector3f vmax,vmin,outmax,outmin;
		max.toM3D(vmax);
		min.toM3D(vmin);
		m3dRotateVector(outmax,vmax,mx3);
		m3dRotateVector(outmin,vmin,mx3);
		max.fromM3D(outmax);
		min.fromM3D(outmin);
		//and then correct them.
		tmp = max.z;
		max.z = min.z;
		min.z = tmp;
				

		for(int i = 0;i<clones;i++){

				M3DVector3f a;
				a[0] = 30 + (i*(max.z - min.z) *1.5);
				a[2] = 30;
				a[1] = theTerrain->getHeightAt(a[0],a[2]);
				houseFrame[i].SetOrigin(a);
		}

	break;
	case 1:
		for(int i = 0;i<clones;i++){
			
			if(i==3)
			{}else{
				M3DVector3f a;
				a[0] = 116;
				a[2] = 30 + (i*(max.z - min.z) *1.5);
				a[1] = theTerrain->getHeightAt(a[0],a[2]);
				houseFrame[i].SetOrigin(a);
				//houseFrame[i].RotateLocalY(D2R*180);
			}
		}
	default:
		break;
	}
	
}



void House::render(GLMatrixStack *pMVM)
{
	for(int j=0;j<clones;j++){
		pMVM->PushMatrix();
		pMVM->MultMatrix(houseFrame[j]);

		pMVM->Scale(scaleValue,scaleValue,scaleValue);

		if(mode==0)
			pMVM->MultMatrix(mx4);
		

		if(multi){
			for(int i=0;i<numParts;i++){
				myS->useTex(&myTex[i]);
				myS->bind(pMVM,pGLGT);
				mesh[i].Draw();	
			}
		}else{
			myS->bind(pMVM,pGLGT);
			mesh->Draw();	
		}
			
		pMVM->PopMatrix();
	}
		
}
void House::getColInfo(Vec3 *cmax,Vec3 *cmin){
	cmax->x = (max.x*scaleValue);
	cmax->y = (max.y*scaleValue);
	cmax->z = (max.z*scaleValue);
	
	cmin->x = (min.x*scaleValue);
	cmin->y = (min.y*scaleValue);
	cmin->z = (min.z*scaleValue);
}
Vec3 House::getColBox(int obj){
	
	Vec3 v;
	M3DVector3f mv;
	houseFrame[obj].GetOrigin(mv);
	v.fromM3D(mv);
	return v;

}

House::~House(void)
{
}
