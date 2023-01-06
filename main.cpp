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
#include "stb_image.h"

// global var to start/stop rotation here
bool start_y = false;
bool start_z = false;
bool start_x = false;
// change bools for snowman arms
bool ortho = false;
bool change = false;
bool change2 = false;
bool change3 = false;
bool change4 = false;
bool change5 = false;
// for moving reindeer forward and back or snowmen forward and back
bool reindeerBack = false;
bool snowmanBack = false;
bool snowman2Back = false;
bool snowman3Back = false;
bool snowman4Back = false;
bool snowman5Back = false;
// from LearnOpenGL Camera tut
bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
// pi
#define M_PI 3.1415926535897932384626433832795
/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// meshes to be used in project
#define MESH_NAME "Models/snowmanbod.dae"
#define ARM1_MESH "Models/rightArmSmaller.dae"
#define ARM2_MESH "Models/leftArmSmaller.dae"
#define GROUND_PLANE_MESH "Models/snowterrain.dae"
#define HAT_MESH "Models/hat.dae"
#define NOSE "Models/carrot.dae"
#define EYES "Models/eyes.dae"
#define LEAVES "Models/leaves.dae"
#define TRUNK "Models/trunk.obj"
#define REINDEER "Models/reindeerbody.dae"
#define MAIN_BELL "Models/mainbell.dae"
#define HARNESS "Models/reindeerleather.dae"
#define CANDYCANE "Models/candycane.dae"
#define LAMPPOST "Models/lampNoGlass.obj"
#define LAMPLIGHT "Models/lampGlass.obj" 

// textures
#define TERRAIN "textures/terrain.jpg"
#define SNOW_TEX "textures/snowTex.jpg"
#define LEATHER "textures/leatherdark.jpg"
#define BARK "textures/tree-bark.jpeg"
#define CARROT "textures/carrot.jpg"
#define LEAVES_TEX "textures/green_square.jpg"
#define TRUNKTEX "textures/tree_brown.jpg"
#define REINDEER_TEX "textures/reindeer.jpg"
#define BELL_TEX "textures/reindeerbell.jpg"
#define CANDY_CANE_TEX "textures/stripes.jpg"
#define LAMP_METAL "textures/blackmetal.jpg"
#define LAMP_GLASS "textures/yellow.jpg"

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
ModelData cane2Data;
ModelData cane3Data;
ModelData lampData;
ModelData lampGlassData;

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
GLuint loc5[8 * 3]; // SNOWMAN 2
GLuint loc6[8 * 3]; // SNOWMAN 3
GLuint loc7[8 * 3]; // SNOWMAN 4
GLuint loc8[8 * 3]; // SNOWMAN 5
GLfloat rotate_y = 0.0f;
GLfloat rotate_y2 = 0.0f;
GLfloat rotate_y3 = 0.0f;
GLfloat rotate_y4 = 0.0f;
GLfloat rotate_y5 = 0.0f;
GLfloat rotate_x = 0.0f;
GLfloat rotate_z = 0.0f;
GLfloat translate_x = 0.0f;
GLfloat translate_y = 0.0f;
GLfloat translate_z = 0.0f;
vec3 translate_base = vec3(0.0f, 0.0f, 0.0f);
// camera variables
vec3 cameraPos = vec3(5.0f, 5.5f, 16.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
vec3 scaling = vec3(1.0f, 1.0f, 1.0f);

// snowman positions
float snowmanPos_z = 3.0f;
vec3 snowmanPos = vec3(7.0f, 0.0f, snowmanPos_z);
float snowman2Pos_z = -5.0f;
vec3 snowman2Pos = vec3(12.0f, 0.0f, snowman2Pos_z);
float snowman3Pos_z = 2.0f;
vec3 snowman3Pos = vec3(17.0f, 0.0f, snowman3Pos_z);
float snowman4Pos_z = -3.0f;
vec3 snowman4Pos = vec3(-3.0f, 0.0f, snowman4Pos_z);
float snowman5Pos_z = 2.0f;
vec3 snowman5Pos = vec3(2.0f, 0.0f, snowman5Pos_z);

// rotating reindeer
GLfloat rotate_reindeer_y = 0.0f;

// object starting positions
vec3 groundPos = vec3(0.0f, -3.9f, 0.0f);
vec3 treePos = vec3(5.0f, 0.2f, 2.0f);
vec3 arm1Pos = vec3(0.0f, 0.0f, 0.0f);
vec3 arm2Pos = vec3(0.0f, 0.0f, 0.0f);
vec3 armJoint1Pos = vec3(0.0f, 0.0f, 0.0f);
vec3 armJoint2Pos = vec3(0.0f, 0.0f, 0.0f);
float reindeerPos_z = 0.0f;
float reindeer2Pos_z = 90.0f;
float reindeer3Pos_z = 220.0f;
float reindeer4Pos_z = 120.0f;
float reindeer5Pos_z = 0.0f;
float reindeer6Pos_z = 220.0f;
float reindeerPos_y = 10.0f;
vec3 reindeerPos = vec3(8.0f, reindeerPos_y, 0.0f);
vec3 canePos = vec3(7.0f, -0.1f, -10.0f);
vec3 canePos2 = vec3(0.0f, -0.1f, -10.0f);
vec3 canePos3 = vec3(-7.0f, -0.1f, -10.0f);
vec3 lampPos = vec3(8.0f, 0.5f, -5.0f);

// animating the snowman arms, the hats and the canes
float armjoint1_z = 0.0f;
float armjoint2_z = 0.0f;
float armjoint1_z2 = 0.0f;
float armjoint2_z2 = 0.0f;
float armjoint1_z3 = 0.0f;
float armjoint2_z3 = 0.0f;
float armjoint1_z4 = 0.0f;
float armjoint2_z4 = 0.0f;
float armjoint1_z5 = 0.0f;
float armjoint2_z5 = 0.0f;
float arm_wave = 0.0f;
float hat_y = 0.0f;
float cane_y = 0.0f;

// textures
unsigned int terrain_texture;
unsigned int snow_texture;
unsigned int leather_texture;
unsigned int bark_texture;
unsigned int trunk_texture;
unsigned int carrot_texture;
unsigned int leaves_texture;
unsigned int reindeer_texture;
unsigned int bell_texture;
unsigned int cane_texture;
unsigned int lampMetal_texture;
unsigned int lampGlass_texture;

// VBOs and VAO
unsigned int VP_VBO[60]; // vertex positions
unsigned int VN_VBO[60]; // vertex normals
unsigned int VT_VBO[60]; // vertex textures
unsigned int VAO[3];

#pragma region MESH LOADING
/*----------------------------------------------------------------------------
MESH LOADING FUNCTION
----------------------------------------------------------------------------*/

// loading in meshes (models)
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

// adding a shader
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

// compiling shaders
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
	leavesData = load_mesh(LEAVES);
	trunkData = load_mesh(TRUNK);
	reindeerData = load_mesh(REINDEER);
	mainBellData = load_mesh(MAIN_BELL);
	harnessData = load_mesh(HARNESS);
	caneData = load_mesh(CANDYCANE);
	lampData = load_mesh(LAMPPOST);
	lampGlassData = load_mesh(LAMPLIGHT);

	// load textures
	terrain_texture = load_tex(TERRAIN);
	snow_texture = load_tex(SNOW_TEX);
	leather_texture = load_tex(LEATHER);
	bark_texture = load_tex(BARK);
	carrot_texture = load_tex(CARROT);
	leaves_texture = load_tex(LEAVES_TEX);
	trunk_texture = load_tex(TRUNKTEX);
	reindeer_texture = load_tex(REINDEER_TEX);
	bell_texture = load_tex(BELL_TEX);
	cane_texture = load_tex(CANDY_CANE_TEX);
	lampMetal_texture = load_tex(LAMP_METAL);
	lampGlass_texture = load_tex(LAMP_GLASS);

	// generate the buffers
	glGenBuffers(60, VP_VBO);
	glGenBuffers(60, VN_VBO);
	glGenBuffers(60, VT_VBO);

	// SNOWMAN 1!!!!!!!!!
	// main body of snowman
	loc1[0] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[1] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[2] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec2), &mesh_data.mTextureCoords[0], GL_STATIC_DRAW);
	// arm 1
	loc1[3] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[4] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[5] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, arm1Data.mPointCount * sizeof(vec3), &arm1Data.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, arm1Data.mPointCount * sizeof(vec3), &arm1Data.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, arm1Data.mPointCount * sizeof(vec2), &arm1Data.mTextureCoords[0], GL_STATIC_DRAW);
	// hat
	loc1[6] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[7] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[8] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, hatData.mPointCount * sizeof(vec3), &hatData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, hatData.mPointCount * sizeof(vec3), &hatData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, hatData.mPointCount * sizeof(vec2), &hatData.mTextureCoords[0], GL_STATIC_DRAW);
	// arm 2
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

	// SNOWMAN 2!!!!!!!!!
	// main body of snowman
	loc5[0] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc5[1] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc5[2] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[24]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[24]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[24]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec2), &mesh_data.mTextureCoords[0], GL_STATIC_DRAW);
	// arm 1
	loc5[3] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc5[4] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc5[5] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[25]);
	glBufferData(GL_ARRAY_BUFFER, arm1Data.mPointCount * sizeof(vec3), &arm1Data.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[25]);
	glBufferData(GL_ARRAY_BUFFER, arm1Data.mPointCount * sizeof(vec3), &arm1Data.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[25]);
	glBufferData(GL_ARRAY_BUFFER, arm1Data.mPointCount * sizeof(vec2), &arm1Data.mTextureCoords[0], GL_STATIC_DRAW);
	// hat
	loc5[6] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc5[7] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc5[8] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[26]);
	glBufferData(GL_ARRAY_BUFFER, hatData.mPointCount * sizeof(vec3), &hatData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[26]);
	glBufferData(GL_ARRAY_BUFFER, hatData.mPointCount * sizeof(vec3), &hatData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[26]);
	glBufferData(GL_ARRAY_BUFFER, hatData.mPointCount * sizeof(vec2), &hatData.mTextureCoords[0], GL_STATIC_DRAW);
	// arm 2
	loc5[12] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc5[13] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc5[14] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[27]);
	glBufferData(GL_ARRAY_BUFFER, arm2Data.mPointCount * sizeof(vec3), &arm2Data.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[27]);
	glBufferData(GL_ARRAY_BUFFER, arm2Data.mPointCount * sizeof(vec3), &arm2Data.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[27]);
	glBufferData(GL_ARRAY_BUFFER, arm2Data.mPointCount * sizeof(vec2), &arm2Data.mTextureCoords[0], GL_STATIC_DRAW);
	// carrot
	loc5[9] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc5[10] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc5[11] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[28]);
	glBufferData(GL_ARRAY_BUFFER, carrotData.mPointCount * sizeof(vec3), &carrotData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[28]);
	glBufferData(GL_ARRAY_BUFFER, carrotData.mPointCount * sizeof(vec3), &carrotData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[28]);
	glBufferData(GL_ARRAY_BUFFER, carrotData.mPointCount * sizeof(vec2), &carrotData.mTextureCoords[0], GL_STATIC_DRAW);
	//eyes
	loc5[15] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc5[16] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc5[17] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[29]);
	glBufferData(GL_ARRAY_BUFFER, eyesData.mPointCount * sizeof(vec3), &eyesData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[29]);
	glBufferData(GL_ARRAY_BUFFER, eyesData.mPointCount * sizeof(vec3), &eyesData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[29]);
	glBufferData(GL_ARRAY_BUFFER, eyesData.mPointCount * sizeof(vec2), &eyesData.mTextureCoords[0], GL_STATIC_DRAW);

	// SNOWMAN 3
	// main body
	loc6[0] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc6[1] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc6[2] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[30]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[30]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[30]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec2), &mesh_data.mTextureCoords[0], GL_STATIC_DRAW);
	// arm 1
	loc6[3] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc6[4] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc6[5] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[31]);
	glBufferData(GL_ARRAY_BUFFER, arm1Data.mPointCount * sizeof(vec3), &arm1Data.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[31]);
	glBufferData(GL_ARRAY_BUFFER, arm1Data.mPointCount * sizeof(vec3), &arm1Data.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[31]);
	glBufferData(GL_ARRAY_BUFFER, arm1Data.mPointCount * sizeof(vec2), &arm1Data.mTextureCoords[0], GL_STATIC_DRAW);
	// hat
	loc6[6] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc6[7] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc6[8] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[32]);
	glBufferData(GL_ARRAY_BUFFER, hatData.mPointCount * sizeof(vec3), &hatData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[32]);
	glBufferData(GL_ARRAY_BUFFER, hatData.mPointCount * sizeof(vec3), &hatData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[32]);
	glBufferData(GL_ARRAY_BUFFER, hatData.mPointCount * sizeof(vec2), &hatData.mTextureCoords[0], GL_STATIC_DRAW);
	// arm 2
	loc6[12] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc6[13] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc6[14] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[33]);
	glBufferData(GL_ARRAY_BUFFER, arm2Data.mPointCount * sizeof(vec3), &arm2Data.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[33]);
	glBufferData(GL_ARRAY_BUFFER, arm2Data.mPointCount * sizeof(vec3), &arm2Data.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[33]);
	glBufferData(GL_ARRAY_BUFFER, arm2Data.mPointCount * sizeof(vec2), &arm2Data.mTextureCoords[0], GL_STATIC_DRAW);
	// carrot
	loc6[9] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc6[10] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc6[11] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[34]);
	glBufferData(GL_ARRAY_BUFFER, carrotData.mPointCount * sizeof(vec3), &carrotData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[34]);
	glBufferData(GL_ARRAY_BUFFER, carrotData.mPointCount * sizeof(vec3), &carrotData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[34]);
	glBufferData(GL_ARRAY_BUFFER, carrotData.mPointCount * sizeof(vec2), &carrotData.mTextureCoords[0], GL_STATIC_DRAW);
	//eyes
	loc6[15] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc6[16] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc6[17] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[35]);
	glBufferData(GL_ARRAY_BUFFER, eyesData.mPointCount * sizeof(vec3), &eyesData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[35]);
	glBufferData(GL_ARRAY_BUFFER, eyesData.mPointCount * sizeof(vec3), &eyesData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[35]);
	glBufferData(GL_ARRAY_BUFFER, eyesData.mPointCount * sizeof(vec2), &eyesData.mTextureCoords[0], GL_STATIC_DRAW);

	// SNOWMAN 4!!!!!!!!!
	// main body of snowman
	loc7[0] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc7[1] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc7[2] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[45]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[45]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[45]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec2), &mesh_data.mTextureCoords[0], GL_STATIC_DRAW);
	// arm 1
	loc7[3] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc7[4] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc7[5] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[46]);
	glBufferData(GL_ARRAY_BUFFER, arm1Data.mPointCount * sizeof(vec3), &arm1Data.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[46]);
	glBufferData(GL_ARRAY_BUFFER, arm1Data.mPointCount * sizeof(vec3), &arm1Data.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[46]);
	glBufferData(GL_ARRAY_BUFFER, arm1Data.mPointCount * sizeof(vec2), &arm1Data.mTextureCoords[0], GL_STATIC_DRAW);
	// hat
	loc7[6] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc7[7] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc7[8] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[47]);
	glBufferData(GL_ARRAY_BUFFER, hatData.mPointCount * sizeof(vec3), &hatData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[47]);
	glBufferData(GL_ARRAY_BUFFER, hatData.mPointCount * sizeof(vec3), &hatData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[47]);
	glBufferData(GL_ARRAY_BUFFER, hatData.mPointCount * sizeof(vec2), &hatData.mTextureCoords[0], GL_STATIC_DRAW);
	// arm 2
	loc7[12] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc7[13] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc7[14] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[48]);
	glBufferData(GL_ARRAY_BUFFER, arm2Data.mPointCount * sizeof(vec3), &arm2Data.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[48]);
	glBufferData(GL_ARRAY_BUFFER, arm2Data.mPointCount * sizeof(vec3), &arm2Data.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[48]);
	glBufferData(GL_ARRAY_BUFFER, arm2Data.mPointCount * sizeof(vec2), &arm2Data.mTextureCoords[0], GL_STATIC_DRAW);
	// carrot
	loc7[9] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc7[10] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc7[11] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[49]);
	glBufferData(GL_ARRAY_BUFFER, carrotData.mPointCount * sizeof(vec3), &carrotData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[49]);
	glBufferData(GL_ARRAY_BUFFER, carrotData.mPointCount * sizeof(vec3), &carrotData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[49]);
	glBufferData(GL_ARRAY_BUFFER, carrotData.mPointCount * sizeof(vec2), &carrotData.mTextureCoords[0], GL_STATIC_DRAW);
	//eyes
	loc7[15] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc7[16] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc7[17] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[50]);
	glBufferData(GL_ARRAY_BUFFER, eyesData.mPointCount * sizeof(vec3), &eyesData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[50]);
	glBufferData(GL_ARRAY_BUFFER, eyesData.mPointCount * sizeof(vec3), &eyesData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[50]);
	glBufferData(GL_ARRAY_BUFFER, eyesData.mPointCount * sizeof(vec2), &eyesData.mTextureCoords[0], GL_STATIC_DRAW);

	// SNOWMAN 5!!!!!!!!!
	// main body of snowman
	loc8[0] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc8[1] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc8[2] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[51]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[51]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[51]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec2), &mesh_data.mTextureCoords[0], GL_STATIC_DRAW);
	// arm 1
	loc8[3] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc8[4] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc8[5] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[52]);
	glBufferData(GL_ARRAY_BUFFER, arm1Data.mPointCount * sizeof(vec3), &arm1Data.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[52]);
	glBufferData(GL_ARRAY_BUFFER, arm1Data.mPointCount * sizeof(vec3), &arm1Data.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[52]);
	glBufferData(GL_ARRAY_BUFFER, arm1Data.mPointCount * sizeof(vec2), &arm1Data.mTextureCoords[0], GL_STATIC_DRAW);
	// hat
	loc8[6] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc8[7] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc8[8] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[53]);
	glBufferData(GL_ARRAY_BUFFER, hatData.mPointCount * sizeof(vec3), &hatData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[53]);
	glBufferData(GL_ARRAY_BUFFER, hatData.mPointCount * sizeof(vec3), &hatData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[53]);
	glBufferData(GL_ARRAY_BUFFER, hatData.mPointCount * sizeof(vec2), &hatData.mTextureCoords[0], GL_STATIC_DRAW);
	// arm 2
	loc8[12] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc8[13] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc8[14] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[54]);
	glBufferData(GL_ARRAY_BUFFER, arm2Data.mPointCount * sizeof(vec3), &arm2Data.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[54]);
	glBufferData(GL_ARRAY_BUFFER, arm2Data.mPointCount * sizeof(vec3), &arm2Data.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[54]);
	glBufferData(GL_ARRAY_BUFFER, arm2Data.mPointCount * sizeof(vec2), &arm2Data.mTextureCoords[0], GL_STATIC_DRAW);
	// carrot
	loc8[9] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc8[10] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc8[11] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[55]);
	glBufferData(GL_ARRAY_BUFFER, carrotData.mPointCount * sizeof(vec3), &carrotData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[55]);
	glBufferData(GL_ARRAY_BUFFER, carrotData.mPointCount * sizeof(vec3), &carrotData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[55]);
	glBufferData(GL_ARRAY_BUFFER, carrotData.mPointCount * sizeof(vec2), &carrotData.mTextureCoords[0], GL_STATIC_DRAW);
	//eyes
	loc8[15] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc8[16] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc8[17] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[56]);
	glBufferData(GL_ARRAY_BUFFER, eyesData.mPointCount * sizeof(vec3), &eyesData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[56]);
	glBufferData(GL_ARRAY_BUFFER, eyesData.mPointCount * sizeof(vec3), &eyesData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[56]);
	glBufferData(GL_ARRAY_BUFFER, eyesData.mPointCount * sizeof(vec2), &eyesData.mTextureCoords[0], GL_STATIC_DRAW);

	// REINDEER BODY 1
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

	// REINDEER 3
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

	// REINDEER 4
	// REINDEER BODY
	loc4[18] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc4[19] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc4[20] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[36]);
	glBufferData(GL_ARRAY_BUFFER, reindeerData.mPointCount * sizeof(vec3), &reindeerData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[36]);
	glBufferData(GL_ARRAY_BUFFER, reindeerData.mPointCount * sizeof(vec3), &reindeerData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[36]);
	glBufferData(GL_ARRAY_BUFFER, reindeerData.mPointCount * sizeof(vec2), &reindeerData.mTextureCoords[0], GL_STATIC_DRAW);

	// REINDEER 5
	// REINDEER BODY
	loc4[21] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc4[22] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc4[23] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[37]);
	glBufferData(GL_ARRAY_BUFFER, reindeerData.mPointCount * sizeof(vec3), &reindeerData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[37]);
	glBufferData(GL_ARRAY_BUFFER, reindeerData.mPointCount * sizeof(vec3), &reindeerData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[37]);
	glBufferData(GL_ARRAY_BUFFER, reindeerData.mPointCount * sizeof(vec2), &reindeerData.mTextureCoords[0], GL_STATIC_DRAW);

	// REINDEER 6
	// REINDEER BODY
	loc3[18] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc3[19] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc3[20] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[38]);
	glBufferData(GL_ARRAY_BUFFER, reindeerData.mPointCount * sizeof(vec3), &reindeerData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[38]);
	glBufferData(GL_ARRAY_BUFFER, reindeerData.mPointCount * sizeof(vec3), &reindeerData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[38]);
	glBufferData(GL_ARRAY_BUFFER, reindeerData.mPointCount * sizeof(vec2), &reindeerData.mTextureCoords[0], GL_STATIC_DRAW);

	// terrain
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

	// CANDY CANE
	loc2[6] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2[7] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc2[8] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[20]);
	glBufferData(GL_ARRAY_BUFFER, caneData.mPointCount * sizeof(vec3), &caneData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[20]);
	glBufferData(GL_ARRAY_BUFFER, caneData.mPointCount * sizeof(vec3), &caneData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[20]);
	glBufferData(GL_ARRAY_BUFFER, caneData.mPointCount * sizeof(vec2), &caneData.mTextureCoords[0], GL_STATIC_DRAW);

	// CANDY CANE
	loc2[9] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2[10] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc2[11] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[21]);
	glBufferData(GL_ARRAY_BUFFER, caneData.mPointCount * sizeof(vec3), &caneData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[21]);
	glBufferData(GL_ARRAY_BUFFER, caneData.mPointCount * sizeof(vec3), &caneData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[21]);
	glBufferData(GL_ARRAY_BUFFER, caneData.mPointCount * sizeof(vec2), &caneData.mTextureCoords[0], GL_STATIC_DRAW);

	// LAMP WITHOUT GLASS
	loc2[12] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2[13] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc2[14] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[22]);
	glBufferData(GL_ARRAY_BUFFER, lampData.mPointCount * sizeof(vec3), &lampData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[22]);
	glBufferData(GL_ARRAY_BUFFER, lampData.mPointCount * sizeof(vec3), &lampData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[22]);
	glBufferData(GL_ARRAY_BUFFER, lampData.mPointCount * sizeof(vec2), &lampData.mTextureCoords[0], GL_STATIC_DRAW);

	// LAMP GLASS
	loc2[15] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2[16] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc2[17] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[23]);
	glBufferData(GL_ARRAY_BUFFER, lampGlassData.mPointCount * sizeof(vec3), &lampGlassData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[23]);
	glBufferData(GL_ARRAY_BUFFER, lampGlassData.mPointCount * sizeof(vec3), &lampGlassData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[23]);
	glBufferData(GL_ARRAY_BUFFER, lampGlassData.mPointCount * sizeof(vec2), &lampGlassData.mTextureCoords[0], GL_STATIC_DRAW);

	// LEAVES
	loc2[18] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2[19] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc2[20] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[39]);
	glBufferData(GL_ARRAY_BUFFER, leavesData.mPointCount * sizeof(vec3), &leavesData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[39]);
	glBufferData(GL_ARRAY_BUFFER, leavesData.mPointCount * sizeof(vec3), &leavesData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[39]);
	glBufferData(GL_ARRAY_BUFFER, leavesData.mPointCount * sizeof(vec2), &leavesData.mTextureCoords[0], GL_STATIC_DRAW);

	// TRUNK
	loc2[21] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2[22] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc2[23] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[40]);
	glBufferData(GL_ARRAY_BUFFER, trunkData.mPointCount * sizeof(vec3), &trunkData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[40]);
	glBufferData(GL_ARRAY_BUFFER, trunkData.mPointCount * sizeof(vec3), &trunkData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[40]);
	glBufferData(GL_ARRAY_BUFFER, trunkData.mPointCount * sizeof(vec2), &trunkData.mTextureCoords[0], GL_STATIC_DRAW); 

	// LEAVES
	loc1[18] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[19] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[20] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[41]);
	glBufferData(GL_ARRAY_BUFFER, leavesData.mPointCount * sizeof(vec3), &leavesData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[41]);
	glBufferData(GL_ARRAY_BUFFER, leavesData.mPointCount * sizeof(vec3), &leavesData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[41]);
	glBufferData(GL_ARRAY_BUFFER, leavesData.mPointCount * sizeof(vec2), &leavesData.mTextureCoords[0], GL_STATIC_DRAW);

	// TRUNK
	loc1[21] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc1[22] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc1[23] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[42]);
	glBufferData(GL_ARRAY_BUFFER, trunkData.mPointCount * sizeof(vec3), &trunkData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[42]);
	glBufferData(GL_ARRAY_BUFFER, trunkData.mPointCount * sizeof(vec3), &trunkData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[42]);
	glBufferData(GL_ARRAY_BUFFER, trunkData.mPointCount * sizeof(vec2), &trunkData.mTextureCoords[0], GL_STATIC_DRAW);

	// LEAVES
	loc5[18] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc5[19] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc5[20] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[43]);
	glBufferData(GL_ARRAY_BUFFER, leavesData.mPointCount * sizeof(vec3), &leavesData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[43]);
	glBufferData(GL_ARRAY_BUFFER, leavesData.mPointCount * sizeof(vec3), &leavesData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[43]);
	glBufferData(GL_ARRAY_BUFFER, leavesData.mPointCount * sizeof(vec2), &leavesData.mTextureCoords[0], GL_STATIC_DRAW);

	// TRUNK
	loc5[21] = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc5[22] = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc5[23] = glGetAttribLocation(shaderProgramID, "vertex_texture");
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[44]);
	glBufferData(GL_ARRAY_BUFFER, trunkData.mPointCount * sizeof(vec3), &trunkData.mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[44]);
	glBufferData(GL_ARRAY_BUFFER, trunkData.mPointCount * sizeof(vec3), &trunkData.mNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[44]);
	glBufferData(GL_ARRAY_BUFFER, trunkData.mPointCount * sizeof(vec2), &trunkData.mTextureCoords[0], GL_STATIC_DRAW);

}
#pragma endregion VBO_FUNCTIONS

void display() {

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// NEW
	glUseProgram(shaderProgramID);
	//baseShader.setVec3("viewPos", camera.Position);

	//Declare your uniform variables that will be used in your shader
	GLuint proj_mat_location = glGetUniformLocation(shaderProgramID, "proj");
	GLuint view_mat_location = glGetUniformLocation(shaderProgramID, "view");
	GLuint matrix_location = glGetUniformLocation(shaderProgramID, "model");
	GLuint color_location = glGetUniformLocation(shaderProgramID, "color");
	//GLuint ambient_location = glGetUniformLocation(shaderProgramID, "material.ambient");
	GLuint diffuse_location = glGetUniformLocation(shaderProgramID, "material.diffuse");
	GLuint specular_location = glGetUniformLocation(shaderProgramID, "material.specular");
	GLuint shininess_location = glGetUniformLocation(shaderProgramID, "material.shininess");
	// camera position
	GLuint camPos = glGetUniformLocation(shaderProgramID, "viewPos");
	glUniform3fv(camPos, 1, cameraPos.v);
	// set up the lights
	// LIGHT 0
	GLuint light0_location = glGetUniformLocation(shaderProgramID, "pointLights[0].position");
	GLuint light0_ambient = glGetUniformLocation(shaderProgramID, "pointLights[0].ambient");
	GLuint light0_diffuse = glGetUniformLocation(shaderProgramID, "pointLights[0].diffuse");
	GLuint light0_specular = glGetUniformLocation(shaderProgramID, "pointLights[0].specular");
	GLuint light0_constant = glGetUniformLocation(shaderProgramID, "Light[0].constant");
	GLuint light0_linear = glGetUniformLocation(shaderProgramID, "Light[0].linear");
	GLuint light0_quadratic = glGetUniformLocation(shaderProgramID, "Light[0].quadratic");
	vec3 light0Position = vec3(1.0f, 15.0f, -5.0f);
	vec3 lightAmbient = vec3(0.1f, 0.1f, 0.1f);
	vec3 lightDiffuse = vec3(0.8f, 0.8f, 0.8f);
	vec3 lightSpecular = vec3(0.8f, 0.8f, 0.8f);
	float lightConstant = 1.0f;
	float lightLinear = 0.09f;
	float lightQuadratic = 0.032f;
	glUniform3fv(light0_location, 1, light0Position.v);
	glUniform3fv(light0_ambient, 1, lightAmbient.v);
	glUniform3fv(light0_diffuse, 1, lightDiffuse.v);
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
	vec3 light1Position = vec3(-10.0f, 4.5f, -2.0f);
	glUniform3fv(light1_location, 1, light1Position.v);
	glUniform3fv(light1_ambient, 1, lightAmbient.v);
	glUniform3fv(light1_diffuse, 1, lightDiffuse.v);
	glUniform3fv(light1_specular, 1, lightSpecular.v);
	glUniform1f(light1_constant, lightConstant);
	glUniform1f(light1_linear, lightLinear);
	glUniform1f(light1_quadratic, lightQuadratic);
	// LIGHT 2
	GLuint light2_location = glGetUniformLocation(shaderProgramID, "pointLights[2].position");
	GLuint light2_ambient = glGetUniformLocation(shaderProgramID, "pointLights[2].ambient");
	GLuint light2_diffuse = glGetUniformLocation(shaderProgramID, "pointLights[2].diffuse");
	GLuint light2_specular = glGetUniformLocation(shaderProgramID, "pointLights[2].specular");
	GLuint light2_constant = glGetUniformLocation(shaderProgramID, "Light[2].constant");
	GLuint light2_linear = glGetUniformLocation(shaderProgramID, "Light[2].linear");
	GLuint light2_quadratic = glGetUniformLocation(shaderProgramID, "Light[2].quadratic");
	vec3 light2Position = vec3(5.0f, 8.0f, 0.0f);
	glUniform3fv(light2_location, 2, light2Position.v);
	glUniform3fv(light2_ambient, 2, lightAmbient.v);
	glUniform3fv(light2_diffuse, 2, lightDiffuse.v);
	glUniform3fv(light2_specular, 2, lightSpecular.v);
	glUniform1f(light2_constant, lightConstant);
	glUniform1f(light2_linear, lightLinear);
	glUniform1f(light2_quadratic, lightQuadratic);

	// Root of the Hierarchy
	mat4 projection = perspective(fov, (float)width / (float)height, 0.1f, 1000.0f);
	// Switch between Ortho and Perspective views based on whether persp is true or false
	
	// set up view
	mat4 view = identity_mat4();
	view = look_at(cameraPos, cameraPos + cameraFront, cameraUp);

	// update uniforms & draw
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, projection.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);

	// translating, rotating and scaling the SNOWMAN 1
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
	vec3 specular = vec3(0.5f, 0.5f, 0.5f);
	float shininess = 2.0f;
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

	// SNOWMAN 2!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// translating, rotating and scaling the main model
	mat4 model2 = identity_mat4();
	model2 = translate(model2, translate_base);
	model2 = rotate_z_deg(model2, rotate_z);
	model2 = rotate_y_deg(model2, rotate_y2);
	model2 = rotate_x_deg(model2, rotate_x);
	model2 = translate(model2, snowman2Pos);
	model2 = scale(model2, scaling);
	//vec3 ambient = vec3(0.9f, 0.9f, 0.9f);
	diffuse = vec3(1.0f, 1.0f, 1.0f);
	shininess = 2.0f;
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model2.m);
	glEnableVertexAttribArray(loc5[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[24]);
	glVertexAttribPointer(loc5[0], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc5[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[24]);
	glVertexAttribPointer(loc5[1], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	// texture - snow
	glEnableVertexAttribArray(loc5[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[24]);
	glVertexAttribPointer(loc5[2], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, snow_texture);
	glDrawArrays(GL_TRIANGLES, 0, mesh_data.mPointCount);

	// child -- hierarchical arms - arm1
	mat4 modelChild2Arm1 = identity_mat4();
	modelChild2Arm1 = rotate_z_deg(modelChild2Arm1, armjoint1_z2);
	modelChild2Arm1 = rotate_y_deg(modelChild2Arm1, 0.0f);
	modelChild2Arm1 = rotate_x_deg(modelChild2Arm1, 0.0f);
	modelChild2Arm1 = translate(modelChild2Arm1, arm1Pos);
	modelChild2Arm1 = model2 * modelChild2Arm1;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelChild2Arm1.m);
	glEnableVertexAttribArray(loc5[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[25]);
	glVertexAttribPointer(loc5[3], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc5[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[25]);
	glVertexAttribPointer(loc5[4], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc5[5]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[25]);
	glVertexAttribPointer(loc5[5], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, bark_texture);
	glDrawArrays(GL_TRIANGLES, 0, arm1Data.mPointCount);

	// child -- hierarchical arms - arm2
	mat4 modelChild2Arm2 = identity_mat4();
	modelChild2Arm2 = rotate_z_deg(modelChild2Arm2, armjoint2_z2);
	modelChild2Arm2 = rotate_y_deg(modelChild2Arm2, 0.0f);
	modelChild2Arm2 = rotate_x_deg(modelChild2Arm2, 0.0f);
	modelChild2Arm2 = translate(modelChild2Arm2, arm2Pos);
	modelChild2Arm2 = model2 * modelChild2Arm2;
	color = vec3(0.5f, 0.25f, 0.0f);
	glUniform3fv(color_location, 1, color.v);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelChild2Arm2.m);
	glEnableVertexAttribArray(loc5[9]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[27]);
	glVertexAttribPointer(loc5[9], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc5[10]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[27]);
	glVertexAttribPointer(loc5[10], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_TRIANGLES, 0, arm2Data.mPointCount);

	// hat -- hierarchical
	mat4 modelHat2 = identity_mat4();
	modelHat2 = rotate_z_deg(modelHat2, 0.0f);
	modelHat2 = rotate_y_deg(modelHat2, hat_y);
	modelHat2 = rotate_x_deg(modelHat2, 0.0f);
	modelHat2 = translate(modelHat2, vec3(0.0f, 0.0f, 0.0f));
	modelHat2 = model2 * modelHat2;
	//ambient = vec3(0.8f, 0.8f, 0.8f);
	diffuse = vec3(0.51f, 0.51f, 0.51f);
	specular = vec3(0.5f, 0.5f, 0.5f);
	shininess = 40.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelHat2.m);
	glEnableVertexAttribArray(loc5[6]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[26]);
	glVertexAttribPointer(loc5[6], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc5[7]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[26]);
	glVertexAttribPointer(loc5[7], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc5[8]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[26]);
	glVertexAttribPointer(loc5[8], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, leather_texture);
	glDrawArrays(GL_TRIANGLES, 0, hatData.mPointCount);

	// carrot
	mat4 modelCarrot2 = identity_mat4();
	modelCarrot2 = model2 * modelCarrot2;
	//ambient = vec3(0.8f, 0.8f, 0.8f);
	diffuse = vec3(0.4f, 0.4f, 0.4f);
	specular = vec3(1.0f, 1.0f, 1.0f);
	shininess = 100.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelCarrot2.m);
	glEnableVertexAttribArray(loc5[12]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[28]);
	glVertexAttribPointer(loc5[12], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc5[13]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[28]);
	glVertexAttribPointer(loc5[13], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc5[14]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[28]);
	glVertexAttribPointer(loc5[14], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, carrot_texture);
	glDrawArrays(GL_TRIANGLES, 0, carrotData.mPointCount);

	// eyes
	mat4 modelEyes2 = identity_mat4();
	modelEyes2 = model2 * modelEyes2;
	//ambient = vec3(0.05f, 0.05f, 0.06f);
	diffuse = vec3(0.18f, 0.17f, 0.225f);
	specular = vec3(0.99f, 0.99f, 0.99f);
	shininess = 100.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelEyes2.m);
	glEnableVertexAttribArray(loc5[15]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[29]);
	glVertexAttribPointer(loc5[15], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc5[16]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[29]);
	glVertexAttribPointer(loc5[16], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc5[17]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[29]);
	glVertexAttribPointer(loc5[17], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, leather_texture);
	glDrawArrays(GL_TRIANGLES, 0, eyesData.mPointCount);

	// SNOWMAN 3!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// translating, rotating and scaling the main model
	mat4 model3 = identity_mat4();
	model3 = translate(model3, translate_base);
	model3 = rotate_z_deg(model3, rotate_z);
	model3 = rotate_y_deg(model3, rotate_y3);
	model3 = rotate_x_deg(model3, rotate_x);
	model3 = translate(model3, snowman3Pos);
	model3 = scale(model3, scaling);
	//vec3 ambient = vec3(0.9f, 0.9f, 0.9f);
	diffuse = vec3(1.0f, 1.0f, 1.0f);
	shininess = 2.0f;
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model3.m);
	glEnableVertexAttribArray(loc6[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[30]);
	glVertexAttribPointer(loc6[0], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc6[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[30]);
	glVertexAttribPointer(loc6[1], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	// texture - snow
	glEnableVertexAttribArray(loc6[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[30]);
	glVertexAttribPointer(loc6[2], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, snow_texture);
	glDrawArrays(GL_TRIANGLES, 0, mesh_data.mPointCount);

	// child -- hierarchical arms - arm1
	mat4 modelChild3Arm1 = identity_mat4();
	modelChild3Arm1 = rotate_z_deg(modelChild3Arm1, armjoint1_z3);
	modelChild3Arm1 = rotate_y_deg(modelChild3Arm1, 0.0f);
	modelChild3Arm1 = rotate_x_deg(modelChild3Arm1, 0.0f);
	modelChild3Arm1 = translate(modelChild3Arm1, arm1Pos);
	modelChild3Arm1 = model3 * modelChild3Arm1;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelChild3Arm1.m);
	glEnableVertexAttribArray(loc6[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[31]);
	glVertexAttribPointer(loc6[3], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc6[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[31]);
	glVertexAttribPointer(loc6[4], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc6[5]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[31]);
	glVertexAttribPointer(loc6[5], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, bark_texture);
	glDrawArrays(GL_TRIANGLES, 0, arm1Data.mPointCount);

	// child -- hierarchical arms - arm2
	mat4 modelChild3Arm2 = identity_mat4();
	modelChild3Arm2 = rotate_z_deg(modelChild3Arm2, armjoint2_z3);
	modelChild3Arm2 = rotate_y_deg(modelChild3Arm2, 0.0f);
	modelChild3Arm2 = rotate_x_deg(modelChild3Arm2, 0.0f);
	modelChild3Arm2 = translate(modelChild3Arm2, arm2Pos);
	modelChild3Arm2 = model3 * modelChild3Arm2;
	color = vec3(0.5f, 0.25f, 0.0f);
	glUniform3fv(color_location, 1, color.v);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelChild3Arm2.m);
	glEnableVertexAttribArray(loc6[9]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[33]);
	glVertexAttribPointer(loc6[9], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc6[10]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[33]);
	glVertexAttribPointer(loc6[10], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_TRIANGLES, 0, arm2Data.mPointCount);

	// hat -- hierarchical
	mat4 modelHat3 = identity_mat4();
	modelHat3 = rotate_z_deg(modelHat3, 0.0f);
	modelHat3 = rotate_y_deg(modelHat3, hat_y);
	modelHat3 = rotate_x_deg(modelHat3, 0.0f);
	modelHat3 = translate(modelHat3, vec3(0.0f, 0.0f, 0.0f));
	modelHat3 = model3 * modelHat3;
	//ambient = vec3(0.8f, 0.8f, 0.8f);
	diffuse = vec3(0.51f, 0.51f, 0.51f);
	specular = vec3(0.5f, 0.5f, 0.5f);
	shininess = 40.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelHat3.m);
	glEnableVertexAttribArray(loc6[6]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[32]);
	glVertexAttribPointer(loc6[6], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc6[7]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[32]);
	glVertexAttribPointer(loc6[7], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc6[8]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[32]);
	glVertexAttribPointer(loc6[8], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, leather_texture);
	glDrawArrays(GL_TRIANGLES, 0, hatData.mPointCount);

	// carrot
	mat4 modelCarrot3 = identity_mat4();
	modelCarrot3 = model3 * modelCarrot3;
	//ambient = vec3(0.8f, 0.8f, 0.8f);
	diffuse = vec3(0.4f, 0.4f, 0.4f);
	specular = vec3(1.0f, 1.0f, 1.0f);
	shininess = 100.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelCarrot3.m);
	glEnableVertexAttribArray(loc6[12]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[34]);
	glVertexAttribPointer(loc6[12], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc6[13]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[34]);
	glVertexAttribPointer(loc6[13], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc6[14]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[34]);
	glVertexAttribPointer(loc6[14], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, carrot_texture);
	glDrawArrays(GL_TRIANGLES, 0, carrotData.mPointCount);

	// eyes
	mat4 modelEyes3 = identity_mat4();
	modelEyes3 = model3 * modelEyes3;
	//ambient = vec3(0.05f, 0.05f, 0.06f);
	diffuse = vec3(0.18f, 0.17f, 0.225f);
	specular = vec3(0.99f, 0.99f, 0.99f);
	shininess = 100.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelEyes3.m);
	glEnableVertexAttribArray(loc6[15]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[35]);
	glVertexAttribPointer(loc6[15], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc6[16]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[35]);
	glVertexAttribPointer(loc6[16], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc6[17]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[35]);
	glVertexAttribPointer(loc6[17], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, leather_texture);
	glDrawArrays(GL_TRIANGLES, 0, eyesData.mPointCount);

	// SNOWMAN 4!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// translating, rotating and scaling the main model
	mat4 model4 = identity_mat4();
	model4 = translate(model4, translate_base);
	model4 = rotate_z_deg(model4, rotate_z);
	model4 = rotate_y_deg(model4, rotate_y4);
	model4 = rotate_x_deg(model4, rotate_x);
	model4 = translate(model4, snowman4Pos);
	model4 = scale(model4, scaling);
	//vec3 ambient = vec3(0.9f, 0.9f, 0.9f);
	diffuse = vec3(1.0f, 1.0f, 1.0f);
	shininess = 2.0f;
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model4.m);
	glEnableVertexAttribArray(loc7[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[45]);
	glVertexAttribPointer(loc7[0], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc7[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[45]);
	glVertexAttribPointer(loc7[1], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	// texture - snow
	glEnableVertexAttribArray(loc7[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[45]);
	glVertexAttribPointer(loc7[2], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, snow_texture);
	glDrawArrays(GL_TRIANGLES, 0, mesh_data.mPointCount);

	// child -- hierarchical arms - arm1
	mat4 modelChild4Arm1 = identity_mat4();
	modelChild4Arm1 = rotate_z_deg(modelChild4Arm1, armjoint1_z4);
	modelChild4Arm1 = rotate_y_deg(modelChild4Arm1, 0.0f);
	modelChild4Arm1 = rotate_x_deg(modelChild4Arm1, 0.0f);
	modelChild4Arm1 = translate(modelChild4Arm1, arm1Pos);
	modelChild4Arm1 = model4 * modelChild4Arm1;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelChild4Arm1.m);
	glEnableVertexAttribArray(loc7[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[46]);
	glVertexAttribPointer(loc7[3], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc7[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[46]);
	glVertexAttribPointer(loc7[4], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc7[5]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[46]);
	glVertexAttribPointer(loc7[5], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, bark_texture);
	glDrawArrays(GL_TRIANGLES, 0, arm1Data.mPointCount);

	// child -- hierarchical arms - arm2
	mat4 modelChild4Arm2 = identity_mat4();
	modelChild4Arm2 = rotate_z_deg(modelChild4Arm2, armjoint2_z4);
	modelChild4Arm2 = rotate_y_deg(modelChild4Arm2, 0.0f);
	modelChild4Arm2 = rotate_x_deg(modelChild4Arm2, 0.0f);
	modelChild4Arm2 = translate(modelChild4Arm2, arm2Pos);
	modelChild4Arm2 = model4 * modelChild4Arm2;
	color = vec3(0.5f, 0.25f, 0.0f);
	glUniform3fv(color_location, 1, color.v);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelChild4Arm2.m);
	glEnableVertexAttribArray(loc7[9]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[48]);
	glVertexAttribPointer(loc7[9], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc7[10]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[48]);
	glVertexAttribPointer(loc7[10], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_TRIANGLES, 0, arm2Data.mPointCount);

	// hat -- hierarchical
	mat4 modelHat4 = identity_mat4();
	modelHat4 = rotate_z_deg(modelHat4, 0.0f);
	modelHat4 = rotate_y_deg(modelHat4, hat_y);
	modelHat4 = rotate_x_deg(modelHat4, 0.0f);
	modelHat4 = translate(modelHat4, vec3(0.0f, 0.0f, 0.0f));
	modelHat4 = model4 * modelHat4;
	//ambient = vec3(0.8f, 0.8f, 0.8f);
	diffuse = vec3(0.51f, 0.51f, 0.51f);
	specular = vec3(0.5f, 0.5f, 0.5f);
	shininess = 40.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelHat4.m);
	glEnableVertexAttribArray(loc7[6]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[47]);
	glVertexAttribPointer(loc7[6], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc7[7]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[47]);
	glVertexAttribPointer(loc7[7], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc7[8]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[47]);
	glVertexAttribPointer(loc7[8], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, leather_texture);
	glDrawArrays(GL_TRIANGLES, 0, hatData.mPointCount);

	// carrot
	mat4 modelCarrot4 = identity_mat4();
	modelCarrot4 = model4 * modelCarrot4;
	//ambient = vec3(0.8f, 0.8f, 0.8f);
	diffuse = vec3(0.4f, 0.4f, 0.4f);
	specular = vec3(1.0f, 1.0f, 1.0f);
	shininess = 100.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelCarrot4.m);
	glEnableVertexAttribArray(loc7[12]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[49]);
	glVertexAttribPointer(loc7[12], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc7[13]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[49]);
	glVertexAttribPointer(loc7[13], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc7[14]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[49]);
	glVertexAttribPointer(loc7[14], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, carrot_texture);
	glDrawArrays(GL_TRIANGLES, 0, carrotData.mPointCount);

	// eyes
	mat4 modelEyes4 = identity_mat4();
	modelEyes4 = model4 * modelEyes4;
	//ambient = vec3(0.05f, 0.05f, 0.06f);
	diffuse = vec3(0.18f, 0.17f, 0.225f);
	specular = vec3(0.99f, 0.99f, 0.99f);
	shininess = 100.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelEyes4.m);
	glEnableVertexAttribArray(loc7[15]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[50]);
	glVertexAttribPointer(loc7[15], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc7[16]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[50]);
	glVertexAttribPointer(loc7[16], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc7[17]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[50]);
	glVertexAttribPointer(loc7[17], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, leather_texture);
	glDrawArrays(GL_TRIANGLES, 0, eyesData.mPointCount);

	// SNOWMAN 5!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// translating, rotating and scaling the main model
	mat4 model5 = identity_mat4();
	model5 = translate(model5, translate_base);
	model5 = rotate_z_deg(model5, rotate_z);
	model5 = rotate_y_deg(model5, rotate_y5);
	model5 = rotate_x_deg(model5, rotate_x);
	model5 = translate(model5, snowman5Pos);
	model5 = scale(model5, scaling);
	//vec3 ambient = vec3(0.9f, 0.9f, 0.9f);
	diffuse = vec3(1.0f, 1.0f, 1.0f);
	shininess = 2.0f;
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model5.m);
	glEnableVertexAttribArray(loc8[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[51]);
	glVertexAttribPointer(loc8[0], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc8[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[51]);
	glVertexAttribPointer(loc8[1], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	// texture - snow
	glEnableVertexAttribArray(loc8[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[51]);
	glVertexAttribPointer(loc8[2], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, snow_texture);
	glDrawArrays(GL_TRIANGLES, 0, mesh_data.mPointCount);

	// child -- hierarchical arms - arm1
	mat4 modelChild5Arm1 = identity_mat4();
	modelChild5Arm1 = rotate_z_deg(modelChild5Arm1, armjoint1_z5);
	modelChild5Arm1 = rotate_y_deg(modelChild5Arm1, 0.0f);
	modelChild5Arm1 = rotate_x_deg(modelChild5Arm1, 0.0f);
	modelChild5Arm1 = translate(modelChild5Arm1, arm1Pos);
	modelChild5Arm1 = model5 * modelChild5Arm1;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelChild5Arm1.m);
	glEnableVertexAttribArray(loc8[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[52]);
	glVertexAttribPointer(loc8[3], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc8[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[52]);
	glVertexAttribPointer(loc8[4], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc8[5]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[52]);
	glVertexAttribPointer(loc8[5], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, bark_texture);
	glDrawArrays(GL_TRIANGLES, 0, arm1Data.mPointCount);

	// child -- hierarchical arms - arm2
	mat4 modelChild5Arm2 = identity_mat4();
	modelChild5Arm2 = rotate_z_deg(modelChild5Arm2, armjoint2_z5);
	modelChild5Arm2 = rotate_y_deg(modelChild5Arm2, 0.0f);
	modelChild5Arm2 = rotate_x_deg(modelChild5Arm2, 0.0f);
	modelChild5Arm2 = translate(modelChild5Arm2, arm2Pos);
	modelChild5Arm2 = model5 * modelChild5Arm2;
	color = vec3(0.5f, 0.25f, 0.0f);
	glUniform3fv(color_location, 1, color.v);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelChild5Arm2.m);
	glEnableVertexAttribArray(loc8[9]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[54]);
	glVertexAttribPointer(loc8[9], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc8[10]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[54]);
	glVertexAttribPointer(loc8[10], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_TRIANGLES, 0, arm2Data.mPointCount);

	// hat -- hierarchical
	mat4 modelHat5 = identity_mat4();
	modelHat5 = rotate_z_deg(modelHat5, 0.0f);
	modelHat5 = rotate_y_deg(modelHat5, hat_y);
	modelHat5 = rotate_x_deg(modelHat5, 0.0f);
	modelHat5 = translate(modelHat5, vec3(0.0f, 0.0f, 0.0f));
	modelHat5 = model5 * modelHat5;
	//ambient = vec3(0.8f, 0.8f, 0.8f);
	diffuse = vec3(0.51f, 0.51f, 0.51f);
	specular = vec3(0.5f, 0.5f, 0.5f);
	shininess = 40.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelHat5.m);
	glEnableVertexAttribArray(loc8[6]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[53]);
	glVertexAttribPointer(loc8[6], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc8[7]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[53]);
	glVertexAttribPointer(loc8[7], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc8[8]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[53]);
	glVertexAttribPointer(loc8[8], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, leather_texture);
	glDrawArrays(GL_TRIANGLES, 0, hatData.mPointCount);

	// carrot
	mat4 modelCarrot5 = identity_mat4();
	modelCarrot5 = model5 * modelCarrot5;
	//ambient = vec3(0.8f, 0.8f, 0.8f);
	diffuse = vec3(0.4f, 0.4f, 0.4f);
	specular = vec3(1.0f, 1.0f, 1.0f);
	shininess = 100.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelCarrot5.m);
	glEnableVertexAttribArray(loc8[12]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[55]);
	glVertexAttribPointer(loc8[12], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc8[13]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[55]);
	glVertexAttribPointer(loc8[13], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc8[14]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[55]);
	glVertexAttribPointer(loc8[14], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, carrot_texture);
	glDrawArrays(GL_TRIANGLES, 0, carrotData.mPointCount);

	// eyes
	mat4 modelEyes5 = identity_mat4();
	modelEyes5 = model5 * modelEyes5;
	//ambient = vec3(0.05f, 0.05f, 0.06f);
	diffuse = vec3(0.18f, 0.17f, 0.225f);
	specular = vec3(0.99f, 0.99f, 0.99f);
	shininess = 100.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelEyes5.m);
	glEnableVertexAttribArray(loc8[15]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[56]);
	glVertexAttribPointer(loc8[15], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc8[16]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[56]);
	glVertexAttribPointer(loc8[16], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc8[17]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[56]);
	glVertexAttribPointer(loc8[17], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, leather_texture);
	glDrawArrays(GL_TRIANGLES, 0, eyesData.mPointCount);

	// reindeer
	mat4 modelReindeer = identity_mat4();
	modelReindeer = translate(modelReindeer, vec3(10.0f, reindeerPos_y, 0.0f));
	modelReindeer = scale(modelReindeer, vec3(0.6f, 0.6f, 0.6f));
	modelReindeer = rotate_y_deg(modelReindeer, reindeerPos_z);
	modelReindeer = translate(modelReindeer, vec3(10.0f, 0.0f, 0.0f));
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
	modelReindeer2 = translate(modelReindeer2, vec3(5.0f, reindeerPos_y, 0.0f));
	modelReindeer2 = scale(modelReindeer2, vec3(0.6f, 0.6f, 0.6f));
	modelReindeer2 = rotate_y_deg(modelReindeer2, reindeer2Pos_z);
	modelReindeer2 = translate(modelReindeer2, vec3(10.0f, 0.0f, 0.0f));
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
	modelReindeer3 = translate(modelReindeer3, vec3(15.0f, reindeerPos_y, 0.0f));
	modelReindeer3 = scale(modelReindeer3, vec3(0.6f, 0.6f, 0.6f));
	modelReindeer3 = rotate_y_deg(modelReindeer3, reindeer3Pos_z);
	modelReindeer3 = translate(modelReindeer3, vec3(10.0f, 0.0f, 0.0f));
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

	// reindeer 4
	mat4 modelReindeer4 = identity_mat4();
	modelReindeer4 = translate(modelReindeer4, vec3(15.0f, reindeerPos_y, 0.0f));
	modelReindeer4 = scale(modelReindeer4, vec3(0.6f, 0.6f, 0.6f));
	modelReindeer4 = rotate_y_deg(modelReindeer4, reindeer4Pos_z);
	modelReindeer4 = translate(modelReindeer4, vec3(0.0f, 0.0f, 0.0f));
	//ambient = vec3(0.9f, 0.9f, 0.9f);
	diffuse = vec3(0.4f, 0.4f, 0.4f);
	specular = vec3(0.0f, 0.0f, 0.0f);
	shininess = 120.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelReindeer4.m);
	glEnableVertexAttribArray(loc4[18]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[36]);
	glVertexAttribPointer(loc4[18], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc4[19]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[36]);
	glVertexAttribPointer(loc4[19], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc4[20]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[36]);
	glVertexAttribPointer(loc4[20], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, reindeer_texture);
	glDrawArrays(GL_TRIANGLES, 0, reindeerData.mPointCount);

	// reindeer 5
	mat4 modelReindeer5 = identity_mat4();
	modelReindeer5 = translate(modelReindeer5, vec3(7.0f, reindeerPos_y, 0.0f));
	modelReindeer5 = scale(modelReindeer5, vec3(0.6f, 0.6f, 0.6f));
	modelReindeer5 = rotate_y_deg(modelReindeer5, reindeer5Pos_z);
	modelReindeer5 = translate(modelReindeer5, vec3(0.0f, 0.0f, 0.0f));
	//ambient = vec3(0.9f, 0.9f, 0.9f);
	diffuse = vec3(0.4f, 0.4f, 0.4f);
	specular = vec3(0.0f, 0.0f, 0.0f);
	shininess = 120.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelReindeer5.m);
	glEnableVertexAttribArray(loc4[21]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[37]);
	glVertexAttribPointer(loc4[21], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc4[22]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[37]);
	glVertexAttribPointer(loc4[22], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc4[23]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[37]);
	glVertexAttribPointer(loc4[23], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, reindeer_texture);
	glDrawArrays(GL_TRIANGLES, 0, reindeerData.mPointCount);

	// reindeer 6
	mat4 modelReindeer6 = identity_mat4();
	modelReindeer6 = translate(modelReindeer6, vec3(18.0f, reindeerPos_y, 0.0f));
	modelReindeer6 = scale(modelReindeer6, vec3(0.6f, 0.6f, 0.6f));
	modelReindeer6 = rotate_y_deg(modelReindeer6, reindeer6Pos_z);
	modelReindeer6 = translate(modelReindeer6, vec3(0.0f, 0.0f, 0.0f));
	//ambient = vec3(0.9f, 0.9f, 0.9f);
	diffuse = vec3(0.4f, 0.4f, 0.4f);
	specular = vec3(0.0f, 0.0f, 0.0f);
	shininess = 120.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelReindeer6.m);
	glEnableVertexAttribArray(loc3[18]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[38]);
	glVertexAttribPointer(loc3[18], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc3[19]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[38]);
	glVertexAttribPointer(loc3[19], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[0]);
	glEnableVertexAttribArray(loc3[20]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[38]);
	glVertexAttribPointer(loc3[20], 2, GL_FLOAT, GL_FALSE, 0, NULL);
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
	glUniform1f(shininess_location, shininess);
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

	// leaves model
	mat4 leavesModel = identity_mat4();
	leavesModel = scale(leavesModel, vec3(3.0f, 3.0f, 3.0f));
	leavesModel = translate(leavesModel, vec3(30.0f, 0.0f, 0.0f));
	color = vec3(0.0f, 0.7f, 0.0f);
	glUniform3fv(color_location, 1, color.v);
	//ambient = vec3(1.0f, 1.0f, 1.0f);
	diffuse = vec3(1.0f, 1.0f, 1.0f);
	specular = vec3(0.2f, 0.2f, 0.2f);
	shininess = 50.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, leavesModel.m);
	glEnableVertexAttribArray(loc2[18]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[39]);
	glVertexAttribPointer(loc2[18], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2[19]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[39]);
	glVertexAttribPointer(loc2[19], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[1]);
	glEnableVertexAttribArray(loc2[20]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[39]);
	glVertexAttribPointer(loc2[20], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, leaves_texture);
	glDrawArrays(GL_TRIANGLES, 0, leavesData.mPointCount);

	// tree model
	mat4 treeModel = identity_mat4();
	treeModel = scale(treeModel, vec3(3.0f, 3.0f, 3.0f));
	treeModel = translate(treeModel, vec3(30.0f, 0.0f, 0.0f));
	color = vec3(0.0f, 0.7f, 0.0f);
	glUniform3fv(color_location, 1, color.v);
	//ambient = vec3(1.0f, 1.0f, 1.0f);
	diffuse = vec3(1.0f, 1.0f, 1.0f);
	specular = vec3(0.2f, 0.2f, 0.2f);
	shininess = 50.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, treeModel.m);
	glEnableVertexAttribArray(loc2[21]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[40]);
	glVertexAttribPointer(loc2[21], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2[22]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[40]);
	glVertexAttribPointer(loc2[22], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[1]);
	glEnableVertexAttribArray(loc2[23]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[40]);
	glVertexAttribPointer(loc2[23], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, trunk_texture);
	glDrawArrays(GL_TRIANGLES, 0, trunkData.mPointCount); 

	// leaves model
	mat4 leavesModel2 = identity_mat4();
	leavesModel2 = scale(leavesModel2, vec3(3.0f, 3.0f, 3.0f));
	leavesModel2 = translate(leavesModel2, vec3(26.0f, 0.0f, 2.0f));
	color = vec3(0.0f, 0.7f, 0.0f);
	glUniform3fv(color_location, 1, color.v);
	//ambient = vec3(1.0f, 1.0f, 1.0f);
	diffuse = vec3(1.0f, 1.0f, 1.0f);
	specular = vec3(0.2f, 0.2f, 0.2f);
	shininess = 50.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, leavesModel2.m);
	glEnableVertexAttribArray(loc1[18]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[41]);
	glVertexAttribPointer(loc1[18], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc1[19]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[41]);
	glVertexAttribPointer(loc1[19], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[1]);
	glEnableVertexAttribArray(loc1[20]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[41]);
	glVertexAttribPointer(loc1[20], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, leaves_texture);
	glDrawArrays(GL_TRIANGLES, 0, leavesData.mPointCount);

	// tree model
	mat4 treeModel2 = identity_mat4();
	treeModel2 = scale(treeModel2, vec3(3.0f, 3.0f, 3.0f));
	treeModel2 = translate(treeModel2, vec3(26.0f, 0.0f, 2.0f));
	color = vec3(0.0f, 0.7f, 0.0f);
	glUniform3fv(color_location, 1, color.v);
	//ambient = vec3(1.0f, 1.0f, 1.0f);
	diffuse = vec3(1.0f, 1.0f, 1.0f);
	specular = vec3(0.2f, 0.2f, 0.2f);
	shininess = 50.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, treeModel2.m);
	glEnableVertexAttribArray(loc1[21]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[42]);
	glVertexAttribPointer(loc1[21], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc1[22]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[42]);
	glVertexAttribPointer(loc1[22], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[1]);
	glEnableVertexAttribArray(loc1[23]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[42]);
	glVertexAttribPointer(loc1[23], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, trunk_texture);
	glDrawArrays(GL_TRIANGLES, 0, trunkData.mPointCount);

	// leaves model
	mat4 leavesModel3 = identity_mat4();
	leavesModel3 = scale(leavesModel3, vec3(3.0f, 3.0f, 3.0f));
	leavesModel3 = translate(leavesModel3, vec3(24.0f, 0.0f, -5.0f));
	color = vec3(0.0f, 0.7f, 0.0f);
	glUniform3fv(color_location, 1, color.v);
	//ambient = vec3(1.0f, 1.0f, 1.0f);
	diffuse = vec3(1.0f, 1.0f, 1.0f);
	specular = vec3(0.2f, 0.2f, 0.2f);
	shininess = 50.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, leavesModel3.m);
	glEnableVertexAttribArray(loc5[18]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[43]);
	glVertexAttribPointer(loc5[18], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc5[19]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[43]);
	glVertexAttribPointer(loc5[19], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[1]);
	glEnableVertexAttribArray(loc5[20]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[43]);
	glVertexAttribPointer(loc5[20], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, leaves_texture);
	glDrawArrays(GL_TRIANGLES, 0, leavesData.mPointCount);

	// tree model
	mat4 treeModel3 = identity_mat4();
	treeModel3 = scale(treeModel3, vec3(3.0f, 3.0f, 3.0f));
	treeModel3 = translate(treeModel3, vec3(24.0f, 0.0f, -5.0f));
	color = vec3(0.0f, 0.7f, 0.0f);
	glUniform3fv(color_location, 1, color.v);
	//ambient = vec3(1.0f, 1.0f, 1.0f);
	diffuse = vec3(1.0f, 1.0f, 1.0f);
	specular = vec3(0.2f, 0.2f, 0.2f);
	shininess = 50.0f;
	//glUniform3fv(ambient_location, 1, ambient.v);
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, treeModel3.m);
	glEnableVertexAttribArray(loc5[21]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[44]);
	glVertexAttribPointer(loc5[21], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc5[22]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[44]);
	glVertexAttribPointer(loc5[22], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[1]);
	glEnableVertexAttribArray(loc5[23]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[44]);
	glVertexAttribPointer(loc5[23], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, trunk_texture);
	glDrawArrays(GL_TRIANGLES, 0, trunkData.mPointCount);
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
	glUniform1f(shininess_location, shininess);
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

	// candy cane
	mat4 cane_model2 = identity_mat4();
	cane_model2 = scale(cane_model2, vec3(0.015f, 0.015f, 0.015f));
	cane_model2 = rotate_y_deg(cane_model2, cane_y);
	cane_model2 = translate(cane_model2, canePos2);
	diffuse = vec3(1.0f, 0.8f, 0.8f);
	specular = vec3(0.95f, 0.95f, 0.95f);
	shininess = 40.0f;
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, cane_model2.m);
	glEnableVertexAttribArray(loc2[6]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[20]);
	glVertexAttribPointer(loc2[6], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2[7]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[20]);
	glVertexAttribPointer(loc2[7], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[1]);
	glEnableVertexAttribArray(loc2[8]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[20]);
	glVertexAttribPointer(loc2[8], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, cane_texture);
	glDrawArrays(GL_TRIANGLES, 0, caneData.mPointCount);

	// candy cane
	mat4 cane_model3 = identity_mat4();
	cane_model3 = scale(cane_model3, vec3(0.015f, 0.015f, 0.015f));
	cane_model3 = rotate_y_deg(cane_model3, cane_y);
	cane_model3 = translate(cane_model3, canePos3);
	diffuse = vec3(1.0f, 0.8f, 0.8f);
	specular = vec3(0.95f, 0.95f, 0.95f);
	shininess = 40.0f;
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniform1f(shininess_location, shininess);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, cane_model3.m);
	glEnableVertexAttribArray(loc2[9]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[21]);
	glVertexAttribPointer(loc2[9], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2[10]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[21]);
	glVertexAttribPointer(loc2[10], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[1]);
	glEnableVertexAttribArray(loc2[11]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[21]);
	glVertexAttribPointer(loc2[11], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, cane_texture);
	glDrawArrays(GL_TRIANGLES, 0, caneData.mPointCount);

	stbi_set_flip_vertically_on_load(true);

	// lamp post
	mat4 lampPost_model = identity_mat4();
	lampPost_model = translate(lampPost_model, lampPos);
	diffuse = vec3(1.0f, 1.0f, 1.0f);
	specular = vec3(1.0f, 1.0f, 1.0f);
	shininess = 100.0f;
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, lampPost_model.m);
	glEnableVertexAttribArray(loc2[12]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[22]);
	glVertexAttribPointer(loc2[12], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2[13]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[22]);
	glVertexAttribPointer(loc2[13], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[1]);
	glEnableVertexAttribArray(loc2[14]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[22]);
	glVertexAttribPointer(loc2[14], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, lampMetal_texture);
	glDrawArrays(GL_TRIANGLES, 0, lampData.mPointCount);

	// lamp glass
	mat4 lampGlass_model = identity_mat4();
	lampGlass_model = translate(lampGlass_model, lampPos);
	diffuse = vec3(0.8f, 0.9f, 0.2f);
	specular = vec3(0.5f, 0.5f, 0.5f);
	shininess = 40.0f;
	glUniform3fv(diffuse_location, 1, diffuse.v);
	glUniform3fv(specular_location, 1, specular.v);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, lampGlass_model.m);
	glEnableVertexAttribArray(loc2[15]);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[23]);
	glVertexAttribPointer(loc2[15], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2[16]);
	glBindBuffer(GL_ARRAY_BUFFER, VN_VBO[23]);
	glVertexAttribPointer(loc2[16], 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(VAO[1]);
	glEnableVertexAttribArray(loc2[17]);
	glBindBuffer(GL_ARRAY_BUFFER, VT_VBO[23]);
	glVertexAttribPointer(loc2[17], 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, lampGlass_texture);
	glDrawArrays(GL_TRIANGLES, 0, lampGlassData.mPointCount);


	glutSwapBuffers();
}

// function to change between perspective and orthographic projection
void changeProjection() {
	if (keyStates['p']) {
		persp = true;
	}
	else if (keyStates['o']) {
		persp = false;
	}
}

// function to rotate snowmen models
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
			rotate_y2 += speed * delta;
			rotate_y2 = fmodf(rotate_y2, 360.0f);
			rotate_y3 += speed * delta;
			rotate_y3 = fmodf(rotate_y3, 360.0f);
		}
		if (keyStates['z']) {
			rotate_z += speed * delta;
			rotate_z = fmodf(rotate_z, 360.0f);
		}
	}
}

// function to translate snowmen and change user view
void translateModel(float delta) {
	float speed = 1.0f;
	// reverse direction if '-' pressed
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
			cameraPos -= vec3(0.0f, -0.5f, 0.0f) * 0.2f;
		}
		// move the camera down
		if (keyStates['s']) {
			cameraPos += vec3(0.0f, -0.5f, 0.0f) * 0.2f;
		}
		// move the camera to the left
		if (keyStates['a']) {
			cameraPos -= normalise(cross(cameraFront, cameraUp)) * 0.2f;
		}
		// move the camera to the right
		if (keyStates['d']) {
			cameraPos += normalise(cross(cameraFront, cameraUp)) * 0.2f;
		}
		// move the camera forward
		if (keyStates['i']) {
			cameraPos -= vec3(0.0f, 0.0f, -0.5f) * 0.2f;
		}
		// move the camera back
		if (keyStates['z']) {
			cameraPos -= vec3(0.0f, 0.0f, 0.5f) * 0.2f;
		}
	}
}

// function to scale the size of the snowman model
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

// function to animate the first snowman to be constantly moving back and forward
void snowmanMovements() {
	// animate the crowd to be constantly moving here
	if (snowmanPos_z > -1.0f && snowmanBack == false) {
		snowmanPos_z = snowmanPos_z - 0.08f;
		snowmanPos += vec3(0.0f, 0.0f, -0.08f);

	}
	else {
		snowmanBack = true;
		snowmanPos_z += 0.08f;
		snowmanPos -= vec3(0.0f, 0.0f, -0.08f);
		rotate_y = 0.0f;
		if (snowmanPos_z > 4.0f) {
			rotate_y = 180.0f;
			snowmanBack = false;
		}
	}
}

// function to animate the snowman to constantly be moving back and forward
void snowman2Movements() {
	// animate the crowd to be constantly moving here
	if (snowman2Pos_z > -1.0f && snowman2Back == false) {
		snowman2Pos_z = snowman2Pos_z - 0.1f;
		snowman2Pos += vec3(0.0f, 0.0f, -0.1f);
		if (snowman2Pos_z > 1.5f && snowman2Pos_z < 2.5f) {
			snowman2Pos -= vec3(0.0f, -0.1f, 0.0f);
		}
		if (snowman2Pos_z > 0.5f && snowman2Pos_z < 1.5f) {
			snowman2Pos -= vec3(0.0f, 0.1f, 0.0f);
		}
	}
	else {
		snowman2Back = true;
		snowman2Pos_z += 0.1f;
		snowman2Pos -= vec3(0.0f, 0.0f, -0.1f);
		rotate_y2 = 0.0f;
		if (snowman2Pos_z > 4.0f) {
			rotate_y2 = 180.0f;
			snowman2Back = false;
		}
		if (snowman2Pos_z > 1.5f && snowman2Pos_z < 2.5f) {
			snowman2Pos -= vec3(0.0f, 0.1f, 0.0f);
		}
		if (snowman2Pos_z > 0.5f && snowman2Pos_z < 1.5f) {
			snowman2Pos -= vec3(0.0f, -0.1f, 0.0f);
		}
	}
}

// function to animate the third snowman to constantly be moving back and forward
void snowman3Movements() {
	// animate the crowd to be constantly moving here
	if (snowman3Pos_z > -1.0f && snowman3Back == false) {
		snowman3Pos_z = snowman3Pos_z - 0.05f;
		snowman3Pos += vec3(0.0f, 0.0f, -0.05f);
		if (snowman3Pos_z > 3.0f) {
			snowman3Pos -= vec3(0.0f, 0.05f, 0.0f);
		}
	}
	else {
		snowman3Back = true;
		snowman3Pos_z += 0.05f;
		snowman3Pos -= vec3(0.0f, 0.0f, -0.05f);
		rotate_y3 = 0.0f;
		if (snowman3Pos_z > 4.0f) {
			rotate_y3 = 180.0f;
			snowman3Back = false;
		}
		if (snowman3Pos_z > 3.0f) {
			snowman3Pos -= vec3(0.0f, -0.05f, 0.0f);
		}

	}
}

// function to animate the first snowman to be constantly moving back and forward
void snowman4Movements() {
	// animate the crowd to be constantly moving here
	if (snowman4Pos_z > -1.0f && snowman4Back == false) {
		snowman4Pos_z = snowman4Pos_z - 0.08f;
		snowman4Pos += vec3(0.0f, 0.0f, -0.08f);

	}
	else {
		snowman4Back = true;
		snowman4Pos_z += 0.08f;
		snowman4Pos -= vec3(0.0f, 0.0f, -0.08f);
		rotate_y4 = 0.0f;
		if (snowman4Pos_z > 4.0f) {
			rotate_y4 = 180.0f;
			snowman4Back = false;
		}
	}
}

// function to animate the first snowman to be constantly moving back and forward
void snowman5Movements() {
	// animate the crowd to be constantly moving here
	if (snowman5Pos_z > -1.0f && snowman5Back == false) {
		snowman5Pos_z = snowman5Pos_z - 0.08f;
		snowman5Pos += vec3(0.0f, 0.0f, -0.08f);

	}
	else {
		snowman5Back = true;
		snowman5Pos_z += 0.08f;
		snowman5Pos -= vec3(0.0f, 0.0f, -0.08f);
		rotate_y5 = 0.0f;
		if (snowman5Pos_z > 4.0f) {
			rotate_y5 = 180.0f;
			snowman5Back = false;
		}
	}
}
// function to allow arm waving and hat spinning for hierarchical animation
void animations(float delta) {
	// snowman 1 arms
	if (keyStates['m']) {
		if (keyStates['l']) {
			if (armjoint1_z < 1.0f && change == false) {
				//printf("%f", armjoint1_z);
				armjoint1_z += 10.0f * delta;
			}
			else {
				//printf("%f", armjoint1_z);
				armjoint1_z -= 10.0f * delta;
				change = true;
				if (armjoint1_z < -3.0f) {
					change = false;
				}
			}
		}
		if (keyStates[';']) {
			if (armjoint2_z < 4.0f && change == false) {
				//printf("%f", armjoint1_z);
				armjoint2_z += 10.0f * delta;
			}
			else {
				//printf("%f", armjoint1_z);
				armjoint2_z -= 10.0f * delta;
				change = true;
				if (armjoint2_z < 1.0f) {
					change = false;
				}
			}
		}
	}
	// snowman 2 arms
	if (keyStates['n']) {
		if (keyStates['l']) {
			if (armjoint1_z2 < 1.0f && change2 == false) {
				//printf("%f", armjoint1_z);
				armjoint1_z2 += 10.0f * delta;
			}
			else {
				//printf("%f", armjoint1_z);
				armjoint1_z2 -= 10.0f * delta;
				change2 = true;
				if (armjoint1_z2 < -3.0f) {
					change2 = false;
				}
			}
		}
		if (keyStates[';']) {
			if (armjoint2_z2 < 4.0f && change2 == false) {
				//printf("%f", armjoint1_z);
				armjoint2_z2 += 10.0f * delta;
			}
			else {
				//printf("%f", armjoint1_z);
				armjoint2_z2 -= 10.0f * delta;
				change2 = true;
				if (armjoint2_z2 < 1.0f) {
					change2 = false;
				}
			}
		}
	}
	// snowman 3 arms
	if (keyStates['q']) {
		if (keyStates['l']) {
			if (armjoint1_z3 < 1.0f && change3 == false) {
				//printf("%f", armjoint1_z);
				armjoint1_z3 += 10.0f * delta;
			}
			else {
				//printf("%f", armjoint1_z);
				armjoint1_z3 -= 10.0f * delta;
				change3 = true;
				if (armjoint1_z3 < -3.0f) {
					change3 = false;
				}
			}
		}
		if (keyStates[';']) {
			if (armjoint2_z3 < 4.0f && change3 == false) {
				//printf("%f", armjoint1_z);
				armjoint2_z3 += 10.0f * delta;
			}
			else {
				//printf("%f", armjoint1_z);
				armjoint2_z3 -= 10.0f * delta;
				change3 = true;
				if (armjoint2_z3 < 1.0f) {
					change3 = false;
				}
			}
		}
	}
	// snowman 4 arms
	if (keyStates['#']) {
		if (keyStates['l']) {
			if (armjoint1_z4 < 1.0f && change4 == false) {
				//printf("%f", armjoint1_z);
				armjoint1_z4 += 10.0f * delta;
			}
			else {
				//printf("%f", armjoint1_z);
				armjoint1_z4 -= 10.0f * delta;
				change4 = true;
				if (armjoint1_z4 < -3.0f) {
					change4 = false;
				}
			}
		}
		if (keyStates[';']) {
			if (armjoint2_z4 < 4.0f && change4 == false) {
				//printf("%f", armjoint1_z);
				armjoint2_z4 += 10.0f * delta;
			}
			else {
				//printf("%f", armjoint1_z);
				armjoint2_z4 -= 10.0f * delta;
				change4 = true;
				if (armjoint2_z4 < 1.0f) {
					change4 = false;
				}
			}
		}
	}
	// snowman 5 arms
	if (keyStates['=']) {
		if (keyStates['l']) {
			if (armjoint1_z5 < 1.0f && change5 == false) {
				//printf("%f", armjoint1_z);
				armjoint1_z5 += 10.0f * delta;
			}
			else {
				//printf("%f", armjoint1_z);
				armjoint1_z5 -= 10.0f * delta;
				change5 = true;
				if (armjoint1_z5 < -3.0f) {
					change5 = false;
				}
			}
		}
		if (keyStates[';']) {
			if (armjoint2_z5 < 4.0f && change5 == false) {
				//printf("%f", armjoint1_z);
				armjoint2_z5 += 10.0f * delta;
			}
			else {
				//printf("%f", armjoint1_z);
				armjoint2_z5 -= 10.0f * delta;
				change5 = true;
				if (armjoint2_z5 < 1.0f) {
					change5 = false;
				}
			}
		}
	}
	// move arms out/in from body
	if (keyStates['v']) {
		arm1Pos += vec3(0.01f, 0.0000f, 0.0f);
		arm2Pos += vec3(-0.01f, 0.0000f, 0.0f);
	}
	if (keyStates['b']) {
		arm1Pos += vec3(-0.01f, 0.0000f, 0.0f);
		arm2Pos += vec3(0.01f, 0.0000f, 0.0f);
	}
	// spin hats!
	if (keyStates['h']) {
		hat_y += 45.0f * delta;
		hat_y = fmodf(hat_y, 360.0f);
	}
}

// continuously call these functions
void updateScene() {

	static DWORD last_time = 0;
	DWORD curr_time = timeGetTime();
	if (last_time == 0)
		last_time = curr_time;
	float delta = (curr_time - last_time) * 0.001f;
	last_time = curr_time;

	// rotate the candy canes and have the reindeer rotate around fixed points and change altitude
	cane_y = cane_y + 5.0f;
	reindeerPos_z += 2.0f;
	reindeer2Pos_z += 2.0f;
	reindeer3Pos_z += 2.0f;
	reindeer4Pos_z += 2.0f;
	reindeer5Pos_z += 2.0f;
	reindeer6Pos_z += 2.0f;
	reindeerPos_y = 13 + 2*sin(reindeerPos_z/90);

	// calculate distances from snowmen to camera for collision detection
	vec3 distance = (snowmanPos - cameraPos);
	vec3 distance2 = (snowman2Pos - cameraPos);
	vec3 distance3 = (snowman3Pos - cameraPos);
	vec3 distance4 = (snowman4Pos - cameraPos);
	vec3 distance5 = (snowman5Pos - cameraPos);
	float distanceX = abs(distance.v[0]);
	float distanceY = abs(distance.v[1]);
	float distanceZ = abs(distance.v[2]);
	float distanceX2 = abs(distance2.v[0]);
	float distanceY2 = abs(distance2.v[1]);
	float distanceZ2 = abs(distance2.v[2]);
	float distanceX3 = abs(distance3.v[0]);
	float distanceY3 = abs(distance3.v[1]);
	float distanceZ3 = abs(distance3.v[2]);
	float distanceX4 = abs(distance4.v[0]);
	float distanceY4 = abs(distance4.v[1]);
	float distanceZ4 = abs(distance4.v[2]);
	float distanceX5 = abs(distance5.v[0]);
	float distanceY5 = abs(distance5.v[1]);
	float distanceZ5 = abs(distance5.v[2]);
	// if the x and z values total to less than 4 stop moving the snowman 
	if (distanceX > 2.0f || distanceZ > 4.0f) {
		snowmanMovements();
	}
	if (distanceX2 > 2.0f || distanceZ2 > 4.0f) {
		snowman2Movements();
	}
	if (distanceX3 > 2.0f || distanceZ3 > 4.0f) {
		snowman3Movements();
	}
	if (distanceX4 > 2.0f || distanceZ4 > 4.0f) {
		snowman4Movements();
	}
	if (distanceX5 > 2.0f || distanceZ5 > 4.0f) {
		snowman5Movements();
	}
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

// initialisation function - compile shader and generate buffers
void init()
{
	// Set up the shaders
	GLuint shaderProgramID = CompileShaders();
	// load mesh into a vertex buffer array
	generateObjectBufferMesh();

}

// allowing mouse movements
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

// main function
int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Winter Wonderland");

	// Tell glut where the display function is and call keyboard functions
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
