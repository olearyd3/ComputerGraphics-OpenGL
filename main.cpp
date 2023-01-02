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
//#include "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/obj_parser.h"
//#include "camera.h"
//#include "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/Mesh.h"
#include "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/Shader.h"
#define STB_IMAGE_IMPLEMENTATION
//#include "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/Model.h"
#include "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/Skybox.h"
#include "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/stb_image.h"
//#include "camera.h"

//global var to start/stop rotation here
bool start_y = false;
bool start_z = false;
bool start_x = false;
bool ortho = false;
bool change = false;
bool reindeerBack = false;
bool snowmanBack = false;
bool snowman2Back = false;
bool snowman3Back = false;
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
#define LAMPPOST "C:/Users/markj/Downloads/lampNoGlass.obj"
#define LAMPLIGHT "C:/Users/markj/Downloads/lampGlass.obj" 
// textures
#define TERRAIN "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/textures/terrain.jpg"
#define SNOW_TEX "C:/Users/markj/Downloads/snowTex.jpg"
#define LEATHER "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/textures/leatherdark.jpg"
#define BARK "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/textures/tree-bark.jpeg"
#define CARROT "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/textures/carrot.jpg"
#define LEAVES_TEX "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/textures/firtree.jpg"
#define REINDEER_TEX "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/textures/reindeer.jpg"
#define BELL_TEX "C:/Users/markj/source/repos/compgraphics-lab3/compgraphics-lab3/textures/reindeerbell.jpg"
#define CANDY_CANE_TEX "C:/Users/markj/Downloads/stripes.jpg"
#define LAMP_METAL "C:/Users/markj/Downloads/blackmetal.jpg"
#define LAMP_GLASS "C:/Users/markj/Downloads/yellow.jpg"
//#define WOOD "C:/Users/markj/Downloads/treeTrunk2.jpg"

#define MAX_PARTICLES 1000
#define WCX		640
#define WCY		480
#define RAIN	0
#define SNOW	1
#define	HAIL	2


float slowdown = -0.1;
float velocity = 0.0;
float zoom = -40.0;
float pan = 0.0;
float tilt = 0.0;
float hailsize = 0.1;

int loop;
int fall;

//floor colors
float r = 0.0;
float g = 1.0;
float b = 0.0;
float ground_points[100][100][3];
float ground_colors[100][100][4];
float accum = 10.0;

typedef struct {
	// Life
	bool alive;	// is the particle alive?
	float life;	// particle lifespan
	float fade; // decay
	// color
	float red;
	float green;
	float blue;
	// Position/direction
	float xpos;
	float ypos;
	float zpos;
	// Velocity/Direction, only goes down in y dir
	float vel;
	// Gravity
	float gravity;
}particles;

// Paticle System
particles par_sys[MAX_PARTICLES];

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

//Camera camera(vec3(0.0f, 0.0f, 3.0f));

//Shaders
Shader skyboxShader;
Shader objectShader;
GLuint diffuseShaderProgramID, specularShaderProgramID, textureShaderProgramID;

// skybox
Skybox sky;
unsigned int skyBoxTextureMap;

// model transform variables
GLuint loc1[8 * 3];
GLuint loc2[8 * 3];
GLuint loc3[8 * 3];
GLuint loc4[8 * 3];
GLuint loc5[8 * 3]; // SNOWMAN 2
GLuint loc6[8 * 3]; // SNOWMAN 3
GLfloat rotate_y = 0.0f;
GLfloat rotate_y2 = 0.0f;
GLfloat rotate_y3 = 0.0f;
GLfloat rotate_x = 0.0f;
GLfloat rotate_z = 0.0f;
GLfloat translate_x = 0.0f;
GLfloat translate_y = 0.0f;
GLfloat translate_z = 0.0f;
vec3 translate_base = vec3(0.0f, 0.0f, 0.0f);
vec3 cameraPos = vec3(5.0f, 5.5f, 16.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
vec3 scaling = vec3(1.0f, 1.0f, 1.0f);

float snowmanPos_z = 3.0f;
vec3 snowmanPos = vec3(7.0f, 0.0f, snowmanPos_z);
float snowman2Pos_z = -5.0f;
vec3 snowman2Pos = vec3(12.0f, 0.0f, snowman2Pos_z);
float snowman3Pos_z = 2.0f;
vec3 snowman3Pos = vec3(17.0f, 0.0f, snowman3Pos_z);

GLfloat rotate_reindeer_y = 0.0f;

vec3 groundPos = vec3(0.0f, -3.9f, 0.0f);
vec3 treePos = vec3(5.0f, 0.2f, 2.0f);
vec3 arm1Pos = vec3(0.0f, 0.0f, 0.0f);
vec3 arm2Pos = vec3(0.0f, 0.0f, 0.0f);
vec3 armJoint1Pos = vec3(0.0f, 0.0f, 0.0f);
vec3 armJoint2Pos = vec3(0.0f, 0.0f, 0.0f);
float reindeerPos_z = 0.0f;
float reindeer2Pos_z = 90.0f;
float reindeer3Pos_z = 220.0f;
vec3 reindeerPos = vec3(8.0f, 10.0f, 0.0f);
vec3 canePos = vec3(7.0f, -0.1f, -10.0f);
vec3 canePos2 = vec3(0.0f, -0.1f, -10.0f);
vec3 canePos3 = vec3(-7.0f, -0.1f, -10.0f);
vec3 lampPos = vec3(8.0f, 0.5f, -5.0f);
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
unsigned int lampMetal_texture;
unsigned int lampGlass_texture;

// VBOs and VAO
unsigned int VP_VBO[40]; // vertex positions
unsigned int VN_VBO[40]; // vertex normals
unsigned int VT_VBO[40]; // vertex textures
unsigned int VAO[3];
unsigned int terrainVAO, terrainVBO, terrainIBO;

int w, h, nrChannels;
int numStrips;
int numTrisPerStrip;

unsigned int skyboxVAO, skyboxVBO;
unsigned int cubemapTexture;

//unsigned int loadCubemap(vector<std::string> faces);

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
	lampData = load_mesh(LAMPPOST);
	lampGlassData = load_mesh(LAMPLIGHT);
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
	lampMetal_texture = load_tex(LAMP_METAL);
	lampGlass_texture = load_tex(LAMP_GLASS);

	//height map
	/*unsigned char* data = stbi_load("C:/Users/markj/Downloads/iceland_heightmap.png", &w, &h, &nrChannels, 0);
	if (data)
	{
		std::cout << "Loaded heightmap of size " << h << " x " << w << std::endl;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	std::vector<float> vertices;
	float yScale = 64.0f / 256.0f, yShift = 16.0f;
	int rez = 1;
	unsigned bytePerPixel = nrChannels;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			unsigned char* pixelOffset = data + (j + w * i) * bytePerPixel;
			unsigned char y = pixelOffset[0];

			// vertex
			vertices.push_back(-h / 2.0f + h * i / (float)h);   // vx
			vertices.push_back((int)y * yScale - yShift);   // vy
			vertices.push_back(-w / 2.0f + w * j / (float)w);   // vz
		}
	}
	std::cout << "Loaded " << vertices.size() / 3 << " vertices" << std::endl;
	stbi_image_free(data);

	std::vector<unsigned> indices;
	for (unsigned i = 0; i < h - 1; i += rez)
	{
		for (unsigned j = 0; j < w; j += rez)
		{
			for (unsigned k = 0; k < 2; k++)
			{
				indices.push_back(j + w * (i + k * rez));
			}
		}
	}
	std::cout << "Loaded " << indices.size() << " indices" << std::endl;

	numStrips = (h - 1) / rez;
	numTrisPerStrip = (w / rez) * 2 - 2;
	std::cout << "Created lattice of " << numStrips << " strips with " << numTrisPerStrip << " triangles each" << std::endl;
	std::cout << "Created " << numStrips * numTrisPerStrip << " triangles total" << std::endl;*/

	// first, configure the cube's VAO (and terrainVBO + terrainIBO)
	/*glGenVertexArrays(1, &terrainVAO);
	glBindVertexArray(terrainVAO);

	glGenBuffers(1, &terrainVBO);
	glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &terrainIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);*/

	// generate the buffers
	glGenBuffers(40, VP_VBO);
	glGenBuffers(40, VN_VBO);
	glGenBuffers(40, VT_VBO);

	// SNOWMAN 1!!!!!!!!!
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
	// main root object - snowman
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

	/*glGenVertexArrays(1, &terrainVAO);
	glBindVertexArray(VAO[2]);

	glGenBuffers(1, &terrainVBO);
	glBindBuffer(GL_ARRAY_BUFFER, VP_VBO[24]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &terrainIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VN_VBO[24]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);*/

}
#pragma endregion VBO_FUNCTIONS


unsigned int loadSkyTexture()
{
	std::string faces[] =
	{
		"C:/Users/markj/Downloads/skybox/skybox/right.jpg",
		"C:/Users/markj/Downloads/skybox/skybox/left.jpg",
		"C:/Users/markj/Downloads/skybox/skybox/top.jpg",
		"C:/Users/markj/Downloads/skybox/skybox/bottom.jpg",
		"C:/Users/markj/Downloads/skybox/skybox/front.jpg",
		"C:/Users/markj/Downloads/skybox/skybox/back.jpg"
	};

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < 6; i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;

}

void setUpSkyBox() {
	stbi_set_flip_vertically_on_load(false);
	//set up skybox 
	sky = Skybox();
	sky.genSkybox();
	skyBoxTextureMap = loadSkyTexture();
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
	objectShader.use();
	stbi_set_flip_vertically_on_load(true);
}
void setupShaders() {
	objectShader = Shader("Shaders/OriginalVertexShader.txt", "Shaders/OriginalFragmentShader.txt");
	skyboxShader = Shader("Shaders/skyBox.vs", "Shaders/skyBox.fs");
}

// load textures
   // -------------
	/*vector<std::string> faces
	{
		"C:/Users/markj/Downloads/skybox/skybox/right.jpg",
		"C:/Users/markj/Downloads/skybox/skybox/left.jpg",
		"C:/Users/markj/Downloads/skybox/skybox/top.jpg",
		"C:/Users/markj/Downloads/skybox/skybox/bottom.jpg",
		"C:/Users/markj/Downloads/skybox/skybox/front.jpg",
		"C:/Users/markj/Downloads/skybox/skybox/back.jpg"
	};*/
	//cubemapTexture = loadCubemap(faces);

void initParticles(int i) {
	par_sys[i].alive = true;
	par_sys[i].life = 5.0;
	par_sys[i].fade = 0.1;

	par_sys[i].xpos = (rand() % 2);
	par_sys[i].ypos = 30.0;
	par_sys[i].zpos = (float)(rand() % 2);

	par_sys[i].red = 1.0;
	par_sys[i].green = 1.0;
	par_sys[i].blue = 1.0;

	par_sys[i].vel = velocity;
	par_sys[i].gravity = -0.1;

}

void drawSnow() {
	float x, y, z;
	for (int i = 0; i < 100; i++) {
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(10.0f, 10.0f, 10.0f);
		glutSolidSphere(2.0, 16, 16);
		glPopMatrix();
	}
	/*for (loop = 0; loop < MAX_PARTICLES; loop = loop + 2) {
		if (par_sys[loop].alive == true) {
			x = par_sys[loop].xpos;
			y = par_sys[loop].ypos;
			z = par_sys[loop].zpos + zoom;

			// Draw particles
			glPushMatrix();
			glColor3f(1.0, 1.0, 1.0);
			glTranslatef(x, y, z);
			glutSolidSphere(2.0, 16, 16);
			glPopMatrix();

			glPushMatrix();
			glColor3f(1.0, 1.0, 1.0);
			glTranslatef(x, y+10, z);
			glutSolidSphere(2.0, 16, 16);
			glPopMatrix();

			if (par_sys[loop].ypos > -4.0f && snowBack == false) {
				printf("LOOP 1: %f\n", par_sys[loop].ypos);
				par_sys[loop].ypos = par_sys[loop].ypos - 0.15f;
			}
			else {
				snowBack = true;
				par_sys[loop].ypos += 0.15f;
				printf("LOOP 2: %f\n", par_sys[loop].ypos);
				if (par_sys[loop].ypos > 8.0f) {
					printf("LOOP 3: %f\n", par_sys[loop].ypos);
					snowBack = false;
				}
			}
			// Update values
			//Move
			/*par_sys[loop].ypos -= par_sys[loop].vel / slowdown;
			printf("VELOCITY: %f\n", par_sys[loop].ypos);
			par_sys[loop].vel += par_sys[loop].gravity;
			// Decay
			par_sys[loop].life -= par_sys[loop].fade;*/

			/*if (par_sys[loop].ypos <= -10) {
				int zi = z - zoom + 10;
				int xi = x + 10;
				ground_colors[zi][xi][0] = 1.0;
				ground_colors[zi][xi][2] = 1.0;
				ground_colors[zi][xi][3] += 1.0;
				if (ground_colors[zi][xi][3] > 1.0) {
					ground_points[xi][zi][1] += 0.1;
				}
				par_sys[loop].life = -1.0;
			}

			//Revive
			if (par_sys[loop].life < 0.0) {
				initParticles(loop);
			}
		}
	}*/
}

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
	vec3 light1Position = vec3(-10.0f, 4.5f, -2.0f);
	glUniform3fv(light1_location, 1, light1Position.v);
	glUniform3fv(light1_ambient, 1, lightDiffuse.v);
	glUniform3fv(light1_diffuse, 1, lightSpecular.v);
	glUniform3fv(light1_specular, 1, lightSpecular.v);
	glUniform1f(light1_constant, lightConstant);
	glUniform1f(light1_linear, lightLinear);
	glUniform1f(light1_quadratic, lightQuadratic);

	GLuint light2_location = glGetUniformLocation(shaderProgramID, "pointLights[2].position");
	GLuint light2_ambient = glGetUniformLocation(shaderProgramID, "pointLights[2].ambient");
	GLuint light2_diffuse = glGetUniformLocation(shaderProgramID, "pointLights[2].diffuse");
	GLuint light2_specular = glGetUniformLocation(shaderProgramID, "pointLights[2].specular");
	GLuint light2_constant = glGetUniformLocation(shaderProgramID, "Light[2].constant");
	GLuint light2_linear = glGetUniformLocation(shaderProgramID, "Light[2].linear");
	GLuint light2_quadratic = glGetUniformLocation(shaderProgramID, "Light[2].quadratic");
	vec3 light2Position = vec3(5.0f, 8.0f, 0.0f);
	glUniform3fv(light2_location, 2, light2Position.v);
	glUniform3fv(light2_ambient, 2, lightDiffuse.v);
	glUniform3fv(light2_diffuse, 2, lightSpecular.v);
	glUniform3fv(light2_specular, 2, lightSpecular.v);
	glUniform1f(light2_constant, lightConstant);
	glUniform1f(light2_linear, lightLinear);
	glUniform1f(light2_quadratic, lightQuadratic);
	//GLuint light1_location = glGetUniformLocation(shaderProgramID, "Light[1].position");

	// Root of the Hierarchy
	mat4 projection = perspective(fov, (float)width / (float)height, 0.1f, 1000.0f);
	// Switch between Ortho and Perspective views based on whether persp is true or false
	
	// set up view
	mat4 view = identity_mat4();
	view = look_at(cameraPos, cameraPos + cameraFront, cameraUp);

	// update uniforms & draw
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, projection.m);
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
	modelChild2Arm1 = rotate_z_deg(modelChild2Arm1, armjoint1_z);
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
	modelChild2Arm2 = rotate_z_deg(modelChild2Arm2, armjoint2_z);
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
	modelChild3Arm1 = rotate_z_deg(modelChild3Arm1, armjoint1_z);
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
	modelChild3Arm2 = rotate_z_deg(modelChild3Arm2, armjoint2_z);
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

	// reindeer
	mat4 modelReindeer = identity_mat4();
	modelReindeer = translate(modelReindeer, vec3(10.0f, 15.0f, 0.0f));
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
	modelReindeer2 = translate(modelReindeer2, vec3(5.0f, 15.0f, 0.0f));
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
	modelReindeer3 = translate(modelReindeer3, vec3(15.0f, 15.0f, 0.0f));
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

	/*mat4 terrainModel = identity_mat4();
	terrainModel = translate(terrainModel, vec3(0.0f, 0.0f, 0.0f));

	// render the cube
	glBindVertexArray(VAO[2]);
	glBindTexture(GL_TEXTURE_2D, snow_texture);
	//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (unsigned strip = 0; strip < numStrips; strip++) {
		glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
			numTrisPerStrip + 2,   // number of indices to render
			GL_UNSIGNED_INT,     // index data type
			(void*)(sizeof(unsigned) * (numTrisPerStrip + 2) * strip)); // offset to starting index
	}*/
	//draw skybox 
	//skyboxShader.use();
	//glDepthFunc(GL_LEQUAL);
	//view = identity_mat4();
	//skyboxShader.setMat4("view", view);
	//skyboxShader.setMat4("projection", projection);
	//sky.drawSkyBox(skyboxShader, skyBoxTextureMap);

	// procedural terrain
	/*int i, j;
	float x, y, z;
	glColor3f(r, g, b);
	glBegin(GL_QUADS);
	// along z - y const
	for (i = -10; i + 1 < 90; i++) {
		// along x - y const
		for (j = -10; j + 1 < 90; j++) {
			glColor3fv(ground_colors[i + 10][j + 10]);
			glVertex3f(ground_points[j + 10][i + 10][0],
				ground_points[j + 10][i + 10][1]-10,
				ground_points[j + 10][i + 10][2]);
			glColor3fv(ground_colors[i + 10][j + 1 + 10]);
			glVertex3f(ground_points[j + 1 + 10][i + 10][0],
				ground_points[j + 1 + 10][i + 10][1]-10,
				ground_points[j + 1 + 10][i + 10][2]);
			glColor3fv(ground_colors[i + 1 + 10][j + 1 + 10]);
			glVertex3f(ground_points[j + 1 + 10][i + 1 + 10][0],
				ground_points[j + 1 + 10][i + 1 + 10][1]-10,
				ground_points[j + 1 + 10][i + 1 + 10][2]);
			glColor3fv(ground_colors[i + 1 + 10][j + 10]);
			glVertex3f(ground_points[j + 10][i + 1 + 10][0],
				ground_points[j + 10][i + 1 + 10][1]-10,
				ground_points[j + 10][i + 1 + 10][2]);
		}

	}
	glEnd();
	if (fall == SNOW) {
		drawSnow();
	}*/

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

void translateModel(float delta) {
	float speed = 1.0f;
	// reverse direction if '-' pressed
	if (keyStates['-']) {
		speed = -speed;
	}
	/*if (keyStates['#']) {
		printf("Hashtag pressed!");
		fall = SNOW;
		glutPostRedisplay();
	}*/
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

void reindeerMovements(float delta) {
	// animate the crowd to be constantly moving here
	/*if (reindeerPos_z > -4.0f && reindeerBack == false) {
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
	}*/

}

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

void animations(float delta) {
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
	if (keyStates['v']) {
		arm1Pos += vec3(0.01f, 0.0000f, 0.0f);
		arm2Pos += vec3(-0.01f, 0.0000f, 0.0f);
	}
	if (keyStates['b']) {
		arm1Pos += vec3(-0.01f, 0.0000f, 0.0f);
		arm2Pos += vec3(0.01f, 0.0000f, 0.0f);
	}
	if (keyStates['h']) {
		hat_y += 45.0f * delta;
		hat_y = fmodf(hat_y, 360.0f);
	}
}

/*void InitializeTerrain()
{
	// loop through all of the heightfield points, calculating
	// the coordinates for each point
	for (int z = 0; z < MAP_Z; z++)
	{
		for (int x = 0; x < MAP_X; x++)
		{
			terrain[x][z][0] = float(x) * MAP_SCALE;
			terrain[x][z][1] = (float)imageData[(z * MAP_Z + x) * 3];
			terrain[x][z][2] = -float(z) * MAP_SCALE;
		}
	}
}*/

void updateScene() {

	static DWORD last_time = 0;
	DWORD curr_time = timeGetTime();
	if (last_time == 0)
		last_time = curr_time;
	float delta = (curr_time - last_time) * 0.001f;
	last_time = curr_time;

	cane_y = cane_y + 5.0f;
	reindeerPos_z += 2.0f;
	reindeer2Pos_z += 2.0f;
	reindeer3Pos_z += 2.0f;
	// reindeer movements as crowd
	reindeerMovements(delta);
	snowmanMovements();
	snowman2Movements();
	snowman3Movements();
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

	// procedural terrain
	/*int x, z;

	// Ground Verticies
	  // Ground Colors
	for (z = 0; z < 100; z++) {
		for (x = 0; x < 100; x++) {
			ground_points[x][z][0] = x;
			ground_points[x][z][1] = accum + (rand() % 2);
			ground_points[x][z][2] = z;

			ground_colors[z][x][0] = r; // red value
			ground_colors[z][x][1] = g; // green value
			ground_colors[z][x][2] = b; // blue value
			ground_colors[z][x][3] = 0.0; // acummulation factor
		}
	}

	// Initialize particles
	for (loop = 0; loop < MAX_PARTICLES; loop++) {
		initParticles(loop);
	}*/
	// skybox
	//setupShaders();
	//setUpSkyBox();
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

/*unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	int width, height, nrComponents;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}*/

int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Winter Wonderland");

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
