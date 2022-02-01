//
#include <loader.h>

loader::loader()
{
	isEOF = false;
	isAnnoying = false;
}

loader::~loader()
{
	isEOF = false;
	isAnnoying = false;
}

bool loader::loadObject(string filename, obj &inputObj)
{
	aiMesh *mesh;
	aiVector3D vert;
	aiVector3D texture;
	aiFace face;
	glm::vec3 tempVec;
	glm::vec3 tempColor;
	glm::vec2 tempTex;
	int indice;

	//load object from file
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(filename, \
			aiProcess_CalcTangentSpace      |  \
			aiProcess_GenNormals            |  \
			aiProcess_JoinIdenticalVertices |  \
			aiProcess_Triangulate           |  \
			aiProcess_SortByPType			|  \
			aiProcess_FlipUVs				|  \
			0
			);

	//if fail, throw error
	if (scene == NULL)
	{
		//read failed, return empty object
		cout << "FILE " << filename << " UNABLE TO BE READ" << endl;
		return false;
	}

	//otherwise, pack object data into the OBJ ADT and send to inputObj
	//get mesh (should be at index 0 since it's the only mesh)
	mesh = scene->mMeshes[0];

	obj final;
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		//get vertices
		vert = mesh->mVertices[i];
		tempVec.x = vert.x;
		tempVec.y = vert.y;
		tempVec.z = vert.z;

		//get color (get them from normals)
		vert = mesh->mNormals[i];
		tempColor.r = vert.x;
		tempColor.g = vert.y;
		tempColor.b = vert.z;

		//get texture coordinates
		if (mesh->HasTextureCoords(0))
		{
			texture = mesh->mTextureCoords[0][i];
			tempTex.x = texture.x;
			tempTex.y = texture.y;
			//if fails, default to 0,0 coords
 		}
		else
		{
			tempTex = glm::vec2(0,0);
		}

		//push into object
		final.addVert(Vertex(tempVec, tempColor, tempTex));
	}

	//load indices
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		face = mesh->mFaces[i];

		for (int c = 0; c < face.mNumIndices; c++)
		{
			final.addIndice(face.mIndices[c]+1);
		}
	}

	mesh = NULL;
	scene = NULL;

	inputObj = final;
	return true;
}

bool loader::loadShader(string filename, string& output) {
	isEOF = false;
	string final = "";
	char temp;
	filein.open(filename.c_str());

	if (filein.fail())
	{
		//read failed, return empty object
		cout << "FILE " << filename << " UNABLE TO BE READ" << endl;
		return false;
	}

	//load file
	while(!isEOF) {
		temp = filein.get();
		if (filein.eof())
		{
			isEOF = true;
			//throw away EOF bit
		} else
		{
			final += temp;
		}
	}

	//return file
	output = final;

	filein.close();
	return true;
}

bool loader::loadTexture(string filename, Texture &output)
{
	Magick::Blob blob;
	Magick::Image *tex;

	// load texture
	try
	{
		tex = new Magick::Image(filename);
		tex->write(&blob, "RGBA");
	}
	catch (Magick::Error& Error)
	{
		cout << "Error loading Texture " << filename << ": " << Error.what() << endl;
		return false;
	}

	//extract data
	output = Texture(tex->rows(), tex->columns(), blob.data());
	return true;
}


/*========
 * DEPRECATED
 *
 * All these functions below are deprecated
 * assimp now handles all the file loading
 * =======
 */

void loader::readLine(fstream &file, char &current, obj &object)
{
	switch (current)
	{
	case '#':
		readComment(file, current, object);
		break;
	case 'o':
		readObj(file, current, object);
		break;
	case 'v':
		if (file.peek() == ' ') {
			readVertex(file, current, object);
		}
		else if (file.peek() == 'n') {
			file.get();
			readUnknown(file, current, object);
		}
		break;
	case 's':
		readSection(file, current, object);
		break;
	case 'f':
		readIndice(file, current, object);
		break;
	default:
		readUnknown(file, current, object);
		break;
	}
}

void loader::readComment(fstream &file, char &current, obj &object)
{
	char read;
	//ignore everything
	while (true)
	{
		read = file.get();
		if (isAnnoying)
			cout << read;

		if (read == '\n' || read == '\r') {
			break;
		}

		if (read == -1) {
			isEOF = true;
			break;
		}
	}
	if (isAnnoying)
		cout << endl;
	current = file.get();
}

void loader::readObj(fstream &file, char &current, obj &object)
{
	string read;
	//assume it has no spaces
	//currently does nothing
	file >> read;
	if (isAnnoying)
		cout << read << endl;

	//next char
	file >> read;
	current = read.c_str()[0];
}

void loader::readVertex(fstream &file, char &current, obj &object)
{
	glm::vec3 vect;
	float reader;
	//read in three float values
	file >> vect.x;
	file >> vect.y;
	file >> vect.z;

	//build vertex and add
	object.addRaw(vect);
	if (isAnnoying)
		cout << vect.x << "," << vect.y << "," << vect.z << endl;

	//load next
	file >> current;
}

void loader::readSection(fstream &file, char &current, obj &object)
{
	string read;
	//assume it has no spaces
	//currently does nothing
	file >> read;
	if (isAnnoying)
		cout << read << endl;
	//next char
	file >> current;
}

void loader::readIndice(fstream &file, char &current, obj &object)
{
	string readin;
	unsigned int *indSet = NULL;
	for (int i = 0; i < 3; i++)
	{
		//read indice set
		file >> readin;
		indSet = getIndices(readin);

		//read in first number

		object.addIndice(indSet[0]);

		if (isAnnoying)
			cout << indSet[0] << ",";

		delete indSet;
		indSet = NULL;
	}
	if (isAnnoying)
		cout << endl;

	//next char
	if (current == -1)
		isEOF = true;
	else
		while (true)
		{
			current = file.get();
			if (current >= 'A' && current <= 'z')
				break;
			else if (current == -1)
			{
				isEOF = true;
				break;
			}
		}
}

unsigned int* loader::getIndices(string parse)
{
	unsigned int *final = new unsigned int[3];

	//extract first number
	string number = "";
	int stringIndex = 0;
	for (int i = 0; i < 3; i++) {
		//reset
		number = "";

		while (parse[stringIndex] >= '0' && parse[stringIndex] <= '9')
		{
			//get all chars that make up the number
			number += parse[stringIndex];
			stringIndex++;
		}
		//move to start of next number
		stringIndex++;

		//if there is no number, default to 0
		if (number.compare("") == 0)
			number = "0";

		//parse string to int
		final[i] = atoi(number.c_str());
	}

	return final;
}

void loader::readUnknown(fstream &file, char &current, obj &object)
{
	//skip line
	if (isAnnoying)
		cout << "[UNKNOWN]: ";
	while (true)
	{
		current = file.get();
		if (isAnnoying)
			cout << current;
		if (current == -1)
		{
			isEOF = true;
			break;
		}

		if (current == '\n')
		{
			current = file.get();
			break;
		}
	}

	if (isAnnoying)
		cout << endl;
}

void loader::calculateNormals(obj &object)
{
	//OBSOLETE
}

bool loader::isHeader(string test)
{
	//OBSOLETE
	return false;
}
