#include "Shader.h"
#include "Mesh.h"

Shader::Shader(const string &vertex,const string &fragment ,const string &geometry,const string &tcs,const string &tes)	{
	glewInit();
	program	= glCreateProgram();

	objects[SHADER_VERTEX] = generateShader(vertex ,GL_VERTEX_SHADER );
	objects[SHADER_FRAGMENT] = generateShader(fragment ,GL_FRAGMENT_SHADER);
	objects[SHADER_GEOMETRY] = 0;
	objects[SHADER_TCS] = 0;
	objects[SHADER_TES] = 0;

	if (!geometry.empty()) {
		objects[SHADER_GEOMETRY] = generateShader(geometry,
			GL_GEOMETRY_SHADER);
		glAttachShader(program ,objects[SHADER_GEOMETRY]);
	}

	if (!tcs.empty()) {
		objects[SHADER_TCS] = generateShader(tcs,
			GL_TESS_CONTROL_SHADER);
		glAttachShader(program ,objects[SHADER_TCS]);
	}

	if (!tes.empty()) {
		objects[SHADER_TES] = generateShader(tes,
			GL_TESS_EVALUATION_SHADER);
		glAttachShader(program ,objects[SHADER_TES]);
	}

	glAttachShader(program ,objects[SHADER_VERTEX]);
	glAttachShader(program ,objects[SHADER_FRAGMENT]);

	setDefaultAttributes();
}


Shader::~Shader(void) {
	for (int i = 0; i < 3; ++i) {
		glDetachShader(program, objects[i]);
		glDeleteShader(objects[i]);
	}
	glDeleteProgram(program);
}

void Shader::setDefaultAttributes () {
	glBindAttribLocation (program , VERTEX_BUFFER , "position");
	glBindAttribLocation (program , COLOUR_BUFFER , "colour");
	glBindAttribLocation (program , NORMAL_BUFFER , "normal"); 
	glBindAttribLocation (program , TEXTURE_BUFFER , "texCoord");
}

GLuint Shader::generateShader(string from, GLenum type) {
	cout << "Compiling Shader..." << endl ;

	string load;
	if (!loadShaderFile(from ,load)) {
		cout << "Compiling failed !" << endl ;
		loadFailed = true ;
		return 0;
	}
	GLuint shader = glCreateShader(type);

	const char * chars = load.c_str();
	glShaderSource (shader, 1, &chars, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv (shader ,GL_COMPILE_STATUS ,&status);

	if (status == GL_FALSE) {
		cout << "Compiling failed!" << endl;
		char error[512];
		glGetInfoLogARB (shader, sizeof(error), NULL ,error);
		cout << error;
		loadFailed = true;
		return 0;
	}
	cout << "Compiling success!" << endl << endl;
	loadFailed = false;
	return shader;
}

bool Shader::loadShaderFile(string from, string & into) {
	ifstream file;
	string temp;

	cout << "Loading shader text from " << from << endl << endl;

	file.open(from.c_str());
	if (!file.is_open ()) {
		cout << "File does not exist!" << endl;
		return false;
	}
	while (!file.eof()) {
		getline(file ,temp );
		into += temp + "\n";
	}

	file.close();
	cout << into << endl << endl;
	cout << "Loaded shader text!" << endl << endl;
	return true;
}

bool Shader::linkProgram() {
	if(loadFailed) {
		return false;
	}
	glLinkProgram(program);

	GLint code;
	glGetProgramiv(program ,GL_LINK_STATUS ,&code);
	return code == GL_TRUE ? true : false;
}