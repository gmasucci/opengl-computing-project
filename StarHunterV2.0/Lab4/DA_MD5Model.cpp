#include "DA_MD5Model.h"



//	a quick define will help to detect empty space
#define EMPTYSPACE(c) (' ' == c || '\t' == c || '\r' ==c || '\n' == c )


//	take care of all the constructors first
DA::DA_MD5Model::Animation::Animation()
{
	nframes				=	0	;
	nframeRate			=	0	;
	nAnimatedSections	=	0	;
}

DA::DA_MD5Model::DA_MD5Model(void)
{	
	numJoints	=	0		;
	numMeshes	=	0		;
	currAnim	=	-1		;
	currFrame	=	0		;
	animTime	=	0.0f	;
	glGenVertexArrays(1,&vao);
	glGenBuffers(4,vbo);
	
}

void DA::DA_MD5Model::clear()
{
	//	delete the meshes . . . 
	for ( size_t i=0; i < meshes.size(); i++ )
		if ( meshes[i] )
			delete meshes[i];
	//	and animations . . . 
	for ( size_t i=0; i < animations.size(); i++ )
		if ( animations[i] )
			delete animations[i];
	//	now we can clear the vectors for the above
	meshes.clear();
	animations.clear();
	joints.clear();
}
DA::DA_MD5Model::~DA_MD5Model(void)
{

}

void DA::DA_MD5Model::computeNormals()
{
	  // zero out the normals
  for ( size_t i=0; i < meshes.size(); i++) {


    // for each normal, add contribution to normal from every face that vertex
    // is part of
    for ( size_t j=0; j < meshes[i]->triangles.size(); j++ ) {
      Vertex &v0 = meshes[i]->verts[ meshes[i]->triangles[j].v[0] ];
      Vertex &v1 = meshes[i]->verts[ meshes[i]->triangles[j].v[1] ];
      Vertex &v2 = meshes[i]->verts[ meshes[i]->triangles[j].v[2] ];

      float Ax = v0.pos[0] - v1.pos[0];
      float Ay = v0.pos[1] - v1.pos[1];
      float Az = v0.pos[2] - v1.pos[2];

      float Bx = v2.pos[0] - v1.pos[0];
      float By = v2.pos[1] - v1.pos[1];
      float Bz = v2.pos[2] - v1.pos[2];

//			FROM T'INTERNET:
//			cross ( v1,  v2) = [Ay*Bz - By*Az , 
//								Az*Bx - Bz*Ax ,
//								Ax*By - Bx*Ay ]

      float nx = Ay * Bz - By * Az;
      float ny = Az * Bx - Bz * Ax;
      float nz = Ax * By - Bx * Ay;

      v0.normal[0] += nx;
      v0.normal[1] += ny;
      v0.normal[2] += nz;

      v1.normal[0] += nx;
      v1.normal[1] += ny;
      v1.normal[2] += nz;

      v2.normal[0] += nx;
      v2.normal[1] += ny;
      v2.normal[2] += nz;
    }

    // normalize each normal
    for ( size_t j=0; j < meshes[i]->verts.size(); j++ ) {
      Vertex &v = meshes[i]->verts[j];

      float mag = (float)sqrt( float(v.normal[0]*v.normal[0] + v.normal[1]*v.normal[1] + v.normal[2]*v.normal[2]) );

      // avoid division by zero
      if ( mag > 0.0001f ) {
        v.normal[0] /= mag;
        v.normal[1] /= mag;
        v.normal[2] /= mag;
      }
    }
  } // for (meshes)
}

void DA::DA_MD5Model::genVertices()
{
	  for ( size_t i=0; i < meshes.size(); i++ ) {
    for ( size_t j=0; j < meshes[i]->verts.size(); j++ ) {
      Vertex &v = meshes[i]->verts[j];
      v.pos[0] = v.pos[1] = v.pos[2] = 0.0;
      
      for ( int k=0; k < v.weightCount; k++ ) {
        Weight &w = meshes[i]->weights[v.weightIndex + k];
        Joint &joint = joints[w.joint];
        
        Quaternion q(w.pos[0], w.pos[1], w.pos[2], 0.0f);
        Quaternion result = joint.quat*q*joint.quat.conjugate();
        v.pos[0] += (joint.tridpos[0] + result[0])*w.w;
        v.pos[1] += (joint.tridpos[1] + result[1])*w.w;
        v.pos[2] += (joint.tridpos[2] + result[2])*w.w;
      } // for (weights)
    } // for (mesh vertices)
  } // for (meshes)
}

void DA::DA_MD5Model::genVertices(Frame &frame)
{
  for ( size_t i=0; i < meshes.size(); i++ ) {
    for ( size_t j=0; j < meshes[i]->verts.size(); j++ ) {
      Vertex &v = meshes[i]->verts[j];
      v.pos[0] = v.pos[1] = v.pos[2] = 0.0;
      
      for ( int k=0; k < v.weightCount; k++ ) {
        Weight &w = meshes[i]->weights[v.weightIndex + k];
        Joint &joint = frame.joints[w.joint];
        
        Quaternion q(w.pos[0], w.pos[1], w.pos[2], 0.0f);
        Quaternion result = joint.quat*q*joint.quat.conjugate();
        v.pos[0] += (joint.tridpos[0] + result[0])*w.w;
        v.pos[1] += (joint.tridpos[1] + result[1])*w.w;
        v.pos[2] += (joint.tridpos[2] + result[2])*w.w;
      } // for (weights)
    } // for (mesh vertices)
  } // for (meshes)
}

void DA::DA_MD5Model::computeFrames(Animation &anim)
{
	/*
		now this function is pretty much unoptimised as I used it as found 
		(and I cant find the site it was from again either:^(
	*/
	for ( int i=0; i < anim.nframes; i++ ) {
    // allocate storage for joints for this frame
    anim.frames[i].joints.resize(numJoints);

    for ( int j=0; j < numJoints; j++ ) {
      const Joint &joint = anim.baseJoints[j];
  
      float pos[3]    = { joint.tridpos[0],  joint.tridpos[1],  joint.tridpos[2]  };
      float orient[3] = { joint.quat[0], joint.quat[1], joint.quat[2] };
  
      int n = 0;
      for ( int k=0; k < 3; k++ )
        if ( anim.jointInfo[j].flags & (1 << k)  ) {
          pos[k] = anim.frames[i].animatedSections[anim.jointInfo[j].startIndex + n];
          n++;
        }

      for ( int k=0; k < 3; k++ )
        if ( anim.jointInfo[j].flags & (8 << k)  ) {
          orient[k] = anim.frames[i].animatedSections[anim.jointInfo[j].startIndex + n];
          n++;
        }

      Quaternion q = computeW(orient[0], orient[1], orient[2]);

      Joint &frameJoint = anim.frames[i].joints[j];
      frameJoint.name   = anim.jointInfo[j].name;
      frameJoint.parent = anim.jointInfo[j].parentIndex;

      // root joint?
      if ( anim.jointInfo[j].parentIndex < 0 ) {
        frameJoint.tridpos[0] = pos[0];
        frameJoint.tridpos[1] = pos[1];
        frameJoint.tridpos[2] = pos[2];
        frameJoint.quat = q;
      }
      else {
        Joint &parent = anim.frames[i].joints[anim.jointInfo[j].parentIndex];
  
        // rotate position (qp is quaternion representation of position)
        Quaternion qp(pos[0], pos[1], pos[2], 0.0f);
        Quaternion result = parent.quat*qp*parent.quat.conjugate();
  
        frameJoint.tridpos[0] = result[0] + parent.tridpos[0];
        frameJoint.tridpos[1] = result[1] + parent.tridpos[1];
        frameJoint.tridpos[2] = result[2] + parent.tridpos[2];
  
        // store orientation of this joint
        frameJoint.quat = parent.quat*q;
        frameJoint.quat.normalise();
      } // else
    } // for
  } // for
}

DA::Quaternion DA::DA_MD5Model::computeW(float x, float y, float z) const
{
  /*
	this will make a quaternion out of an x,y,z
	user entered value, and calculate the relevant
	w value
  */
  float w = 1.0f - x*x - y*y - z*z;
  w = w < 0.0 ? 0.0f : (float)-sqrt( double(w) );
  //	setup the returnable values
  Quaternion retval(x, y, z, w);
  //	normalise the quaternion
  retval.normalise();
  //	and post it back to sender in its new form
  return retval;
}

//	a method to skip over empty spaces
void DA::DA_MD5Model::skipBlanks(std::ifstream &fin) 
{
  char c = '\0';
  while ( !fin.eof() ) {
    fin.get(c);

    if ( !EMPTYSPACE(c) ) {
      fin.putback(c);
      break;
    }
  }
}

void DA::DA_MD5Model::skipComments(std::ifstream &fin) {
  char c;
  fin.get(c);

  if ( c != '/' )
    throw Exception("skipComments: invalid comment type expectec start with //");

  while ( !fin.eof() && c != '\n' )
    fin.get(c);

  // put back last character read
  fin.putback(c);
}

// reads in sequence of a number of floats enclosed by parentheses
void DA::DA_MD5Model::readVector(std::ifstream &fin, float *v, int number) {
  if ( getNextToken(fin) != TOKEN_LPAREN )
    throw Exception("readVector: expected '('");

  for ( int i=0; i < number; i++ )
    v[i] = readFloat(fin);

  if ( getNextToken(fin) != TOKEN_RPAREN )
    throw Exception("readVector: expected ')'");
}

float DA::DA_MD5Model::readFloat(std::ifstream &fin) {
  std::string str;
  TOKENS t = getNextToken(fin, &str);

  // integer tokens are just numbers with out a decimal point, so they'll
  // suffice here as well
  if ( t != TOKEN_FLOAT && t != TOKEN_INT )
    throw Exception("readFloat: expected float (even an int would have done . . .)");

  float f = 0.0f;
  sscanf(str.c_str(), "%f", &f);

  return f;
}

// reads in next token from file and matches it to a token type,
// if tokStr is non-NULL then it will be set to the text of the token
DA::DA_MD5Model::TOKENS DA::DA_MD5Model::getNextToken(std::ifstream &fin, std::string *tokStr) {
  skipBlanks(fin);
  std::string str;

  TOKENS t = TOKEN_INVALID;

  while ( !fin.eof() ) {
    char c = '\0';
    fin.get(c);

    // single character tokens
    if ( '{' == c || '}' == c || '(' == c || ')' == c ) {
      // if already reading in a token, treat this as a delimiter
      if ( t != TOKEN_INVALID ) {
        fin.putback(c);
        if ( tokStr != NULL )
          (*tokStr) = str;
      }
 
      if ( '{' == c )
        t = TOKEN_LBRACE;
      if ( '}' == c )
        t = TOKEN_RBRACE;
      if ( '(' == c )
        t = TOKEN_LPAREN;
      if ( ')' == c )
        t = TOKEN_RPAREN;

      if ( tokStr) {
        (*tokStr) = std::string();
        (*tokStr) += c;
      }
      return t;
    }
    if ( isdigit(c) ) {
      str += c;
      if ( TOKEN_INVALID == t )
        t = TOKEN_INT;
      else if ( t != TOKEN_INT && t != TOKEN_FLOAT && t != TOKEN_KEYWORD ) {
        std::string msg("getNextToken: token got was invalid '");
        msg += str + "'";
        throw Exception(msg);
      }
    }
    if ( '-' == c ) {
      str += c;
      if ( TOKEN_INVALID == t )
        t = TOKEN_INT;
      else {
        std::string msg("getNextToken: token got was invalid '");
        msg += str + "'";
        throw Exception(msg);
      }
    }
    if ( isalpha(c) ) {
      str += c;
      if ( TOKEN_INVALID == t )
        t = TOKEN_KEYWORD;
      else if ( t != TOKEN_KEYWORD ) {
        std::string msg("getNextToken: token got was invalid '");
        msg += str + "'";
        throw Exception(msg);
      }
    }
    if ( '"' == c ) {
      // treat as a delimeter if already reading in a token
      if ( t != TOKEN_INVALID ) {
        fin.putback(c);
        if ( tokStr != NULL )
          (*tokStr) = str;
        return t;
      }
      readString(fin, str);

      if ( tokStr != NULL )
        (*tokStr) = str;

      return TOKEN_STRING;
    }
    if ( '.' == c ) {
      str += c;
      if ( t != TOKEN_INT ) {
        std::string msg("getNextToken: token got was invalid '");
        msg += str + "'";
        throw Exception(msg);
      }
      t = TOKEN_FLOAT;
    }
    if ( '/' == c ) {
      // treat as a delimeter if already reading in a token
      if ( t != TOKEN_INVALID ) {
        if ( tokStr != NULL )
          (*tokStr) = str;
        return t;
      }
      
      skipComments(fin);
      skipBlanks(fin);
      continue;
    }

    // treat whitespace as a delimeter
    if ( EMPTYSPACE(c) ) {
      if ( tokStr != NULL )
        (*tokStr) = str;
      return t;
    }

    // at this point token type should be set, if it hasn't been then
    // token is invalid
    if ( TOKEN_INVALID == t ) {
        std::string msg("getNextToken: token got was invalid '");
        str += c;
        msg += str + "'";
        throw Exception(msg);
    }
  }

  return TOKEN_INVALID;
}

void DA::DA_MD5Model::readString(std::ifstream &fin, std::string &str) {
  str = std::string();

  // read characters until closing parenthesis (") found
  char c = '\0';
  do {
    fin.get(c);

    if ( fin.eof() )
      throw Exception("readString: reached end of file before the end of the string (\") was found");

    if ( c != '"')
      str += c;
  } while ( c != '"' );
}

// reads in an integer terminal and returns its value
int DA::DA_MD5Model::readInt(std::ifstream &fin) {
  std::string str;
  TOKENS t = getNextToken(fin, &str);

  if ( t != TOKEN_INT )
    throw Exception("readInt: expected integer value . . . You trying to confuse me??");

  return atoi( str.c_str() );
}

void DA::DA_MD5Model::readElements(std::ifstream &fin) {
  while ( !fin.eof() ) {
    std::string str;
    TOKENS tok = getNextToken(fin, &str);
  
    // token is TOKEN_INVALID when end of file is reached
    if ( TOKEN_INVALID == tok )
      break;
  
    if ( str == "commandline" )
      readCommandLine(fin);
    else if ( str == "numJoints" )
      readNumJoints(fin);
    else if ( str == "numMeshes" )
      readNumMeshes(fin);
    else if ( str == "joints" )
      readJoints(fin);
    else if ( str == "mesh" )
      readMesh(fin);
    else {
      // invalid element
      throw Exception( std::string("readElements: What type of element was that meant to be?? '") + str + "'");
    }
  } // while ( not EOF )
}

void DA::DA_MD5Model::readCommandLine(std::ifstream &fin) {
  // commandline elements can be ignored, but make sure the syntax is correct
  if ( getNextToken(fin) != TOKEN_STRING )
    throw Exception("MD5Model::readCommandLineEl(): expected string");
}

void DA::DA_MD5Model::loadMesh(const char *filename) {
  // sanity check
  if ( !filename )
    throw Exception("loadMesh: I'm not a mindreader i need a filename!!");

  std::cout << "Trying to open file...";
  // attempt to open file for reading
  std::ifstream fin(filename, std::ifstream::in);

  // was open successful?
  if ( !fin.is_open() ) {
    std::string msg = std::string("loadMesh: can't open ") +
                      std::string(filename) + std::string(" for reading, is it locked? is it even there??");
    throw Exception(msg);
  }
  std::cout << "Success" << std::endl;
  // read in file version
  std::string str;
  getNextToken(fin, &str);

  // token must read "MD5Version"
  if ( str != "MD5Version" )
    throw Exception("loadMesh: I need to be able to find an 'MD5Version', so where is it?");

  // get version #
  int ver = readInt(fin);
  
  // must be version 10
  if ( ver != 10 )
    throw Exception("loadMesh: MD5Version must be 10, thats the latest version, anything else and its too old.  I, unlike you it seems, keep up with the latest trends.");
  std::cout << "MD5 is version 10. This pleases me." << std::endl;

  // clear any data before reading file
  clear();

  // read in all of the MD5Model elements
  std::cout << "Reading elements from model...";
  readElements(fin);
  std::cout << "Done." << std::endl;
  // close input file (should be done destructor anyway...)
  fin.close();
  // calculate vertex positions and normals from information in joints
  std::cout << "Generating vertices from weights...";
  genVertices();
  std::cout << "Done." << std::endl;
  std::cout << "Computing normals...";
  computeNormals();
  std::cout << "Done." << std::endl;
 
  //a = new GLTriangleBatch[meshes.size()];
  //b = new GLBatch[meshes.size()];
	a = new MyTBatch[meshes.size()];
}

void DA::DA_MD5Model::readMesh(std::ifstream &fin) {
  // make sure numMeshes has been set
  if ( numMeshes <= 0 )
    throw Exception("readMeshes: numMeshes needs to be set before reading the 'mesh' block");

  TOKENS t = getNextToken(fin);

  // expect an opening brace { to begin block of joints
  if ( t != TOKEN_LBRACE )
    throw Exception("readMesh: expected { after the string 'mesh'");

  Mesh *mesh = new Mesh;

  // read in all mesh information in block until '}' is hit
  // (if EOF is reached before this, then the read*() methods
  //  will close the file and throw an exception)
  std::string str;
  t = getNextToken(fin, &str);
  while ( t != TOKEN_RBRACE ) {
    if ( str == "vert" ) {
      // syntax: vert (vertex index) '(' (x) (y) (z) ')' (weight index) (weight count)
      Vertex vert;

      int index = readInt(fin);
      readVector(fin, vert.tc, 2);
      vert.weightIndex = readInt(fin);
      vert.weightCount = readInt(fin);

      // make sure index is within bounds of vector of vertices
      if ( size_t(index) >= mesh->verts.size() )
        throw Exception("readMesh: vertex index must be equal to or greater than the value of numverts");

      mesh->verts[index] = vert;
    }
    else if ( str == "tri" ) {
      // syntax: tri (triangle index) (v0 index) (v1 index) (v2 index)
      Triangle tri;
      
      int index = readInt(fin);

      // make sure index is within bounds of vector of triangles
      if ( size_t(index) >= mesh->triangles.size() )
        throw Exception("readMesh: triangle index must be equal to or greater to the value of numtris");

      tri.v[0] = readInt(fin);
      tri.v[1] = readInt(fin);
      tri.v[2] = readInt(fin);
      mesh->triangles[index] = tri;
    }
    else if ( str == "weight" ) {
      Weight weight;

      int weightIndex = readInt(fin);
      weight.joint = readInt(fin);
      weight.w = readFloat(fin);
      readVector(fin, weight.pos, 3);

      if ( size_t(weightIndex) >= mesh->weights.size() )
        throw Exception("readMesh: weight index must be equal to or greater to the value of numweights");

      mesh->weights[weightIndex] = weight;
    }
    else if ( str == "shader" ) {
      std::string shader;
      if ( getNextToken(fin, &shader) != TOKEN_STRING )
        throw Exception("readMesh: expected a string to follow the string 'shader'");
    }
    else if ( str == "numverts" ) {
      if ( mesh->verts.size() > 0 )
        throw Exception("readMesh: numverts has already been set, you wont be able to bend space by adding more of them");

      int n = readInt(fin);

      if ( n <= 0 )
        throw Exception("readMesh: numverts must be greater than 0");

      mesh->verts.resize(n);
    }
    else if ( str == "numtris" ) {
      if ( mesh->triangles.size() > 0 )
        throw Exception("readMesh: numtris has already been set");

      int n = readInt(fin);

      if ( n <= 0 )
        throw Exception("readMesh: numtris must be greater than 0");

      mesh->triangles.resize(n);
    }
    else if ( str == "numweights" ) {
      if ( mesh->weights.size() > 0 )
        throw Exception("readMesh: numweights has already been set");

      int n = readInt(fin);

      if ( n <= 0 )
        throw Exception("readMesh: numweights must be greater than 0");

      mesh->weights.resize(n);
    }

    // read next token
    t = getNextToken(fin, &str);
  }

  meshes.push_back(mesh);
}

int DA::DA_MD5Model::loadAnim(const char *filename) {
  // attempt to open file for reading
  std::ifstream fin(filename, std::ifstream::in);

  if ( !fin.is_open() ) {
    std::string msg = std::string("loadAnim: can't open ") +
                      std::string(filename) + std::string(" for reading");
    throw Exception(msg);
  }

  // read in file version
  std::string str;
  getNextToken(fin, &str);

  // token must read "MD5Version"
  if ( str != "MD5Version" )
    throw Exception("loadAnim: what's the 'MD5Version' then?? Do I look like a mindreader?");
  
  // get version #
  int ver = readInt(fin);

  // must be version 10
  if ( ver != 10 )
    throw Exception("loadAnim: MD5Version must be 10, or are we goint to use antique models?");

  Animation *anim = new Animation;
  if ( !anim )
    throw Exception("loadAnim: Storage, please!  Cant find storage for the animation.");

  readAnimElements(fin, *anim);

  // close file (should be done by destructor anyway)
  fin.close();

  // add to array of animations
  int animIndex = (int)animations.size();
  animations.push_back(anim);

  // build frames for this animation
  computeFrames(*anim);

  // make this the current animation
  setAnim(animIndex, 0);

  return animIndex;
}

void DA::DA_MD5Model::readNumJoints(std::ifstream &fin) {
  // if number of joints has already been set, can't set it again
  if ( numJoints != 0 )
    throw Exception("readNumJoints: the number of joints is already set, trying to do it again will NOT let you be double-jointed");

  // read in next token, should be an integer
  int n = readInt(fin);

  if ( n <= 0 )
    throw Exception("readNumJoints: you need at least 1 joint for a skeleton, otherwise it would be a bone");

  numJoints =  n;
  //joints.resize(numJoints);
}

void DA::DA_MD5Model::readNumMeshes(std::ifstream &fin) {
  // if number of meshes has already been set, can't set it again
  if ( numMeshes != 0 )
    throw Exception("readNumMeshes: the number of meshes is already set, doint it again will NOT give you a thicker skin");

  // read in next token, should be an integer
  int n = readInt(fin);

  if ( n <= 0 )
    throw Exception("readNumMeshes: you need at least 1 mesh to display, or is everything ethereal??");

  numMeshes =  n;
  //meshes.resize(numMeshes);
}

void DA::DA_MD5Model::readJoints(std::ifstream &fin) {
  // make sure numJoints has been set
  if ( numJoints <= 0 )
    throw Exception("readJoints: how many of them?? use readNumJoints to set it before you try this");

  TOKENS t = getNextToken(fin);

  // expect an opening brace { to begin block of joints
  if ( t != TOKEN_LBRACE )
    throw Exception("readJoints: OK...Where is the { following the 'joints' text??");

  // read in each joint in block until '}' is hit
  // (if EOF is reached before this, then the read*() methods
  //  will close the file and throw an exception)
  std::string str;
  t = getNextToken(fin, &str);
  while ( t != TOKEN_RBRACE ) {
    Joint joint;

    // token should be name of joint (a string)
    if ( t != TOKEN_STRING )
      throw Exception("readJoints: whats the name?? theres no name here!!");

    // read in index of parent joint
    int parentIndex = readInt(fin);
  
    // read in joint position
    readVector(fin, joint.tridpos, 3);
  
    // read in first 3 components of quaternion (must compute 4th)
    float quat[4];
    readVector(fin, quat, 3);

    joint.quat = computeW(quat[0], quat[1], quat[2]);

    // add index to parent's list of child joints (if it has a parent,
    // root joints have parent index -1)
    if ( parentIndex >= 0 ) {
      if ( size_t(parentIndex) >= joints.size() )
        throw Exception("readJoints: you know what helps? parents for joints, an index of 'em is mandatory even...so where are they??");

      joints[parentIndex].children.push_back( int(joints.size()) );
    }

    // add joint to vector of joints
    joints.push_back(joint);

    // get next token
    t = getNextToken(fin, &str);
  }
}

void DA::DA_MD5Model::readNumAnimatedComponents(std::ifstream &fin, Animation &anim) {
  // make sure parameter hasn't been set, as has been done with all of the
  // others
  if ( anim.nAnimatedSections != 0 )
    throw Exception("readNumAnimatedComponents: nAnimatedSections done already, trying to double up your moves are you?");

  // read in next token, should be an integer, if not scream at the user
  int n = readInt(fin);

  if ( n <= 0 )
    throw Exception("readNumAnimatedComponents: nAnimatedSections has to be >0, or theres no animation, see?");

  anim.nAnimatedSections = n;
}

void DA::DA_MD5Model::readAnimElements(std::ifstream &fin, Animation &anim) {
  while ( !fin.eof() ) {
    std::string str;
    TOKENS tok = getNextToken(fin, &str);

    // token is TOKEN_INVALID when end of file is reached
    if ( TOKEN_INVALID == tok )
      break;

    if ( str == "commandline" )
      readCommandLine(fin);
    else if ( str == "numJoints" ) {
      // just make sure that the number of joints is the same as the number
      // specified in the mesh file
      int n = readInt(fin);

      if ( n != numJoints )
        throw Exception("readAnimElements: animation file does not match the specified mesh (diff. number of joints)");
    }
    else if ( str == "numMeshes" ) {
      // just make sure that the number of meshes is the same as the number
      // specified in the mesh file
      int n = readInt(fin);

      if ( n != numMeshes )
        throw Exception("readAnimElements: animation file does not match specified mesh(different number of meshes)");
    }
    else if ( str == "numFrames" )
      readNumFrames(fin, anim);
    else if ( str == "frameRate" )
      readFrameRate(fin, anim);
    else if ( str == "numAnimatedComponents" )
      readNumAnimatedComponents(fin, anim);
    else if ( str == "hierarchy" )
      readHierarchy(fin, anim);
    else if ( str == "bounds" )
      readBounds(fin, anim);
    else if ( str == "baseframe" )
      readBaseframe(fin, anim);
    else if ( str == "frame" )
      readFrame(fin, anim);
    else {
      // invalid element
      throw Exception( std::string("readAnimElements: unknown element type encountered: '") + str + "'");
    }
  }
}

void DA::DA_MD5Model::readFrameRate(std::ifstream &fin, Animation &anim) {
  // if framerate has already been set, can't set it again
  if ( anim.nframeRate != 0 )
    throw Exception("readFrameRate: frameRate already set");

  // read in next token, should be an integer
  int n = readInt(fin);

  if ( n <= 0 )
    throw Exception("readFrameRate: time being what it is, the frameRate must be >0 (or there wont be any frames per second see?)");

  anim.nframeRate =  n;
}

void DA::DA_MD5Model::readNumFrames(std::ifstream &fin, Animation &anim) {
  // if number of frames has already been set, can't set it again
  if ( anim.nframes != 0 )
    throw Exception("readNumFrames: number of frames already set . . . time dilation anyone?");

  // read in next token, should be an integer
  int n = readInt(fin);

  if ( n <= 0 )
    throw Exception("readNumFrames: there must be at least 1 frame . . . or what do we show???");

  anim.nframes =  n;
  anim.frames.resize(n);
}

void DA::DA_MD5Model::readFrame(std::ifstream &fin, Animation &anim) {
  // numAnimatedComponents has to have been set before frame element
  if ( 0 == anim.nAnimatedSections )
    throw Exception("readFrame: nAnimatedSections must be set before 'frame' block");

  // read frame index
  int frameIndex = readInt(fin);

  if ( frameIndex < 0 || frameIndex >= anim.nframes )
    throw Exception("readFrame: invalid frame index");

  // get reference to frame and set number of animated components
  Frame &frame = anim.frames[frameIndex];
  frame.animatedSections.resize(anim.nAnimatedSections);

  TOKENS t = getNextToken(fin);

  // expect an opening brace { to begin block
  if ( t != TOKEN_LBRACE )
    throw Exception("readFrame: expected starting {");

  for ( int i=0; i < anim.nAnimatedSections; i++ )
    frame.animatedSections[i] = readFloat(fin);

  t = getNextToken(fin);

  // expect a closing brace } to end block
  if ( t != TOKEN_RBRACE )
    throw Exception("readFrame: expected ending }");
}

void DA::DA_MD5Model::readHierarchy(std::ifstream &fin, Animation &anim) {
  TOKENS t = getNextToken(fin);

  // expect an opening brace { to begin hierarchy block
  if ( t != TOKEN_LBRACE )
    throw Exception("MD5Model::readHierarchy(): expected { to follow 'hierarchy'");

  anim.jointInfo.clear();

  // read in each joint in block until '}' is hit
  // (if EOF is reached before this, then the read*() methods
  //  will close the file and throw an exception)
  std::string str;
  t = getNextToken(fin, &str);
  while ( t != TOKEN_RBRACE ) {
    JointInformation info;

    // token should be name of a joint (a string)
    if ( t != TOKEN_STRING )
      throw Exception("readHierarchy: expected a string containing the name");

    info.name        = str;
    info.parentIndex = readInt(fin);
    info.flags       = readInt(fin);
    info.startIndex  = readInt(fin);

    anim.jointInfo.push_back(info);
    
    // get next token
    t = getNextToken(fin, &str);
  }

  if ( int(anim.jointInfo.size()) != numJoints )
    throw Exception("readHierarchy: hierarchy entries and the number of joints do not correspond in quantity . . . better go check that");
}

void DA::DA_MD5Model::readBaseframe(std::ifstream &fin, Animation &anim) {
  TOKENS t = getNextToken(fin);

  // expect an opening brace { to begin block
  if ( t != TOKEN_LBRACE )
    throw Exception("readBaseframe: expected starting { after the string 'baseframe'");

  anim.baseJoints.resize(numJoints);

  int i;
  for ( i=0; i < numJoints; i++ ) {
    // read in joint position
    readVector(fin, anim.baseJoints[i].tridpos, 3);

    // read in first 3 components of quaternion (must compute 4th with our handy dany function:^)
    float quat[3];
    readVector(fin, quat, 3);

    anim.baseJoints[i].quat = computeW(quat[0], quat[1], quat[2]);
  }

  if ( i < numJoints - 1 )
    throw Exception("readBaseframe: not enough joints (well, bone fusion might happen . . . just not here)");

  // next token must be a closing brace }
  t = getNextToken(fin);

  if ( TOKEN_LPAREN == t )
    throw Exception("readBaseframe(): too many joints (okay, its had a few broken bones its an abused model)");

  // expect a closing brace } to end block
  if ( t != TOKEN_RBRACE )
    throw Exception("readBaseframe: expected ending }");
}

void DA::DA_MD5Model::readBounds(std::ifstream &fin, Animation &anim) {
  TOKENS t = getNextToken(fin);

  // expect an opening brace { to begin block
  if ( t != TOKEN_LBRACE )
    throw Exception("readBounds: expected a { after string 'bounds'");

  if ( anim.nframes != int(anim.frames.size()) )
    throw Exception("readBounds: how about having some frames before setting bounds of an animation??");

  // read in bound for each frame
  for ( int i=0; i < anim.nframes; i++ )
  {
    readVector(fin, anim.frames[i].min, 3);
    readVector(fin, anim.frames[i].max, 3);
  }

  // next token must be a closing brace }
  t = getNextToken(fin);

  if ( TOKEN_LPAREN == t )
    throw Exception("readBounds: number of bounds greater than the number of frames");

  // expect a closing brace } to end block
  if ( t != TOKEN_RBRACE )
    throw Exception("readBounds: expected a closing } somewhere, sometime, just to match the opening one . . . ");
}

void DA::DA_MD5Model::setAnim(int animIndex, int frameIndex) {
  if ( animIndex < 0 || animIndex >= int(animations.size()) )
    throw Exception("setAnim: pick a value that exists maybe?? works much better that way you know.");

  if ( currAnim != animIndex ) {
    currAnim = animIndex;
    setFrame(frameIndex);
  }
}

void DA::DA_MD5Model::setFrame(int frameIndex) {
  // sanity check #1
  if ( animations.size() == 0 || currAnim < 0 )
    throw Exception("setFrame: no animation has beens set, anything else youd like me to read from your mind?");

  // sanity check #2
  if ( frameIndex < 0 || !animations[currAnim] ||
       animations[currAnim]->nframes <= 0 ||
       animations[currAnim]->nframes <= frameIndex )
    throw Exception("setFrame: frame choice is out of bounds for the index");

  genVertices(animations[currAnim]->frames[frameIndex]);
  computeNormals();
  currFrame = frameIndex;
  animTime = 0.0f;
}

void DA::DA_MD5Model::animate(float dt) {
  //	1st check (the animation)
  if ( currAnim < 0 || currAnim >= int(animations.size()) || !animations[currAnim] )
    throw Exception("animate: currAnim is invalid, plaease select an animation that exists");

  Animation *anim = animations[currAnim];

  //	2nd check . . . the frame
  if ( currFrame < 0 || currFrame >= int(anim->nframes) )
    throw Exception("animate: currFrame is invalid . . . Sometimes I wonder about you");

  //	whats the next frame's index
  int nextFrameIndex = currFrame >= anim->nframes - 1 ? 0 : currFrame + 1;

  //	update the animation time now . . . 
  animTime += dt*float(anim->nframeRate);
  if ( animTime > 1.0f ) {
    while ( animTime > 1.0f )
      animTime -= 1.0f;
    currFrame = nextFrameIndex;
    nextFrameIndex = currFrame >= anim->nframes - 1 ? 0 : currFrame + 1;
  }

  //	ensure the storage area is the right size for the new interpolated frame
  if ( int(interpolationFrame.joints.size()) != numJoints )
    interpolationFrame.joints.resize(numJoints);
//	now we can interpolate away to our hearts content!

  Frame &frame     = anim->frames[currFrame],
        &nextFrame = anim->frames[nextFrameIndex];

  // interpolate between the joints of the current frame and those of the next
  // frame and store them in interpFrame
  /*
		here we can interpolate between the current frame's joints
  */
  for ( int i=0; i < numJoints; i++ ) {
    Joint &interpJoint = interpolationFrame.joints[i];

    // linearly interpolate between joint positions
    float *pos1 = frame.joints[i].tridpos,
          *pos2 = nextFrame.joints[i].tridpos;
    interpJoint.tridpos[0] = pos1[0] + animTime*(pos2[0] - pos1[0]);
    interpJoint.tridpos[1] = pos1[1] + animTime*(pos2[1] - pos1[1]);
    interpJoint.tridpos[2] = pos1[2] + animTime*(pos2[2] - pos1[2]);

    interpJoint.quat = DA::Quaternion::slerp(frame.joints[i].quat, nextFrame.joints[i].quat, animTime);
  }

  genVertices(interpolationFrame);
  computeNormals();

	for(size_t i=0;i<meshes.size();i++){
	 int s = meshes[i]->triangles.size();
	  a[i].BeginMesh(s*3);

		for ( size_t j=0; j < meshes[i]->triangles.size(); j++){ //for each tri
			int t0,t1,t2;
			t0 = meshes[i]->triangles[j].v[0];
			t1 = meshes[i]->triangles[j].v[1];
			t2 = meshes[i]->triangles[j].v[2];
			//get the three verts of the tri.
			M3DVector3f ps[3],nm[3];
			M3DVector2f tx[3];
			//pos
			ps[0][0] = meshes[i]->verts[t0].pos[0];
			ps[0][1] = meshes[i]->verts[t0].pos[1];
			ps[0][2] = meshes[i]->verts[t0].pos[2];

			ps[1][0] = meshes[i]->verts[t1].pos[0];
			ps[1][1] = meshes[i]->verts[t1].pos[1];
			ps[1][2] = meshes[i]->verts[t1].pos[2];

			ps[2][0] = meshes[i]->verts[t2].pos[0];
			ps[2][1] = meshes[i]->verts[t2].pos[1];
			ps[2][2] = meshes[i]->verts[t2].pos[2];

			//tex
			tx[0][0] = meshes[i]->verts[t0].tc[0];
			tx[0][1] = meshes[i]->verts[t0].tc[1];

			tx[1][0] = meshes[i]->verts[t1].tc[0];
			tx[1][1] = meshes[i]->verts[t1].tc[1];

			tx[2][0] = meshes[i]->verts[t2].tc[0];
			tx[2][1] = meshes[i]->verts[t2].tc[1];

			//normal
			nm[0][0] = meshes[i]->verts[t0].normal[0];
			nm[0][1] = meshes[i]->verts[t0].normal[1];
			nm[0][2] = meshes[i]->verts[t0].normal[2];

			nm[1][0] = meshes[i]->verts[t1].normal[0];
			nm[1][1] = meshes[i]->verts[t1].normal[1];
			nm[1][2] = meshes[i]->verts[t1].normal[2];

			nm[2][0] = meshes[i]->verts[t2].normal[0];
			nm[2][1] = meshes[i]->verts[t2].normal[1];
			nm[2][2] = meshes[i]->verts[t2].normal[2];

			a[i].AddTriangle(ps,nm,tx);

		}

		a[i].End();
	}
	

}

void DA::DA_MD5Model::render()
 {

	//glPushAttrib(GL_CURRENT_BIT);
	//glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT); 
	//
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_COLOR_ARRAY);

	//for( size_t i=0; i < meshes[0]->verts.size(); i++ ) {
	//	const Mesh *mesh = meshes[0];

	//	glVertexPointer(3, GL_FLOAT, GLsizei(sizeof(Vertex)), mesh->verts[i].pos);
	//	glNormalPointer(GL_FLOAT, GLsizei(sizeof(Vertex)), mesh->verts[i].normal);
	//	glTexCoordPointer(2, GL_FLOAT, GLsizei(sizeof(Vertex)), mesh->verts[i].tc);

	//	glDrawElements(GL_TRIANGLES, GLsizei(mesh->triangles.size()*3), GL_UNSIGNED_INT, &mesh->triangles[0]);
	//}

	//glPopClientAttrib();
	//glPopAttrib();

	 //HERE GOES FUCKIN NOTHIN
	//int counter=0;
	//int ms = meshes.size();
	//for(int i=0;i<ms;i++){
	//	counter = 0;
	//	int vs = meshes[i]->verts.size();
	//	meshes[i]->vert = new GLfloat[vs*3];

	//	for(int j=0;j<vs;j++){
	//		GLfloat v0,v1,v2;
	//		v0 = meshes[i]->verts[j].pos[0];
	//		v1 = meshes[i]->verts[j].pos[1];
	//		v2 = meshes[i]->verts[j].pos[2];
	//		
	//		meshes[i]->vert[counter] = v0;
	//		counter++;
	//		meshes[i]->vert[counter] = v1;
	//		counter++;
	//		meshes[i]->vert[counter] = v2;
	//		counter++;			
	//	}
	//	
	//	int ts = meshes[i]->triangles.size();
	//	meshes[i]->tris = new GLushort[ts*3];
	//	counter = 0;
	//	for(int j=0;j<ts;j++){
	//		GLushort t0,t1,t2;
	//		t0 = meshes[i]->triangles[j].v[0];
	//		t1 = meshes[i]->triangles[j].v[1];
	//		t2 = meshes[i]->triangles[j].v[2];

	//		meshes[i]->tris[counter] = t0;
	//		counter++;
	//		meshes[i]->tris[counter] = t1;
	//		counter++;
	//		meshes[i]->tris[counter] = t2;
	//		counter++;

	//	}
	//}




	//for(size_t p=0;p<meshes.size();p++){
	//glBindVertexArray(vao);
	//
	//glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
	//glEnableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*meshes[p]->verts.size()*3,meshes[p]->vert,GL_DYNAMIC_DRAW);
	//glVertexAttribPointer(GLT_ATTRIBUTE_VERTEX,3,GL_FLOAT,GL_FALSE,0,0);

	//glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
	//glEnableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*meshes[p]->verts.size()*3,meshes[p]->verts[0].normal,GL_DYNAMIC_DRAW);
	//glVertexAttribPointer(GLT_ATTRIBUTE_NORMAL,3,GL_FLOAT,GL_FALSE,0,0);

	//glBindBuffer(GL_ARRAY_BUFFER,vbo[2]);
	//glEnableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*meshes[p]->verts.size()*2,meshes[p]->verts[0].tc,GL_DYNAMIC_DRAW);
	//glVertexAttribPointer(GLT_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo[3]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*meshes[p]->triangles.size()*3,meshes[p]->tris,GL_DYNAMIC_DRAW);
	////unbind vao;
	//glBindVertexArray(0);

	////---actual render---
	////bind vao;
	//glBindVertexArray(vao);
	//glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
	//glEnableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
	//glVertexAttribPointer(GLT_ATTRIBUTE_VERTEX,3,GL_FLOAT,GL_FALSE,0,0);

	//glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
	//glEnableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
	//glVertexAttribPointer(GLT_ATTRIBUTE_NORMAL,3,GL_FLOAT,GL_FALSE,0,0);

	//glBindBuffer(GL_ARRAY_BUFFER,vbo[2]);
	//glEnableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0);
	//glVertexAttribPointer(GLT_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo[3]);
	//
	//glDrawElements(GL_TRIANGLES,meshes[p]->triangles.size(),GL_UNSIGNED_INT,0);

	//glBindVertexArray(0);
	//}
	
//			
//----------Doesnt work :( -----------------
	//for(size_t i=0;i<meshes.size();i++){
	//	int s = meshes[i]->triangles.size();
	//
	//	b[i].Begin(GL_TRIANGLES,s*3,0);
	//	for(size_t a=0;a<s;a++){
	//		int t0,t1,t2;
	//		t0 = meshes[i]->triangles[a].v[0];
	//		t1 = meshes[i]->triangles[a].v[1];
	//		t2 = meshes[i]->triangles[a].v[2];
	//		
	//		b[i].MultiTexCoord2fv(0,meshes[i]->verts[t0].tc);
	//		b[i].Normal3fv(meshes[i]->verts[t0].normal);
	//		b[i].Vertex3fv(meshes[i]->verts[t0].pos);

	//		b[i].MultiTexCoord2fv(0,meshes[i]->verts[t1].tc);
	//		b[i].Normal3fv(meshes[i]->verts[t1].normal);
	//		b[i].Vertex3fv(meshes[i]->verts[t1].pos);

	//		b[i].MultiTexCoord2fv(0,meshes[i]->verts[t2].tc);
	//		b[i].Normal3fv(meshes[i]->verts[t2].normal);
	//		b[i].Vertex3fv(meshes[i]->verts[t2].pos);
	//		
	//	}

	//	b[i].End();
	//	b[i].Draw();
	//}

	
	for(size_t i=0;i<meshes.size();i++){
		a[i].Draw();
	}

	//delete[] a;
}