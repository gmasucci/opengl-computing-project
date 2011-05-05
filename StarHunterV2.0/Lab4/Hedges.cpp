#include "Hedges.h"


Hedges::Hedges(int mode,int numhedges,GLuint *tex,Terrain *map,GLGeometryTransform *pGLGTin,Camera *camIn)
{
	clones = numhedges;
	hedgeFrame = new GLFrame[clones];

	char* fname = "hedge";
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

void Hedges::setup(){
	float tmp;
	switch(mode){
	case 0:
		//Rotate max,min points by rotation matrix.
		M3DVector3f vmax,vmin,outmax,outmin;
		max.toM3D(vmax);
		min.toM3D(vmin);
		m3dRotateVector(outmax,vmax,mx3);
		m3dRotateVector(outmin,vmin,mx3);
		max.fromM3D(outmax);
		min.fromM3D(outmin);
		//tmp = max.x;
		//max.x = min.x;
		//min.x = tmp;

		tmp = max.z;
		max.z = min.z;
		min.z = tmp;
				

		for(int i = 0;i<clones;i++){
			if(i==15){
			}else{
				M3DVector3f a;
				a[0] = 126;
				a[2] = 24 + (i*3.07);
				a[1] = theTerrain->getHeightAt(a[0],a[2]);
				hedgeFrame[i].SetOrigin(a);
			}
		}
	break;
	case 1:
		for(int i = 0;i<clones;i++){
			if(i==55){
			}else{
				M3DVector3f a;
				a[0] = 30 + (i*3.07);
				a[2] = 118;
				a[1] = theTerrain->getHeightAt(a[0],a[2]);
				hedgeFrame[i].SetOrigin(a);
			}
		}
	default:
		break;
	}
	
}



void Hedges::render(GLMatrixStack *pMVM)
{
	for(int j=0;j<clones;j++){
		pMVM->PushMatrix();
		pMVM->MultMatrix(hedgeFrame[j]);

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
void Hedges::getColInfo(Vec3 *cmax,Vec3 *cmin){
	cmax->x = (max.x*scaleValue);
	cmax->y = (max.y*scaleValue);
	cmax->z = (max.z*scaleValue);
	
	cmin->x = (min.x*scaleValue);
	cmin->y = (min.y*scaleValue);
	cmin->z = (min.z*scaleValue);
}
Vec3 Hedges::getColBox(int obj){
	
	Vec3 v;
	M3DVector3f mv;
	hedgeFrame[obj].GetOrigin(mv);
	v.fromM3D(mv);
	return v;


}

Hedges::~Hedges(void)
{
}
