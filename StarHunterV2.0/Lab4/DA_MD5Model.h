
#ifndef DA_MD5MODEL_H
#define DA_MD5MODEL_H
#include "Quaternion.h"
#include "MyTBatch.h"
#include <vector>
#include <string>
#include <StopWatch.h>
////#include <GL/GL.h>
//#include <GL/freeglut.h>

//#define GL_CLIENT_VERTEX_ARRAY_BIT 0x2;


namespace DA
{
	class DA_MD5Model
	{
	protected:

		//	a nice enum to set up some tokeniser values
		enum TOKENS {	TOKEN_KEYWORD,
						TOKEN_INT,
						TOKEN_FLOAT,
						TOKEN_STRING,
						TOKEN_LBRACE,
						TOKEN_RBRACE,
						TOKEN_LPAREN,
						TOKEN_RPAREN,
						TOKEN_INVALID 		};
	public:
		
		DA_MD5Model(void);		//	base constructor
		
		GLfloat			*tris;
		MyTBatch		*a;
		GLuint			vao;
		GLuint			vbo[4];// position, normal, texture, faces(tris);

		~DA_MD5Model(void);		//	base destructor

/*
it occurs to me we need some extra classes or structs to start using the md5 format so here goes . . .

and I will try to keep all names self-evident in purpose
*/

	//	skeletal animation relies on joint in the skeleton, otherwise its skeletal statuary :^D
	class Joint {
	public:
		std::string name;
		float tridpos[3];
		Quaternion quat;
		int parent;
		std::vector<int> children;
	};
	//	now we have a joint, may just need to know something about it, so I can use it
	class JointInformation {
	public:
		std::string name;
		int parentIndex;
		int flags;
		int startIndex;
	};

	//	well, all animations have to be made up of something so frames will do
	class Frame 
	{
		public:
		std::vector<float> animatedSections;
		std::vector<Joint> joints;
		float min[3]; // min point of bounding box
		float max[3]; // max point of bounding box
	};

	//	somewhere to read animation info into
	class Animation
	{
	public:
		Animation();								//	constructor
		//~Animation();								//	destructor
		int nframes;								//	frame count variable
		int nframeRate;								//	frame rate for the animation
		int nAnimatedSections;					//	how many animated sections?
		std::vector<Frame> frames;
		std::vector<Joint> baseJoints;
		std::vector<JointInformation> jointInfo;
	};

	//	before we try building anything need lots of triangles to compose it with
	class Triangle 
	{
	/*	if this class gets any more complicated i will scream!!!!
		still, i havent used a single struct yet . . . .
	*/
	public:
		int v[3]; // vertex indices
	};

	//	joints need weighting for an md5 model so here goes
	class Weight
	{
		//	and yes, yet another grand old complicated class, but its to do with encapsulation ;^)
	public:
		int joint;
		float w;
		float pos[3];
	};

	//	before i make meshes i may well need weighted vertices to use so here goes
	class Vertex 
	{
	public:
		int weightIndex;
		int weightCount;
		GLfloat pos[3];
		GLfloat tc[2];
		GLfloat normal[3];
	};

	//	now that i have weighting, can get going with a mesh:)
	class Mesh 
	{
	public:
		std::string texture;
		std::vector<Vertex> verts;
		std::vector<Triangle> triangles;
		std::vector<Weight> weights;
		GLfloat		*vert;
		GLfloat		*norm;
		GLfloat		*texc;
		GLushort	*tris;
	};

		//	need to set up some storage areas for the md5 class
		int numJoints;
		int numMeshes;
		int currAnim;
		int currFrame;
		float animTime;
		Frame interpolationFrame; // used to interpolate between frames
		std::string textureName;		
		//	some vector based storage too for control and ease of use
		std::vector<Mesh*> meshes;
		std::vector<Joint> joints;
		std::vector<Animation*> animations;

		//	need some handy functions to let us use an md5 Model as more than a set of numbers in memory
		void clear();
		void loadMesh(const char *filename);
		void render();
		
		
		/*	an exception handler would be very nice so the program shouldnt crash at runtime
			however as we know the best laid plans of men and mice . . .
		*/
	class Exception //: public std::runtime_error
	{
	public:
		Exception(){
			std::cerr << "Unknown Error." << std::endl;
			MessageBox(NULL, "Something's gone wrong...", "Unknown Error!", MB_OK | MB_ICONERROR);
		}
		Exception(const std::string &msg){
			std::cerr << msg << std::endl;
			LPCSTR s = msg.c_str();
			MessageBox(NULL, s, "Fail!", MB_OK | MB_ICONERROR);
		}
	};

		/*
			right, onto the workhorse methods
		*/
	//	generator and build functions
		Quaternion computeW(float x, float y, float z) const;
		void genVertices();
		void genVertices(Frame &frame);
		void computeNormals();
		void computeFrames(Animation &anim);
		

	//	text handling methods
		//	skipper functions
		void skipBlanks(std::ifstream &fin);					//	skips empty spaces
		void skipComments(std::ifstream &fin);					//	skip over any comments in the MD5 file
		void skipWhitespaces(std::ifstream &fin);				//	parse through text and skip white space in the file

		//	read functions
		void	readVector(std::ifstream &fin, float *v, int num);		//	reads in a vector composef of num number of floats
		float	readFloat(std::ifstream &fin);							//	read in a float (or an int to be used as a float)
		int		readInt(std::ifstream &fin);							//	read in an int value from the file
		void	readString(std::ifstream &fin, std::string &str);		//	parse through the text and read in a string from file
		//	element handling read functions
		void	readElements(std::ifstream &fin);					//	read elements in from the file (generic)
		void	readCommandLine(std::ifstream &fin);				//	read in a command-line element
		void	readNumJoints(std::ifstream &fin);					//	read in the number of joint elements
		void	readNumMeshes(std::ifstream &fin);					//	read in the number of mesh elements
		void	readJoints(std::ifstream &fin);						//	read in a joint element
		void	readMesh(std::ifstream &fin);						//	read in a mesh element

		//	animation functions
		void  readAnimElements(std::ifstream &fin, Animation &anim);			//	read the elements of the animation
		void  readNumAnimatedComponents(std::ifstream &fin, Animation &anim);	//	read in animated components
		void  readFrameRate(std::ifstream &fin, Animation &anim);				//	read in the framerate of the animation
		void  readFrame(std::ifstream &fin, Animation &anim);					//	read in frame data
		void  readNumFrames(std::ifstream &fin, Animation &anim);				//	read in the number of frames
		void  readHierarchy(std::ifstream &fin, Animation &anim);				//	read in hierarchy info from file
		void  readBaseframe(std::ifstream &fin, Animation &anim);				//	read in the base frame for the animation
		void  readBounds(std::ifstream &fin, Animation &anim);					//	read in the bounds of the animation cycle
		int  loadAnim(const char *filename);									//	load in the animation from file
		void setAnim(int animIndex, int frameIndex = 0);						//	set the current animation
		void setFrame(int frameIndex);											//	set the current frame
		void animate(float dt);													//	animate the model
		// how many animations does this model have??  handily vectors can report on their size:)
		inline int countAnims() const { return int(animations.size()); }

		//	token handler function
		TOKENS getNextToken(std::ifstream &fin, std::string *tokStr=NULL);	//	parse through the text looking for tokenisation items
		
		


	};
}

#endif