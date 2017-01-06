#include "Mesh.h"
#include "glm\gtc\matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Mesh::Mesh(void) {
	glewInit();

	for( int i = 0; i < MAX_BUFFER ; ++i) {
		bufferObject[i] = 0;
	}
	glGenVertexArrays(1, &arrayObject);

	numVertices		= NULL;
	texture			= NULL;
	numIndices		= NULL;
	indices			= NULL;
	textureCoords	= NULL;
	vertices		= NULL;
	colours			= NULL;
	normals			= NULL;
	type			= NULL;

	m_xSize = 0;
	m_ySize = 0;
	m_zSize = 0;
}

Mesh::~Mesh(void) {
	glDeleteVertexArrays(1, &arrayObject);
	glDeleteBuffers(MAX_BUFFER ,bufferObject);
	glDeleteTextures(1 ,& texture);
	delete[]vertices;
	delete[]textureCoords;
	delete[]colours;
	delete[]normals;
	delete[]indices;
}

void Mesh::draw() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D , texture);

	glBindVertexArray(arrayObject);

	if(bufferObject[INDEX_BUFFER]) {
		glDrawElements (type ,numIndices ,GL_UNSIGNED_INT ,0);
	}
	else {
		glDrawArrays(type , 0, numVertices);
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D , 0);
}

/// <summary>
/// Source modified from:
/// http://en.wikibooks.org/wiki/OpenGL_Programming/Bounding_box
/// </summary>
/// <param name="mesh">The mesh.</param>
/// <param name="modelMat">The model mat.</param>
void Mesh::drawBoundingBox(Mesh* mesh, glm::mat4 modelMat) {
	if (mesh->numVertices == 0) {
		return;
	}

	// Cube 1x1x1, centered on origin
	GLfloat vertices[] = {
		-0.5, -0.5, -0.5, 1.0,
			0.5, -0.5, -0.5, 1.0,
			0.5,  0.5, -0.5, 1.0,
		-0.5,  0.5, -0.5, 1.0,
		-0.5, -0.5,  0.5, 1.0,
			0.5, -0.5,  0.5, 1.0,
			0.5,  0.5,  0.5, 1.0,
		-0.5,  0.5,  0.5, 1.0,
	};
	GLuint vbo_vertices;
	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
 
	GLushort elements[] = {
		0, 1, 2, 3,
		4, 5, 6, 7,
		0, 4, 1, 5, 2, 6, 3, 7
	};
	GLuint ibo_elements;
	glGenBuffers(1, &ibo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
 
	GLfloat
		min_x, max_x,
		min_y, max_y,
		min_z, max_z;
		min_x = max_x = mesh->vertices[0].x;
		min_y = max_y = mesh->vertices[0].y;
		min_z = max_z = mesh->vertices[0].z;
	for (int i = 0; i < mesh->numVertices; i++) {
		if (mesh->vertices[i].x < min_x) min_x = mesh->vertices[i].x;
		if (mesh->vertices[i].x > max_x) max_x = mesh->vertices[i].x;
		if (mesh->vertices[i].y < min_y) min_y = mesh->vertices[i].y;
		if (mesh->vertices[i].y > max_y) max_y = mesh->vertices[i].y;
		if (mesh->vertices[i].z < min_z) min_z = mesh->vertices[i].z;
		if (mesh->vertices[i].z > max_z) max_z = mesh->vertices[i].z;
	}
	glm::vec3 size = glm::vec3(max_x-min_x, max_y-min_y, max_z-min_z);
	glm::vec3 center = glm::vec3((min_x+max_x)/2, (min_y+max_y)/2, (min_z+max_z)/2);
	glm::mat4 transform =  glm::translate(glm::mat4(1), center)* glm::scale(glm::mat4(1), size);
	
	/* Apply object's transformation matrix */
	glm::mat4 m = modelMat * transform;
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(m));
 
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
	0,					// attribute
	4,                  // number of elements per vertex, here (x,y,z,w)
	GL_FLOAT,           // the type of each element
	GL_FALSE,           // take our values as-is
	0,                  // no extra data between each position
	0                   // offset of first element
	);
 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4*sizeof(GLushort)));
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8*sizeof(GLushort)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
 
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
 
	glDeleteBuffers(1, &vbo_vertices);
	glDeleteBuffers(1, &ibo_elements);
}

Mesh* Mesh::importModel(const string &filename) {
	Mesh*m = new Mesh();
	m->type = GL_TRIANGLES;
	
	// Create an instance of the Importer class
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll 
	// propably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(
		filename, 
		aiProcess_Triangulate            |
		aiProcessPreset_TargetRealtime_Quality
	);

	// If the import failed, report it
	if(scene) {
		const aiMesh* mesh = scene->mMeshes[0];
		m->numVertices = mesh->mNumVertices;
		m->numIndices = mesh->mNumFaces * 3;

		// VERTEX
		if(mesh->HasPositions()) {
			m->vertices = new glm::vec3[m->numVertices];
			for (GLuint i = 0 ; i < mesh->mNumVertices; i++) {
				const aiVector3D* pPos = &(mesh->mVertices[i]);
				m->vertices[i] = glm::vec3 (pPos->x, pPos->y, pPos->z);
			}
		}

		// INDICES
		if(mesh->HasFaces()) {
			m->indices = new unsigned int[m->numIndices];
			for (GLuint i = 0 ; i < mesh->mNumFaces; i ++) {
				const aiFace& face = mesh->mFaces[i];
				m->indices[3*i] = face.mIndices[0];
				m->indices[3*i+1] = face.mIndices[1];
				m->indices[3*i+2] = face.mIndices[2];
			}
		}

		// NORMALS
		if(mesh->HasNormals()) {
			m->normals = new glm::vec3[m->numVertices];
			for (GLuint i = 0 ; i < mesh->mNumVertices; i++) {
				const aiVector3D* pNorm = &(mesh->mNormals[i]);
				m->normals[i] = glm::vec3 (pNorm->x, pNorm->y, pNorm->z);
			}
		}

		// TEXTURE COORDINATES
		if(mesh->HasTextureCoords(0)) {
			m->textureCoords = new glm::vec2[m->numVertices];
			for (GLuint i = 0 ; i < mesh->mNumVertices; i++) {
				const aiVector3D* pTex = &(mesh->mTextureCoords[0][i]);
				m->textureCoords[i] = glm::vec2 (pTex->x, 1-pTex->y);
			}
		}
	}else {
		throw new std::string(importer.GetErrorString());
	}

	m->bufferData();
	m->calculateSize(m);
	return m;
}

void Mesh::calculateSize(Mesh* mesh) {
	GLfloat
		min_x, max_x,
		min_y, max_y,
		min_z, max_z;
		min_x = max_x = mesh->vertices[0].x;
		min_y = max_y = mesh->vertices[0].y;
		min_z = max_z = mesh->vertices[0].z;
	for (int i = 0; i < mesh->numVertices; i++) {
		if (mesh->vertices[i].x < min_x) min_x = mesh->vertices[i].x;
		if (mesh->vertices[i].x > max_x) max_x = mesh->vertices[i].x;
		if (mesh->vertices[i].y < min_y) min_y = mesh->vertices[i].y;
		if (mesh->vertices[i].y > max_y) max_y = mesh->vertices[i].y;
		if (mesh->vertices[i].z < min_z) min_z = mesh->vertices[i].z;
		if (mesh->vertices[i].z > max_z) max_z = mesh->vertices[i].z;
	}
	glm::vec3 size = glm::vec3(max_x-min_x, max_y-min_y, max_z-min_z);

	mesh->m_xSize = size.x;
	mesh->m_ySize = size.y;
	mesh->m_zSize = size.z;
	int x = 0;
}

//How you implement this function is up to you, but it
//should create some VBOs (and perhaps a VAO?), and upload
//some data into it, so that it can be used by the Draw function
void Mesh :: bufferData () {
	#define normalIndex 3

	glBindVertexArray(arrayObject);
	glGenBuffers(1, &bufferObject[VERTEX_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER , bufferObject[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER , numVertices * sizeof(glm::vec3),
		vertices ,GL_STATIC_DRAW );
	glVertexAttribPointer(VERTEX_BUFFER ,3, GL_FLOAT ,GL_FALSE ,0, 0);
	glEnableVertexAttribArray(VERTEX_BUFFER);

	if(textureCoords) { 
		glGenBuffers(1, & bufferObject[ TEXTURE_BUFFER ]);
		glBindBuffer(GL_ARRAY_BUFFER , bufferObject[ TEXTURE_BUFFER ]);
		glBufferData(GL_ARRAY_BUFFER , numVertices * sizeof(glm::vec2),
			textureCoords , GL_STATIC_DRAW);
		glVertexAttribPointer(TEXTURE_BUFFER , 2, GL_FLOAT , GL_FALSE ,0 ,0);
		glEnableVertexAttribArray(TEXTURE_BUFFER);
	}

	if(colours) {
		glGenBuffers(1, &bufferObject[COLOUR_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER , bufferObject[COLOUR_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER , numVertices * sizeof(glm::vec4),
			colours ,GL_STATIC_DRAW);
		glVertexAttribPointer(COLOUR_BUFFER ,4 ,GL_FLOAT ,GL_FALSE ,0 ,0);
		glEnableVertexAttribArray(COLOUR_BUFFER);
	}

	if(normals) {
		glGenBuffers(1, & bufferObject [NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER ,bufferObject[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER ,numVertices * sizeof(glm::vec3),
			normals, GL_STATIC_DRAW);
		glVertexAttribPointer(NORMAL_BUFFER ,3, GL_FLOAT ,GL_FALSE ,0 ,0);
		glEnableVertexAttribArray(NORMAL_BUFFER);
	}

	if(indices) {
		glGenBuffers(1, &bufferObject[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
			bufferObject[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER ,numIndices * sizeof(GLuint),
			indices, GL_STATIC_DRAW );
	}

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindVertexArray (0);
}

void Mesh::generateNormals () {
	if (!normals) {
		normals = new glm::vec3[numVertices];
	}
	for ( GLuint i = 0; i < numVertices ; ++i){
		normals[i] = glm::vec3();
	}
	if( indices ) { // Generate per - vertex normals
		for ( GLuint i = 0; i < numIndices ; i +=3){
			unsigned int a = indices [i];
			unsigned int b = indices [i +1];
			unsigned int c = indices [i +2];

			glm::vec3 normal = glm::cross(
				( vertices [b]- vertices [a]) ,( vertices [c]- vertices [a ]));

			normals [a] += normal ;
			normals [b] += normal ;
			normals [c] += normal ;
		}
	}
	else { // It 's just a list of triangles , so generate face normals
		for ( GLuint i = 0; i < numVertices ; i +=3){
			glm::vec3 &a = vertices [i];
			glm::vec3 &b = vertices [i +1];
			glm::vec3 &c = vertices [i +2];

			glm::vec3 normal = glm::cross(b-a,c-a);

			normals [i] = normal;
			normals [i+1] = normal;
			normals [i+2] = normal;
		}
	}
	for ( GLuint i = 0; i < numVertices; ++i){
		glm::normalize(normals[i]);
	}
}
