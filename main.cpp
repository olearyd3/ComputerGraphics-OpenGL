// Windows includes (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "maths_funcs.h"
#define STB_IMAGE_IMPLEMENTATION
#include "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/stb_image.h"
//#include "camera.h"

//global var to start/stop rotation here
bool start_y = false;
bool start_z = false;
bool start_x = false;
bool ortho = false;
bool change = false;
bool reindeerBack = false;
// from LearnOpenGL Camera tut
bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
#define M_PI 3.1415926535897932384626433832795
/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// meshes to be used in project
#define MESH_NAME "C:/Users/markj/Downloads/snowmanbod.dae"
#define ARM1_MESH "C:/Users/markj/Downloads/rightArmSmaller.dae"
#define ARM2_MESH "C:/Users/markj/Downloads/leftArmSmaller.dae"
#define GROUND_PLANE_MESH "C:/Users/markj/Downloads/snowterrain.dae"
#define HAT_MESH "C:/Users/markj/Downloads/hat.dae"
#define NOSE "C:/Users/markj/Downloads/carrot.dae"
#define EYES "C:/Users/markj/Downloads/eyes.dae"
#define FIR "C:/Users/markj/Downloads/4m7qrzwizbnk-fir/fir.obj"
#define LEAVES "C:/Users/markj/Downloads/treeleaves.dae"
#define TRUNK "C:/Users/markj/Downloads/treetrunk.dae"
#define REINDEER "C:/Users/markj/Downloads/reindeerbody.dae"
#define MAIN_BELL "C:/Users/markj/Downloads/mainbell.dae"
#define HARNESS "C:/Users/markj/Downloads/reindeerleather.dae"
#define CANDYCANE "C:/Users/markj/Downloads/candycane.dae"
// textures
#define TERRAIN "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/textures/terrain.jpg"
#define SNOW_TEX "C:/Users/markj/Downloads/sno2.jpg"
#define LEATHER "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/textures/leatherdark.jpg"
#define BARK "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/textures/tree-bark.jpeg"
#define CARROT "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/textures/carrot.jpg"
#define LEAVES_TEX "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/textures/firtree.jpg"
#define REINDEER_TEX "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/textures/reindeer.jpg"
#define BELL_TEX "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/textures/reindeerbell.jpg"
#define CANDY_CANE_TEX "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/textures/candycane.jpeg"
//#define WOOD "C:/Users/markj/Downloads/treeTrunk2.jpg"
/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/

#pragma region SimpleTypes
typedef struct namedType
{
	size_t mPointCount = 0;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<vec2> mTextureCoords;
} ModelData;
#pragma endregion SimpleTypes

using namespace std;

// shader variables and models
GLuint shaderProgramID;
ModelData mesh_data;
ModelData arm1Data;
ModelData arm2Data;
ModelData groundData;
ModelData armJ1Data;
ModelData armJ2Data;
ModelData hatData;
ModelData carrotData;
ModelData eyesData;
ModelData treeData;
ModelData leavesData;
ModelData trunkData;
ModelData reindeerData;
ModelData mainBellData;
ModelData harnessData;
ModelData caneData;
// screen size
int width = 920;
int height = 690;
//Perspective Transform Variables
bool keyStates[256];
bool persp = true;

// model transform variables
GLuint loc1[8 * 3];
GLuint loc2[8 * 3];
GLuint loc3[8 * 3];
GLuint loc4[8 * 3];
GLfloat rotate_y = 0.0f;
GLfloat rotate_x = 0.0f;
GLfloat rotate_z = 0.0f;
GLfloat translate_x = 0.0f;
GLfloat translate_y = 0.0f;
GLfloat translate_z = 0.0f;
vec3 translate_base = vec3(0.0f, 0.0f, 0.0f);
vec3 cameraPos = vec3(5.0f, 3.5f, 12.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
vec3 scaling = vec3(1.0f, 1.0f, 1.0f);

vec3 snowmanPos = vec3(7.0f, 0.0f, 3.0f);
vec3 groundPos = vec3(0.0f, -3.9f, 0.0f);
vec3 treePos = vec3(5.0f, 0.2f, 2.0f);
vec3 arm1Pos = vec3(0.0f, 0.0f, 0.0f);
vec3 arm2Pos = vec3(0.0f, 0.0f, 0.0f);
vec3 armJoint1Pos = vec3(0.0f, 0.0f, 0.0f);
vec3 armJoint2Pos = vec3(0.0f, 0.0f, 0.0f);
float reindeerPos_z = 2.0f;
vec3 reindeerPos = vec3(8.0f, 0.5f, reindeerPos_z);
vec3 canePos = vec3(7.0f, -0.1f, 0.0f);

// animating the snowman arms
float armjoint1_z = 0.0f;
float armjoint2_z = 0.0f;
float arm_wave = 0.0f;
float hat_y = 0.0f;
float cane_y = 0.0f;
// textures
unsigned int terrain_texture;
unsigned int snow_texture;
unsigned int leather_texture;
unsigned int bark_texture;
unsigned int carrot_texture;
unsigned int leaves_texture;
unsigned int reindeer_texture;
unsigned int bell_texture;
unsigned int cane_texture;

// VBOs and VAO
unsigned int VP_VBO[20]; // vertex positions
unsigned int VN_VBO[20]; // vertex normals
unsigned int VT_VBO[20]; // vertex textures
unsigned int VAO[3];

#pragma region MESH LOADING
/*----------------------------------------------------------------------------
MESH LOADING FUNCTION
----------------------------------------------------------------------------*/

ModelData load_mesh(const char* file_name) {
	ModelData modelData;

	/* Use assimp to read the model file, forcing it to be read as    */
	/* triangles. The second flag (aiProcess_PreTransformVertices) is */
	/* relevant if there are multiple meshes in the model file that   */
	/* are offset from the origin. This is pre-transform them so      */
	/* they're in the right position.                                 */
	const aiScene* scene = aiImportFile(
		file_name,
		aiProcess_Triangulate | aiProcess_PreTransformVertices
	);

	if (!scene) {
		fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
		return modelData;
	}

	printf("  %i materials\n", scene->mNumMaterials);
	printf("  %i meshes\n", scene->mNumMeshes);
	printf("  %i textures\n", scene->mNumTextures);

	for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
		const aiMesh* mesh = scene->mMeshes[m_i];
		printf("    %i vertices in mesh\n", mesh->mNumVertices);
		modelData.mPointCount += mesh->mNumVertices;
		for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
			if (mesh->HasPositions()) {
				const aiVector3D* vp = &(mesh->mVertices[v_i]);
				modelData.mVertices.push_back(vec3(vp->x, vp->y, vp->z));
			}
			if (mesh->HasNormals()) {
				const aiVector3D* vn = &(mesh->mNormals[v_i]);
				modelData.mNormals.push_back(vec3(vn->x, vn->y, vn->z));
			}
			if (mesh->HasTextureCoords(0)) {
				const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
				modelData.mTextureCoords.push_back(vec2(vt->x, vt->y));
			}
			if (mesh->HasTangentsAndBitangents()) {
				/* You can extract tangents and bitangents here              */
				/* Note that you might need to make Assimp generate this     */
				/* data for you. Take a look at the flags that aiImportFile  */
				/* can take.                                                 */
			}
		}
	}

	aiReleaseImport(scene);
	return modelData;
}

#pragma endregion MESH LOADING

// Loads Textures using STB Image
#pragma region TEXTURE LOADING
unsigned int load_tex(const char* file_name) {
	// load and create a texture 
	unsigned int texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(file_name, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}
#pragma endregion TEXTURE LOADING

// Shader Functions- click on + to expand
#pragma region SHADER_FUNCTIONS
char* readShaderSource(const char* shaderFile) {
	FILE* fp;
	//printf("%p", &fp);
	fopen_s(&fp, shaderFile, "rb");

	if (fp == NULL) {
		return NULL;
	}

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);
	buf[size] = '\0';

	fclose(fp);

	return buf;
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// create a shader object
	GLuint ShaderObj = glCreateShader(ShaderType);
	if (ShaderObj == 0) {
		std::cerr << "Error creating shader..." << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	const char* pShaderSource = readShaderSource(pShaderText);

	// Bind the source code to the shader, this happens before compilation
	glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderSource, NULL);
	// compile the shader and check for errors
	glCompileShader(ShaderObj);
	GLint success;
	// check for shader related errors using glGetShaderiv
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024] = { '\0' };
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		std::cerr << "Error compiling "
			<< (ShaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader program: " << InfoLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	// Attach the compiled shader object to the program object
	glAttachShader(ShaderProgram, ShaderObj);
}

GLuint CompileShaders()
{
	//Start the process of setting up our shaders by creating a program ID
	//Note: we will link all the shaders together into this ID
	shaderProgramID = glCreateProgram();
	if (shaderProgramID == 0) {
		std::cerr << "Error creating shader program..." << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}

	// Create two shader objects, one for the vertex, and one for the fragment shader
	AddShader(shaderProgramID, "Shaders/OriginalVertexShader.txt", GL_VERTEX_SHADER);
	AddShader(shaderProgramID, "Shaders/OriginalFragmentShader.txt", GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { '\0' };
	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}

	// program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
	glValidateProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	// Finally, use the linked shader program
	// Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
	glUseProgram(shaderProgramID);
	return shaderProgramID;
}

#pragma endregion SHADER_FUNCTIONS

// VBO Functions - click on + to expand
#pragma region VBO_FUNCTIONS
void generateObjectBufferMesh() {
	/*----------------------------------------------------------------------------
	LOAD MESH HERE AND COPY INTO BUFFERS
	----------------------------------------------------------------------------*/

	//Note: you may get an error "vector subscript out of range" if you are using this code for a mesh that doesnt have positions and normals
	//Might be an idea to do a check for that before generating and binding the buffer.

	// load in the models
	mesh_data = load_mesh(MESH_NAME);
	groundData = load_mesh(GROUND_PLANE_MESH);
	arm1Data = load_mesh(ARM1_MESH);
	arm2Data = load_mesh(ARM2_MESH);
	hatData = load_mesh(HAT_MESH);
	carrotData = load_mesh(NOSE);
	eyesData = load_mesh(EYES);
	treeData = load_mesh(FIR);
	leavesData = load_mesh(LEAVES);
	trunkData = load_mesh(TRUNK);
	reindeerData = load_mesh(REINDEER);
	mainBellData = load_mesh(MAIN_BELL);
	harnessData = load_mesh(HARNESS);
	caneData = load_mesh(CANDYCANE);
	// load textures
	terrain_texture = load_tex(TERRAIN);
	snow_texture = load_tex(SNOW_TEX);
	leather_texture = load_tex(LEATHER);
	bark_texture = load_tex(BARK);
	carrot_texture = load_tex(CARROT);
	leaves_texture = load_tex(LEAVES_TEX);
	reindeer_texture = load_tex(REINDEER_TEX);
	bell_texture = load_tex(BELL_TEX);
	cane_texture = load_tex(CANDY_CANE_TEX);

	// generate the buffers
	glGenBuffers(20, VP_VBO);
	glGenBuffers(20, VN_VBO);
	glGenBuffers(20, VT_VBO);

	// main root object - snowman
	loc1[0] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[1] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[2] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec2), &mesh_data.mTextureCoords[0], GL_STATIC_DRAW);

	// child of main body -- arms 1 and 2
	loc1[3] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[4] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[5] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, arm1Data.mPointCount * sizeof(vec3), &arm1Data.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, arm1Data.mPointCount * sizeof(vec3), &arm1Data.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, arm1Data.mPointCount * sizeof(vec2), &arm1Data.mTextureCoords[0], GL_STATIC_DRAW);

	loc1[12] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[13] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[14] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[5]);
	glBufferData(GL_ARRAY_BUFFER, arm2Data.mPointCount * sizeof(vec3), &arm2Data.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[5]);
	glBufferData(GL_ARRAY_BUFFER, arm2Data.mPointCount * sizeof(vec3), &arm2Data.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[5]);
	glBufferData(GL_ARRAY_BUFFER, arm2Data.mPointCount * sizeof(vec2), &arm2Data.mTextureCoords[0], GL_STATIC_DRAW);

	// carrot
	loc1[9] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[10] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[11] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, carrotData.mPointCount * sizeof(vec3), &carrotData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, carrotData.mPointCount * sizeof(vec3), &carrotData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, carrotData.mPointCount * sizeof(vec2), &carrotData.mTextureCoords[0], GL_STATIC_DRAW);

	//eyes
	loc1[15] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[16] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[17] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[6]);
	glBufferData(GL_ARRAY_BUFFER, eyesData.mPointCount * sizeof(vec3), &eyesData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[6]);
	glBufferData(GL_ARRAY_BUFFER, eyesData.mPointCount * sizeof(vec3), &eyesData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[6]);
	glBufferData(GL_ARRAY_BUFFER, eyesData.mPointCount * sizeof(vec2), &eyesData.mTextureCoords[0], GL_STATIC_DRAW);

	// HAT 
	loc1[6] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[7] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[8] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, hatData.mPointCount * sizeof(vec3), &hatData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, hatData.mPointCount * sizeof(vec3), &hatData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, hatData.mPointCount * sizeof(vec2), &hatData.mTextureCoords[0], GL_STATIC_DRAW);

	// REINDEER BODY
	loc3[9] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc3[10] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc3[11] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[10]);
	glBufferData(GL_ARRAY_BUFFER, reindeerData.mPointCount * sizeof(vec3), &reindeerData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[10]);
	glBufferData(GL_ARRAY_BUFFER, reindeerData.mPointCount * sizeof(vec3), &reindeerData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[10]);
	glBufferData(GL_ARRAY_BUFFER, reindeerData.mPointCount * sizeof(vec2), &reindeerData.mTextureCoords[0], GL_STATIC_DRAW);
	// reindeer main bell
	loc3[12] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc3[13] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc3[14] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[11]);
	glBufferData(GL_ARRAY_BUFFER, mainBellData.mPointCount * sizeof(vec3), &mainBellData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[11]);
	glBufferData(GL_ARRAY_BUFFER, mainBellData.mPointCount * sizeof(vec3), &mainBellData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[11]);
	glBufferData(GL_ARRAY_BUFFER, mainBellData.mPointCount * sizeof(vec2), &mainBellData.mTextureCoords[0], GL_STATIC_DRAW);
	// reindeer harness
	loc3[15] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc3[16] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc3[17] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[12]);
	glBufferData(GL_ARRAY_BUFFER, harnessData.mPointCount * sizeof(vec3), &harnessData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[12]);
	glBufferData(GL_ARRAY_BUFFER, harnessData.mPointCount * sizeof(vec3), &harnessData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[12]);
	glBufferData(GL_ARRAY_BUFFER, harnessData.mPointCount * sizeof(vec2), &harnessData.mTextureCoords[0], GL_STATIC_DRAW);
	// REINDEER 2
	// REINDEER BODY
	loc4[0] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc4[1] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc4[2] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[13]);
	glBufferData(GL_ARRAY_BUFFER, reindeerData.mPointCount * sizeof(vec3), &reindeerData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[13]);
	glBufferData(GL_ARRAY_BUFFER, reindeerData.mPointCount * sizeof(vec3), &reindeerData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[13]);
	glBufferData(GL_ARRAY_BUFFER, reindeerData.mPointCount * sizeof(vec2), &reindeerData.mTextureCoords[0], GL_STATIC_DRAW);
	// reindeer main bell
	loc4[3] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc4[4] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc4[5] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[14]);
	glBufferData(GL_ARRAY_BUFFER, mainBellData.mPointCount * sizeof(vec3), &mainBellData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[14]);
	glBufferData(GL_ARRAY_BUFFER, mainBellData.mPointCount * sizeof(vec3), &mainBellData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[14]);
	glBufferData(GL_ARRAY_BUFFER, mainBellData.mPointCount * sizeof(vec2), &mainBellData.mTextureCoords[0], GL_STATIC_DRAW);
	// reindeer harness
	loc4[6] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc4[7] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc4[8] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[15]);
	glBufferData(GL_ARRAY_BUFFER, harnessData.mPointCount * sizeof(vec3), &harnessData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[15]);
	glBufferData(GL_ARRAY_BUFFER, harnessData.mPointCount * sizeof(vec3), &harnessData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[15]);
	glBufferData(GL_ARRAY_BUFFER, harnessData.mPointCount * sizeof(vec2), &harnessData.mTextureCoords[0], GL_STATIC_DRAW);
	// REINDEER BODY
	loc4[9] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc4[10] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc4[11] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[16]);
	glBufferData(GL_ARRAY_BUFFER, reindeerData.mPointCount * sizeof(vec3), &reindeerData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[16]);
	glBufferData(GL_ARRAY_BUFFER, reindeerData.mPointCount * sizeof(vec3), &reindeerData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[16]);
	glBufferData(GL_ARRAY_BUFFER, reindeerData.mPointCount * sizeof(vec2), &reindeerData.mTextureCoords[0], GL_STATIC_DRAW);
	// REINDEER 3
	// reindeer main bell
	loc4[12] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc4[13] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc4[14] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[17]);
	glBufferData(GL_ARRAY_BUFFER, mainBellData.mPointCount * sizeof(vec3), &mainBellData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[17]);
	glBufferData(GL_ARRAY_BUFFER, mainBellData.mPointCount * sizeof(vec3), &mainBellData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[17]);
	glBufferData(GL_ARRAY_BUFFER, mainBellData.mPointCount * sizeof(vec2), &mainBellData.mTextureCoords[0], GL_STATIC_DRAW);
	// reindeer harness
	loc4[15] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc4[16] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc4[17] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[18]);
	glBufferData(GL_ARRAY_BUFFER, harnessData.mPointCount * sizeof(vec3), &harnessData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[18]);
	glBufferData(GL_ARRAY_BUFFER, harnessData.mPointCount * sizeof(vec3), &harnessData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[18]);
	glBufferData(GL_ARRAY_BUFFER, harnessData.mPointCount * sizeof(vec2), &harnessData.mTextureCoords[0], GL_STATIC_DRAW);

	// ground -- need to fix texture
	loc2[0] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2[1] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc2[2] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, groundData.mPointCount * sizeof(vec3), &groundData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, groundData.mPointCount * sizeof(vec3), &groundData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, groundData.mPointCount * sizeof(vec2), &groundData.mTextureCoords[0], GL_STATIC_DRAW);

	// CANDY CANE
	loc2[3] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2[4] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc2[5] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[19]);
	glBufferData(GL_ARRAY_BUFFER, caneData.mPointCount * sizeof(vec3), &caneData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[19]);
	glBufferData(GL_ARRAY_BUFFER, caneData.mPointCount * sizeof(vec3), &caneData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[19]);
	glBufferData(GL_ARRAY_BUFFER, caneData.mPointCount * sizeof(vec2), &caneData.mTextureCoords[0], GL_STATIC_DRAW);
}
#pragma endregion VBO_FUNCTIONS

void display() {

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);


	//Declare your uniform variables that will be used in your shader
	GLuint proj_mat_location = glGetUniformLocation(shaderProgramID, "proj");
	GLuint view_mat_location = glGetUniformLocation(shaderProgramID, "view");
	GLuint matrix_location = glGetUniformLocation(shaderProgramID, "model");
	GLuint color_location = glGetUniformLocation(shaderProgramID, "color");
	//GLuint ambient_location = glGetUniformLocation(shaderProgramID, "material.ambient");
	GLuint diffuse_location = glGetUniformLocation(shaderProgramID, "material.diffuse");
	GLuint specular_location = glGetUniformLocation(shaderProgramID, "material.specular");
	GLuint shininess_location = glGetUniformLocation(shaderProgramID, "material.shininess");

	// set up the lights
	// LIGHT 0
	GLuint light0_location = glGetUniformLocation(shaderProgramID, "pointLights[0].position");
	GLuint light0_ambient = glGetUniformLocation(shaderProgramID, "pointLights[0].ambient");
	GLuint light0_diffuse = glGetUniformLocation(shaderProgramID, "pointLights[0].diffuse");
	GLuint light0_specular = glGetUniformLocation(shaderProgramID, "pointLights[0].specular");
	GLuint light0_constant = glGetUniformLocation(shaderProgramID, "Light[0].constant");
	GLuint light0_linear = glGetUniformLocation(shaderProgramID, "Light[0].linear");
	GLuint light0_quadratic = glGetUniformLocation(shaderProgramID, "Light[0].quadratic");
	vec3 light0Position = vec3(800.0f, 500.0f, 20.0f);
	vec3 lightAmbient = vec3(0.2f, 0.2f, 0.2f);
	vec3 lightDiffuse = vec3(0.1f, 0.1f, 0.1f);
	vec3 lightSpecular = vec3(0.8f, 0.8f, 0.8f);
	float lightConstant = 1.0f;
	float lightLinear = 0.001;
	float lightQuadratic = 0.005;
	glUniform3fv(light0_location, 1, light0Position.v);
	glUniform3fv(light0_ambient, 1, lightDiffuse.v);
	glUniform3fv(light0_diffuse, 1, lightSpecular.v);
	glUniform3fv(light0_specular, 1, lightSpecular.v);
	glUniform1f(light0_constant, lightConstant);
	glUniform1f(light0_linear, lightLinear);
	glUniform1f(light0_quadratic, lightQuadratic);
	// LIGHT 1
	GLuint light1_location = glGetUniformLocation(shaderProgramID, "pointLights[1].position");
	GLuint light1_ambient = glGetUniformLocation(shaderProgramID, "pointLights[1].ambient");
	GLuint light1_diffuse = glGetUniformLocation(shaderProgramID, "pointLights[1].diffuse");
	GLuint light1_specular = glGetUniformLocation(shaderProgramID, "pointLights[1].specular");
	GLuint light1_constant = glGetUniformLocation(shaderProgramID, "Light[1].constant");
	GLuint light1_linear = glGetUniformLocation(shaderProgramID, "Light[1].linear");
	GLuint light1_quadratic = glGetUniformLocation(shaderProgramID, "Light[1].quadratic");
	vec3 light2Position = vec3(-70.0f, 700.0f, -70.0f);

	glUniform3fv(light1_location, 1, light2Position.v);
	glUniform3fv(light1_ambient, 1, lightDiffuse.v);
	glUniform3fv(light1_diffuse, 1, lightSpecular.v);
	glUniform3fv(light1_specular, 1, lightSpecular.v);
	glUniform1f(light1_constant, lightConstant);
	glUniform1f(light1_linear, lightLinear);
	glUniform1f(light1_quadratic, lightQuadratic);
	//GLuint light1_location = glGetUniformLocation(shaderProgramID, "Light[1].position");

	// Root of the Hierarchy
	glm::mat4 projection;
	// Switch between Ortho and Perspective views based on whether persp is true or false
	if (persp) {
		projection = glm::perspective(fov, (float)width / (float)height, 0.1f, 1000.0f);
	}
	else {
		projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
	}

	// set up view
	mat4 view = identity_mat4();
	view = look_at(cameraPos, cameraPos + cameraFront, cameraUp);

	// update uniforms & draw
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);

	// translating, rotating and scaling the main model
	mat4 model = identity_mat4();
	model = translate(model, translate_base);
	model = rotate_z_deg(model, rotate_z);
	model = rotate_y_deg(model, rotate_y);
	model = rotate_x_deg(model, rotate_x);
	model = translate(model, snowmanPos);
	model = scale(model, scaling);
	vec3 color = vec3(0.9f, 0.9f, 0.9f);
	glUniform3fv(color_location, 1, color.v);
	//vec3 ambient = vec3(0.9f, 0.9f, 0.9f);
	vec3 diffuse = vec3(1.0f, 1.0f, 1.0f);
	vec3 specular = vec3(1.0f, 1.0f, 1.0f);
	float shininess = 2.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);

	glEnableVertexAttribArray(loc1[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[0]);
	glVertexAttribPointer(loc1[0], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc1[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[0]);
	glVertexAttribPointer(loc1[1], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	// texture - snow
	glEnableVertexAttribArray(loc1[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[0]);
	glVertexAttribPointer(loc1[2], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, snow_texture);
	glDrawArrays(GL_TRIANGLES, 0, mesh_data.mPointCount);

	// child -- hierarchical arms
	mat4 modelChildArm1 = identity_mat4();
	modelChildArm1 = rotate_z_deg(modelChildArm1, armjoint1_z);
	modelChildArm1 = rotate_y_deg(modelChildArm1, 0.0f);
	modelChildArm1 = rotate_x_deg(modelChildArm1, 0.0f);
	modelChildArm1 = translate(modelChildArm1, arm1Pos);
	modelChildArm1 = model * modelChildArm1;
	color = vec3(0.5f, 0.25f, 0.0f);
	glUniform3fv(color_location, 1, color.v);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelChildArm1.m);
	glEnableVertexAttribArray(loc1[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[2]);
	glVertexAttribPointer(loc1[3], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc1[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[2]);
	glVertexAttribPointer(loc1[4], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc1[5]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[2]);
	glVertexAttribPointer(loc1[5], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, bark_texture);
	glDrawArrays(GL_TRIANGLES, 0, arm1Data.mPointCount);

	// child -- hierarchical arms
	mat4 modelChildArm2 = identity_mat4();
	modelChildArm2 = rotate_z_deg(modelChildArm2, armjoint2_z);
	modelChildArm2 = rotate_y_deg(modelChildArm2, 0.0f);
	modelChildArm2 = rotate_x_deg(modelChildArm2, 0.0f);
	modelChildArm2 = translate(modelChildArm2, arm2Pos);
	modelChildArm2 = model * modelChildArm2;
	color = vec3(0.5f, 0.25f, 0.0f);
	glUniform3fv(color_location, 1, color.v);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelChildArm2.m);
	glEnableVertexAttribArray(loc1[12]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[5]);
	glVertexAttribPointer(loc1[12], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc1[13]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[5]);
	glVertexAttribPointer(loc1[13], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_TRIANGLES, 0, arm2Data.mPointCount);

	// hat -- hierarchical
	mat4 modelHat = identity_mat4();
	modelHat = rotate_z_deg(modelHat, 0.0f);
	modelHat = rotate_y_deg(modelHat, hat_y);
	modelHat = rotate_x_deg(modelHat, 0.0f);
	modelHat = translate(modelHat, vec3(0.0f, 0.0f, 0.0f));
	modelHat = model * modelHat;
	//ambient = vec3(0.8f, 0.8f, 0.8f);
	diffuse = vec3(0.51f, 0.51f, 0.51f);
	specular = vec3(0.5f, 0.5f, 0.5f);
	shininess = 40.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelHat.m);
	glEnableVertexAttribArray(loc1[6]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[3]);
	glVertexAttribPointer(loc1[6], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc1[7]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[3]);
	glVertexAttribPointer(loc1[7], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc1[8]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[3]);
	glVertexAttribPointer(loc1[8], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, leather_texture);
	glDrawArrays(GL_TRIANGLES, 0, hatData.mPointCount);

	// carrot
	mat4 modelCarrot = identity_mat4();
	modelCarrot = model * modelCarrot;
	//ambient = vec3(0.8f, 0.8f, 0.8f);
	diffuse = vec3(0.4f, 0.4f, 0.4f);
	specular = vec3(1.0f, 1.0f, 1.0f);
	shininess = 100.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniform3fv(color_location, 1, color.v);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelCarrot.m);
	glEnableVertexAttribArray(loc1[9]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[4]);
	glVertexAttribPointer(loc1[9], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc1[10]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[4]);
	glVertexAttribPointer(loc1[10], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc1[11]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[4]);
	glVertexAttribPointer(loc1[11], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, carrot_texture);
	glDrawArrays(GL_TRIANGLES, 0, carrotData.mPointCount);

	// eyes
	mat4 modelEyes = identity_mat4();
	modelEyes = model * modelEyes;
	//ambient = vec3(0.05f, 0.05f, 0.06f);
	diffuse = vec3(0.18f, 0.17f, 0.225f);
	specular = vec3(0.99f, 0.99f, 0.99f);
	shininess = 100.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniform3fv(color_location, 1, color.v);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelEyes.m);
	glEnableVertexAttribArray(loc1[15]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[6]);
	glVertexAttribPointer(loc1[15], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc1[16]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[6]);
	glVertexAttribPointer(loc1[16], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc1[17]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[6]);
	glVertexAttribPointer(loc1[17], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, leather_texture);
	glDrawArrays(GL_TRIANGLES, 0, eyesData.mPointCount);

	// reindeer
	mat4 modelReindeer = identity_mat4();
	modelReindeer = translate(modelReindeer, reindeerPos);
	modelReindeer = rotate_y_deg(modelReindeer, 240.0f);
	modelReindeer = scale(modelReindeer, vec3(0.6f, 0.6f, 0.6f));
	//ambient = vec3(0.9f, 0.9f, 0.9f);
	diffuse = vec3(0.4f, 0.4f, 0.4f);
	specular = vec3(0.0f, 0.0f, 0.0f);
	shininess = 120.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelReindeer.m);
	glEnableVertexAttribArray(loc3[9]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[10]);
	glVertexAttribPointer(loc3[9], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc3[10]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[10]);
	glVertexAttribPointer(loc3[10], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc3[11]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[10]);
	glVertexAttribPointer(loc3[11], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, reindeer_texture);
	glDrawArrays(GL_TRIANGLES, 0, reindeerData.mPointCount);

	// reindeer 2
	mat4 modelReindeer2 = identity_mat4();
	modelReindeer2 = translate(modelReindeer2, reindeerPos + vec3(-4.0f, 0.0f, 0.0f));
	modelReindeer2 = rotate_y_deg(modelReindeer2, 240.0f);
	modelReindeer2 = scale(modelReindeer2, vec3(0.6f, 0.6f, 0.6f));
	//ambient = vec3(0.9f, 0.9f, 0.9f);
	diffuse = vec3(0.4f, 0.4f, 0.4f);
	specular = vec3(0.0f, 0.0f, 0.0f);
	shininess = 120.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelReindeer2.m);
	glEnableVertexAttribArray(loc4[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[13]);
	glVertexAttribPointer(loc4[0], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc4[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[13]);
	glVertexAttribPointer(loc4[1], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc4[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[13]);
	glVertexAttribPointer(loc4[2], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, reindeer_texture);
	glDrawArrays(GL_TRIANGLES, 0, reindeerData.mPointCount);

	// reindeer 3
	mat4 modelReindeer3 = identity_mat4();
	modelReindeer3 = translate(modelReindeer3, reindeerPos + vec3(-8.0f, 0.0f, 0.0f));
	modelReindeer3 = rotate_y_deg(modelReindeer3, 240.0f);
	modelReindeer3 = scale(modelReindeer3, vec3(0.6f, 0.6f, 0.6f));
	//ambient = vec3(0.9f, 0.9f, 0.9f);
	diffuse = vec3(0.4f, 0.4f, 0.4f);
	specular = vec3(0.0f, 0.0f, 0.0f);
	shininess = 120.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelReindeer3.m);
	glEnableVertexAttribArray(loc4[9]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[16]);
	glVertexAttribPointer(loc4[9], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc4[10]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[16]);
	glVertexAttribPointer(loc4[10], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc4[11]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[16]);
	glVertexAttribPointer(loc4[11], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, reindeer_texture);
	glDrawArrays(GL_TRIANGLES, 0, reindeerData.mPointCount);

	// reindeer main bell
	mat4 modelMainBell = identity_mat4();
	modelMainBell = modelReindeer * modelMainBell;
	//modelMainBell = translate(modelMainBell, reindeerPos);
	//modelMainBell = rotate_y_deg(modelMainBell, 180.0f);
	//modelMainBell = scale(modelMainBell, vec3(0.6f, 0.6f, 0.6f));
	//ambient = vec3(0.25f, 0.2f, 0.07f);
	diffuse = vec3(0.75f, 0.61f, 0.226f);
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	specular = vec3(1.0f, 1.0f, 1.0f);
	shininess = 70.0f;
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelMainBell.m);
	glEnableVertexAttribArray(loc3[12]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[11]);
	glVertexAttribPointer(loc3[12], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc3[13]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[11]);
	glVertexAttribPointer(loc3[13], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc4[14]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[11]);
	glVertexAttribPointer(loc4[14], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, bell_texture);
	glDrawArrays(GL_TRIANGLES, 0, mainBellData.mPointCount);

	// reindeer harness
	mat4 modelHarness = identity_mat4();
	modelHarness = modelReindeer * modelHarness;
	//modelMainBell = translate(modelMainBell, reindeerPos);
	//modelMainBell = rotate_y_deg(modelMainBell, 180.0f);
	//modelMainBell = scale(modelMainBell, vec3(0.6f, 0.6f, 0.6f));
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelHarness.m);
	glEnableVertexAttribArray(loc3[15]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[12]);
	glVertexAttribPointer(loc3[15], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc3[16]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[12]);
	glVertexAttribPointer(loc3[16], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc1[17]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[12]);
	glVertexAttribPointer(loc1[17], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, leather_texture);
	glDrawArrays(GL_TRIANGLES, 0, harnessData.mPointCount);

	// main ground model
	mat4 ground_model = identity_mat4();
	ground_model = translate(ground_model, groundPos);
	ground_model = scale(ground_model, vec3(5.0f, 5.0f, 5.0f));
	color = vec3(0.0f, 0.7f, 0.0f);
	glUniform3fv(color_location, 1, color.v);
	//ambient = vec3(1.0f, 1.0f, 1.0f);
	diffuse = vec3(1.0f, 1.0f, 1.0f);
	specular = vec3(0.2f, 0.2f, 0.2f);
	shininess = 50.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, ground_model.m);
	glEnableVertexAttribArray(loc2[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[1]);
	glVertexAttribPointer(loc2[0], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[1]);
	glVertexAttribPointer(loc2[1], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[1]);
	glEnableVertexAttribArray(loc2[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[1]);
	glVertexAttribPointer(loc2[2], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, terrain_texture);
	glDrawArrays(GL_TRIANGLES, 0, groundData.mPointCount);

	// candy cane
	mat4 cane_model = identity_mat4();
	cane_model = scale(cane_model, vec3(0.015f, 0.015f, 0.015f));
	cane_model = rotate_y_deg(cane_model, cane_y);
	cane_model = translate(cane_model, canePos);
	diffuse = vec3(1.0f, 0.8f, 0.8f);
	specular = vec3(0.95f, 0.95f, 0.95f);
	shininess = 40.0f;
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, cane_model.m);
	glEnableVertexAttribArray(loc2[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[19]);
	glVertexAttribPointer(loc2[3], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[19]);
	glVertexAttribPointer(loc2[4], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[1]);
	glEnableVertexAttribArray(loc2[5]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[19]);
	glVertexAttribPointer(loc2[5], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, cane_texture);
	glDrawArrays(GL_TRIANGLES, 0, caneData.mPointCount);

	glutSwapBuffers();
}

void changeProjection() {
	if (keyStates['p']) {
		persp = true;
	}
	else if (keyStates['o']) {
		persp = false;
	}
}

void rotateModel(float delta) {
	// set rotation speed
	float speed = 90.0f;
	// if '-' pressed, reduce rotation speed
	if (keyStates['-']) {
		speed = -speed;
	}

	if (keyStates['r']) {
		if (keyStates['x']) {
			// Rotate the model around the x axis at 45 degrees per second
			rotate_x += speed * delta;
			rotate_x = fmodf(rotate_x, 360.0f);
		}
		if (keyStates['y']) {
			// Rotate the model around the y axis at 45 degrees per second
			rotate_y += speed * delta;
			rotate_y = fmodf(rotate_y, 360.0f);
		}
		if (keyStates['z']) {
			rotate_z += speed * delta;
			rotate_z = fmodf(rotate_z, 360.0f);
		}
	}
}

void translateModel(float delta) {
	float speed = 1.0f;
	// reduce speed if '-' pressed
	if (keyStates['-']) {
		speed = -speed;
	}
	// if t pressed along with x, y or z translate in that direction
	if (keyStates['t']) {
		// move the model to the left
		if (keyStates['a']) {
			translate_base += vec3(-0.06f, 0.0f, 0.0f);
		}
		// move the model to the right
		if (keyStates['d']) {
			translate_base -= vec3(-0.06f, 0.0f, 0.0f);
		}
		// move the model up
		if (keyStates['w']) {
			translate_base += vec3(0.0f, 0.06f, 0.0f);
		}
		// move the model down
		if (keyStates['s']) {
			translate_base -= vec3(0.0f, 0.06f, 0.0f);
		}
		// move the model forward
		if (keyStates['i']) {
			translate_base += vec3(0.0f, 0.0f, 0.06f);
		}
		// move the model back
		if (keyStates['z']) {
			translate_base += vec3(0.0f, 0.0f, -0.06f);
		}
	}
	if (keyStates['u']) {
		// move the camera up
		if (keyStates['w']) {
			cameraPos -= vec3(0.0f, -0.5f, 0.0f) * 0.1f;
		}
		// move the camera down
		if (keyStates['s']) {
			cameraPos += vec3(0.0f, -0.5f, 0.0f) * 0.1f;
		}
		// move the camera to the left
		if (keyStates['a']) {
			cameraPos -= normalise(cross(cameraFront, cameraUp)) * 0.1f;
		}
		// move the camera to the right
		if (keyStates['d']) {
			cameraPos += normalise(cross(cameraFront, cameraUp)) * 0.1f;
		}
		// move the camera forward
		if (keyStates['i']) {
			cameraPos -= vec3(0.0f, 0.0f, -0.5f) * 0.1f;
		}
		// move the camera back
		if (keyStates['z']) {
			cameraPos -= vec3(0.0f, 0.0f, 0.5f) * 0.1f;
		}
	}
}

void scaleModel(float delta) {
	const float speed = 1.001f;
	// if c pressed, make scale bigger
	if (keyStates['c']) {
		if (keyStates['i']) {
			scaling = scaling * speed;
		}
		if (keyStates['z']) {
			scaling = scaling * 0.999;
		}
	}
}

void reindeerMovements() {
	// animate the crowd to be constantly moving here
	if (reindeerPos_z > -4.0f && reindeerBack == false) {
		reindeerPos_z = reindeerPos_z - 0.15f;
		reindeerPos += vec3(0.0f, 0.0f, -0.15f);
	}
	else {
		reindeerBack = true;
		reindeerPos_z += 0.15f;
		reindeerPos -= vec3(0.0f, 0.0f, -0.15f);
		if (reindeerPos_z > 8.0f) {
			reindeerBack = false;
		}
	}
}

void animations(float delta) {
	if (keyStates['m']) {
		if (keyStates['l']) {
			if (armjoint1_z < 1.0f && change == false) {
				//printf("%f", armjoint1_z);
				armjoint1_z += 5.0f * delta;
			}
			else {
				//printf("%f", armjoint1_z);
				armjoint1_z -= 5.0f * delta;
				change = true;
				if (armjoint1_z < -3.0f) {
					change = false;
				}
			}
		}
		if (keyStates[';']) {
			if (armjoint2_z < 4.0f && change == false) {
				//printf("%f", armjoint1_z);
				armjoint2_z += 5.0f * delta;
			}
			else {
				//printf("%f", armjoint1_z);
				armjoint2_z -= 5.0f * delta;
				change = true;
				if (armjoint2_z < 1.0f) {
					change = false;
				}
			}
		}
	}
	if (keyStates['v']) {
		arm1Pos += vec3(0.0001f, 0.0000f, 0.0f);
		arm2Pos += vec3(-0.0001f, 0.0000f, 0.0f);
	}
	if (keyStates['b']) {
		arm1Pos += vec3(-0.0001f, 0.0000f, 0.0f);
		arm2Pos += vec3(0.0001f, 0.0000f, 0.0f);
	}
	if (keyStates['h']) {
		hat_y += 45.0f * delta;
		hat_y = fmodf(hat_y, 360.0f);
	}
}

void updateScene() {

	static DWORD last_time = 0;
	DWORD curr_time = timeGetTime();
	if (last_time == 0)
		last_time = curr_time;
	float delta = (curr_time - last_time) * 0.001f;
	last_time = curr_time;

	cane_y = cane_y + 5.0f;
	// reindeer movements as crowd
	reindeerMovements();
	// Changing projection type
	changeProjection();
	// Model transformations
	rotateModel(delta);
	translateModel(delta);
	scaleModel(delta);
	animations(delta);
	// If ESC pressed, quit the window
	if (keyStates[27]) {
		glutWarpPointer(width + 10, -10);
	}
	// Draw the next frame
	glutPostRedisplay();
}

// while a key is being held down start
void keyDown(unsigned char key, int x, int y) {
	keyStates[key] = true;
}

// when a key is released stop
void keyUp(unsigned char key, int x, int y) {
	keyStates[key] = false;
}

void init()
{
	// Set up the shaders
	GLuint shaderProgramID = CompileShaders();
	// load mesh into a vertex buffer array
	generateObjectBufferMesh();

}

void mouseCallback(int xposIn, int yposIn) {
	// if scroll wheel is used, zoom in/out accordingly --- BUGGY AF NEED TO FIX!!

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.2f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	vec3 direction = vec3(cos(ONE_DEG_IN_RAD * yaw) * cos(ONE_DEG_IN_RAD * pitch), sin(ONE_DEG_IN_RAD * (pitch)), sin(ONE_DEG_IN_RAD * yaw) * cos(ONE_DEG_IN_RAD * pitch));
	cameraFront = normalise(direction);

}

int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Lab 3");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutPassiveMotionFunc(mouseCallback);
	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	// Set up your objects and shadersw
	init();
	// Begin infinite event loop
	glutMainLoop();
	return 0;
}
